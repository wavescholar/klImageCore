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
#include "umc_mpeg2_muxer_linbuf.h"

#include "ippcore.h"
#include "ipps.h"

using namespace UMC;

#define IS_WRAPPED (m_iFirstUsedByte > m_iFirstFreeByte)

MPEG2MuxerLinearBuffer::MPEG2MuxerLinearBuffer()
{
    m_pBuf = NULL;
    m_pAllocBuf = NULL;
    m_pFirstUsedSample = NULL;
    m_pFirstFreeSample = NULL;
    m_pLastUsedSample = NULL;
    m_pLastFreeSample = NULL;
    m_uiAlignmentFlags = 0;
    m_uiInputSize = 0;
    m_iBufferSize = 0;
    m_iUsedSize = 0;
    m_iUnusedSizeAtTheEnd = 0;
    m_iFirstFreeByte = 0;
    m_iFirstUsedByte = 0;
    m_uiNOfUsedSamples = 0;
    m_uiNOfAllocatedSamples = 0;
    m_dNextDTS = -1.0;
    m_bEndOfStream = false;
} //MPEG2MuxerLinearBuffer::MPEG2MuxerLinearBuffer()

MPEG2MuxerLinearBuffer::~MPEG2MuxerLinearBuffer()
{
    Close();
} //MPEG2MuxerLinearBuffer::~MPEG2MuxerLinearBuffer()

Status MPEG2MuxerLinearBuffer::Init(MediaReceiverParams *pInit)
{
    MPEG2MuxerLinearBufferParams *pParams = DynamicCast<MPEG2MuxerLinearBufferParams, MediaReceiverParams> (pInit);

    if (NULL == pParams)
        return UMC_ERR_NULL_PTR;

    if (0 == pParams->uiOfFrames || 0 == pParams->uiBufferSize || 0 == pParams->uiInputSize)
        return UMC_ERR_INVALID_PARAMS;

    Close();

    // init mutex
    m_synchro.Reset();

    // allocate buffer
    m_iBufferSize = (Ipp32s)pParams->uiBufferSize;
    UMC_ALLOC_ARR(m_pAllocBuf, Ipp8u, m_iBufferSize + m_iMaxOutputSize)

    m_pBuf = m_pAllocBuf + m_iMaxOutputSize;
    m_uiInputSize = pParams->uiInputSize;
    m_uiAlignmentFlags = pParams->uiAlignmentFlags;

    // allocate sample structures
    UMC_NEW(m_pFirstUsedSample, MPEG2MuxerSample);

    m_pFirstFreeSample = m_pLastFreeSample = m_pLastUsedSample = m_pFirstUsedSample;
    for (m_uiNOfAllocatedSamples = 1; m_uiNOfAllocatedSamples < pParams->uiOfFrames; m_uiNOfAllocatedSamples++)
    {
        UMC_NEW(m_pLastFreeSample->pNext, MPEG2MuxerSample);
        m_pLastFreeSample->pNext->pPrev = m_pLastFreeSample;
        m_pLastFreeSample = m_pLastFreeSample->pNext;
    }

    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::Init(MediaReceiverParams *pInit)

Status MPEG2MuxerLinearBuffer::Close(void)
{
    AutomaticMutex guard(m_synchro);

    UMC_FREE(m_pAllocBuf);

    while (m_pFirstUsedSample)
    {
        MPEG2MuxerSample *curSample = m_pFirstUsedSample;
        m_pFirstUsedSample = m_pFirstUsedSample->pNext;
        UMC_DELETE(curSample);
    }

    m_pBuf = NULL;
    m_uiAlignmentFlags = 0;
    m_uiInputSize = 0;
    m_iBufferSize = 0;
    m_iFirstFreeByte = 0;
    m_iFirstUsedByte = 0;
    m_iUsedSize = 0;
    m_iUnusedSizeAtTheEnd = 0;
    m_uiNOfUsedSamples = 0;
    m_uiNOfAllocatedSamples = 0;
    m_dNextDTS = -1.0;
    m_bEndOfStream = false;

    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::Close(void)

Status MPEG2MuxerLinearBuffer::Reset(void)
{
    AutomaticMutex guard(m_synchro);
    m_uiNOfUsedSamples = 0;
    m_iUsedSize = 0;
    m_iUnusedSizeAtTheEnd = 0;
    m_pFirstFreeSample = m_pLastUsedSample = m_pFirstUsedSample;
    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::Reset(void)

Status MPEG2MuxerLinearBuffer::LockInputBuffer(MPEG2MuxerSample *pData)
{
    AutomaticMutex guard(m_synchro);

    // check error(s)
    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    // case of wrapped data
    if ((IS_WRAPPED) && (m_iFirstFreeByte + m_uiInputSize >= m_iFirstUsedByte))
         return UMC_ERR_NOT_ENOUGH_BUFFER;

    // case of linear data
    if ((!IS_WRAPPED) && (m_iFirstFreeByte + m_uiInputSize > m_iBufferSize))
    {
        // is there enough space at the beginning of buffer?
        if (m_uiInputSize >= m_iFirstUsedByte)
            return UMC_ERR_NOT_ENOUGH_BUFFER;

        m_iUnusedSizeAtTheEnd = m_iBufferSize - m_iFirstFreeByte;
        m_iFirstFreeByte = 0;
    }

    pData->pData = m_pBuf + m_iFirstFreeByte;
    pData->uiSize = IS_WRAPPED ? (m_iFirstUsedByte - m_iFirstFreeByte) : (m_iBufferSize - m_iFirstFreeByte);
    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::LockInputBuffer(MPEG2MuxerSample *pData)

Status MPEG2MuxerLinearBuffer::UnLockInputBuffer(MPEG2MuxerSample *pData, Status streamStatus)
{
    AutomaticMutex guard(m_synchro);

    if (UMC_OK != streamStatus)
    {
        m_bEndOfStream = true;
        return UMC_OK;
    }

    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    Ipp32s iInputSize = (Ipp32s)pData->uiSize;
    if (iInputSize <= 0)
        return UMC_OK;

    // check if too much data was written (wrapped)
    if ((IS_WRAPPED) && (m_iFirstFreeByte + iInputSize >= m_iFirstUsedByte))
        return UMC_ERR_FAILED;

    // check if too much data was written (linear)
    if ((!IS_WRAPPED) && (m_iFirstFreeByte + iInputSize > m_iBufferSize))
        return UMC_ERR_FAILED;

    m_pFirstFreeSample->pData = m_pBuf + m_iFirstFreeByte;
    m_pFirstFreeSample->uiSize = iInputSize;
    m_pFirstFreeSample->uiTSOffset = pData->uiTSOffset;
    m_pFirstFreeSample->dPTS = pData->dPTS;
    m_pFirstFreeSample->dDTS = pData->dDTS;
    m_pFirstFreeSample->dDuration = pData->dDuration;
    m_pFirstFreeSample->uiFrameType = pData->uiFrameType;
    m_pFirstFreeSample->dExactDTS = (pData->dDTS > 0.0) ? pData->dDTS : (pData->dPTS > 0.0) ? pData->dPTS : m_dNextDTS;
    m_dNextDTS = m_pFirstFreeSample->dExactDTS + m_pFirstFreeSample->dDuration;

    m_uiNOfUsedSamples++;

    if (NULL == m_pFirstFreeSample->pNext)
    {
        //if there is no free sample create new one
        UMC_NEW(m_pFirstFreeSample->pNext, MPEG2MuxerSample);
        m_pLastFreeSample = m_pFirstFreeSample->pNext;
        m_pLastFreeSample->pPrev = m_pFirstFreeSample;
        m_uiNOfAllocatedSamples++;
    }

    m_pFirstFreeSample = m_pFirstFreeSample->pNext;
    m_iFirstFreeByte += iInputSize;
    m_iUsedSize += iInputSize;

    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::UnLockInputBuffer(MPEG2MuxerSample *pData, Status streamStatus)

Status MPEG2MuxerLinearBuffer::LockOutputBuffer(MPEG2MuxerSample *pData)
{
    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    if (0 == m_uiNOfUsedSamples)
        return UMC_ERR_NOT_ENOUGH_DATA;

    Ipp32s i, iRequestedSize = (Ipp32s)pData->uiSize;
    if (iRequestedSize > m_iUsedSize && !m_bEndOfStream && !m_uiAlignmentFlags)
        return UMC_ERR_NOT_ENOUGH_DATA;

    if ((IS_WRAPPED) && (m_iFirstUsedByte + iRequestedSize > m_iBufferSize - m_iUnusedSizeAtTheEnd))
    { // not enough data at the end in case of wrapped buffer
        Ipp8u *pFrom = m_pBuf + m_iFirstUsedByte;
        Ipp8u *pTo = m_pBuf - (m_iBufferSize - m_iUnusedSizeAtTheEnd - m_iFirstUsedByte);
        Ipp32u uiSize = m_iBufferSize - m_iUnusedSizeAtTheEnd - m_iFirstUsedByte;
        ippsCopy_8u(pFrom, pTo, uiSize);

        // update moved samples
        while (m_pFirstUsedSample->pData >= m_pBuf + m_iFirstUsedByte)
            m_pFirstUsedSample->pData += pTo - pFrom;

        m_iFirstUsedByte = m_iBufferSize;
        m_iUnusedSizeAtTheEnd = 0;
    }

    *pData = *m_pFirstUsedSample;
    pData->pData = m_pFirstUsedSample->pData;
    pData->uiSize = 0;

    bool bIsDataEnough = false;
    MPEG2MuxerSample *pSample = m_pFirstUsedSample;
    for (i = 0; i < (Ipp32s)m_uiNOfUsedSamples; i++)
    { // this loop update real time stamps
        if (pData->dPTS < 0.0 && iRequestedSize - (Ipp32s)pData->uiSize > (Ipp32s)pSample->uiTSOffset)
        {
            pData->dPTS = pSample->dPTS;
            pData->dDTS = pSample->dDTS;
            pData->uiTSOffset = pData->uiSize + pSample->uiTSOffset;
        }

        pData->uiSize += pSample->uiSize;

        // enough amount of data has been accumulated
        if ((Ipp32s)pData->uiSize >= iRequestedSize)
        {
            pData->uiSize = iRequestedSize;
            bIsDataEnough = true;
            break;
        }

        // check type of next sample (for padding) if exists
        pSample = pSample->pNext;
        if ((pSample->uiSize > 0) && (m_uiAlignmentFlags & pSample->uiFrameType))
        {
            bIsDataEnough = true;
            break;
        }
    }

    if (!bIsDataEnough && !m_bEndOfStream)
        return UMC_ERR_NOT_ENOUGH_DATA;

    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::LockOutputBuffer(MPEG2MuxerSample *pData)

Status MPEG2MuxerLinearBuffer::UnLockOutputBuffer(MPEG2MuxerSample *pData)
{
    if (NULL == pData)
        return UMC_ERR_NULL_PTR;

    Ipp32s iUnlockSize = (Ipp32s)pData->uiSize;
    if (iUnlockSize <= 0)
        return UMC_OK;

    while (m_pFirstUsedSample->uiSize > 0 && (Ipp32s)m_pFirstUsedSample->uiSize <= iUnlockSize)
    { // unlock whole samples
        iUnlockSize -= (Ipp32s)m_pFirstUsedSample->uiSize;
        m_iUsedSize -= (Ipp32s)m_pFirstUsedSample->uiSize;
        m_pFirstUsedSample->uiSize = 0;
        m_uiNOfUsedSamples--;

        // update sample links
        m_pLastFreeSample->pNext = m_pFirstUsedSample;
        m_pLastFreeSample->pNext->pPrev = m_pLastFreeSample;
        m_pLastFreeSample = m_pLastFreeSample->pNext;
        m_pFirstUsedSample = m_pFirstUsedSample->pNext;
        m_pFirstUsedSample->pPrev->pNext = NULL;
        m_pFirstUsedSample->pPrev = NULL;
    }

    if (iUnlockSize > 0 && iUnlockSize < (Ipp32s)m_pFirstUsedSample->uiSize)
    { // unlock part of the last sample
        Ipp64f dUnlockTime = ((Ipp64f)iUnlockSize / m_pFirstUsedSample->uiSize) * m_pFirstUsedSample->dDuration;
        m_pFirstUsedSample->dExactDTS += dUnlockTime;
        m_pFirstUsedSample->dDuration -= dUnlockTime;
        m_pFirstUsedSample->uiSize -= iUnlockSize;
        m_pFirstUsedSample->pData += iUnlockSize;
        m_iUsedSize -= iUnlockSize;

        if ((Ipp32s)m_pFirstUsedSample->uiTSOffset >= iUnlockSize)
        {
            m_pFirstUsedSample->uiTSOffset -= iUnlockSize;
        }
        else
        {
            m_pFirstUsedSample->uiTSOffset = 0;
            m_pFirstUsedSample->dPTS = -1.0;
            m_pFirstUsedSample->dDTS = -1.0;
        }
    }

    if (m_uiNOfUsedSamples > 0)
        m_iFirstUsedByte = (Ipp32s)(m_pFirstUsedSample->pData - m_pBuf);
    else
        m_iFirstUsedByte = m_iFirstFreeByte;
    return UMC_OK;
} //Status MPEG2MuxerLinearBuffer::UnLockOutputBuffer(MPEG2MuxerSample *pData)

#endif
