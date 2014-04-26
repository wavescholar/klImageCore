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

#include "umc_default_memory_allocator.h"
#include "umc_demuxer.h"
#include "umc_mpeg2ps_parser.h"
#include "umc_mpeg2ts_parser.h"

//#define DUMP_SOMETHING
//#define DUMP_VIDEO_1X_TXT
//#define DUMP_VIDEO_FF_TXT
//#define DUMP_VIDEO_BF_TXT
//#define DUMP_AUDIO_TXT
//#define DUMP_VIDEO_1X_BIN
//#define DUMP_VIDEO_FF_BIN
//#define DUMP_VIDEO_BF_BIN
//#define DUMP_AUDIO_BIN

static const Ipp32s VIDEO_BUFFER_SIZE =  4 * 1024 * 1024;
static const Ipp32s AUDIO_BUFFER_SIZE =  1 * 1024 * 1024;

#define DEM_CHECK_INIT CHECK_OBJ_INIT(m_pParser)

using namespace UMC;


Demuxer::Demuxer()
{
    m_pParser = NULL;
}

Demuxer::~Demuxer()
{
    Close();
}

Status Demuxer::Run(void)
{
    DEM_CHECK_INIT;
    return UMC_OK;
}

Status Demuxer::Stop(void)
{
    DEM_CHECK_INIT;
    return UMC_OK;
}

Status Demuxer::Close(void)
{
    DEM_CHECK_INIT;

    Ipp32s i;
    for (i = 0; i < MAX_TRACK; i++)
    {
        if (m_pFC[i])
        {
            delete m_pFC[i];
            m_pFC[i] = NULL;
        }
        if (m_pPrevData[i])
        {
            delete m_pPrevData[i];
            m_pPrevData[i] = NULL;
        }
    }

    if (m_pParser)
    {
        delete m_pParser;
        m_pParser = NULL;
    }
    if (m_bInnerMemAllocator && m_pMemoryAllocator)
    {
        delete m_pMemoryAllocator;
        m_pMemoryAllocator = NULL;
        m_bInnerMemAllocator = false;
    }

    return UMC_OK;
}

Status Demuxer::Init(SplitterParams *pInit)
{
    if (m_pParser) // already initialized
        return UMC_ERR_FAILED;

    if(!pInit)
        return UMC_ERR_NULL_PTR;

    if (NULL == pInit->m_pDataReader)
        return UMC_ERR_NULL_PTR;

    m_pParser = NULL;
    m_pMemoryAllocator = NULL;

    Status umcRes = CreateAndInitParser(pInit);
    if (UMC_OK != umcRes)
    {
        TerminateInit();
        return umcRes;
    }

    // estimate duration
    m_dDuration = -1.0;
    m_dBytesPerSec = -1.0;
    m_uiSourceSize = m_pParser->GetSize();
    m_bEncryptedData = (pInit->m_iFlags & FLAG_VSPL_ENCRYPTED_DATA) > 0;
    if (m_uiSourceSize > 0 && !m_bEncryptedData)
    {
        m_Corrector.AttachParser(m_pParser);
        m_dDuration = GetExactDuration();
        if (m_dDuration < 0)
            m_Corrector.AttachParser(NULL);
    }

    // determine memory allocator
    m_bInnerMemAllocator = (pInit->m_pMemoryAllocator == NULL);
    m_pMemoryAllocator = m_bInnerMemAllocator ? new DefaultMemoryAllocator : pInit->m_pMemoryAllocator;
    if (!m_pMemoryAllocator)
    {
        TerminateInit();
        return UMC_ERR_ALLOC;
    }

    // initialize info fields
    m_SplInfo.m_systemType = m_pParser->GetSystemStreamType();
    m_SplInfo.m_fRate = 1.0;
    m_SplInfo.m_fDuration = -1.0;
    m_SplInfo.m_iTracks = 0;
    m_SplInfo.m_iFlags = 0;
    m_SplInfo.m_ppTrackInfo = m_pInfoStack;
    ippsSet_8u(0, (Ipp8u *)m_pInfoStack, MAX_TRACK * sizeof(void *));
    ippsSet_8u(0, (Ipp8u *)m_pPrevData, MAX_TRACK * sizeof(void *));
    ippsSet_8u(0, (Ipp8u *)m_pFC, MAX_TRACK * sizeof(void *));
    m_uiTracksReady = 0;
    m_uiTracks = 0;
    m_iCurTrack = -1;
    m_iRefTrack = -1;

    m_bBufFilled = false;
    m_bPSIWasChanged = false;
    m_bEndOfStream = false;
    m_dRate = 1.0;
    m_uiNOfFrames = 0;
    m_uiTotalSize = 0;
    m_uiAdaptiveFactor = 1;
    m_dPrevTimeGap = -1.0;
    m_dAudToBuf = 0.05;
    if (DynamicCast<DemuxerParams, SplitterParams>(pInit))
        m_dAudToBuf = ((DemuxerParams*)pInit)->m_dAudToBuf;
    return UMC_OK;
}

Status Demuxer::SetTrackState(Ipp32u nTrack, TrackState state)
{
    if (nTrack >= m_uiTracks)
        return UMC_ERR_FAILED;

    if (NULL == m_pFC[nTrack])
        return UMC_ERR_FAILED;

    if(state == TRACK_ENABLED)
    {
        // reset buffer if it was unselected before
        if (!m_pFC[nTrack]->GetInfo()->m_bEnabled)
            m_pFC[nTrack]->Reset();

        m_pFC[nTrack]->GetInfo()->m_bEnabled = true;
        if (m_SplInfo.m_ppTrackInfo[nTrack])
            m_SplInfo.m_ppTrackInfo[nTrack]->m_bEnabled = true;
    }
    else if(state == TRACK_DISABLED)
    {
        m_pFC[nTrack]->GetInfo()->m_bEnabled = false;
        if (m_SplInfo.m_ppTrackInfo[nTrack])
            m_SplInfo.m_ppTrackInfo[nTrack]->m_bEnabled = false;
    }
    else
        return UMC_ERR_UNSUPPORTED; // unsupported state

    if (m_iRefTrack < 0 || !m_pFC[m_iRefTrack]->GetInfo()->m_bEnabled)
        AssignRefTrack();

    return UMC_OK;
}

Status Demuxer::SetRate(Ipp64f rate)
{
    m_dRate = rate;
    m_uiAdaptiveFactor = 1;
    m_dPrevTimeGap = -1.0;
    m_bEndOfStream = false;

    Ipp32s i;
    for (i = 0; i < (Ipp32s)m_uiTracks; i++)
    {
        if (NULL != m_pFC[i])
        {
            m_pFC[i]->Reset();
            m_pPrevData[i]->SetBufferPointer(NULL, 0);
            m_pFC[i]->SetRate(rate);
        }
    }

    if (m_dRate < 0.0 && m_iRefTrack >= 0 && (m_pFC[m_iRefTrack]->GetInfo()->m_type & TRACK_ANY_VIDEO))
        m_Segmentator.RestartSegmentation(m_pParser, m_iRefTrack, (VideoFrameConstructor *)m_pFC[m_iRefTrack]);
    else
        return UMC_ERR_FAILED;
    return UMC_OK;
}

Status Demuxer::SetPosition(Ipp64u pos)
{
    Status umcRes = m_pParser->SetPosition(pos);
    if (UMC_OK != umcRes)
        return umcRes;

    m_iCurTrack = -1;
    m_bEndOfStream = false;

    Ipp32s i;
    for (i = 0; i < (Ipp32s)m_uiTracks; i++)
    {
        if (NULL != m_pFC[i])
        {
            m_pFC[i]->Reset();
            m_pPrevData[i]->SetBufferPointer(NULL, 0);
        }
    }

    if (m_dRate < 0.0 && m_iRefTrack >= 0 && (m_pFC[m_iRefTrack]->GetInfo()->m_type & TRACK_ANY_VIDEO))
        m_Segmentator.RestartSegmentation(m_pParser, m_iRefTrack, (VideoFrameConstructor *)m_pFC[m_iRefTrack]);
    return UMC_OK;
}

Status Demuxer::SetTimePosition(Ipp64f timePos)
{
    Status umcRes = UMC_OK;
    Ipp32s attempt;

    if (!m_Corrector.IsAttachedWithParser() || 0 == m_uiSourceSize || m_dDuration <= 0.0)
    {
        Ipp64f duration = GetDuration();
        return SetPosition(duration > 0.0 ? (Ipp64u)(timePos * m_dBytesPerSec) : (Ipp64u)0);
    }

    timePos += m_Start.dTime;
    if (timePos < m_Start.dTime)
        timePos = m_Start.dTime;
    if (timePos > m_End.dTime)
        timePos = m_End.dTime;

    // reset streaming variables
    m_iCurTrack = -1;
    m_bEndOfStream = false;

    // reset buffers
    Ipp32u uiTrack;
    for (uiTrack = 0; uiTrack < m_uiTracks; uiTrack++)
    {
        if (m_pFC[uiTrack])
        {
            m_pFC[uiTrack]->Reset();
            m_pPrevData[uiTrack]->SetBufferPointer(NULL, 0);
        }
    }

    // step-by-step approximation
    CheckPoint checkPointA = m_Start;
    CheckPoint checkPointB = m_End;
    CheckPoint checkPointC;
    Ipp64f dLocalBytesPerSec;
    for (attempt = 0; attempt < 5; attempt++)
    {
        dLocalBytesPerSec = (Ipp64s)(checkPointB.uiPos - checkPointA.uiPos) / (checkPointB.dTime - checkPointA.dTime);
        checkPointC.uiPos = (Ipp64u)((Ipp64s)checkPointA.uiPos + (dLocalBytesPerSec * (timePos - checkPointA.dTime)));
        umcRes = m_pParser->SetPosition(checkPointC.uiPos);
        if (UMC_OK != umcRes) return umcRes;
        m_Corrector.ResetAfterReposition();

        umcRes = m_Corrector.GetSystemTime(checkPointC);
        if (UMC_OK != umcRes)
            return umcRes;

        // approximated point is coincide with one of ends (A or B)
        if (checkPointC.uiPos == checkPointA.uiPos || checkPointC.uiPos == checkPointB.uiPos)
            break;

        if (checkPointC.dTime < timePos)
            checkPointA = checkPointC;
        else
            checkPointB = checkPointC;

        if (UMC_OK != umcRes || (timePos - checkPointC.dTime < 0.6 && timePos - checkPointC.dTime > -0.6))
            break;
    }

    umcRes = m_pParser->SetPosition(checkPointC.uiPos);
    if (m_dRate < 0.0 && m_iRefTrack >= 0 && (m_pFC[m_iRefTrack]->GetInfo()->m_type & TRACK_ANY_VIDEO))
        m_Segmentator.RestartSegmentation(m_pParser, m_iRefTrack, (VideoFrameConstructor *)m_pFC[m_iRefTrack]);

    return UMC_OK;
}

Status Demuxer::GetTimePosition(Ipp64f &)
{
    return UMC_ERR_NOT_IMPLEMENTED;
}

Status Demuxer::GetInfo(SplitterInfo **ppInfo)
{
    Ipp32u i;
    if (!ppInfo)
        return UMC_ERR_NULL_PTR;

    bool bDurationUpdated = false;
    m_SplInfo.m_fRate = m_dRate;
    if (m_SplInfo.m_fDuration < 0.0)
    {
        bDurationUpdated = true;
        m_SplInfo.m_fDuration = GetDuration();
    }

    m_SplInfo.m_systemType = m_pParser->GetSystemStreamType();
    if (IsPure() && m_pFC[0])
    {
        if (TRACK_MPEG1V == m_pFC[0]->GetInfo()->m_type)
            m_SplInfo.m_systemType = MPEG1_PURE_VIDEO_STREAM;
    }

    m_SplInfo.m_iTracks = m_uiTracks;
    m_SplInfo.m_iFlags = 0;

    for (i = 0; i < m_SplInfo.m_iTracks; i++)
    {
        m_SplInfo.m_ppTrackInfo[i] = m_pFC[i] ? m_pFC[i]->GetInfo() : m_pParser->GetTrackInfo(i);
        ((Mpeg2TrackInfo *)m_SplInfo.m_ppTrackInfo[i])->SetDuration(m_SplInfo.m_fDuration);
    }

    ppInfo[0] = &m_SplInfo;
    return UMC_OK;
}

Status Demuxer::CheckNextData(MediaData *data, Ipp32u *pTrack)
{
    Status umcRes = UMC_OK;
    SplMediaData sample;

    if (NULL == m_pParser)
        return UMC_ERR_NOT_INITIALIZED;

    if (m_bEndOfStream)
        return UMC_ERR_END_OF_STREAM;

    if (NULL == data || NULL == pTrack)
        return UMC_ERR_NULL_PTR;

    if (m_bPSIWasChanged)
    {
        // CheckNextData was called after notification about PSI changes
        // destroy all frame constructors
        Ipp32u i;
        for(i = 0; i < m_uiTracks; i++)
        {
            // invalidate allocated TrackInfo structure (instead of deleting them)
            if (m_SplInfo.m_ppTrackInfo[i])
                ((Mpeg2TrackInfo *)m_SplInfo.m_ppTrackInfo[i])->m_iFirstFrameOrder = -1;

            // destroy allocated frame constructors
            if (m_pFC[i])
            {
                delete m_pFC[i];
                m_pFC[i] = NULL;
            }
        }

        m_uiTracks = 0;
        m_bPSIWasChanged = false;
    }

    if (m_dRate < 0.0)
        umcRes = CheckNextDataBackward(&sample, pTrack);
    else
        umcRes = CheckNextDataForward(&sample, pTrack);

    if (UMC_ERR_END_OF_STREAM == umcRes)
    {
        // if end of stream occurs, deliver END_OF_STREAM to all FCs
        Ipp32u i = 0;
        for (i = 0; i < m_uiTracks; i++)
        {
            if (m_pFC[i])
                umcRes = m_pFC[i]->UnLockInputBuffer(NULL, UMC_ERR_END_OF_STREAM);
            if (umcRes == UMC_OK)
            {
                umcRes = m_pFC[i]->GetLastFrame(&sample);
                *pTrack = m_iCurTrack = i;
                if (m_pFC[i]->GetInfo()->m_iFirstFrameOrder < 0)
                {
                    m_pFC[i]->GetInfo()->m_iFirstFrameOrder = m_uiTracksReady;
                    m_uiTracksReady += 1;
                }
                break;
            }
        }

        if (UMC_OK != umcRes)
            m_bEndOfStream = true;
    }

    if (UMC_OK == umcRes)
    {
        SplMediaData *pMediaSampleData = DynamicCast<SplMediaData, MediaData>(data);
        if (pMediaSampleData)
            *pMediaSampleData = sample;
        else
            *data = *((MediaData *)&sample);
    }

    return umcRes;
}

Status Demuxer::GetNextData(MediaData *data, Ipp32u *pTrack)
{
    Status umcRes = UMC_ERR_END_OF_STREAM;
    Ipp32s i;

    // parse one frame (exactly)
    umcRes = CheckNextData(data, pTrack);
    if (UMC_OK == umcRes)
    {
        // if OK, get parsed frame and return it
        umcRes = m_pFC[m_iCurTrack]->LockOutputBuffer(data);

        if (UMC_OK == umcRes)
            m_pFC[m_iCurTrack]->UnLockOutputBuffer(data);
    }

    if (UMC_OK != umcRes)
    {
        // in case of error try to find frame in buffers
        for (i = 0; i < (Ipp32s)m_uiTracks; i++)
        {
            if (m_pFC[i])
            {
                umcRes = m_pFC[i]->LockOutputBuffer(data);
                if (UMC_OK == umcRes)
                {
                    umcRes = m_pFC[i]->UnLockOutputBuffer(data);
                    *pTrack = i;
                    break;
                }
            }
        }
    }

    return umcRes;
}

Status Demuxer::CheckNextDataForward(SplMediaData *data, Ipp32u *pTrack)
{
    Status umcRes = UMC_OK;
    SplMediaData chunk;
    bool bFrameFound = false;

    chunk.SetBufferPointer((Ipp8u *)1, 0);

    // try to find next frame in last used buffer
    if (-1 != m_iCurTrack)
    {
        umcRes = m_pFC[m_iCurTrack]->UnLockInputBuffer(NULL);
        if (UMC_OK == umcRes)
        {
            bFrameFound = true;
            *pTrack = m_iCurTrack;
        }
        else
            m_iCurTrack = -1;
    }

    while (!bFrameFound)
    {
        // check current data chunk
        umcRes = m_pParser->CheckNextData(&chunk, pTrack);
        if (UMC_OK != umcRes)
        {
            if (UMC_WRN_INVALID_STREAM == umcRes)
            {
                Ipp32u i;
                for(i = 0; i < m_uiTracks; i++)
                {
                    if (m_pFC[i])
                        m_pFC[i]->UnLockInputBuffer(NULL, UMC_ERR_END_OF_STREAM);
                }

                m_iCurTrack = -1;
                m_iRefTrack = -1;
                m_bPSIWasChanged = true;
            }
            return umcRes;
        }

        m_iCurTrack = *pTrack;

        // check and if stream is new try to register it
        if (NULL == m_pFC[m_iCurTrack])
            umcRes = CreateFrameConstructor(m_iCurTrack);

        // pass data from parser to frame constructor
        if ((!m_pFC[m_iCurTrack]) ||
            (!m_pFC[m_iCurTrack]->GetInfo()->m_bEnabled &&
              m_pFC[m_iCurTrack]->GetInfo()->m_iFirstFrameOrder >= 0) ||
            (m_dRate > 1.0 && m_iCurTrack != m_iRefTrack))
        {
            // skip when frame constructor was failed to be created
            // skip packets from all except reference track at non-normal playback
            // skip packets from unselected tracks for those first frame was constructed
            umcRes = m_pParser->MoveToNextHeader();
            if (UMC_OK != umcRes) return umcRes;
        }
        else
        {
            // order space for chunk in FC's buffer
            umcRes = m_pFC[m_iCurTrack]->LockInputBuffer(&chunk);
            if (UMC_OK != umcRes)
            {
                m_bBufFilled = true;
                return umcRes;
            }

            // copy chunk from parser to FC's buffer
            umcRes = m_pParser->GetNextData(&chunk, pTrack);
            if (UMC_OK != umcRes)
                return umcRes;

            // ask for whole frame
            umcRes = m_pFC[m_iCurTrack]->UnLockInputBuffer(&chunk);
            if (UMC_OK == umcRes)
                bFrameFound = true;
        }
    }

    m_pFC[m_iCurTrack]->GetLastFrame(data);
    if (m_pFC[m_iCurTrack]->GetInfo()->m_iFirstFrameOrder < 0)
    {
        m_pFC[m_iCurTrack]->GetInfo()->m_iFirstFrameOrder = m_uiTracksReady;
        m_uiTracksReady += 1;
    }
    if (m_iCurTrack == m_iRefTrack)
    {
        m_uiTotalSize += data->GetDataSize();
        m_uiNOfFrames++;
    }

    // very fast forward adaptive jump
    if (m_uiSourceSize > 0 && -1 != m_iCurTrack && m_dRate > 4.0 && m_dBytesPerSec > 0.0)
    {
        Ipp64u pos, newPos;
        Ipp64f frameRate = ((VideoStreamInfo *)m_pFC[m_iRefTrack]->GetInfo()->m_pStreamInfo)->fFramerate;
        Ipp32s bytesPerFrame = (Ipp32s)(0.5 * m_dBytesPerSec / ((frameRate > 0.0) ? frameRate : 30.0));
        pos = data->GetAbsPos();
        newPos = pos + (Ipp32s)(bytesPerFrame * m_dRate * m_uiAdaptiveFactor);
        m_pParser->SetPosition(newPos);

        if (-1 != m_iRefTrack)
            m_pFC[m_iRefTrack]->SoftReset();
    }

    return umcRes;
}

Status Demuxer::CheckNextDataBackward(SplMediaData *data, Ipp32u *pTrack)
{
    if (!m_pParser)
        return UMC_ERR_FAILED;
    Status umcRes = UMC_OK;
    for (;;)
    {
        // pull next segment
        umcRes = m_Segmentator.PullSegment(data);
        if (UMC_OK != umcRes && UMC_ERR_NOT_ENOUGH_DATA != umcRes)
            return umcRes;

        // check if at least one sample is available
        if (UMC_OK == m_pFC[m_iRefTrack]->LockOutputBuffer(data))
            break;
    }

    if (UMC_OK == umcRes && m_uiSourceSize > 0 && m_dRate < -4.0 && m_dBytesPerSec > 0.0)
    {
        Ipp64f frameRate = ((VideoStreamInfo *)m_pFC[m_iRefTrack]->GetInfo()->m_pStreamInfo)->fFramerate;
        Ipp32s bytesPerFrame = (Ipp32s)(1.0 * m_dBytesPerSec / ((frameRate > 0.0) ? frameRate : 30.0));
        m_Segmentator.AdditionalJump((Ipp32s)(bytesPerFrame * (m_dRate + 1.0) * m_uiAdaptiveFactor));
    }

    pTrack[0] = m_iRefTrack;
    return UMC_OK;
}

Status Demuxer::CheckNextData(MediaData* data, Ipp32u uiTrack)
{
    if (NULL == data)
        return UMC_ERR_NULL_PTR;

    if (uiTrack >= m_uiTracks)
        return UMC_ERR_FAILED;

    m_pFC[uiTrack]->UnLockOutputBuffer(m_pPrevData[uiTrack]);
    if (!m_pFC[uiTrack] || !m_pFC[uiTrack]->GetInfo()->m_bEnabled)
        return UMC_ERR_FAILED;

    m_pFC[uiTrack]->UnLockOutputBuffer(m_pPrevData[uiTrack]);
    return m_pFC[uiTrack]->LockOutputBuffer(data);
}

Status Demuxer::GetNextData(MediaData* data, Ipp32u uiTrack)
{
    if (NULL == data)
        return UMC_ERR_NULL_PTR;

    if (uiTrack >= m_uiTracks)
        return UMC_ERR_FAILED;

    if (!m_pFC[uiTrack] || !m_pFC[uiTrack]->GetInfo()->m_bEnabled)
        return UMC_ERR_FAILED;

    m_pFC[uiTrack]->UnLockOutputBuffer(m_pPrevData[uiTrack]);
    Status umcRes = m_pFC[uiTrack]->LockOutputBuffer(data);
    if (UMC_OK == umcRes)
        m_pPrevData[uiTrack][0] = data[0];

#ifdef DUMP_SOMETHING
    if (UMC_OK == umcRes)
        DumpSample(uiTrack);
#endif //DUMP_SOMETHING

    return umcRes;
}

Ipp32s Demuxer::GetTrackByPid(Ipp32u uiPid)
{
    return m_pParser ? m_pParser->GetTrackByPid(uiPid) : -1;
}

void Demuxer::AlterQuality(Ipp64f time)
{
    if (time > 0.0)
        if (time > m_dPrevTimeGap)
            m_uiAdaptiveFactor++;
    m_dPrevTimeGap = time;
}

Status Demuxer::CreateFrameConstructor(Ipp32s iTrack)
{
    Status umcRes = UMC_OK;
    Ipp32u size = AUDIO_BUFFER_SIZE;

    Mpeg2TrackInfo *pInfo = m_pParser->GetTrackInfo(iTrack);
    if (!pInfo)
        return UMC_ERR_FAILED;

    FrameConstructor* pFC = 0;
    if (m_bEncryptedData)
    {
        pFC = new PesFrameConstructor;
        size = AUDIO_BUFFER_SIZE;
        if (m_iRefTrack < 0)
            m_iRefTrack = iTrack;
    }
    else if (pInfo->m_type & TRACK_ANY_VIDEO)
    {
        if ((TRACK_MPEG1V | TRACK_MPEG2V) & pInfo->m_type)
            pFC = new Mpeg2FrameConstructor;
        else if (TRACK_MPEG4V == pInfo->m_type)
            pFC = new Mpeg4FrameConstructor;
        else if (TRACK_H261 == pInfo->m_type)
            pFC = new H261FrameConstructor;
        else if (TRACK_H263 == pInfo->m_type)
            pFC = new H263FrameConstructor;
        else if (TRACK_H264 == pInfo->m_type)
            pFC = new H264FrameConstructor;
        else
            pFC = new FrameConstructor;

        if (-1 == m_iRefTrack || !(m_pParser->GetTrackInfo(m_iRefTrack)->m_type & TRACK_ANY_VIDEO))
            m_iRefTrack = iTrack;

        size = VIDEO_BUFFER_SIZE;
    }
    else if (pInfo->m_type & TRACK_ANY_AUDIO)
    {
        if (MPEGx_PURE_AUDIO_STREAM == m_SplInfo.m_systemType ||
            WAVE_STREAM             == m_SplInfo.m_systemType ||
            ADTS_STREAM             == m_SplInfo.m_systemType ||
            ADIF_STREAM             == m_SplInfo.m_systemType ||
            AVI_STREAM              == m_SplInfo.m_systemType)
            pFC = new PureAudioFrameConstructor();
        else
        {
            if (AAC_MPEG4_STREAM == ((AudioStreamInfo *)pInfo->m_pStreamInfo)->streamType)
                pFC = new TimeStampedAudioFrameConstructor();
            else
                pFC = new BufferedAudioFrameConstructor(m_dAudToBuf);
        }

        if (-1 == m_iRefTrack)
            m_iRefTrack = iTrack;
    }
    else if (pInfo->m_type & TRACK_ANY_VBI)
    {
        pFC = new TimeStampedAudioFrameConstructor;
    }
    else if (pInfo->m_type & TRACK_ANY_SPECIAL)
    {
        pFC = new FrameConstructor;
    }
    else // TRACK_UNKNOWN
    {
        return UMC_OK;
    }

    FrameConstructorParams fcParams;
    fcParams.m_nOfFrames = 10;
    fcParams.m_lBufferSize = size;
    fcParams.m_pInfo = pInfo;
    fcParams.m_pMemoryAllocator = m_pMemoryAllocator;
    fcParams.m_bStopAtFrame = true;
    fcParams.m_bPureStream = IsPure();

    umcRes = pFC->Init((MediaReceiverParams *)&fcParams);
    if (UMC_OK != umcRes) {
        delete pFC;
        return umcRes;
    }

    if (!m_pPrevData[iTrack])
        m_pPrevData[iTrack] = new MediaData;
    m_pPrevData[iTrack]->SetBufferPointer(NULL, 0);

    m_pFC[iTrack] = pFC;
    m_uiTracks = m_pParser->GetNumberOfTracks();

    return umcRes;
}

Status Demuxer::CreateAndInitParser(SplitterParams *pParams)
{
    bool bEncrypted = (pParams->m_iFlags & FLAG_VSPL_ENCRYPTED_DATA) > 0;
    SystemStreamType type = bEncrypted ? MPEG2_TRANSPORT_STREAM : DetectSystem(pParams->m_pDataReader);
    StreamParserParams *pParserParams = NULL;
    switch (type)
    {
        case MPEG1_PROGRAMM_STREAM:
        case MPEG2_PROGRAMM_STREAM:
        case MPEG2_PES_PACKETS_STREAM:
            m_pParser = new Mpeg2PsParser;
            pParserParams = new StreamParserParams;
            break;
        case MPEG2_TRANSPORT_STREAM:
        case MPEG2_TRANSPORT_STREAM_TTS:
        case MPEG2_TRANSPORT_STREAM_TTS0:
            m_pParser = new Mpeg2TsParser;
            pParserParams = new Mpeg2TsParserParams;
            break;
        case WAVE_STREAM:
            m_pParser = new WaveParser;
            pParserParams = new StreamParserParams;
            break;
        case UNDEF_STREAM:
            return UMC_ERR_INVALID_STREAM;
        default: // pure streams
            m_pParser = new StreamParser;
            pParserParams = new StreamParserParams;
            break;
    }

    if (!m_pParser || !pParserParams)
    {
        delete pParserParams;
        return UMC_ERR_ALLOC;
    }

    // prepare stream parser parameters
    pParserParams->m_pDataReader = pParams->m_pDataReader;
    pParserParams->m_SystemType = type;
    if (DynamicCast<DemuxerParams, SplitterParams>(pParams) && DynamicCast<Mpeg2TsParserParams, StreamParserParams>(pParserParams))
        ((Mpeg2TsParserParams*)pParserParams)->m_bDetectPSIChanges = ((DemuxerParams*)pParams)->m_pOnPSIChangeEvent != NULL;

    Status umcRes = m_pParser->Init(*pParserParams);
    delete pParserParams;
    return umcRes;
}

Ipp64f Demuxer::GetDuration(void)
{
    // first, check if parser provides duration
    if (m_pParser->GetDuration() >= 0.0)
    {
        m_dDuration = m_pParser->GetDuration();
        if (m_dDuration > 0.0)
            m_dBytesPerSec = (((Ipp64f)(Ipp64s)m_uiSourceSize) / m_dDuration);
        return m_dDuration;
    }

    if (m_dDuration < 0.0 || IsPure())
    {
        // if parser isn't able to provide duration, try to estimate it
        if (m_uiSourceSize > 0)
        {
            if (-1 != m_iRefTrack && m_pFC[m_iRefTrack])
            {
                // estimation for pure streams
                Ipp32u bitrate = 0;
                Ipp64f frame_rate = 0.0;
                if (m_pFC[m_iRefTrack]->GetInfo()->m_type & TRACK_ANY_VIDEO)
                {
                    bitrate = ((VideoStreamInfo *)m_pFC[m_iRefTrack]->GetInfo()->m_pStreamInfo)->iBitrate;
                    frame_rate = ((VideoStreamInfo *)m_pFC[m_iRefTrack]->GetInfo()->m_pStreamInfo)->fFramerate;
                    if (frame_rate > 0.0)
                        bitrate = 0; // do not use bitrate for video if framerate is valid
                    else if (0 == bitrate)
                        frame_rate = 30.0; // if both framerate and bitrate are invalid assume 30fps
                }
                else if (m_pFC[m_iRefTrack]->GetInfo()->m_type & TRACK_ANY_AUDIO)
                    bitrate = ((AudioStreamInfo *)m_pFC[m_iRefTrack]->GetInfo()->m_pStreamInfo)->iBitrate;

                if (bitrate > 0)
                    m_dDuration = (Ipp64f)(Ipp64s)m_uiSourceSize / (bitrate / 8);
                else if (frame_rate > 0.0 && m_uiTotalSize > 0 && m_uiNOfFrames > 0 && (m_uiNOfFrames > 30 || m_bEndOfStream || m_bBufFilled))
                    m_dDuration = ((((Ipp64f)(Ipp64s)m_uiSourceSize) / (Ipp64s)m_uiTotalSize) * m_uiNOfFrames) / frame_rate;
                if (m_dDuration >= 0.0)
                    m_dBytesPerSec = (((Ipp64f)(Ipp64s)m_uiSourceSize) / m_dDuration);
            }
        }
    }

    return m_dDuration;
}

void Demuxer::TerminateInit()
{
    delete m_pParser;
    m_pParser = NULL;
    if (m_bInnerMemAllocator)
    {
        delete m_pMemoryAllocator;
        m_pMemoryAllocator = NULL;
        m_bInnerMemAllocator = false;
    }
}

bool Demuxer::IsPure(void)
{
    if (AVI_STREAM == m_SplInfo.m_systemType ||
        MP4_ATOM_STREAM == m_SplInfo.m_systemType ||
        ASF_STREAM == m_SplInfo.m_systemType ||
        MPEG1_PES_PACKETS_STREAM == m_SplInfo.m_systemType ||
        MPEG1_PROGRAMM_STREAM == m_SplInfo.m_systemType ||
        MPEG2_PES_PACKETS_STREAM == m_SplInfo.m_systemType ||
        MPEG2_PROGRAMM_STREAM == m_SplInfo.m_systemType ||
        MPEG2_TRANSPORT_STREAM == m_SplInfo.m_systemType ||
        MPEG2_TRANSPORT_STREAM_TTS == m_SplInfo.m_systemType)
        return false;
    else
        return true;
}

bool Demuxer::IsSeekable(void)
{
    if (m_iRefTrack < 0 ||
       (m_iRefTrack >= 0 && !m_pFC[m_iRefTrack]) ||
       (m_iRefTrack >= 0 && m_pFC[m_iRefTrack] && !(m_pFC[m_iRefTrack]->GetInfo()->m_type & TRACK_ANY_VIDEO)))
       return false;
    else
        return true;
}

static const Ipp32u IS_LINEAR = 1;
static const Ipp32u NUM_OF_INTERVALS = 10;
Ipp64f Demuxer::GetExactDuration(void)
{
    Status umcRes = UMC_OK;
    CheckPoint prevPoint;
    CheckPoint curPoint;
    Ipp64u uiInitStep;
    Ipp64u uiStepAtTheEnd;
    Ipp64u uiStep;
    Ipp64u uiSavedPos;

    if (m_dDuration >= 0.0)
        return m_dDuration;

    m_dBytesPerSec = 0;
    m_dDuration = -1.0;

    uiSavedPos = m_pParser->GetPosition();
    if (0 == m_uiSourceSize)
        return -1.0;

    uiInitStep = uiStep = (IS_LINEAR) ? (m_uiSourceSize / NUM_OF_INTERVALS) : (m_uiSourceSize >> (NUM_OF_INTERVALS - 1));
    if (0 == uiStep)
        return -1.0;

    // get first check point
    m_pParser->SetPosition(0);
    m_Corrector.ResetAfterReposition();
    umcRes = m_Corrector.GetSystemTime(m_Start);
    if (UMC_OK != umcRes)
    {
        m_pParser->SetPosition(uiSavedPos);
        return -1.0;
    }

    uiStepAtTheEnd = 100000;
    if (MPEG1_PROGRAMM_STREAM == m_SplInfo.m_systemType || MPEG2_PROGRAMM_STREAM == m_SplInfo.m_systemType)
        uiStepAtTheEnd = 10000;

    // get last point without discontinuity detection
    // after loop m_End will contain position of latest time
    Ipp64u start = m_uiSourceSize;
    Ipp64u end = m_uiSourceSize;
    CheckPoint tmpEnd(-1.0, 0);
    m_End.dTime = -1.0;
    while (m_End.dTime < 0.0)
    {
        start = (start > uiStepAtTheEnd) ? start - uiStepAtTheEnd : 0;
        m_pParser->SetPosition(start);
        m_Corrector.ResetAfterReposition();
        tmpEnd.uiPos = start;

        // find latest time at the interval [start; end]
        umcRes = UMC_OK;
        while (UMC_OK == umcRes && m_End.uiPos < end)
            umcRes = m_Corrector.GetSystemTimeUpTo(m_End, end);

        end = start;
        uiStepAtTheEnd *= 2;
    }

    // do additional jumps between m_Start and m_End with discontinuity detection
    Ipp64u uiLastGapPos = 0;
    Ipp64f dCurBps, dPrevBps = -1.0;
    prevPoint = curPoint = m_Start;
    while (curPoint.uiPos < m_End.uiPos)
    {
        curPoint.uiPos += uiStep;
        if (curPoint.uiPos > m_End.uiPos)
            curPoint.uiPos = m_End.uiPos;

        m_pParser->SetPosition(curPoint.uiPos);
        m_Corrector.ResetAfterReposition();

        umcRes = m_Corrector.GetSystemTime(curPoint);
        if (UMC_OK != umcRes)
            break;

        uiStep = (IS_LINEAR) ? uiStep : (curPoint.uiPos - uiLastGapPos);
        dCurBps = prevPoint.GetBPS(curPoint);

        // here is criteria of discontinuity
        if ((curPoint.dTime < prevPoint.dTime) || // negative time-stamp delta
            (dCurBps > 0.0 && (dPrevBps / dCurBps) > 100.) || // too large positive time-stamp delta
            (dPrevBps < 0.0 && dCurBps < 1000.0)) // very small bitrate for at step is very suspisious
        {
            if (TryToDetectDiscontinuity(prevPoint, curPoint, 3))
            {
                uiStep = uiInitStep;
                uiLastGapPos = curPoint.uiPos;
                dCurBps = prevPoint.GetBPS(curPoint);
            }
        }

        dPrevBps = dCurBps;
        prevPoint = curPoint;
    }

    m_End = prevPoint;
    if (m_End.dTime > m_Start.dTime)
    {
        m_dDuration = m_End.dTime - m_Start.dTime;
        m_dBytesPerSec = m_Start.GetBPS(m_End);
    }

    m_pParser->SetPosition(uiSavedPos);
    m_Corrector.ResetAfterReposition();
    return m_dDuration;
}

bool Demuxer::TryToDetectDiscontinuity(CheckPoint start, CheckPoint &rEnd, Ipp32s nOfTries)
{
    if (0 == m_uiSourceSize)
        return false;

    Status umcRes = UMC_OK;
    CheckPoint end = rEnd;
    CheckPoint middle;
    Ipp64f dBpsAt1stHalf, dBpsAt2ndHalf, dCurBPS = 0;
    Ipp32s attempt = 0;

    // this loop is trying to localize discontinuity in the 2 seconds interval
    bool isDetected = false;
    while (attempt < nOfTries || isDetected)
    {
        m_Corrector.ResetAfterReposition();
        umcRes = m_pParser->SetPosition((end.uiPos + start.uiPos) / 2);
        if (UMC_OK != umcRes)
            return false;

        //find next valid time stamp
        umcRes = m_Corrector.GetSystemTime(middle);
        if (UMC_OK != umcRes)
            return false;

        dBpsAt1stHalf = start.GetBPS(middle);
        dBpsAt2ndHalf = middle.GetBPS(end);

        if (dBpsAt1stHalf < 0)
        { // negative jump at 1st half
            isDetected = true;
            end = middle;
            dCurBPS = dBpsAt2ndHalf;
        }
        else if (dBpsAt2ndHalf < 0)
        { // negative jump at 2nd half
            isDetected = true;
            start = middle;
            dCurBPS = dBpsAt1stHalf;
        }
        else if (dBpsAt2ndHalf / dBpsAt1stHalf > 100.0)
        { // can't detect negative jump, assume positive jump at 1st half
            isDetected = true;
            end = middle;
            dCurBPS = dBpsAt2ndHalf;
        }
        else if (dBpsAt1stHalf / dBpsAt2ndHalf > 100.0)
        { // can't detect negative jump, assume positive jump at 2nd half
            isDetected = true;
            start = middle;
            dCurBPS = dBpsAt1stHalf;
        }
        else if (!isDetected)
            break;

        if (dCurBPS <= 0.0)
            dCurBPS = 500000.0;

        attempt++;
        if (isDetected && ((Ipp64s)(end.uiPos - start.uiPos) / dCurBPS) < 2.0)
            break;
    }

    if (isDetected)
    { // scan small fragment (2 seconds) for discontinuity
        isDetected = false;
        middle.uiPos = start.uiPos;
        m_pParser->SetPosition(middle.uiPos);
        m_Corrector.ResetAfterReposition();
        while (UMC_OK == umcRes && middle.uiPos < end.uiPos)
        {
            umcRes = m_Corrector.GetSystemTime(middle, &isDetected);
            if (UMC_OK != umcRes)
                return isDetected;

            if (isDetected)
                break;
        }

        // move end check point to point at the gap detected
        if (isDetected)
            rEnd = middle;
    }

    return isDetected;
}

void Demuxer::AssignRefTrack(void)
{
    Ipp32u i;
    Ipp32s iATrack = -1;
    Ipp32s iVTrack = -1;
    Ipp32s iOTrack = -1;
    for (i = 0; i < m_uiTracks; i++)
    {
        if (m_pFC[i] && m_pFC[i]->GetInfo()->m_bEnabled)
        {
            if (iATrack < 0 && (m_pFC[i]->GetInfo()->m_type & TRACK_ANY_AUDIO))
                iATrack = i;
            else if (iVTrack < 0 && (m_pFC[i]->GetInfo()->m_type & TRACK_ANY_VIDEO))
                iVTrack = i;
            else if (iOTrack < 0 && (m_pFC[i]->GetInfo()->m_type & TRACK_ANY_SPECIAL))
                iOTrack = i;
        }
    }
    m_iRefTrack = (iVTrack >= 0) ? iVTrack : ((iATrack >= 0) ? iATrack : iOTrack);
}

#ifdef DUMP_SOMETHING
void Demuxer::DumpSample(Ipp32u uiTrack)
{
    char binFileName[256] = {0};
    char txtFileName[256] = {0};

    if (m_pFC[uiTrack]->GetInfo()->m_Type & TRACK_ANY_AUDIO)
    {
#ifdef DUMP_AUDIO_BIN
        strcpy(&binFileName[0], "C:/zzz_audio.mpg");
#endif //DUMP_AUDIO_BIN
#ifdef DUMP_AUDIO_TXT
        strcpy(&txtFileName[0], "C:/zzz_audio.txt");
#endif //DUMP_AUDIO_TXT
    }

    if (m_pFC[uiTrack]->GetInfo()->m_Type & TRACK_ANY_VIDEO)
    {
        if (m_dRate > 1.0)
        {
#ifdef DUMP_VIDEO_FF_BIN
            strcpy(&binFileName[0], "C:/zzz_video_ff.mpg");
#endif //DUMP_VIDEO_FF_BIN
#ifdef DUMP_VIDEO_FF_TXT
            strcpy(&txtFileName[0], "C:/zzz_video_ff.txt");
#endif //DUMP_VIDEO_FF_TXT
        }
        else if (m_dRate < 0.0)
        {
#ifdef DUMP_VIDEO_BF_BIN
            strcpy(&binFileName[0], "C:/zzz_video_bf.mpg");
#endif //DUMP_VIDEO_BF_BIN
#ifdef DUMP_VIDEO_BF_TXT
            strcpy(&txtFileName[0], "C:/zzz_video_bf.txt");
#endif //DUMP_VIDEO_BF_TXT
        }
        else
        {
#ifdef DUMP_VIDEO_1X_BIN
            strcpy(&binFileName[0], "C:/zzz_video_1x.mpg");
#endif //DUMP_VIDEO_1X_BIN
#ifdef DUMP_VIDEO_1X_TXT
            strcpy(&txtFileName[0], "C:/zzz_video_1x.txt");
#endif //DUMP_VIDEO_1X_TXT
        }
    }

    if (binFileName[0] || txtFileName[0])
    {
        SplMediaData sampleData;
        m_pFC[uiTrack]->LockOutputBuffer(&sampleData);

        if (txtFileName[0])
        {
            FILE *fd_txt = fopen(txtFileName, "a");
            if (fd_txt)
            {
                fprintf(fd_txt, "size = %6d, absPos = %8x, pts = %2.3f, type = %d\n", sampleData.GetDataSize(), (int)sampleData.GetAbsPos(), sampleData.GetTime(), sampleData.GetFrameType());
                fclose(fd_txt);
            }
        }

        if (binFileName[0])
        {
            FILE *fd = fopen(binFileName, "ab");
            if (fd)
            {
                fwrite(sampleData.GetDataPointer(), sampleData.GetDataSize(), 1, fd);
                fclose(fd);
            }
        }
    }
}
#endif //DUMP_SOMETHING

TimeStampCorrector::TimeStampCorrector()
: m_pParser(NULL), m_dPrevValidSystemTime(0.0), m_ListOfGaps()
{
}

bool TimeStampCorrector::IsAttachedWithParser() const
{
    return (m_pParser != NULL);
}

void TimeStampCorrector::AttachParser(StreamParser *pParser)
{
    m_pParser = pParser;
    m_dPrevValidSystemTime = -1.0;
}

void TimeStampCorrector::ResetAfterReposition(void)
{
    m_dPrevValidSystemTime = -1.0;
}

bool TimeStampCorrector::CorrectTime(CheckPoint &rCheckPoint)
{
    CheckPoint gap;
    Status err = UMC_OK;
    bool isNewGapDetected = false;

    // make corrections using known gaps
    err = m_ListOfGaps.First(gap);
    while (UMC_OK == err)
    {
        if (rCheckPoint.uiPos >= gap.uiPos)
            rCheckPoint.dTime -= gap.dTime;
        err = m_ListOfGaps.Next(gap);
    }

    if (m_dPrevValidSystemTime < 0.0)
    {
        m_dPrevValidSystemTime = rCheckPoint.dTime;
        return isNewGapDetected;
    }

    // check for negative (even small ones) and positive jumps (only big)
    if (rCheckPoint.dTime < m_dPrevValidSystemTime - 1.0 || rCheckPoint.dTime > m_dPrevValidSystemTime + 10.0)
    { // new gap is found
        gap.uiPos = rCheckPoint.uiPos;
        gap.dTime = rCheckPoint.dTime - m_dPrevValidSystemTime;
        m_ListOfGaps.Add(gap);
        rCheckPoint.dTime -= gap.dTime;
        isNewGapDetected = true;
    }

    m_dPrevValidSystemTime = rCheckPoint.dTime;
    return isNewGapDetected;
}

Status TimeStampCorrector::GetSystemTimeUpTo(CheckPoint &rCheckPoint, Ipp64u upToPos, bool *pIsGapDetectHere)
{
    if (!m_pParser)
        return UMC_ERR_NOT_INITIALIZED;

    CheckPoint checkPoint;
    Status umcRes = m_pParser->GetSystemTime(checkPoint, upToPos);
    if (UMC_OK != umcRes)
        return umcRes;

    if(CorrectTime(checkPoint) && pIsGapDetectHere)
        pIsGapDetectHere[0] = true;

    rCheckPoint = checkPoint;
    return UMC_OK;
}

Status TimeStampCorrector::GetSystemTime(CheckPoint &rCheckPoint, bool *pIsGapDetectHere)
{
    return GetSystemTimeUpTo(rCheckPoint, (Ipp64u)-1, pIsGapDetectHere);
}

Segmentator::Segmentator(void)
{
    m_pParser = NULL;
    m_pVFC = NULL;
    m_uiTrack = 0;
    m_iSegFrames = 0;
    m_uiSegStart = 0;
    m_uiSegEnd = 0;
    m_uiSegSize = 200000;
    m_bEndOfStream = false;
    m_bLastSegment = false;
    m_bNewSegment = true;
}

void Segmentator::RestartSegmentation(StreamParser *pParser, Ipp32u uiTrack, VideoFrameConstructor *pVFC)
{
    m_pParser = pParser;
    m_uiTrack = uiTrack;
    m_pVFC = pVFC;
    m_iSegFrames = 0;
    m_uiSegEnd = pParser->GetPosition();
    m_uiSegStart = m_uiSegEnd - IPP_MIN(m_uiSegSize, m_uiSegEnd);
    m_bEndOfStream = false;
    m_bLastSegment = false;
    m_bNewSegment = true;
}

void Segmentator::AdditionalJump(Ipp32s iJumpSize)
{
    if (!m_bNewSegment)
        return; // additional jumps are not allowed when current segment is not finished

    if (iJumpSize < (Ipp32s)m_uiSegSize)
        return; // jump is smaller that current segment size

    if ((Ipp64u)iJumpSize >= m_uiSegEnd)
        m_bEndOfStream = true;

    m_uiSegEnd -= IPP_MIN((Ipp64u)iJumpSize, m_uiSegEnd);
    m_uiSegStart -= IPP_MIN((Ipp64u)iJumpSize, m_uiSegStart);
}

Status Segmentator::PullSegment(SplMediaData *pData)
{
    Status umcRes = UMC_OK;
    Ipp32u curTrack;
    Ipp64u uiCurChunkPos;
    SplMediaData chunk;
    SplMediaData *pChunk;
    bool bEndOfSegment = false;

    // we are at the beginning of the stream, last segment was pulled previously
    if (m_bEndOfStream)
        return UMC_ERR_END_OF_STREAM;

    if (m_bNewSegment)
    {
        // go to start of next segment
        m_pParser->SetPosition(m_uiSegStart);
        m_bNewSegment = false;
    }

    chunk.SetBufferPointer((Ipp8u *)1, 0);

    for (; !bEndOfSegment;)
    {
        // find next packet from video track
        umcRes = CheckPacketFromTrack(&chunk);
        if (UMC_OK != umcRes && UMC_ERR_END_OF_STREAM != umcRes)
            return umcRes;

        // save position of last chunk
        uiCurChunkPos = (UMC_OK == umcRes) ? chunk.GetAbsPos() : m_uiSegEnd;
        if (uiCurChunkPos >= m_uiSegEnd && (!m_pVFC->IsFrameStartFound() || UMC_ERR_END_OF_STREAM == umcRes))
        { // end of segment
            break;
        }

        // order space for chunk in FC's buffer
        umcRes = m_pVFC->LockInputBuffer(&chunk);
        if (UMC_OK != umcRes)
            return umcRes;

        // copy chunk from parser to FC's buffer
        umcRes = m_pParser->GetNextData(&chunk, &curTrack);

        // parse received chunk
        pChunk = &chunk;
        for (; UMC_OK == umcRes;)
        {
            umcRes = m_pVFC->PreUnLockInputBuffer(pChunk);
            pChunk = NULL;
            if (UMC_OK == umcRes)
            {
                m_pVFC->GetLastFrame(pData);
                m_iSegFrames += 1;
                if (1 == m_iSegFrames)
                { // update bottom boundary of segment using position of first sample
                    m_pVFC->GetLastFrame(pData);
                    m_uiSegStart = pData->GetAbsPos();
                }
                if (uiCurChunkPos >= m_uiSegEnd)
                {
                    bEndOfSegment = true;
                    break; // end of segment
                }
            }
        }
    }

    // update segment boundaries
    if (m_iSegFrames > 0 || m_pVFC->IsFrameStartFound())
    { // current segment size is enough
        m_uiSegEnd = m_uiSegStart;
        m_uiSegStart -= IPP_MIN(m_uiSegStart, m_uiSegSize);
    }
    else
    { // current segment size is too small
        m_uiSegStart -= IPP_MIN(m_uiSegStart, m_uiSegSize);
        m_uiSegSize *= 2;
    }

    m_bNewSegment = true;
    if (0 == m_uiSegStart)
    {
        m_bEndOfStream = m_bLastSegment;
        m_bLastSegment = true;
    }

    // at least one frame was found?
    umcRes = m_iSegFrames > 0 ? UMC_OK : UMC_ERR_NOT_ENOUGH_DATA;
    m_iSegFrames = 0;

    // clean up remained bytes
    m_pVFC->GetLastFrame(pData);
    m_pVFC->SoftReset();
    return umcRes;
}

Status Segmentator::CheckPacketFromTrack(SplMediaData *pData)
{
    Ipp32u curTrack;
    for (;;)
    {
        Status umcRes = m_pParser->CheckNextData(pData, &curTrack);
        if (UMC_OK != umcRes || curTrack == m_uiTrack)
            return umcRes;
        if (curTrack != m_uiTrack)
            umcRes = m_pParser->MoveToNextHeader();
    }
}

SystemStreamType Demuxer::DetectSystem(DataReader* pDataReader)
{
    Status umcRes = UMC_OK;
    Ipp8u byteCode;
    Ipp32u longCode;

    if (!pDataReader)
        return UNDEF_STREAM;

    umcRes = pDataReader->Check8u(&byteCode, 0);
    if (UMC_OK != umcRes)
        return UNDEF_STREAM;

    umcRes = pDataReader->Check32u(&longCode, 0);
    if (UMC_OK != umcRes)
        return UNDEF_STREAM;

    if (longCode == 'RIFF') // RIFF
    {
        umcRes = pDataReader->Check32u(&longCode, 8);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;
        if (longCode == 'AVI ')
            return AVI_STREAM;
        else
            return WAVE_STREAM;
    }

    //search some MPEG2 PS start code, it can start from bunch of zeros before
    if (0 == longCode)
    {
        Ipp32s offset = 1;
        while (0 == longCode)
        {
            umcRes = pDataReader->Check32u(&longCode, offset);
            if (UMC_OK != umcRes)
                return UNDEF_STREAM;
            offset++;
        }

        umcRes = pDataReader->Check32u(&longCode, offset);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;

        if ((longCode & 0x0000fff0) == 0x0000fff0)
            return MPEGx_PURE_AUDIO_STREAM;
    }

    if (ID_TS_SYNC == byteCode) //check if transport stream, 0x47 is sync byte for it
        return MPEG2_TRANSPORT_STREAM;
    else if ((0x100 | ID_PS_PACK) == longCode || 0x000001BC == (longCode & 0xFFFFFFFC)) //check if program stream
        return MPEG2_PROGRAMM_STREAM;
    else if ((longCode >= 0x000001C0) && (longCode <= 0x000001DF))  //check if PES audio stream,
        return MPEG2_PES_PACKETS_STREAM;
    else if ((longCode >= 0x000001E0) && (longCode <= 0x000001EF))  //check if PES video stream,
        return MPEG2_PES_PACKETS_STREAM;
    else if (SC_MPEG2_SEQ == longCode) //check if MPEG2 Sequence or Picture start code
        return MPEG2_PURE_VIDEO_STREAM;
    else if (SC_MPEG2_PIC == (longCode & (~0xff))) //check if MPEG2 Sequence or Picture start code
    {
        if (7 == (longCode & 0x1f) ||
            9 == (longCode & 0x1f))
            return H264_PURE_VIDEO_STREAM;

        // create MPEG4FC to assure that stream is actually mpeg4 video
        size_t uiSize = 32 * 1024;
        Mpeg4FrameConstructor fc;
        FrameConstructorParams fcPar;
        fcPar.m_lBufferSize = uiSize;
        fcPar.m_nOfFrames = 1;
        umcRes = fc.Init(&fcPar);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;

        // get buffer
        MediaData data;
        data.SetBufferPointer((Ipp8u *)1, uiSize);
        data.SetDataSize(uiSize);
        umcRes = fc.LockInputBuffer(&data);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;

        // read data to buffer
        umcRes = pDataReader->CacheData(data.GetDataPointer(), uiSize, 0);
        if (UMC_ERR_END_OF_STREAM == umcRes && uiSize > 0)
            umcRes = UMC_OK;
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;

        data.SetDataSize(uiSize);
        fc.UnLockInputBuffer(&data);
        if (TRACK_MPEG4V == fc.GetInfo()->m_type)
            return MPEG4_PURE_VIDEO_STREAM;
    }
    else if ((longCode & 0xfffffc00) == 0x00008000) //check if H.263 pure video
        return H263_PURE_VIDEO_STREAM;
    else if ((longCode & 0xfffff000) == 0x00010000) //check if H.261 pure video
        return H261_PURE_VIDEO_STREAM;
    else if ((longCode & 0x1f) == 7 || (longCode == 0x00000001)) //check if H.264 pure video
        return H264_PURE_VIDEO_STREAM;
    else if (longCode == 0x41444946)
        return ADIF_STREAM;
    else if ((longCode & 0xffe00000) == 0xffe00000 || (longCode & 0xffffff00) == 0x49443300 || (longCode == 0))
    {
        if((longCode & 0x60000) == 0) // layer 4
            return ADTS_STREAM;

        //skip ID3 header
        if ((longCode & 0xffffff00) == 0x49443300)
        {
            Ipp8u byte;
            Ipp32s id3size = 0;
            pDataReader->Get8u(&byte); // 'I'
            pDataReader->Get8u(&byte); // 'D'
            pDataReader->Get8u(&byte); // '3'
            pDataReader->Get8u(&byte); // major version != 0xFF
            pDataReader->Get8u(&byte); // revision number != 0xFF
            pDataReader->Get8u(&byte); // flags (only 3 first bits are used)
            pDataReader->Get8u(&byte); // 21...27 bits of size < 0x80
            id3size |= byte << 21;
            pDataReader->Get8u(&byte); // 14...20 bits of size < 0x80
            id3size |= byte << 14;
            pDataReader->Get8u(&byte); // 07...13 bits of size < 0x80
            id3size |= byte << 7;
            pDataReader->Get8u(&byte); // 00...06 bits of size < 0x80
            id3size |= byte;
            pDataReader->MovePosition(id3size);
        }

        return MPEGx_PURE_AUDIO_STREAM;
    }
    else if ((longCode >> 16) == SC_AC3) //check if AC3 audio stream
        return MPEGx_PURE_AUDIO_STREAM;

    umcRes = pDataReader->Check8u(&byteCode,4);
    if (UMC_OK != umcRes)
        return UNDEF_STREAM;

    if (0x47 == byteCode)
    {
        umcRes = pDataReader->Check8u(&byteCode,196);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;
        if (0x47 == byteCode)
            return MPEG2_TRANSPORT_STREAM_TTS;
    }

    // look over 3kb for MPEG2 system stream start codes
    for (Ipp32u i = 0; i < 3 * 1024; i++)
    {
        umcRes = pDataReader->Check32u(&longCode, i);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;
        if ((0x100 | ID_PS_PACK) == longCode || 0x000001BC == (longCode & 0xFFFFFFFC))
            return MPEG2_PROGRAMM_STREAM;

        umcRes = pDataReader->Check8u(&byteCode, i);
        if (UMC_OK != umcRes)
            return UNDEF_STREAM;

        if (ID_TS_SYNC == byteCode)
        {
            umcRes = pDataReader->Check8u(&byteCode, i + 188);
            if (UMC_OK != umcRes)
                return UNDEF_STREAM;

            if (ID_TS_SYNC == byteCode)
            {
                umcRes = pDataReader->Check8u(&byteCode, i + 2 * 188);
                if (UMC_OK != umcRes)
                    return UNDEF_STREAM;
                if (ID_TS_SYNC == byteCode)
                    return MPEG2_TRANSPORT_STREAM;
            }

            umcRes = pDataReader->Check8u(&byteCode, i + 192);
            if (UMC_OK != umcRes)
                return UNDEF_STREAM;

            if (ID_TS_SYNC == byteCode)
            {
                umcRes = pDataReader->Check8u(&byteCode, i + 2 * 192);
                if (UMC_OK != umcRes)
                    return UNDEF_STREAM;

                if (ID_TS_SYNC == byteCode)
                    return MPEG2_TRANSPORT_STREAM_TTS;
            }
        }
    }

    return UNDEF_STREAM;
}

#endif
