/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_BITSTREAM_READER_H__
#define __UMC_BITSTREAM_READER_H__

#include "umc_defs.h"
#include "umc_data_reader.h"

namespace UMC
{

class BitstreamReader
{
public:
    BitstreamReader(void);
    virtual ~BitstreamReader(void) {}

    // Initialize bit stream reader from DataReader
    virtual Status Init(DataReader *pReader);

    virtual Ipp32u PeekBits(Ipp32u iNum);

    virtual Ipp32u GetBits(Ipp32u iNum);

    virtual void SkipBits(Ipp32u iNum);

    // Move reader to last unread byte
    void AlignReader();

    // Get unsigned integer Exp-Golomb-coded element (move pointer)
    Ipp32u GetUE(void);

    // Get signed integer Exp-Golomb-coded element (move pointer)
    Ipp32s GetSE(void);

    // Get number of full unread bytes in stream
    Ipp64u GetBytesSize(void) {return (m_pReader->GetSize() - m_pReader->GetPosition() - m_iCacheSize/8);}

    // Get number of unread bits in stream
    Ipp64u GetBitsSize(void) {return ((m_pReader->GetSize() - m_pReader->GetPosition())*8 + m_iCacheSize);}

    // Get position in stream
    Ipp64u GetPosition(void) {return (m_pReader->GetPosition() - m_iCacheSize/8);}

    // Empty bit cache;
    void Flush() { m_iCacheSize = 0; }

    // Align bit cache;
    void FlushToByte() { m_iCacheSize -= m_iCacheSize%8; }

protected:
    virtual void FillCache();

    DataReader *m_pReader;
    Ipp64u      m_iCache;
    Ipp32u      m_iCacheSize;
};

}
#endif
