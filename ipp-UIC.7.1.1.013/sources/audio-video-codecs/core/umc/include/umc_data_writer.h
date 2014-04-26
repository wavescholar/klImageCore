/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DATA_WRITER_H__
#define __UMC_DATA_WRITER_H__

/*
//  Class:       DataWriter
//
//  Notes:       Base abstract class of data writer. Class describes
//               the high level interface of abstract source of data.
//               All specific ( reading from file, from network, inernet, etc ) must be implemented in
//               derived classes.
//               Splitter uses this class to obtain data
//
*/

#include "umc_structures.h"

namespace UMC
{

class DataWriterParams
{
public:
    DYNAMIC_CAST_DECL_BASE(DataWriterParams)

    DataWriterParams(void) {}
    virtual ~DataWriterParams(void) {}
};

class DataWriter
{
public:
    DYNAMIC_CAST_DECL_BASE(DataWriter)

    DataWriter(void) {}
    virtual ~DataWriter(void) {}

    // Initialization abstract destination media
    virtual Status Init(DataWriterParams* pParams) = 0;

    // Close destination media
    virtual Status Close(void) = 0;

    // Reset all internal parameters to start writing from begin
    virtual Status Reset(void) = 0;

    // Write data to output stream
    virtual Status PutData(const void *pData, size_t &iSize) = 0;

    // Get absolute position in the stream
    virtual Ipp64u GetPosition(void) = 0;

    // Move position relatively
    virtual Status MovePosition(Ipp64s iOffset) = 0;

    // Set position absolutely
    virtual Status SetPosition(Ipp64u iPosition)
    {
        return MovePosition(iPosition - GetPosition());
    }

    Status Put8u(Ipp8u iByte)
    {
        size_t iSize = 1;
        return PutData(&iByte, iSize);
    }

    // Write 2 bytes
    Status Put16uSwap(Ipp16u iWord)
    {
        size_t iSize = 2;
        iWord = LITTLE_ENDIAN_SWAP16(iWord);
        return PutData(&iWord, iSize);
    }

    // Write 2 bytes without swap
    Status Put16uNoSwap(Ipp16u iWord)
    {
        size_t iSize = 2;
        iWord = BIG_ENDIAN_SWAP16(iWord);
        return PutData(&iWord, iSize);
    }

    // Write 4 bytes
    Status Put32uSwap(Ipp32u iDWord)
    {
        size_t iSize = 4;
        iDWord = LITTLE_ENDIAN_SWAP32(iDWord);
        return PutData(&iDWord, iSize);
    }

    // Write 4 bytes without swap
    Status Put32uNoSwap(Ipp32u iDWord)
    {
        size_t iSize = 4;
        iDWord = BIG_ENDIAN_SWAP32(iDWord);
        return PutData(&iDWord, iSize);
    }

    // Write 8 bytes
    Status Put64uSwap(Ipp64u iQWord)
    {
        size_t iSize = 8;
        iQWord = LITTLE_ENDIAN_SWAP64(iQWord);
        return PutData(&iQWord, iSize);
    }

    // Write 8 bytes without swap
    Status Put64uNoSwap(Ipp64u iQWord)
    {
        size_t iSize = 8;
        iQWord = BIG_ENDIAN_SWAP64(iQWord);
        return PutData(&iQWord, iSize);
    }
};

}

#endif
