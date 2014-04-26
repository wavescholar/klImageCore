/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_BITSTREAM_INLINES_H
#define __UMC_H264_BITSTREAM_INLINES_H

#include "umc_h264_bitstream.h"
#include "umc_h264_dec_init_tables_cabac.h"
#include "umc_h264_dec_tables.h"

using namespace UMC_H264_DECODER;

namespace UMC
{

#define _h264GetBits(current_data, offset, nbits, data) \
{ \
    Ipp32u x; \
 \
    VM_ASSERT((nbits) > 0 && (nbits) <= 32); \
    VM_ASSERT(offset >= 0 && offset <= 31); \
 \
    offset -= (nbits); \
 \
    if (offset >= 0) \
    { \
        x = current_data[0] >> (offset + 1); \
    } \
    else \
    { \
        offset += 32; \
 \
        x = current_data[1] >> (offset); \
        x >>= 1; \
        x += current_data[0] << (31 - offset); \
        current_data++; \
    } \
 \
    VM_ASSERT(offset >= 0 && offset <= 31); \
 \
    (data) = x & bits_data[nbits]; \
}

#define ippiSkipNBits(current_data, offset, nbits) \
{ \
    /* check error(s) */ \
    VM_ASSERT((nbits) > 0 && (nbits) <= 32); \
    VM_ASSERT(offset >= 0 && offset <= 31); \
    /* decrease number of available bits */ \
    offset -= (nbits); \
    /* normalize bitstream pointer */ \
    if (0 > offset) \
    { \
        offset += 32; \
        current_data++; \
    } \
    /* check error(s) again */ \
    VM_ASSERT(offset >= 0 && offset <= 31); \
 }


#define ippiGetBits1(current_data, offset, data) \
{ \
    data = ((current_data[0] >> (offset)) & 1);  \
    offset -= 1; \
    if (offset < 0) \
    { \
        offset = 31; \
        current_data += 1; \
    } \
}

#define ippiGetBits8( current_data, offset, data) \
    _h264GetBits(current_data, offset, 8, data);

#define ippiGetNBits( current_data, offset, nbits, data) \
    _h264GetBits(current_data, offset, nbits, data);

#define ippiUngetNBits(current_data, offset, nbits) \
{ \
    VM_ASSERT(offset >= 0 && offset <= 31); \
 \
    offset += (nbits); \
    if (offset > 31) \
    { \
        offset -= 32; \
        current_data--; \
    } \
 \
    VM_ASSERT(offset >= 0 && offset <= 31); \
}

#define ippiUngetBits32(current_data, offset) \
    VM_ASSERT(offset >= 0 && offset <= 31); \
    current_data--;

#define ippiAlignBSPointerRight(current_data, offset) \
{ \
    if ((offset & 0x07) != 0x07) \
    { \
        offset = (offset | 0x07) - 8; \
        if (offset == -1) \
        { \
            offset = 31; \
            current_data++; \
        } \
    } \
}

#define ippiNextBits(current_data, bp, nbits, data) \
{ \
    Ipp32u x; \
 \
    VM_ASSERT((nbits) > 0 && (nbits) <= 32); \
    VM_ASSERT(nbits >= 0 && nbits <= 31); \
 \
    Ipp32s offset = bp - (nbits); \
 \
    if (offset >= 0) \
    { \
        x = current_data[0] >> (offset + 1); \
    } \
    else \
    { \
        offset += 32; \
 \
        x = current_data[1] >> (offset); \
        x >>= 1; \
        x += current_data[0] << (31 - offset); \
    } \
 \
    VM_ASSERT(offset >= 0 && offset <= 31); \
 \
    (data) = x & bits_data[nbits]; \
}

#define RefreshCABACBits(codOffset, pBits, iBits) \
{ \
    Ipp16u *pRealPointer; \
    /* we have to handle the bit pointer very thorougly. */ \
    /* this sophisticated logic is used to avoid compilers' warnings. */ \
    /* In two words we just select required word by the pointer */ \
    pRealPointer = (Ipp16u *) (((Ipp8u *) 0) + \
                               ((((Ipp8u *) pBits) - (Ipp8u *) 0) ^ 2)); \
    codOffset |= *(pRealPointer) << (-iBits); \
    pBits += 1; \
    iBits += 16; \
}

inline
void H264Bitstream::AlignPointerRight(void)
{
    ippiAlignBSPointerRight(m_pbs, m_bitOffset);

} // void H264Bitstream::AlignPointerRight(void)


inline
Ipp32u H264Bitstream::Peek1Bit()
{
    return h264Peek1Bit(m_pbs, m_bitOffset);
} // H264Bitstream::GetBits()

inline
void H264Bitstream::Drop1Bit()
{
    h264Drop1Bit(m_pbs, m_bitOffset);
} // H264Bitstream::GetBits()

inline
bool H264Bitstream::NextBit()
{
    Ipp32s bp;
    Ipp32u w;

    bp = m_bitOffset - 1;

    if (bp < 0)
    {
        w = m_pbs[0] & 1;
        if (w)
        {
            m_pbs++;
            m_bitOffset = 31;
            return true;
        }
    }
    else
    {
        w = m_pbs[0] >> m_bitOffset;
        w = w & 1;
        if (w)
        {
            m_bitOffset = bp;
            return true;
        }
    }
    return false;

} // H264Bitstream::SearchBits()

inline
Ipp32u H264Bitstream::DecodeSingleBin_CABAC(Ipp32u ctxIdx)
{
    Ipp32u codIOffset = m_lcodIOffset;
    Ipp32u codIRange = m_lcodIRange;
    Ipp32u codIRangeLPS;
    Ipp32u pState = context_array[ctxIdx].pStateIdxAndVal;
    Ipp32u binVal;

#ifdef STORE_CABAC_BITS
    Ipp8u preState = context_array[ctxIdx].pStateIdxAndVal;
#endif

#ifdef CABAC_OPTIMIZATION
    codIRangeLPS = rangeTabLPS[pState >> 1][(codIRange >> (6 + CABAC_MAGIC_BITS)) - 4];
#else
    codIRangeLPS = rangeTabLPS[pState][(codIRange >> (6 + CABAC_MAGIC_BITS)) - 4];
#endif
    codIRange -= codIRangeLPS << CABAC_MAGIC_BITS;

    // most probably state.
    // it is more likely to decode most probably value.
    if (codIOffset < codIRange)
    {
        binVal = pState & 1;
        context_array[ctxIdx].pStateIdxAndVal = transIdxMPS[pState];

#ifndef STORE_CABAC_BITS

        // there is no likely case.
        // we take new bit with 50% probability.

        {
            Ipp32s numBits = NumBitsToGetTableSmall[codIRange >> (CABAC_MAGIC_BITS + 7)];

            codIRange <<= numBits;
            codIOffset <<= numBits;
            m_lcodIOffset = codIOffset;
            m_lcodIRange = codIRange;

#if (CABAC_MAGIC_BITS > 0)
            {
                Ipp32s iMagicBits;

                iMagicBits = m_iMagicBits - numBits;
                // in most cases we don't require to refresh cabac variables.
                if (iMagicBits)
                {
                    m_iMagicBits = iMagicBits;
                    return binVal;
                }

                RefreshCABACBits(m_lcodIOffset, m_pMagicBits, iMagicBits);
                m_iMagicBits = iMagicBits;
            }

            return binVal;

#else // !(CABAC_MAGIC_BITS > 0)

            m_lcodIOffset |= GetBits(numBits);
            return binVal;

#endif // (CABAC_MAGIC_BITS > 0)

        }

#endif // STORE_CABAC_BITS

    }
    else
    {
        codIOffset -= codIRange;
        codIRange = codIRangeLPS << CABAC_MAGIC_BITS;

        binVal = (pState & 1) ^ 1;
        context_array[ctxIdx].pStateIdxAndVal = transIdxLPS[pState];
    }

    // Renormalization process
    // See subclause 9.3.3.2.2 of H.264
    //if (codIRange < (0x100<<(CABAC_MAGIC_BITS)))
    {
#ifdef CABAC_OPTIMIZATION
            Ipp32u numBits = 0;
            Ipp32u numBitsIndex = codIRange >> CABAC_MAGIC_BITS;
            if(numBitsIndex)
                BSR(numBits, numBitsIndex);
#else
            Ipp32u numBits = NumBitsToGetTbl[codIRange >> CABAC_MAGIC_BITS];
#endif

        codIRange <<= numBits;
        codIOffset <<= numBits;

#if (CABAC_MAGIC_BITS > 0)
        {
            Ipp32s iMagicBits;

            iMagicBits = m_iMagicBits - numBits;

            if (0 >= iMagicBits)
            {
                RefreshCABACBits(codIOffset, m_pMagicBits, iMagicBits);
                m_iMagicBits = iMagicBits;
            }
            else
                m_iMagicBits = iMagicBits;
        }
#else // !(CABAC_MAGIC_BITS > 0)
        codIOffset |= GetBits(numBits);
#endif // (CABAC_MAGIC_BITS > 0)
    }

#ifdef STORE_CABAC_BITS
    sym_cnt++;

    if (cabac_bits==NULL) cabac_bits=fopen(__CABAC_FILE__,"w+t");
    if (cabac_bits)
#ifdef CABAC_DECORER_COMP
        fprintf(cabac_bits,"sb %d %d %d %d %d\n",
        ctxIdx,
        codIRange>>CABAC_MAGIC_BITS,
        codIOffset>>CABAC_MAGIC_BITS,
        binVal,sym_cnt);
#else
        fprintf(cabac_bits,"sb %d %d %d %d %d %d %d\n",ctxIdx,preState>>1,preState&1,context_array[ctxIdx].pStateIdxAndVal>>1,context_array[ctxIdx].pStateIdxAndVal&1,binVal,sym_cnt);
#endif
    fflush(cabac_bits);
#endif

    m_lcodIOffset = codIOffset;
    m_lcodIRange = codIRange;

    return binVal;

} //Ipp32s H264Bitstream::DecodeSingleBin_CABAC(Ipp32s ctxIdx)

inline
Ipp32u H264Bitstream::DecodeSymbolEnd_CABAC(void)
{
    Ipp32u binVal = 1;
    Ipp32u codIOffset = m_lcodIOffset;
    Ipp32u codIRange = m_lcodIRange;

    // See subclause 9.3.3.2.4 of H.264 standard
    if (codIOffset < (codIRange - (2 << CABAC_MAGIC_BITS)))
    {
        codIRange -= (2 << CABAC_MAGIC_BITS);

        // Renormalization process
        // See subclause 9.3.3.2.2 of H.264
        if (codIRange < (0x100 << (CABAC_MAGIC_BITS)))
        {
            codIRange <<= 1;
            codIOffset <<= 1;

#if (CABAC_MAGIC_BITS > 0)
            m_iMagicBits -= 1;
            if (0 >= m_iMagicBits)
                RefreshCABACBits(codIOffset, m_pMagicBits, m_iMagicBits);
#else // !(CABAC_MAGIC_BITS > 0)
            codIOffset |= GetBits(1);
#endif // (CABAC_MAGIC_BITS > 0)
        }
        binVal = 0;
        m_lcodIOffset = codIOffset;
        m_lcodIRange = codIRange;

    }

#ifdef STORE_CABAC_BITS
    sym_cnt++;

    if (cabac_bits==NULL) cabac_bits=fopen(__CABAC_FILE__,"w+t");
    if (cabac_bits)
#ifdef CABAC_DECORER_COMP
        fprintf(cabac_bits,"fsb %d %d %d %d\n",m_lcodIRange>>CABAC_MAGIC_BITS,
                                               m_lcodIOffset>>CABAC_MAGIC_BITS,
                                               binVal,sym_cnt);
#else
    fprintf(cabac_bits,"fsb %d %d\n",binVal,sym_cnt);
#endif
    fflush(cabac_bits);
#endif

    return binVal;

} //Ipp32s H264Bitstream::DecodeSymbolEnd_CABAC(void)

inline
Ipp32u H264Bitstream::DecodeBypass_CABAC(void)
{
    // See subclause 9.3.3.2.3 of H.264 standard
    Ipp32u binVal;
#if (CABAC_MAGIC_BITS > 0)
    m_lcodIOffset = (m_lcodIOffset << 1);

    m_iMagicBits -= 1;
    if (0 >= m_iMagicBits)
        RefreshCABACBits(m_lcodIOffset, m_pMagicBits, m_iMagicBits);
#else // !(CABAC_MAGIC_BITS > 0)
    m_lcodIOffset = (m_lcodIOffset << 1) | Get1Bit();
#endif // (CABAC_MAGIC_BITS > 0)

    if (m_lcodIOffset >= m_lcodIRange)
    {
        binVal = 1;
        m_lcodIOffset -= m_lcodIRange;
   }
    else
    {
        binVal = 0;
    }

#ifdef STORE_CABAC_BITS
    sym_cnt++;
    if (cabac_bits==NULL) cabac_bits=fopen(__CABAC_FILE__,"w+t");
    if (cabac_bits)
#ifdef CABAC_DECORER_COMP
        fprintf(cabac_bits,"bp %d %d %d %d\n",m_lcodIRange>>CABAC_MAGIC_BITS,
                                              m_lcodIOffset>>CABAC_MAGIC_BITS,
                                              binVal,sym_cnt);
#else
    fprintf(cabac_bits,"bp %d %d\n",binVal,sym_cnt);
#endif
    fflush(cabac_bits);
#endif

    return binVal;

} //Ipp32s H264Bitstream::DecodeBypass_CABAC(void)

inline
Ipp32s H264Bitstream::DecodeBypassSign_CABAC(Ipp32s val)
{
    // See subclause 9.3.3.2.3 of H.264 standard
    Ipp32s binVal;

#if (CABAC_MAGIC_BITS > 0)
    m_lcodIOffset = (m_lcodIOffset << 1);

    m_iMagicBits -= 1;
    if (0 >= m_iMagicBits)
        RefreshCABACBits(m_lcodIOffset, m_pMagicBits, m_iMagicBits);
#else
    m_lcodIOffset = (m_lcodIOffset << 1) | Get1Bit();
#endif

    if (m_lcodIOffset >= m_lcodIRange)
    {
        binVal = -val;
        m_lcodIOffset -= m_lcodIRange;
   }
    else
    {
        binVal = val;
    }
#ifdef STORE_CABAC_BITS
    sym_cnt++;
    if (cabac_bits==NULL) cabac_bits=fopen(__CABAC_FILE__,"w+t");
    if (cabac_bits)
#ifdef CABAC_DECORER_COMP
        fprintf(cabac_bits,"bp %d %d %d %d\n",m_lcodIRange>>CABAC_MAGIC_BITS,
                                              m_lcodIOffset>>CABAC_MAGIC_BITS,
                                              binVal<0,sym_cnt);
#else
    fprintf(cabac_bits,"bp %d %d\n",binVal<0,sym_cnt);
#endif
    fflush(cabac_bits);
#endif

    return binVal;

} // Ipp32s H264Bitstream::DecodeBypassSign_CABAC()

inline
Ipp32u H264BaseBitstream::GetBits(const Ipp32u nbits)
{
    Ipp32u w, n = nbits;

    ippiGetNBits(m_pbs, m_bitOffset, n, w);
    return(w);
}

inline
Ipp32s H264BaseBitstream::GetVLCElement(bool bIsSigned)
{
    Ipp32s sval = 0;

    IppStatus ippRes = ippiDecodeExpGolombOne_H264_1u32s(&m_pbs, &m_bitOffset, &sval, bIsSigned);

    if (ippStsNoErr > ippRes)
        throw h264_exception(UMC_ERR_INVALID_STREAM);

    return sval;

}

inline
Ipp32u H264BaseBitstream::Get1Bit()
{
    Ipp32u w;

    ippiGetBits1(m_pbs, m_bitOffset, w);
    return(w);

} // H264Bitstream::Get1Bit()

inline size_t H264BaseBitstream::BytesDecoded()
{
    return (size_t)((Ipp8u*)m_pbs - (Ipp8u*)m_pbsBase) + ((31 - m_bitOffset) >> 3);
}

inline size_t H264BaseBitstream::BitsDecoded()
{
    return (size_t)((Ipp8u*)m_pbs - (Ipp8u*)m_pbsBase) * 8 + (31 - m_bitOffset);
}

inline size_t H264BaseBitstream::BytesLeft()
{
    return ((Ipp32s)m_maxBsSize - (Ipp32s) BytesDecoded());
}

} // namespace UMC

#endif // __UMC_H264_BITSTREAM_INLINES_H
