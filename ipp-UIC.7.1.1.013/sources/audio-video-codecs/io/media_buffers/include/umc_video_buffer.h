/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_VIDEO_BUFFER_H
#define __UMC_VIDEO_BUFFER_H

#include "umc_sample_buffer.h"

namespace UMC
{

class VideoBufferParams : public MediaBufferParams
{
public:
    DYNAMIC_CAST_DECL(VideoBufferParams, MediaBufferParams)

    VideoBufferParams(void)
    {
        m_lIPDistance = 0;
        m_lGOPSize    = 0;
    }

    Ipp32u m_lIPDistance;         // distance between I,P & B frames
    Ipp32u m_lGOPSize;            // size of GOP
};

class VideoBuffer : public SampleBuffer
{
public:
    DYNAMIC_CAST_DECL(VideoBuffer, SampleBuffer)

    VideoBuffer(void);
    virtual ~VideoBuffer(void);

    // Initialize buffer
    virtual Status Init(MediaReceiverParams* init);
    // Lock input buffer
    virtual Status LockInputBuffer(MediaData* in);
    // Unlock input buffer
    virtual Status UnLockInputBuffer(MediaData* in, Status StreamStatus = UMC_OK);
    // Lock output buffer
    virtual Status LockOutputBuffer(MediaData* out);
    // Unlock output buffer
    virtual Status UnLockOutputBuffer(MediaData* out);
    // Release object
    virtual Status Close(void);

protected:
    // Build help pattern
    bool BuildPattern(void);

    Ipp32u m_lFrameNumber;        // (Ipp32u) number of current frame
    Ipp32u m_lIPDistance;         // (Ipp32u) distance between I,P & P frame(s)
    Ipp32u m_lGOPSize;            // (Ipp32u) size of GOP
    FrameType *m_pEncPattern;     // (FrameType *) pointer to array of encoding pattern

    size_t m_nImageSize;        // (size_t) size of frame(s)
};

} // namespace UMC

#endif // __UMC_VIDEO_BUFFER_H
