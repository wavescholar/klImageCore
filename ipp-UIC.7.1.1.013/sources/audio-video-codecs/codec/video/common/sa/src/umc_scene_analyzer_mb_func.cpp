/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_scene_analyzer_mb_func.h"

namespace UMC
{

#define temp eax
#define counter ecx
#define src_step edx
#define src esi

Ipp32u ippiGetIntraBlockDeviation_4x4_8u(const Ipp8u *pSrc, Ipp32s srcStep)
{
#if defined WINDOWS && defined IA32
    __asm
    {
        mov src, dword ptr [pSrc]
        mov src_step, dword ptr [srcStep]
        lea temp, [src_step + src_step * 02h]
        pxor mm4, mm4
        pxor mm5, mm5
        pxor mm6, mm6
        pxor mm7, mm7

        movd mm0, qword ptr [src]
        movd mm1, qword ptr [src + src_step]
        movd mm2, qword ptr [src + src_step * 02h]
        movd mm3, qword ptr [src + temp]
        psadbw mm4, mm0
        psadbw mm5, mm1
        psadbw mm6, mm2
        psadbw mm7, mm3
        paddd mm4, mm5
        paddd mm4, mm6
        paddd mm4, mm7

        mov temp, 08h
        movd mm5, temp
        paddsw mm4, mm5
        psraw mm4, 04h
        punpcklbw mm4, mm4
        punpcklbw mm4, mm4
        movq mm5, mm4
        movq mm6, mm4
        movq mm7, mm4

        psadbw mm4, mm0
        psadbw mm5, mm1
        psadbw mm6, mm2
        psadbw mm7, mm3
        paddd mm4, mm5
        paddd mm4, mm6
        paddd mm4, mm7

        movd temp, mm4

        emms
    }
#else
    const Ipp8u *pCur;
    Ipp32u average;
    Ipp32u deviation;
    Ipp32u x, y;

    // check error(s)
    if (0 == pSrc)
        return 0;

    // reset variables
    average = 0;
    deviation = 0;

    //
    // get 4x4 block average
    //

    // cycle over rows
    pCur = pSrc;
    for (y = 0; y < 4; y += 1)
    {
        // cycle in the row
        for (x = 0; x < 4; x += 1)
        {
            // getting frame mode sum of components
            average += pCur[x];
        }

        // advance source pointer
        pCur += srcStep;
    }

    // get the average
    average = (average + 8) / 16;

    //
    // get 4x4 block deviation
    //

    // cycle over rows
    pCur = pSrc;
    for (y = 0; y < 4; y += 1)
    {
        // cycle in the row
        for (x = 0; x < 4; x += 1)
        {
            Ipp32s temp;
            Ipp32s signExtended;

            // getting sum of components
            temp = pCur[x] - average;
            signExtended = temp >> 31;

            // we use simple scheme of abs calculation,
            // using one ADD and one XOR operation
            deviation += (temp ^ signExtended) - signExtended;
        }

        // advance source pointer
        pCur += srcStep;
    }

    return deviation;
#endif
}

Ipp32u ippiGetAverage4x4_8u(const Ipp8u *pSrc, Ipp32s srcStep)
{
#if defined WINDOWS && defined IA32
    __asm
    {
        mov src, dword ptr [pSrc]
        mov src_step, dword ptr [srcStep]
        pxor mm4, mm4

        movd mm0, qword ptr [src]
        movd mm1, qword ptr [src + src_step]
        lea src, [src + src_step * 02h]
        movd mm2, qword ptr [src]
        movd mm3, qword ptr [src + src_step]
        psadbw mm0, mm4
        psadbw mm1, mm4
        psadbw mm2, mm4
        psadbw mm3, mm4
        paddd mm0, mm1
        paddd mm0, mm2
        paddd mm0, mm3

        movd temp, mm0
        add temp, 08h
        shr temp, 04h
        emms
    }
#else
    Ipp32u y;
    Ipp32u sum;

    sum = 0;

    // cycle over rows
    for (y = 0; y < 4; y += 1)
    {
        Ipp32u x;

        // cycle in the row
        for (x = 0; x < 4; x += 1)
        {
            sum += pSrc[x];
        }

        // set the next line
        pSrc += srcStep;
    }

    return (sum + 8) / 16;
#endif
} // Ipp32u ippiGetAverage4x4_8u(const Ipp8u *pSrc, Ipp32s srcStep)

#undef temp
#undef counter
#undef src_step
#undef src

#define ref eax
#define counter ecx
#define dst_step edx
#define src esi
#define dst edi

IppStatus ippiGetResidual4x4_8u16s(const Ipp8u *pRef, Ipp32s refStep,
                                   const Ipp8u *pSrc, Ipp32s srcStep,
                                   Ipp16s *pDst, Ipp32s dstStep)
{
#if defined WINDOWS && defined IA32
    __asm
    {
        mov ref, dword ptr [pRef]
        mov src, dword ptr [pSrc]
        mov dst, dword ptr [pDst]
        mov dst_step, dword ptr [dstStep]

        pxor mm7, mm7
        mov counter, 04h

RESTART:
        movd mm0, dword ptr [src]
        movd mm2, dword ptr [ref]
        punpcklbw mm0, mm7
        punpcklbw mm2, mm7
        psubsw mm0, mm2
        movq qword ptr [dst], mm0

        add src, dword ptr [srcStep]
        add ref, dword ptr [refStep]
        lea dst, [dst + dst_step * 02h]
        sub counter, 01h
        jnz RESTART

        emms
    }
#else
    Ipp32u y;

    // cycle over the rows
    for (y = 0; y < 4; y += 1)
    {
        Ipp32u x;

        // cycle in the row
        for (x = 0; x < 4; x += 1)
        {
            pDst[x] = pSrc[x] - pRef[x];
        }

        // set the next line
        pRef += refStep;
        pSrc += srcStep;
        pDst += dstStep;
    }

    return ippStsOk;
#endif
} // IppStatus ippiGetResidual4x4_8u16s(const Ipp8u *pRef, Ipp32s refStep,

#undef ref
#undef counter
#undef dst_step
#undef src
#undef dst

Ipp32u ippiGetInterBlockDeviation_4x4_16s(const Ipp16s *pSrc, Ipp32s srcStep)
{
    const Ipp16s *pCur;
    Ipp32u deviation;
    Ipp32u x, y;

    // check error(s)
    if (0 == pSrc)
        return 0;

    // reset variables
    deviation = 0;

    //
    // get 4x4 block deviation
    //

    // cycle over rows
    pCur = pSrc;
    for (y = 0; y < 4; y += 1)
    {
        // cycle in the row
        for (x = 0; x < 4; x += 1)
        {
            Ipp32s temp;
            Ipp32s signExtended;

            // getting sum of components
            temp = pCur[x];
            signExtended = temp >> 31;

            // we use simple scheme of abs calculation,
            // using one ADD and one XOR operation
            deviation += (temp ^ signExtended) - signExtended;
        }

        // advance source pointer
        pCur += srcStep;
    }

    return deviation;

} // Ipp32u ippiGetInterBlockDeviation_4x4_16s(const Ipp16s *pSrc, Ipp32s srcStep)

} // namespace UMC
