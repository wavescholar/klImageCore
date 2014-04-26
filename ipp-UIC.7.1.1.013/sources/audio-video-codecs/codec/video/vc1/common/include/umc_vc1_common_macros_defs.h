/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder macros defines
//
*/

#ifndef __UMC_VC1_COMMON_MACROS_DEFS_H__
#define __UMC_VC1_COMMON_MACROS_DEFS_H__

#include <stdio.h>
#include "ippdefs.h"

#define VC1_CLIP(x) (!(x&~255)?x:(x<0?0:255))
#define VC1_SIGN(x) ((x<0)?-1:(x>0?1:0))
#define VC1_LUT_SET(value,lut) (lut[value])
//#define align_value<Ipp32u>(x) ((x&3)?((x&0xFFFFFFFC) + 4):x)

namespace UMC
{
    class VC1BitstreamParser
    {
    public:
        template <class T>
        inline static void GetNBits(Ipp32u* &pCurrentData, Ipp32s& offset, Ipp32u nbits, T &data)
        {
            Ipp32u x;
            offset -= (nbits);

            if(offset >= 0)
            {
                x = pCurrentData[0] >> (offset + 1);
            }
            else
            {
                offset += 32;

                x = pCurrentData[1] >> (offset);
                x >>= 1;
                x += pCurrentData[0] << (31 - offset);
                pCurrentData++;
            }
            data = T(x & ((0x00000001 << (nbits&0x1F)) - 1));
        }
        template <class T>
        inline static void CheckNBits(Ipp32u* pCurrentData, Ipp32u offset, Ipp32u nbits, T &data)
        {
            Ipp32s bp;
            Ipp32u x;


            bp = offset - (nbits);

            if(bp < 0)
            {
                bp += 32;
                x = pCurrentData[1] >> bp;
                x >>= 1;
                x += pCurrentData[0] << (31 - bp);
            }
            else
            {
                x = pCurrentData[0] >> bp;
                x >>= 1;
            }

            data = T(x & ((0x00000001 << (nbits&0x1F)) - 1));
        }

    };

}
// from histrorical reasons. Unsafety work with bitstream
#define VC1NextNBits( current_data, offset, nbits, data) __VC1NextBits(current_data, offset, nbits, data)
#define __VC1GetBits(current_data, offset, nbits, data)                 \
{                                                                       \
    Ipp32u x;                                                           \
                                                                        \
    VM_ASSERT((nbits) >= 0 && (nbits) <= 32);                           \
    VM_ASSERT(offset >= 0 && offset <= 31);                             \
                                                                        \
    offset -= (nbits);                                                  \
                                                                        \
    if(offset >= 0)                                                     \
    {                                                                   \
        x = current_data[0] >> (offset + 1);                            \
    }                                                                   \
    else                                                                \
    {                                                                   \
        offset += 32;                                                   \
                                                                        \
        x = current_data[1] >> (offset);                                \
        x >>= 1;                                                        \
        x += current_data[0] << (31 - offset);                          \
        current_data++;                                                 \
    }                                                                   \
                                                                        \
    VM_ASSERT(offset >= 0 && offset <= 31);                             \
                                                                        \
    (data) = x & ((0x00000001 << (nbits&0x1F)) - 1);                    \
}

#define VC1GetNBits( current_data, offset, nbits, data) __VC1GetBits(current_data, offset, nbits, data)
#define __VC1NextBits(current_data, offset, nbits, data)                  \
{                                                                       \
    Ipp32s bp;                                                          \
    Ipp32u x;                                                           \
                                                                        \
    VM_ASSERT((nbits) >= 0 && (nbits) <= 32);                           \
    VM_ASSERT(offset >= 0 && offset <= 31);                             \
                                                                        \
    bp = offset - (nbits);                                              \
                                                                        \
    if(bp < 0)                                                          \
    {                                                                   \
        bp += 32;                                                       \
        x = current_data[1] >> bp;                                      \
        x >>= 1;                                                        \
        x += current_data[0] << (31 - bp);                              \
    }                                                                   \
    else                                                                \
    {                                                                   \
        x = current_data[0] >> bp;                                      \
        x >>= 1;                                                        \
    }                                                                   \
                                                                        \
    (data) = x & ((0x00000001 << (nbits&0x1F)) - 1);                    \
}

#endif //__VC1_DEC_MACROS_DEFS_H__
