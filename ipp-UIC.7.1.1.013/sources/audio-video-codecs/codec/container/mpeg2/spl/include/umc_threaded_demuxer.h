/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_THREADED_DEMUXER_H__
#define __UMC_THREADED_DEMUXER_H__

#define VM_THREAD_CATCHCRASH

#include "umc_thread.h"
#include "umc_event.h"
#include "umc_mutex.h"
#include "umc_media_data.h"
#include "umc_splitter.h"
#include "umc_demuxer.h"

namespace UMC
{

class Demuxer;
struct RulesMatchingState;
Ipp32u VM_THREAD_CALLCONVENTION ThreadRoutineStarter(void* u);

class ThreadedDemuxer : public Splitter
{
public:
    DYNAMIC_CAST_DECL(ThreadedDemuxer, Splitter)

    ThreadedDemuxer();
    virtual ~ThreadedDemuxer();
    virtual Status Init(SplitterParams *pParams);
    virtual Status Close(void);
    virtual Status SetTrackState(Ipp32u nTrack, TrackState iState);
    virtual Status Run(void);
    virtual Status Stop(void);

    // trick-modes
    virtual Status SetRate(Ipp64f rate);
    virtual Status SetTimePosition(Ipp64f timePos);
    virtual Status GetTimePosition(Ipp64f& timePos);
    virtual void AlterQuality(Ipp64f time);

    // getting data
    virtual Status GetNextData(MediaData *data, Ipp32u uiTrack);
    virtual Status CheckNextData(MediaData *data, Ipp32u uiTrack);

    // getting info
    virtual Status GetInfo(SplitterInfo** ppInfo);

protected:
    Status AnalyzeParams(SplitterParams *pParams);
    virtual Demuxer* CreateCoreDemux() const;
    void TerminateInit(void);
    bool TryTrackByRules(Ipp32u uiTrack);
    Ipp32u ThreadRoutine();
    friend Ipp32u VM_THREAD_CALLCONVENTION ThreadRoutineStarter(void* u);

    // pointer to the core demuxing object
    // this pointer is used to check if object initialized
    Demuxer *m_pDemuxer;
    // signaled when PSI is changed
    Event *m_pOnPSIChangeEvent;
    // internal thread
    Thread m_DemuxerThread;
    // signaled when init finished
    Event m_OnInit;
    // signaled when blocking buffer is unlocked
    Event m_OnUnlock;
    // PID of blocking buffer
    Ipp32s m_iBlockingPID;
    // saved initialization flags
    Ipp32u m_uiFlags;
    // stop flag
    bool m_bStop;
    // end of stream flag
    bool m_bEndOfStream;
    // indicates than new track is enabled
    bool m_bAutoEnable;
    // syncro for changing of flag m_bAutoEnable
    Mutex m_OnAutoEnable;
    // rules matching state, contains rules as well
    RulesMatchingState *m_pRulesState;
};

}

#endif
