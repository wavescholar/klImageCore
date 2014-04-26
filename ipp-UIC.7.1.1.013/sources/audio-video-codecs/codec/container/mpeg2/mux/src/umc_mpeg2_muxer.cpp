/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_MUXER

#include "vm_debug.h"
#include "umc_automatic_mutex.h"
#include "umc_mpeg2_muxer.h"
#include "umc_mpeg2_muxer_linbuf.h"
#include "umc_mpeg2_muxer_chunk.h"

#include "ippcore.h"
#include "ipps.h"

#define MAX_VIDEO_TRACK_FOR_MPEG2PS 16
#define MAX_MPEG_AUDIO_TRACK_FOR_MPEG2PS 32
#define MAX_AC3_AUDIO_TRACK_FOR_MPEG2PS 8
#define MAX_LPCM_AUDIO_TRACK_FOR_MPEG2PS 8

#define IS_PCR_PID(PROG, ES_NUM) (m_pPMTTableWriterParams[PROG].pPID[ES_NUM] == m_pPMTTableWriterParams[PROG].uiPCRPID)

using namespace UMC;

Status CheckUserIds(IdBank& bank, const MPEG2MuxerParams& par);

MPEG2Muxer::MPEG2Muxer(void)
{
    //vm_debug_setlevel((vm_debug_level)(VM_DEBUG_ERROR | VM_DEBUG_WARNING /*| VM_DEBUG_INFO*/));

    m_pMPEG2MuxerParams = NULL;

    // reset variable(s)
    m_bQuit = false;
    m_uiTotalRate = 0;

    m_pPATTableWriterParams = NULL;
    m_pPATTableWriter = NULL;
    m_pPMTTableWriterParams = NULL;
    m_ppPMTTableWriter = NULL;

    // frame selection for system clock approximation
    m_dReferenceClock = 0.0;
    m_dReferenceClockOfPrevSI = -5.0;

    max_video_buff = 0;
    max_audio_buff = 0;
} //MPEG2Muxer::MPEG2Muxer(void)

MPEG2Muxer::~MPEG2Muxer(void)
{
    Close();
} //MPEG2Muxer::~MPEG2Muxer(void)

Status MPEG2Muxer::Close(void)
{
    Status status;
#if 0
    if (m_lpDataWriter) {
      Ipp32u total_size, size2;
      m_lpDataWriter->GetPosition(&total_size, &size2);

      Ipp32s audio_bitrate = m_pESState[0].bitrate;
      Ipp32s video_bitrate = m_pESState[1].bitrate;
      Ipp64f framerate = m_pESState[1].frameRate;
      Ipp64f ave_video_size = video_bitrate/(8.0*framerate);
      Ipp64f duration = num_video_frames/framerate;
      Ipp64f expected_video_size = video_bitrate*duration/8.0;
      Ipp64f expected_audio_size = audio_bitrate*duration/8.0;
      TmpLog("\n%.4f Video(%dKbit/s %.4f), Audio(%dKbit/s %.4f), Muxed(%.2fMb - %.2fMb - %.2fMb = %.2fMb (%.2f%%))\n",
        (Ipp64f)max_video_size/max_video_buff,
        video_bitrate/1000,
        total_video_size/expected_video_size,
        audio_bitrate/1000,
        total_audio_size/expected_audio_size,
        total_size*1e-6,
        total_video_size*1e-6,
        total_audio_size*1e-6,
        (total_size - total_video_size - total_audio_size)*1e-6,
        100.0*(total_size - total_video_size - total_audio_size)/total_size
        );
    }
#endif

    m_bQuit = false;
    m_uiTotalRate = 0;

    // frame selection for system clock approximation
    m_dReferenceClock = 0.0;
    m_dReferenceClockOfPrevSI = -5.0;

    if (m_pPMTTableWriterParams)
    {
        UMC_FREE(m_pPMTTableWriterParams->pPID);
        UMC_FREE(m_pPMTTableWriterParams->pStreamType);
        UMC_DELETE_ARR(m_pPMTTableWriterParams);
    }

    if (m_ppPMTTableWriter)
    {
        Ipp32s i;
        for (i = 0; i < m_pPATTableWriterParams->iNOfPrograms; i++)
            UMC_DELETE(m_ppPMTTableWriter[i]);

        UMC_FREE(m_ppPMTTableWriter);
    }

    if (m_pPATTableWriterParams)
    {
        UMC_FREE(m_pPATTableWriterParams->pProgramNumbers);
        UMC_DELETE(m_pPATTableWriterParams);
    }

    UMC_DELETE(m_pPATTableWriter);

    UMC_CHECK_FUNC(status, Muxer::Close());

    return UMC_OK;
} //Status MPEG2Muxer::Close(void)

Status MPEG2Muxer::Init(MuxerParams *lpInit)
{
    SystemHeaderParams sysHeaderParams;
    Ipp16u uiPCRTrackNum = 0x1FFF;
    Status status;

    // release muxer before initialization
    Close();

    // allocate MPEG2MuxerParams
    UMC_NEW(m_pParams, MPEG2MuxerParams);
    m_pMPEG2MuxerParams = (MPEG2MuxerParams*)m_pParams;

    // Base muxer Init
    UMC_CHECK_FUNC(status, CopyMuxerParams(lpInit));

    MPEG2MuxerParams *pMPEG2MuxerParams = DynamicCast<MPEG2MuxerParams, MuxerParams>(lpInit);
    if (pMPEG2MuxerParams)
    {
        m_pMPEG2MuxerParams->m_uiAlignmentFlags = pMPEG2MuxerParams->m_uiAlignmentFlags;
        m_pMPEG2MuxerParams->m_dSystemTimeDelay = pMPEG2MuxerParams->m_dSystemTimeDelay;
        m_pMPEG2MuxerParams->m_uiChunkSizeLimit = IPP_MAX(pMPEG2MuxerParams->m_uiChunkSizeLimit, 0x100);
        m_pMPEG2MuxerParams->m_uiProgramNum = pMPEG2MuxerParams->m_uiProgramNum;
        m_pMPEG2MuxerParams->m_uiProgramPid = pMPEG2MuxerParams->m_uiProgramPid;
        m_pMPEG2MuxerParams->m_uiTransportStreamId = pMPEG2MuxerParams->m_uiTransportStreamId;
    }

    if (IsPure(m_pParams->m_SystemType))
    {
        MPEG2ChunkWriterParams params;
        if (MPEG2_PURE_VIDEO_STREAM == m_pParams->m_SystemType ||
            MPEG4_PURE_VIDEO_STREAM == m_pParams->m_SystemType ||
            H264_PURE_VIDEO_STREAM  == m_pParams->m_SystemType ||
            MPEGx_PURE_VIDEO_STREAM == m_pParams->m_SystemType)
        {
            if (!CHECK_TRACK_TYPE(0, VIDEO_TRACK))
                return UMC_ERR_INVALID_PARAMS;

            params.uiBitRate = m_pParams->m_pTrackParams[0].info.video->iBitrate;
        }
        else if (MPEG2_PURE_AUDIO_STREAM == m_pParams->m_SystemType ||
                 MPEGx_PURE_AUDIO_STREAM == m_pParams->m_SystemType)
        {
            if (!CHECK_TRACK_TYPE(0, AUDIO_TRACK))
                return UMC_ERR_INVALID_PARAMS;

            params.uiBitRate = m_pParams->m_pTrackParams[0].info.audio->iBitrate;
        }

        params.pDataWriter = m_pParams->m_lpDataWriter;
        params.uiAlignmentFlags = 0;
        params.esType = MPEG2MUX_ES_UNKNOWN;
        params.uiOfFrames = 1;
        params.uiInputSize = (Ipp32u)m_pParams->m_pTrackParams[0].bufferParams.m_prefInputBufferSize;
        params.uiBufferSize = params.uiInputSize * m_pParams->m_pTrackParams[0].bufferParams.m_numberOfFrames;

        UMC_NEW(m_ppBuffers[0], MPEG2ChunkWriter);
        UMC_CHECK_FUNC(status, m_ppBuffers[0]->Init(&params));
    }
    else if (IsPSOrPES(m_pParams->m_SystemType) || IsTS(m_pParams->m_SystemType))
    {
        if (IsPSOrPES(m_pParams->m_SystemType))
        {
            if (MPEG2_PES_PACKETS_STREAM == m_pParams->m_SystemType)
                m_uiTotalNumStreams = 1;

            for (Ipp32u nNum = 0; nNum < m_uiTotalNumStreams; nNum++)
                UMC_NEW(m_ppBuffers[nNum], MPEG2PSChunkWriter);

            UMC_ALLOC_ARR(sysHeaderParams.pSystemSizeBound, Ipp32u, m_uiTotalNumStreams);
            UMC_ALLOC_ARR(sysHeaderParams.pSystemStreamID, Ipp32u, m_uiTotalNumStreams);
        }
        else // TS, TTS or TTS0
        {
            // choose PCR_PID
            for (uiPCRTrackNum = 0; uiPCRTrackNum < m_uiTotalNumStreams; uiPCRTrackNum++)
                if (CHECK_TRACK_TYPE(uiPCRTrackNum, VIDEO_TRACK))
                    break;

            if (uiPCRTrackNum == m_uiTotalNumStreams)
                uiPCRTrackNum = 0;

            if (IsTTS0(m_pParams->m_SystemType))
            {
                for (Ipp32u nNum = 0; nNum < m_uiTotalNumStreams; nNum++)
                    UMC_NEW(m_ppBuffers[nNum], MPEG2TTSChunkWriter(true));
            }
            else if (IsTTS(m_pParams->m_SystemType))
            {
                for (Ipp32u nNum = 0; nNum < m_uiTotalNumStreams; nNum++)
                    UMC_NEW(m_ppBuffers[nNum], MPEG2TTSChunkWriter(false));
            }
            else
            {
                for (Ipp32u nNum = 0; nNum < m_uiTotalNumStreams; nNum++)
                    UMC_NEW(m_ppBuffers[nNum], MPEG2TSChunkWriter);
            }

            UMC_NEW(m_pPATTableWriterParams, PATTableWriterParams);
            m_pPATTableWriterParams->pDataWriter = m_pParams->m_lpDataWriter;
            m_pPATTableWriterParams->iNOfPrograms = 1;
            m_pPATTableWriterParams->transportStreamId = pMPEG2MuxerParams->m_uiTransportStreamId;

            UMC_ALLOC_ARR(m_pPATTableWriterParams->pProgramNumbers, TsProgramNumbers, m_pPATTableWriterParams->iNOfPrograms);
            m_pPATTableWriterParams->pProgramNumbers[0].pProgramNum = m_pMPEG2MuxerParams->m_uiProgramNum;
            m_pPATTableWriterParams->pProgramNumbers[0].pProgramPid = m_pMPEG2MuxerParams->m_uiProgramPid;

            if (IsTTS0(m_pParams->m_SystemType))
            {
                UMC_NEW(m_pPATTableWriter, TTSPATTableWriter(true));
            }
            else if (IsTTS(m_pParams->m_SystemType))
            {
                UMC_NEW(m_pPATTableWriter, TTSPATTableWriter(false));
            }
            else
            {
                UMC_NEW(m_pPATTableWriter, PATTableWriter);
            }

            UMC_CHECK_FUNC(status, m_pPATTableWriter->Init(m_pPATTableWriterParams));

            UMC_NEW_ARR(m_pPMTTableWriterParams, PMTTableWriterParams, m_pPATTableWriterParams->iNOfPrograms);
            UMC_ALLOC_ZERO_ARR(m_ppPMTTableWriter, PMTTableWriter*, m_pPATTableWriterParams->iNOfPrograms);

            for (Ipp32s nNum = 0; nNum < m_pPATTableWriterParams->iNOfPrograms; nNum++)
            {
                if (IsTTS0(m_pParams->m_SystemType))
                {
                    UMC_NEW(m_ppPMTTableWriter[nNum], TTSPMTTableWriter(true));
                }
                if (IsTTS(m_pParams->m_SystemType))
                {
                    UMC_NEW(m_ppPMTTableWriter[nNum], TTSPMTTableWriter(false));
                }
                else
                {
                    UMC_NEW(m_ppPMTTableWriter[nNum], PMTTableWriter);
                }

                m_pPMTTableWriterParams[nNum].pDataWriter = m_pParams->m_lpDataWriter;
                m_pPMTTableWriterParams[nNum].uiProgramNumber = m_pPATTableWriterParams->pProgramNumbers[nNum].pProgramNum;
                m_pPMTTableWriterParams[nNum].uiProgramPID = m_pPATTableWriterParams->pProgramNumbers[nNum].pProgramPid;
            }

            // this suggests that only one program is currently supported
            m_pPMTTableWriterParams[0].uiNOfStreams = m_uiTotalNumStreams;
            UMC_ALLOC_ARR(m_pPMTTableWriterParams[0].pStreamType, MPEG2MuxerESType, m_uiTotalNumStreams);
            UMC_ALLOC_ARR(m_pPMTTableWriterParams[0].pPID, Ipp32u, m_uiTotalNumStreams);
        }

        m_uiTotalRate = 0;

        IdBank bank;
        UMC_CHECK_FUNC(status, CheckUserIds(bank, *m_pMPEG2MuxerParams));

        //initialize chunk writers
        // use ts params as the most derived class
        MPEG2TSChunkWriterParams params;
        params.uiAlignmentFlags = m_pMPEG2MuxerParams->m_uiAlignmentFlags;
        params.pDataWriter = m_pParams->m_lpDataWriter;
        params.uiOfFrames = 30;
        params.bPackHeaderPresent = true;
        params.uiPacketSize = m_pMPEG2MuxerParams->m_uiChunkSizeLimit;

        // track statistic
        Ipp32u uiPID = MPEG2MUX_INITIAL_ES_PID;
        Ipp32u uiNOfVideoTracks = 0;
        Ipp32u uiNOfMPEGVideoTracks = 0;
        Ipp32u uiNOfAudioTracks = 0;
        Ipp32u uiNOfMPEGAudioTracks = 0;
        Ipp32u uiNOfPrivateAudioTracks = 0;
        Ipp32u uiNOfAC3AudioTracks = 0;
        Ipp32u uiNOfLPCMAudioTracks = 0;
        Ipp32u uiRateOfPrivateTracks = 0;
        for(Ipp32u nNum = 0; nNum < m_uiTotalNumStreams; nNum++)
        {
            params.uiInputSize = (Ipp32u)m_pParams->m_pTrackParams[nNum].bufferParams.m_prefInputBufferSize;
            params.uiBufferSize = params.uiInputSize *
                m_pParams->m_pTrackParams[nNum].bufferParams.m_numberOfFrames;

            if (CHECK_TRACK_TYPE(nNum, AUDIO_TRACK))
            { // audio
                const AudioStreamInfo* info = m_pParams->m_pTrackParams[nNum].info.audio;
                params.esType = ConvertAudioType(m_pParams->m_SystemType, info->streamType);
                if (MPEG2MUX_ES_UNKNOWN == params.esType)
                    return UMC_ERR_UNSUPPORTED;

                params.dFrameRate = 0.0;
                params.uiBitRate = info->iBitrate;
                if (params.uiBitRate == 0)
                    params.uiBitRate = 100000;

                if (MPEG2MUX_ES_AC3_AUDIO == params.esType)
                {
                    params.uiStreamID = MPEG2MUX_PES_ID_PRIVATE_1;
                    params.uiSubStreamID = info->iStreamPID;
                    if (params.uiSubStreamID == IdBank::NO_ID)
                    {
                        params.uiSubStreamID = bank.GetUnusedId(IdBank::AC3_SUB_ID);
                        if (params.uiSubStreamID == IdBank::NO_ID)
                            return UMC_ERR_INVALID_PARAMS;

                        if (IsPSOrPES(m_pParams->m_SystemType))
                            bank.Set(IdBank::AC3_SUB_ID, params.uiSubStreamID);
                    }

                    uiNOfAC3AudioTracks++;
                    uiNOfPrivateAudioTracks++;
                    uiRateOfPrivateTracks += params.uiBitRate;
                }
                else if (MPEG2MUX_ES_LPCM_AUDIO == params.esType)
                {
                    params.uiStreamID = MPEG2MUX_PES_ID_PRIVATE_1;
                    params.uiSubStreamID = info->iStreamPID;
                    if (params.uiSubStreamID == IdBank::NO_ID)
                    {
                        params.uiSubStreamID = bank.GetUnusedId(IdBank::LPCM_SUB_ID);
                        if (params.uiSubStreamID == IdBank::NO_ID)
                            return UMC_ERR_INVALID_PARAMS;

                        if (IsPSOrPES(m_pParams->m_SystemType))
                            bank.Set(IdBank::LPCM_SUB_ID, params.uiSubStreamID);
                    }

                    params.uiBitRate = info->audioInfo.m_iSampleFrequency * info->audioInfo.m_iBitPerSample * info->audioInfo.m_iChannels;
                    if (params.uiBitRate == 0)
                        params.uiBitRate = 100000;

                    uiNOfLPCMAudioTracks++;
                    uiNOfPrivateAudioTracks++;
                    uiRateOfPrivateTracks += params.uiBitRate;
                }
                else // MPEG audio
                {
                    if (IsTS(m_pParams->m_SystemType))
                    {
                        params.uiStreamID = bank.GetUnusedId(IdBank::AUD_ES_ID);
                    }
                    else
                    {
                        params.uiStreamID = info->iStreamPID;
                        if (params.uiStreamID == IdBank::NO_ID)
                        {
                            params.uiStreamID = bank.GetUnusedId(IdBank::AUD_ES_ID);
                            if (params.uiStreamID == IdBank::NO_ID)
                                return UMC_ERR_INVALID_PARAMS;
                            bank.Set(IdBank::AUD_ES_ID, params.uiStreamID);
                        }

                        sysHeaderParams.pSystemStreamID[sysHeaderParams.uiSystemNumberOfStreams] = params.uiStreamID;
                        sysHeaderParams.pSystemSizeBound[sysHeaderParams.uiSystemNumberOfStreams] = params.uiBitRate / 8;
                        sysHeaderParams.uiSystemNumberOfStreams++;
                    }

                    uiNOfMPEGAudioTracks++;
                }

                if (IsTS(m_pParams->m_SystemType))
                {
                    params.uiPID = info->iStreamPID;
                    if (params.uiPID == IdBank::NO_ID)
                    {
                        params.uiPID = bank.GetUnusedId(IdBank::TS_PID);
                        if (params.uiPID == IdBank::NO_ID)
                            return UMC_ERR_INVALID_PARAMS;

                        bank.Set(IdBank::TS_PID, params.uiPID);
                    }

                    m_pPMTTableWriterParams[0].pStreamType[nNum] = params.esType;
                    m_pPMTTableWriterParams[0].pPID[nNum] = params.uiPID;
                    uiPID++;
                }

                uiNOfAudioTracks++;
            }
            else if (CHECK_TRACK_TYPE(nNum, VIDEO_TRACK))
            { // video
                const VideoStreamInfo* info = m_pParams->m_pTrackParams[nNum].info.video;
                params.esType = ConvertVideoType(m_pParams->m_SystemType, info->streamType);
                if (MPEG2MUX_ES_UNKNOWN == params.esType)
                    return UMC_ERR_UNSUPPORTED;

                params.dFrameRate = info->fFramerate;
                if (params.dFrameRate <= 0.0)
                    params.dFrameRate = 30.0;

                params.uiBitRate = info->iBitrate;
                if (params.uiBitRate == 0)
                    params.uiBitRate = 1000000;

                if (IsTS(m_pParams->m_SystemType))
                {
                    params.uiStreamID = bank.GetUnusedId(IdBank::VID_ES_ID);
                    params.uiPID = info->iStreamPID;
                    if (params.uiPID == IdBank::NO_ID)
                    {
                        params.uiPID = bank.GetUnusedId(IdBank::TS_PID);
                        if (params.uiPID == IdBank::NO_ID)
                            return UMC_ERR_INVALID_PARAMS;

                        bank.Set(IdBank::TS_PID, params.uiPID);
                    }
                    m_pPMTTableWriterParams[0].pStreamType[nNum] = params.esType;
                    m_pPMTTableWriterParams[0].pPID[nNum] = params.uiPID;
                    uiPID++;
                }
                else
                {
                    params.uiStreamID = info->iStreamPID;
                    if (params.uiStreamID == IdBank::NO_ID)
                    {
                        params.uiStreamID = bank.GetUnusedId(IdBank::VID_ES_ID);
                        if (params.uiStreamID == IdBank::NO_ID)
                            return UMC_ERR_INVALID_PARAMS;
                        bank.Set(IdBank::VID_ES_ID, params.uiStreamID);
                    }

                    sysHeaderParams.pSystemStreamID[sysHeaderParams.uiSystemNumberOfStreams] = params.uiStreamID;
                    sysHeaderParams.pSystemSizeBound[sysHeaderParams.uiSystemNumberOfStreams] = params.uiBitRate / 8;
                    sysHeaderParams.uiSystemNumberOfStreams++;
                }

                uiNOfVideoTracks++;
                if (MPEG2MUX_ES_MPEG1_VIDEO == params.esType ||
                    MPEG2MUX_ES_MPEG2_VIDEO == params.esType)
                    uiNOfMPEGVideoTracks++;
            }
            else if (VBI_TRACK == m_pParams->m_pTrackParams[nNum].type)
            { // VBI
                params.esType = MPEG2MUX_ES_VBI_DATA;
                params.uiStreamID = MPEG2MUX_PES_ID_PRIVATE_1;
                params.uiPID = bank.GetUnusedId(IdBank::TS_PID);
                if (params.uiPID == IdBank::NO_ID)
                    return UMC_ERR_INVALID_PARAMS;
                bank.Set(IdBank::TS_PID, params.uiPID);

                m_pPMTTableWriterParams[0].pStreamType[nNum] = params.esType;
                m_pPMTTableWriterParams[0].pPID[nNum] = params.uiPID;
                uiPID++;
            }

            if (IsTS(m_pParams->m_SystemType))
            {
                params.bIsPCRPID = false;
                if (nNum == uiPCRTrackNum)
                {
                    params.bIsPCRPID = true;
                    m_pPMTTableWriterParams[0].uiPCRPID = params.uiPID;
                }
            }

            m_uiTotalRate += params.uiBitRate / 8;
            UMC_CHECK_FUNC(status, m_ppBuffers[nNum]->Init(&params));
        }

        // create chunk writers
        if (MPEG2_PROGRAMM_STREAM == m_pParams->m_SystemType)
        {
            if (uiNOfPrivateAudioTracks > 0)
            {
                sysHeaderParams.pSystemStreamID[sysHeaderParams.uiSystemNumberOfStreams] = MPEG2MUX_PES_ID_PRIVATE_1;
                sysHeaderParams.pSystemSizeBound[sysHeaderParams.uiSystemNumberOfStreams] = uiRateOfPrivateTracks / 8;
            }

            sysHeaderParams.uiSystemRateBound = m_uiTotalRate;
            sysHeaderParams.uiSystemAudioBound = uiNOfMPEGAudioTracks;
            sysHeaderParams.uiSystemVideoBound = uiNOfMPEGVideoTracks;

            for (Ipp32u nNum = 0; nNum < m_uiTotalNumStreams; nNum++)
            {
                MPEG2PSChunkWriter *pPSChunkWriter = (MPEG2PSChunkWriter *)(m_ppBuffers[nNum]);
                UMC_CHECK_FUNC(status, pPSChunkWriter->SetSystemHeaderParams(&sysHeaderParams));
            }

            UMC_FREE(sysHeaderParams.pSystemStreamID);
            UMC_FREE(sysHeaderParams.pSystemSizeBound);
        }
        else if (IsTS(m_pParams->m_SystemType))
        {
            UMC_CHECK_FUNC(status, m_ppPMTTableWriter[0]->Init(&m_pPMTTableWriterParams[0]));
        }
    }
    else
        return UMC_ERR_INVALID_PARAMS;

    // init mutex
    m_synchro.Reset();

    return UMC_OK;
} //Status MPEG2Muxer::Init(MuxerParams* lpInit)

Status MPEG2Muxer::PutEndOfStream(Ipp32s iTrack)
{
    Status umcRes = Muxer::PutEndOfStream(iTrack);
    WriteInterleavedFramesIntoStream(false);
    return umcRes;
} //Status MPEG2Muxer::PutEndOfStream(Ipp32s iTrack)

Status MPEG2Muxer::UnlockBuffer(MediaData *pData, Ipp32s iTrack)
{
    Status umcRes = Muxer::UnlockBuffer(pData, iTrack);
    WriteInterleavedFramesIntoStream(false);
    return umcRes;
} //Status MPEG2Muxer::UnlockBuffer(MediaData *pData, Ipp32s iTrack)

Status MPEG2Muxer::Flush(void)
{
    WriteInterleavedFramesIntoStream(true);
    return UMC_OK;
} //Status MPEG2Muxer::Flush(void)

Status MPEG2Muxer::WriteInterleavedFramesIntoStream(bool bFlushMode)
{
    AutomaticMutex guard(m_synchro);

    Status umcRes = UMC_OK;
    Ipp32s num;

    while (!m_bQuit)
    {
        if (IsPure(m_pParams->m_SystemType))
        {
            UMC_CHECK_FUNC(umcRes, ((MPEG2ChunkWriter *)m_ppBuffers[0])->WriteChunk());
        }
        else
        {
            UMC_CHECK_FUNC(umcRes, GetStreamToWrite(num, bFlushMode));

            Ipp64f dExactTime;
            GetOutputTime(num, dExactTime);

            Ipp64f dReferenceClock;
            if (dExactTime - m_pMPEG2MuxerParams->m_dSystemTimeDelay > m_dReferenceClock)
                dReferenceClock = dExactTime - m_pMPEG2MuxerParams->m_dSystemTimeDelay;
            else
                dReferenceClock = m_dReferenceClock + 1.0 / MPEG2MUX_SYS_CLOCK_FREQ;

            ((MPEG2ChunkWriter *)m_ppBuffers[num])->SetReferenceClock(IsTTS0(m_pParams->m_SystemType) ? 0.0 : dReferenceClock);
            ((MPEG2ChunkWriter *)m_ppBuffers[num])->SetMuxRate(m_uiTotalRate);

            if ((dReferenceClock - m_dReferenceClockOfPrevSI) > 4.9)
            {
                if (IsTS(m_pParams->m_SystemType))
                {
                    Ipp32s i;
                    m_pPATTableWriter->SetReferenceClock(dReferenceClock);
                    UMC_CHECK_FUNC(umcRes, m_pPATTableWriter->WriteChunk());
                    dReferenceClock += 1.0 / MPEG2MUX_SYS_CLOCK_FREQ;

                    for (i = 0; i < m_pPATTableWriterParams->iNOfPrograms; i++)
                    {
                        m_ppPMTTableWriter[i]->SetReferenceClock(dReferenceClock);
                        UMC_CHECK_FUNC(umcRes, m_ppPMTTableWriter[i]->WriteChunk());
                        dReferenceClock += 1.0 / MPEG2MUX_SYS_CLOCK_FREQ;
                    }
                }
                else
                {
                    ((MPEG2PSChunkWriter *)m_ppBuffers[num])->ToggleSystemHeader();
                }

                m_dReferenceClockOfPrevSI = dReferenceClock;
            }

            umcRes = ((MPEG2ChunkWriter *)m_ppBuffers[num])->WriteChunk();
            if (UMC_OK != umcRes)
            {
                return umcRes;
            }

            m_dReferenceClock = dReferenceClock;
        }
    }

    return UMC_OK;
} //Status MPEG2Muxer::WriteInterleavedFramesIntoStream(void)

Status MPEG2Muxer::GetOutputTime(Ipp32s nStreamNumber, Ipp64f &dTime)
{
    return ((MPEG2ChunkWriter *)m_ppBuffers[nStreamNumber])->GetOutputTime(dTime);
} //Status MPEG2Muxer::GetOutputTime(Ipp32u nStreamNumber, Ipp64f &dTime)

Ipp32u GetStreamId(const TrackParams& tPar)
{
    switch (tPar.type)
    {
    case AUDIO_TRACK: return tPar.info.audio->iStreamPID;
    case VIDEO_TRACK: return tPar.info.video->iStreamPID;
    default: return IdBank::NO_ID;
    }
}

IdBank::Type GetIdTypeByEsType(const TrackParams& tPar)
{
    if (tPar.type == AUDIO_TRACK)
    {
        switch (tPar.info.audio->streamType)
        {
        case LPCM_AUDIO:
            return IdBank::LPCM_SUB_ID;
        case AC3_AUDIO:
            return IdBank::AC3_SUB_ID;
        default:
            return IdBank::AUD_ES_ID;
        }
    }
    else // video
    {
        return IdBank::VID_ES_ID;
    }
}

Status SetIdIfEverythingIsOk(IdBank& bank, IdBank::Type type, Ipp32u id)
{
    if (id != IdBank::NO_ID)
    {
        if (!bank.IsValid(type, id))
            return UMC_ERR_INVALID_PARAMS;
        if (bank.IsSet(type, id))
            return UMC_ERR_INVALID_PARAMS;
        bank.Set(type, id);
    }
    return UMC_OK;
}

Status CheckUserIds(IdBank& bank, const MPEG2MuxerParams& par)
{
    // first check IDs assigned by user
    // if user's IDs conflict report an error
    // then assign IDs which user didn't assign (streamId = 0)
    Status status;

    if (IsTS(par.m_SystemType))
    {
        UMC_CHECK_FUNC(status, SetIdIfEverythingIsOk(bank, IdBank::TS_PID, par.m_uiProgramPid));
        for (Ipp32s i = 0; i < par.m_nNumberOfTracks; i++)
        {
            if (!par.m_pTrackParams[i].info.undef)
                return UMC_ERR_NULL_PTR;
            UMC_CHECK_FUNC(status, SetIdIfEverythingIsOk(bank, IdBank::TS_PID, GetStreamId(par.m_pTrackParams[i])));
        }
    }
    else if (IsPSOrPES(par.m_SystemType))
    {
        for (Ipp32s i = 0; i < par.m_nNumberOfTracks; i++)
        {
            const TrackParams& track = par.m_pTrackParams[i];
            if (track.type != AUDIO_TRACK && track.type != VIDEO_TRACK)
                return UMC_ERR_INVALID_PARAMS;
            if (!track.info.undef)
                return UMC_ERR_NULL_PTR;
            UMC_CHECK_FUNC(status, SetIdIfEverythingIsOk(bank, GetIdTypeByEsType(track), GetStreamId(track)));
        }
    }

    return UMC_OK;
}

#endif
