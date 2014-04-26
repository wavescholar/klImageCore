/*
 *
 *                  INTEL CORPORATION PROPRIETARY INFORMATION
 *     This software is supplied under the terms of a license agreement or
 *     nondisclosure agreement with Intel Corporation and may not be copied
 *     or disclosed except in accordance with the terms of that agreement.
 *       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
 *
 */

#ifndef __NULL_VIDEO_RENDERER_H__
#define __NULL_VIDEO_RENDERER_H__

#include "umc_video_render.h"
#include "umc_sample_buffer.h"

namespace UMC
{

class NULLVideoRenderParams: public VideoRenderParams
{
public:
    DYNAMIC_CAST_DECL(NULLVideoRenderParams, VideoRenderParams)

    NULLVideoRenderParams()
    {
        m_iNumberOfBuffers = 8;
    };

    Ipp32u m_iNumberOfBuffers;
};

class NULLVideoRender: public VideoRender
{
public:
    DYNAMIC_CAST_DECL(NULLVideoRender, VideoRender)

    NULLVideoRender(void);
    virtual ~NULLVideoRender(void);

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
    Status ShowLastFrame(void) {return UMC_OK;}

    // Do preparation before changing stream position
    Status PrepareForRePosition(void) {return UMC_OK;}

    VideoRenderType GetRenderType(void) { return NULL_VIDEO_RENDER; }

protected:
    NULLVideoRenderParams m_params;      // renderer initialization parameters
    MediaBufferParams     m_BufferParam; // buffer initialization parameters
    SampleBuffer          m_Frames;      // buffer to store decoded frames

    bool m_bStop;
};

} // namespace UMC

#endif // __NULL_VIDEO_RENDERER_H__
