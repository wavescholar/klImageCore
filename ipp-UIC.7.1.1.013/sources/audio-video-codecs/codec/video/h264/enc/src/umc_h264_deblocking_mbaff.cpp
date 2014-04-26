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

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include "umc_h264_deblocking.h"
#include "umc_h264_video_encoder.h"
#include "vm_types.h"
#include "vm_debug.h"
#include "umc_h264_to_ipp.h"
#include "umc_h264_bme.h"

inline IppStatus ownFilterDeblockingLuma_VerEdge_MBAFF_H264(
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp32u nAlpha,
    Ipp32u nBetha,
    Ipp8u* pThresholds,
    Ipp8u* pBs,
    Ipp32s)
{
    return(ippiFilterDeblockingLuma_VerEdge_MBAFF_H264_8u_C1IR(pSrcDst, srcdstStep, nAlpha, nBetha, pThresholds, pBs));
}

inline IppStatus ownFilterDeblockingChroma_VerEdge_MBAFF_H264(
    Ipp8u* pSrcDst,
    Ipp32s srcdstStep,
    Ipp32u nAlpha,
    Ipp32u nBetha,
    Ipp8u* pThresholds,
    Ipp8u* pBs,
    Ipp32s)
{
    return(ippiFilterDeblockingChroma_VerEdge_MBAFF_H264_8u_C1IR(pSrcDst, srcdstStep, nAlpha, nBetha, pThresholds, pBs));
}

inline IppStatus ownFilterDeblockingLuma_VerEdge_MBAFF_H264(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp32u  nAlpha,
    Ipp32u  nBetha,
    Ipp8u*  pThresholds,
    Ipp8u*  pBs,
    Ipp32s  bitdepth)
{
    Ipp16u a[2], b[2];

    a[0] = nAlpha;
    b[0] = nBetha;
    IppiFilterDeblock_16u d;
    d.pSrcDstPlane = pSrcDst;
    d.srcDstStep = srcdstStep;
    d.pAlpha = a;
    d.pBeta = b;
    d.pThresholds = (Ipp16u*)pThresholds;
    d.pBs = pBs;
    d.bitDepth = bitdepth;
    return(ippiFilterDeblockingLumaVerEdgeMBAFF_H264_16u_C1IR(&d));
}

inline IppStatus ownFilterDeblockingChroma_VerEdge_MBAFF_H264(
    Ipp16u* pSrcDst,
    Ipp32s  srcdstStep,
    Ipp32u  nAlpha,
    Ipp32u  nBetha,
    Ipp8u*  pThresholds,
    Ipp8u*  pBs,
    Ipp32s  bitdepth)
{
    Ipp16u a[2],b[2];

    a[0] = nAlpha;
    b[0] = nBetha;
    /// Should be corrected
    IppiFilterDeblock_16u d;
    d.pSrcDstPlane = pSrcDst;
    d.srcDstStep = srcdstStep;
    d.pAlpha = a;
    d.pBeta = b;
    d.pThresholds = (Ipp16u*)pThresholds;
    d.pBs = pBs;
    d.bitDepth = bitdepth;
    return(ippiFilterDeblockingChromaVerEdgeMBAFF_H264_16u_C1IR(&d));
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockChromaHorizontalMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp8u bTmp[16];
    Ipp32s pitchPixels = pParams->m_base.pitchPixels;

    //
    // chroma deblocking
    //

    //
    // step 1. Deblock origin external edge
    //
    {
        // save internal edges strength
        CopyEdgeStrength(bTmp + 0, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0);
        CopyEdgeStrength(bTmp + 4, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4);
        CopyEdgeStrength(bTmp + 8, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8);
        CopyEdgeStrength(bTmp + 12, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12);
        // skip all internal edges
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4, 0);
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8, 0);
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12, 0);
        // set pitch
        pParams->m_base.pitchPixels *= 2;
        // perform deblocking
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
    }

    //
    // step 2. Deblock extra external edge
    //
    {
        // set extra edge strength
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING], pParams->StrengthExtra);
        // correct neighbour MB info
        pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] += ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
        // correct U & V pointer
        pParams->m_base.pU += pitchPixels;
        pParams->m_base.pV += pitchPixels;
        // perform deblocking
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
        // restore values
        pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] -= ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
        pParams->m_base.pU -= pitchPixels;
        pParams->m_base.pV -= pitchPixels;
        pParams->m_base.pitchPixels = pitchPixels;
    }

    //
    // step 3. Deblock internal edges
    //
    {
        pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
        // set internal edge strength
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0, 0);
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4, bTmp + 4);
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8, bTmp + 8);
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12, bTmp + 12);
        // perform deblocking
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
        // restore strength
        pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 1;
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0, bTmp + 0);
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockChromaVerticalMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    // do not deblocking of 4:0:0 format
    if (core_enc->m_PicParamSet.chroma_format_idc)
    {
        //
        // step 1. Perform complex deblocking on external edge
        //
        {
            Ipp32u MBAddr = pParams->m_base.nMBAddr;
            Ipp8u Clipping[16];
            Ipp8u Alpha[2];
            Ipp8u Beta[2];
            Ipp32s AlphaC0Offset = pParams->m_base.nAlphaC0Offset;
            Ipp32s BetaOffset = pParams->m_base.nBetaOffset;
            Ipp32s pmq_QP = core_enc->m_mbinfo.mbs[MBAddr].QP;
            Ipp32s pmp_QP;
            Ipp8u *pClipTab;
            Ipp32s QP;
            Ipp32s index;
            Ipp8u *pStrength = pParams->StrengthComplex;
            Ipp32u nPlane;

            for (nPlane = 0; nPlane < 2; nPlane += 1)
            {
                // prepare variables
                if ((0 == nPlane) || (core_enc->m_PicParamSet.chroma_qp_index_offset != core_enc->m_PicParamSet.second_chroma_qp_index_offset))
                {
                    Ipp32u i;
                    Ipp32s chroma_qp_offset = (0 == nPlane) ? (core_enc->m_PicParamSet.chroma_qp_index_offset) : (core_enc->m_PicParamSet.second_chroma_qp_index_offset);


                    for (i = 0; i < 2; i += 1)
                    {
                        // get upper neighbour block QP
                        pmp_QP = core_enc->m_mbinfo.mbs[pParams->nLeft[i]].QP;

                        // external edge variables averaging???
                        QP = ENCODER_QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];
                        //QP = ENCODER_QP_SCALE_CR[getChromaQP(pmp_QP, chroma_qp_offset, core_enc->m_SeqParamSet.bit_depth_chroma)];

                        // external edge variables
                        index = IClip(0, 51, QP + BetaOffset);
                        Beta[i] = getEncoderBethaTable(index);

                        index = IClip(0, 51, QP + AlphaC0Offset);
                        Alpha[i] = getEncoderAlphaTable(index);
                        pClipTab = getEncoderClipTab(index);

                        // create clipping values
                        Clipping[i * 4 + 0] = pClipTab[pStrength[i * 4 + 0]];
                        Clipping[i * 4 + 1] = pClipTab[pStrength[i * 4 + 1]];
                        Clipping[i * 4 + 2] = pClipTab[pStrength[i * 4 + 2]];
                        Clipping[i * 4 + 3] = pClipTab[pStrength[i * 4 + 3]];
                    }
                }

                // perform deblocking
                if (0 == pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr))
                {
                    ownFilterDeblockingChroma_VerEdge_MBAFF_H264(
                        ((0 == nPlane) ? (pParams->m_base.pU) : (pParams->m_base.pV)),
                        pParams->m_base.pitchPixels * 2 * sizeof(PIXTYPE),
                        Alpha[0],
                        Beta[0],
                        Clipping,
                        pStrength,
                        core_enc->m_SeqParamSet.bit_depth_chroma);
                    ownFilterDeblockingChroma_VerEdge_MBAFF_H264(
                        ((0 == nPlane) ? (pParams->m_base.pU) : (pParams->m_base.pV)) + pParams->m_base.pitchPixels,
                        pParams->m_base.pitchPixels * 2 * sizeof(PIXTYPE),
                        Alpha[1],
                        Beta[1],
                        Clipping + 4,
                        pStrength + 4,
                        core_enc->m_SeqParamSet.bit_depth_chroma);
                }
                else
                {
                    ownFilterDeblockingChroma_VerEdge_MBAFF_H264(
                        ((0 == nPlane) ? (pParams->m_base.pU) : (pParams->m_base.pV)),
                        pParams->m_base.pitchPixels*sizeof(PIXTYPE),
                        Alpha[0],
                        Beta[0],
                        Clipping,
                        pStrength,
                        core_enc->m_SeqParamSet.bit_depth_chroma);
                    ownFilterDeblockingChroma_VerEdge_MBAFF_H264(
                        ((0 == nPlane) ? (pParams->m_base.pU) : (pParams->m_base.pV)) + pParams->m_base.pitchPixels * 4,
                        pParams->m_base.pitchPixels*sizeof(PIXTYPE),
                        Alpha[1],
                        Beta[1],
                        Clipping + 4,
                        pStrength + 4,
                        core_enc->m_SeqParamSet.bit_depth_chroma);
                }
            }
        }

        //
        // step 2. Perform complex deblocking on internal edges
        //
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &pParams->m_base);
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockLumaHorizontalMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp8u bTmp[16];
    Ipp32s pitchPixels = pParams->m_base.pitchPixels;

    //
    // luma deblocking
    //

    //
    // step 1. Deblock origin external edge
    //
    {
        // save internal edges strength
        CopyEdgeStrength(bTmp + 0, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0);
        CopyEdgeStrength(bTmp + 4, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4);
        CopyEdgeStrength(bTmp + 8, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8);
        CopyEdgeStrength(bTmp + 12, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12);
        // skip all internal edges
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4, 0);
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8, 0);
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12, 0);
        // set pitch
        pParams->m_base.pitchPixels *= 2;
        // perform deblocking
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, (DeblockingParameters<PIXTYPE>*)pParams);
    }

    //
    // step 2. Deblock extra external edge
    //
    {
        // set extra edge strength
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING], pParams->StrengthExtra);
        // correct neighbour MB info
        pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] += ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
        // correct Y pointer
        pParams->m_base.pY += pitchPixels;
        // perform deblocking
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, (DeblockingParameters<PIXTYPE>*)pParams);
        // restore values
        pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] -= ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
        pParams->m_base.pY -= pitchPixels;
        pParams->m_base.pitchPixels = pitchPixels;
    }

    //
    // step 3. Deblock internal edges
    //
    {
        pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
        // set internal edge strength
        SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0, 0);
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4, bTmp + 4);
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8, bTmp + 8);
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12, bTmp + 12);
        // perform deblocking
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, (DeblockingParameters<PIXTYPE>*)pParams);
        // restore strength
        pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 1;
        CopyEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0, bTmp + 0);
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockLumaVerticalMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    //
    // step 1. Perform complex deblocking on external edge
    //
    {
        Ipp32u MBAddr = pParams->m_base.nMBAddr;
        Ipp8u Clipping[16];
        Ipp8u Alpha[2];
        Ipp8u Beta[2];
        Ipp32s AlphaC0Offset = pParams->m_base.nAlphaC0Offset;
        Ipp32s BetaOffset = pParams->m_base.nBetaOffset;
        Ipp32s pmq_QP = core_enc->m_mbinfo.mbs[MBAddr].QP;
        Ipp32s pmp_QP;
        Ipp8u *pClipTab;
        Ipp32s QP;
        Ipp32s index;
        Ipp8u *pStrength = pParams->StrengthComplex;
        Ipp32u i;

        // prepare variables
        for (i = 0;i < 2;i += 1)
        {
            // get upper neighbour block QP
            pmp_QP = core_enc->m_mbinfo.mbs[pParams->nLeft[i]].QP;

            // luma variables
            QP = (pmp_QP + pmq_QP + 1) >> 1 ;

            // external edge variables
            index = IClip(0, 51, QP + BetaOffset);
            Beta[i] = getEncoderBethaTable(index);

            index = IClip(0, 51, QP + AlphaC0Offset);
            Alpha[i] = getEncoderAlphaTable(index);
            pClipTab = getEncoderClipTab(index);

            // create clipping values
            Clipping[i * 4 + 0] = pClipTab[pStrength[i * 4 + 0]];
            Clipping[i * 4 + 1] = pClipTab[pStrength[i * 4 + 1]];
            Clipping[i * 4 + 2] = pClipTab[pStrength[i * 4 + 2]];
            Clipping[i * 4 + 3] = pClipTab[pStrength[i * 4 + 3]];
        }

        // perform deblocking
        if (0 == pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr))
        {
            ownFilterDeblockingLuma_VerEdge_MBAFF_H264(
                pParams->m_base.pY,
                pParams->m_base.pitchPixels * 2 * sizeof(PIXTYPE),
                Alpha[0],
                Beta[0],
                Clipping,
                pStrength,
                core_enc->m_PicParamSet.bit_depth_luma);
            ownFilterDeblockingLuma_VerEdge_MBAFF_H264(
                pParams->m_base.pY + pParams->m_base.pitchPixels,
                pParams->m_base.pitchPixels * 2 * sizeof(PIXTYPE),
                Alpha[1],
                Beta[1],
                Clipping + 4,
                pStrength + 4,
                core_enc->m_PicParamSet.bit_depth_luma);
        }
        else
        {
            ownFilterDeblockingLuma_VerEdge_MBAFF_H264(
                pParams->m_base.pY,
                pParams->m_base.pitchPixels*sizeof(PIXTYPE),
                Alpha[0],
                Beta[0],
                Clipping,
                pStrength,
                core_enc->m_PicParamSet.bit_depth_luma);
            ownFilterDeblockingLuma_VerEdge_MBAFF_H264(
                pParams->m_base.pY + pParams->m_base.pitchPixels * 8,
                pParams->m_base.pitchPixels*sizeof(PIXTYPE),
                Alpha[1],
                Beta[1],
                Clipping + 4,
                pStrength + 4,
                core_enc->m_PicParamSet.bit_depth_luma);
        }
    }

    //
    // step 2. Perform complex deblocking on internal edges
    //
    H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, (DeblockingParameters<PIXTYPE>*)pParams);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersISliceMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    // set deblocking flag(s)
    pParams->m_base.DeblockingFlag[VERTICAL_DEBLOCKING] = 1;
    pParams->m_base.DeblockingFlag[HORIZONTAL_DEBLOCKING] = 1;

    // fill "complex deblocking" strength
    if (pParams->UseComplexVerticalDeblocking)
    {
        SetEdgeStrength(pParams->StrengthComplex + 0, 4);
        SetEdgeStrength(pParams->StrengthComplex + 4, 4);
    }

    // calculate strengths
    if (pParams->m_base.ExternalEdgeFlag[VERTICAL_DEBLOCKING])
    {
        // deblocking with strong deblocking of external edge
        SetEdgeStrength(pParams->m_base.Strength[VERTICAL_DEBLOCKING] + 0, 4);
    }

    SetEdgeStrength(pParams->m_base.Strength[VERTICAL_DEBLOCKING] + 4, 3);
    SetEdgeStrength(pParams->m_base.Strength[VERTICAL_DEBLOCKING] + 8, 3);
    SetEdgeStrength(pParams->m_base.Strength[VERTICAL_DEBLOCKING] + 12, 3);

    if (pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING])
    {
        if ((pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->m_base.nMBAddr)) ||
            (pParams->ExtraHorizontalEdge))
        {
            // deblocking field macroblock with external edge
            SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0, 3);
        }
        else
        {
            // deblocking with strong deblocking of external edge
            SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 0, 4);
        }
    }

    SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 4, 3);
    SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 8, 3);
    SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING] + 12, 3);

    if (pParams->ExtraHorizontalEdge)
    {
        // set extra edge strength
        SetEdgeStrength(pParams->StrengthExtra + 0, 3);
        SetEdgeStrength(pParams->StrengthExtra + 4, 0);
        SetEdgeStrength(pParams->StrengthExtra + 8, 0);
        SetEdgeStrength(pParams->StrengthExtra + 12, 0);
    }
}


template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFComplexFrameExternalEdge(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->m_base.nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->StrengthComplex;
    Ipp32u i, nNeighbourBlockInc;

    // mixed edge is always deblocked
    {
        Ipp32u *pDeblockingFlag = &(pParams->m_base.DeblockingFlag[VERTICAL_DEBLOCKING]);
        *pDeblockingFlag = 1;
    }

    // calculate neighbour block addition
    nNeighbourBlockInc = ((pParams->m_base.nMBAddr&1)==1) ? (2) : (0);

    // we fill parameters using 2 passes
    for (i = 0;i < 2;i += 1)
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nLeft[i];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[VERTICAL_DEBLOCKING][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[VERTICAL_DEBLOCKING][NEIGHBOUR_BLOCK][idx / 2 + nNeighbourBlockInc];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                    pStrength[idx] = 2;
                // when blocks have no coeffs
                // set strength is 1
                else
                    pStrength[idx] = 1;
            }
        }
        // external edge required in strong filtering
        else
            SetEdgeStrength(pStrength + 0, 4);

        pStrength += 4;
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFComplexFieldExternalEdge(void* state, DeblockingParametersMBAFF<PIXTYPE>* pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->m_base.nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->StrengthComplex;
    Ipp32u i;

    // mixed edge is always deblocked
    {
        Ipp32u *pDeblockingFlag = &(pParams->m_base.DeblockingFlag[VERTICAL_DEBLOCKING]);
        *pDeblockingFlag = 1;
    }

    // we fill parameters using 2 passes
    for (i = 0;i < 2;i += 1)
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nLeft[i];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[VERTICAL_DEBLOCKING][CURRENT_BLOCK][idx / 2 + i * 2];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[VERTICAL_DEBLOCKING][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                    pStrength[idx] = 2;
                // when blocks have no coeffs
                // set strength is 1
                else
                    pStrength[idx] = 1;
            }
        }
        // external edge required in strong filtering
        else
            SetEdgeStrength(pStrength + 0, 4);

        pStrength += 4;
    }

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFField(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //

    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nBlock, nNeighbourBlock;
                    Ipp32s iRefQ, iRefP;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                    {
                        nBlock = idx * 4;
                        nNeighbourBlock = idx * 4 + 3;
                    }
                    else
                    {
                        nBlock = idx;
                        nNeighbourBlock = idx + 12;
                    }

                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for current block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefQ = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                        }
                        else
                            iRefQ = -1;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefP = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                        }
                        else
                            iRefP = -1;
                    }

                    VM_ASSERT((iRefP != -1) || (iRefQ != -1));

                    // when reference indexes are equal
                    if (iRefQ == iRefP)
                    {
                        H264MotionVector *pVectorQ, *pVectorP;

                        pVectorQ = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                        pVectorP = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;

                        // compare motion vectors
                        if ((4 <= abs(pVectorQ->mvx - pVectorP->mvx)) ||
                            (2 <= abs(pVectorQ->mvy - pVectorP->mvy)))
                        {
                            pStrength[idx] = 1;
                            *pDeblockingFlag = 1;
                        }
                        else
                            pStrength[idx] = 0;
                    }
                    // when reference indexes are different
                    else
                    {
                        pStrength[idx] = 1;
                        *pDeblockingFlag = 1;
                    }
                }
            }
        }
        // external edge required in strong filtering
        else
        {
            if (HORIZONTAL_DEBLOCKING == dir)
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            *pDeblockingFlag = 1;
        }
    }

    //
    // internal edge(s)
    //
    {
        Ipp32u idx;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // compare motion vectors & reference indexes
            else
            {
                Ipp32u nBlock, nNeighbourBlock;
                Ipp32s iRefQ, iRefP;

                // calc block and neighbour block number
                if (VERTICAL_DEBLOCKING == dir)
                {
                    nBlock = (idx & 3) * 4 + (idx >> 2);
                    nNeighbourBlock = nBlock - 1;
                }
                else
                {
                    nBlock = idx;
                    nNeighbourBlock = idx - 4;
                }

                VM_ASSERT(-1 == core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock]);
                VM_ASSERT(-1 == core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nNeighbourBlock]);

                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;

                    // select reference index for current block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                    iRefQ = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);

                    // select reference index for previous block
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefP = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                }

                VM_ASSERT((iRefP != -1) || (iRefQ != -1));

                // when reference indexes are equal
                if (iRefQ == iRefP)
                {
                    H264MotionVector *pVectorQ, *pVectorP;

                    pVectorQ = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                    pVectorP = core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;

                    // compare motion vectors
                    if ((4 <= abs(pVectorQ->mvx - pVectorP->mvx)) ||
                        (2 <= abs(pVectorQ->mvy - pVectorP->mvy)))
                    {
                        pStrength[idx] = 1;
                        *pDeblockingFlag = 1;
                    }
                    else
                        pStrength[idx] = 0;
                }
                // when reference indexes are different
                else
                {
                    pStrength[idx] = 1;
                    *pDeblockingFlag = 1;
                }
            }
        }
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge(void* state, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[HORIZONTAL_DEBLOCKING];
    Ipp32u nNeighbour;

    //
    // external edge
    //

    // mixed edge is always deblocked
    {
        Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[HORIZONTAL_DEBLOCKING]);
        *pDeblockingFlag = 1;
    }

    // select neighbour addres
    nNeighbour = pParams->nNeighbour[HORIZONTAL_DEBLOCKING];

    // when neighbour macroblock isn't intra
    if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
    {
        Ipp32u idx;

        // select neighbour
//fc        H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
        Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

        // cicle on blocks
        for (idx = 0;idx < 4;idx += 1)
        {
            Ipp32u blkQ, blkP;

            blkQ = ENCODER_EXTERNAL_BLOCK_MASK[HORIZONTAL_DEBLOCKING][CURRENT_BLOCK][idx];
            blkP = ENCODER_EXTERNAL_BLOCK_MASK[HORIZONTAL_DEBLOCKING][NEIGHBOUR_BLOCK][idx];

            // when one of couple of blocks has coeffs
            if ((cbp_luma & blkQ) ||
//fc                (pNeighbour->cbp_luma & blkP))
                (cbp_luma_nb & blkP))
                pStrength[idx] = 2;
            // when blocks nave no coeffs
            // we set strength is 1 in mixed mode
            else
                pStrength[idx] = 1;
        }
    }
    // external edge required in strong filtering
    else
        SetEdgeStrength(pStrength + 0, 3);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersPSliceMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u mbtype = (core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->m_base.nMBAddr)->mbtype;
    Ipp32u nAboveMBFieldCoded;

    // when this macroblock is intra coded
    if (IS_INTRA_MBTYPE(mbtype))
    {
        H264CoreEncoder_PrepareDeblockingParametersISliceMBAFF<COEFFSTYPE, PIXTYPE>(state, pParams);
        return;
    }

    nAboveMBFieldCoded = pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING]);
    // when there are no special cases
    if ((0 == nAboveMBFieldCoded) &&
        (0 == pParams->m_base.MBFieldCoded) &&
        (0 == pParams->UseComplexVerticalDeblocking))
    {
        // prepare whole macroblock parameters
        H264CoreEncoder_PrepareDeblockingParametersPSlice<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
        return;
    }

    // when current macroblock is frame coded
    if (0 == pParams->m_base.MBFieldCoded)
    {
        // prepare "complex vertical deblocking" parameters
        if (pParams->UseComplexVerticalDeblocking)
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFComplexFrameExternalEdge<COEFFSTYPE, PIXTYPE>(state, pParams);

        // fill vertical edges parameters
        H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &pParams->m_base);

        // fill extra edge parameters
        if (pParams->ExtraHorizontalEdge)
        {
            // set correct above neighbour
            pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] += ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
            // obtain parameters
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
            // restore above neighbour
            pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] -= ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
            // copy parameters to right place
            CopyEdgeStrength(pParams->StrengthExtra, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING]);

            // fill horizontal edges parameters
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
            H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 1;
        }
        else
            H264CoreEncoder_PrepareDeblockingParametersPSlice4<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);

    }
    // when current macroblock is field coded
    else
    {
        // prepare "complex vertical deblocking" parameters
        if (pParams->UseComplexVerticalDeblocking)
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFComplexFieldExternalEdge<COEFFSTYPE, PIXTYPE>(state, pParams);

        // fill vertical edges parameters
        H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFField<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &pParams->m_base);

        // when above macroblock is frame coded
        if (nAboveMBFieldCoded != (Ipp32u)pParams->m_base.MBFieldCoded)
        {
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFField<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 1;
        }
        else
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFField<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSlice4MBAFFField(void* state, Ipp32u dir, DeblockingParameters<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u MBAddr = pParams->nMBAddr;
//fc    Ipp32u cbp_luma = (core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma;
    Ipp32u cbp_luma = ((core_enc->m_mbinfo.mbs + MBAddr)->cbp_luma & 0xffff) << 1;
    Ipp8u *pStrength = pParams->Strength[dir];
    Ipp32u *pDeblockingFlag = &(pParams->DeblockingFlag[dir]);

    //
    // external edge
    //
    if (pParams->ExternalEdgeFlag[dir])
    {
        Ipp32u nNeighbour;

        // select neighbour addres
        nNeighbour = pParams->nNeighbour[dir];

        // when neighbour macroblock isn't intra
        if (!IS_INTRA_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->mbtype))
        {
            Ipp32u idx;

            // select neighbour
//fc            H264MacroblockLocalInfo *pNeighbour = core_enc->m_mbinfo.mbs + nNeighbour;
            Ipp32u cbp_luma_nb = ((core_enc->m_mbinfo.mbs + nNeighbour)->cbp_luma & 0xffff) << 1;

            // cicle on blocks
            for (idx = 0;idx < 4;idx += 1)
            {
                Ipp32u blkQ, blkP;

                blkQ = ENCODER_EXTERNAL_BLOCK_MASK[dir][CURRENT_BLOCK][idx];
                blkP = ENCODER_EXTERNAL_BLOCK_MASK[dir][NEIGHBOUR_BLOCK][idx];

                // when one of couple of blocks has coeffs
                if ((cbp_luma & blkQ) ||
//fc                    (pNeighbour->cbp_luma & blkP))
                    (cbp_luma_nb & blkP))
                {
                    pStrength[idx] = 2;
                    *pDeblockingFlag = 1;
                }
                // compare motion vectors & reference indexes
                else
                {
                    Ipp32u nBlock, nNeighbourBlock;
                    Ipp32s iRefQFrw, iRefPFrw, iRefQBck, iRefPBck;

                    // calc block and neighbour block number
                    if (VERTICAL_DEBLOCKING == dir)
                    {
                        nBlock = idx * 4;
                        nNeighbourBlock = nBlock + 3;
                    }
                    else
                    {
                        nBlock = idx;
                        nNeighbourBlock = idx + 12;
                    }

                    {
                        H264EncoderFrame<PIXTYPE> **pRefPicList;
                        Ipp32s index;

                        // select reference index for current block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefQFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                        }
                        else
                            iRefQFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefQBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                        }
                        else
                            iRefQBck = -1;

                        // select reference index for previous block
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                0,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPFrw = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                        }
                        else
                            iRefPFrw = -1;
                        index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][nNeighbour].RefIdxs[nNeighbourBlock];
                        if (0 <= index)
                        {
                            pRefPicList = H264EncoderFrame_GetRefPicList(
                                core_enc->m_pCurrentFrame,
                                (core_enc->m_pCurrentFrame->m_mbinfo.mbs + nNeighbour)->slice_id,
                                1,
                                core_enc->m_MaxSliceSize)->m_RefPicList;
                            iRefPBck = H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                        }
                        else
                            iRefPBck = -1;
                    }

                    // when reference indexes are equal
                    if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                        ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                    {
                        // set initial value of strength
                        pStrength[idx] = 0;

                        // when forward and backward reference pictures of previous block are different
                        if (iRefPFrw != iRefPBck)
                        {
                            const H264MotionVector *pVectorQFrw, *pVectorQBck;
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select current block motion vectors
                            pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                            pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                            // select previous block motion vectors
                            if (iRefQFrw == iRefPFrw)
                            {
                                pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                            }
                            else
                            {
                                pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;
                                pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            }

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (2 <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (2 <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                        // when forward and backward reference pictures of previous block are equal
                        else
                        {
                            const H264MotionVector *pVectorQFrw, *pVectorQBck;
                            const H264MotionVector *pVectorPFrw, *pVectorPBck;

                            // select current block motion vectors
                            pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                            pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                            // select previous block motion vectors
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][nNeighbour].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][nNeighbour].MotionVectors + nNeighbourBlock;

                            // compare motion vectors
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                                (2 <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                                (2 <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                            {
                                if ((4 <= abs(pVectorQFrw->mvx - pVectorPBck->mvx)) ||
                                    (2 <= abs(pVectorQFrw->mvy - pVectorPBck->mvy)) ||
                                    (4 <= abs(pVectorQBck->mvx - pVectorPFrw->mvx)) ||
                                    (2 <= abs(pVectorQBck->mvy - pVectorPFrw->mvy)))
                                {
                                    pStrength[idx] = 1;
                                    *pDeblockingFlag = 1;
                                }
                            }
                        }
                    }
                    // when reference indexes are different
                    else
                    {
                        pStrength[idx] = 1;
                        *pDeblockingFlag = 1;
                    }
                }
            }
        }
        // external edge required in strong filtering
        else
        {
            if (HORIZONTAL_DEBLOCKING == dir)
                SetEdgeStrength(pStrength + 0, 3);
            else
                SetEdgeStrength(pStrength + 0, 4);
            *pDeblockingFlag = 1;
        }
    }

    //
    // internal edge(s)
    //
    {
        Ipp32u idx;

        // cicle of edge(s)
        // we do all edges in one cicle
        for (idx = 4;idx < 16;idx += 1)
        {
            Ipp32u blkQ;

            blkQ = ENCODER_INTERNAL_BLOCKS_MASK[dir][idx - 4];

            if (cbp_luma & blkQ)
            {
                pStrength[idx] = 2;
                *pDeblockingFlag = 1;
            }
            // compare motion vectors & reference indexes
            else
            {
                Ipp32u nBlock, nNeighbourBlock;
                Ipp32s iRefQFrw, iRefQBck, iRefPFrw, iRefPBck;

                // calc block and neighbour block number
                if (VERTICAL_DEBLOCKING == dir)
                {
                    nBlock = (idx & 3) * 4 + (idx >> 2);
                    nNeighbourBlock = nBlock - 1;
                }
                else
                {
                    nBlock = idx;
                    nNeighbourBlock = idx - 4;
                }

                {
                    H264EncoderFrame<PIXTYPE> **pRefPicList;
                    Ipp32s index;

                    // select forward reference pictures list
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        0,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select forward reference index for block(s)
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nBlock];
                    iRefQFrw = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[0][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefPFrw = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);

                    // select backward reference pictures list
                    pRefPicList = H264EncoderFrame_GetRefPicList(
                        core_enc->m_pCurrentFrame,
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr)->slice_id,
                        1,
                        core_enc->m_MaxSliceSize)->m_RefPicList;
                    // select backward reference index for block(s)
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nBlock];
                    iRefQBck = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                    index = core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[1][MBAddr].RefIdxs[nNeighbourBlock];
                    iRefPBck = (index < 0) ?
                            (-1) :
                            H264EncoderFrame_DeblockPicID(
                                pRefPicList[index >> 1],
                                index & 1);
                }

                // when reference indexes are equal
                if (((iRefQFrw == iRefPFrw) && (iRefQBck == iRefPBck)) ||
                    ((iRefQFrw == iRefPBck) && (iRefQBck == iRefPFrw)))
                {
                    // set initial value of strength
                    pStrength[idx] = 0;

                    // when forward and backward reference pictures of previous block are different
                    if (iRefPFrw != iRefPBck)
                    {
                        const H264MotionVector *pVectorQFrw, *pVectorQBck;
                        const H264MotionVector *pVectorPFrw, *pVectorPBck;

                        // select current block motion vectors
                        pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                        pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                        // select previous block motion vectors
                        if (iRefQFrw == iRefPFrw)
                        {
                            pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nNeighbourBlock;
                        }
                        else
                        {
                            pVectorPFrw = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nNeighbourBlock;
                            pVectorPBck = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;
                        }

                        // compare motion vectors
                        if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                            (2 <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                            (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                            (2 <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                        {
                            pStrength[idx] = 1;
                            *pDeblockingFlag = 1;
                        }
                    }
                    // when forward and backward reference pictures of previous block are equal
                    else
                    {
                        const H264MotionVector *pVectorQFrw, *pVectorQBck;
                        const H264MotionVector *pVectorPFrw, *pVectorPBck;

                        // select current block motion vectors
                        pVectorQFrw = (iRefQFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nBlock;
                        pVectorQBck = (iRefQBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nBlock;

                        // select previous block motion vectors
                        pVectorPFrw = (iRefPFrw < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[0][MBAddr].MotionVectors + nNeighbourBlock;
                        pVectorPBck = (iRefPBck < 0)? &null_mv: core_enc->m_pCurrentFrame->m_mbinfo.MV[1][MBAddr].MotionVectors + nNeighbourBlock;

                        // compare motion vectors
                        if ((4 <= abs(pVectorQFrw->mvx - pVectorPFrw->mvx)) ||
                            (2 <= abs(pVectorQFrw->mvy - pVectorPFrw->mvy)) ||
                            (4 <= abs(pVectorQBck->mvx - pVectorPBck->mvx)) ||
                            (2 <= abs(pVectorQBck->mvy - pVectorPBck->mvy)))
                        {
                            if ((4 <= abs(pVectorQFrw->mvx - pVectorPBck->mvx)) ||
                                (2 <= abs(pVectorQFrw->mvy - pVectorPBck->mvy)) ||
                                (4 <= abs(pVectorQBck->mvx - pVectorPFrw->mvx)) ||
                                (2 <= abs(pVectorQBck->mvy - pVectorPFrw->mvy)))
                            {
                                pStrength[idx] = 1;
                                *pDeblockingFlag = 1;
                            }
                        }
                    }
                }
                // when reference indexes are different
                else
                {
                    pStrength[idx] = 1;
                    *pDeblockingFlag = 1;
                }
            }
        }
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_PrepareDeblockingParametersBSliceMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE> *pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u mbtype = (core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->m_base.nMBAddr)->mbtype;
    Ipp32u nAboveMBFieldCoded;

    // when this macroblock is intra coded
    if (IS_INTRA_MBTYPE(mbtype))
    {
        H264CoreEncoder_PrepareDeblockingParametersISliceMBAFF<COEFFSTYPE, PIXTYPE>(state, pParams);
        return;
    }

    nAboveMBFieldCoded = pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING]);
    // when there are no special cases
    if ((0 == nAboveMBFieldCoded) &&
        (0 == pParams->m_base.MBFieldCoded) &&
        (0 == pParams->UseComplexVerticalDeblocking))
    {
        // prepare whole macroblock parameters
        H264CoreEncoder_PrepareDeblockingParametersBSlice<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
        return;
    }

    // when current macroblock is frame coded
    if (0 == pParams->m_base.MBFieldCoded)
    {
        // prepare "complex vertical deblocking" parameters
        if (pParams->UseComplexVerticalDeblocking)
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFComplexFrameExternalEdge<COEFFSTYPE, PIXTYPE>(state, pParams);

        // fill vertical edges parameters
        H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &pParams->m_base);

        // fill extra edge parameters
        if (pParams->ExtraHorizontalEdge)
        {
            // set correct above neighbour
            pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] += ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
            // obtain parameters
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
            // restore above neighbour
            pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] -= ((pParams->m_base.nMBAddr) & 1) ? 2*core_enc->m_WidthInMBs - 1: 1;
            // copy parameters to right place
            CopyEdgeStrength(pParams->StrengthExtra, pParams->m_base.Strength[HORIZONTAL_DEBLOCKING]);

            // fill horizontal edges parameters
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
            H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 1;
        }
        else
            H264CoreEncoder_PrepareDeblockingParametersBSlice4<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);

    }
    // when current macroblock is field coded
    else
    {
        // prepare "complex vertical deblocking" parameters
        if (pParams->UseComplexVerticalDeblocking)
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFComplexFieldExternalEdge<COEFFSTYPE, PIXTYPE>(state, pParams);

        // fill vertical edges parameters
        H264CoreEncoder_PrepareDeblockingParametersBSlice4MBAFFField<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &pParams->m_base);

        // when above macroblock is frame coded
        if ((nAboveMBFieldCoded != (Ipp32u)pParams->m_base.MBFieldCoded) &&
            (pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING]))
        {
            H264CoreEncoder_PrepareDeblockingParametersPSlice4MBAFFMixedExternalEdge<COEFFSTYPE, PIXTYPE>(state, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
            H264CoreEncoder_PrepareDeblockingParametersBSlice4MBAFFField<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
            pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 1;
        }
        else
            H264CoreEncoder_PrepareDeblockingParametersBSlice4MBAFFField<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &pParams->m_base);
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_ResetDeblockingVariablesMBAFF(void* state, DeblockingParametersMBAFF<PIXTYPE>* pParams)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    PIXTYPE *pY, *pU, *pV;
    Ipp32s offset;
    Ipp32s mbXOffset, mbYOffset;
    Ipp32s pic_pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels;
    Ipp32s MBAddr = pParams->m_base.nMBAddr;
    Ipp32s nCurrMB_X, nCurrMB_Y;
    Ipp32u nFieldMacroblockMode;

/*    // from linear MB number to MBAFF MB number
    {
        Ipp32u nRemain;
        nRemain = MBAddr % (mb_width * 2);
        MBAddr = MBAddr - nRemain + nRemain / 2;
        if (nRemain & 1)
            MBAddr += mb_width;
    }
*/
    // load slice header
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice = &core_enc->m_Slices[core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr].slice_id];
    // load planes
    pY = (PIXTYPE*)core_enc->m_pReconstructFrame->m_pYPlane;
    pU = (PIXTYPE*)core_enc->m_pReconstructFrame->m_pUPlane;
    pV = (PIXTYPE*)core_enc->m_pReconstructFrame->m_pVPlane;

    // prepare macroblock variables
    nCurrMB_X = ((MBAddr>>1) % core_enc->m_WidthInMBs);
    nCurrMB_Y = ((MBAddr>>1) / core_enc->m_WidthInMBs)*2 + (MBAddr&1);

    mbXOffset = nCurrMB_X * 16;
    mbYOffset = nCurrMB_Y * 16;

    // obtain macroblock mode
    nFieldMacroblockMode = pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr);
    // calc plane's offsets
    offset = core_enc->m_pMBOffsets[MBAddr].uLumaOffset[1][nFieldMacroblockMode];
    pY += offset;
    offset = core_enc->m_pMBOffsets[MBAddr].uChromaOffset[1][nFieldMacroblockMode];
    pU += offset;
    pV += offset;
    // correct y, u & v offset
    if (nFieldMacroblockMode)
    {
        pic_pitchPixels *= 2;
    }

    // set external edge variables
    // there are only two values:
    // zero - do not deblocking
    // nonzero - do deblocking
    pParams->m_base.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = nCurrMB_X;
    pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = (nFieldMacroblockMode) ? (1 < nCurrMB_Y) : (nCurrMB_Y);

    if (DEBLOCK_FILTER_ON_NO_SLICE_EDGES == curr_slice->m_disable_deblocking_filter_idc)
    {
        // don't filter at slice boundaries
        if (pParams->m_base.ExternalEdgeFlag[VERTICAL_DEBLOCKING])
        {
            if (core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr].slice_id !=
                core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr - 2].slice_id)
                pParams->m_base.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = 0;
        }

        if (pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING])
        {
            if (core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr].slice_id !=
                core_enc->m_pCurrentFrame->m_mbinfo.mbs[MBAddr - core_enc->m_WidthInMBs * 2].slice_id)
                pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
        }
    }

    // set left MB couple adresses
    {
        Ipp32u nRemain;
        nRemain = pParams->m_base.nMBAddr % (core_enc->m_WidthInMBs * 2);
        pParams->nLeft[0] = pParams->m_base.nMBAddr - nRemain + nRemain / 2 - 1;
        pParams->nLeft[1] = pParams->nLeft[0] + 1;
    }

    // set "complex deblocking" flag
    // when left & current macroblock are coded in different modes
    pParams->UseComplexVerticalDeblocking = 0;
    if (pParams->m_base.ExternalEdgeFlag[VERTICAL_DEBLOCKING])
    {
        if (nFieldMacroblockMode !=
            (Ipp32u) pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr - 2))
        {
            // when left macroblocks aren't intra blocks
            // or have different QP
            if (IS_INTER_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->nLeft[0])->mbtype) ||
                IS_INTER_MBTYPE((core_enc->m_pCurrentFrame->m_mbinfo.mbs + pParams->nLeft[1])->mbtype) ||
                (core_enc->m_mbinfo.mbs[pParams->nLeft[0]].QP != core_enc->m_mbinfo.mbs[pParams->nLeft[1]].QP))
            {
                pParams->m_base.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = 0;
                pParams->UseComplexVerticalDeblocking = 1;
            }
        }
    }

    // obtain extra horizontal edge condition
    // when above macroblock is field coded and
    // current macroblock is frame coded
    pParams->ExtraHorizontalEdge = 0;
    if ((0 == (pParams->m_base.nMBAddr & 1)) &&
        (pParams->m_base.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING]))
    {
        if ((0 == nFieldMacroblockMode) &&
            (0 != pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr - core_enc->m_WidthInMBs * 2)))
            pParams->ExtraHorizontalEdge = 1;
    }

    // reset external edges strength
    SetEdgeStrength(pParams->m_base.Strength[VERTICAL_DEBLOCKING], 0);
    SetEdgeStrength(pParams->m_base.Strength[HORIZONTAL_DEBLOCKING], 0);

    // set neighbour addreses
    pParams->m_base.nNeighbour[VERTICAL_DEBLOCKING] = MBAddr - 2;
    if (0 == nFieldMacroblockMode)
    {
        if ((MBAddr&1)==0)
        {
            if (pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr - core_enc->m_WidthInMBs * 2))
                pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - core_enc->m_WidthInMBs * 2;
            else
                pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - core_enc->m_WidthInMBs * 2 + 1;
        }
        else
        {
            pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - 1;
        }
    } else {
        if ((MBAddr&1)==0)
        {
            if (pGetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs + MBAddr - core_enc->m_WidthInMBs * 2))
                pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - core_enc->m_WidthInMBs * 2;
            else
                pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - core_enc->m_WidthInMBs * 2 + 1;
        }
        else
        {
            pParams->m_base.nNeighbour[HORIZONTAL_DEBLOCKING] = MBAddr - core_enc->m_WidthInMBs * 2;
        }
    }

    // set deblocking flag(s)
    pParams->m_base.DeblockingFlag[VERTICAL_DEBLOCKING] = 0;
    pParams->m_base.DeblockingFlag[HORIZONTAL_DEBLOCKING] = 0;

    // save variables
    pParams->m_base.pY = pY;
    pParams->m_base.pU = pU;
    pParams->m_base.pV = pV;
    pParams->m_base.pitchPixels = pic_pitchPixels;
    pParams->m_base.nMBAddr = MBAddr;
    pParams->m_base.nMaxMVector = (nFieldMacroblockMode) ? (2) : (4);
    pParams->m_base.MBFieldCoded = nFieldMacroblockMode;

    // set slice's variables
    pParams->m_base.nAlphaC0Offset = curr_slice->m_slice_alpha_c0_offset;
    pParams->m_base.nBetaOffset = curr_slice->m_slice_beta_offset;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockISliceMBAFF(
    void* state,
    Ipp32u MBAddr)
{
    __align(16)
    DeblockingParametersMBAFF<PIXTYPE> params;

    // prepare deblocking parameters
    params.m_base.nMBAddr = MBAddr;
    H264CoreEncoder_ResetDeblockingVariablesMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    H264CoreEncoder_PrepareDeblockingParametersISliceMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform luma deblocking
    if (0 == params.UseComplexVerticalDeblocking)
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockLumaVerticalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    if (0 == params.ExtraHorizontalEdge)
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockLumaHorizontalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform chroma deblocking
    if (0 == params.UseComplexVerticalDeblocking)
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockChromaVerticalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    if (0 == params.ExtraHorizontalEdge)
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockChromaHorizontalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockPSliceMBAFF(void* state, Ipp32u MBAddr)
{
    __align(16)
    DeblockingParametersMBAFF<PIXTYPE> params;

    // prepare deblocking parameters
    params.m_base.nMBAddr = MBAddr;
    H264CoreEncoder_ResetDeblockingVariablesMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    H264CoreEncoder_PrepareDeblockingParametersPSliceMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform luma deblocking
    if (0 == params.UseComplexVerticalDeblocking)
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockLumaVerticalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    if (0 == params.ExtraHorizontalEdge)
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockLumaHorizontalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform chroma deblocking
    if (0 == params.UseComplexVerticalDeblocking)
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockChromaVerticalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    if (0 == params.ExtraHorizontalEdge)
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockChromaHorizontalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_DeblockMacroblockBSliceMBAFF(void* state, Ipp32u MBAddr)
{
    __align(16)
    DeblockingParametersMBAFF<PIXTYPE> params;

    // prepare deblocking parameters
    params.m_base.nMBAddr = MBAddr;
    H264CoreEncoder_ResetDeblockingVariablesMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    H264CoreEncoder_PrepareDeblockingParametersBSliceMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform luma deblocking
    if (0 == params.UseComplexVerticalDeblocking)
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockLumaVerticalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    if (0 == params.ExtraHorizontalEdge)
        H264CoreEncoder_DeblockLuma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockLumaHorizontalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

    // perform chroma deblocking
    if (0 == params.UseComplexVerticalDeblocking)
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, VERTICAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockChromaVerticalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);
    if (0 == params.ExtraHorizontalEdge)
        H264CoreEncoder_DeblockChroma<COEFFSTYPE, PIXTYPE>(state, HORIZONTAL_DEBLOCKING, &params.m_base);
    else
        H264CoreEncoder_DeblockChromaHorizontalMBAFF<COEFFSTYPE, PIXTYPE>(state, &params);

}

// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template void H264CoreEncoder_DeblockMacroblockBSliceMBAFF<COEFFSTYPE, PIXTYPE>(void*, Ipp32u);
template void H264CoreEncoder_DeblockMacroblockPSliceMBAFF<COEFFSTYPE, PIXTYPE>(void*, Ipp32u);
template void H264CoreEncoder_DeblockMacroblockISliceMBAFF<COEFFSTYPE, PIXTYPE>(void*, Ipp32u);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template void H264CoreEncoder_DeblockMacroblockBSliceMBAFF<COEFFSTYPE, PIXTYPE>(void*, Ipp32u);
template void H264CoreEncoder_DeblockMacroblockPSliceMBAFF<COEFFSTYPE, PIXTYPE>(void*, Ipp32u);
template void H264CoreEncoder_DeblockMacroblockISliceMBAFF<COEFFSTYPE, PIXTYPE>(void*, Ipp32u);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER

