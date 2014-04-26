/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_memory_writer.h"

#include "ipps.h"

using namespace UMC;


MemoryWriter::MemoryWriter()
{
    m_pBuffer      = NULL;
    m_iBufferSize  = 0;
    m_iPosition    = 0;
}

Status MemoryWriter::Init(DataWriterParams *pBaseParams)
{
    MemoryWriterParams *pParams = DynamicCast<MemoryWriterParams, DataWriterParams>(pBaseParams);

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

Status MemoryWriter::Close()
{
    m_pBuffer      = NULL;
    m_iBufferSize  = 0;
    m_iPosition    = 0;
    return UMC_OK;
}

Status MemoryWriter::Reset()
{
    m_pBuffer      = m_params.m_pBuffer;
    m_iBufferSize  = m_params.m_iBufferSize;
    m_iPosition    = 0;
    return UMC_OK;
}

Status MemoryWriter::PutData(const void *pData, size_t &iSize)
{
    bool bOverflow = false;

    if(!pData)
        return UMC_ERR_NULL_PTR;

    if(!m_pBuffer)
        return UMC_ERR_NOT_INITIALIZED;

    if(m_iBufferSize < m_iPosition + iSize)
    {
        bOverflow = true;
        iSize = m_iBufferSize - (size_t)m_iPosition;
        if(!iSize)
            return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    if(iSize > IPP_MAX_32S)
    {
        size_t iCount = iSize;
        do
        {
            if(ippStsNoErr != ippsCopy_8u((Ipp8u*)pData, &m_pBuffer[m_iPosition], IPP_MAX_32S))
                return UMC_ERR_FAILED;
            iCount -= IPP_MAX_32S;
            m_iPosition += IPP_MAX_32S;
        } while(iCount > IPP_MAX_32S);
        if(ippStsNoErr != ippsCopy_8u((Ipp8u*)pData, &m_pBuffer[m_iPosition], (int)iCount))
            return UMC_ERR_FAILED;
        m_iPosition += iCount;
    }
    else
    {
        if(ippStsNoErr != ippsCopy_8u((Ipp8u*)pData, &m_pBuffer[m_iPosition], (int)iSize))
            return UMC_ERR_FAILED;
        m_iPosition += iSize;
    }

    if(bOverflow)
        return UMC_ERR_NOT_ENOUGH_BUFFER;

    return UMC_OK;
}

Ipp64u MemoryWriter::GetPosition()
{
    return m_iPosition;
}

Status MemoryWriter::MovePosition(Ipp64s iOffset)
{
    if(!m_pBuffer)
        return UMC_ERR_NOT_INITIALIZED;

    m_iPosition = m_iPosition + iOffset;

    if((Ipp64s)m_iPosition < 0)
        m_iPosition = 0;
    else if(m_iPosition >= m_iBufferSize)
    {
        m_iPosition = m_iBufferSize;
        return UMC_ERR_NOT_ENOUGH_BUFFER;
    }

    return UMC_OK;
}
