/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __GDI_VIDEO_RENDER_H__
#define __GDI_VIDEO_RENDER_H__

#include "umc_video_render.h"
#include "umc_sample_buffer.h"
#include "umc_system_context.h"

namespace UMC
{

class GDIVideoRenderParams: public VideoRenderParams
{
public:
    DYNAMIC_CAST_DECL(GDIVideoRenderParams, VideoRenderParams)

    GDIVideoRenderParams()
    {
        m_iNumberOfBuffers = 8;
    }

    SysRenderContext m_context;
    Ipp32u           m_iNumberOfBuffers;
};

class GDIVideoRender: public VideoRender
{
public:
    DYNAMIC_CAST_DECL(GDIVideoRender, VideoRender)

    GDIVideoRender();
    virtual ~GDIVideoRender();

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

    VideoRenderType GetRenderType(void) { return GDI_VIDEO_RENDER; }

protected:
    GDIVideoRenderParams m_params;      // renderer initialization parameters
    MediaBufferParams    m_BufferParam; // buffer initialization parameters
    SampleBuffer         m_Frames;      // buffer to store decoded frames

    BITMAPINFOHEADER bitmapHeader;
    HDC  m_wdc;
    bool m_bCriticalError; // critical error occured
    bool m_bStop;          // splitter was stopped
};

} // namespace UMC

#endif // __GDI_VIDEO_RENDER_H__
