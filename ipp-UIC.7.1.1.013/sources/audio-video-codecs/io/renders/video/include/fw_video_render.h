/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __FW_VIDEO_RENDER_H__
#define __FW_VIDEO_RENDER_H__

#include "umc_video_render.h"
#include "umc_sample_buffer.h"
#include "umc_file_writer.h"

namespace UMC
{

class FWVideoRenderParams : public VideoRenderParams
{
public:
    DYNAMIC_CAST_DECL(FWVideoRenderParams, VideoRenderParams)

    FWVideoRenderParams()
    {
        m_iNumberOfBuffers = 8;
    };

    DString   m_sFileName;
    Ipp32u    m_iNumberOfBuffers;
};

class FWVideoRender: public VideoRender
{
public:
    DYNAMIC_CAST_DECL(FWVideoRender, VideoRender)

    FWVideoRender(void);
    virtual ~FWVideoRender(void);

    // Initialize the render, return false if failed, call Close anyway
    virtual Status Init(MediaReceiverParams* pInit);

    // Terminate the render
    virtual Status Close(void);

    // Lock input buffer
    virtual Status LockInputBuffer(MediaData *in);

    // Unlock input buffer
    virtual Status UnLockInputBuffer(MediaData *in, Status StreamStatus = UMC_OK);

    // Break waiting(s)
    Status Stop(void)
    {
        m_bStop = true;
        return UMC_OK;
    };

    // Reset media receiver
    Status Reset(void)
    {
        m_Frames.Reset();
        return UMC_OK;
    };

    // Peek presentation of next frame, return presentation time
    virtual Status GetRenderFrame(Ipp64f *pTime);

    // Rendering the current frame
    virtual Status RenderFrame(void);

    // Show the last rendered frame
    virtual Status ShowLastFrame(void) {return UMC_OK;}

    // Do preparation before changing stream position
    virtual Status PrepareForRePosition(void) {return UMC_OK;}

    VideoRenderType GetRenderType(void) { return FW_VIDEO_RENDER; }

protected:
    FWVideoRenderParams m_params;      // renderer initialization parameters
    MediaBufferParams   m_BufferParam; // buffer initialization parameters
    SampleBuffer        m_Frames;      // buffer to store decoded frames

    FileWriter  m_fileWriter;     // file writer
    bool        m_bCriticalError; // critical error occured
    bool        m_bStop;          // splitter was stopped
};

} // namespace UMC

#endif // __FW_VIDEO_RENDER_H__
