/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_media_data.h"

#include "ippdefs.h"
#include "ipps.h"

using namespace UMC;


MediaData::MediaData()
{
    m_pBufferPointer   = NULL;
    m_pDataPointer     = NULL;
    m_iBufferSize      = 0;
    m_iDataSize        = 0;
    m_fPTSStart        = -1;
    m_fPTSEnd          = 0;
    m_frameType        = NONE_PICTURE;
    m_iFlags           = 0;
    m_iInvalid         = 0;
    m_bMemoryAllocated = false;
}

MediaData::~MediaData()
{
    Close();
}

Status MediaData::Alloc(size_t iLength)
{
    MediaData::Close();

    if(iLength)
    {
        m_pBufferPointer = (Ipp8u*)malloc(iLength);
        if(!m_pBufferPointer)
            return UMC_ERR_ALLOC;

        m_pDataPointer     = m_pBufferPointer;
        m_iBufferSize      = iLength;
        m_bMemoryAllocated = true;
    }

    return UMC_OK;
}

Status MediaData::Close(void)
{
    if(m_bMemoryAllocated)
    {
        free(m_pBufferPointer);
        m_bMemoryAllocated = false;
    }

    m_pBufferPointer   = NULL;
    m_pDataPointer     = NULL;
    m_iBufferSize      = 0;
    m_iDataSize        = 0;
    m_frameType        = NONE_PICTURE;

    return UMC_OK;
}

// Set the pointer to a buffer allocated by the user
// bytes define the size of buffer
// size of data is equal to buffer size after this call
Status MediaData::SetBufferPointer(Ipp8u *pBuffer, size_t iSize)
{
    MediaData::Close();

    m_pBufferPointer = pBuffer;
    m_pDataPointer   = pBuffer;
    m_iBufferSize    = iSize;
    m_iDataSize      = 0;

    return UMC_OK;
}

Status MediaData::SetDataSize(size_t iSize)
{
    if(!m_pBufferPointer)
        return UMC_ERR_NULL_PTR;

    if(iSize > (m_iBufferSize - (m_pDataPointer - m_pBufferPointer)))
        return UMC_ERR_FAILED;

    m_iDataSize = iSize;

    return UMC_OK;
}

Status MediaData::MoveDataPointer(Ipp64s iBytes)
{
    Ipp64s iPos = (Ipp64s)(m_pDataPointer - m_pBufferPointer) + iBytes;

    if(iPos < 0 || iPos > (Ipp64s)m_iBufferSize)
        return UMC_ERR_FAILED;

    if(iBytes > (Ipp64s)m_iDataSize)
        iBytes = m_iDataSize;

    m_pDataPointer = m_pDataPointer + iBytes;
    m_iDataSize    = m_iDataSize + (-iBytes);

    return UMC_OK;
}

Status MediaData::ResetDataPointer()
{
    m_iDataSize    = (m_pDataPointer - m_pBufferPointer) + m_iDataSize;
    m_pDataPointer = m_pBufferPointer;

    return UMC_OK;
}

Status MediaData::Reset()
{
    m_pDataPointer = m_pBufferPointer;
    m_iDataSize    = 0;
    m_fPTSStart    = -1;
    m_fPTSEnd      = 0;
    m_frameType    = NONE_PICTURE;
    m_iInvalid     = 0;

    return UMC_OK;
}

MediaData& MediaData::operator=(const MediaData& src)
{
    MediaData::Close();

    m_pDataPointer     = src.m_pDataPointer;
    m_pBufferPointer   = src.m_pBufferPointer;
    m_fPTSStart        = src.m_fPTSStart;
    m_fPTSEnd          = src.m_fPTSEnd;
    m_iBufferSize      = src.m_iBufferSize;
    m_iDataSize        = src.m_iDataSize;
    m_frameType        = src.m_frameType;
    m_iInvalid         = src.m_iInvalid;
    m_iFlags           = src.m_iFlags;
    m_bMemoryAllocated = false;

    return *this;
}

Status MediaData::MoveDataTo(MediaData* pDst)
{
    MediaData *pSrc;
    Ipp8u     *pDataEnd;
    Ipp8u     *pBufferEnd;
    size_t     iSize;

    if(!m_pDataPointer)
        return UMC_ERR_NOT_INITIALIZED;

    if(!pDst || !pDst->m_pDataPointer)
        return UMC_ERR_NULL_PTR;


    pSrc       = this;
    pDataEnd   = pDst->m_pDataPointer + pDst->m_iDataSize;
    pBufferEnd = pDst->m_pBufferPointer + pDst->m_iBufferSize;
    iSize      = IPP_MIN(pSrc->m_iDataSize, (size_t)(pBufferEnd - pDataEnd));

    if(iSize)
        memcpy(pDataEnd, pSrc->m_pDataPointer, iSize);

    pDst->m_iDataSize += iSize;
    pSrc->MoveDataPointer(iSize);

    pDst->m_fPTSStart = pSrc->m_fPTSStart;
    pDst->m_fPTSEnd   = pSrc->m_fPTSEnd;
    pDst->m_frameType = pSrc->m_frameType;
    pDst->m_iInvalid  = pSrc->m_iInvalid;

    return UMC_OK;

}
