/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_DATA_READER_H__
#define __UMC_DATA_READER_H__

/*
//  Class:       DataReader
//
//  Notes:       Base abstract class of data reader. Class describes
//               the high level interface of abstract source of data.
//               All specific ( reading from file, from network, inernet, etc ) must be implemented in
//               derived classes.
//               Splitter uses this class to obtain data
//
*/

#include "umc_structures.h"

namespace UMC
{

class DataReaderParams
{
public:
    DYNAMIC_CAST_DECL_BASE(DataReaderParams)

    DataReaderParams(void) {}
    virtual ~DataReaderParams(void) {}
};

class DataReader
{
public:
    DYNAMIC_CAST_DECL_BASE(DataReader)

    DataReader(void) {}
    virtual ~DataReader(void) {}

    // Initialization abstract source data
    virtual Status Init(DataReaderParams *pParams) = 0;

    // Close source data
    virtual Status Close(void) = 0;

    // Reset all internal parameters to start reading from begin
    virtual Status Reset(void) = 0;

    // Read iSize bytes and copy to data (return number bytes which was copy).
    virtual Status GetData(void *pData, size_t &iSize) = 0;

    // Cache and check data
    virtual Status CacheData(void *pData, size_t &iSize, Ipp64s iOffset = 0) = 0;

    // Get absolute position in the stream
    virtual Ipp64u GetPosition(void) = 0;

    // Move position relatively
    virtual Status MovePosition(Ipp64s iOffset) = 0;

    // Set position absolutely
    virtual Status SetPosition(Ipp64u iPosition)
    {
        return MovePosition(iPosition - GetPosition());
    }

    // Get size of the source data
    virtual Ipp64u GetSize(void) = 0;

    // Check for EOS
    virtual bool IsEOS(void) { return (GetPosition() >= GetSize())?true:false; }

    // Check byte value
    Status Check8u(Ipp8u *pData, Ipp64s iOffset)
    {
        size_t iSize = 1;
        return CacheData(pData, iSize, iOffset);
    }

    // Check Ipp16s value
    Status Check16u(Ipp16u *pData, Ipp64s iOffset)
    {
        size_t iSize = 2;
        Status status = CacheData(pData, iSize, iOffset);
        *pData = LITTLE_ENDIAN_SWAP16(*pData);

        return status;
    }

    // Check Ipp32u value
    Status Check32u(Ipp32u *pData, Ipp64s iOffset)
    {
        size_t iSize = 4;
        Status status = CacheData(pData, iSize, iOffset);
        *pData = LITTLE_ENDIAN_SWAP32(*pData);

        return status;
    }

    // Return 1 byte
    Status Get8u(Ipp8u *pData)
    {
        size_t iSize = 1;
        return GetData(pData, iSize);
    }

    // Return 2 bytes
    Status Get16uSwap(Ipp16u *pData)
    {
        size_t iSize = 2;
        Status status = GetData(pData, iSize);
        *pData = LITTLE_ENDIAN_SWAP16(*pData);

        return status;
    }

    // Return 2 bytes without swap
    Status Get16uNoSwap(Ipp16u *pData)
    {
        size_t iSize = 2;
        Status status = GetData(pData, iSize);
        *pData = BIG_ENDIAN_SWAP16(*pData);

        return status;
    }

    // Return 4 bytes
    Status Get32uSwap(Ipp32u *pData)
    {
        size_t iSize = 4;
        Status status = GetData(pData, iSize);
        *pData = LITTLE_ENDIAN_SWAP32(*pData);

        return status;
    }

    // Return 4 bytes without swap
    Status Get32uNoSwap(Ipp32u *pData)
    {
        size_t iSize = 4;
        Status status = GetData(pData, iSize);
        *pData = BIG_ENDIAN_SWAP32(*pData);

        return status;
    }

    // Return 8 bytes
    Status Get64uSwap(Ipp64u *pData)
    {
        size_t iSize = 8;
        Status status = GetData(pData, iSize);
        *pData = LITTLE_ENDIAN_SWAP64(*pData);

        return status;
    }

    // Return 8 bytes without swap
    Status Get64uNoSwap(Ipp64u *pData)
    {
        size_t iSize = 8;
        Status status = GetData(pData, iSize);
        *pData = BIG_ENDIAN_SWAP64(*pData);

        return status;
    }
};

}

#endif
