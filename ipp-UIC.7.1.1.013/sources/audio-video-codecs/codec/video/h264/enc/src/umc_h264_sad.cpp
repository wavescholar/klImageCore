//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include "umc_h264_video_encoder.h"
#include "umc_h264_tables.h"
#include "umc_h264_to_ipp.h"
#include "umc_h264_bme.h"

#include "ippvc.h"

Ipp32u SAT8x8D(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSAT8x8D_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 2;
}

#ifdef BITDEPTH_9_12
Ipp32u SAT8x8D(const Ipp16u *pSrc1, Ipp32s src1Step, const Ipp16u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSAT8x8D_16u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 2;
}
#endif

Ipp32u SATD_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step, Ipp32s width, Ipp32s height)
{
#ifndef INTRINSIC_OPT
    __ALIGN16 Ipp16s tmpBuff[4][4];
    __ALIGN16 Ipp16s diffBuff[4][4];
#endif
    Ipp32s x, y;
    Ipp32u satd = 0;

    for( y = 0; y < height; y += 4 ) {
        for( x = 0; x < width; x += 4 )  {
#ifndef INTRINSIC_OPT
            Ipp32s b;
            ippiSub4x4_8u16s_C1R(pSrc1 + x, src1Step, pSrc2 + x, src2Step, &diffBuff[0][0], 8);
            for (b = 0; b < 4; b ++) {
                Ipp32s a01, a23, b01, b23;

                a01 = diffBuff[b][0] + diffBuff[b][1];
                a23 = diffBuff[b][2] + diffBuff[b][3];
                b01 = diffBuff[b][0] - diffBuff[b][1];
                b23 = diffBuff[b][2] - diffBuff[b][3];
                tmpBuff[b][0] = a01 + a23;
                tmpBuff[b][1] = a01 - a23;
                tmpBuff[b][2] = b01 - b23;
                tmpBuff[b][3] = b01 + b23;
            }
            for (b = 0; b < 4; b ++) {
                Ipp32s a01, a23, b01, b23;

                a01 = tmpBuff[0][b] + tmpBuff[1][b];
                a23 = tmpBuff[2][b] + tmpBuff[3][b];
                b01 = tmpBuff[0][b] - tmpBuff[1][b];
                b23 = tmpBuff[2][b] - tmpBuff[3][b];
                satd += ABS(a01 + a23) + ABS(a01 - a23) + ABS(b01 - b23) + ABS(b01 + b23);
            }
#else
            __ALIGN16 __m128i  _p_0, _p_1, _p_2, _p_3, _p_4, _p_5, _p_7, _p_zero;
            const Ipp8u *pS1, *pS2;
            Ipp32s  s;

            pS1 = pSrc1 + x;
            pS2 = pSrc2 + x;
            _p_zero = _mm_setzero_si128();
            _p_0 = _mm_cvtsi32_si128(*(int*)(pS1));
            _p_4 = _mm_cvtsi32_si128(*(int*)(pS2));
            _p_1 = _mm_cvtsi32_si128(*(int*)(pS1+src1Step));
            _p_5 = _mm_cvtsi32_si128(*(int*)(pS2+src2Step));
            _p_0 = _mm_unpacklo_epi8(_p_0, _p_zero);
            _p_4 = _mm_unpacklo_epi8(_p_4, _p_zero);
            _p_1 = _mm_unpacklo_epi8(_p_1, _p_zero);
            _p_5 = _mm_unpacklo_epi8(_p_5, _p_zero);
            _p_0 = _mm_sub_epi16(_p_0, _p_4);
            _p_1 = _mm_sub_epi16(_p_1, _p_5);
            pS1 += 2 * src1Step;
            pS2 += 2 * src2Step;
            _p_2 = _mm_cvtsi32_si128(*(int*)(pS1));
            _p_4 = _mm_cvtsi32_si128(*(int*)(pS2));
            _p_3 = _mm_cvtsi32_si128(*(int*)(pS1+src1Step));
            _p_5 = _mm_cvtsi32_si128(*(int*)(pS2+src2Step));
            _p_2 = _mm_unpacklo_epi8(_p_2, _p_zero);
            _p_4 = _mm_unpacklo_epi8(_p_4, _p_zero);
            _p_3 = _mm_unpacklo_epi8(_p_3, _p_zero);
            _p_5 = _mm_unpacklo_epi8(_p_5, _p_zero);
            _p_2 = _mm_sub_epi16(_p_2, _p_4);
            _p_3 = _mm_sub_epi16(_p_3, _p_5);
            _p_5 = _mm_subs_epi16(_p_0, _p_1);
            _p_0 = _mm_adds_epi16(_p_0, _p_1);
            _p_7 = _mm_subs_epi16(_p_2, _p_3);
            _p_2 = _mm_adds_epi16(_p_2, _p_3);
            _p_1 = _mm_subs_epi16(_p_0, _p_2);
            _p_0 = _mm_adds_epi16(_p_0, _p_2);
            _p_3 = _mm_adds_epi16(_p_5, _p_7);
            _p_5 = _mm_subs_epi16(_p_5, _p_7);
            _p_0 = _mm_unpacklo_epi16(_p_0, _p_1);
            _p_5 = _mm_unpacklo_epi16(_p_5, _p_3);
            _p_7 = _mm_unpackhi_epi32(_p_0, _p_5);
            _p_0 = _mm_unpacklo_epi32(_p_0, _p_5);
            _p_1 = _mm_srli_si128(_p_0, 8);
            _p_3 = _mm_srli_si128(_p_7, 8);
            _p_5 = _mm_subs_epi16(_p_0, _p_1);
            _p_0 = _mm_adds_epi16(_p_0, _p_1);
            _p_2 = _mm_subs_epi16(_p_7, _p_3);
            _p_7 = _mm_adds_epi16(_p_7, _p_3);
            _p_1 = _mm_subs_epi16(_p_0, _p_7);
            _p_0 = _mm_adds_epi16(_p_0, _p_7);
            _p_3 = _mm_adds_epi16(_p_5, _p_2);
            _p_5 = _mm_subs_epi16(_p_5, _p_2);
            _p_0 = _mm_unpacklo_epi16(_p_0, _p_1);
            _p_5 = _mm_unpacklo_epi16(_p_5, _p_3);
            _p_2 = _mm_unpackhi_epi32(_p_0, _p_5);
            _p_0 = _mm_unpacklo_epi32(_p_0, _p_5);
            _p_3 = _mm_srai_epi16(_p_2, 15);
            _p_1 = _mm_srai_epi16(_p_0, 15);
            _p_2 = _mm_xor_si128(_p_2, _p_3);
            _p_0 = _mm_xor_si128(_p_0, _p_1);
            _p_2 = _mm_sub_epi16(_p_2, _p_3);
            _p_0 = _mm_sub_epi16(_p_0, _p_1);
            _p_0 = _mm_add_epi16(_p_0, _p_2);
            _p_2 = _mm_srli_si128(_p_0, 8);
            _p_0 = _mm_add_epi16(_p_0, _p_2);
            _p_2 = _mm_srli_si128(_p_0, 4);
            _p_0 = _mm_add_epi16(_p_0, _p_2);
            s = _mm_cvtsi128_si32(_p_0);
            satd += (s >> 16) + (Ipp16s)s;
#endif
        }
        pSrc1 += 4 * src1Step;
        pSrc2 += 4 * src2Step;
    }
    return satd >> 1;
}

Ipp32u SATD4x4_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD4x4_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

Ipp32u SATD4x8_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD4x8_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

Ipp32u SATD8x4_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD8x4_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

Ipp32u SATD8x8_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD8x8_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

Ipp32u SATD8x16_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD8x16_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

Ipp32u SATD16x8_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD16x8_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

Ipp32u SATD16x16_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step)
{
    Ipp32u satd = 0;
    ippiSATD16x16_8u32s_C1R(pSrc1, src1Step, pSrc2, src2Step, (Ipp32s*)&satd);
    return satd >> 1;
}

#ifdef BITDEPTH_9_12
Ipp32u SATD_16u_C1R(const Ipp16u *pSrc1, Ipp32s src1Step, const Ipp16u *pSrc2, Ipp32s src2Step, Ipp32s width, Ipp32s height)
{
    __ALIGN16 Ipp32s tmpBuff[4][4];
    __ALIGN16 Ipp16s diffBuff[4][4];
    Ipp32s x, y;
    Ipp32u satd = 0;

    for( y = 0; y < height; y += 4 ) {
        for( x = 0; x < width; x += 4 )  {
            Ipp32s b;
            ippiSub4x4_16u16s_C1R(pSrc1 + x, src1Step*sizeof(Ipp16u), pSrc2 + x, src2Step*sizeof(Ipp16u), &diffBuff[0][0], 8);
            for (b = 0; b < 4; b ++) {
                Ipp32s a01, a23, b01, b23;

                a01 = diffBuff[b][0] + diffBuff[b][1];
                a23 = diffBuff[b][2] + diffBuff[b][3];
                b01 = diffBuff[b][0] - diffBuff[b][1];
                b23 = diffBuff[b][2] - diffBuff[b][3];
                tmpBuff[b][0] = a01 + a23;
                tmpBuff[b][1] = a01 - a23;
                tmpBuff[b][2] = b01 - b23;
                tmpBuff[b][3] = b01 + b23;
            }
            for (b = 0; b < 4; b ++) {
                Ipp32s a01, a23, b01, b23;

                a01 = tmpBuff[0][b] + tmpBuff[1][b];
                a23 = tmpBuff[2][b] + tmpBuff[3][b];
                b01 = tmpBuff[0][b] - tmpBuff[1][b];
                b23 = tmpBuff[2][b] - tmpBuff[3][b];
                satd += ABS(a01 + a23) + ABS(a01 - a23) + ABS(b01 - b23) + ABS(b01 + b23);
            }
        }
        pSrc1 += 4 * src1Step;
        pSrc2 += 4 * src2Step;
    }
    return satd >> 1;
}
#endif

IppStatus __STDCALL SAD4x2_8u32s_C1R(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s)
{
    Ipp32s d0 = pSrcCur[0] - pSrcRef[0];
    Ipp32s d1 = pSrcCur[1] - pSrcRef[1];
    Ipp32s d2 = pSrcCur[srcCurStep+0] - pSrcRef[srcRefStep+0];
    Ipp32s d3 = pSrcCur[srcCurStep+1] - pSrcRef[srcRefStep+1];
    Ipp32s d4 = pSrcCur[2] - pSrcRef[2];
    Ipp32s d5 = pSrcCur[3] - pSrcRef[3];
    Ipp32s d6 = pSrcCur[srcCurStep+2] - pSrcRef[srcRefStep+2];
    Ipp32s d7 = pSrcCur[srcCurStep+3] - pSrcRef[srcRefStep+3];
    *pDst = ABS(d0) + ABS(d1) + ABS(d2) + ABS(d3) + ABS(d4) + ABS(d5) + ABS(d6) + ABS(d7);
    return ippStsNoErr;
}


IppStatus __STDCALL SAD2x4_8u32s_C1R(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s)
{
    Ipp32s d0 = pSrcCur[0] - pSrcRef[0];
    Ipp32s d1 = pSrcCur[1] - pSrcRef[1];
    Ipp32s d2 = pSrcCur[srcCurStep+0] - pSrcRef[srcRefStep+0];
    Ipp32s d3 = pSrcCur[srcCurStep+1] - pSrcRef[srcRefStep+1];
    pSrcCur += srcCurStep * 2;
    pSrcRef += srcRefStep * 2;
    Ipp32s d4 = pSrcCur[0] - pSrcRef[0];
    Ipp32s d5 = pSrcCur[1] - pSrcRef[1];
    Ipp32s d6 = pSrcCur[srcCurStep+0] - pSrcRef[srcRefStep+0];
    Ipp32s d7 = pSrcCur[srcCurStep+1] - pSrcRef[srcRefStep+1];
    *pDst = ABS(d0) + ABS(d1) + ABS(d2) + ABS(d3) + ABS(d4) + ABS(d5) + ABS(d6) + ABS(d7);
    return ippStsNoErr;
}


IppStatus __STDCALL SAD2x2_8u32s_C1R(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s)
{
    Ipp32s d0 = pSrcCur[0] - pSrcRef[0];
    Ipp32s d1 = pSrcCur[1] - pSrcRef[1];
    Ipp32s d2 = pSrcCur[srcCurStep+0] - pSrcRef[srcRefStep+0];
    Ipp32s d3 = pSrcCur[srcCurStep+1] - pSrcRef[srcRefStep+1];
    *pDst = ABS(d0) + ABS(d1) + ABS(d2) + ABS(d3);
    return ippStsNoErr;
}


Ipp32u SATD4x2_8u_C1R(const Ipp8u *pCur, Ipp32s pitchPixelsCur, const Ipp8u *pRef, Ipp32s pitchPixelsRef)
{
    Ipp32s d0 = pCur[0] - pRef[0];
    Ipp32s d1 = pCur[1] - pRef[1];
    Ipp32s d2 = pCur[pitchPixelsCur+0] - pRef[pitchPixelsRef+0];
    Ipp32s d3 = pCur[pitchPixelsCur+1] - pRef[pitchPixelsRef+1];
    Ipp32s a0 = d0 + d2;
    Ipp32s a1 = d1 + d3;
    Ipp32s a2 = d0 - d2;
    Ipp32s a3 = d1 - d3;
    Ipp32u sad = ABS(a0 + a1) + ABS(a0 - a1) + ABS(a2 + a3) + ABS(a2 - a3);
    d0 = pCur[2] - pRef[2];
    d1 = pCur[3] - pRef[3];
    d2 = pCur[pitchPixelsCur+2] - pRef[pitchPixelsRef+2];
    d3 = pCur[pitchPixelsCur+3] - pRef[pitchPixelsRef+3];
    a0 = d0 + d2;
    a1 = d1 + d3;
    a2 = d0 - d2;
    a3 = d1 - d3;
    sad += ABS(a0 + a1) + ABS(a0 - a1) + ABS(a2 + a3) + ABS(a2 - a3);
    return sad;
}


Ipp32u SATD2x4_8u_C1R(const Ipp8u *pCur, Ipp32s pitchPixelsCur, const Ipp8u *pRef, Ipp32s pitchPixelsRef)
{
    Ipp32s d0 = pCur[0] - pRef[0];
    Ipp32s d1 = pCur[1] - pRef[1];
    Ipp32s d2 = pCur[pitchPixelsCur+0] - pRef[pitchPixelsRef+0];
    Ipp32s d3 = pCur[pitchPixelsCur+1] - pRef[pitchPixelsRef+1];
    Ipp32s a0 = d0 + d2;
    Ipp32s a1 = d1 + d3;
    Ipp32s a2 = d0 - d2;
    Ipp32s a3 = d1 - d3;
    Ipp32u sad = ABS(a0 + a1) + ABS(a0 - a1) + ABS(a2 + a3) + ABS(a2 - a3);
    pCur += pitchPixelsCur * 2;
    pRef += pitchPixelsRef * 2;
    d0 = pCur[0] - pRef[0];
    d1 = pCur[1] - pRef[1];
    d2 = pCur[pitchPixelsCur+0] - pRef[pitchPixelsRef+0];
    d3 = pCur[pitchPixelsCur+1] - pRef[pitchPixelsRef+1];
    a0 = d0 + d2;
    a1 = d1 + d3;
    a2 = d0 - d2;
    a3 = d1 - d3;
    sad += ABS(a0 + a1) + ABS(a0 - a1) + ABS(a2 + a3) + ABS(a2 - a3);
    return sad;
}


Ipp32u SATD2x2_8u_C1R(const Ipp8u *pCur, Ipp32s pitchPixelsCur, const Ipp8u *pRef, Ipp32s pitchPixelsRef)
{
    Ipp32s d0 = pCur[0] - pRef[0];
    Ipp32s d1 = pCur[1] - pRef[1];
    Ipp32s d2 = pCur[pitchPixelsCur+0] - pRef[pitchPixelsRef+0];
    Ipp32s d3 = pCur[pitchPixelsCur+1] - pRef[pitchPixelsRef+1];
    Ipp32s a0 = d0 + d2;
    Ipp32s a1 = d1 + d3;
    Ipp32s a2 = d0 - d2;
    Ipp32s a3 = d1 - d3;
    Ipp32u sad = ABS(a0 + a1) + ABS(a0 - a1) + ABS(a2 + a3) + ABS(a2 - a3);
    return sad;
}

SADfunc8u SAD_8u[21] = {
    NULL, NULL,
    (SADfunc8u)SAD2x2_8u32s_C1R,
    (SADfunc8u)SAD2x4_8u32s_C1R,
    (SADfunc8u)SAD4x2_8u32s_C1R,
    (SADfunc8u)ippiSAD4x4_8u32s,
    (SADfunc8u)ippiSAD4x8_8u32s_C1R,
    NULL, NULL,
    (SADfunc8u)ippiSAD8x4_8u32s_C1R,
    (SADfunc8u)ippiSAD8x8_8u32s_C1R,
    NULL,
    (SADfunc8u)ippiSAD8x16_8u32s_C1R,
    NULL, NULL, NULL, NULL, NULL,
    (SADfunc8u)ippiSAD16x8_8u32s_C1R,
    NULL,
    (SADfunc8u)ippiSAD16x16_8u32s
};

SATDfunc8u SATD_8u[21] = {
    NULL, NULL,
    SATD2x2_8u_C1R,
    SATD2x4_8u_C1R,
    SATD4x2_8u_C1R,
    SATD4x4_8u_C1R,
    SATD4x8_8u_C1R,
    NULL, NULL,
    SATD8x4_8u_C1R,
    SATD8x8_8u_C1R,
    NULL,
    SATD8x16_8u_C1R,
    NULL, NULL, NULL, NULL, NULL,
    SATD16x8_8u_C1R,
    NULL,
    SATD16x16_8u_C1R
};

#endif
