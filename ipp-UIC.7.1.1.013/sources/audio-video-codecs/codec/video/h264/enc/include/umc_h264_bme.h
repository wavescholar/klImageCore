//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//
//

#ifndef UMC_H264_BME_H__
#define UMC_H264_BME_H__

#include "umc_h264_core_enc.h"
#include "umc_h264_to_ipp.h"
#include "umc_h264_tables.h"

#include "ippdefs.h"

#if _MSC_VER>1000
#pragma warning (disable:4244)
#endif

extern const Ipp8u QPtoChromaQP[52];

inline Ipp32s getLumaQP(Ipp32s QPy, Ipp32s bit_depth_luma)
{
    Ipp32s QP = (QPy + 6*(bit_depth_luma - 8));
    return (QP < 0)?0:QP;
}

inline Ipp32s getLumaQP51(Ipp32s QPy, Ipp32s bit_depth_luma)
{
    Ipp32s QP = (QPy + 6*(bit_depth_luma - 8));
    return ((QP>51)? 51:((QP < 0)?0:QP));
}

inline Ipp32s getChromaQP(Ipp32s QPy, Ipp32s qPOffset, Ipp32s bit_depth_chroma)
{
    Ipp32s QPc;
    Ipp32s qPI = QPy + qPOffset;
    Ipp32s QpBdOffsetC = 6*(bit_depth_chroma - 8);
    qPI = (qPI < -QpBdOffsetC) ? -QpBdOffsetC : (qPI > 51) ? 51 : qPI;
    QPc = (qPI >= 0)? QPtoChromaQP[qPI]: qPI;
    return (QPc + QpBdOffsetC);
}

#define MVBITS(v) (BitsForMV[(v)+BITSFORMV_OFFSET])

inline Ipp32u BITS_COST(Ipp32s bits, Ipp16s *pRDQM)
{
    return pRDQM[bits];
}

#define MVConstraintDelta( x, y ) pRDQM[pBitsX[(x)] + pBitsY[(y)]]

#ifdef IA64 //cl8 Itanium (IA64) optimization bug
inline Ipp32u MVConstraint(Ipp16s x, Ipp16s y, Ipp16s *pRDQM)
{
    return (pRDQM[MVBITS(x) + MVBITS(y)]);
}
#else
inline Ipp32u MVConstraint(Ipp32s x, Ipp32s y, Ipp16s *pRDQM)
{
    return (pRDQM[MVBITS(x) + MVBITS(y)]);
}
#endif

inline Ipp32s RefConstraint(Ipp32s ref_idx, Ipp32s active_refs, Ipp16s *pRDQM)
{
    if (active_refs == 1)
        return pRDQM[1];

    Ipp32s cost;
    if (active_refs > 2)
        cost = pRDQM[MVBITS(ref_idx)];
    else
        cost = pRDQM[ref_idx ?  2 : 1];
    return cost;
}

inline void Diff4x4(Ipp8u* pPred, Ipp8u* pSrc, Ipp32s pitchPixels, Ipp16s* pDiff)
{
    ippiSub4x4_8u16s_C1R(pSrc, pitchPixels, pPred, 16, pDiff, 8);
}

#if defined BITDEPTH_9_12
inline void Diff4x4(Ipp16u* pPred, Ipp16u* pSrc, Ipp32s pitchPixels, Ipp16s* pDiff)
{
    ippiSub4x4_16u16s_C1R(pSrc, pitchPixels*sizeof(Ipp16u), pPred, 16*sizeof(Ipp16u), pDiff, 8);
}
#endif // BITDEPTH_9_12

inline void Diff8x8(Ipp8u* pPred, Ipp8u* pSrc, Ipp32s pitchPixels, Ipp16s* pDiff)
{
    ippiSub8x8_8u16s_C1R(pSrc, pitchPixels, pPred, 16, pDiff, 16);
}

#if defined BITDEPTH_9_12
inline void Diff8x8(Ipp16u* pPred, Ipp16u* pSrc, Ipp32s pitchPixels, Ipp16s* pDiff)
{
    ippiSub8x8_16u16s_C1R(pSrc, pitchPixels*sizeof(Ipp16u), pPred, sizeof(Ipp16u)*16, pDiff, 16);
}
#endif // BITDEPTH_9_12

inline void Copy4x4(const Ipp8u* pSrc, Ipp32s srcPitchPixels, Ipp8u* pDst, Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 4; i ++) {
        pDst[0] = pSrc[0]; pDst[1] = pSrc[1]; pDst[2] = pSrc[2]; pDst[3] = pSrc[3];
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}

#if defined BITDEPTH_9_12
inline void Copy4x4(const Ipp16u* pSrc, Ipp32s srcPitchPixels, Ipp16u* pDst, Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 4; i ++) {
        pDst[0] = pSrc[0]; pDst[1] = pSrc[1]; pDst[2] = pSrc[2]; pDst[3] = pSrc[3];
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif // BITDEPTH_9_12
inline void Copy16x16(const Ipp8u* pSrc, const Ipp32s srcPitchPixels, Ipp8u* pDst, const Ipp32s dstPitchPixels)
{
    ippiCopy16x16_8u_C1R(pSrc, srcPitchPixels, pDst, dstPitchPixels);
}

#if defined BITDEPTH_9_12
inline void Copy16x16(const Ipp16u* pSrc, const Ipp32s srcPitchPixels, Ipp16u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 16; i ++) {
        memcpy( pDst, pSrc, 16*sizeof( Ipp16u ));
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif

inline void Copy16x8(const Ipp8u* pSrc, const Ipp32s srcPitchPixels, Ipp8u* pDst, const Ipp32s dstPitchPixels)
{
    ippiCopy8x8_8u_C1R(pSrc, srcPitchPixels, pDst, dstPitchPixels);
    ippiCopy8x8_8u_C1R(pSrc+8, srcPitchPixels, pDst+8, dstPitchPixels);
}

#if defined BITDEPTH_9_12
inline void Copy16x8(const Ipp16u* pSrc, const Ipp32s srcPitchPixels, Ipp16u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 8; i ++) {
        memcpy( pDst, pSrc, 16*sizeof( Ipp16u ));
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif

inline void Copy8x16(const Ipp8u* pSrc, const Ipp32s srcPitchPixels, Ipp8u* pDst, const Ipp32s dstPitchPixels)
{
    ippiCopy8x8_8u_C1R(pSrc, srcPitchPixels, pDst, dstPitchPixels);
    ippiCopy8x8_8u_C1R(pSrc+8*srcPitchPixels, srcPitchPixels, pDst+8*dstPitchPixels, dstPitchPixels);
}

#if defined BITDEPTH_9_12
inline void Copy8x16(const Ipp16u* pSrc, const Ipp32s srcPitchPixels, Ipp16u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 16; i ++) {
        memcpy( pDst, pSrc, 8*sizeof( Ipp16u ));
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif

inline void Copy8x8(const Ipp8u* pSrc, Ipp32s srcPitchPixels, Ipp8u* pDst, Ipp32s dstPitchPixels)
{
    ippiCopy8x8_8u_C1R(pSrc, srcPitchPixels, pDst, dstPitchPixels);
}

#if defined BITDEPTH_9_12
inline void Copy8x8(const Ipp16u* pSrc, Ipp32s srcPitchPixels, Ipp16u* pDst, Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 8; i ++) {
        pDst[0] = pSrc[0]; pDst[1] = pSrc[1]; pDst[2] = pSrc[2]; pDst[3] = pSrc[3]; pDst[4] = pSrc[4]; pDst[5] = pSrc[5]; pDst[6] = pSrc[6]; pDst[7] = pSrc[7];
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif // BITDEPTH_9_12

inline void Copy8x4(const Ipp8u* pSrc, const Ipp32s srcPitchPixels, Ipp8u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;
    for (i = 0; i < 4; i ++) {
        pDst[0] = pSrc[0];
        pDst[1] = pSrc[1];
        pDst[2] = pSrc[2];
        pDst[3] = pSrc[3];
        pDst[4] = pSrc[4];
        pDst[5] = pSrc[5];
        pDst[6] = pSrc[6];
        pDst[7] = pSrc[7];
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}

#if defined BITDEPTH_9_12
inline void Copy8x4(const Ipp16u* pSrc, const Ipp32s srcPitchPixels, Ipp16u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 4; i ++) {
        memcpy( pDst, pSrc, 8*sizeof( Ipp16u ));
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif

inline void Copy4x8(const Ipp8u* pSrc, const Ipp32s srcPitchPixels, Ipp8u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;
    for (i = 0; i < 8; i ++) {
        pDst[0] = pSrc[0];
        pDst[1] = pSrc[1];
        pDst[2] = pSrc[2];
        pDst[3] = pSrc[3];
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}

#if defined BITDEPTH_9_12
inline void Copy4x8(const Ipp16u* pSrc, const Ipp32s srcPitchPixels, Ipp16u* pDst, const Ipp32s dstPitchPixels)
{
    Ipp32s i;

    for (i = 0; i < 8; i ++) {
        memcpy( pDst, pSrc, 4*sizeof( Ipp16u ));
        pSrc += srcPitchPixels;
        pDst += dstPitchPixels;
    }
}
#endif



inline Ipp32s SubpelMVAdjust(const H264MotionVector *pMV, Ipp32s pitchPixels, Ipp32s& iXType, Ipp32s& iYType)
{
    Ipp32s iXOffset;
    Ipp32s iYOffset;
    Ipp32s iPelOffset = 0;

    iXType = 0;
    iYType = 0;
    if (pMV->mvx) {
        iXOffset = pMV->mvx >> 2;
        iXType = pMV->mvx & 3;
        iPelOffset += iXOffset;
    }
    if (pMV->mvy) {
        iYOffset = pMV->mvy >> 2;
        iYType = pMV->mvy & 3;
        iPelOffset += iYOffset*pitchPixels;
    }
    return iPelOffset;
}

inline Ipp32s SubpelChromaMVAdjust(const H264MotionVector *pMV, Ipp32s pitchPixels, Ipp32s& iXType, Ipp32s& iYType, Ipp32s chroma_format_idc)
{
    Ipp32s iXOffset = 0;
    Ipp32s iYOffset = 0;
    Ipp32s iPelOffset = 0;

    iXType = 0;
    iYType = 0;
    if (pMV->mvx) {
        switch( chroma_format_idc ) {
            case 1:
            case 2:
                iXOffset = pMV->mvx >> 3;
                iXType = pMV->mvx&7;
                break;
            case 3:
                iXOffset = pMV->mvx >>2;
                iXType = (pMV->mvx & 3)<<1;
                break;
        }
        iPelOffset += iXOffset;
    }
    if (pMV->mvy) {
        switch( chroma_format_idc ) {
            case 1:
                iYOffset = pMV->mvy >> 3;
                iYType = pMV->mvy&7;
                break;
            case 2:
            case 3:
                iYOffset = pMV->mvy >> 2;
                iYType = (pMV->mvy & 3)<<1;
                break;
        }
        iPelOffset += iYOffset*pitchPixels;
    }
    return iPelOffset;
}

inline
Ipp32s CalculateCoeffsCost(Ipp16s *coeffs, Ipp32s count, const Ipp32s *scan)
{
    Ipp32s k, cost = 0;
    const Ipp8u* coeff_cost = &coeff_importance[0];
    if( count == 64 ){
        coeff_cost = &coeff_importance8x8[0];
    }

    for (k = 0; k < count; k++) {
        Ipp32s run = 0;
        for (; k < count && coeffs[scan[k]] == 0; k ++, run ++);
        if (k == count)
            break;
        if (ABS(coeffs[scan[k]]) > 1)
            return 9;
        cost += coeff_cost[run];
    }
    return cost;
}

inline
Ipp32s CalculateCoeffsCost(Ipp32s *coeffs, Ipp32s count, const Ipp32s *scan)
{
    Ipp32s k, cost = 0;
    const Ipp8u* coeff_cost = &coeff_importance[0];
    if( count == 64 ){
        coeff_cost = &coeff_importance8x8[0];
    }

    for (k = 0; k < count; k++) {
        Ipp32s run = 0;
        for (; k < count && coeffs[scan[k]] == 0; k ++, run ++);
        if (k == count)
            break;
        if (ABS(coeffs[scan[k]]) > 1)
            return 9;
        cost += coeff_cost[run];
    }
    return cost;
}

#ifdef BITDEPTH_9_12
Ipp32u SATD_16u_C1R(const Ipp16u *pSrc1, Ipp32s src1Step, const Ipp16u *pSrc2, Ipp32s src2Step, Ipp32s width, Ipp32s height);
Ipp32u SAT8x8D(const Ipp16u *pSrc1, Ipp32s src1Step, const Ipp16u *pSrc2, Ipp32s src2Step);
#endif
Ipp32u SATD_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step, Ipp32s width, Ipp32s height);
Ipp32u SAT8x8D(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);

Ipp32u SATD4x4_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);
Ipp32u SATD4x8_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);
Ipp32u SATD8x4_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);
Ipp32u SATD8x8_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);
Ipp32u SATD8x16_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);
Ipp32u SATD16x8_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);
Ipp32u SATD16x16_8u_C1R(const Ipp8u *pSrc1, Ipp32s src1Step, const Ipp8u *pSrc2, Ipp32s src2Step);

IppStatus __STDCALL SAD4x2_8u32s_C1R(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s mc);
IppStatus __STDCALL SAD2x4_8u32s_C1R(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s mc);
IppStatus __STDCALL SAD2x2_8u32s_C1R(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s mc);


inline Ipp32s SATD16x16(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    return SATD16x16_8u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1);
}

inline Ipp32s SATD16x8(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    return SATD16x8_8u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1);
}

inline Ipp32s SATD8x16(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    return SATD8x16_8u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1);
}

inline Ipp32s SATD8x8(const Ipp8u* pSrc0, Ipp32s srcStepBytes0, const Ipp8u* pSrc1, Ipp32s  srcStepBytes1)
{
    return SATD8x8_8u_C1R(pSrc0, srcStepBytes0, pSrc1, srcStepBytes1);
}

inline Ipp32s SATD8x4(const Ipp8u* pSrc0, Ipp32s srcStepBytes0, const Ipp8u* pSrc1, Ipp32s  srcStepBytes1)
{
    return SATD8x4_8u_C1R(pSrc0, srcStepBytes0, pSrc1, srcStepBytes1);
}

inline Ipp32s SATD4x8(const Ipp8u* pSrc0, Ipp32s srcStepBytes0, const Ipp8u* pSrc1, Ipp32s  srcStepBytes1)
{
    return SATD4x8_8u_C1R(pSrc0, srcStepBytes0, pSrc1, srcStepBytes1);
}

inline Ipp32s SATD4x4(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    return SATD4x4_8u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1);
}

inline Ipp32s SAD16x16(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD;
    ippiSAD16x16_8u32s(pSource0, pitchPixels0, pSource1, pitchPixels1, &SAD, 0);
    return SAD;
}

inline Ipp32s SAD16x8(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD;
    ippiSAD16x8_8u32s_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, &SAD, 0);
    return SAD;
}

inline Ipp32s SAD8x16(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD;
    ippiSAD8x16_8u32s_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, &SAD, 0);
    return SAD;
}

inline Ipp32s SAD8x8(const Ipp8u* pSrc0, Ipp32s pitchPixels0, const Ipp8u* pSrc1, Ipp32s pitchPixels1)
{
    Ipp32s s;
    ippiSAD8x8_8u32s_C1R(pSrc0, pitchPixels0, pSrc1, pitchPixels1, &s, 0);
    return s;
}

inline Ipp32s SAD8x4(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD;
    ippiSAD8x4_8u32s_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, &SAD, 0);
    return SAD;
}

inline Ipp32s SAD4x8(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD;
    ippiSAD4x8_8u32s_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, &SAD, 0);
    return SAD;
}

inline Ipp32s SAD4x4(const Ipp8u *pSource0, Ipp32s pitchPixels0, const Ipp8u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s s;
    ippiSAD4x4_8u32s(pSource0, pitchPixels0, pSource1, pitchPixels1, &s, 0);
    return s;
}

inline Ipp32s SSD4x4( const Ipp8u* p1, Ipp32s pitch1, const Ipp8u* p2, Ipp32s pitch2 )
{
    Ipp32s ssd;
    ippiSSD4x4_8u32s_C1R( p1, pitch1, p2, pitch2, &ssd, 0 );
    return ssd;
}

inline Ipp32s SSD8x8( const Ipp8u* p1, Ipp32s pitch1, const Ipp8u* p2, Ipp32s pitch2 )
{
    Ipp32s ssd;
    ippiSSD8x8_8u32s_C1R( p1, pitch1, p2, pitch2, &ssd, 0 );
    return ssd;
}

inline Ipp32s SSD16x16( const Ipp8u* p1, Ipp32s pitch1, const Ipp8u* p2, Ipp32s pitch2 )
{
    Ipp32s ssd;
    ippiSqrDiff16x16_8u32s( p1, pitch1, p2, pitch2, 0, &ssd );
    return ssd;
}

#if defined BITDEPTH_9_12
inline Ipp32s SATD16x16(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    return SATD_16u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, 16, 16);
}

inline Ipp32s SATD16x8(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    return SATD_16u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, 16, 8);
}

inline Ipp32s SATD8x16(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    return SATD_16u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, 8, 16);
}

inline Ipp32s SATD8x8(const Ipp16u* pSrc0, Ipp32s srcStepBytes0, const Ipp16u* pSrc1, Ipp32s srcStepBytes1)
{
    return SATD_16u_C1R(pSrc0, srcStepBytes0, pSrc1, srcStepBytes1, 8, 8);
}

inline Ipp32s SATD8x4(const Ipp16u* pSrc0, Ipp32s srcStepBytes0, const Ipp16u* pSrc1, Ipp32s srcStepBytes1)
{
    return SATD_16u_C1R(pSrc0, srcStepBytes0, pSrc1, srcStepBytes1, 8, 4);
}

inline Ipp32s SATD4x8(const Ipp16u* pSrc0, Ipp32s srcStepBytes0, const Ipp16u* pSrc1, Ipp32s srcStepBytes1)
{
    return SATD_16u_C1R(pSrc0, srcStepBytes0, pSrc1, srcStepBytes1, 4, 8);
}

inline Ipp32s SATD4x4(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    return SATD_16u_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, 4, 4);
}

inline Ipp32s SAD16x16(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD;
    ippiSAD16x16_16u32s_C1R(pSource0, pitchPixels0*sizeof(Ipp16u), pSource1, pitchPixels1*sizeof(Ipp16u), &SAD, 0);
    return SAD;
}

inline Ipp32s SAD16x8(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD1, SAD2;
    ippiSAD8x8_16u32s_C1R(pSource0, pitchPixels0*sizeof(Ipp16u), pSource1, pitchPixels1*sizeof(Ipp16u), &SAD1, 0);
    ippiSAD8x8_16u32s_C1R(pSource0 + 8, pitchPixels0*sizeof(Ipp16u), pSource1 + 8, pitchPixels1*sizeof(Ipp16u), &SAD2, 0);
    return SAD1 + SAD2;
}

inline Ipp32s SAD8x16(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD1, SAD2;
    ippiSAD8x8_16u32s_C1R(pSource0, pitchPixels0*sizeof(Ipp16u), pSource1, pitchPixels1*sizeof(Ipp16u), &SAD1, 0);
    ippiSAD8x8_16u32s_C1R(pSource0 + 8 * pitchPixels0, pitchPixels0*sizeof(Ipp16u), pSource1 + 8 * pitchPixels1, pitchPixels1*sizeof(Ipp16u), &SAD2, 0);
    return SAD1 + SAD2;
}

inline Ipp32s SAD8x8(const Ipp16u* pSrc0, Ipp32s pitchPixels0, const Ipp16u* pSrc1, Ipp32s pitchPixels1)
{
    Ipp32s s;
    ippiSAD8x8_16u32s_C1R(pSrc0, pitchPixels0*sizeof(Ipp16u), pSrc1, pitchPixels1*sizeof(Ipp16u), &s, 0);
    return s;
}

inline Ipp32s SAD8x4(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD1, SAD2;
    ippiSAD4x4_16u32s_C1R(pSource0, pitchPixels0*sizeof(Ipp16u), pSource1, pitchPixels1*sizeof(Ipp16u), &SAD1, 0);
    ippiSAD4x4_16u32s_C1R(pSource0 + 4, pitchPixels0*sizeof(Ipp16u), pSource1 + 4, pitchPixels1*sizeof(Ipp16u), &SAD2, 0);
    return SAD1 + SAD2;
}

inline Ipp32s SAD4x8(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s SAD1, SAD2;
    ippiSAD4x4_16u32s_C1R(pSource0, pitchPixels0, pSource1, pitchPixels1, &SAD1, 0);
    ippiSAD4x4_16u32s_C1R(pSource0 + 4 * pitchPixels0, pitchPixels0*sizeof(Ipp16u), pSource1 + 4 * pitchPixels1, pitchPixels1*sizeof(Ipp16u), &SAD2, 0);
    return SAD1 + SAD2;
}

inline Ipp32s SAD4x4(const Ipp16u *pSource0, Ipp32s pitchPixels0, const Ipp16u *pSource1, Ipp32s pitchPixels1)
{
    Ipp32s s;
    ippiSAD4x4_16u32s_C1R(pSource0, pitchPixels0*sizeof(Ipp16u), pSource1, pitchPixels1*sizeof(Ipp16u), &s, 0);
    return s;
}

inline Ipp32s SSD4x4( const Ipp16u*, Ipp32s, const Ipp16u*, Ipp32s )
{
    //TODO fix me
    return 0;
}

inline Ipp32s SSD8x8( const Ipp16u*, Ipp32s, const Ipp16u*, Ipp32s )
{
    //TODO fix me
    return 0;
}

inline Ipp32s SSD16x16( const Ipp16u*, Ipp32s, const Ipp16u*, Ipp32s )
{
    //TODO fix me
    return 0;
}
#endif

typedef IppStatus (__STDCALL *SADfunc8u)(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s mcType);
typedef Ipp32u             (*SATDfunc8u)(const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep);
extern SADfunc8u  SAD_8u[];
extern SATDfunc8u SATD_8u[];

template<typename PIXTYPE>
void InterpolateLuma(const PIXTYPE *pY, Ipp32s pitchPixels, PIXTYPE *interpBuff, Ipp32s bStep, Ipp32s dx, Ipp32s dy, IppiSize block, Ipp32s bitDepth, Ipp32s planeSize, const PIXTYPE **pI = NULL, Ipp32s *sI = NULL);

#endif // UMC_H264_BME_H__
