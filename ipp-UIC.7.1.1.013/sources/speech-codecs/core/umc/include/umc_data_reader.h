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
//               derevied classes.
//               Splitter uses this class to obtain data
//
*/

#include "ippdefs.h"
#include "umc_structures.h"
#include "umc_dynamic_cast.h"

namespace UMC
{

class DataReaderParams
{
    DYNAMIC_CAST_DECL_BASE(DataReaderParams)

public:
    /// Default constructor
    DataReaderParams(void){}
    /// Destructor
    virtual ~DataReaderParams(void){}
};

class DataReader
{
    DYNAMIC_CAST_DECL_BASE(DataReader)
public:

    /// Default constructor
    DataReader(void)
        { m_pDataPointer = 0; m_pEODPointer = 0; m_bStop = 0;}
    /// Destructor
    virtual ~DataReader(void){}

    /// Initialization abstract source data
    virtual Status Init(DataReaderParams *InitParams) = 0;

    /// Close source data
    virtual Status Close(void) = 0;

    /// Reset all internal parameters to start reading from begin
    virtual Status Reset(void) = 0;

    /// Return 2 bytes
    Status Get16uSwap(Ipp16u *data);
    /// Return 2 bytes without swap
    Status Get16uNoSwap(Ipp16u *data);

    /// Return 4 bytes
    Status Get32uSwap(Ipp32u *data);
    /// Return 4 bytes without swap
    Status Get32uNoSwap(Ipp32u *data);

    /// Return 8 bytes
    Status Get64uSwap(Ipp64u *data);
    /// Return 8 bytes without swap
    Status Get64uNoSwap(Ipp64u *data);

    /// Return 1 byte
    Status Get8u(Ipp8u *data);

    /// Get data
    Status GetData(void *data, Ipp32u *nsize);

    /**
    Read nsize bytes and copy to data (return number bytes which was copy).
    Cache data in case of small nsize
    */
    virtual Status ReadData(void *data, Ipp32u *nsize) = 0;

    /// Move position on npos bytes
    virtual Status MovePosition(Ipp64u npos) = 0;

    /// Check byte value
    Status Check8u(Ipp8u *ret_byte, size_t how_far);
    /// Check Ipp16s value
    Status Check16u(Ipp16u *ret_short, size_t how_far);
    /// Check Ipp32u value
    Status Check32u(Ipp32u *ret_long, size_t how_far);

    /// Check data
    Status CheckData(void *data, Ipp32u *nsize, Ipp32s how_far);

    // Show data
    size_t ShowData(Ipp8u **data);

    /// Cache and check data
    virtual Status CacheData(void *data, Ipp32u *nsize, Ipp32s how_far) = 0;

    /// Obtain position in the stream
    virtual Ipp64u GetPosition(void) = 0;
    /// Obtain size in source data
    virtual Ipp64u GetSize(void) = 0;
    /// Set new position
    virtual Status SetPosition(Ipp64f pos) = 0;

    /// Set new position
    virtual Status SetPosition (Ipp64u pos)
    {
        Ipp64u curr_pos = GetPosition();
        if (pos >= curr_pos)
        {
            return MovePosition((pos-curr_pos));
        }
        else
        {
            SetPosition(0.0);

            return MovePosition(pos);
        }
    }

    Status StartReadingAfterReposition(void)
    {
        return UMC_OK;
    }

public:
    Ipp8u  *m_pDataPointer;  // Pointer to the current data
    Ipp8u  *m_pEODPointer;   // Pointer to the end of data

protected:
    bool m_bStop;
};

inline
Status DataReader::GetData(void *data, Ipp32u *nsize)
{
    size_t data_sz = (size_t)(*nsize);

    /*if (((size_t)(m_pEODPointer - m_pDataPointer)) >= data_sz)
    {
        memcpy(data, m_pDataPointer, data_sz);
        m_pDataPointer += data_sz;
        return UMC_OK;
    }*/

    Status umcRes = UMC_OK;

    do {
        *nsize = (Ipp32u)data_sz;
        umcRes = ReadData(data, nsize);
    } while (umcRes == UMC_ERR_NOT_ENOUGH_DATA && m_bStop == false);

    return umcRes;
} // Status DataReader::GetData(void *data, Ipp32u *nsize)

inline
Status DataReader::Get8u(Ipp8u *data)
{
    Ipp32u data_sz = 1;

    return GetData(data, &data_sz);

} // Status DataReader::Get8u(Ipp8u *data)

inline
Status DataReader::Get16uNoSwap(Ipp16u *data)
{
    Ipp32u data_sz = 2;
    Status ret = GetData(data, &data_sz);

    *data = BIG_ENDIAN_SWAP16(*data);

    return ret;

} // Status DataReader::Get16uNoSwap(Ipp16u *data)

inline
Status DataReader::Get16uSwap(Ipp16u *data)
{
    Ipp32u data_sz = 2;
    Status ret = GetData(data, &data_sz);

    *data = LITTLE_ENDIAN_SWAP16(*data);

    return ret;

} // Status DataReader::Get16uSwap(Ipp16u *data)

inline
Status DataReader::Get32uNoSwap(Ipp32u *data)
{
    Ipp32u data_sz = 4;
    Status ret = GetData(data,&data_sz);

    *data = BIG_ENDIAN_SWAP32(*data);

    return ret;

} // Status DataReader::Get32uNoSwap(Ipp32u *data)

inline
Status DataReader::Get32uSwap(Ipp32u *data)
{
    Ipp32u data_sz = 4;
    Status ret = GetData(data,&data_sz);

    *data = LITTLE_ENDIAN_SWAP32(*data);
    return ret;

} // Status DataReader::Get32uSwap(Ipp32u *data)

inline
Status DataReader::CheckData(void *data, Ipp32u *nsize, Ipp32s how_far)
{
/*    size_t data_sz = (size_t)(*nsize + how_far);

    if (((size_t)(m_pEODPointer - m_pDataPointer)) >= data_sz)
    {
        memcpy(data, m_pDataPointer + how_far, *nsize);
        return UMC_OK;
    }*/

    return CacheData(data, nsize, how_far);
}

inline
size_t DataReader::ShowData(Ipp8u **data)
{
    *data = m_pDataPointer;
    return (size_t)(m_pEODPointer - m_pDataPointer);
}

inline
Status DataReader::Check8u(Ipp8u *ret_byte, size_t how_far)
{
    Ipp32u data_sz = 1;

    return CheckData(ret_byte, &data_sz, (Ipp32s)how_far);

} // Status DataReader::Check8u(Ipp8u *ret_byte, size_t how_far)

inline
Status DataReader::Check16u(Ipp16u *ret_short, size_t how_far)
{
    Ipp32u data_sz = 2;
    Status ret = CheckData(ret_short, &data_sz, (Ipp32s)how_far);

    *ret_short = LITTLE_ENDIAN_SWAP16(*ret_short);

    return ret;

} // Status DataReader::Check16u(Ipp16u *ret_short, size_t how_far)

inline
Status DataReader::Check32u(Ipp32u *ret_long, size_t how_far)
{
    Ipp32u data_sz = 4;
    Status ret = CheckData(ret_long, &data_sz, (Ipp32s)how_far);

    *ret_long = LITTLE_ENDIAN_SWAP32(*ret_long);

    return ret;

} // Status DataReader::Check32u(Ipp32u *ret_long, size_t how_far)

inline
Status DataReader::Get64uNoSwap(Ipp64u *data)
{
    Ipp32u data_sz = 8;
    Status ret = GetData(data, &data_sz);

    *data = BIG_ENDIAN_SWAP64(*data);

    return ret;

} // Status DataReader::Get64uNoSwap(Ipp64u *data)

inline
Status DataReader::Get64uSwap(Ipp64u *data)
{
    Ipp32u data_sz = 8;
    Status ret = GetData(data, &data_sz);

    *data = LITTLE_ENDIAN_SWAP64(*data);

    return ret;

} // Status DataReader::Get64uSwap(Ipp64u *data)

} // namespace UMC

#endif /* __UIMC_DATA_READER_H__ */
