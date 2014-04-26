/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILE_WRITER_H__
#define __UMC_FILE_WRITER_H__

#include "umc_data_writer.h"

namespace UMC
{

class MemoryWriterParams : public DataWriterParams
{
public:
    DYNAMIC_CAST_DECL(MemoryWriterParams, DataWriterParams)

    MemoryWriterParams()
    {
        m_pBuffer     = NULL;
        m_iBufferSize = 0;
    }

    Ipp8u* m_pBuffer;
    size_t m_iBufferSize;
};

class MemoryWriter : public DataWriter
{
public:
    DYNAMIC_CAST_DECL(MemoryWriter, DataWriter)

    MemoryWriter();

    Status Init(DataWriterParams *pParams);

    Status Close();

    Status Reset();

    Status PutData(const void *pData, size_t &iSize);

    Ipp64u GetPosition();

    Status MovePosition(Ipp64s iOffset);

protected:
    MemoryWriterParams m_params;

    Ipp8u *m_pBuffer;
    size_t m_iBufferSize;
    Ipp64u m_iPosition;
};

}

#endif
