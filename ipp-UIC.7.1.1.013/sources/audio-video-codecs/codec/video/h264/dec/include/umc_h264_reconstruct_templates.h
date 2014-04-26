/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_RECONSTRUCT_TEMPLATES_H
#define __UMC_H264_RECONSTRUCT_TEMPLATES_H

namespace UMC
{

#define NO_PADDING

#if defined(_MSC_VER)
// turn off the "conditional expression is constant" warning
#pragma warning(disable: 4127)
#endif

enum
{
    BI_DIR                      = 0,
    UNI_DIR                     = 1
};

template <typename PlaneY,
          typename PlaneUV,
          Ipp32s color_format,
          Ipp32s is_field,
          Ipp32s is_weight,
          bool nv12_support = false> //color_format == 1 && sizeof(PlaneY) == 1>
class ReconstructMB
{
public:

    enum
    {
        width_chroma_div = nv12_support ? 0 : (color_format < 3),  // for plane
        height_chroma_div = (color_format < 2),  // for plane

        point_width_chroma_div = (color_format < 3),  // for plane
        point_height_chroma_div = (color_format < 2),  // for plane

        roi_width_chroma_div = (color_format < 3),  // for roi
        roi_height_chroma_div = (color_format < 2),  // for roi
    };

    typedef PlaneY * PlanePtrY;
    typedef PlaneUV * PlanePtrUV;

    DECLALIGN(16) class ReconstructParams
    {
    public:
        // prediction pointers
        H264DecoderMotionVector *(m_pMV[2]);                    // (H264DecoderMotionVector * []) array of pointers to motion vectors
        Ipp32s m_iRefIndex[2];                                  // (Ipp32s) current reference index
        Ipp8s *(m_pRefIndex[2]);                                // (Ipp8s * []) pointer to array of reference indexes

        IppVCInterpolateBlock_16u m_lumaInterpolateInfo;
        IppVCInterpolateBlock_16u m_chromaInterpolateInfo;

        VCBidir1_16u   m_bidirLuma;
        VCBidir1_16u   m_bidirChroma[2];

        // current macroblock parameters
        Ipp32s m_iOffsetLuma;                                   // (Ipp32s) luminance offset
        Ipp32s m_iOffsetChroma;                                 // (Ipp32s) chrominance offset
        Ipp32s m_iIntraMBLumaOffset;
        Ipp32s m_iIntraMBChromaOffset;
        Ipp32s m_iIntraMBLumaOffsetTmp;
        Ipp32s m_iIntraMBChromaOffsetTmp;

        H264SegmentDecoder *m_pSegDec;                          // (H264SegmentDecoder *) pointer to segment decoder

        // weighting parameters
        Ipp32s luma_log2_weight_denom;                          // (Ipp32s)
        Ipp32s chroma_log2_weight_denom;                        // (Ipp32s)
        Ipp32s weighted_bipred_idc;                             // (Ipp32s)
        bool m_bBidirWeightMB;                                  // (bool) flag of bidirectional weighting
        bool m_bUnidirWeightMB;                                 // (bool) flag of unidirectional weighting

        // current frame parameters

        bool is_mbaff;
        bool is_bottom_mb;

        PlanePtrY m_pDstY;                                      // (PlanePtrY) pointer to an Y plane
        PlanePtrUV m_pDstU;                                     // (PlanePtrUV) pointer to an U plane
    };

    inline
    Ipp32s ChromaOffset(Ipp32s x, Ipp32s y, Ipp32s pitch)
    {
        return (x >> width_chroma_div) + (y >> height_chroma_div) * pitch;

    } // Ipp32s ChromaOffset(Ipp32s x, Ipp32s y, Ipp32s pitch)

    // we can't use template instead implicit parameter's passing
    // couse we use slightly different types
    void CompensateMotionLumaBlock(ReconstructParams *pParams,
                                   Ipp32s iDir,
                                   Ipp32s iBlockNumber,
                                   Ipp32s iUniDir)
    {
        IppVCInterpolateBlock_16u * interpolateInfo = &pParams->m_lumaInterpolateInfo;
        Ipp32s iRefIndex;
        Ipp32s iRefFieldTop;

        // get reference index
        iRefIndex = (pParams->m_iRefIndex[iDir] = GetReferenceIndex(pParams->m_pRefIndex[iDir], iBlockNumber));
        if (is_field && pParams->is_mbaff)
            iRefIndex >>= 1;
        VM_ASSERT(iRefIndex >= 0);

        // get reference frame & pitch
        interpolateInfo->pSrc[0] = (Ipp16u*) pParams->m_pSegDec->m_pRefPicList[iDir][iRefIndex]->m_pYPlane;
        VM_ASSERT(interpolateInfo->pSrc[0]);

        if (is_field)
        {
            if (pParams->is_mbaff)
            {
                iRefFieldTop = pParams->is_bottom_mb ^ (pParams->m_iRefIndex[iDir] & 1);
                pParams->m_iRefIndex[iDir] = iRefIndex;
            }
            else
                iRefFieldTop = GetReferenceField(pParams->m_pSegDec->m_pFields[iDir], iRefIndex);

            if (iRefFieldTop)
                interpolateInfo->pSrc[0] = (Ipp16u*)((PlanePtrY)(interpolateInfo->pSrc[0]) + (interpolateInfo->srcStep >> 1 ));
        }

        // create vectors & factors
        interpolateInfo->pointVector.x = pParams->m_pMV[iDir][iBlockNumber].mvx;
        interpolateInfo->pointVector.y = pParams->m_pMV[iDir][iBlockNumber].mvy;

        // we should do interpolation if vertical or horizontal vector isn't zero
        if (interpolateInfo->pointVector.x | interpolateInfo->pointVector.y)
        {
            // fill parameters
            ippiInterpolateLumaBlock(pParams->m_pDstY, interpolateInfo);

            // save pointers for optimized interpolation
            pParams->m_bidirLuma.pSrc[iDir] = interpolateInfo->pDst[0];
            pParams->m_bidirLuma.srcStep[iDir] = interpolateInfo->dstStep;
        }
        else
        {
            Ipp32s iOffset = pParams->m_iOffsetLuma +
                             pParams->m_iIntraMBLumaOffset;

            // save pointers for optimized interpolation
            if (0 == iUniDir)
            {
                pParams->m_bidirLuma.pSrc[iDir] = (Ipp16u*)((PlanePtrY) (interpolateInfo->pSrc[0]) + iOffset);
                pParams->m_bidirLuma.srcStep[iDir] = interpolateInfo->srcStep;
            }
            // we have to do interpolation for uni-direction motion
            else
            {
                interpolateInfo->pSrc[0] = (Ipp16u*) ((PlanePtrY)(interpolateInfo->pSrc[0]) + iOffset);
                ippiInterpolateLuma(pParams->m_pDstY, interpolateInfo);

                // save pointers for optimized interpolation
                pParams->m_bidirLuma.pSrc[iDir] = interpolateInfo->pDst[0];
                pParams->m_bidirLuma.srcStep[iDir] = interpolateInfo->srcStep;
            }
        }
    } // void CompensateMotionLumaBlock(ReconstructParams *pParams,

    void CompensateMotionChromaBlock(ReconstructParams *pParams,
                                     Ipp32s iDir,
                                     Ipp32s iBlockNumber,
                                     Ipp32s iUniDir)
    {
        IppVCInterpolateBlock_16u * interpolateInfo = &pParams->m_chromaInterpolateInfo;
        Ipp32s iRefIndex;
        Ipp32s iRefFieldTop;

        // get reference index
        iRefIndex = (pParams->m_iRefIndex[iDir] = GetReferenceIndex(pParams->m_pRefIndex[iDir], iBlockNumber));
        if (is_field && pParams->is_mbaff)
            iRefIndex >>= 1;
        VM_ASSERT(iRefIndex >= 0);

        // get reference frame & pitch
        {
            interpolateInfo->pSrc[0] = (Ipp16u*)pParams->m_pSegDec->m_pRefPicList[iDir][iRefIndex]->m_pUPlane;
            interpolateInfo->pSrc[1] = (Ipp16u*)pParams->m_pSegDec->m_pRefPicList[iDir][iRefIndex]->m_pVPlane;
            VM_ASSERT(interpolateInfo->pSrc[0]);
            VM_ASSERT(interpolateInfo->pSrc[1]);
        }

        if (is_field)
        {
            if (pParams->is_mbaff)
            {
                iRefFieldTop = pParams->is_bottom_mb ^ (pParams->m_iRefIndex[iDir] & 1);
                pParams->m_iRefIndex[iDir] = iRefIndex;
            }
            else
                iRefFieldTop = GetReferenceField(pParams->m_pSegDec->m_pFields[iDir], iRefIndex);

            if (iRefFieldTop)
            {
                {
                    interpolateInfo->pSrc[0] = (Ipp16u*)((PlanePtrUV)(interpolateInfo->pSrc[0]) + (interpolateInfo->srcStep >> 1));
                    interpolateInfo->pSrc[1] = (Ipp16u*)((PlanePtrUV)(interpolateInfo->pSrc[1]) + (interpolateInfo->srcStep >> 1));
                }
            }
        }

        // save vector
        interpolateInfo->pointVector.x = pParams->m_pMV[iDir][iBlockNumber].mvx;
        interpolateInfo->pointVector.y = pParams->m_pMV[iDir][iBlockNumber].mvy;

        // adjust vectors when we decode a field
        if ((is_field) && (1 == color_format))
        {
            if (pParams->is_mbaff)
            {
                interpolateInfo->pointVector.y += (pParams->is_bottom_mb - iRefFieldTop) * 2;
            }
            else
            {
                if (!pParams->m_pSegDec->m_field_index && iRefFieldTop)
                    interpolateInfo->pointVector.y -= 2;
                else if(pParams->m_pSegDec->m_field_index && !iRefFieldTop)
                    interpolateInfo->pointVector.y += 2;
            }
        }

        // we should do interpolation if vertical or horizontal vector isn't zero
        if ((interpolateInfo->pointVector.x | interpolateInfo->pointVector.y) ||
            (iUniDir))
        {
            // scale motion vector
            interpolateInfo->pointVector.x <<= ((Ipp32s) (3 <= color_format));
            interpolateInfo->pointVector.y <<= ((Ipp32s) (2 <= color_format));


            {
                ippiInterpolateChromaBlock(pParams->m_pDstU, interpolateInfo);

                // save pointers for optimized interpolation
                pParams->m_bidirChroma[0].pSrc[iDir] = interpolateInfo->pDst[0];
                pParams->m_bidirChroma[1].pSrc[iDir] = interpolateInfo->pDst[1];
                pParams->m_bidirChroma[1].srcStep[iDir] = interpolateInfo->dstStep;
            }

            pParams->m_bidirChroma[0].srcStep[iDir] = interpolateInfo->dstStep;
        }
        else
        {
            Ipp32s iOffset = pParams->m_iOffsetChroma +
                             pParams->m_iIntraMBChromaOffset;

            // save pointers for optimized interpolation
            {
                pParams->m_bidirChroma[0].pSrc[iDir] = (Ipp16u*)((PlanePtrUV)(interpolateInfo->pSrc[0]) + iOffset);
                pParams->m_bidirChroma[1].pSrc[iDir] = (Ipp16u*)((PlanePtrUV)(interpolateInfo->pSrc[1]) + iOffset);
                pParams->m_bidirChroma[1].srcStep[iDir] = interpolateInfo->srcStep;
            }
            pParams->m_bidirChroma[0].srcStep[iDir] = interpolateInfo->srcStep;
        }

    } // void CompensateMotionChromaBlock(ReconstructParams *pParams,

    inline
    void InterpolateMacroblock(ReconstructParams *pParams)
    {
        // combine bidir predictions into one,
        // no weighting

        InterpolateBlock(pParams->m_pDstY, &pParams->m_bidirLuma);

        if (color_format)
        {
            {
                InterpolateBlock(pParams->m_pDstU,
                                &pParams->m_bidirChroma[1]);
                InterpolateBlock(pParams->m_pDstU,
                                 &pParams->m_bidirChroma[0]);
            }
        }

    } // void InterpolateMacroblock(ReconstructParams *pParams)

    inline
    void BiDirWeightMacroblock(ReconstructParams *pParams)
    {
        // combine bidir predictions into one,
        // explicit weighting
        BiDirWeightBlock(pParams->m_pDstY, &pParams->m_bidirLuma,
                         pParams->luma_log2_weight_denom,
                         pParams->m_pSegDec->m_pPredWeight[D_DIR_FWD][pParams->m_iRefIndex[D_DIR_FWD]].luma_weight,
                         pParams->m_pSegDec->m_pPredWeight[D_DIR_FWD][pParams->m_iRefIndex[D_DIR_FWD]].luma_offset,
                         pParams->m_pSegDec->m_pPredWeight[D_DIR_BWD][pParams->m_iRefIndex[D_DIR_BWD]].luma_weight,
                         pParams->m_pSegDec->m_pPredWeight[D_DIR_BWD][pParams->m_iRefIndex[D_DIR_BWD]].luma_offset);

        if (color_format)
        {
            {
                BiDirWeightBlock(pParams->m_pDstU,
                                 &pParams->m_bidirChroma[1],
                                 pParams->chroma_log2_weight_denom,
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_FWD][pParams->m_iRefIndex[D_DIR_FWD]].chroma_weight[1],
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_FWD][pParams->m_iRefIndex[D_DIR_FWD]].chroma_offset[1],
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_BWD][pParams->m_iRefIndex[D_DIR_BWD]].chroma_weight[1],
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_BWD][pParams->m_iRefIndex[D_DIR_BWD]].chroma_offset[1]);

                BiDirWeightBlock(pParams->m_pDstU,
                                 &pParams->m_bidirChroma[0],
                                 pParams->chroma_log2_weight_denom,
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_FWD][pParams->m_iRefIndex[D_DIR_FWD]].chroma_weight[0],
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_FWD][pParams->m_iRefIndex[D_DIR_FWD]].chroma_offset[0],
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_BWD][pParams->m_iRefIndex[D_DIR_BWD]].chroma_weight[0],
                                 pParams->m_pSegDec->m_pPredWeight[D_DIR_BWD][pParams->m_iRefIndex[D_DIR_BWD]].chroma_offset[0]);
            }
        }

    } // void BiDirWeightMacroblock(ReconstructParams *pParams)

    inline
    void BiDirWeightMacroblockImplicit(ReconstructParams *pParams, Ipp32s iBlockNumber)
    {
        FactorArrayValue iDistScaleFactor;

        if (is_field && pParams->is_mbaff)
        {
            Ipp32s curfield = pParams->is_bottom_mb;
            Ipp32s ref0field = curfield ^ (GetReferenceIndex(pParams->m_pRefIndex[D_DIR_FWD], iBlockNumber) & 1);
            Ipp32s ref1field = curfield ^ (GetReferenceIndex(pParams->m_pRefIndex[D_DIR_BWD], iBlockNumber) & 1);

            const FactorArrayValue *pDistScaleFactors = pParams->m_pSegDec->m_pSlice->GetDistScaleFactorAFF()->values[pParams->m_iRefIndex[D_DIR_BWD]][curfield][ref0field][ref1field];
            iDistScaleFactor = (FactorArrayValue) (pDistScaleFactors[pParams->m_iRefIndex[D_DIR_FWD]] >> 2);
        }
        else
        {
            iDistScaleFactor = (FactorArrayValue) (pParams->m_pSegDec->m_pSlice->GetDistScaleFactor()->values[pParams->m_iRefIndex[D_DIR_BWD]][pParams->m_iRefIndex[D_DIR_FWD]] >> 2);
        }

        // combine bidir predictions into one,
        // implicit weighting

        BiDirWeightBlockImplicit(pParams->m_pDstY, &pParams->m_bidirLuma,
                                 64 - iDistScaleFactor,
                                 iDistScaleFactor);

        if (color_format)
        {
            {
                BiDirWeightBlockImplicit(pParams->m_pDstU,
                                         &pParams->m_bidirChroma[1],
                                         64 - iDistScaleFactor,
                                         iDistScaleFactor);

                BiDirWeightBlockImplicit(pParams->m_pDstU,
                                         &pParams->m_bidirChroma[0],
                                         64 - iDistScaleFactor,
                                         iDistScaleFactor);
            }
        }
    } // void BiDirWeightMacroblockImplicit(ReconstructParams *pParams)

    inline
    void UniDirWeightLuma(ReconstructParams *pParams, Ipp32s iDir)
    {
        UniDirWeightBlock(pParams->m_pDstY,
                          &pParams->m_bidirLuma,
                          pParams->luma_log2_weight_denom,
                          pParams->m_pSegDec->m_pPredWeight[iDir][pParams->m_iRefIndex[iDir]].luma_weight,
                          pParams->m_pSegDec->m_pPredWeight[iDir][pParams->m_iRefIndex[iDir]].luma_offset);

    } // void UniDirWeightLuma(ReconstructParams *params, Ipp32s iDir)

    inline
    void UniDirWeightChroma(ReconstructParams *pParams, Ipp32s iDir)
    {
        {
            UniDirWeightBlock(pParams->m_pDstU,
                              &pParams->m_bidirChroma[1],
                              pParams->chroma_log2_weight_denom,
                              pParams->m_pSegDec->m_pPredWeight[iDir][pParams->m_iRefIndex[iDir]].chroma_weight[1],
                              pParams->m_pSegDec->m_pPredWeight[iDir][pParams->m_iRefIndex[iDir]].chroma_offset[1]);

            UniDirWeightBlock(pParams->m_pDstU,
                              &pParams->m_bidirChroma[0],
                              pParams->chroma_log2_weight_denom,
                              pParams->m_pSegDec->m_pPredWeight[iDir][pParams->m_iRefIndex[iDir]].chroma_weight[0],
                              pParams->m_pSegDec->m_pPredWeight[iDir][pParams->m_iRefIndex[iDir]].chroma_offset[0]);
        }

    } // void UniDirWeightLuma(ReconstructParams *params, Ipp32s iDir)

    void CompensateBiDirBlock(ReconstructParams &params,
                              PlanePtrY pDstY,
                              PlanePtrUV pDstU,
                              PlanePtrUV pDstV,
                              Ipp32s iPitchLuma,
                              Ipp32s iPitchChroma,
                              Ipp32s iBlockNumber)
    {
        // set the destination
        params.m_lumaInterpolateInfo.pDst[0] = (Ipp16u*)(((PlanePtrY) params.m_pSegDec->m_pPredictionBuffer) +
                         params.m_iIntraMBLumaOffsetTmp);
        params.m_lumaInterpolateInfo.dstStep = 16;
        // do forward prediction
        CompensateMotionLumaBlock(&params, D_DIR_FWD, iBlockNumber, BI_DIR);

        if (color_format)
        {
            params.m_chromaInterpolateInfo.pDst[0] = (Ipp16u*)((PlanePtrUV) ((((PlanePtrY) params.m_pSegDec->m_pPredictionBuffer) + 16 * 16) +
                                          params.m_iIntraMBChromaOffsetTmp));
            {
                params.m_chromaInterpolateInfo.pDst[1] = (Ipp16u*)((PlanePtrUV) params.m_pSegDec->m_pPredictionBuffer + 2*16 * 16 +
                                              params.m_iIntraMBChromaOffsetTmp);
            }
            params.m_chromaInterpolateInfo.dstStep = 16;
            // do forward prediction
            CompensateMotionChromaBlock(&params, D_DIR_FWD, iBlockNumber, BI_DIR);
        }

        // set the destination
        params.m_lumaInterpolateInfo.pDst[0] = (Ipp16u*)(pDstY + params.m_iIntraMBLumaOffset);
        params.m_lumaInterpolateInfo.dstStep = iPitchLuma;

        params.m_bidirLuma.pDst = (Ipp16u*)((PlanePtrY)pDstY + params.m_iIntraMBLumaOffset);
        params.m_bidirLuma.dstStep = iPitchLuma;
        params.m_bidirLuma.roiSize = params.m_lumaInterpolateInfo.sizeBlock;

        // do backward prediction
        CompensateMotionLumaBlock(&params, D_DIR_BWD, iBlockNumber, BI_DIR);

        if (color_format)
        {
            params.m_bidirChroma[0].pDst = params.m_chromaInterpolateInfo.pDst[0] = (Ipp16u*)(pDstU + params.m_iIntraMBChromaOffset);
            params.m_bidirChroma[0].dstStep = params.m_chromaInterpolateInfo.dstStep = iPitchChroma;
            params.m_bidirChroma[0].roiSize = params.m_chromaInterpolateInfo.sizeBlock;

            {
                params.m_bidirChroma[1].pDst = params.m_chromaInterpolateInfo.pDst[1] = (Ipp16u*)(pDstV + params.m_iIntraMBChromaOffset);
                params.m_bidirChroma[1].dstStep = iPitchChroma;
                params.m_bidirChroma[1].roiSize = params.m_chromaInterpolateInfo.sizeBlock;
            }

            // do backward prediction
            CompensateMotionChromaBlock(&params, D_DIR_BWD, iBlockNumber, BI_DIR);
        }

        // do waighting
        if ((is_weight) &&
            (params.m_bBidirWeightMB))
        {
            if (1 == params.weighted_bipred_idc)
                BiDirWeightMacroblock(&params);
            else if (2 == params.weighted_bipred_idc)
                BiDirWeightMacroblockImplicit(&params, iBlockNumber);
            else
                VM_ASSERT(0);
        }
        else
        {
            InterpolateMacroblock(&params);
        }

    } // void CompensateBiDirBlock(ReconstructParams &params,

    void CompensateUniDirBlock(ReconstructParams &params,
                               PlanePtrY pDstY,
                               PlanePtrUV pDstU,
                               PlanePtrUV pDstV,
                               Ipp32s iPitchLuma,
                               Ipp32s iPitchChroma,
                               Ipp32s iDir,
                               Ipp32s iBlockNumber)
    {
        // set the destination
        params.m_lumaInterpolateInfo.pDst[0] = (Ipp16u*)(pDstY + params.m_iIntraMBLumaOffset);
        params.m_lumaInterpolateInfo.dstStep = iPitchLuma;

        params.m_bidirLuma.pDst = (Ipp16u*)((PlanePtrY)pDstY + params.m_iIntraMBLumaOffset);
        params.m_bidirLuma.dstStep = iPitchLuma;
        params.m_bidirLuma.roiSize = params.m_lumaInterpolateInfo.sizeBlock;

        // do forward prediction
        CompensateMotionLumaBlock(&params, iDir, iBlockNumber, UNI_DIR);

        if (color_format)
        {
            params.m_bidirChroma[0].pDst = params.m_chromaInterpolateInfo.pDst[0] = (Ipp16u*)(pDstU + params.m_iIntraMBChromaOffset);
            params.m_bidirChroma[0].dstStep = params.m_chromaInterpolateInfo.dstStep = iPitchChroma;
            params.m_bidirChroma[0].roiSize = params.m_chromaInterpolateInfo.sizeBlock;

            {
                params.m_bidirChroma[1].pDst = params.m_chromaInterpolateInfo.pDst[1] = (Ipp16u*)(pDstV + params.m_iIntraMBChromaOffset);
                params.m_bidirChroma[1].dstStep = iPitchChroma;
                params.m_bidirChroma[1].roiSize = params.m_chromaInterpolateInfo.sizeBlock;
            }

            // do forward prediction
            CompensateMotionChromaBlock(&params, iDir, iBlockNumber, UNI_DIR);
        }

        // optional prediction weighting
        if ((is_weight) &&
            (params.m_bUnidirWeightMB))
        {
            const PredWeightTable &pTab = params.m_pSegDec->m_pPredWeight[iDir][params.m_iRefIndex[iDir]];

            if (pTab.luma_weight_flag)
                UniDirWeightLuma(&params, iDir);

            if ((color_format) &&
                (pTab.chroma_weight_flag))
                UniDirWeightChroma(&params, iDir);
        }

    } // void CompensateUniDirBlock(ReconstructParams &params,

    void CompensateBlock8x8(PlanePtrY pDstY,
                            PlanePtrUV pDstU,
                            PlanePtrUV pDstV,
                            Ipp32s iPitchLuma,
                            Ipp32s iPitchChroma,
                            ReconstructParams &params,
                            Ipp32s iSubBlockType,
                            Ipp32s iSubBlockDir,
                            Ipp32s iSubBlockNumber)
    {
        switch (iSubBlockType)
        {
        case SBTYPE_8x8:
            {
                params.m_lumaInterpolateInfo.sizeBlock.width = 8;
                params.m_lumaInterpolateInfo.sizeBlock.height = 8;
                params.m_chromaInterpolateInfo.sizeBlock.width = 8 >> roi_width_chroma_div;
                params.m_chromaInterpolateInfo.sizeBlock.height = 8 >> roi_height_chroma_div;
                params.m_iIntraMBLumaOffset = 0;
                params.m_iIntraMBChromaOffset = 0;

                if ((D_DIR_BIDIR == iSubBlockDir) ||
                    (D_DIR_DIRECT == iSubBlockDir) ||
                    (D_DIR_DIRECT_SPATIAL_BIDIR == iSubBlockDir))
                {
                    params.m_iIntraMBLumaOffsetTmp = 0;
                    params.m_iIntraMBChromaOffsetTmp = 0;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber);
                }
                else
                {
                    Ipp32s iDir = ((D_DIR_BWD == iSubBlockDir) || (D_DIR_DIRECT_SPATIAL_BWD == iSubBlockDir)) ?
                                   (D_DIR_BWD) :
                                   (D_DIR_FWD);

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber);
                }
            }
            break;

        case SBTYPE_8x4:
            {
                params.m_lumaInterpolateInfo.sizeBlock.width = 8;
                params.m_lumaInterpolateInfo.sizeBlock.height = 4;
                params.m_chromaInterpolateInfo.sizeBlock.width = 8 >> roi_width_chroma_div;
                params.m_chromaInterpolateInfo.sizeBlock.height = 4 >> roi_height_chroma_div;
                params.m_iIntraMBLumaOffset = 0;
                params.m_iIntraMBChromaOffset = 0;

                if ((D_DIR_BIDIR == iSubBlockDir) ||
                    (D_DIR_DIRECT == iSubBlockDir) ||
                    (D_DIR_DIRECT_SPATIAL_BIDIR == iSubBlockDir))
                {
                    params.m_iIntraMBLumaOffsetTmp = 0;
                    params.m_iIntraMBChromaOffsetTmp = 0;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber);

                    // set sub-block offset for second half of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.y += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.y += 4 >> point_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 4 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (4 >> height_chroma_div) * iPitchChroma;

                    params.m_iIntraMBLumaOffsetTmp = 4 * 16;
                    params.m_iIntraMBChromaOffsetTmp = (4 >> height_chroma_div) * 16;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber + 4);
                }
                else
                {
                    Ipp32s iDir = ((D_DIR_BWD == iSubBlockDir) || (D_DIR_DIRECT_SPATIAL_BWD == iSubBlockDir)) ?
                                   (D_DIR_BWD) :
                                   (D_DIR_FWD);

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber);

                    // set sub-block offset for second half of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.y += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.y += 4 >> point_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 4 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (4 >> height_chroma_div) * iPitchChroma;

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber + 4);
                }
            }
            break;

        case SBTYPE_4x8:
            {
                params.m_lumaInterpolateInfo.sizeBlock.width = 4;
                params.m_lumaInterpolateInfo.sizeBlock.height = 8;
                params.m_chromaInterpolateInfo.sizeBlock.width = 4 >> roi_width_chroma_div;
                params.m_chromaInterpolateInfo.sizeBlock.height = 8 >> roi_height_chroma_div;
                params.m_iIntraMBLumaOffset = 0;
                params.m_iIntraMBChromaOffset = 0;

                if ((D_DIR_BIDIR == iSubBlockDir) ||
                    (D_DIR_DIRECT == iSubBlockDir) ||
                    (D_DIR_DIRECT_SPATIAL_BIDIR == iSubBlockDir))
                {
                    params.m_iIntraMBLumaOffsetTmp = 0;
                    params.m_iIntraMBChromaOffsetTmp = 0;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber);

                    // set sub-block offset for second half of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 4 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 4;
                    params.m_iIntraMBChromaOffset = (4 >> width_chroma_div);

                    params.m_iIntraMBLumaOffsetTmp = 4;
                    params.m_iIntraMBChromaOffsetTmp = (4 >> width_chroma_div);

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber + 1);
                }
                else
                {
                    Ipp32s iDir = ((D_DIR_BWD == iSubBlockDir) || (D_DIR_DIRECT_SPATIAL_BWD == iSubBlockDir)) ?
                                   (D_DIR_BWD) :
                                   (D_DIR_FWD);

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber);

                    // set sub-block offset for second half of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 4 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 4;
                    params.m_iIntraMBChromaOffset = (4 >> width_chroma_div);

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber + 1);
                }
            }
            break;

        default:
            // 4x4 sub division
            {
                params.m_lumaInterpolateInfo.sizeBlock.width = 4;
                params.m_lumaInterpolateInfo.sizeBlock.height = 4;
                params.m_chromaInterpolateInfo.sizeBlock.width = 4 >> roi_width_chroma_div;
                params.m_chromaInterpolateInfo.sizeBlock.height = 4 >> roi_height_chroma_div;
                params.m_iIntraMBLumaOffset = 0;
                params.m_iIntraMBChromaOffset = 0;

                if ((D_DIR_BIDIR == iSubBlockDir) ||
                    (D_DIR_DIRECT == iSubBlockDir) ||
                    (D_DIR_DIRECT_SPATIAL_BIDIR == iSubBlockDir))
                {
                    params.m_iIntraMBLumaOffsetTmp = 0;
                    params.m_iIntraMBChromaOffsetTmp = 0;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber);

                    // set sub-block offset for second quarter of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 4 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 4;
                    params.m_iIntraMBChromaOffset = (4 >> width_chroma_div);

                    params.m_iIntraMBLumaOffsetTmp = 4;
                    params.m_iIntraMBChromaOffsetTmp = (4 >> width_chroma_div);

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber + 1);

                    // set sub-block offset for third quarter of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x -= 4;
                    params.m_lumaInterpolateInfo.pointBlockPos.y += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x -= 4 >> point_width_chroma_div;
                    params.m_chromaInterpolateInfo.pointBlockPos.y += 4 >> point_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 4 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (4 >> height_chroma_div) * iPitchChroma;

                    params.m_iIntraMBLumaOffsetTmp = 4 * 16;
                    params.m_iIntraMBChromaOffsetTmp = (4 >> height_chroma_div) * 16;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber + 4);

                    // set sub-block offset for fourth quarter of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 4 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 4 + 4 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (4 >> width_chroma_div) +
                                                    (4 >> height_chroma_div) * iPitchChroma;

                    params.m_iIntraMBLumaOffsetTmp = 4 + 4 * 16;
                    params.m_iIntraMBChromaOffsetTmp = (4 >> width_chroma_div) +
                                                       (4 >> height_chroma_div) * 16;

                    CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                         iSubBlockNumber + 5);
                }
                else
                {
                    Ipp32s iDir = ((D_DIR_BWD == iSubBlockDir) || (D_DIR_DIRECT_SPATIAL_BWD == iSubBlockDir)) ?
                                   (D_DIR_BWD) :
                                   (D_DIR_FWD);

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber);

                    // set sub-block offset for second quarter of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 4 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 4;
                    params.m_iIntraMBChromaOffset = (4 >> width_chroma_div);

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber + 1);

                    // set sub-block offset for third quarter of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x -= 4;
                    params.m_lumaInterpolateInfo.pointBlockPos.y += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x -= 4 >> point_width_chroma_div;
                    params.m_chromaInterpolateInfo.pointBlockPos.y += 4 >> point_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 4 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (4 >> height_chroma_div) * iPitchChroma;

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber + 4);

                    // set sub-block offset for fourth quarter of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 4;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 4 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 4 + 4 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (4 >> width_chroma_div) +
                                                    (4 >> height_chroma_div) * iPitchChroma;

                    CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                          iSubBlockNumber + 5);
                }
            }
            break;
        }

    } // void CompensateBlock8x8(PlanePtrY pDstY,

    void CompensateMotionMacroBlock(PlanePtrY pDstY,
                                    PlanePtrUV pDstV,
                                    PlanePtrUV pDstU,
                                    Ipp32u mbXOffset, // for edge clipping
                                    Ipp32u mbYOffset,
                                    Ipp32s offsetY,
                                    Ipp32s offsetC,
                                    Ipp32s pitch_luma,
                                    Ipp32s pitch_chroma,
                                    H264SegmentDecoder *sd)
    {
        Ipp32s mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        Ipp8s *psbdir = sd->m_cur_mb.LocalMacroblockInfo->sbdir;

        bool bBidirWeightMB = false;    // is bidir weighting in effect for the MB?
        bool bUnidirWeightMB = false;    // is explicit L0 weighting in effect for the MB?

        // Optional weighting vars
        Ipp32u weighted_bipred_idc = 0;
        Ipp32u luma_log2_weight_denom = 0;
        Ipp32u chroma_log2_weight_denom = 0;

        RefIndexType *pRefIndexL0 = 0;
        RefIndexType *pRefIndexL1 = 0;

        VM_ASSERT(IS_INTER_MBTYPE(sd->m_cur_mb.GlobalMacroblockInfo->mbtype));

        ReconstructParams params;
        params.is_mbaff = sd->m_isMBAFF;
        params.is_bottom_mb = (params.is_mbaff && (sd->m_CurMBAddr & 1)) ? 1 : 0;

        pRefIndexL0 = sd->m_cur_mb.GetReferenceIndexStruct(0)->refIndexs;

        if (((PREDSLICE == sd->m_pSliceHeader->slice_type) ||
            (S_PREDSLICE == sd->m_pSliceHeader->slice_type)) &&
            (sd->m_pPicParamSet->weighted_pred_flag != 0))
        {
            // L0 weighting specified in pic param set. Get weighting params
            // for the slice.
            luma_log2_weight_denom = sd->m_pSliceHeader->luma_log2_weight_denom;
            chroma_log2_weight_denom = sd->m_pSliceHeader->chroma_log2_weight_denom;
            bUnidirWeightMB = true;
        }

        // get luma interp func pointer table in cache
        if (sd->m_pSliceHeader->slice_type == BPREDSLICE)
        {
            VM_ASSERT(sd->m_pRefPicList[1][0]);

            pRefIndexL1 = sd->m_cur_mb.GetReferenceIndexStruct(1)->refIndexs;
            // DIRECT MB have the same subblock partition structure as the
            // colocated MB. Take advantage of that to perform motion comp
            // for the direct MB using the largest partitions possible.
            if (mbtype == MBTYPE_DIRECT || mbtype == MBTYPE_SKIPPED)
            {
                mbtype = MBTYPE_INTER_8x8;
            }

            // Bi-dir weighting?
            weighted_bipred_idc = sd->m_pPicParamSet->weighted_bipred_idc;
            if (weighted_bipred_idc == 1)
            {
                // explicit bidir weighting
                luma_log2_weight_denom = sd->m_pSliceHeader->luma_log2_weight_denom;
                chroma_log2_weight_denom = sd->m_pSliceHeader->chroma_log2_weight_denom;
                bUnidirWeightMB = true;
                bBidirWeightMB = true;
            }
            if (weighted_bipred_idc == 2)
            {
                bBidirWeightMB = true;
            }
        }

        params.m_lumaInterpolateInfo.bitDepth = sd->bit_depth_luma;
        params.m_lumaInterpolateInfo.sizeFrame = sd->m_pCurrentFrame->lumaSize();
        params.m_lumaInterpolateInfo.sizeFrame.height >>= is_field;

        params.m_lumaInterpolateInfo.srcStep = sd->m_pCurrentFrame->pitch_luma();

        params.m_chromaInterpolateInfo.bitDepth = sd->bit_depth_chroma;
        params.m_chromaInterpolateInfo.sizeFrame = sd->m_pCurrentFrame->chromaSize();
        params.m_chromaInterpolateInfo.sizeFrame.height >>= is_field;

        params.m_chromaInterpolateInfo.srcStep = sd->m_pCurrentFrame->pitch_chroma();

        if (is_field)
        {
            params.m_lumaInterpolateInfo.srcStep *= 2;
            params.m_chromaInterpolateInfo.srcStep *= 2;
        }

        params.m_bidirLuma.bitDepth = sd->bit_depth_luma;
        params.m_bidirChroma[0].bitDepth = sd->bit_depth_chroma;
        params.m_bidirChroma[1].bitDepth = sd->bit_depth_chroma;

        Ipp32s iPitchLuma = pitch_luma;
        Ipp32s iPitchChroma = pitch_chroma;

        params.m_pMV[0] = sd->m_cur_mb.MVs[0]->MotionVectors;
        params.m_pMV[1] = (BPREDSLICE == sd->m_pSliceHeader->slice_type) ? (sd->m_cur_mb.MVs[1]->MotionVectors) : (NULL);
        params.m_pRefIndex[0] = pRefIndexL0;
        params.m_pRefIndex[1] = pRefIndexL1;
        params.m_iOffsetLuma = offsetY;
        params.m_iOffsetChroma = offsetC;
        params.m_lumaInterpolateInfo.pointBlockPos.x = mbXOffset;
        params.m_lumaInterpolateInfo.pointBlockPos.y = mbYOffset;
        params.m_chromaInterpolateInfo.pointBlockPos.x = mbXOffset >> point_width_chroma_div;
        params.m_chromaInterpolateInfo.pointBlockPos.y = mbYOffset >> point_height_chroma_div;
        if (is_weight)
        {
            params.luma_log2_weight_denom = luma_log2_weight_denom;
            params.chroma_log2_weight_denom = chroma_log2_weight_denom;
            params.weighted_bipred_idc = weighted_bipred_idc;
            params.m_bBidirWeightMB = bBidirWeightMB;
            params.m_bUnidirWeightMB = bUnidirWeightMB;
        }

        params.m_pSegDec = sd;

        if (mbtype != MBTYPE_INTER_8x8 && mbtype != MBTYPE_INTER_8x8_REF0)
        {
            // reconstruct macro block
            switch (mbtype)
            {
            case MBTYPE_INTER_16x8:
                {
                    params.m_lumaInterpolateInfo.sizeBlock.width = 16;
                    params.m_lumaInterpolateInfo.sizeBlock.height = 8;
                    params.m_chromaInterpolateInfo.sizeBlock.width = 16 >> roi_width_chroma_div;
                    params.m_chromaInterpolateInfo.sizeBlock.height = 8 >> roi_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 0;
                    params.m_iIntraMBChromaOffset = 0;

                    if (D_DIR_BIDIR == psbdir[0] || D_DIR_DIRECT_SPATIAL_BIDIR == psbdir[0])
                    {
                        params.m_iIntraMBLumaOffsetTmp = 0;
                        params.m_iIntraMBChromaOffsetTmp = 0;

                        CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                             0);
                    }
                    else
                    {
                        Ipp32s iDir = ((D_DIR_BWD == psbdir[0]) || (D_DIR_DIRECT_SPATIAL_BWD == psbdir[0])) ?
                                       (D_DIR_BWD) :
                                       (D_DIR_FWD);

                        CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                              0);
                    }

                    // set sub-block offset for second half of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.y += 8;
                    params.m_chromaInterpolateInfo.pointBlockPos.y += 8 >> point_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 8 * iPitchLuma;
                    params.m_iIntraMBChromaOffset = (8 >> height_chroma_div) * iPitchChroma;

                    if (D_DIR_BIDIR == psbdir[1] || D_DIR_DIRECT_SPATIAL_BIDIR == psbdir[1])
                    {
                        params.m_iIntraMBLumaOffsetTmp = 8 * 16;
                        params.m_iIntraMBChromaOffsetTmp = (8 >> height_chroma_div) * 16;

                        CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                             8);
                    }
                    else
                    {
                        Ipp32s iDir = ((D_DIR_BWD == psbdir[1]) || (D_DIR_DIRECT_SPATIAL_BWD == psbdir[1])) ?
                                       (D_DIR_BWD) :
                                       (D_DIR_FWD);

                        CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                              8);
                    }
                }
                break;

            case MBTYPE_INTER_8x16:
                {
                    params.m_lumaInterpolateInfo.sizeBlock.width = 8;
                    params.m_lumaInterpolateInfo.sizeBlock.height = 16;
                    params.m_chromaInterpolateInfo.sizeBlock.width = 8 >> roi_width_chroma_div;
                    params.m_chromaInterpolateInfo.sizeBlock.height = 16 >> roi_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 0;
                    params.m_iIntraMBChromaOffset = 0;

                    if (D_DIR_BIDIR == psbdir[0] || D_DIR_DIRECT_SPATIAL_BIDIR == psbdir[0])
                    {
                        params.m_iIntraMBLumaOffsetTmp = 0;
                        params.m_iIntraMBChromaOffsetTmp = 0;

                        CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                             0);
                    }
                    else
                    {
                        Ipp32s iDir = ((D_DIR_BWD == psbdir[0]) || (D_DIR_DIRECT_SPATIAL_BWD == psbdir[0])) ?
                                       (D_DIR_BWD) :
                                       (D_DIR_FWD);

                        CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                              0);
                    }

                    // set sub-block offset for second half of MB
                    params.m_lumaInterpolateInfo.pointBlockPos.x += 8;
                    params.m_chromaInterpolateInfo.pointBlockPos.x += 8 >> point_width_chroma_div;
                    params.m_iIntraMBLumaOffset = 8;
                    params.m_iIntraMBChromaOffset = 8 >> width_chroma_div;

                    if (D_DIR_BIDIR == psbdir[1] || D_DIR_DIRECT_SPATIAL_BIDIR == psbdir[1])
                    {
                        params.m_iIntraMBLumaOffsetTmp = 8;
                        params.m_iIntraMBChromaOffsetTmp = 8 >> width_chroma_div;

                        CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                             2);
                    }
                    else
                    {
                        Ipp32s iDir = ((D_DIR_BWD == psbdir[1]) || (D_DIR_DIRECT_SPATIAL_BWD == psbdir[1])) ?
                                       (D_DIR_BWD) :
                                       (D_DIR_FWD);

                        CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                              2);
                    }
                }
                break;

            default:
                {
                    params.m_lumaInterpolateInfo.sizeBlock.width = 16;
                    params.m_lumaInterpolateInfo.sizeBlock.height = 16;
                    params.m_chromaInterpolateInfo.sizeBlock.width = 16 >> roi_width_chroma_div;
                    params.m_chromaInterpolateInfo.sizeBlock.height = 16 >> roi_height_chroma_div;
                    params.m_iIntraMBLumaOffset = 0;
                    params.m_iIntraMBChromaOffset = 0;

                    if (MBTYPE_BIDIR == mbtype)
                    {
                        params.m_iIntraMBLumaOffsetTmp = 0;
                        params.m_iIntraMBChromaOffsetTmp = 0;

                        CompensateBiDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                                             0);
                    }
                    else
                    {
                        Ipp32s iDir = (MBTYPE_BACKWARD == mbtype) ? (D_DIR_BWD) : (D_DIR_FWD);

                        CompensateUniDirBlock(params, pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma, iDir,
                                              0);
                    }
                }
                break;
            }
        }
        else
        {
            Ipp8s *pSubBlockType = sd->m_cur_mb.GlobalMacroblockInfo->sbtype;
            Ipp8s *pSubBlockDir = sd->m_cur_mb.LocalMacroblockInfo->sbdir;

            // sub block 0
            CompensateBlock8x8(pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                params, pSubBlockType[0], pSubBlockDir[0], 0);

            // sub block 1
            pDstY += 8;
            pDstU += (8 >> width_chroma_div);
            pDstV += (8 >> width_chroma_div);
            params.m_iOffsetLuma = offsetY + 8;
            params.m_iOffsetChroma = offsetC + (8 >> width_chroma_div);
            params.m_lumaInterpolateInfo.pointBlockPos.x = mbXOffset + 8;
            params.m_lumaInterpolateInfo.pointBlockPos.y = mbYOffset;
            params.m_chromaInterpolateInfo.pointBlockPos.x = (mbXOffset + 8) >> point_width_chroma_div;
            params.m_chromaInterpolateInfo.pointBlockPos.y = mbYOffset >> point_height_chroma_div;
            CompensateBlock8x8(pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                params, pSubBlockType[1], pSubBlockDir[1], 2);

            // sub block 2
            pDstY += - 8 + 8 * iPitchLuma;
            pDstU += - (8 >> width_chroma_div)  +
                     (8 >> height_chroma_div) * iPitchChroma;
            pDstV += - (8 >> width_chroma_div) +
                     (8 >> height_chroma_div) * iPitchChroma;
            params.m_iOffsetLuma = offsetY + 8 * iPitchLuma;
            params.m_iOffsetChroma = offsetC + (8 >> height_chroma_div) * iPitchChroma;
            params.m_lumaInterpolateInfo.pointBlockPos.x = mbXOffset;
            params.m_lumaInterpolateInfo.pointBlockPos.y = mbYOffset + 8;
            params.m_chromaInterpolateInfo.pointBlockPos.x = mbXOffset >> point_width_chroma_div;
            params.m_chromaInterpolateInfo.pointBlockPos.y = (mbYOffset + 8) >> point_height_chroma_div;
            CompensateBlock8x8(pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                params, pSubBlockType[2], pSubBlockDir[2], 8);

            // sub block 3
            pDstY += 8;
            pDstU += (8 >> width_chroma_div);
            pDstV += (8 >> width_chroma_div);
            params.m_iOffsetLuma = offsetY + 8 + 8 * iPitchLuma;
            params.m_iOffsetChroma = offsetC + (8 >> width_chroma_div) +
                                     (8 >> height_chroma_div) * iPitchChroma;
            params.m_lumaInterpolateInfo.pointBlockPos.x = mbXOffset + 8;
            params.m_lumaInterpolateInfo.pointBlockPos.y = mbYOffset + 8;
            params.m_chromaInterpolateInfo.pointBlockPos.x = (mbXOffset + 8) >> point_width_chroma_div;
            params.m_chromaInterpolateInfo.pointBlockPos.y = (mbYOffset + 8) >> point_height_chroma_div;
            CompensateBlock8x8(pDstY, pDstU, pDstV, iPitchLuma, iPitchChroma,
                params, pSubBlockType[3], pSubBlockDir[3], 10);
        }
    } // void CompensateMotionMacroBlock(PlanePtrY pDstY,

    ////////////////////////////////////////////////////////////////////////////////
    // Copy raw pixel values from the bitstream to the reconstructed frame for
    // all luma and chroma blocks of one macroblock.
    ////////////////////////////////////////////////////////////////////////////////
    void ReconstructPCMMB(Ipp32u offsetY, Ipp32u offsetC, Ipp32s pitch_luma, Ipp32s pitch_chroma,
                          H264SegmentDecoder * sd)
    {
        PlanePtrY  pDstY;
        PlanePtrUV pDstU;
        PlanePtrUV pDstV;
        Ipp32s i;

        // to retrieve non-aligned pointer from m_pCoeffBlocksRead
        pDstY = (PlanePtrY)sd->m_pYPlane + offsetY;
        pDstU = (PlanePtrUV)sd->m_pUPlane + offsetC;
        pDstV = (PlanePtrUV)sd->m_pVPlane + offsetC;

        PlanePtrY pCoeffBlocksRead_Y = reinterpret_cast<PlanePtrY> (sd->m_pCoeffBlocksRead);
        // get pointer to raw bytes from m_pCoeffBlocksRead
        for (i = 0; i<16; i++)
            ippsCopy_8u((Ipp8u*)(pCoeffBlocksRead_Y + i * 16), (Ipp8u*)(pDstY + i * pitch_luma), 16*sizeof(PlaneY));

        sd->m_pCoeffBlocksRead = (UMC::CoeffsPtrCommon)((Ipp8u*)sd->m_pCoeffBlocksRead +
                                 256*sizeof(PlaneY));

        if (color_format)
        {
            Ipp32s iWidth = (color_format == 3) ? 16 : 8;
            Ipp32s iHeight = 8 + 8 * (color_format >> 1);

            {
                PlanePtrUV pCoeffBlocksRead_UV = (PlanePtrUV) (sd->m_pCoeffBlocksRead);
                for (i = 0; i < iHeight; i += 1)
                    ippsCopy_8u((Ipp8u*)(pCoeffBlocksRead_UV + i * iWidth), (Ipp8u*)(pDstU + i * pitch_chroma), iWidth*sizeof(PlaneUV));

                pCoeffBlocksRead_UV += iWidth * iHeight * sizeof(PlaneUV);

                for (i = 0; i < iHeight; i += 1)
                    ippsCopy_8u((Ipp8u*)(pCoeffBlocksRead_UV + i * iWidth), (Ipp8u*)(pDstV + i * pitch_chroma), iWidth*sizeof(PlaneUV));

                sd->m_pCoeffBlocksRead = (UMC::CoeffsPtrCommon)((Ipp8u*)sd->m_pCoeffBlocksRead +
                    2* iWidth * iHeight * sizeof(PlaneUV));
            }
        }
    } // void ReconstructPCMMB(
};
#if defined(_MSC_VER)
#pragma warning(default: 4127)
#endif

} // namespace UMC

#endif // __UMC_H264_RECONSTRUCT_TEMPLATES_H
