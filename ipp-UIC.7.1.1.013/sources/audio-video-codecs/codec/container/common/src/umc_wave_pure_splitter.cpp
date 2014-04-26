/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_wave_pure_splitter.h"
#include "umc_bitstream_reader.h"

using namespace UMC;


WAVESplitter::WAVESplitter()
{
    m_pDataReader = NULL;
    m_iBufferSize = 0;
    m_iResilience = 0;
}

WAVESplitter::~WAVESplitter()
{
    Close();
}

Status WAVESplitter::Init(SplitterParams *pParams)
{
    Status status;
    Ipp32u iDWord;
    Ipp16u iWord;
    Ipp64u iHeaderOffset;
    Ipp16u iBlockSize;
    bool   bWaveExt = false;

    if(!pParams)
        return UMC_OK;

    m_pDataReader = pParams->m_pDataReader;

    m_info.m_iFlags     = pParams->m_iFlags;
    m_info.m_systemType = WAVE_STREAM;
    m_info.m_iTracks    = 1;
    m_info.m_fDuration  = 0;

    m_info.m_ppTrackInfo = new TrackInfo*[1];
    if(!m_info.m_ppTrackInfo)
        return UMC_ERR_ALLOC;

    m_info.m_ppTrackInfo[0] = new TrackInfo;
    if(m_info.m_ppTrackInfo[0] == NULL)
        return UMC_ERR_ALLOC;

    m_info.m_ppTrackInfo[0]->m_pStreamInfo = new AudioStreamInfo;
    if(m_info.m_ppTrackInfo[0]->m_pStreamInfo == NULL)
        return UMC_ERR_ALLOC;

    m_info.m_ppTrackInfo[0]->m_bEnabled = true;

    AudioStreamInfo *pInfo = (AudioStreamInfo*)m_info.m_ppTrackInfo[0]->m_pStreamInfo;

    // start wave parsing
    UMC_CHECK_FUNC(status, m_pDataReader->Get32uSwap(&iDWord));
    if(iDWord != 'RIFF')
        return UMC_ERR_INVALID_STREAM;

    UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));
    UMC_CHECK_FUNC(status, m_pDataReader->Get32uSwap(&iDWord));
    if(iDWord != 'WAVE')
        return UMC_ERR_INVALID_STREAM;

    UMC_CHECK_FUNC(status, m_pDataReader->Get32uSwap(&iDWord));
    if(iDWord != 'fmt ')
        return UMC_ERR_INVALID_STREAM;

    UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));
    iHeaderOffset = iDWord + m_pDataReader->GetPosition();

    UMC_CHECK_FUNC(status, m_pDataReader->Get16uNoSwap(&iWord));

    if(UMC_WAVE_FORMAT_EXTENSIBLE == iWord)
    {
        UMC_CHECK_FUNC(status, m_pDataReader->Check16u(&iWord, 22));
        iWord    = LITTLE_ENDIAN_SWAP16(iWord);
        bWaveExt = true;
    }

    switch(iWord)
    {
    case UMC_WAVE_FORMAT_PCM:
        pInfo->streamType               = PCM_AUDIO;
        m_info.m_ppTrackInfo[0]->m_type = TRACK_PCM;
        break;
    case UMC_WAVE_FORMAT_MPEGLAYER3:
        pInfo->streamType               = MP2L3_AUDIO;
        m_info.m_ppTrackInfo[0]->m_type = TRACK_MPEGA;
        break;
    default:
        m_info.m_ppTrackInfo[0]->m_bEnabled = false;
        return UMC_ERR_UNSUPPORTED;
    }

    UMC_CHECK_FUNC(status, m_pDataReader->Get16uNoSwap(&iWord));
    pInfo->audioInfo.m_iChannels = iWord;

    UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));
    pInfo->audioInfo.m_iSampleFrequency = iDWord;

    UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));
    pInfo->iBitrate = 8 * iDWord;

    UMC_CHECK_FUNC(status, m_pDataReader->Get16uNoSwap(&iBlockSize));
    m_iBufferSize = iBlockSize * pInfo->audioInfo.m_iSampleFrequency; // one second buffer

    UMC_CHECK_FUNC(status, m_pDataReader->Get16uNoSwap(&iWord));
    pInfo->audioInfo.m_iBitPerSample = iWord;

    if(bWaveExt)
    {
        UMC_CHECK_FUNC(status, m_pDataReader->Get16uNoSwap(&iWord));
        UMC_CHECK_FUNC(status, m_pDataReader->Get16uNoSwap(&iWord));

        UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));
        pInfo->audioInfo.m_iChannelMask = iDWord;
    }

    if(iHeaderOffset < m_pDataReader->GetPosition())
        return UMC_ERR_INVALID_STREAM;

    iHeaderOffset -= m_pDataReader->GetPosition();
    UMC_CHECK_FUNC(status, m_pDataReader->MovePosition(iHeaderOffset)); // skip remaining data

    UMC_CHECK_FUNC(status, m_pDataReader->Check32u(&iDWord, 0)); // search for data chunk
    while('data' != iDWord)
    {
        UMC_CHECK_FUNC(status, m_pDataReader->Get32uSwap(&iDWord));
        UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));
        UMC_CHECK_FUNC(status, m_pDataReader->MovePosition(iDWord));
        UMC_CHECK_FUNC(status, m_pDataReader->Check32u(&iDWord, 0));
    }

    UMC_CHECK_FUNC(status, m_pDataReader->Get32uSwap(&iDWord));
    UMC_CHECK_FUNC(status, m_pDataReader->Get32uNoSwap(&iDWord));

    pInfo->fDuration = (Ipp64f)(m_pDataReader->GetSize() - m_pDataReader->GetPosition())/(iBlockSize*pInfo->audioInfo.m_iSampleFrequency);

    return UMC_OK;
}

Status WAVESplitter::Close()
{
    if(m_info.m_ppTrackInfo && m_info.m_ppTrackInfo[0])
        UMC_DELETE(m_info.m_ppTrackInfo[0]->m_pStreamInfo);
    if(m_info.m_ppTrackInfo)
        UMC_DELETE(m_info.m_ppTrackInfo[0]);
    UMC_DELETE_ARR(m_info.m_ppTrackInfo);

    return UMC_OK;
}

Status WAVESplitter::GetNextData(MediaData *pData, Ipp32u )
{
    size_t iReadSize;

    if(!pData)
        return UMC_ERR_NULL_PTR;

    if(m_data.GetBufferSize() < m_iBufferSize)
        m_data.Alloc(m_iBufferSize);

    AudioStreamInfo *pInfo = (AudioStreamInfo*)m_info.m_ppTrackInfo[0]->m_pStreamInfo;

    m_data.m_fPTSStart = (Ipp64f)m_pDataReader->GetPosition()/(pInfo->audioInfo.m_iBitPerSample/8*pInfo->audioInfo.m_iSampleFrequency);

    iReadSize = m_iBufferSize;
    m_pDataReader->GetData(m_data.GetBufferPointer(), iReadSize);
    m_data.SetDataSize(iReadSize);

    m_data.m_fPTSEnd = (Ipp64f)m_pDataReader->GetPosition()/(pInfo->audioInfo.m_iBitPerSample/8*pInfo->audioInfo.m_iSampleFrequency);

    *pData = m_data;

    return UMC_OK;
}

Status WAVESplitter::CheckNextData(MediaData *pData, Ipp32u )
{
    size_t iReadSize;

    if(!pData)
        return UMC_ERR_NULL_PTR;

    if(m_data.GetBufferSize() < m_iBufferSize)
        m_data.Alloc(m_iBufferSize);

    iReadSize = m_iBufferSize;
    m_pDataReader->CacheData(m_data.GetBufferPointer(), iReadSize);
    m_data.SetDataSize(iReadSize);

    *pData = m_data;

    return UMC_OK;
}
