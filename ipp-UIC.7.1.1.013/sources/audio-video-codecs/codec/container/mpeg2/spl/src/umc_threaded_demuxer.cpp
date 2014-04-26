/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_SPLITTER

#include "vm_time.h"
#include "vm_thread.h"
#include "umc_automatic_mutex.h"
#include "umc_threaded_demuxer.h"
#include "umc_demuxer.h"

#include "ippcore.h"

#define TDEM_CHECK_INIT CHECK_OBJ_INIT(m_pDemuxer)

using namespace UMC;


struct SplInitPreset
{
    Ipp32u uiRules;
    const TrackRule *pRules;
};

const TrackRule SplInitPreset_1A_0V[1] = { TrackRule(TRACK_ANY_AUDIO, -1,  1, false) };
const TrackRule SplInitPreset_0A_1V[1] = { TrackRule(TRACK_ANY_VIDEO, -1,  1, false) };
const TrackRule SplInitPreset_1A_1V[2] = { TrackRule(TRACK_ANY_AUDIO, -1,  1, false),
                                           TrackRule(TRACK_ANY_VIDEO, -1,  1, false) };
const TrackRule SplInitPreset_2A_1V[2] = { TrackRule(TRACK_ANY_AUDIO, -1,  2, false),
                                           TrackRule(TRACK_ANY_VIDEO, -1,  1, false) };
const TrackRule SplInitPreset_AA_1V[2] = { TrackRule(TRACK_ANY_AUDIO, -1, -1, false),
                                           TrackRule(TRACK_ANY_VIDEO, -1,  1, false) };
const TrackRule SplInitPreset_AA_AV[2] = { TrackRule(TRACK_ANY_AUDIO, -1, -1, false),
                                           TrackRule(TRACK_ANY_VIDEO, -1, -1, false) };

#define ADD_SPL_INIT_PRESET(NAME) \
  { sizeof(NAME) / sizeof (TrackRule), NAME }

const SplInitPreset SplInitPresets[] = {
    ADD_SPL_INIT_PRESET(SplInitPreset_1A_0V),
    ADD_SPL_INIT_PRESET(SplInitPreset_0A_1V),
    ADD_SPL_INIT_PRESET(SplInitPreset_1A_1V),
    ADD_SPL_INIT_PRESET(SplInitPreset_2A_1V),
    ADD_SPL_INIT_PRESET(SplInitPreset_AA_1V),
    ADD_SPL_INIT_PRESET(SplInitPreset_AA_AV)
};

namespace UMC
{
Ipp32u VM_THREAD_CALLCONVENTION ThreadRoutineStarter(void* u)
{
    return ((UMC::ThreadedDemuxer*)u)->ThreadRoutine();
}
};

ThreadedDemuxer::ThreadedDemuxer()
{
    m_pDemuxer = NULL;
}

ThreadedDemuxer::~ThreadedDemuxer()
{
    Close();
}

Status ThreadedDemuxer::Close(void)
{
    TDEM_CHECK_INIT;
    Stop();
    m_OnInit.Reset();
    m_bStop = true;
    m_bEndOfStream = true;

    if (m_pRulesState)
    {
        if (m_pRulesState->m_pRules)
        {
            delete[] m_pRulesState->m_pRules;
            m_pRulesState->m_pRules = NULL;
        }
        if (m_pRulesState->m_pRuleState)
        {
            ippFree(m_pRulesState->m_pRuleState);
            m_pRulesState->m_pRuleState = NULL;
        }
        delete m_pRulesState;
        m_pRulesState = NULL;
    }

    if (m_pDemuxer)
    {
        delete m_pDemuxer;
        m_pDemuxer = NULL;
    }

    return UMC_OK;
}

Status ThreadedDemuxer::Stop(void)
{
    TDEM_CHECK_INIT;
    m_bStop = true;
    m_pDemuxer->Stop();
    m_OnUnlock.Set();
    if (m_DemuxerThread.IsValid())
        m_DemuxerThread.Wait();
    return UMC_OK;
}

Status ThreadedDemuxer::Run(void)
{
    TDEM_CHECK_INIT;
    if (!m_bStop)
        return UMC_OK;

    m_bStop = false;
    m_OnInit.Reset();
    Status umcRes = m_DemuxerThread.Create((vm_thread_callback)ThreadRoutineStarter,(void *)this);
    if (UMC_OK != umcRes)
        return umcRes;

    m_OnInit.Wait();
    return UMC_OK;
}

Status ThreadedDemuxer::GetInfo(SplitterInfo** ppInfo)
{
    TDEM_CHECK_INIT;
    if (!ppInfo)
        return UMC_ERR_NULL_PTR;

    m_OnAutoEnable.Lock();
    Status umcRes = m_pDemuxer->GetInfo(ppInfo);
    if (UMC_OK != umcRes)
        return umcRes;

    ppInfo[0]->m_iFlags = m_uiFlags;
    if (m_bAutoEnable)
    {
        ppInfo[0]->m_iFlags |= FLAG_VSPL_NEW_TRACK;
        m_bAutoEnable = false;
    }

    Ipp32u i;
    for (i = 0; i < ppInfo[0]->m_iTracks; i++)
    {
        if (((Mpeg2TrackInfo *)ppInfo[0]->m_ppTrackInfo[i])->m_iFirstFrameOrder < 0)
            ppInfo[0]->m_ppTrackInfo[i]->m_bEnabled = false;
    }

    m_OnAutoEnable.Unlock();
    return UMC_OK;
}

Demuxer* ThreadedDemuxer::CreateCoreDemux() const
{
    return new Demuxer();
}

Status ThreadedDemuxer::Init(SplitterParams *pInit)
{
    if (m_pDemuxer) // already initialized
        return UMC_ERR_FAILED;

    if(!pInit)
        return UMC_ERR_NULL_PTR;

    m_pRulesState = new RulesMatchingState;
    if (!m_pRulesState)
    {
        TerminateInit();
        return UMC_ERR_ALLOC;
    }

    m_uiFlags = pInit->m_iFlags;
    Status umcRes = AnalyzeParams(pInit);
    if (UMC_OK != umcRes)
    {
        TerminateInit();
        return umcRes;
    }

    m_pOnPSIChangeEvent = NULL;
    if (DynamicCast<DemuxerParams, SplitterParams>(pInit))
        m_pOnPSIChangeEvent = ((DemuxerParams*)pInit)->m_pOnPSIChangeEvent;

    m_pDemuxer = CreateCoreDemux();
    if (NULL == m_pDemuxer)
    {
        TerminateInit();
        return UMC_ERR_ALLOC;
    }

    umcRes = m_pDemuxer->Init(pInit);
    if (UMC_OK != umcRes)
    {
        TerminateInit();
        return umcRes;
    }

    m_bStop = false;
    m_bEndOfStream = false;
    m_bAutoEnable = false;
    m_pRulesState->Reset();
    m_iBlockingPID = -1;

    umcRes = m_OnUnlock.Init(1, 0);
    if (UMC_OK != umcRes)
    {
        TerminateInit();
        return umcRes;
    }

    umcRes = m_OnInit.Init(1, 0);
    if (UMC_OK != umcRes)
    {
        TerminateInit();
        return umcRes;
    }

    umcRes = m_DemuxerThread.Create((vm_thread_callback)ThreadRoutineStarter, (void *)this);
    if (UMC_OK != umcRes)
    {
        TerminateInit();
        return umcRes;
    }

    m_OnInit.Wait();
    return UMC_OK;
}

Status ThreadedDemuxer::CheckNextData(MediaData *data, Ipp32u uiTrack)
{
    TDEM_CHECK_INIT;
    return m_pDemuxer->CheckNextData(data, uiTrack);
}

Status ThreadedDemuxer::GetNextData(MediaData *data, Ipp32u uiTrack)
{
    TDEM_CHECK_INIT;
    if ((Ipp32s)uiTrack == m_iBlockingPID || -1 == m_iBlockingPID)
        m_OnUnlock.Set();

    return m_pDemuxer->GetNextData(data, uiTrack);
}

Status ThreadedDemuxer::SetRate(Ipp64f rate)
{
    TDEM_CHECK_INIT;
    Stop();
    m_bEndOfStream = false;
    m_pDemuxer->SetRate(rate);

    return UMC_OK;
}

Status ThreadedDemuxer::SetTrackState(Ipp32u nTrack, TrackState state)
{
    TDEM_CHECK_INIT;
    Status umcRes = m_pDemuxer->SetTrackState(nTrack, state);
    if (UMC_OK != umcRes)
        return umcRes;

    // if manual track management cancel auto-enabling
    m_pRulesState->m_bAnyRuleValidAfterInit = false;
    return UMC_OK;
}

Status ThreadedDemuxer::SetTimePosition(Ipp64f timePos)
{
    TDEM_CHECK_INIT;
    Stop();
    m_bEndOfStream = false;
    Status umcRes = m_pDemuxer->SetTimePosition(timePos);
    if (UMC_OK != umcRes)
        return umcRes;

    return umcRes;
}

Status ThreadedDemuxer::GetTimePosition(Ipp64f &timePos)
{
    TDEM_CHECK_INIT;
    return m_pDemuxer->GetTimePosition(timePos);
}

void ThreadedDemuxer::TerminateInit(void)
{
    if (m_pRulesState)
    {
        if (m_pRulesState->m_pRules)
        {
            delete[] m_pRulesState->m_pRules;
            m_pRulesState->m_pRules = NULL;
        }
        if (m_pRulesState->m_pRuleState)
        {
            ippFree(m_pRulesState->m_pRuleState);
            m_pRulesState->m_pRuleState = NULL;
        }
        delete m_pRulesState;
        m_pRulesState = NULL;
    }

    if (m_pDemuxer)
    {
        delete m_pDemuxer;
        m_pDemuxer = NULL;
    }
}

Status ThreadedDemuxer::AnalyzeParams(SplitterParams *pParams)
{
    Ipp32u i;
    const TrackRule *pRules = NULL;
    if (DynamicCast<DemuxerParams, SplitterParams>(pParams))
    {
        pRules = ((DemuxerParams *)pParams)->m_pRules;
        m_pRulesState->m_uiRules = ((DemuxerParams *)pParams)->m_uiRules;
    }

    // presets have higher priority than manually defined rules
    if (pParams->m_iFlags & FLAG_VSPL_PRESET_MASK)
    {
        Ipp32s iPreset = (pParams->m_iFlags & FLAG_VSPL_PRESET_MASK) - 1;
        if ((size_t)iPreset < sizeof(SplInitPresets) / sizeof(SplInitPresets[0]))
        {
            pRules = SplInitPresets[iPreset].pRules;
            m_pRulesState->m_uiRules = SplInitPresets[iPreset].uiRules;
        }
    }

    if (0 == m_pRulesState->m_uiRules || NULL == pRules)
        return UMC_ERR_INIT;

    m_pRulesState->m_pRules = new TrackRule[m_pRulesState->m_uiRules];
    m_pRulesState->m_pRuleState = (Ipp32s*)ippMalloc(m_pRulesState->m_uiRules*sizeof(Ipp32s));
    if (!m_pRulesState->m_pRules || !m_pRulesState->m_pRuleState)
        return UMC_ERR_ALLOC;

    ippsSet_32s(0, m_pRulesState->m_pRuleState, m_pRulesState->m_uiRules);
    ippsCopy_8u((Ipp8u *)pRules, (Ipp8u *)m_pRulesState->m_pRules, m_pRulesState->m_uiRules * sizeof(TrackRule));

    // analyse rules
    for (i = 0; i < m_pRulesState->m_uiRules; i++)
    {
        // when FLAG_VSPL_FAST_INIT is set, whole playback is like init stage
        if (m_uiFlags & FLAG_VSPL_FAST_INIT)
            m_pRulesState->m_pRules[i].m_bValidAfterInit = true;
        if (m_pRulesState->m_pRules[i].m_bValidAfterInit)
            m_pRulesState->m_bAnyRuleValidAfterInit = true;

        // if positive PID is specified it implies that iMaxNOfESs isn't greater than 1
        if (m_pRulesState->m_pRules[i].m_iPID >= 0 && m_pRulesState->m_pRules[i].m_iLimit > 1)
            m_pRulesState->m_pRules[i].m_iLimit = 1;

        // count total number of tracks defined by rules
        if (m_pRulesState->m_pRules[i].m_iLimit < 0)
            m_pRulesState->m_iMaxTracks = MAX_TRACK;
        else if (m_pRulesState->m_iMaxTracks < MAX_TRACK)
            m_pRulesState->m_iMaxTracks += m_pRulesState->m_pRules[i].m_iLimit;
    }

    return UMC_OK;
}

Ipp32u ThreadedDemuxer::ThreadRoutine()
{
    Status umcRes = UMC_OK;
    MediaData data;
    Ipp32u uiTrack = 0;

    // initialization
    while (!m_bStop && !(m_uiFlags & FLAG_VSPL_FAST_INIT))
    {
        umcRes = m_pDemuxer->CheckNextData(&data, &uiTrack);
        if (UMC_OK == umcRes)
        { // next access unit is received, check if it's from new track
            if (!m_pRulesState->m_bIsTriedByRules[uiTrack])
            {
                if (TryTrackByRules(uiTrack))
                { // match some rule, leave it enabled
                    m_OnAutoEnable.Lock();
                    m_bAutoEnable = true;
                    m_OnAutoEnable.Unlock();
                }
                else
                { // not match to any rule, disable it
                    m_pDemuxer->SetTrackState(uiTrack, TRACK_DISABLED);
                }
            }

            if (m_pRulesState->m_iMatchedTracks >= m_pRulesState->m_iMaxTracks)
            {
                m_bStop = (m_uiFlags & FLAG_VSPL_STOP_AFTER_INIT) != 0;
                break;
            }
        }
        else if (UMC_WRN_INVALID_STREAM == umcRes)
        { // change of PAT is detected, notify app and prepare for re-init
            if (m_pOnPSIChangeEvent)
                m_pOnPSIChangeEvent->Set();
            m_OnInit.Reset();
            m_pRulesState->Reset();
            return 0;
        }
        else if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes || UMC_ERR_END_OF_STREAM == umcRes ||
            m_pRulesState->m_iMatchedTracks >= m_pRulesState->m_iMaxTracks)
        { // initialization stage completes (any buffer is full, EOS or all rules are matched)
            m_bStop = (m_uiFlags & FLAG_VSPL_STOP_AFTER_INIT) != 0;
            break;
        }
        else
        {
            m_OnInit.Set();
            return 1;
        }
    }

    // notify main thread that initialization stage is passed
    m_OnInit.Set();

    // main loop
    while (!m_bStop)
    {
        umcRes = m_pDemuxer->CheckNextData(&data, &uiTrack);
        if (UMC_OK == umcRes)
        { // next access unit is received
            if (!m_pRulesState->m_bIsTriedByRules[uiTrack])
            {
                if (TryTrackByRules(uiTrack))
                { // matches some rule, leave it enabled
                    m_OnAutoEnable.Lock();
                    m_bAutoEnable = true;
                    m_OnAutoEnable.Unlock();
                }
                else
                { // doesn't match to any rule, disable it
                    m_pDemuxer->SetTrackState(uiTrack, TRACK_DISABLED);
                }
            }
        }
        else if (UMC_WRN_INVALID_STREAM == umcRes)
        { // change of PAT is detected, notify app and prepare for re-init
            if (m_pOnPSIChangeEvent)
                m_pOnPSIChangeEvent->Set();
            m_OnInit.Reset();
            m_pRulesState->Reset();
            return 0;
        }
        else if (UMC_ERR_NOT_ENOUGH_BUFFER == umcRes)
        { // buffer is full, it blocks parsing thread, what until app get data from fulled buffer
            m_iBlockingPID = uiTrack;
            m_OnUnlock.Wait();
            m_iBlockingPID = -1;
            m_OnUnlock.Reset();
        }
        else
            break;
    }

    m_bStop = true;
    m_bEndOfStream = (UMC_ERR_END_OF_STREAM == umcRes);
    return 0;
}

void ThreadedDemuxer::AlterQuality(Ipp64f time)
{
    if (m_pDemuxer)
        m_pDemuxer->AlterQuality(time);
}

bool ThreadedDemuxer::TryTrackByRules(Ipp32u uiTrack)
{
    // if all possible tracks enabled
    if (m_pRulesState->m_iMatchedTracks >= m_pRulesState->m_iMaxTracks)
        return false;

    // if init finished and enabling after init isn't required
    if (UMC_OK == m_OnInit.Wait(0) && !m_pRulesState->m_bAnyRuleValidAfterInit)
        return false;

    SplitterInfo *pSplInfo;
    Status umcRes = m_pDemuxer->GetInfo(&pSplInfo);
    if (UMC_OK != umcRes)
        return false;

    TrackInfo *pInfo = pSplInfo->m_ppTrackInfo[uiTrack];
    m_pRulesState->m_bIsTriedByRules[uiTrack] = true;

    Ipp32u rule;
    for (rule = 0; rule < m_pRulesState->m_uiRules; rule++)
    {
        // check if this rule valid at current state of threaded demuxer
        if (UMC_OK == m_OnInit.Wait(0) && !m_pRulesState->m_pRules[rule].m_bValidAfterInit)
            continue;

        // check if required number of tracks of this type was already found
        if (m_pRulesState->m_pRules[rule].m_iLimit >= 0 && m_pRulesState->m_pRuleState[rule] >= m_pRulesState->m_pRules[rule].m_iLimit)
            continue;

        // check if track type fits type mask of rule
        if (!(pInfo->m_type & m_pRulesState->m_pRules[rule].m_TypeMask))
            continue;

        // check if track PID fits required PID (if it's positive)
        if (m_pRulesState->m_pRules[rule].m_iPID >= 0 && (Ipp32s)pInfo->m_iPID != m_pRulesState->m_pRules[rule].m_iPID)
            continue;

        m_pRulesState->m_pRuleState[rule]++;
        m_pRulesState->m_iMatchedTracks++;
        return true;
    }

    return false;
}

#endif
