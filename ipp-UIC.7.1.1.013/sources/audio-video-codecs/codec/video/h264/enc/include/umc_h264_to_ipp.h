//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef UMC_H264_TO_IPP_H
#define UMC_H264_TO_IPP_H

#include "umc_h264_core_enc.h"

struct CabacStates
{
    Ipp8u   absLevelM1[10];
    Ipp8u*  sig;
    Ipp8u*  last;
};

inline void ownTransformDequantLumaDC_H264(Ipp16s* pSrcDst, Ipp32s QP, const Ipp16s* pScaleLevels/* = NULL*/)
{
    ippiTransformQuantInvLumaDC4x4_H264_16s_C1I(pSrcDst, QP, pScaleLevels);
}

inline void ownTransformDequantChromaDC_H264(Ipp16s* pSrcDst, Ipp32s QP, const Ipp16s* pLevelScale/* = NULL*/)
{
    ippiTransformQuantInvChromaDC2x2_H264_16s_C1I (pSrcDst, QP, pLevelScale);
}

inline void ownDequantTransformResidualAndAdd_H264(const Ipp8u*  pPred, Ipp16s* pSrcDst, const Ipp16s* pDC, Ipp8u* pDst, Ipp32s PredStep, Ipp32s DstStep, Ipp32s QP, Ipp32s AC, Ipp32s, const Ipp16s* pScaleLevels)
{
    ippiTransformQuantInvAddPred4x4_H264_16s_C1IR(pPred, PredStep, pSrcDst, pDC, pDst, DstStep, QP, AC, pScaleLevels);
}

inline void ownTransformLuma8x8Fwd_H264(const Ipp16s* pDiffBuf,Ipp16s* pTransformResult)
{
    ippiTransformFwdLuma8x8_H264_16s_C1(pDiffBuf, pTransformResult);
}

void QuantOptLuma8x8_H264_16s_C1_8u16s(
    const Ipp16s* pSrc,
    Ipp16s* pDst,
    Ipp32s  Qp6,
    Ipp32s  intra,
    const Ipp16s* pScanMatrix,
    const Ipp16s* pScaleLevels,
    Ipp32s* pNumLevels,
    Ipp32s* pLastCoeff,
    const H264Slice<Ipp16s, Ipp8u>* curr_slice,
    CabacStates* cbSt,
    const Ipp16s* pInvLevelScale);

inline void ownQuantLuma8x8_H264(
    const Ipp16s* pSrc,
    Ipp16s* pDst,
    Ipp32s  Qp6,
    Ipp32s  intra,
    const Ipp16s* pScanMatrix,
    const Ipp16s* pScaleLevels,
    Ipp32s* pNumLevels,
    Ipp32s* pLastCoeff,
    const H264Slice<Ipp16s, Ipp8u>* curr_slice,
    CabacStates* cbSt,
    const Ipp16s* pInvLevelScale)
{
    if(curr_slice == NULL )
        ippiQuantLuma8x8_H264_16s_C1(pSrc, pDst,Qp6,intra,pScanMatrix,pScaleLevels,pNumLevels,pLastCoeff);
    else
        QuantOptLuma8x8_H264_16s_C1_8u16s(pSrc, pDst,Qp6,intra,pScanMatrix,pScaleLevels,pNumLevels,pLastCoeff,curr_slice,cbSt,pInvLevelScale);
}

inline void ownQuantLuma8x8Inv_H264(Ipp16s* pSrcDst, Ipp32s Qp6, const Ipp16s* pInvLevelScale)
{
    ippiQuantLuma8x8Inv_H264_16s_C1I(pSrcDst, Qp6, pInvLevelScale);
}

inline void ownTransformLuma8x8InvAddPred_H264(const Ipp8u* pPred, Ipp32s PredStepPixels, Ipp16s* pSrcDst, Ipp8u* pDst, Ipp32s DstStepPixels, Ipp32s)
{
    ippiTransformLuma8x8InvAddPred_H264_16s8u_C1R(pPred, PredStepPixels*sizeof(Ipp8u), pSrcDst,pDst, DstStepPixels*sizeof(Ipp8u));
}

inline void ownEncodeChromaDcCoeffsCAVLC_H264(
    Ipp16s* pSrc,
    Ipp8u*  pTrailingOnes,
    Ipp8u*  pTrailingOneSigns,
    Ipp8u*  pNumOutCoeffs,
    Ipp8u*  pTotalZeroes,
    Ipp16s* pLevels,
    Ipp8u*  pRuns)
{
    ippiEncodeChromaDcCoeffsCAVLC_H264_16s(pSrc,pTrailingOnes, pTrailingOneSigns, pNumOutCoeffs, pTotalZeroes, pLevels, pRuns);
}

inline void ownEncodeChroma422DC_CoeffsCAVLC_H264(
    const Ipp16s *pSrc,
    Ipp8u *Trailing_Ones,
    Ipp8u *Trailing_One_Signs,
    Ipp8u *NumOutCoeffs,
    Ipp8u *TotalZeros,
    Ipp16s *Levels,
    Ipp8u *Runs)
{
    ippiEncodeCoeffsCAVLCChromaDC2x4_H264_16s (pSrc, Trailing_Ones, Trailing_One_Signs, NumOutCoeffs, TotalZeros, Levels, Runs);
}

inline void ownTransformDequantChromaDC422_H264(
    Ipp16s *pSrcDst,
    Ipp32s QPChroma,
    Ipp16s* pScaleLevels)
{
    ippiTransformQuantInvChromaDC2x4_H264_16s_C1I(pSrcDst, QPChroma, pScaleLevels);
}

inline void ownTransformQuantChroma422DC_H264(
    Ipp16s *pDCBuf,
    Ipp16s *pTBuf,
    Ipp32s  QPChroma,
    Ipp32s* NumCoeffs,
    Ipp32s  intra,
    Ipp32s  NeedTransform,
    const Ipp16s* pScaleLevels)
{
    ippiTransformQuantFwdChromaDC2x4_H264_16s_C1I(pDCBuf, pTBuf, QPChroma, NumCoeffs, intra, NeedTransform, pScaleLevels);
}

inline void ownSumsDiff8x8Blocks4x4(
    Ipp8u* pSrc,
    Ipp32s srcStepPixels,
    Ipp8u* pPred,
    Ipp32s predStepPixels,
    Ipp16s* pDC,
    Ipp16s* pDiff)
{
    ippiSumsDiff8x8Blocks4x4_8u16s_C1(pSrc, srcStepPixels*sizeof(Ipp8u), pPred, predStepPixels*sizeof(Ipp8u), pDC, pDiff);
}

inline void ownTransformQuantChromaDC_H264(
    Ipp16s* pSrcDst,
    Ipp16s* pTBlock,
    Ipp32s  QPChroma,
    Ipp32s*    pNumLevels,
    Ipp32s  intra,
    Ipp32s  needTransform,
    const Ipp16s *pScaleLevels/* = NULL*/)
{
    ippiTransformQuantFwdChromaDC2x2_H264_16s_C1I(pSrcDst, pTBlock, QPChroma, pNumLevels, intra, needTransform,pScaleLevels);
}

IppStatus TransformQuantOptFwd4x4_H264_16s_C1(
    Ipp16s* pSrcDst,
    Ipp16s* pDst,
    Ipp32s  Qp,
    Ipp32s* pNumLevels,
    Ipp32s  Intra,
    Ipp16s* pScanMatrix,
    Ipp32s* pLastCoeff,
    Ipp16s* pScaleLevels,
    const H264Slice<Ipp16s, Ipp8u> * curr_slice,
    Ipp32s sCoeff,
    CabacStates* states);

inline void ownTransformQuantResidual_H264(
    Ipp16s* pSrcDst,
    Ipp16s* pDst,
    Ipp32s  Qp,
    Ipp32s* pNumLevels,
    Ipp32s  Intra,
    Ipp16s* pScanMatrix,
    Ipp32s* pLastCoeff,
    Ipp16s* pScaleLevels/* = NULL*/,
    const H264Slice<Ipp16s, Ipp8u>* curr_slice/* = NULL*/,
    Ipp32s sCoeff/* = 0*/,
    CabacStates* states/* = NULL*/)
{
    if( curr_slice == NULL )
        ippiTransformQuantFwd4x4_H264_16s_C1(pSrcDst,pDst, Qp, pNumLevels, (Ipp8u)Intra, pScanMatrix,pLastCoeff, pScaleLevels);
    else
        TransformQuantOptFwd4x4_H264_16s_C1(pSrcDst,pDst, Qp, pNumLevels, (Ipp8u)Intra, pScanMatrix,pLastCoeff, pScaleLevels, curr_slice, sCoeff, states);
}

inline void ownInterpolateLuma_H264(
    const Ipp8u*   src,
    Ipp32s   src_pitch,
    Ipp8u*   dst,
    Ipp32s   dst_pitch,
    Ipp32s   xh,
    Ipp32s   yh,
    IppiSize sz,
    Ipp32s)
{
    ippiInterpolateLuma_H264_8u_C1R(src, src_pitch, dst, dst_pitch, xh, yh, sz);
}

inline void ownInterpolateLumaTop_H264_8u16s(
    const Ipp8u* src,
    Ipp32s   src_pitch,
    Ipp8u*   dst,
    Ipp32s   dst_pitch,
    Ipp32s   xh,
    Ipp32s   yh,
    Ipp32s   outPixels,
    IppiSize sz,
    Ipp32s)
{
    ippiInterpolateLumaTop_H264_8u_C1R(src, src_pitch, dst, dst_pitch, xh, yh, outPixels, sz);
}

inline void ownInterpolateLumaBottom_H264_8u16s(
    const Ipp8u* src,
    Ipp32s   src_pitch,
    Ipp8u*   dst,
    Ipp32s   dst_pitch,
    Ipp32s   xh,
    Ipp32s   yh,
    Ipp32s   outPixels,
    IppiSize sz,
    Ipp32s)
{
    ippiInterpolateLumaBottom_H264_8u_C1R(src, src_pitch, dst, dst_pitch, xh, yh, outPixels, sz);
}

inline void ownInterpolateChroma_H264(
    const Ipp8u* src,
    Ipp32s src_pitch,
    Ipp8u* dst,
    Ipp32s dst_pitch,
    Ipp32s xh,
    Ipp32s yh,
    IppiSize sz,
    Ipp32s)
{
    ippiInterpolateChroma_H264_8u_C1R(src, src_pitch, dst, dst_pitch, xh, yh, sz);
}

inline void ownInterpolateChromaTop_H264(
    const Ipp8u* src,
    Ipp32s   src_pitch,
    Ipp8u*   dst,
    Ipp32s   dst_pitch,
    Ipp32s   xh,
    Ipp32s   yh,
    Ipp32s   outPixels,
    IppiSize sz,
    Ipp32s)
{
    ippiInterpolateChromaTop_H264_8u_C1R(src, src_pitch, dst, dst_pitch, xh, yh, outPixels, sz);
}

inline void ownInterpolateChromaBottom_H264(
    const Ipp8u* src,
    Ipp32s   src_pitch,
    Ipp8u*   dst,
    Ipp32s   dst_pitch,
    Ipp32s   xh,
    Ipp32s   yh,
    Ipp32s   outPixels,
    IppiSize sz,
    Ipp32s)
{
    ippiInterpolateChromaBottom_H264_8u_C1R(src, src_pitch, dst, dst_pitch, xh, yh, outPixels, sz);
}

inline void ownInterpolateBlock_H264(
    const Ipp8u* pSrc1,
    const Ipp8u* pSrc2,
    Ipp8u* pDst,
    Ipp32s width,
    Ipp32s height,
    Ipp32s pitchPixels)
{
    ippiInterpolateBlock_H264_8u_P2P1R(const_cast<Ipp8u*>(pSrc1), const_cast<Ipp8u*>(pSrc2), pDst, width, height, pitchPixels);
}

inline void ownInterpolateBlock_H264_A(
    const Ipp8u* pSrc1,
    const Ipp8u* pSrc2,
    Ipp8u* pDst,
    Ipp32s width,
    Ipp32s height,
    Ipp32s pitchPix1,
    Ipp32s pitchPix2,
    Ipp32s pitchPix3)
{
    ippiInterpolateBlock_H264_8u_P3P1R(pSrc1, pSrc2, pDst, width, height, pitchPix1, pitchPix2, pitchPix3);
}

inline void ownEncodeCoeffsCAVLC_H264(
    Ipp16s* pSrc,
    Ipp8u   AC,
    Ipp32s* pScanMatrix,
    Ipp8u   Count,
    Ipp8u*  Trailing_Ones,
    Ipp8u*  Trailing_One_Signs,
    Ipp8u*  NumOutCoeffs,
    Ipp8u*  TotalZeroes,
    Ipp16s* Levels,
    Ipp8u*  Runs)
{
    ippiEncodeCoeffsCAVLC_H264_16s(pSrc, AC, pScanMatrix, Count, Trailing_Ones, Trailing_One_Signs, NumOutCoeffs, TotalZeroes, Levels, Runs);
}

inline void ownSumsDiff16x16Blocks4x4(
    Ipp8u* pSrc,
    Ipp32s srcStepPixels,
    Ipp8u* pPred,
    Ipp32s predStepPixels,
    Ipp16s* pDC,
    Ipp16s* pDiff)
{
    ippiSumsDiff16x16Blocks4x4_8u16s_C1(pSrc, srcStepPixels*sizeof(Ipp8u), pPred, predStepPixels*sizeof(Ipp8u), pDC, pDiff);
}

inline void ownTransformQuantLumaDC_H264(
    Ipp16s* pDCBuf,
    Ipp16s* pQBuf,
    Ipp32s QP,
    Ipp32s* iNumCoeffs,
    Ipp32s intra,
    const Ipp16s* scan,
    Ipp32s* iLastCoeff,
    const Ipp16s* pLevelScale/* = NULL*/)
{
    ippiTransformQuantFwdLumaDC4x4_H264_16s_C1I(pDCBuf,pQBuf,QP,iNumCoeffs,intra, scan,iLastCoeff, pLevelScale);
}

inline void ownEdgesDetect16x16(
    const Ipp8u *pSrc,
    Ipp32s srcStepPixels,
    Ipp32s EdgePelDifference,
    Ipp32s EdgePelCount,
    Ipp32s *pRes)
{
    Ipp8u uRes;
    ippiEdgesDetect16x16_8u_C1R(pSrc, srcStepPixels, (Ipp8u)EdgePelDifference,(Ipp8u)EdgePelCount,&uRes);
    *pRes = uRes;
}

#ifdef BITDEPTH_9_12
inline void ownTransformDequantLumaDC_H264(
    Ipp32s* pSrcDst,
    Ipp32s QP,
    const Ipp16s* pScaleLevels/* = NULL*/)
{
    ippiTransformQuantInvLumaDC4x4_H264_32s_C1I(pSrcDst, QP, pScaleLevels);
}

inline void ownTransformDequantChromaDC_H264(
    Ipp32s* pSrcDst,
    Ipp32s QP,
    const Ipp16s *pLevelScale/* = NULL*/)
{
    ippiTransformQuantInvChromaDC2x2_H264_32s_C1I (pSrcDst, QP, pLevelScale);
}

inline void ownDequantTransformResidualAndAdd_H264(
    const Ipp16u* pPred,
    Ipp32s* pSrcDst,
    const Ipp32s* pDC,
    Ipp16u* pDst,
    Ipp32s  PredStep,
    Ipp32s  DstStep,
    Ipp32s  QP,
    Ipp32s  AC,
    Ipp32s  bit_depth,
    const Ipp16s* pScaleLevels/* = NULL*/)
{
    ippiTransformQuantInvAddPred4x4_H264_32s_C1IR (pPred, PredStep*sizeof(Ipp16s), pSrcDst, pDC, pDst, DstStep*sizeof(Ipp16u), QP, AC, bit_depth, pScaleLevels);
}

inline void ownTransformLuma8x8Fwd_H264(
    const Ipp16s* pDiffBuf,
    Ipp32s* pTransformResult)
{
    ippiTransformFwdLuma8x8_H264_16s32s_C1(pDiffBuf, pTransformResult);
}

inline void ownQuantLuma8x8_H264(
    const Ipp32s* pSrc,
    Ipp32s* pDst,
    Ipp32s  Qp6,
    Ipp32s  intra,
    const Ipp16s* pScanMatrix,
    const Ipp16s* pScaleLevels,
    Ipp32s* pNumLevels,
    Ipp32s* pLastCoeff,
    const H264Slice<Ipp32s, Ipp16u>*,
    CabacStates*,
    const Ipp16s*)
{
    ippiQuantLuma8x8_H264_32s_C1(pSrc, pDst,Qp6,intra,pScanMatrix,pScaleLevels,pNumLevels,pLastCoeff);
}

inline void ownQuantLuma8x8Inv_H264(
    Ipp32s* pSrcDst,
    Ipp32s Qp6,
    const Ipp16s* pInvLevelScale)
{
    ippiQuantInvLuma8x8_H264_32s_C1I(pSrcDst, Qp6, pInvLevelScale);
}

inline void ownTransformLuma8x8InvAddPred_H264(
    const Ipp16u* pPred,
    Ipp32s PredStepPixels,
    Ipp32s* pSrcDst,
    Ipp16u* pDst,
    Ipp32s DstStepPixels,
    Ipp32s bit_depth)

{
    ippiTransformInvAddPredLuma8x8_H264_32s16u_C1R(pPred, PredStepPixels*sizeof(Ipp16u), pSrcDst, pDst, DstStepPixels*sizeof(Ipp16u), bit_depth);
}

inline void ownEncodeChromaDcCoeffsCAVLC_H264(
    const Ipp32s* pSrc,
    Ipp8u*  pTrailingOnes,
    Ipp8u*  pTrailingOneSigns,
    Ipp8u*  pNumOutCoeffs,
    Ipp8u*  pTotalZeroes,
    Ipp32s* pLevels,
    Ipp8u*  pRuns)
{
    ippiEncodeCoeffsCAVLCChromaDC2x2_H264_32s(pSrc,pTrailingOnes, pTrailingOneSigns, pNumOutCoeffs, pTotalZeroes, pLevels, pRuns);
}

inline void ownEncodeChroma422DC_CoeffsCAVLC_H264(
    const Ipp32s *pSrc,
    Ipp8u *Trailing_Ones,
    Ipp8u *Trailing_One_Signs,
    Ipp8u *NumOutCoeffs,
    Ipp8u *TotalZeros,
    Ipp32s *Levels,
    Ipp8u *Runs)
{
    ippiEncodeCoeffsCAVLCChromaDC2x4_H264_32s(pSrc, Trailing_Ones, Trailing_One_Signs, NumOutCoeffs, TotalZeros, Levels, Runs);
}

inline void ownTransformQuantChromaDC_H264(
    Ipp32s* pSrcDst,
    Ipp32s* pTBlock,
    Ipp32s  QPChroma,
    Ipp32s* pNumLevels,
    Ipp32s  intra,
    Ipp32s  needTransform,
    const Ipp16s* pScaleLevels/* = NULL*/)
{
    ippiTransformQuantFwdChromaDC2x2_H264_32s_C1I(pSrcDst, pTBlock, QPChroma, pNumLevels, intra, needTransform, pScaleLevels);
}

IppStatus TransformQuantOptFwd4x4_H264_16s32s_C1(
    Ipp16s* pSrc,
    Ipp32s* pDst,
    Ipp32s  Qp,
    Ipp32s* pNumLevels,
    Ipp32s  intra,
    const Ipp16s* pScanMatrix,
    Ipp32s* pLastCoeff,
    const Ipp16s* pLevelScales,
    const H264Slice<Ipp32s, Ipp16u>* curr_slice,
    Ipp32s sCoeff,
    CabacStates* states);


inline void ownTransformQuantResidual_H264(
    Ipp16s* pSrc,
    Ipp32s* pDst,
    Ipp32s  Qp,
    Ipp32s* pNumLevels,
    Ipp32s  intra,
    const Ipp16s* pScanMatrix,
    Ipp32s* pLastCoeff,
    const Ipp16s* pLevelScales/* = NULL*/,
    const H264Slice<Ipp32s, Ipp16u>*,
    Ipp32s,
    CabacStates*)
{
    ippiTransformQuantFwd4x4_H264_16s32s_C1(pSrc, pDst, Qp, pNumLevels, intra, pScanMatrix,pLastCoeff, pLevelScales);
}

inline void ownInterpolateBlock_H264(
    const Ipp16u* pSrc1,
    const Ipp16u* pSrc2,
    Ipp16u* pDst,
    Ipp32s  width,
    Ipp32s  height,
    Ipp32s  pitchPixels)
{
    IppVCBidir_16u info;
    info.pSrc1 = (Ipp16u*)pSrc1;
    info.pSrc2 = (Ipp16u*)pSrc2;
    info.pDst = pDst;
    info.dstStep = info.srcStep2 = info.srcStep1 = pitchPixels;
    info.roiSize.width = width;
    info.roiSize.height = height;
    ippiBidir_H264_16u_P2P1R( &info );
}

inline void ownInterpolateBlock_H264_A(
    const Ipp16u* pSrc1,
    const Ipp16u* pSrc2,
    Ipp16u* pDst,
    Ipp32s  width,
    Ipp32s  height,
    Ipp32s pitchPix1,
    Ipp32s pitchPix2,
    Ipp32s pitchPix3)
{
    IppVCBidir_16u info;
    info.pSrc1 = (Ipp16u*)pSrc1;
    info.pSrc2 = (Ipp16u*)pSrc2;
    info.pDst = pDst;
    info.dstStep = pitchPix3;
    info.srcStep2 = pitchPix2;
    info.srcStep1 = pitchPix1;
    info.roiSize.width = width;
    info.roiSize.height = height;
    ippiBidir_H264_16u_P2P1R( &info );
}

inline void ownEncodeCoeffsCAVLC_H264(
    const Ipp32s* pSrc,
    Ipp8u AC,
    const Ipp32s* pScanMatrix,
    Ipp32s Count,
    Ipp8u* Trailing_Ones,
    Ipp8u* Trailing_One_Signs,
    Ipp8u* NumOutCoeffs,
    Ipp8u* TotalZeroes,
    Ipp32s* Levels,
    Ipp8u* Runs)
{
    ippiEncodeCoeffsCAVLC_H264_32s(pSrc, AC, pScanMatrix, Count, Trailing_Ones, Trailing_One_Signs, NumOutCoeffs, TotalZeroes, Levels, Runs);
}

inline void ownSumsDiff16x16Blocks4x4(
    const Ipp16u* pSrc,
    Ipp32s srcStepPixels,
    const Ipp16u* pPred,
    Ipp32s predStepPixels,
    Ipp32s* pDC,
    Ipp16s* pDiff)
{
    ippiSumsDiff16x16Blocks4x4_16u32s_C1R(pSrc, srcStepPixels*sizeof(Ipp16s), pPred, predStepPixels*sizeof(Ipp16s), pDC, pDiff);
}

inline void ownTransformQuantLumaDC_H264(
    Ipp32s* pDCBuf,
    Ipp32s* pQBuf,
    Ipp32s QP,
    Ipp32s* iNumCoeffs,
    Ipp32s intra,
    const Ipp16s* scan,
    Ipp32s* iLastCoeff,
    const Ipp16s* pLevelScale/* = NULL*/)
{
    ippiTransformQuantFwdLumaDC4x4_H264_32s_C1I (pDCBuf,pQBuf,QP,iNumCoeffs,intra, scan,iLastCoeff, pLevelScale);
}

inline void ownInterpolateLuma_H264(
    const Ipp16u* src,
    Ipp32s   src_pitch,
    Ipp16u*  dst,
    Ipp32s   dst_pitch,
    Ipp32s   xh,
    Ipp32s   yh,
    IppiSize sz,
    Ipp32s   bit_depth)
{
    //    IppVCInterpolate_16u info = { (Ipp16u*)src,src_pitch, dst, dst_pitch, xh, yh, sz, bit_depth };
    IppVCInterpolate_16u info;
    info.pSrc = (Ipp16u*)src;
    info.srcStep = src_pitch;
    info.pDst = dst;
    info.dstStep = dst_pitch;
    info.dx = xh;
    info.dy = yh;
    info.roiSize.width = sz.width;
    info.roiSize.height = sz.height;
    info.bitDepth = bit_depth;
    ippiInterpolateLuma_H264_16u_C1R(&info);
}

inline void ownInterpolateLumaTop_H264(
    const Ipp16u* src,
    Ipp32s    src_pitch,
    Ipp16u*   dst,
    Ipp32s    dst_pitch,
    Ipp32s    xh,
    Ipp32s    yh,
    Ipp32s    outPixels,
    IppiSize  sz,
    Ipp32s    bit_depth)
{
    IppVCInterpolate_16u info;
    info.pSrc = (Ipp16u*)src;
    info.srcStep = src_pitch;
    info.pDst = dst;
    info.dstStep = dst_pitch;
    info.dx = xh;
    info.dy = yh;
    info.roiSize.width = sz.width;
    info.roiSize.height = sz.height;
    info.bitDepth = bit_depth;
    ippiInterpolateLumaTop_H264_16u_C1R( &info, outPixels );
}

inline void ownInterpolateLumaBottom_H264(
    const Ipp16u* src,
    Ipp32s src_pitch,
    Ipp16u* dst,
    Ipp32s dst_pitch,
    Ipp32s xh,
    Ipp32s yh,
    Ipp32s outPixels,
    IppiSize sz,
    Ipp32s bit_depth)
{
    //    IppVCInterpolate_16u info = { (Ipp16u*)src, src_pitch, dst, dst_pitch, xh, yh, sz, bit_depth };
    IppVCInterpolate_16u info;
    info.pSrc = (Ipp16u*)src;
    info.srcStep = src_pitch;
    info.pDst = dst;
    info.dstStep = dst_pitch;
    info.dx = xh;
    info.dy = yh;
    info.roiSize.width = sz.width;
    info.roiSize.height = sz.height;
    info.bitDepth = bit_depth;
    ippiInterpolateLumaBottom_H264_16u_C1R( &info, outPixels );
}

inline void ownTransformDequantChromaDC422_H264(
    Ipp32s *pSrcDst,
    Ipp32s QPChroma,
    Ipp16s* pScaleLevels/* = NULL*/)
{
    ippiTransformQuantInvChromaDC2x4_H264_32s_C1I(pSrcDst, QPChroma, pScaleLevels);
}

inline void ownTransformQuantChroma422DC_H264(
    Ipp32s *pDCBuf,
    Ipp32s *pTBuf,
    Ipp32s QPChroma,
    Ipp32s* NumCoeffs,
    Ipp32s Intra,
    Ipp32s NeedTransform,
    const Ipp16s* pScaleLevels/* = NULL*/)
{
    ippiTransformQuantFwdChromaDC2x4_H264_32s_C1I(pDCBuf, pTBuf, QPChroma, NumCoeffs, Intra, NeedTransform, pScaleLevels);
}

inline void ownSumsDiff8x8Blocks4x4(
    const Ipp16u* pSrc,
    Ipp32s  srcStepPixels,
    const Ipp16u* pPred,
    Ipp32s  predStepPixels,
    Ipp32s* pDC,
    Ipp16s* pDiff)
{
    ippiSumsDiff8x8Blocks4x4_16u32s_C1R(pSrc, srcStepPixels*sizeof(Ipp16u), pPred, predStepPixels*sizeof(Ipp16u), pDC, pDiff);
}

inline void ownInterpolateChromaBottom_H264(
    const Ipp16u* src,
    Ipp32s src_pitch,
    Ipp16u* dst,
    Ipp32s dst_pitch,
    Ipp32s xh,
    Ipp32s yh,
    Ipp32s outPixels,
    IppiSize sz,
    Ipp32s bit_depth)
{
    //    IppVCInterpolate_16u info = { (Ipp16u*)src, src_pitch, dst, dst_pitch, xh, yh, sz, bit_depth };
    IppVCInterpolate_16u info;
    info.pSrc = (Ipp16u*)src;
    info.srcStep = src_pitch;
    info.pDst = dst;
    info.dstStep = dst_pitch;
    info.dx = xh;
    info.dy = yh;
    info.roiSize.width = sz.width;
    info.roiSize.height = sz.height;
    info.bitDepth = bit_depth;
    ippiInterpolateChromaBottom_H264_16u_C1R( &info, outPixels );
}

inline void ownInterpolateChromaTop_H264(
    const Ipp16u* src,
    Ipp32s src_pitch,
    Ipp16u* dst,
    Ipp32s dst_pitch,
    Ipp32s xh,
    Ipp32s yh,
    Ipp32s outPixels,
    IppiSize sz,
    Ipp32s bit_depth)
{
    //    IppVCInterpolate_16u info = { (Ipp16u*)src, src_pitch, dst, dst_pitch, xh, yh, sz, bit_depth };
    IppVCInterpolate_16u info;
    info.pSrc = (Ipp16u*)src;
    info.srcStep = src_pitch;
    info.pDst = dst;
    info.dstStep = dst_pitch;
    info.dx = xh;
    info.dy = yh;
    info.roiSize.width = sz.width;
    info.roiSize.height = sz.height;
    info.bitDepth = bit_depth;
    ippiInterpolateChromaTop_H264_16u_C1R( &info, outPixels );
}

inline void ownEdgesDetect16x16(
    const Ipp16u* pSrc,
    Ipp32s srcStepPixels,
    Ipp32s EdgePelDifference,
    Ipp32s EdgePelCount,
    Ipp32s* pRes)
{
    ippiEdgesDetect16x16_16u_C1R(pSrc, srcStepPixels * sizeof(Ipp16u), EdgePelDifference, EdgePelCount, pRes);
}

inline void ownInterpolateChroma_H264(
    const Ipp16u* src,
    Ipp32s src_pitch,
    Ipp16u* dst,
    Ipp32s dst_pitch,
    Ipp32s xh,
    Ipp32s yh,
    IppiSize sz,
    Ipp32s bit_depth)
{
//    IppVCInterpolate_16u info = { (Ipp16u*)src,src_pitch, dst, dst_pitch, xh, yh, sz, bit_depth };
    IppVCInterpolate_16u info;
    info.pSrc = (Ipp16u*)src;
    info.srcStep = src_pitch;
    info.pDst = dst;
    info.dstStep = dst_pitch;
    info.dx = xh;
    info.dy = yh;
    info.roiSize.width = sz.width;
    info.roiSize.height = sz.height;
    info.bitDepth = bit_depth;
    ippiInterpolateChroma_H264_16u_C1R( &info );
}

#endif // BITDEPTH_9_12
#endif // UMC_H264_TO_IPP_H
