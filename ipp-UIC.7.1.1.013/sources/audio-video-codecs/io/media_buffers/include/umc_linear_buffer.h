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

#ifndef __UMC_LINEAR_BUFFER_H
#define __UMC_LINEAR_BUFFER_H

#include "umc_media_buffer.h"

namespace UMC
{

class SampleInfo
{
public:
    Ipp64f m_dTime;                                             // (Ipp64f) PTS of media sample
    Ipp64f m_dTimeAux;                                          // (Ipp64f) Additional time stamp, it can be interpreted as DTS of end time
    FrameType m_FrameType;                                      // (FrameType) Frame type
    size_t m_lDataSize;                                         // (size_t) media sample size
    size_t m_lBufferSize;                                       // (size_t) media sample buffer size
    Ipp8u *m_pbData;                                            // (Ipp8u *) pointer to data
    SampleInfo *m_pNext;                                        // (SampleInfo *) pointer to next media sample info
};

class LinearBuffer : public MediaBuffer
{
public:
    DYNAMIC_CAST_DECL(LinearBuffer, MediaBuffer)

    LinearBuffer(void);
    virtual ~LinearBuffer(void);

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

    // Stop buffer
    virtual Status Stop(void);

    // Release object
    virtual Status Close(void);

    // Reset object
    virtual Status Reset(void);

protected:
    Mutex m_synchro;                                         // (vm_mutex) synchro object

    Ipp8u *m_pbAllocatedBuffer;                                 // (Ipp8u *) pointer to allocated buffer
    MemID m_midAllocatedBuffer;                                 // (MemID) memory ID of allocated buffer
    size_t m_lAllocatedBufferSize;                              // (size_t) size of allocated buffer

    Ipp8u *m_pbBuffer;                                          // (Ipp8u *) pointer to allocated buffer
    size_t m_lBufferSize;                                       // (size_t) size of using buffer

    Ipp8u *m_pbFree;                                            // (Ipp8u *) pointer to free space
    size_t m_lFreeSize;                                         // (size_t) size of free space
    size_t m_lInputSize;                                        // (size_t) size of input data potion

    Ipp8u *m_pbUsed;                                            // (Ipp8u *) pointer to used space
    size_t m_lUsedSize;                                         // (size_t) size of used space
    size_t m_lDummySize;                                        // (size_t) size of dummy size at end of buffer
    size_t m_lOutputSize;                                       // (size_t) size of output data potion

    SampleInfo *m_pSamples;                                     // (SampleInfo *) queue of filled sample info
    SampleInfo *m_pFreeSampleInfo;                              // (SampleInfo *) queue of free sample info structures

    bool m_bEndOfStream;                                        // (bool) end of stream reached
    bool m_bQuit;                                               // (bool) end of stream reached and buffer is empty

    MediaBufferParams m_Params;                                 // (MediaBufferParams) parameters of current buffer

    SampleInfo m_Dummy;                                         // (SampleInfo) sample to handle data gaps
};

} // namespace UMC

#endif // __UMC_LINEAR_BUFFER_H
