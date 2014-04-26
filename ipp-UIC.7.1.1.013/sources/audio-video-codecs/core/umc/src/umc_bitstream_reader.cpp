/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_bitstream_reader.h"

using namespace UMC;


BitstreamReader::BitstreamReader(void)
{
    m_pReader    = NULL;
    m_iCache     = 0;
    m_iCacheSize = 0;
}

Status BitstreamReader::Init(DataReader *pReader)
{
    if(!pReader)
        return UMC_ERR_INVALID_PARAMS;

    m_pReader    = pReader;
    m_iCache     = 0;
    m_iCacheSize = 0;

    return UMC_OK;
}

Ipp32u BitstreamReader::PeekBits(Ipp32u iNum)
{
    if(!iNum)
        return 0;

    if(iNum > m_iCacheSize)
        FillCache();

    return ((m_iCache >> (m_iCacheSize - iNum)) & (((Ipp64u)(-1)) >> (64 - iNum)));
}


Ipp32u BitstreamReader::GetBits(Ipp32u iNum)
{
    if(!iNum)
        return 0;

    if(iNum > m_iCacheSize)
        FillCache();

    m_iCacheSize -= iNum;
    return ((m_iCache >> m_iCacheSize) & (((Ipp64u)(-1)) >> (64 - iNum)));
}

void BitstreamReader::SkipBits(Ipp32u iNum)
{
    if(!iNum)
        return;

    if (iNum <= m_iCacheSize)
        m_iCacheSize -= iNum;
    else
    {
        iNum -= m_iCacheSize;
        m_pReader->MovePosition(iNum >> 3);
        m_iCacheSize = 0;
        FillCache();
        m_iCacheSize -= iNum & 0x7;
    }
}

void BitstreamReader::AlignReader(void)
{
    if(m_iCacheSize)
    {
        m_pReader->MovePosition(-(Ipp64s)(m_iCacheSize/8));
        m_iCache >>= (m_iCacheSize/8)*8;
        m_iCacheSize %= 8;
    }
}

Ipp32u BitstreamReader::GetUE(void)
{
    Ipp32s iZeros;

    // count leading zeros
    iZeros = 0;
    while(0 == PeekBits(1))
    {
        iZeros++;
        SkipBits(1);
    }

    // get value
    return (GetBits(iZeros + 1) - 1);
}

Ipp32s BitstreamReader::GetSE(void)
{
    Ipp32s iZeros;
    Ipp32s iValue;

    // count leading zeros
    iZeros = 0;
    while (0 == PeekBits(1))
    {
        iZeros++;
        SkipBits(1);
    }

    // get value
    iValue = GetBits(iZeros);
    return ((GetBits(1)) ? (-iValue) : (iValue));
}

void BitstreamReader::FillCache()
{
    Ipp8u  iByte;
    size_t iSize = 1;
    while(56 >= m_iCacheSize)
    {
        m_pReader->GetData(&iByte, iSize);
        m_iCache = (m_iCache << 8) | iByte;
        m_iCacheSize += 8;
    }
}
