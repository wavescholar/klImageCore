/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_THREAD_H
#define __UMC_H264_THREAD_H

#include <vector>

#include "umc_event.h"
#include "umc_thread.h"
#include "umc_mutex.h"

namespace UMC
{

class H264SegmentDecoderMultiThreaded;

class H264Thread : public Thread
{
public:
    H264Thread();
    virtual ~H264Thread();

    void Reset();

    Status Init(Ipp32s iNumber, H264SegmentDecoderMultiThreaded * segmentDecoder);

    H264SegmentDecoderMultiThreaded * GetSegmentDecoder();

    void Sleep();

    void Awake();

    void Release(void);

protected:

    Ipp32s m_iNumber;                                           // ordinal number of decoder
    Mutex m_mGuard;

    //
    // Threading routines
    //

    // Starting routine for decoding thread
    static Ipp32u VM_THREAD_CALLCONVENTION DecodingThreadRoutine(void *p);

    Event m_sleepEvent;
    Event m_doneEvent;

    volatile Ipp32s m_isSleepNow;
    volatile Ipp32s m_isDoneWait;
    volatile bool m_bQuit;                                     // quit flag for additional thread(s)
    Status m_Status;                                           // async return value

    H264SegmentDecoderMultiThreaded * m_segmentDecoder;

private:
    // we lock assignment operator to avoid any
    // accasionaly assignments
    H264Thread & operator = (H264Thread &)
    {
        return *this;
    } // H264SliceDecoder & operator = (H264SliceDecoder &)

};

class H264ThreadGroup 
{
public:

    H264ThreadGroup();

    virtual ~H264ThreadGroup();

    void AddThread(H264Thread * thread);

    void RemoveThread(H264Thread * thread);

    void AwakeThreads();

    //void WaitThreads();

    void Reset();

    void Release();

    Ipp32u GetThreadNum() const;

private:
    typedef std::vector<H264Thread *> ThreadsList;
    ThreadsList m_threads;
    Mutex m_mGuard;
    bool  m_rejectAwake;
};

} // namespace UMC

#endif // __UMC_H264_THREAD_H
