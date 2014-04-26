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
#include "umc_config.h"
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include <algorithm>

#include "umc_h264_thread.h"
#include "umc_h264_dec_defs_dec.h"

#include "umc_h264_segment_decoder_mt.h"

namespace UMC
{

H264Thread::H264Thread()
{
    m_isSleepNow = 0;
    m_bQuit = false;
    m_isDoneWait = 0;
} // H264Thread() :

H264Thread::~H264Thread()
{
    Release();
} // ~H264Thread(void)

H264SegmentDecoderMultiThreaded * H264Thread::GetSegmentDecoder() 
{
    return m_segmentDecoder;
}

Status H264Thread::Init(Ipp32s iNumber, H264SegmentDecoderMultiThreaded * segmentDecoder)
{
    // release object before initialization
    Release();

    // save thread number(s)
    m_iNumber = iNumber;

    m_segmentDecoder = segmentDecoder;

    // threading tools
    m_bQuit = false;
    m_isSleepNow = 0;
    m_isDoneWait = 0;
    m_sleepEvent.Init(0, 0);
    m_doneEvent.Init(0, 0);

    // start decoding thread
    if (UMC_OK != Create(DecodingThreadRoutine, this))
        return UMC_ERR_INIT;

    return UMC_OK;
} // Status Init(Ipp32u nNumber)

void H264Thread::Sleep()
{
    m_mGuard.Lock();
    m_isSleepNow = 1;
    m_mGuard.Unlock();

    m_sleepEvent.Wait();
}

void H264Thread::Awake()
{
    AutomaticMutex guard(m_mGuard);

    if (m_isSleepNow)
    {
        m_sleepEvent.Set();
        m_isSleepNow = 0;
    }
}

void H264Thread::Reset()
{
    // threading tools
    if (IsValid())
    {
        {
        AutomaticMutex guard(m_mGuard);
        if (m_isSleepNow)
            return;

        m_isDoneWait = 1;
        }

        m_doneEvent.Wait();
    }
} // void Reset(void)

void H264Thread::Release()
{
    Reset();

    if (IsValid())
    {
        m_bQuit = true;
        Awake();
        Wait();
    }

    Close();
} // void Release(void)

Ipp32u VM_THREAD_CALLCONVENTION H264Thread::DecodingThreadRoutine(void *p)
{
    H264Thread *pObj = (H264Thread *) p;
    H264SegmentDecoderMultiThreaded * segmentDecoder = pObj->GetSegmentDecoder();

    // check error(s)
    if (NULL == p)
        return 0x0baad;

    try
    {
        while (false == pObj->m_bQuit) // do segment decoding
        {
            try
            {
                pObj->m_Status = segmentDecoder->ProcessSegment();
            }
            catch(...) {}

            if (pObj->m_Status == UMC_ERR_NOT_ENOUGH_DATA)
            {
                pObj->m_mGuard.Lock();
                pObj->m_isSleepNow = 1;
                if (pObj->m_isDoneWait)
                {
                    pObj->m_doneEvent.Set();
                    pObj->m_isDoneWait = 0;
                }

                pObj->m_mGuard.Unlock();
                pObj->Sleep();
            }
        }
    }
    catch(...) {}

    return 0x0264dec0 + pObj->m_iNumber;

} // Ipp32u DecodingThreadRoutine(void *p)


H264ThreadGroup::H264ThreadGroup()
    : m_rejectAwake(false)
{
}

H264ThreadGroup::~H264ThreadGroup()
{
    Release();
}

/*void H264ThreadGroup::WaitThreads()
{
    AutomaticMutex guard(m_mGuard);

    for (Ipp32u i = 0; i < m_threads.size(); i++)
    {
        //m_threads[i]->WaitForEndOfProcessing();
    }
}*/

void H264ThreadGroup::Release()
{
    m_rejectAwake = true;

    for (Ipp32u i = 0; i < m_threads.size(); i++)
    {
        m_threads[i]->Release();
    }

    for (Ipp32u i = 0; i < m_threads.size(); i++)
    {
        delete m_threads[i];
    }

    m_threads.clear();
    m_rejectAwake = false;
}

void H264ThreadGroup::Reset()
{
    m_rejectAwake = true;

    for (Ipp32u i = 0; i < m_threads.size(); i++)
    {
        m_threads[i]->Reset();
    }

    m_rejectAwake = false;
}

void H264ThreadGroup::AddThread(H264Thread * thread)
{
    AutomaticMutex guard(m_mGuard);
    m_threads.push_back(thread);
}

void H264ThreadGroup::RemoveThread(H264Thread * thread)
{
    AutomaticMutex guard(m_mGuard);
    m_threads.erase(std::remove(m_threads.begin(), m_threads.end(), thread));
}

void H264ThreadGroup::AwakeThreads()
{
    if (m_rejectAwake)
        return;

    m_mGuard.Lock();
    for (Ipp32u i = 0; i < m_threads.size(); i++)
    {
        m_threads[i]->Awake();
    }
    m_mGuard.Unlock();
}

Ipp32u H264ThreadGroup::GetThreadNum() const
{
    return (Ipp32u)m_threads.size();
}

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
