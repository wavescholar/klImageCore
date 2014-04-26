/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_audio_data.h"
#include "umc_structures.h"

#include "ipps.h"

using namespace UMC;


AudioData::AudioData(void)
{
    m_iChannels        = 0;
    m_iSampleFrequency = 0;
    m_iBitPerSample    = 0;
    m_iChannelMask     = 0;
    m_iAlignment       = 1;
}

AudioData::~AudioData(void)
{
    Close();
}

Status AudioData::Close(void)
{
    m_iChannels        = 0;
    m_iSampleFrequency = 0;
    m_iBitPerSample    = 0;
    m_iChannelMask     = 0;
    m_iAlignment       = 1;

    MediaData::Close();

    return UMC_OK;
}

Status AudioData::Init(Ipp32u iChannels, Ipp32u iSampleFrequency, Ipp32u iBitPerSample)
{
    Status status;

    if(0 == iChannels || 0 == iSampleFrequency || 0 == iBitPerSample)
        return UMC_ERR_INVALID_PARAMS;

    status = Close();
    if(UMC_OK != status)
        return status;

    m_iChannels         = iChannels;
    m_iSampleFrequency  = iSampleFrequency;
    m_iBitPerSample     = iBitPerSample;

    return UMC_OK;
}

Status AudioData::Init(AudioData *pData)
{
    if(this == pData)
        return UMC_ERR_FAILED;

    Close();

    // copy properties but clear external pointers
    *this = *pData;
    m_iBufferSize      = 0;
    m_iDataSize        = 0;
    m_pBufferPointer   = NULL;
    m_pDataPointer     = NULL;
    m_bMemoryAllocated = false;

    return UMC_OK;
}

Status AudioData::SetAlignment(Ipp32u iAlignment)
{
    if(iAlignment == 0)
        return UMC_ERR_INVALID_PARAMS;

    for(Ipp32u i = 1; i < (1 << 16); i <<= 1)
    {
        if (i & iAlignment)
        {
            m_iAlignment = i;
            break; // stop at last nonzero bit
        }
    }

    if(m_iAlignment != iAlignment)
        return UMC_ERR_INVALID_PARAMS;

    return UMC_OK;
}

Status AudioData::Alloc(size_t iLength)
{
    return MediaData::Alloc(iLength + m_iAlignment - 1);
}

Status AudioData::SetBufferPointer(Ipp8u *pBuffer, size_t iSize)
{
    Ipp8u *pABuf  = align_pointer<Ipp8u *>(pBuffer, m_iAlignment);

    if(iSize < (m_iBitPerSample + 7)/8)
    {
        SetDataSize(0);
        return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    MediaData::SetBufferPointer(pBuffer, iSize);
    SetDataSize(iSize + (pABuf - pBuffer));
    MoveDataPointer((Ipp64s)(pABuf - pBuffer));

    return UMC_OK;
}

Status AudioData::Copy(AudioData *pDstData)
{
    if(!pDstData)
        return UMC_ERR_NULL_PTR;

    if(this == pDstData)
        return UMC_OK;

    // check data compatibility
    if(m_iChannels != pDstData->m_iChannels)
        return UMC_ERR_INVALID_PARAMS;
    if(m_iSampleFrequency != pDstData->m_iSampleFrequency)
        return UMC_ERR_INVALID_PARAMS;
    if(m_iBitPerSample != pDstData->m_iBitPerSample)
        return UMC_ERR_INVALID_PARAMS;
    if(GetBufferSize() != pDstData->GetBufferSize())
        return UMC_ERR_INVALID_PARAMS;

    IppStatus status = ippsCopy_8u((Ipp8u*)GetBufferPointer(), (Ipp8u*)pDstData->GetBufferPointer(), GetBufferSize());
    if(status != ippStsNoErr)
        return UMC_ERR_FAILED;

    return UMC_OK;
}

void AudioData::operator=(const AudioData &par)
{
    MediaData::operator=(par);

    m_iChannels         = par.m_iChannels;
    m_iSampleFrequency  = par.m_iSampleFrequency;
    m_iBitPerSample     = par.m_iBitPerSample;
    m_iAlignment        = par.m_iAlignment;
}
