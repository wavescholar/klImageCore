/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_FILE_READER

#include "umc_file_reader.h"
#ifdef UMC_ENABLE_FILE_READER_MMAP
#include "vm_mmap.h"
#endif

using namespace UMC;


FileReader::FileReader()
{
#ifdef UMC_ENABLE_FILE_READER_MMAP
    m_pBuffer         = 0;
    m_iViewPosition   = 0;
    m_iViewSize       = 0;
    m_iGranularity    = vm_mmap_get_alloc_granularity();
    vm_mmap_set_invalid(&m_memMap);
#endif
    m_pFile           = NULL;
    m_iFileSize       = 0;
    m_iPosition       = 0;
}

FileReader::~FileReader()
{
    Close();
}

Status FileReader::Init(DataReaderParams *pInit)
{
    FileReaderParams* pParams = DynamicCast<FileReaderParams, DataReaderParams>(pInit);

    if(!pParams)
        return UMC_ERR_NULL_PTR;

    m_params = *pParams;

    Close();

#ifdef UMC_ENABLE_FILE_READER_MMAP
    if(m_params.m_bUseMMAP)
    {
        if(!m_iGranularity)
            return UMC_ERR_INIT;

        size_t iViewSize;

        // align size to page boundary
        if(m_params.m_iBufferSize)
            m_params.m_iBufferSize = m_iGranularity * ((m_params.m_iBufferSize + m_iGranularity - 1) / m_iGranularity);
        else
            m_params.m_iBufferSize = m_iGranularity * 1;

        m_iViewSize = m_params.m_iBufferSize;

        if(m_iViewSize == 0)
            return UMC_ERR_INIT;

        m_iFileSize = vm_mmap_create(&m_memMap, (vm_char*)m_params.m_sFileName, FLAG_ATTRIBUTE_READ);
        if(0 == m_iFileSize)
        {
            vm_mmap_set_invalid(&m_memMap);
            return UMC_ERR_OPEN_FAILED;
        }

        iViewSize = m_iViewSize;
        if(iViewSize > m_iFileSize)
            iViewSize = (size_t)m_iFileSize;

        m_pBuffer = m_pDataPointer = (Ipp8u*)vm_mmap_set_view(&m_memMap, &m_iPosition, &iViewSize);
        if(NULL == m_pBuffer)
            return UMC_ERR_FAILED;

        m_pEODPointer   = m_pBuffer + iViewSize;
        m_iViewPosition = m_iPosition;
    }
    else
    {
#endif
        m_pFile = vm_file_open((vm_char*)m_params.m_sFileName, VM_STRING("rb"));
        if(NULL == m_pFile)
            return UMC_ERR_FAILED;

        if(!m_params.m_iBufferSize)
            m_params.m_iBufferSize = 0xFFFF;

        if(setvbuf(m_pFile, NULL, _IOFBF, m_params.m_iBufferSize))
            return UMC_ERR_FAILED;

        if(vm_file_fseek(m_pFile, 0, VM_FILE_SEEK_END))
            return UMC_ERR_FAILED;

        m_iFileSize = vm_file_ftell(m_pFile);
        if(-1 == (Ipp64s)m_iFileSize)
            return UMC_ERR_FAILED;

        if(vm_file_fseek(m_pFile, 0, VM_FILE_SEEK_SET))
            return UMC_ERR_FAILED;
#ifdef UMC_ENABLE_FILE_READER_MMAP
    }
#endif

    return UMC_OK;
}

Status FileReader::Close()
{
#ifdef UMC_ENABLE_FILE_READER_MMAP
    vm_mmap_close(&m_memMap);
    vm_mmap_set_invalid(&m_memMap);

    m_pDataPointer    = NULL;
    m_pBuffer         = NULL;
    m_pEODPointer     = NULL;
    m_iViewPosition   = 0;
    m_iViewSize       = 0;
#endif

    if(m_pFile)
    {
        vm_file_fclose(m_pFile);
        m_pFile = NULL;
    }

    m_iFileSize = 0;
    m_iPosition = 0;

    return UMC_OK;
}

Status FileReader::Reset()
{
#ifdef UMC_ENABLE_FILE_READER_MMAP
    if(m_params.m_bUseMMAP)
    {
        size_t iViewSize;

        if(!vm_mmap_is_valid(&m_memMap))
            return UMC_ERR_NOT_INITIALIZED;
        vm_mmap_unmap(&m_memMap);

        iViewSize = m_iViewSize;
        if(iViewSize > m_iFileSize)
            iViewSize = (size_t)m_iFileSize;

        m_iPosition     = 0;
        m_iViewPosition = 0;

        m_pBuffer = m_pDataPointer = (Ipp8u*)vm_mmap_set_view(&m_memMap, &m_iPosition, &iViewSize);
        if(NULL == m_pBuffer)
            return UMC_ERR_FAILED;

        m_pEODPointer = m_pBuffer + iViewSize;
    }
    else
    {
#endif
        if(!m_pFile)
            return UMC_ERR_NOT_INITIALIZED;

        if(vm_file_fseek(m_pFile, 0, VM_FILE_SEEK_SET))
            return UMC_ERR_FAILED;

        m_iPosition = 0;
#ifdef UMC_ENABLE_FILE_READER_MMAP
    }
#endif

    return UMC_OK;
}

Status FileReader::GetData(void *pData, size_t &iSize)
{
    Status status = UMC_OK;

#ifdef UMC_ENABLE_FILE_READER_MMAP
    if(m_params.m_bUseMMAP)
    {
        size_t iViewSize = m_iViewSize;
        Ipp64u iPosition = m_iPosition;

        if(!pData)
            return UMC_ERR_NULL_PTR;

        if(!vm_mmap_is_valid(&m_memMap))
            return UMC_ERR_NOT_INITIALIZED;

        if(iSize > m_iFileSize - m_iPosition)
        {
            iSize  = (size_t)(m_iFileSize - m_iPosition);
            status = UMC_ERR_END_OF_STREAM;
        }

        if(!iSize)
            return status;

        if((size_t)(m_pEODPointer - m_pDataPointer) >= iSize)
        {
            memcpy(pData, m_pDataPointer, iSize);
            m_pDataPointer += iSize;
            m_iPosition += iSize;
            return status;
        }

        iPosition = m_iPosition - (m_iPosition%m_iGranularity);

        if(iSize > (iViewSize - (size_t)(m_iPosition - iPosition)))
            iViewSize = iSize + (size_t)(m_iPosition - iPosition);

        if(iViewSize > m_iFileSize - iPosition)
            iViewSize = (size_t)(m_iFileSize - iPosition);

        m_pBuffer = m_pDataPointer = (Ipp8u*)vm_mmap_set_view(&m_memMap, &iPosition, &iViewSize);
        if(NULL == m_pBuffer)
            return UMC_ERR_FAILED;

        m_pDataPointer += (m_iPosition - iPosition);
        m_pEODPointer   = m_pBuffer + iViewSize;
        m_iViewPosition = iPosition;

        memcpy(pData, m_pDataPointer, iSize);
        m_pDataPointer += iSize;
        m_iPosition += iSize;
    }
    else
    {
#endif
        size_t iReadSize = 0;

        if(!m_pFile)
            return UMC_ERR_NOT_INITIALIZED;

        if(!pData)
            return UMC_ERR_NULL_PTR;

        if(iSize > m_iFileSize - m_iPosition)
        {
            iSize  = (size_t)(m_iFileSize - m_iPosition);
            status = UMC_ERR_END_OF_STREAM;
        }

        if(!iSize)
            return status;

        iReadSize = vm_file_fread(pData, 1, iSize, m_pFile);

        m_iPosition += iReadSize;

        if(iReadSize < iSize)
        {
            iSize = iReadSize;
            if(!vm_file_feof(m_pFile))
                return UMC_ERR_FAILED;
            else
                status = UMC_ERR_END_OF_STREAM;
        }

        iSize = iReadSize;
#ifdef UMC_ENABLE_FILE_READER_MMAP
    }
#endif

    return status;
}

Status FileReader::CacheData(void *pData, size_t &iSize, Ipp64s iOffset)
{
#ifdef UMC_ENABLE_FILE_READER_MMAP
    if(m_params.m_bUseMMAP)
    {
        size_t iViewSize = m_iViewSize;
        Ipp64u iPosition = m_iPosition + iOffset;
        Ipp64u iPositionAlign;
        Ipp8u *pBuffer;

        if(!pData)
            return UMC_ERR_NULL_PTR;

        if(!vm_mmap_is_valid(&m_memMap))
            return UMC_ERR_NOT_INITIALIZED;

        if((Ipp64s)iPosition < 0)
            iPosition = 0;
        else if(iPosition > m_iFileSize)
            iPosition = m_iFileSize;

        if(iSize > m_iFileSize - iPosition)
            iSize = (size_t)(m_iFileSize - iPosition);

        if(!iSize)
            return UMC_OK;

        if((iPosition >= m_iViewPosition) && ((iPosition + iSize) <= (m_iViewPosition + (m_pEODPointer - m_pBuffer))))
        {
            memcpy(pData, m_pDataPointer + (iPosition - m_iPosition), iSize);
            return UMC_OK;
        }

        iPositionAlign = iPosition - (iPosition%m_iGranularity);

        if(iSize > (iViewSize - (size_t)(iPosition - iPositionAlign)))
            iViewSize = iSize + (size_t)(iPosition - iPositionAlign);

        if(iViewSize > m_iFileSize - iPositionAlign)
            iViewSize = (size_t)(m_iFileSize - iPositionAlign);

        pBuffer = (Ipp8u*)vm_mmap_set_view(&m_memMap, &iPositionAlign, &iViewSize);
        if(NULL == pBuffer)
            return UMC_ERR_FAILED;

        memcpy(pData, pBuffer + (iPosition - iPositionAlign), iSize);

        // return view window to previous position
        iViewSize = (size_t)(m_pEODPointer - m_pBuffer);
        vm_mmap_set_view(&m_memMap, &m_iViewPosition, &iViewSize);
    }
    else
    {
#endif
        Ipp64u iPosition = m_iPosition + iOffset;

        if(!m_pFile)
            return UMC_ERR_NOT_INITIALIZED;

        if(!pData)
            return UMC_ERR_NULL_PTR;

        if((Ipp64s)iPosition < 0)
            iPosition = 0;
        else if(iPosition > m_iFileSize)
            iPosition = m_iFileSize;

        if(iSize > m_iFileSize - iPosition)
            iSize = (size_t)(m_iFileSize - iPosition);

        if(!iSize)
            return UMC_OK;

        if(vm_file_fseek(m_pFile, iPosition, VM_FILE_SEEK_SET))
            return UMC_ERR_FAILED;

        iSize = vm_file_fread(pData, 1, iSize, m_pFile);

        if(vm_file_fseek(m_pFile, m_iPosition, VM_FILE_SEEK_SET))
            return UMC_ERR_FAILED;
#ifdef UMC_ENABLE_FILE_READER_MMAP
    }
#endif

    return UMC_OK;
}

Status FileReader::MovePosition(Ipp64s iOffset)
{
#ifdef UMC_ENABLE_FILE_READER_MMAP
    if(m_params.m_bUseMMAP)
    {
        if(!vm_mmap_is_valid(&m_memMap))
            return UMC_ERR_NOT_INITIALIZED;

        m_iPosition = m_iPosition + iOffset;

        if((Ipp64s)m_iPosition < 0)
            m_iPosition = 0;
        else if(m_iPosition > m_iFileSize)
            m_iPosition = m_iFileSize;

        if((m_iPosition < m_iViewPosition) || (m_iPosition > (m_iViewPosition + (m_pEODPointer - m_pBuffer))))
        {   // out of view
            vm_mmap_unmap(&m_memMap);
            m_pBuffer = m_pDataPointer = m_pEODPointer = 0;
            m_iViewPosition = m_iPosition;
        }
        else
            m_pDataPointer += iOffset;
    }
    else
    {
#endif
        if(!m_pFile)
            return UMC_ERR_NOT_INITIALIZED;

        m_iPosition = m_iPosition + iOffset;

        if((Ipp64s)m_iPosition < 0)
            m_iPosition = 0;
        else if(m_iPosition > m_iFileSize)
            m_iPosition = m_iFileSize;

        if(vm_file_fseek(m_pFile, m_iPosition, VM_FILE_SEEK_SET))
            return UMC_ERR_FAILED;
#ifdef UMC_ENABLE_FILE_READER_MMAP
    }
#endif

    return UMC_OK;
}

Ipp64u FileReader::GetPosition()
{
    return m_iPosition;
}

Ipp64u FileReader::GetSize()
{
    return m_iFileSize;
}

#endif
