/*
//
//                INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UIC_TRANSCODER_CON_H__
#define __UIC_TRANSCODER_CON_H__

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif

#define PRINT_DEC 0
#define PRINT_ENC 1

inline Ipp32u AlignStep(Ipp32u step) { return (step + 31) & 0xFFFFFFE0; }


template<class T> T* addrAdd(const T* ptr, int bytes)
{
    return (T*)(&((char*)ptr)[bytes]);
}


template<class T> void addrInc(T& ptr, int bytes)
{
    ptr = addrAdd(ptr, bytes);
}


void Convert_P3C3_32s_8u(
    const Ipp32s *src0, int srcStep0,
    const Ipp32s *src1, int srcStep1,
    const Ipp32s *src2, int srcStep2,
    Ipp8u        *dst,  int dstStep,
    Ipp32u width, Ipp32u height)
{
    for(Ipp32u y = 0; y < height; y++)
    {
        for(Ipp32u x = 0; x < width; x++)
        {
            dst[3*x + 0] = (Ipp8u)src0[x];
            dst[3*x + 1] = (Ipp8u)src1[x];
            dst[3*x + 2] = (Ipp8u)src2[x];
        }

        addrInc(src0, srcStep0);
        addrInc(src1, srcStep1);
        addrInc(src2, srcStep2);

        addrInc(dst, dstStep);
    }

    return;
} // Convert_P3C3_32s_8u()

#endif // __UIC_TRANSCODER_CON_H__

