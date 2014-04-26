/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "vm_debug.h"
#include "umc_muxer.h"
#include "ippcore.h"
#include "ipps.h"

using namespace UMC;


Status MuxerParams::Close(void)
{
    Ipp32s i;

    if (!m_bAllocated)
        return UMC_OK;

    for (i = 0; i < m_nNumberOfTracks; i++)
    {
        if (m_pTrackParams[i].type == VIDEO_TRACK)
        {
            UMC_DELETE(m_pTrackParams[i].info.video);
        }
        else if (m_pTrackParams[i].type == AUDIO_TRACK)
        {
            UMC_DELETE(m_pTrackParams[i].info.audio);
        }
    }

    UMC_DELETE_ARR(m_pTrackParams);
    m_bAllocated = false;

    return UMC_OK;
}


Status MuxerParams::operator=(MuxerParams& p)
{
    Ipp32s i;

    MuxerParams::Close();

    m_SystemType      = p.m_SystemType;
    m_lFlags          = p.m_lFlags;
    m_nNumberOfTracks = p.m_nNumberOfTracks;
    m_lpDataWriter    = p.m_lpDataWriter;

    m_pTrackParams = NULL;
    UMC_CHECK_PTR(p.m_pTrackParams);
    UMC_CHECK(m_nNumberOfTracks > 0, UMC_OK);

    UMC_NEW_ARR(m_pTrackParams, TrackParams, m_nNumberOfTracks);

    for (i = 0; i < m_nNumberOfTracks; i++)
    {
        m_pTrackParams[i]            = p.m_pTrackParams[i];
        m_pTrackParams[i].info.undef = NULL;

        if (m_pTrackParams[i].type == VIDEO_TRACK)
        {
            UMC_NEW(m_pTrackParams[i].info.video, VideoStreamInfo);
            *m_pTrackParams[i].info.video = *p.m_pTrackParams[i].info.video;
        }
        else if (m_pTrackParams[i].type == AUDIO_TRACK)
        {
            UMC_NEW(m_pTrackParams[i].info.audio, AudioStreamInfo);
            *m_pTrackParams[i].info.audio = *p.m_pTrackParams[i].info.audio;
        }
    }

    m_bAllocated = true;

    return UMC_OK;
}

/******************************************************************************/

Muxer::Muxer(void)
{
    m_pParams           = NULL;
    m_ppBuffers         = NULL;
    m_uiTotalNumStreams = 0;
    m_pTrackParams      = NULL;
}


Muxer::~Muxer(void)
{
    Muxer::Close();
}


Status Muxer::CopyMuxerParams(MuxerParams* lpInit)
{
    Ipp32u i;

    UMC_CHECK_PTR(lpInit);

    if (!m_pParams)
    {
        UMC_NEW(m_pParams, MuxerParams);
    }

    m_uiTotalNumStreams = lpInit->m_nNumberOfTracks;
    //  UMC_CHECK(m_uiTotalNumStreams >= 0, UMC_ERR_INVALID_PARAMS);

    *m_pParams = *lpInit; // via operator= !!!
    m_pTrackParams = m_pParams->m_pTrackParams; // copy of pointer, don't delete!

    // check MediaBufferParams
    for (i = 0; i < m_uiTotalNumStreams; i++)
    {
        UMC_CHECK_PTR(lpInit->m_pTrackParams[i].info.undef);

        if (!m_pTrackParams[i].bufferParams.m_prefInputBufferSize)
        {
            Ipp32s bitrate = 4000000;
            if (m_pTrackParams[i].type == VIDEO_TRACK)
            {
                bitrate = m_pTrackParams[i].info.video->iBitrate;
                if (0 == bitrate)
                {
                    bitrate = 4000000;
                }
            }
            else if (m_pTrackParams[i].type == AUDIO_TRACK)
            {
                bitrate = m_pTrackParams[i].info.audio->iBitrate;
                if (0 == bitrate)
                {
                    bitrate = 100000;
                }
            }
            else
                return UMC_ERR_INVALID_PARAMS;

            m_pTrackParams[i].bufferParams.m_prefInputBufferSize = bitrate >> 3; /* 1 sec in bytes */
        }

        if (!m_pTrackParams[i].bufferParams.m_prefOutputBufferSize)
            m_pTrackParams[i].bufferParams.m_prefOutputBufferSize = m_pTrackParams[i].bufferParams.m_prefInputBufferSize;

        if (!m_pTrackParams[i].bufferParams.m_numberOfFrames)
            m_pTrackParams[i].bufferParams.m_numberOfFrames = 5;
    }

    // Alloc pointers to MediaBuffer (and set to NULL)
    UMC_ALLOC_ZERO_ARR(m_ppBuffers, MediaBuffer*, m_uiTotalNumStreams);

    return UMC_OK;
}


Status Muxer::Close(void)
{
    if (m_ppBuffers)
    {
        Ipp32u i;
        for (i = 0; i < m_uiTotalNumStreams; i++)
        {
            UMC_DELETE(m_ppBuffers[i]);
        }
        UMC_FREE(m_ppBuffers);
    }

    UMC_DELETE(m_pParams);

    return UMC_OK;
}


Ipp32s Muxer::GetTrackIndex(MuxerTrackType type, Ipp32s index)
{
    Ipp32u i;

    for (i = 0; i < m_uiTotalNumStreams; i++)
    {
        if (m_pTrackParams[i].type == type)
        {
            if (index <= 0)
                return (Ipp32s)i;
            index--;
        }
    }

    return -1;
}


Status Muxer::LockBuffer(MediaData* lpData, Ipp32s iTrack)
{
    Status status;

    UMC_CHECK_PTR(lpData);
    UMC_CHECK(iTrack >= 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK((Ipp32u)iTrack < m_uiTotalNumStreams, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(m_ppBuffers, UMC_ERR_NOT_INITIALIZED);

    UMC_CHECK_FUNC(status, m_ppBuffers[iTrack]->LockInputBuffer(lpData));
    return UMC_OK;
} // Muxer::LockBuffer()


Status Muxer::UnlockBuffer(MediaData* lpData, Ipp32s iTrack)
{
    Status status;

    UMC_CHECK_PTR(lpData);
    UMC_CHECK(iTrack >= 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK((Ipp32u)iTrack < m_uiTotalNumStreams, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK(m_ppBuffers, UMC_ERR_NOT_INITIALIZED);

    UMC_CHECK_FUNC(status, m_ppBuffers[iTrack]->UnLockInputBuffer(lpData));
    return UMC_OK;
} // Muxer::UnlockBuffer()


Status Muxer::PutData(MediaData* lpData, Ipp32s iTrack)
{
    Status status;
    MediaData data;

    UMC_CHECK_FUNC(status, LockBuffer(&data, iTrack));

    // copy data
    UMC_CHECK(lpData->GetDataSize() <= data.GetBufferSize(), UMC_ERR_NOT_ENOUGH_BUFFER);
    ippsCopy_8u((Ipp8u*)lpData->GetDataPointer(), (Ipp8u*)data.GetDataPointer(), (Ipp32s)lpData->GetDataSize());

    // copy time & frame type
    data.m_fPTSStart = lpData->m_fPTSStart;
    data.m_fPTSEnd   = lpData->m_fPTSEnd;
    data.m_frameType = lpData->m_frameType;
    data.SetDataSize(lpData->GetDataSize());

    UMC_CHECK_FUNC(status, UnlockBuffer(&data, iTrack));
    return UMC_OK;
} // Muxer::PutData()


Status Muxer::PutEndOfStream(Ipp32s iTrack)
{
    Status status;

    UMC_CHECK(m_ppBuffers, UMC_ERR_NOT_INITIALIZED);
    UMC_CHECK(iTrack >= 0, UMC_ERR_INVALID_PARAMS);
    UMC_CHECK((Ipp32u)iTrack < m_uiTotalNumStreams, UMC_ERR_INVALID_PARAMS);

    UMC_CHECK_FUNC(status, m_ppBuffers[iTrack]->UnLockInputBuffer(NULL, UMC_ERR_END_OF_STREAM));
    return UMC_OK;
} // Muxer::PutEndOfStream()


Status Muxer::PutVideoData(MediaData* lpData, Ipp32s index)
{
    Status status;
    UMC_CHECK_FUNC(status, PutData(lpData, GetTrackIndex(VIDEO_TRACK, index)));
    return UMC_OK;
} // Muxer::PutVideoData()


Status Muxer::PutAudioData(MediaData* lpData, Ipp32s index)
{
    Status status;
    UMC_CHECK_FUNC(status, PutData(lpData, GetTrackIndex(AUDIO_TRACK, index)));
    return UMC_OK;
} // Muxer::PutAudioData()


Status Muxer::GetStreamToWrite(Ipp32s& rStreamNumber, bool bFlushMode)
{
    static const Ipp64f MAXIMUM_DOUBLE = 1.7E+308;

    Status umcRes;
    Ipp32u streamNum, minNum = 0;
    Ipp64f streamTime, minTime = MAXIMUM_DOUBLE;

    for (streamNum = 0; streamNum < (Ipp32u)m_uiTotalNumStreams; streamNum++)
    {
        umcRes = GetOutputTime(streamNum, streamTime);
        if (UMC_ERR_NOT_ENOUGH_DATA == umcRes && !bFlushMode)
            return umcRes;

        if (UMC_OK == umcRes)
        {
            if (streamTime < minTime)
            {
                minNum = streamNum;
                minTime = streamTime;
            }
        }
    }

    // no more data in buffers
    if (minTime >= MAXIMUM_DOUBLE)
        return UMC_ERR_END_OF_STREAM;

    rStreamNumber = minNum;

    return UMC_OK;
} // Muxer::GetStreamToWrite()
