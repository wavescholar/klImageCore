/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILE_READER_H__
#define __UMC_FILE_READER_H__

#include "umc_config.h"
#include "umc_data_reader.h"
#include "umc_string.h"

namespace UMC
{

class FileReaderParams : public DataReaderParams
{
public:
    DYNAMIC_CAST_DECL(FileReaderParams, DataReaderParams)

    FileReaderParams()
    {
        m_iBufferSize = 0;
#ifdef UMC_ENABLE_FILE_READER_MMAP
        m_bUseMMAP    = true;
#endif
    }

    DString   m_sFileName;
    size_t    m_iBufferSize; // setvbuf value for fio and view size for mmap
#ifdef UMC_ENABLE_FILE_READER_MMAP
    bool      m_bUseMMAP;
#endif
};

class FileReader : public DataReader
{
public:
    DYNAMIC_CAST_DECL(FileReader, DataReader)

    FileReader();
    virtual ~FileReader();

    Status Init(DataReaderParams *pParams);

    Status Close();

    Status Reset();

    Status GetData(void *pData, size_t &iSize);

    Status CacheData(void *pData, size_t &iSize, Ipp64s iOffset);

    Status MovePosition(Ipp64s iOffset);

    Ipp64u GetPosition();

    Ipp64u GetSize();

protected:
    FileReaderParams m_params;

#ifdef UMC_ENABLE_FILE_READER_MMAP
    vm_mmap  m_memMap;
    Ipp8u   *m_pBuffer;
    Ipp8u   *m_pDataPointer;
    Ipp8u   *m_pEODPointer;
    Ipp32u   m_iGranularity;
    Ipp64u   m_iViewPosition; // position of view window in the file
    size_t   m_iViewSize;     // prefered view size, not actual one!
#endif
    vm_file *m_pFile;
    Ipp64u   m_iFileSize;
    Ipp64u   m_iPosition;     // absolute position in the file
};

}

#endif
