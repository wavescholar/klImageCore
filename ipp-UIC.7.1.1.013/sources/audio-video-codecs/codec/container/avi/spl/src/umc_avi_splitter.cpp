/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_AVI_SPLITTER

#include "vm_debug.h"
#include "vm_time.h"
#include "umc_media_data.h"
#include "umc_sample_buffer.h"
#include "umc_avi_splitter.h"

#include "ippcore.h"
#include "ipps.h"

using namespace UMC;


AVISplitter::AVISplitter()
{
    m_pTrack = NULL;
    m_pDuration = NULL;
    ippsSet_8u(0, (Ipp8u *)&m_AviHdr, sizeof(MainAviHeader));
    m_bCheckVBRAudio = false;
    m_bIsVBRAudio = false;
}

AVISplitter::~AVISplitter()
{
    Close();
}

Status AVISplitter::Init(SplitterParams *pInitParams)
{
    if (m_pReader) // already initialized
        return UMC_ERR_FAILED;

    if(!pInitParams)
        return UMC_ERR_NULL_PTR;

    Status umcRes = UMC_OK;
    if (!pInitParams->m_pDataReader)
        return UMC_ERR_NULL_PTR;

    umcRes = IndexSplitter::Init(pInitParams);
    if (UMC_OK != umcRes) return TerminateInit(umcRes);

    m_uiFlags = pInitParams->m_iFlags;
    m_pDataReader = pInitParams->m_pDataReader;
    umcRes = m_AviChunkReader.Init(m_pDataReader, &m_ReaderMutex);
    if (UMC_OK != umcRes) return TerminateInit(umcRes);

    umcRes = ReadFormat();
    if (UMC_OK != umcRes) return TerminateInit(umcRes);

    m_pTrackIndex = new TrackIndex[m_AviHdr.uiStreams];
    m_pReadESThread = new vm_thread[m_AviHdr.uiStreams];
    if (!m_pTrackIndex || !m_pReadESThread) return TerminateInit(UMC_ERR_ALLOC);

    m_ppMediaBuffer = (MediaBuffer **)new SampleBuffer*[m_AviHdr.uiStreams];
    if (!m_ppMediaBuffer) return TerminateInit(UMC_ERR_ALLOC);
    ippsSet_8u(0, (Ipp8u *)m_ppMediaBuffer, m_AviHdr.uiStreams * sizeof(MediaBuffer *));

    umcRes = GenerateIndex();
    if (UMC_OK != umcRes) return TerminateInit(umcRes);

    umcRes = FillSplitterInfo();
    if (UMC_OK != umcRes) return TerminateInit(umcRes);

    m_ppLockedFrame = new MediaData*[m_AviHdr.uiStreams];
    if (!m_ppLockedFrame) return TerminateInit(UMC_ERR_ALLOC);
    m_pIsLocked = new Ipp32s[m_AviHdr.uiStreams];
    if (!m_pIsLocked) return TerminateInit(UMC_ERR_ALLOC);

    Ipp32s i;
    IndexEntry entry;
    for (i = 0; i < (Ipp32s)m_AviHdr.uiStreams; i++)
    {
        m_pTrackIndex[i].First(entry);
        m_ppLockedFrame[i] = new MediaData;
        if (!m_ppLockedFrame[i]) return TerminateInit(UMC_ERR_ALLOC);
        m_pIsLocked[i] = 0;
        vm_thread_set_invalid(&m_pReadESThread[i]);
    }

    umcRes = Run();
    if (UMC_OK != umcRes) return TerminateInit(umcRes);
    return UMC_OK;
}

Status AVISplitter::Run(void)
{
    if (!m_pDataReader)
        return UMC_ERR_NOT_INITIALIZED;

    if (!m_bFlagStop)
        return UMC_OK;

    Ipp32u i;
    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        if (!m_ppMediaBuffer[i])
        {
            MediaBufferParams params;
            params.m_prefInputBufferSize = m_pTrack[i].m_uiMaxSampleSize;
            params.m_prefOutputBufferSize = m_pTrack[i].m_uiMaxSampleSize;
            params.m_numberOfFrames = 25;
            m_ppMediaBuffer[i] = new SampleBuffer;
            if (!m_ppMediaBuffer[i])
                return UMC_ERR_ALLOC;

            Status umcRes = m_ppMediaBuffer[i]->Init(&params);
            if (UMC_OK != umcRes)
                return umcRes;
        }
    }

    Status umcRes = IndexSplitter::Run();
    if (UMC_OK != umcRes)
        return umcRes;

    for (i = 0; i < m_pInfo->m_iTracks; i++)
    {
        MediaData data;
        if (((m_pInfo->m_fRate == 1.0) ||
            (m_pInfo->m_ppTrackInfo[i]->m_type & TRACK_ANY_VIDEO)) &&
            (m_pInfo->m_ppTrackInfo[i]->m_bEnabled))
            while (!m_bFlagStop && UMC_ERR_NOT_ENOUGH_DATA == (umcRes = CheckNextData(&data, i)));
        if (UMC_OK != umcRes)
            return umcRes;
    }

    return UMC_OK;
}

Status AVISplitter::TerminateInit(Status umcRes)
{
    Ipp32u i;
    for (i = 0; i < m_AviHdr.uiStreams; i++)
    {
        if (m_ppMediaBuffer)
            delete m_ppMediaBuffer[i];
        if (m_ppLockedFrame)
            delete m_ppLockedFrame[i];
        if (m_pInfo && m_pInfo->m_ppTrackInfo && m_pInfo->m_ppTrackInfo[i])
        {
            delete m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo;
            delete m_pInfo->m_ppTrackInfo[i]->m_pHeader;
        }
        if (m_pTrack && m_pTrack[i].m_pStreamFormat)
            ippFree(m_pTrack[i].m_pStreamFormat);
        if (m_pTrack && m_pTrack[i].m_pIndex)
            ippFree(m_pTrack[i].m_pIndex);
    }
    delete[] m_ppMediaBuffer;
    m_ppMediaBuffer = NULL;
    delete[] m_ppLockedFrame;
    m_ppLockedFrame = NULL;
    delete[] m_pIsLocked;
    m_pIsLocked = NULL;
    delete[] m_pTrackIndex;
    m_pTrackIndex = NULL;
    delete[] m_pReadESThread;
    m_pReadESThread = NULL;
    delete[] m_ppMediaBuffer;
    m_ppMediaBuffer = NULL;
    delete[] m_pTrack;
    m_pTrack = NULL;
    if (m_pInfo)
        delete[] m_pInfo->m_ppTrackInfo;
    delete m_pInfo;
    m_pInfo = NULL;
    m_pReader = NULL;
    return umcRes;
}

Status AVISplitter::FillSplitterInfo(void)
{
    Status umcRes = UMC_OK;
    m_pInfo = new SplitterInfo;
    if (!m_pInfo)
        return UMC_ERR_ALLOC;

    m_pInfo->m_ppTrackInfo = new TrackInfo*[m_AviHdr.uiStreams];
    if (!m_pInfo->m_ppTrackInfo)
        return UMC_ERR_ALLOC;

    m_pInfo->m_fDuration = 0.0;
    m_pInfo->m_systemType = AVI_STREAM;
    m_pInfo->m_iFlags = m_uiFlags;
    m_pInfo->m_iTracks = m_AviHdr.uiStreams;

    Ipp32u i;
    for (i = 0; i < m_AviHdr.uiStreams; i++)
    {
        m_pInfo->m_ppTrackInfo[i] = new TrackInfo;
        if (!m_pInfo->m_ppTrackInfo[i])
            return UMC_ERR_ALLOC;

        m_pInfo->m_ppTrackInfo[i]->m_bEnabled = true;
        m_pInfo->m_ppTrackInfo[i]->m_iPID = i;

        if (AVI_FOURCC_auds == m_pTrack[i].m_StreamHeader.fccType)
        {
            m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo = new AudioStreamInfo();
            if (!m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo)
                return UMC_ERR_ALLOC;

            umcRes = FillAudioStreamInfo(i, m_pInfo->m_ppTrackInfo[i]);
            if (UMC_OK != umcRes)
                return umcRes;

            m_pInfo->m_fDuration = IPP_MAX(m_pInfo->m_fDuration, ((AudioStreamInfo *)m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo)->fDuration);
        }
        else if (AVI_FOURCC_vids == m_pTrack[i].m_StreamHeader.fccType ||
                 AVI_FOURCC_iavs == m_pTrack[i].m_StreamHeader.fccType ||
                 AVI_FOURCC_ivas == m_pTrack[i].m_StreamHeader.fccType)
        {
            m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo = new VideoStreamInfo();
            if (!m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo)
                return UMC_ERR_ALLOC;

            if (AVI_FOURCC_vids == m_pTrack[i].m_StreamHeader.fccType)
                umcRes = FillVideoStreamInfo(i, m_pInfo->m_ppTrackInfo[i]);
            else
                umcRes = FillDvStreamInfo(i, m_pInfo->m_ppTrackInfo[i]);
            if (UMC_OK != umcRes)
                return umcRes;

            m_pInfo->m_fDuration = IPP_MAX(m_pInfo->m_fDuration, ((VideoStreamInfo *)m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo)->fDuration);
        }
    }

    return UMC_OK;
}

Status AVISplitter::Close()
{
    IndexSplitter::Close();

    UMC_DELETE_ARR(m_pReadESThread);
    UMC_DELETE_ARR(m_pDuration);

    Ipp32u i;
    for (i = 0; i < m_AviHdr.uiStreams; i++)
    {
        UMC_DELETE(m_ppMediaBuffer[i]);
        UMC_DELETE(m_ppLockedFrame[i]);
        if (m_pInfo && m_pInfo->m_ppTrackInfo[i])
        {
            UMC_DELETE(m_pInfo->m_ppTrackInfo[i]->m_pStreamInfo);
            UMC_DELETE(m_pInfo->m_ppTrackInfo[i]->m_pHeader);
            UMC_DELETE(m_pInfo->m_ppTrackInfo[i]);
        }
        if (m_pTrack)
        {
            UMC_FREE(m_pTrack[i].m_pStreamFormat)
            UMC_FREE(m_pTrack[i].m_pIndex)
        }
    }

    UMC_DELETE_ARR(m_pTrackIndex);
    UMC_DELETE_ARR(m_ppMediaBuffer);
    UMC_DELETE_ARR(m_ppLockedFrame);
    UMC_DELETE_ARR(m_pIsLocked);

    if (m_pInfo)
    {
        UMC_DELETE_ARR(m_pInfo->m_ppTrackInfo);
        UMC_DELETE(m_pInfo);
    }

    UMC_DELETE_ARR(m_pTrack);

    memset(&m_AviHdr, 0, sizeof(MainAviHeader));
    m_bCheckVBRAudio = false;
    m_bIsVBRAudio = false;
    return UMC_OK;
}

Status AVISplitter::ReadFormat()
{
    m_pDataReader->Reset();
    Status umcRes = m_AviChunkReader.DescendRIFF(AVI_FOURCC_AVI_);
    if (UMC_OK != umcRes)
        return umcRes;

    umcRes = m_AviChunkReader.DescendLIST(AVI_FOURCC_HDRL);
    if (UMC_OK != umcRes)
        return umcRes;

    umcRes = m_AviChunkReader.DescendChunk(AVI_FOURCC_AVIH);
    if (UMC_OK != umcRes)
        return umcRes;

    umcRes = m_AviChunkReader.GetData((Ipp8u *)&m_AviHdr, sizeof(MainAviHeader));
    if (UMC_OK != umcRes)
        return umcRes;

    // ascend from avih
    m_AviHdr.SwapBigEndian();
    umcRes = m_AviChunkReader.Ascend();
    if (UMC_OK != umcRes)
        return umcRes;

    m_pTrack = new AviTrack[m_AviHdr.uiStreams];
    if (!m_pTrack)
        return UMC_ERR_ALLOC;

    Ipp32u i;
    for (i = 0; i < m_AviHdr.uiStreams; i++)
    {
        umcRes = ReadStreamsInfo(i);
        if (UMC_OK != umcRes)
            return umcRes;
    }

    //  Ascend from hdrl
    umcRes = m_AviChunkReader.Ascend();
    if (UMC_OK != umcRes)
        return umcRes;

    //  Ascend from avi_
    umcRes = m_AviChunkReader.Ascend();
    if (UMC_OK != umcRes)
        return umcRes;

    // go to the beginning of file
    m_AviChunkReader.GoChunkHead();
    return UMC_OK;
}

Status AVISplitter::ReadStreamsInfo(Ipp32u uiTrack)
{
    AviTrack &track = m_pTrack[uiTrack];
    Status umcRes = m_AviChunkReader.DescendLIST(AVI_FOURCC_STRL);

    umcRes = m_AviChunkReader.DescendChunk(AVI_FOURCC_STRH);
    if (UMC_OK != umcRes)
        return umcRes;

    umcRes = m_AviChunkReader.GetData((Ipp8u *)&track.m_StreamHeader, sizeof(AviSplStreamHeader));
    if (UMC_OK != umcRes)
        return umcRes;
    track.m_StreamHeader.SwapBigEndian();

    umcRes = m_AviChunkReader.Ascend();
    if (UMC_OK != umcRes)
        return umcRes;

    umcRes = m_AviChunkReader.DescendChunk(AVI_FOURCC_STRF);
    if (UMC_OK != umcRes)
        return umcRes;

    // reader stream format
    track.m_uiStreamFormatSize = m_AviChunkReader.GetChunkSize();
    track.m_pStreamFormat = (Ipp8u*)ippMalloc(track.m_uiStreamFormatSize);
    if (!track.m_pStreamFormat)
        return UMC_ERR_ALLOC;
    umcRes = m_AviChunkReader.GetData((Ipp8u *)track.m_pStreamFormat, track.m_uiStreamFormatSize);
    if (UMC_OK != umcRes)
        return umcRes;

    if (AVI_FOURCC_vids == track.m_StreamHeader.fccType)
        ((BitmapInfoHeader *)track.m_pStreamFormat)->SwapBigEndian();
    else if (AVI_FOURCC_auds == track.m_StreamHeader.fccType)
        ((WaveFormatEx *)track.m_pStreamFormat)->SwapBigEndian();

    //  Ascend from 'strf'
    umcRes = m_AviChunkReader.Ascend();
    if (UMC_OK != umcRes)
        return umcRes;

    umcRes = m_AviChunkReader.DescendChunk(AVI_FOURCC_INDX);
    if (umcRes == UMC_OK)
    {
        // index buffer doesn't include chunk header!
        track.m_uiIndexSize = m_AviChunkReader.GetChunkSize();
        track.m_pIndex = (Ipp8u*)ippMalloc(track.m_uiIndexSize);
        if (!track.m_pIndex)
            return UMC_ERR_ALLOC;
        m_AviChunkReader.GetData(track.m_pIndex, track.m_uiIndexSize);
        if (UMC_OK != umcRes)
            return umcRes;

        //  Ascend from 'indx'
        umcRes = m_AviChunkReader.Ascend();
    }

    //  Ascend from LIST
    umcRes = m_AviChunkReader.Ascend();
    if (UMC_OK != umcRes)
        return umcRes;

    return UMC_OK;
}

Status AVISplitter::FillAudioStreamInfo(Ipp32u uiTrack, TrackInfo *pInfo)
{
    AviTrack &rTrack = m_pTrack[uiTrack];
    AudioStreamInfo *pAInfo = (AudioStreamInfo *)pInfo->m_pStreamInfo;
    WaveFormatEx *pWavFmt = (WaveFormatEx *)rTrack.m_pStreamFormat;
    if (!pWavFmt)
        return UMC_ERR_FAILED;

    pAInfo->audioInfo.m_iChannels = pWavFmt->nChannels;
    pAInfo->audioInfo.m_iSampleFrequency = pWavFmt->nSamplesPerSec;
    pAInfo->iBitrate = pWavFmt->nAvgBytesPerSec * 8;
    pAInfo->audioInfo.m_iBitPerSample = pWavFmt->wBitsPerSample ? pWavFmt->wBitsPerSample: 16;
    pAInfo->fDuration = m_pDuration[uiTrack];
    pAInfo->streamType = UNDEF_AUDIO;
    pAInfo->streamSubtype = UNDEF_AUDIO_SUBTYPE;
    if (WAVE_FORMAT_PCM == pWavFmt->wFormatTag)
    {
        pAInfo->streamType = PCM_AUDIO;
        pInfo->m_type = TRACK_PCM;
    }
    else if (WAVE_FORMAT_MPEGLAYER3 == pWavFmt->wFormatTag)
    {
        pAInfo->streamType = (pWavFmt->nSamplesPerSec >= 32000) ? MP1L3_AUDIO : MP2L3_AUDIO;
        pInfo->m_type = TRACK_MPEGA;
    }
    else if (WAVE_FORMAT_DVM == pWavFmt->wFormatTag)
    {
        pAInfo->streamType = AC3_AUDIO;
        pInfo->m_type = TRACK_AC3;
    }
    else if (WAVE_FORMAT_AAC == pWavFmt->wFormatTag)
    {
        pAInfo->streamType = AAC_AUDIO;
        pInfo->m_type = TRACK_AAC;
    }
    else if (WAVE_FORMAT_ALAW == pWavFmt->wFormatTag)
    {
        pAInfo->streamType = ALAW_AUDIO;
        pInfo->m_type = TRACK_AMR;
    }
    else if (WAVE_FORMAT_MULAW == pWavFmt->wFormatTag)
    {
        pAInfo->streamType = MULAW_AUDIO;
        pInfo->m_type = TRACK_AMR;
    }

    pAInfo->audioInfo.m_iChannelMask = 0;
    if (1 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_CENTER;
    else if (2 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT;
    else if (3 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER;
    else if (4 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER | CHANNEL_TOP_CENTER;
    else if (5 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
            CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT;
    else if (6 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER |
            CHANNEL_BACK_LEFT | CHANNEL_BACK_RIGHT | CHANNEL_LOW_FREQUENCY;
    else if (7 == pWavFmt->nChannels)
        pAInfo->audioInfo.m_iChannelMask = CHANNEL_FRONT_LEFT | CHANNEL_FRONT_RIGHT | CHANNEL_FRONT_CENTER | CHANNEL_BACK_LEFT |
            CHANNEL_BACK_RIGHT | CHANNEL_LOW_FREQUENCY | CHANNEL_FRONT_LEFT_OF_CENTER | CHANNEL_FRONT_RIGHT_OF_CENTER;

    return UMC_OK;
}

Status AVISplitter::FillVideoStreamInfo(Ipp32u uiTrack, TrackInfo *pInfo)
{
    AviTrack &rTrack = m_pTrack[uiTrack];
    VideoStreamInfo *pVInfo = (VideoStreamInfo *)pInfo->m_pStreamInfo;
    BitmapInfoHeader* pBmpHdr = (BitmapInfoHeader *)rTrack.m_pStreamFormat;
    if (!pBmpHdr)
        return UMC_ERR_FAILED;

    pVInfo->videoInfo.m_iWidth = pBmpHdr->biWidth;
    pVInfo->videoInfo.m_iHeight = pBmpHdr->biHeight;
    pVInfo->iBitrate = 8 * m_AviHdr.uiMaxBytesPerSec;
    pVInfo->fFramerate = (Ipp64f)rTrack.m_StreamHeader.uiRate / rTrack.m_StreamHeader.uiScale;
    pVInfo->fDuration = m_pDuration[uiTrack];
    pVInfo->streamType = UNDEF_VIDEO;
    pVInfo->streamSubtype = UNDEF_VIDEO_SUBTYPE;
    Ipp32u fccHandler = rTrack.m_StreamHeader.fccHandler;
    if (fccHandler == 0)
        fccHandler = pBmpHdr->biCompression;
    if (AVI_FOURCC_DIVX == fccHandler || AVI_FOURCC_divx == fccHandler ||
        AVI_FOURCC_DX50 == fccHandler || AVI_FOURCC_dx50 == fccHandler ||
        AVI_FOURCC_DIV3 == fccHandler || AVI_FOURCC_div3 == fccHandler ||
        AVI_FOURCC_DIV4 == fccHandler || AVI_FOURCC_div4 == fccHandler ||
        AVI_FOURCC_DIV5 == fccHandler || AVI_FOURCC_div5 == fccHandler ||
        AVI_FOURCC_XVID == fccHandler || AVI_FOURCC_xvid == fccHandler ||
        AVI_FOURCC_MP4V == fccHandler || AVI_FOURCC_mp4v == fccHandler)
    {
        pInfo->m_type = TRACK_MPEG4V;
        pVInfo->streamType = MPEG4_VIDEO;
        if (AVI_FOURCC_DX50 == fccHandler || AVI_FOURCC_dx50 == fccHandler)
            pVInfo->streamSubtype = MPEG4_VIDEO_DIVX5;
    }
    else if (AVI_FOURCC_DVHP == fccHandler || AVI_FOURCC_dvhp == fccHandler ||
             AVI_FOURCC_DVHD == fccHandler || AVI_FOURCC_dvhd == fccHandler ||
             AVI_FOURCC_DVH1 == fccHandler || AVI_FOURCC_dvh1 == fccHandler)
    {
        pVInfo->streamType = DIGITAL_VIDEO_HD;
        pVInfo->videoInfo.m_colorFormat = YUY2;
        pInfo->m_type = TRACK_DVHD;
    }
    else if (AVI_FOURCC_DV50 == fccHandler || AVI_FOURCC_dv50 == fccHandler)
    {
        pVInfo->streamType = DIGITAL_VIDEO_50;
        pVInfo->videoInfo.m_colorFormat = YUY2;
        pInfo->m_type = TRACK_DV50;
    }
    else if (AVI_FOURCC_DVSD == fccHandler || AVI_FOURCC_dvsd == fccHandler ||
             AVI_FOURCC_DSVD == fccHandler || AVI_FOURCC_dsvd == fccHandler ||
             AVI_FOURCC_VIDV == fccHandler || AVI_FOURCC_vidv == fccHandler ||
             AVI_FOURCC_DVSL == fccHandler || AVI_FOURCC_dvsl == fccHandler ||
             AVI_FOURCC_SLDV == fccHandler || AVI_FOURCC_sldv == fccHandler)
    {
        pInfo->m_type = TRACK_DVSD;
        pVInfo->videoInfo.m_colorFormat = YUY2;
        pVInfo->streamType = DIGITAL_VIDEO_SD;
    }
    else if (AVI_FOURCC_WMV3 == fccHandler)
    {
        pInfo->m_type = TRACK_WMV;
        pVInfo->streamType = WMV_VIDEO;
    }
    else if (AVI_FOURCC_H263 == fccHandler || AVI_FOURCC_h263 == fccHandler ||
             AVI_FOURCC_I263 == fccHandler || AVI_FOURCC_i263 == fccHandler ||
             AVI_FOURCC_M263 == fccHandler || AVI_FOURCC_m263 == fccHandler ||
             AVI_FOURCC_VIVO == fccHandler || AVI_FOURCC_vivo == fccHandler)
    {
        pInfo->m_type = TRACK_H263;
        pVInfo->streamType = H263_VIDEO;
    }
    else if (AVI_FOURCC_H261 == fccHandler || AVI_FOURCC_h261 == fccHandler)
    {
        pInfo->m_type = TRACK_H261;
        pVInfo->streamType = H261_VIDEO;
    }
    else if (AVI_FOURCC_MJPG == fccHandler || AVI_FOURCC_mjpg == fccHandler ||
             AVI_FOURCC_MJPX == fccHandler || AVI_FOURCC_mjpx == fccHandler ||
             AVI_FOURCC_dmb1 == fccHandler)
    {
        pInfo->m_type = TRACK_MJPEG;
        pVInfo->streamType = MJPEG_VIDEO;
    }
    else if (AVI_FOURCC_ILV4 == fccHandler || AVI_FOURCC_ilv4 == fccHandler ||
             AVI_FOURCC_VSSH == fccHandler || AVI_FOURCC_vssh == fccHandler ||
             AVI_FOURCC_H264 == fccHandler)
    {
        pInfo->m_type = TRACK_H264;
        pVInfo->streamType = H264_VIDEO;
    }
    else if (AVI_FOURCC_DIB == fccHandler)
    {
        Ipp32u biCompression = ((BitmapInfoHeader *)rTrack.m_pStreamFormat)->biCompression;
        pVInfo->streamType = UNCOMPRESSED_VIDEO;
        pVInfo->streamSubtype = (VideoStreamSubType)biCompression;
        pInfo->m_type = TRACK_YUV;
    }
    else
    {
        // There are many codecs, which can produce uncompressed video.
        // So we compare not a codec handler, but the compression type.
        Ipp32u biCompression = ((BitmapInfoHeader *)rTrack.m_pStreamFormat)->biCompression;
        if (AVI_FOURCC_IYUV == biCompression)
        {
            pInfo->m_type = TRACK_YUV;
            pVInfo->streamType = UNCOMPRESSED_VIDEO;
            pVInfo->streamSubtype = (VideoStreamSubType)biCompression;
        }
    }

    return UMC_OK;
}

Status AVISplitter::FillDvStreamInfo(Ipp32u uiTrack, TrackInfo *pInfo)
{
    Status umcRes = UMC_OK;
    AviTrack &rTrack = m_pTrack[uiTrack];

    VideoStreamInfo *pVInfo = (VideoStreamInfo *)pInfo->m_pStreamInfo;

    IndexEntry entry;
    umcRes = m_pTrackIndex[uiTrack].Last(entry);
    pVInfo->fDuration = (UMC_OK == umcRes) ? entry.GetTimeStamp() : 0.0;
    pVInfo->iBitrate = 8 * m_AviHdr.uiMaxBytesPerSec;
    pVInfo->streamType = UNDEF_VIDEO;
    pVInfo->streamSubtype = UNDEF_VIDEO_SUBTYPE;
    Ipp32u fccHandler = rTrack.m_StreamHeader.fccHandler;
    if (AVI_FOURCC_DVHP == fccHandler || AVI_FOURCC_dvhp == fccHandler ||
        AVI_FOURCC_DVHD == fccHandler || AVI_FOURCC_dvhd == fccHandler ||
        AVI_FOURCC_DVH1 == fccHandler || AVI_FOURCC_dvh1 == fccHandler)
    {
        pVInfo->streamType = DIGITAL_VIDEO_HD;
        pInfo->m_type = TRACK_DVHD;
    }
    else if (AVI_FOURCC_DV50 == fccHandler || AVI_FOURCC_dv50 == fccHandler)
    {
        pVInfo->streamType = DIGITAL_VIDEO_50;
        pInfo->m_type = TRACK_DV50;
    }
    else if (AVI_FOURCC_DVSD == fccHandler || AVI_FOURCC_dvsd == fccHandler ||
             AVI_FOURCC_DSVD == fccHandler || AVI_FOURCC_dsvd == fccHandler ||
             AVI_FOURCC_VIDV == fccHandler || AVI_FOURCC_vidv == fccHandler ||
             AVI_FOURCC_DVSL == fccHandler || AVI_FOURCC_dvsl == fccHandler ||
             AVI_FOURCC_SLDV == fccHandler || AVI_FOURCC_sldv == fccHandler)
    {
        pInfo->m_type = TRACK_DVSD;
        pVInfo->streamType = DIGITAL_VIDEO_SD;
    }
    else
    {
        return UMC_ERR_FAILED;
    }

    Ipp32u uiDVVAuxSrc = ((DvInfo *)rTrack.m_pStreamFormat)->uiDVVAuxSrc;
    Ipp32u uiFieldsNum = (uiDVVAuxSrc >> 21) & 0x1;
    Ipp32u uiSType = (uiDVVAuxSrc >> 16) & 0x1f;
    switch (uiSType)
    {
    case 0: case 1: case 4:
        pVInfo->videoInfo.m_iWidth = 720;
        pVInfo->videoInfo.m_iHeight = (0 == uiFieldsNum) ? 480 : 576;
        pVInfo->fFramerate = (0 == uiFieldsNum) ? 29.97 : 25.00;
        break;
    case 20:
        pVInfo->videoInfo.m_iWidth = 1920;
        pVInfo->videoInfo.m_iHeight = 1080;
        pVInfo->fFramerate = (0 == uiFieldsNum) ? 29.97 : 25.00;
        break;
    case 24:
        pVInfo->videoInfo.m_iWidth = 1280;
        pVInfo->videoInfo.m_iHeight = 720;
        pVInfo->fFramerate = 59.94;
        break;
    default:
        pVInfo->fFramerate = (Ipp64f)rTrack.m_StreamHeader.uiRate / rTrack.m_StreamHeader.uiScale;
        pVInfo->videoInfo.m_iWidth = rTrack.m_StreamHeader.rcFrame.iRight - rTrack.m_StreamHeader.rcFrame.iLeft;
        pVInfo->videoInfo.m_iHeight = rTrack.m_StreamHeader.rcFrame.iBottom - rTrack.m_StreamHeader.rcFrame.iTop;
        if (pVInfo->videoInfo.m_iWidth <= 0) pVInfo->videoInfo.m_iWidth = m_AviHdr.uiWidth;
        if (pVInfo->videoInfo.m_iHeight <= 0) pVInfo->videoInfo.m_iHeight = m_AviHdr.uiHeight;
    }

    return UMC_OK;
}

inline
Ipp32u GetTrackId(Ipp32u fourCC)
{
    return 10 * ((fourCC & 0xFF) - '0') + (((fourCC >> 8) & 0xFF) - '0');
}

inline
bool IsAudio(Ipp32u fourCC)
{
    return ((fourCC & 0xFFFF0000) == AVI_FOURCC_WB);
}

inline
bool IsVideo(Ipp32u fourCC)
{
    return (((fourCC & 0xFFFF0000) == AVI_FOURCC_DC) || ((fourCC & 0xFFFF0000) == AVI_FOURCC_DB));
}

inline
bool IsAudioOrVideo(Ipp32u fourCC)
{
    return (IsAudio(fourCC) || IsVideo(fourCC));
}

inline
Ipp64f GetSampleDuration(bool bIsVideoTrack, bool bIsVBRAudio, Ipp64f dFrameDuration, Ipp32u nBlockAlign, size_t nSampleSize)
{
    if (bIsVideoTrack)
    {
        return dFrameDuration;
    }
    else
    {
        if (bIsVBRAudio)
            return dFrameDuration * ((nSampleSize - 1) / nBlockAlign + 1);
        else
            return dFrameDuration * nSampleSize / nBlockAlign;
    }
}

Status AVISplitter::GenerateIndex()
{
    Status umcRes = UMC_OK;
    bool bUseOldIndex = false;
    Ipp32u i;

    UMC_NEW_ARR(m_pDuration, Ipp64f, m_AviHdr.uiStreams);
    ippsSet_8u(0, (Ipp8u*)m_pDuration, m_AviHdr.uiStreams * sizeof(Ipp64f));

    for(i = 0; i < m_AviHdr.uiStreams; i++)
    {
        if (AVI_FOURCC_auds == m_pTrack[i].m_StreamHeader.fccType ||
            AVI_FOURCC_vids == m_pTrack[i].m_StreamHeader.fccType ||
            AVI_FOURCC_iavs == m_pTrack[i].m_StreamHeader.fccType ||
            AVI_FOURCC_ivas == m_pTrack[i].m_StreamHeader.fccType)
        {
            if (NULL == m_pTrack[i].m_pIndex || 0 == m_pTrack[i].m_uiIndexSize)
                bUseOldIndex = true;
        }
    }

    if (bUseOldIndex)
    {
        Ipp32u riffType = AVI_FOURCC_AVI_;
        bool bExtension = false;

        while (umcRes == UMC_OK)
        {
            if (bExtension)
                riffType = AVI_FOURCC_AVIX;

            umcRes = m_AviChunkReader.DescendRIFF(riffType);
            if (umcRes != UMC_OK)
                return bExtension ? UMC_OK : umcRes;

            umcRes = m_AviChunkReader.DescendLIST(AVI_FOURCC_MOVI);
            if (umcRes != UMC_OK)
                return umcRes;

            // offset in idx1 can be absolute offset, or position relatively to
            // the first byte of "movi" indentificator.
            Ipp64u nMoviChunkStartAddr = m_AviChunkReader.GetChunkHead() - 4;
            Ipp64u nMoviChunkEndAddr = nMoviChunkStartAddr + m_AviChunkReader.GetChunkSize();
            m_AviChunkReader.Ascend();

            umcRes = m_AviChunkReader.DescendChunk(AVI_FOURCC_IDX1);
            if (umcRes != UMC_OK)
                return bExtension ? UMC_OK : umcRes;

            Ipp32u nIndexSize = m_AviChunkReader.GetChunkSize();
            Ipp8u *pIndex = (Ipp8u*)ippMalloc(nIndexSize);
            if (pIndex == NULL)
                return UMC_ERR_ALLOC;

            umcRes = m_AviChunkReader.GetData(pIndex, nIndexSize);
            if (UMC_OK != umcRes)
            {
                ippFree(pIndex);
                return umcRes;
            }

#ifdef _BIG_ENDIAN_
            ippsSwapBytes_32u_I((Ipp32u *)pIndex, nIndexSize / 4);
#endif

            InitIndexUsingOldAVIIndex(pIndex, nIndexSize, nMoviChunkStartAddr, nMoviChunkEndAddr);
            ippFree(pIndex);
            // ascend from idx1
            m_AviChunkReader.Ascend();
            // ascend from avi_ or avix
            m_AviChunkReader.Ascend();
            // look for AVIX list type at next loop
            bExtension = true;
        }
    }
    else
    {
        for (i = 0; i < m_AviHdr.uiStreams; i++)
        {
            if (AVI_FOURCC_auds == m_pTrack[i].m_StreamHeader.fccType ||
                AVI_FOURCC_vids == m_pTrack[i].m_StreamHeader.fccType ||
                AVI_FOURCC_iavs == m_pTrack[i].m_StreamHeader.fccType ||
                AVI_FOURCC_ivas == m_pTrack[i].m_StreamHeader.fccType)
                InitIndexUsingNewAVIIndex(i, m_pTrack[i].m_pIndex, m_pTrack[i].m_uiIndexSize);
        }
    }

    return UMC_OK;
}

Status AVISplitter::InitIndexUsingOldAVIIndex(Ipp8u *pIndexBuffer, Ipp32s nIndexSize,
    Ipp64u nMoviChunkStartAddr, Ipp64u /*nMoviChunkEndAddr*/)
{
    const Ipp32s BYTES_PER_ENTRY = 16;
    Ipp32s nTotalEntry = nIndexSize/BYTES_PER_ENTRY;
    Ipp32s i;

    // members of avi index entry
    Ipp32u nChunkID = 0;
    Ipp32u nFlag = 0;
    Ipp32u nOffset = 0;
    Ipp32u nLength = 0;

    // do a quick check if the content contains VBR audio
    if (!m_bCheckVBRAudio)
    {
        m_bCheckVBRAudio = true;

        bool bAudioPresent = false;
        for (i = 0; i < (Ipp32s)m_AviHdr.uiStreams && !bAudioPresent; i++)
            if (AVI_FOURCC_auds == m_pTrack[i].m_StreamHeader.fccType)
                bAudioPresent = true;

        if (bAudioPresent)
        {
            Ipp8u *pTemp = pIndexBuffer;
            Ipp32s iAudioSampleChecked = 0;
            // check up to 100 audio samples in the first fragment to determine VBR or CBR audio
            const Ipp32s MAX_AUDIO_SAMPLE_TO_CHECK = 100;
            for (i = 0; i < nTotalEntry; i++)
            {
                nChunkID = *(Ipp32u*)pTemp;
                Ipp32u uiTrackNum = GetTrackId(nChunkID);

                // 16 bytes per entry
                nLength = *(Ipp32u*)(pTemp + 12);
                pTemp += BYTES_PER_ENTRY;

                if (uiTrackNum > m_AviHdr.uiStreams || !IsAudio(nChunkID))
                    continue;

                if (nLength < ((WaveFormatEx *)m_pTrack[uiTrackNum].m_pStreamFormat)->nBlockAlign)
                {
                    m_bIsVBRAudio = true;
                    break;
                }
                iAudioSampleChecked++;
                if (iAudioSampleChecked > MAX_AUDIO_SAMPLE_TO_CHECK)
                {
                    // Limit reached, stop checking. Assuming it is CBR audio.
                    break;
                }
            }
        }
    }

    Ipp32u uiTrackNum = 0;
    bool bIsVideoTrack = true;
    bool bAbsoluteOffset = true;

    TrackIndex *pTrackIndex;
    IndexEntry *pEntry;

    SelfDestructingArray<IndexFragment> pFrag(m_AviHdr.uiStreams);
    SelfDestructingArray<Ipp64f> pTimeStamp(m_AviHdr.uiStreams);
    SelfDestructingArray<Ipp32u> pCounter(m_AviHdr.uiStreams);
    SelfDestructingArray<Ipp32u> pSize(m_AviHdr.uiStreams);
    SelfDestructingArray<bool> pIsFirstFrame(m_AviHdr.uiStreams);

    for (i = 0; i < (Ipp32s)m_AviHdr.uiStreams; i++)
    {
        pTimeStamp[i] = 0.0;
        pSize[i] = 0;
        pCounter[i] = 0;
        pIsFirstFrame[i] = true;
    }

    for (i = 0; i < nTotalEntry; i++)
    {
        nChunkID = *(Ipp32u *)pIndexBuffer;
        pIndexBuffer += 4;
        uiTrackNum = GetTrackId(nChunkID);
        if (uiTrackNum >= m_AviHdr.uiStreams)
        {
            pIndexBuffer += 12;
            continue;
        }

        // initialize these variables based on track index
        bIsVideoTrack = !IsAudio(nChunkID);

        pTrackIndex = &m_pTrackIndex[uiTrackNum];

        nFlag = *(Ipp32u *)pIndexBuffer;
        pIndexBuffer += 4;
        // offset includes 8 bytes of chunk header
        nOffset = *(Ipp32u *)pIndexBuffer;
        pIndexBuffer += 4;

        // length does not include 8 bytes of chunk header
        nLength = *(Ipp32u *)pIndexBuffer;
        pIndexBuffer += 4;

        if (i == 0)
        {
            bAbsoluteOffset = nOffset > nMoviChunkStartAddr;
        }

        if ((Ipp32s)pCounter[uiTrackNum] >= pFrag[uiTrackNum].iNOfEntries)
        { // fragment is full
            if (pFrag[uiTrackNum].iNOfEntries > 0 && pFrag[uiTrackNum].pEntryArray)
                pTrackIndex->Add(pFrag[uiTrackNum]);

            // estimated total samples, will be adjusted at the end
            pFrag[uiTrackNum].iNOfEntries = (nTotalEntry - i) / m_AviHdr.uiStreams + 1;
            if (pFrag[uiTrackNum].iNOfEntries < 100)
                pFrag[uiTrackNum].iNOfEntries = 100;

            pCounter[uiTrackNum] = 0;
            pFrag[uiTrackNum].pEntryArray = new IndexEntry[pFrag[uiTrackNum].iNOfEntries];
            if (NULL == pFrag[uiTrackNum].pEntryArray)
                return UMC_ERR_ALLOC;
        }

        pEntry = &pFrag[uiTrackNum].pEntryArray[pCounter[uiTrackNum]];
        pEntry->uiSize = pSize[uiTrackNum] = nLength;
        if (nLength > m_pTrack[uiTrackNum].m_uiMaxSampleSize)
            m_pTrack[uiTrackNum].m_uiMaxSampleSize = nLength;

        pEntry->dDts = -1.0;
        if (pIsFirstFrame[uiTrackNum])
        {
            // Start time, usually it is zero, but it can specify a delay time for
            // a stream that does not start concurrently with the file.
            pIsFirstFrame[uiTrackNum] = false;
            pEntry->dPts = m_pTrack[uiTrackNum].m_StreamHeader.uiStart *
                  (Ipp64f)(m_pTrack[uiTrackNum].m_StreamHeader.uiScale) /
                           m_pTrack[uiTrackNum].m_StreamHeader.uiRate;
        }
        else
        {
            pEntry->dPts = pTimeStamp[uiTrackNum];
        }

        // check index flag
        if (nFlag & AVIIF_KEYFRAME)
            pEntry->uiFlags = I_PICTURE;
/*
        if ((nFlag & AVIIF_FIRSTPART) || (nFlag & AVIIF_LASTPART))
        {
            vm_debug_trace(VM_DEBUG_ERROR, __VM_STRING("WARNING: AVI index has FIRSTPART or LASTPART flag set!\n"));
        }

        if (nFlag & AVIIF_NO_TIME)
        {
            vm_debug_trace(VM_DEBUG_ERROR, __VM_STRING("WARNING: AVI index has NO_TIME flag set!\n"));
        }
*/
        pEntry->stPosition = nOffset + 8; // 8 bytes of chunk header
        if (!bAbsoluteOffset)
            pEntry->stPosition += (size_t)nMoviChunkStartAddr;

        pCounter[uiTrackNum]++;
        pTimeStamp[uiTrackNum] += GetSampleDuration(bIsVideoTrack, m_bIsVBRAudio,
            (Ipp64f)(m_pTrack[uiTrackNum].m_StreamHeader.uiScale) / m_pTrack[uiTrackNum].m_StreamHeader.uiRate,
            ((WaveFormatEx *)m_pTrack[uiTrackNum].m_pStreamFormat)->nBlockAlign,
            nLength);
    }

    for (i = 0; i < (Ipp32s)m_AviHdr.uiStreams; i++)
    {
        m_pDuration[i] = pTimeStamp[i];
        if (pCounter[i] > 0)
        {
            pFrag[i].iNOfEntries = pCounter[i];
            m_pTrackIndex[i].Add(pFrag[i]);
        }
    }

    return UMC_OK;
}

Status AVISplitter::InitIndexUsingNewAVIIndex(Ipp32u nTrackNum, Ipp8u *pIndexBuffer, Ipp32s nIndexSize)
{
    Status umcRes = UMC_OK;
    Ipp8u *buf = pIndexBuffer;

    Ipp16u wLongsPerEntry = *(Ipp16u *)buf;
    wLongsPerEntry = BIG_ENDIAN_SWAP16(wLongsPerEntry);
    buf += 2;

    buf += 1; // skip bIndexSubType

    Ipp8u bIndexType = *(Ipp8u *)buf;
    buf += 1;

    Ipp32u nEntriesInUse = *(Ipp32u *)buf;
    nEntriesInUse = BIG_ENDIAN_SWAP32(nEntriesInUse);
    buf += 4;

    buf += 4; // skip dwChunkId

    Ipp64u qwBaseOffset = 0;
    if (bIndexType == AVI_INDEX_OF_CHUNKS || bIndexType == AVI_INDEX_OF_SUB_2FIELD)
    {
        qwBaseOffset = *((Ipp64u*)buf);
        qwBaseOffset = BIG_ENDIAN_SWAP64(qwBaseOffset);
        buf += 8;
        buf += 4; // reserved

        if (wLongsPerEntry == 0)
            wLongsPerEntry = (bIndexType == AVI_INDEX_OF_CHUNKS) ? 2 : 3;
    }
    else if (bIndexType == AVI_INDEX_OF_INDEXES)
    {
        buf += 12; // reserved (3 Ipp32u)
        if (wLongsPerEntry == 0)
            wLongsPerEntry = 4;
    }

    if (wLongsPerEntry == 0)
        return UMC_ERR_FAILED;

    nIndexSize -= (Ipp32s)(buf - pIndexBuffer);
    nEntriesInUse = IPP_MIN(nEntriesInUse, Ipp32u(nIndexSize / (wLongsPerEntry * 4)));

    // super index
    if (bIndexType == AVI_INDEX_OF_INDEXES)
    {
        Ipp32u i;
        for (i = 0; i < nEntriesInUse; i++)
        {
            Ipp64u qwOffset = *(Ipp64u *)buf;
            qwOffset = BIG_ENDIAN_SWAP64(qwOffset);
            buf += 8;

            // size of index chunk at this offset
            Ipp32u dwSize = *(Ipp32u *)buf;
            dwSize = BIG_ENDIAN_SWAP32(dwSize);
            buf += 4;
            buf += 4; // ignore dwDuration

            Ipp8u *pIndexBuffer = (Ipp8u*)ippMalloc(dwSize);
            if (pIndexBuffer == NULL)
                return UMC_ERR_ALLOC;

            Status umcRes = m_AviChunkReader.GetData(qwOffset, pIndexBuffer, dwSize);
            if (umcRes == UMC_OK)
            {
                // skip standard chunk header - 8 bytes
                umcRes = InitIndexUsingNewAVIIndex(nTrackNum, pIndexBuffer + 8, dwSize - 8);
            }

            ippFree(pIndexBuffer);
        }
    }
    else
    {
        // standard index
        umcRes = InitIndexUsingStandardIndex(nTrackNum, buf, nEntriesInUse, qwBaseOffset, wLongsPerEntry);
    }

    return umcRes;
}

Status AVISplitter::InitIndexUsingStandardIndex(
    Ipp32u nTrackNum,
    Ipp8u *pIndexBuffer,
    Ipp32u nEntriesInUse,
    Ipp64u qwBaseOffset,
    Ipp16u wLongsPerEntry)
{
    // AVI standard index chunk or AVI field index chunk
    TrackIndex *pIndex = &m_pTrackIndex[nTrackNum];
    IndexFragment frag;
    IndexEntry entry;
    Ipp32u i;

    frag.iNOfEntries = nEntriesInUse;
    frag.pEntryArray = new IndexEntry[nEntriesInUse];
    if (NULL == frag.pEntryArray)
        return UMC_ERR_ALLOC;

    memset(frag.pEntryArray, 0, nEntriesInUse * sizeof(IndexEntry));

    bool bIsAudioTrack = (AVI_FOURCC_auds == m_pTrack[nTrackNum].m_StreamHeader.fccType);
    Ipp32s nBlockAlign = !bIsAudioTrack ? 0 : ((WaveFormatEx *)m_pTrack[nTrackNum].m_pStreamFormat)->nBlockAlign;
    Ipp64f dFrameDuration = (Ipp64f)(m_pTrack[nTrackNum].m_StreamHeader.uiScale) / m_pTrack[nTrackNum].m_StreamHeader.uiRate;

    // if we have already checked VBR or CBR audio in the first standard index,
    // don't bother to do another check.
    if (bIsAudioTrack && !m_bCheckVBRAudio)
    {
        m_bCheckVBRAudio = true;
        Ipp8u *pTemp = pIndexBuffer;
        Ipp32u dwSize = 0;
        for (i = 0; i < nEntriesInUse; i++)
        {
            dwSize = (*(Ipp32u*)(pTemp + 4));
            dwSize = BIG_ENDIAN_SWAP32(dwSize);
            dwSize &= 0x7FFFFFFF;

            // move buffer to next entry
            pTemp += wLongsPerEntry * 4;
            if ((Ipp32s)dwSize < nBlockAlign)
            {
                // VBR if sample size is less than block aglin.
                // same other applications check if nBlockAlign is 1152 or 576.
                m_bIsVBRAudio = true;
                break;
            }
        }
    }

    Ipp32u dwOffset = 0;
    Ipp32u dwSize = 0;
    Ipp64f dTimeStamp = 0;
    if (UMC_OK == pIndex->Last(entry))
    {
        dTimeStamp = entry.GetTimeStamp();
        dTimeStamp += GetSampleDuration(!bIsAudioTrack, m_bIsVBRAudio, dFrameDuration, nBlockAlign, entry.uiSize);
    }

    for (i = 0; i < nEntriesInUse; i++)
    {
        dwOffset = *(Ipp32u *)pIndexBuffer;
        dwOffset = BIG_ENDIAN_SWAP32(dwOffset);
        dwSize = *(Ipp32u *)(pIndexBuffer + 4);
        dwSize = BIG_ENDIAN_SWAP32(dwSize);

        // move to next index entry
        pIndexBuffer += wLongsPerEntry * 4;

        frag.pEntryArray[i].stPosition = (size_t)(qwBaseOffset + dwOffset);
        // bit 31 is Key frame flag
        frag.pEntryArray[i].uiSize = dwSize & 0x7FFFFFFF;
        if (frag.pEntryArray[i].uiSize > m_pTrack[nTrackNum].m_uiMaxSampleSize)
            m_pTrack[nTrackNum].m_uiMaxSampleSize = frag.pEntryArray[i].uiSize;

        // bit 31 is set if this is NOT a key frame
        if (((dwSize & 0x8FFFFFFF) >> 31) == 0)
            frag.pEntryArray[i].uiFlags = I_PICTURE;

        // first sample overall
        frag.pEntryArray[i].dDts = -1.0;
        if (i == 0 && UMC_OK != pIndex->First(entry))
        {
            // Start time, usually it is zero, but it can specify a delay time for
            // a stream that does not start concurrently with the file.
            frag.pEntryArray[i].dPts = m_pTrack[nTrackNum].m_StreamHeader.uiStart * dFrameDuration;
        }
        else
        {
            frag.pEntryArray[i].dPts = dTimeStamp;
        }

        dTimeStamp += GetSampleDuration(!bIsAudioTrack, m_bIsVBRAudio, dFrameDuration, nBlockAlign, frag.pEntryArray[i].uiSize);
    }

    m_pDuration[nTrackNum] = dTimeStamp;
    return pIndex->Add(frag);
}

#endif
