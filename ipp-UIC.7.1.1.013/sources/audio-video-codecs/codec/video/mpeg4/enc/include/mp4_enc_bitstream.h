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

#ifndef __UMC_MP4_ENC_BITSTREAM_H__
#define __UMC_MP4_ENC_BITSTREAM_H__

#include <string.h>

#include "ippdefs.h"

#if defined(WINDOWS)
#pragma warning(disable : 4514)     // unreferenced inline function has been removed
#endif

// class for BitStream
class BitStream
{
public :
    Ipp8u*  mPtr;
    Ipp8u*  mBuffer;
    Ipp32s  mBitOff;
    Ipp32s  mBuffSize;
    void    Init(Ipp8u *ptr, Ipp32s size) { mBuffer = mPtr = ptr; mBitOff = 0; mBuffSize = size; };
    void    Reset() { mPtr = mBuffer; mBitOff = 0; };
    Ipp8u*  GetPtr() { return mBuffer; };
    Ipp32s  GetFullness() { return (Ipp32s)(mPtr - mBuffer); };
    Ipp8u*  GetBitPtr() { return mPtr; };
    Ipp32s  GetBitOff() { return mBitOff; };
    void    GetPos(Ipp8u **ptr, Ipp32s *bitOff) { *ptr = mPtr; *bitOff = mBitOff; };
    void    SetPos(Ipp8u *ptr, Ipp32s bitOff = 0) { mPtr = ptr; mBitOff = bitOff; };
    Ipp32s  GetNumBits() { return ((Ipp32s)((mPtr - mBuffer) << 3) + mBitOff); };
    Ipp32s  GetNumBits(Ipp8u *ptr, Ipp32s off) { return ((Ipp32s)((mPtr - ptr) << 3) + mBitOff - off); };
    void    MovePtr(Ipp32s nBits) { mPtr += (mBitOff + nBits) >> 3; mBitOff = (mBitOff + nBits) & 7; };
    void    PutBits(Ipp32u val, Ipp32s n);
    void    PutBit(Ipp32u val);
    void    PutZeroBit() { PutBit(0); };
    void    PutMarkerBit() { PutBit(1); };
    inline void PutStr(const Ipp8s* str);
    BitStream() { mBuffer = mPtr = 0; mBitOff = mBuffSize = 0; };
};

inline void BitStream::PutStr(const Ipp8s *str)
{
    if (mBitOff > 0) {
        mBitOff = 0;
        mPtr ++;
    }
#if (_MSC_VER >= 1500)
    strcpy_s((char*)mPtr, (size_t)(mBuffSize - (mPtr-mBuffer)), (const char*)str);
#else
    strcpy((char *)mPtr, (char *)str);
#endif
    mPtr += strlen((char *)str);
}

#endif
