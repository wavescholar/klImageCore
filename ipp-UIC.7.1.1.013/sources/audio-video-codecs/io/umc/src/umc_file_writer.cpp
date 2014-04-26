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
#ifdef UMC_ENABLE_FILE_WRITER

#include "umc_file_writer.h"

using namespace UMC;


FileWriter::FileWriter()
{
    m_pFile     = NULL;
    m_iFileSize = 0;
}

FileWriter::~FileWriter()
{
    Close();
}

Status FileWriter::Init(DataWriterParams *pInit)
{
    FileWriterParams* pParams = DynamicCast<FileWriterParams, DataWriterParams>(pInit);

    if(!pParams)
        return UMC_ERR_NULL_PTR;

    m_params = *pParams;

    Close();

    if(m_params.m_bAppend)
        m_pFile = vm_file_open((vm_char*)m_params.m_sFileName, VM_STRING("ab"));
    else
        m_pFile = vm_file_open((vm_char*)m_params.m_sFileName, VM_STRING("wb"));
    if(!m_pFile)
        return UMC_ERR_FAILED;

    if(!m_params.m_iBufferSize)
        m_params.m_iBufferSize = 0xFFFF;

    if(setvbuf(m_pFile, NULL, _IOFBF, m_params.m_iBufferSize))
        return UMC_ERR_FAILED;

    return UMC_OK;
}

Status FileWriter::Close()
{
    if(m_pFile)
    {
        vm_file_fclose(m_pFile);
        m_pFile = NULL;
    }

    m_iFileSize = 0;

    return UMC_OK;
}

Status FileWriter::Reset()
{
    if(!m_pFile)
        return UMC_ERR_NOT_INITIALIZED;

    if(vm_file_fseek(m_pFile, 0, VM_FILE_SEEK_SET) != 0)
        return UMC_ERR_FAILED;

    return UMC_OK;
}

Status FileWriter::PutData(const void *pData, size_t &iSize)
{
    size_t iWriteSize = 0;
    Ipp64u iPosition;

    if(!pData)
        return UMC_ERR_NULL_PTR;

    if(!m_pFile)
        return UMC_ERR_NOT_INITIALIZED;

    if(!iSize)
        return UMC_OK;

    iWriteSize = vm_file_fwrite(pData, 1, iSize, m_pFile);
    iPosition  = GetPosition();

    if(iPosition > m_iFileSize)
        m_iFileSize = iPosition;

    if(iWriteSize < iSize)
    {
        iSize = iWriteSize;
        return UMC_ERR_FAILED;
    }

    iSize = iWriteSize;

    return UMC_OK;
}

Status FileWriter::MovePosition(Ipp64s iOffset)
{
    if(!m_pFile)
        return UMC_ERR_NOT_INITIALIZED;

    if(vm_file_fseek(m_pFile, (long long)iOffset, VM_FILE_SEEK_CUR))
        return UMC_ERR_FAILED;

    return UMC_OK;
}

Ipp64u FileWriter::GetPosition()
{
    return (Ipp64u)vm_file_ftell(m_pFile);
}

Ipp64u FileWriter::GetSize()
{
    return m_iFileSize;
}

#endif
