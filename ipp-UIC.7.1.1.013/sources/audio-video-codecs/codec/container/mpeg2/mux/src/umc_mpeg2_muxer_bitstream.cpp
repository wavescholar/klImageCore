/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_MUXER

#include "umc_mpeg2_muxer_bitstream.h"

#include "ipps.h"

using namespace UMC;

AdvancedBitStream::AdvancedBitStream(void)
{
    Reset(NULL, 0);
} //AdvancedBitStream::AdvancedBitStream(void)

void AdvancedBitStream::Reset(void *lpvBuffer, Ipp32s lSize, bool bReset)
{
    if (bReset)
        ippsSet_8u(0, (Ipp8u *)lpvBuffer, lSize);
    m_lpbBuffer = reinterpret_cast<Ipp8u *> (lpvBuffer);
    m_lpb = m_lpbBuffer;
    m_lBufferLength = lSize;
    m_bRegistr = 0;
    m_lRemainBits = 8;
} //void AdvancedBitStream::Reset(void *lpvBuffer, Ipp32s lSize, bool bReset)

void AdvancedBitStream::SetPointer(Ipp32s lBitsFromBegining)
{
    if ((lBitsFromBegining / 8) < m_lBufferLength)
    {
        m_lpb = m_lpbBuffer + lBitsFromBegining / 8;
        m_bRegistr = *m_lpb;
        m_lRemainBits = 8 - (lBitsFromBegining % 8);
    }
} //void AdvancedBitStream::SetPointer(Ipp32s lBitsFromBegining)

void AdvancedBitStream::SkipBits(Ipp32s lSkip)
{
    if (m_lpbBuffer + m_lBufferLength > m_lpb)
    {
        if (m_lRemainBits <= lSkip)
        {
            lSkip -= m_lRemainBits;
            m_lRemainBits = 8;
            m_lpb += 1;
            m_bRegistr = *m_lpb;

            if (lSkip)
                SkipBits(lSkip);
        }
        else
            m_lRemainBits -= lSkip;
    }
} //void AdvancedBitStream::SkipBits(Ipp32s lSkip)

void AdvancedBitStream::AddBits(Ipp32u nBits, Ipp32s lCount)
{
    if (m_lpbBuffer + m_lBufferLength > m_lpb)
    {
        if (lCount < m_lRemainBits)
        {
            m_lRemainBits -= lCount;
            nBits <<= m_lRemainBits;
            m_bRegistr &= ~((~((0xffffffff) << lCount)) << m_lRemainBits);
            m_bRegistr |= (Ipp8u)(nBits);
            *m_lpb = m_bRegistr;
        }
        else
        {
            Ipp32u nPrefix;

            nPrefix = nBits >> (lCount - m_lRemainBits);
            m_bRegistr &= ((0xffffffff) << (m_lRemainBits));
            m_bRegistr |= (Ipp8u)(nPrefix);
            *m_lpb = m_bRegistr;
            lCount -= m_lRemainBits;

            m_lRemainBits = 8;
            m_lpb += 1;
            m_bRegistr = *m_lpb;

            if (lCount)
            {
                Ipp32u nPostfix;

                nPostfix = nBits & ~(0xffffffff << lCount);

                AddBits(nPostfix, lCount);
            }
        }
    }
} //void AdvancedBitStream::AddBits(Ipp32u nBits, Ipp32s lCount)

#endif
