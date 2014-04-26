/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILE_WRITER_H__
#define __UMC_FILE_WRITER_H__

#include "umc_data_writer.h"
#include "umc_string.h"


namespace UMC
{

class FileWriterParams : public DataWriterParams
{
public:
    DYNAMIC_CAST_DECL(FileWriterParams, DataWriterParams)

    FileWriterParams()
    {
        m_iBufferSize = 0;
        m_bAppend     = false;
    }

    DString   m_sFileName;
    size_t    m_iBufferSize; // setvbuf value for fio
    bool      m_bAppend;     // use writer in append mode
};

class FileWriter : public DataWriter
{
public:
    DYNAMIC_CAST_DECL(FileWriter, DataWriter)

    FileWriter();
    virtual ~FileWriter();

    Status Init(DataWriterParams *pParams);

    Status Close();

    Status Reset();

    Status PutData(const void *pData, size_t &iSize);

    Status MovePosition(Ipp64s iOffset);

    Ipp64u GetPosition();

    Ipp64u GetSize();

protected:
    FileWriterParams m_params;

    vm_file *m_pFile;
    Ipp64u   m_iFileSize; // maximum position at which data was written
};

}

#endif
