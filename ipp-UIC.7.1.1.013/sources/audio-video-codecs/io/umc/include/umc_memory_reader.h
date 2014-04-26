/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MEMORY_READER_H__
#define __UMC_MEMORY_READER_H__

#include "umc_data_reader.h"

namespace UMC
{

class MemoryReaderParams : public DataReaderParams
{
public:
    DYNAMIC_CAST_DECL(MemoryReaderParams, DataReaderParams)

    MemoryReaderParams()
    {
        m_pBuffer     = NULL;
        m_iBufferSize = 0;
    }

    Ipp8u *m_pBuffer;
    size_t m_iBufferSize;
};

class MemoryReader : public DataReader
{
public:
    DYNAMIC_CAST_DECL(MemoryReader, DataReader)

    MemoryReader();

    Status Init(DataReaderParams *pParams);

    Status Close();

    Status Reset();

    Status GetData(void *pData, size_t &iSize);

    Status CacheData(void *pData, size_t &iSize, Ipp64s iOffset);

    Ipp64u GetPosition();

    Status MovePosition(Ipp64s iOffset);

    Ipp64u GetSize();

protected:
    MemoryReaderParams m_params;

    Ipp8u *m_pBuffer;
    size_t m_iBufferSize;
    Ipp64u m_iPosition;
    bool   m_bEndOfStream;
};

}

#endif
