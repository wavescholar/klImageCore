/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    class ippVideoBitStream
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG4_VIDEO_ENCODER

#include "mp4_enc_bitstream.h"

#if /*(_WIN32_WCE == 500) && */(_MSC_VER == 1201)
// INTERNAL COMPILER ERROR
#pragma optimize ("g", off)
#endif

void BitStream::PutBits(Ipp32u val, Ipp32s n)
{
    val <<= 32 - n;
    if (mBitOff == 0) {
        mPtr[0] = (Ipp8u)(val >> 24);
        if (n > 8) {
            mPtr[1] = (Ipp8u)(val >> 16);
            if (n > 16) {
                mPtr[2] = (Ipp8u)(val >> 8);
                if (n > 24) {
                    mPtr[3] = (Ipp8u)(val);
                }
            }
        }
    } else {
        mPtr[0] = (Ipp8u)((mPtr[0] & (0xFF << (8 - mBitOff))) | (Ipp8u)(val >> (24 + mBitOff)));
        if (n > 8 - mBitOff) {
            val <<= 8 - mBitOff;
            mPtr[1] = (Ipp8u)(val >> 24);
            if (n > 16 - mBitOff) {
                mPtr[2] = (Ipp8u)(val >> 16);
                if (n > 24 - mBitOff) {
                    mPtr[3] = (Ipp8u)(val >> 8);
                    if (n > 32 - mBitOff) {
                        mPtr[4] = (Ipp8u)val;
                    }
                }
            }
        }
    }
    mPtr += (mBitOff + n) >> 3;
    mBitOff = (mBitOff + n) & 7;
}

void BitStream::PutBit(Ipp32u val)
{
    if (mBitOff == 0) {
        mPtr[0] = (Ipp8u)((val & 1) ? 0x80 : 0x00);
        mBitOff ++;
    } else {
        if (val & 1)
            mPtr[0] = (Ipp8u)(mPtr[0] | (Ipp8u)(0x01 << (7 - mBitOff)));
        else
            mPtr[0] = (Ipp8u)(mPtr[0] & (Ipp8u)(0xff << (8 - mBitOff)));
        mPtr += (mBitOff + 1) >> 3;
        mBitOff = (mBitOff + 1) & 7;
    }
}

#endif //defined (UMC_ENABLE_MPEG4_VIDEO_ENCODER)
