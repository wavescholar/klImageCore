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
#ifdef UMC_ENABLE_GDI_VIDEO_RENDER

#include <windows.h>

#include "gdi_video_render.h"
#include "ipps.h"

using namespace UMC;

static Ipp16u GetNumBits(ColorFormat format)
{
    switch(format)
    {
    case BGR:  return 24;
    case BGRA: return 32;
    default:   return 0;
    }
    return 0;
}

ColorFormat GDIVideoRender::CheckColorFormat(ColorFormat format)
{
    switch(format)
    {
    case BGR:
    case BGRA:
        return format;
    default:
        return BGR;
    }
    return NONE;
}

Ipp32u GDIVideoRender::CheckBitDepth(Ipp32u)
{
    return 8;
}

GDIVideoRender::GDIVideoRender(void)
{
    m_bCriticalError = false;
    m_bStop          = false;
}

GDIVideoRender::~GDIVideoRender(void)
{
    Close();
}

Status GDIVideoRender::Init(MediaReceiverParams *pInit)
{
    GDIVideoRenderParams *params = DynamicCast<GDIVideoRenderParams, MediaReceiverParams>(pInit);
    Ipp16u iDCBPP;
    Status status;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_GDI,
        PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0
    };

    if(NULL == params)
        return UMC_ERR_NULL_PTR;

    Close();

    status = VideoRender::Init(pInit);
    if(UMC_OK != status)
        return status;

    m_params = *params;

    if(m_params.m_videoData.m_colorFormat == NONE)
        return UMC_ERR_NOT_INITIALIZED;

    if(m_params.m_iNumberOfBuffers == 0)
        m_params.m_iNumberOfBuffers = 1;
    if(m_params.m_iNumberOfBuffers > IPP_MAX_8U)
        m_params.m_iNumberOfBuffers = IPP_MAX_8U;

    m_wdc = GetDC(m_params.m_context.m_hWnd);
    if(NULL == m_wdc)
        return UMC_ERR_FAILED;

    if(!SetPixelFormat(m_wdc, ChoosePixelFormat(m_wdc, &pfd), &pfd))
        return UMC_ERR_FAILED;

    iDCBPP = GetNumBits(m_params.m_videoData.m_colorFormat);
    if(iDCBPP == 0)
        return UMC_ERR_NOT_INITIALIZED;

    ippsSet_8u(0, (Ipp8u*)&bitmapHeader, sizeof(BITMAPINFOHEADER));
    bitmapHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bitmapHeader.biWidth       = m_params.m_videoData.m_iWidth;
    bitmapHeader.biHeight      = -(Ipp32s)m_params.m_videoData.m_iHeight;
    bitmapHeader.biPlanes      = 1;
    bitmapHeader.biBitCount    = iDCBPP;
    bitmapHeader.biCompression = BI_RGB;

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

Status GDIVideoRender::Close(void)
{
    ReleaseDC(m_params.m_context.m_hWnd, m_wdc);

    m_BufferParam.m_numberOfFrames       = 0;
    m_BufferParam.m_prefInputBufferSize  = 0;
    m_BufferParam.m_prefOutputBufferSize = 0;
    m_Frames.Close();
    m_bCriticalError = false;
    m_bStop          = false;

    VideoRender::Close();

    return UMC_OK;
}

Status GDIVideoRender::LockInputBuffer(MediaData *pInData)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData>(pInData);

    if(m_bCriticalError || pSrc == NULL)
        return UMC_ERR_FAILED;

    *pSrc = m_params.m_videoData;
    return m_Frames.LockInputBuffer(pInData);
}

Status GDIVideoRender::UnLockInputBuffer(MediaData *pInData, Status streamStatus)
{
    VideoData *pSrc = DynamicCast<VideoData, MediaData>(pInData);

    if(m_bCriticalError || pSrc == NULL || pSrc->GetDataSize() == 0)
        return UMC_ERR_FAILED;

    return m_Frames.UnLockInputBuffer(pInData, streamStatus);
}

Status GDIVideoRender::GetRenderFrame(Ipp64f *pTime)
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

Status GDIVideoRender::ShowLastFrame(void)
{
    return UMC_OK;
}

Status GDIVideoRender::RenderFrame(void)
{
    MediaData srcData;
    Status    status = UMC_OK;
    Ipp32s    iLines;

    status = m_Frames.LockOutputBuffer(&srcData);
    if(UMC_OK != status)
        return status;

    iLines = StretchDIBits(
        m_wdc,                          // handle to DC
        0,                              // x-coord of destination upper-left corner
        0,                              // y-coord of destination upper-left corner
        m_params.m_videoData.m_iWidth,  // width of destination rectangle
        m_params.m_videoData.m_iHeight, // height of destination rectangle
        0,                              // x-coord of source upper-left corner
        0,                              // y-coord of source upper-left corner
        m_params.m_videoData.m_iWidth,  // width of source rectangle
        m_params.m_videoData.m_iHeight, // height of source rectangle
        srcData.GetDataPointer(),       // bitmap bits
        (BITMAPINFO*)&bitmapHeader,     // bitmap data
        DIB_RGB_COLORS,                 // usage options
        SRCCOPY                         // raster operation code
    );

    if(iLines != (Ipp32s)m_params.m_videoData.m_iHeight)
    {
        m_bCriticalError = true;
        return UMC_ERR_FAILED;
    }

    srcData.MoveDataPointer((Ipp32s)srcData.GetDataSize());
    return m_Frames.UnLockOutputBuffer(&srcData);
}

#endif // defined(UMC_ENABLE_GDI_VIDEO_RENDER)
