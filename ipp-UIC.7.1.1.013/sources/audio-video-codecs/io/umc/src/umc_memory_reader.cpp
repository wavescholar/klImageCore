/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_memory_reader.h"

#include "ipps.h"

using namespace UMC;


MemoryReader::MemoryReader()
{
    m_pBuffer      = NULL;
    m_iBufferSize  = 0;
    m_iPosition    = 0;
    m_bEndOfStream = false;
}

Status MemoryReader::Init(DataReaderParams *pBaseParams)
{
    MemoryReaderParams *pParams = DynamicCast<MemoryReaderParams, DataReaderParams>(pBaseParams);

    if(!pParams)
        return UMC_ERR_NULL_PTR;

    m_params = *pParams;

    if(!m_params.m_pBuffer || !m_params.m_iBufferSize)
        return UMC_ERR_INVALID_PARAMS;

    Close();

    m_pBuffer     = m_params.m_pBuffer;
    m_iBufferSize = m_params.m_iBufferSize;

    return UMC_OK;
}

Status MemoryReader::Close()
{
    m_pBuffer      = NULL;
    m_iBufferSize  = 0;
    m_iPosition    = 0;
    m_bEndOfStream = false;
    return UMC_OK;
}

Status MemoryReader::Reset()
{
    m_pBuffer      = m_params.m_pBuffer;
    m_iBufferSize  = m_params.m_iBufferSize;
    m_iPosition    = 0;
    m_bEndOfStream = false;
    return UMC_OK;
}

Status MemoryReader::GetData(void *pData, size_t &iSize)
{
    if(!pData)
        return UMC_ERR_NULL_PTR;

    if(!m_pBuffer)
        return UMC_ERR_NOT_INITIALIZED;

    if(m_bEndOfStream)
    {
        iSize = 0;
        return UMC_ERR_END_OF_STREAM;
    }

    if(m_iBufferSize < m_iPosition + iSize)
        iSize = m_iBufferSize - (size_t)m_iPosition;

    if(iSize > IPP_MAX_32S)
    {
        size_t iCount = iSize;
        do
        {
            if(ippStsNoErr != ippsCopy_8u(&m_pBuffer[m_iPosition], (Ipp8u*)pData, IPP_MAX_32S))
                return UMC_ERR_FAILED;
            iCount -= IPP_MAX_32S;
            m_iPosition += IPP_MAX_32S;
        } while(iCount > IPP_MAX_32S);
        if(ippStsNoErr != ippsCopy_8u(&m_pBuffer[m_iPosition], (Ipp8u*)pData, (int)iCount))
            return UMC_ERR_FAILED;
        m_iPosition += iCount;
    }
    else
    {
        if(ippStsNoErr != ippsCopy_8u(&m_pBuffer[m_iPosition], (Ipp8u*)pData, (int)iSize))
            return UMC_ERR_FAILED;
        m_iPosition += iSize;
    }

    if(m_iPosition >= m_iBufferSize)
    {
        m_bEndOfStream = true;
        return UMC_ERR_END_OF_STREAM;
    }

    return UMC_OK;
}

Status MemoryReader::CacheData(void *pData, size_t &iSize, Ipp64s iOffset)
{
    size_t iCachePos = (size_t)(m_iPosition + iOffset);

    if(!pData)
        return UMC_ERR_NULL_PTR;

    if(!m_pBuffer)
        return UMC_ERR_NOT_INITIALIZED;

    if(((Ipp64s)m_iPosition + iOffset) < 0)
        iCachePos = 0;
    else if(iCachePos > m_iBufferSize)
    {
        iSize = 0;
        return UMC_ERR_END_OF_STREAM;
    }

    if(m_iBufferSize < iCachePos + iSize)
        iSize = m_iBufferSize - iCachePos;

    if(iSize > IPP_MAX_32S)
    {
        size_t iCount = iSize;
        do
        {
            if(ippStsNoErr != ippsCopy_8u(&m_pBuffer[iCachePos], (Ipp8u*)pData, IPP_MAX_32S))
                return UMC_ERR_FAILED;
            iCount -= IPP_MAX_32S;
            iCachePos += IPP_MAX_32S;
        } while(iCount > IPP_MAX_32S);
        if(ippStsNoErr != ippsCopy_8u(&m_pBuffer[iCachePos], (Ipp8u*)pData, (int)iCount))
            return UMC_ERR_FAILED;
        iCachePos += iCount;
    }
    else
    {
        if(ippStsNoErr != ippsCopy_8u(&m_pBuffer[iCachePos], (Ipp8u*)pData, (int)iSize))
            return UMC_ERR_FAILED;
        iCachePos += iSize;
    }

    if(iCachePos >= m_iBufferSize)
        return UMC_ERR_END_OF_STREAM;

    return UMC_OK;
}

Ipp64u MemoryReader::GetPosition()
{
    return m_iPosition;
}

Status MemoryReader::MovePosition(Ipp64s iOffset)
{
    if(!m_pBuffer)
        return UMC_ERR_NOT_INITIALIZED;

    m_iPosition = m_iPosition + iOffset;
    m_bEndOfStream = false;

    if((Ipp64s)m_iPosition < 0)
        m_iPosition = 0;
    else if(m_iPosition >= m_iBufferSize)
    {
        m_bEndOfStream = true;
        m_iPosition    = m_iBufferSize;
        return UMC_ERR_END_OF_STREAM;
    }

    return UMC_OK;
}

Ipp64u MemoryReader::GetSize()
{
    return m_iBufferSize;
}
