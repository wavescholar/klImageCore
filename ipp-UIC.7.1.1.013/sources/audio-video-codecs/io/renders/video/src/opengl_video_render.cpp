/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_OPENGL_VIDEO_RENDER

#include "opengl_video_render.h"
#include "ippcore.h"

#ifdef WINDOWS
#pragma warning(disable: 4127)
#endif

using namespace UMC;


static Ipp32u UMCToInternalFormat(ColorFormat format)
{
    switch(format)
    {
    case BGR:  return GL_BGR;
    case BGRA: return GL_BGRA;
    case RGB:  return GL_RGB;
    case RGBA: return GL_RGBA;
    default:   return 0;
    }
    return 0;
}

ColorFormat OpenGLVideoRender::CheckColorFormat(ColorFormat format)
{
    switch(format)
    {
#ifdef WINDOWS
    case BGR:
    case BGRA:
#else
    case RGBA:
#endif
        return format;
    default:
#ifdef WINDOWS
        return BGR;
#else
        return RGBA;
#endif
    }
    return NONE;
}

Ipp32u OpenGLVideoRender::CheckBitDepth(Ipp32u)
{
    return 8;
}

OpenGLVideoRender::OpenGLVideoRender(void)
{
    m_pTexture       = NULL;
    m_bCriticalError = false;
    m_bStop          = false;
    m_bInitialized   = false;
    m_iRenderFormat  = 0;
    m_dstSize.height = m_dstSize.width = 0;
    m_srcSize.height = m_srcSize.width = 0;

#if defined WINDOWS
    m_wglrc      = 0;
#else
    m_glxcontext = 0;
#endif
}

OpenGLVideoRender::~OpenGLVideoRender(void)
{
    Close();
}

Status OpenGLVideoRender::Init(MediaReceiverParams* pInit)
{
    OpenGLVideoRenderParams *params = DynamicCast<OpenGLVideoRenderParams, MediaReceiverParams>(pInit);
    Status status;

    if(NULL == params)
        return UMC_ERR_NULL_PTR;

    Close();

    status = VideoRender::Init(pInit);
    if(UMC_OK != status)
        return status;

    m_params = *params;

    if(m_params.m_videoData.m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    m_iRenderFormat = UMCToInternalFormat(m_params.m_videoData.m_colorFormat);
    if(m_iRenderFormat == 0)
        return UMC_ERR_NOT_INITIALIZED;

    if(m_params.m_iNumberOfBuffers == 0)
        m_params.m_iNumberOfBuffers = 1;
    if(m_params.m_iNumberOfBuffers > IPP_MAX_8U)
        m_params.m_iNumberOfBuffers = IPP_MAX_8U;

    m_inData = m_params.m_videoData;

    m_srcSize.width  = m_inData.m_iWidth;
    m_srcSize.height = m_inData.m_iHeight;
#if defined WINDOWS
    m_hWnd           = m_params.m_context.m_hWnd;
#else
    m_display        = m_params.m_context.m_pDisplay;
    m_window         = m_params.m_context.m_window;
    m_iScreen        = m_params.m_context.m_iScreen;
    m_pvisinfo       = m_params.m_context.m_pVisualInfo;
#endif

    m_resParams.m_interpolationType = IPPI_INTER_LINEAR;
    status = m_resizer.Init((BaseCodecParams*)&m_resParams);
    if(UMC_OK != status)
        return status;

    if(0 == m_BufferParam.m_prefInputBufferSize)
    {
        // set buffer parameters
        m_BufferParam.m_numberOfFrames = m_params.m_iNumberOfBuffers;
        m_BufferParam.m_prefInputBufferSize = m_BufferParam.m_prefOutputBufferSize = m_params.m_videoData.GetMappingSize();

        status = m_Frames.Init(&m_BufferParam);
        if(UMC_OK != status)
            return status;
    }

    return UMC_OK;
}

Status OpenGLVideoRender::Close(void)
{
#if defined WINDOWS
    if(m_wglrc)
    {
        wglDeleteContext(m_wglrc);
        m_wglrc = 0;
    }
    ReleaseDC(m_hWnd, m_wdc);
#else
    if(m_glxcontext)
    {
        glXDestroyContext(m_display, m_glxcontext);
        m_glxcontext = 0;
    }
#endif

    UMC_FREE(m_pTexture);

    m_BufferParam.m_numberOfFrames       = 0;
    m_BufferParam.m_prefInputBufferSize  = 0;
    m_BufferParam.m_prefOutputBufferSize = 0;
    m_Frames.Close();
    m_bCriticalError = false;
    m_bStop          = false;

    VideoRender::Close();

    return UMC_OK;
}

Status OpenGLVideoRender::InitForCurrentThread(void)
{
#if defined WINDOWS
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    m_wdc = GetDC(m_hWnd);
    if(NULL == m_wdc)
        return UMC_ERR_FAILED;

    if(!SetPixelFormat(m_wdc, ChoosePixelFormat(m_wdc, &pfd), &pfd))
        return UMC_ERR_FAILED;

    m_wglrc = wglCreateContext(m_wdc); // create rendering context
    if(NULL == m_wglrc)
        return UMC_ERR_FAILED;

    if(!wglMakeCurrent(m_wdc, m_wglrc)) // set it as current
        return UMC_ERR_FAILED;
#else
    if (NULL == (m_glxcontext = glXCreateContext( m_display, m_pvisinfo, NULL, true )))
        return UMC_ERR_FAILED;
    if (!glXMakeCurrent(m_display, m_window, m_glxcontext))
        return UMC_ERR_FAILED;
#endif

    // OpenGL context already tied to output window
    // to disable all slow GL components
    // it is not mandatory to disable all if we have accelerated card
    glClearColor(.0f, .0f, .0f, .0f);
    glClearDepth(1.0);
    glDepthFunc(GL_NEVER);

    // disable slow GL extensions
    glDisable(GL_DEPTH_TEST); glDisable(GL_ALPHA_TEST);   glDisable(GL_BLEND);
    glDisable(GL_DITHER);     glDisable(GL_FOG);          glDisable(GL_STENCIL_TEST);
    glDisable(GL_LIGHTING);   glDisable(GL_LOGIC_OP);     glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);

    glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
    glPixelTransferi(GL_RED_SCALE,   1);  glPixelTransferi(GL_RED_BIAS,   0);
    glPixelTransferi(GL_GREEN_SCALE, 1);  glPixelTransferi(GL_GREEN_BIAS, 0);
    glPixelTransferi(GL_BLUE_SCALE,  1);  glPixelTransferi(GL_BLUE_BIAS,  0);
    glPixelTransferi(GL_ALPHA_SCALE, 1);  glPixelTransferi(GL_ALPHA_BIAS, 0);

    if(m_params.m_iFlags & FLAG_VREN_USETEXTURES)
    {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, m_textures);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    }

    glViewport(0, 0, m_dstSize.width, m_dstSize.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRasterPos2i(-1, 1);       // move to the upper left corner
    glPixelZoom(1.0, -1.0);     // top to bottom

#if defined WINDOWS
    SwapBuffers(m_wdc);
#else
    glXSwapBuffers(m_display, m_window);
#endif

    m_bInitialized = true;

    return UMC_OK;
}

Status OpenGLVideoRender::LockInputBuffer(MediaData *pInData)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData>(pInData);

    if(m_bCriticalError || pSrc == NULL)
        return UMC_ERR_FAILED;

    *pSrc = m_params.m_videoData;
    return m_Frames.LockInputBuffer(pInData);
}

Status OpenGLVideoRender::UnLockInputBuffer(MediaData *pInData, Status streamStatus)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData>(pInData);

    if(m_bCriticalError || pSrc == NULL || pSrc->GetDataSize() == 0)
        return UMC_ERR_FAILED;

    return m_Frames.UnLockInputBuffer(pInData, streamStatus);
}

Status OpenGLVideoRender::GetRenderFrame(Ipp64f *pTime)
{
    MediaData mediaData;
    Status    status = UMC_OK;

    if(m_bCriticalError || pTime == NULL)
        return UMC_ERR_FAILED;

    *pTime = -1;

    // get latest frame time stamp
    status = m_Frames.LockOutputBuffer(&mediaData);
    if(status == UMC_ERR_NOT_ENOUGH_DATA)
        return m_bStop ? UMC_ERR_END_OF_STREAM : UMC_ERR_TIMEOUT;
    if(status != UMC_OK)
        return UMC_ERR_FAILED;

    *pTime = mediaData.m_fPTSStart;
    return m_Frames.UnLockOutputBuffer(&mediaData);
}

Status OpenGLVideoRender::ShowLastFrame(void)
{
#if defined WINDOWS
    SwapBuffers(m_wdc);
#else
    glXSwapBuffers(m_display, m_window);
#endif
    return UMC_OK;
}

Status OpenGLVideoRender::RenderFrame(void)
{
    MediaData  srcData;
    VideoData *pOutData = &m_outData;
    IppiSize   curWinSize;
    Status     status = UMC_OK;

#if defined WINDOWS
    ::RECT rect;
    GetClientRect(m_hWnd, &rect);
    curWinSize.height = rect.bottom;
    curWinSize.width  = rect.right;
#else
    XWindowAttributes wattc = {0};
    XGetWindowAttributes(m_display, m_window, &wattc);
    curWinSize.width = wattc.width;
    curWinSize.height = wattc.height;
#endif

    if (!m_bInitialized)
    {
        status = InitForCurrentThread();
        if(UMC_OK != status)
            m_bCriticalError = true;
    }

    if(curWinSize.width > IPP_MAX_16S || curWinSize.height > IPP_MAX_16S) // window seems to be destroyed
        return status;

    // obtain buffer to draw
    status = m_Frames.LockOutputBuffer(&srcData);
    if(UMC_OK != status)
        return status;

    m_inData.SetBufferPointer((Ipp8u*)srcData.GetBufferPointer(), srcData.GetDataSize());

    // reinit buffers if window size has been changed
    if(curWinSize.height != m_winSize.height || curWinSize.width != m_winSize.width)
    {
        if(m_params.m_iFlags & FLAG_VREN_KEEPASPECT)
        {
            Ipp32f scale = IPP_MIN((Ipp32f)curWinSize.width/m_srcSize.width, (Ipp32f)curWinSize.height/m_srcSize.height);
            m_dstSize.width  = (Ipp32u)(scale*m_srcSize.width);
            m_dstSize.height = (Ipp32u)(scale*m_srcSize.height);

            m_winSize.width  = curWinSize.width;
            m_winSize.height = curWinSize.height;
        }
        else
        {
            m_winSize.width  = m_dstSize.width  = curWinSize.width;
            m_winSize.height = m_dstSize.height = curWinSize.height;
        }

        glViewport((m_winSize.width - m_dstSize.width)/2, (m_winSize.height - m_dstSize.height)/2, m_dstSize.width, m_dstSize.height);

        pOutData = &m_outData;
        pOutData->Close();
        if(m_srcSize.width != m_dstSize.width || m_srcSize.height != m_dstSize.height)
        {
            pOutData->Init(m_dstSize.width, m_dstSize.height, m_params.m_videoData.m_colorFormat);
            pOutData->Alloc();
        }

        if(m_params.m_iFlags & FLAG_VREN_USETEXTURES)
        {
            IppiSize textureSize = {1,1};
            if(m_pTexture != NULL)
                ippFree(m_pTexture);

            do
            {
                if (textureSize.width < m_dstSize.width)
                    textureSize.width <<= 1;
                if (textureSize.height < m_dstSize.height)
                    textureSize.height <<= 1;
            } while((textureSize.width < m_dstSize.width) || (textureSize.height < m_dstSize.height));
            // create pixel buffer to hold the whole texture
            // m_texWidth  --- horizontal part of the whole texture to display in output rectangle
            // m_texHeight --- vertical part of the whole texture to display in output rectangle
            m_texWidth = (float)((double)m_dstSize.width/(double)textureSize.width);
            m_texHeight = (float)((double)m_dstSize.height/(double)textureSize.height);
            m_pTexture = (Ipp8u*)ippMalloc((int)(textureSize.width*textureSize.height*4));
            if(NULL == m_pTexture)
                status = UMC_ERR_ALLOC;

            glTexImage2D(GL_TEXTURE_2D, 0, 3, textureSize.width, textureSize.height, 0, m_iRenderFormat, GL_UNSIGNED_BYTE, m_pTexture);
        }
        else
            glRasterPos2i(-1, 1);
    }

    // resize to output pixel buffer
    if(m_dstSize.width != 0 && m_dstSize.height != 0)
    {
        if(m_srcSize.width == m_dstSize.width && m_srcSize.height == m_dstSize.height)
            pOutData = &m_inData;
        else
        {
            status = m_resizer.GetFrame(&m_inData, pOutData);
            if(UMC_OK != status)
                return status;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(m_params.m_iFlags & FLAG_VREN_USETEXTURES)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_dstSize.width, m_dstSize.height, m_iRenderFormat, GL_UNSIGNED_BYTE, pOutData->GetBufferPointer());

            glBegin(GL_POLYGON);
            glTexCoord2f(0.,          0.);          glVertex2f(-1.0,  1.0);
            glTexCoord2f(m_texWidth,  0.);          glVertex2f( 1.0,  1.0);
            glTexCoord2f(m_texWidth,  m_texHeight); glVertex2f( 1.0, -1.0);
            glTexCoord2f(0.,          m_texHeight); glVertex2f(-1.0, -1.0);
            glEnd();
        }
        else
            glDrawPixels(m_dstSize.width, m_dstSize.height,  m_iRenderFormat, GL_UNSIGNED_BYTE, pOutData->GetBufferPointer());

        glFlush();
#if defined WINDOWS
        if(!SwapBuffers(m_wdc))   // to draw on physical screen
            status = UMC_ERR_FAILED;
#else
        glXSwapBuffers(m_display, m_window);
#endif
    }

    // skip processed data
    srcData.MoveDataPointer((Ipp32s)srcData.GetDataSize());
    return m_Frames.UnLockOutputBuffer(&srcData);
}

#endif
