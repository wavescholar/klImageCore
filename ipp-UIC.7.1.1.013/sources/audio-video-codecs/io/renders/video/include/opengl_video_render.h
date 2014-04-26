/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __OPENGL_VIDEO_RENDER_H__
#define __OPENGL_VIDEO_RENDER_H__

#include "umc_config.h"
#include "umc_video_render.h"
#include "umc_sample_buffer.h"
#include "umc_filter_resize.h"
#include "umc_system_context.h"

#if defined WINDOWS
#include <GL/glu.h>
#define GL_RGB  0x1907
#define GL_RGBA 0x1908
#define GL_BGR  0x80E0
#define GL_BGRA 0x80E1
#else
#include <GL/glx.h>
#endif

namespace UMC
{

class OpenGLVideoRenderParams: public VideoRenderParams
{
public:
    DYNAMIC_CAST_DECL(OpenGLVideoRenderParams, VideoRenderParams)

    OpenGLVideoRenderParams()
    {
        m_iNumberOfBuffers = 8;
    };

    SysRenderContext m_context;
    Ipp32u           m_iNumberOfBuffers;
};

class OpenGLVideoRender: public VideoRender
{
public:
    DYNAMIC_CAST_DECL(OpenGLVideoRender, VideoRender)

    OpenGLVideoRender(void);
    virtual ~OpenGLVideoRender(void);

    // Initialize the render
    Status Init(MediaReceiverParams* pInit);

    // Terminate the render
    Status Close(void);

    // Lock input buffer
    Status LockInputBuffer(MediaData *in);

    // Unlock input buffer
    Status UnLockInputBuffer(MediaData *in, Status StreamStatus = UMC_OK);

    // Break waiting(s)
    Status Stop(void)
    {
        m_bStop = true;
        return UMC_OK;
    }

    // Reset media receiver
    Status Reset(void)
    {
        m_Frames.Reset();
        return UMC_OK;
    }

    // Peek presentation of next frame, return presentation time
    Status GetRenderFrame(Ipp64f *pTime);

    // Rendering the current frame
    Status RenderFrame(void);

    // Show the last rendered frame
    Status ShowLastFrame(void);

    // Do preparation before changing stream position
    Status PrepareForRePosition(void) {return UMC_OK;}

    ColorFormat CheckColorFormat(ColorFormat);
    Ipp32u      CheckBitDepth(Ipp32u iDepth);

    VideoRenderType GetRenderType(void) { return OPENGL_VIDEO_RENDER; }

protected:
    // OpenGL context must be created in rendering thread
    Status InitForCurrentThread(void);

    OpenGLVideoRenderParams m_params;      // renderer initialization parameters
    MediaBufferParams       m_BufferParam; // buffer initialization parameters
    SampleBuffer            m_Frames;      // buffer to store decoded frames

#if defined WINDOWS
    HDC         m_wdc;
    HGLRC       m_wglrc;
    HWND        m_hWnd;
#else
    GLXContext   m_glxcontext;
    Display*     m_display;
    Window       m_window;
    XVisualInfo* m_pvisinfo;
    Ipp32u       m_iScreen;
#endif

    IppiSize m_srcSize;
    IppiSize m_dstSize;
    IppiSize m_winSize;
    Ipp32u   m_iRenderFormat;
    bool     m_bCriticalError; // critical error occured
    bool     m_bStop;          // splitter was stopped
    bool     m_bInitialized;

    // specific buffers
    VideoData m_inData;
    VideoData m_outData;

    Ipp8u*    m_pTexture;    // bufer for the texture
    GLuint    m_textures[1];
    float     m_texWidth, m_texHeight;

    VideoResizeParams m_resParams;
    VideoResize       m_resizer;
};

};

#endif
