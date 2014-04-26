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
#if defined (UMC_ENABLE_H264_VIDEO_DECODER)

#include "umc_h264_segment_decoder.h"
#include "umc_h264_dec_deblocking.h"
#include "umc_h264_frame_info.h"


namespace UMC
{
// initialize const array
H264SegmentDecoder::ChromaDeblockingFunction H264SegmentDecoder::DeblockChroma[4] = 
{
    &H264SegmentDecoder::DeblockChroma400,
    &H264SegmentDecoder::DeblockChroma420,
    &H264SegmentDecoder::DeblockChroma422,
    &H264SegmentDecoder::DeblockChroma444
};

H264SegmentDecoder::ChromaDeblockingFunctionMBAFF H264SegmentDecoder::DeblockChromaVerticalMBAFF[4] = 
{
    &H264SegmentDecoder::DeblockChromaVerticalMBAFF400,
    &H264SegmentDecoder::DeblockChromaVerticalMBAFF420,
    &H264SegmentDecoder::DeblockChromaVerticalMBAFF422,
    &H264SegmentDecoder::DeblockChromaVerticalMBAFF444
};

H264SegmentDecoder::ChromaDeblockingFunctionMBAFF H264SegmentDecoder::DeblockChromaHorizontalMBAFF[4] = 
{
    &H264SegmentDecoder::DeblockChromaHorizontalMBAFF400,
    &H264SegmentDecoder::DeblockChromaHorizontalMBAFF420,
    &H264SegmentDecoder::DeblockChromaHorizontalMBAFF422,
    &H264SegmentDecoder::DeblockChromaHorizontalMBAFF444
};

void H264SegmentDecoder::DeblockFrame(Ipp32s iFirstMB, Ipp32s iNumMBs)
{
    m_bFrameDeblocking = true;

    for (m_CurMBAddr = iFirstMB; m_CurMBAddr < iFirstMB + iNumMBs; m_CurMBAddr++)
    {
        DeblockMacroblockMSlice();
    }

} // void H264SegmentDecoder::DeblockFrame(Ipp32s uFirstMB, Ipp32s uNumMBs)

void H264SegmentDecoder::DeblockSegment(Ipp32s iFirstMB, Ipp32s nBorder)
{
    // no filtering edges of this slice
    if (DEBLOCK_FILTER_OFF == m_pSliceHeader->disable_deblocking_filter_idc)
        return;

    // set frame deblocking flag
    m_bFrameDeblocking = false;

    void (H264SegmentDecoder::*pDecFunc)();

    Ipp32s MBYAdjust = 0;
    if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec && m_field_index)
        MBYAdjust = mb_height/2;

    m_CurMBAddr = iFirstMB;

    // set initial macroblock coordinates
    m_CurMB_X = ((m_CurMBAddr >> (Ipp32s) m_isMBAFF) % mb_width);
    m_CurMB_Y = ((m_CurMBAddr >> (Ipp32s) m_isMBAFF) / mb_width) - MBYAdjust;
    m_CurMB_Y <<= (Ipp32s) m_isMBAFF;

    // check mbs info for possible data losses
    if(m_gmbinfo->mbs[m_CurMBAddr].slice_id < 0)
        return;

    InitDeblockingOnce();

    if (m_pSliceHeader->MbaffFrameFlag)
    {
        // select optimized deblocking function
        switch (m_pSliceHeader->slice_type)
        {
        case INTRASLICE:
            pDecFunc = &H264SegmentDecoder::DeblockMacroblockISliceMBAFF;
            break;

        case PREDSLICE:
            pDecFunc = &H264SegmentDecoder::DeblockMacroblockPSliceMBAFF;
            break;

        case BPREDSLICE:
        default:
            pDecFunc = &H264SegmentDecoder::DeblockMacroblockBSliceMBAFF;
            break;
        }
    }
    // non-MBAFF case
    else
    {
        // select optimized deblocking function
        switch (m_pSliceHeader->slice_type)
        {
        case INTRASLICE:
            pDecFunc = &H264SegmentDecoder::DeblockMacroblockISlice;
            break;

        case PREDSLICE:
            pDecFunc = &H264SegmentDecoder::DeblockMacroblockPSlice;
            break;

        case BPREDSLICE:
        default:
            pDecFunc = &H264SegmentDecoder::DeblockMacroblockBSlice;
            break;
        }
    }

    for (Ipp32s curMB = iFirstMB; curMB < nBorder; curMB += 1)
    {
        (this->*pDecFunc)();

        // set next MB coordinates
        if (0 == m_isMBAFF)
            m_CurMB_X += 1;
        else
        {
            m_CurMB_X += m_CurMBAddr & 1;
            m_CurMB_Y ^= 1;
        }
        // set next MB addres
        m_CurMBAddr += 1;
    }
} // void H264SegmentDecoder::DeblockSegment(Ipp32s iFirstMB, Ipp32s iNumMBs)

void H264SegmentDecoder::DeblockMacroblockMSlice()
{
    // check mbs info for possible data losses
    if(m_gmbinfo->mbs[m_CurMBAddr].slice_id < 0)
        return;

    H264Slice* pSlice = m_pCurrentFrame->GetAU(m_pCurrentFrame->GetNumberByParity(m_field_index))->GetSliceByNumber(m_gmbinfo->mbs[m_CurMBAddr].slice_id);

    // when deblocking isn't required
    if ((NULL == pSlice) ||
        (DEBLOCK_FILTER_OFF == pSlice->GetSliceHeader()->disable_deblocking_filter_idc))
        return;

    // select optimized deblocking function
    switch (pSlice->GetSliceHeader()->slice_type)
    {
    case INTRASLICE:
        DeblockMacroblockISlice();
        break;

    case PREDSLICE:
        DeblockMacroblockPSlice();
        break;

    case BPREDSLICE:
        DeblockMacroblockBSlice();
        break;

    default:
        // illegal case. it should never hapen.
        VM_ASSERT(false);
        break;
    }

} // void H264SegmentDecoder::DeblockMacroblockMSlice(Ipp32s m_CurMBAddr)

void H264SegmentDecoder::DeblockMacroblockISlice()
{
    // prepare deblocking parameters
    ResetDeblockingVariables();
    PrepareDeblockingParametersISlice();

    {
        Ipp32u color_format = m_pCurrentFrame->m_chroma_format;

        (this->*DeblockChroma[color_format])(VERTICAL_DEBLOCKING);
        (this->*DeblockChroma[color_format])(HORIZONTAL_DEBLOCKING);
    }

    // perform deblocking
    DeblockLuma(VERTICAL_DEBLOCKING);
    DeblockLuma(HORIZONTAL_DEBLOCKING);

} // void H264SegmentDecoder::DeblockMacroblockISlice()

void H264SegmentDecoder::DeblockMacroblockPSlice()
{
    if (!m_deblockingParams.m_isSameSlice)
    {
        DeblockMacroblockBSlice();
        return;
    }

    // prepare deblocking parameters
    ResetDeblockingVariables();
    PrepareDeblockingParametersPSlice();

    {
        Ipp32u color_format = m_pCurrentFrame->m_chroma_format;

        (this->*DeblockChroma[color_format])(VERTICAL_DEBLOCKING);
        (this->*DeblockChroma[color_format])(HORIZONTAL_DEBLOCKING);
    }

    // perform deblocking
    DeblockLuma(VERTICAL_DEBLOCKING);
    DeblockLuma(HORIZONTAL_DEBLOCKING);
} // void H264SegmentDecoder::DeblockMacroblockPSlice()

void H264SegmentDecoder::DeblockMacroblockBSlice()
{
    // prepare deblocking parameters
    ResetDeblockingVariables();
    PrepareDeblockingParametersBSlice();

    {
        Ipp32u color_format = m_pCurrentFrame->m_chroma_format;

        (this->*DeblockChroma[color_format])(VERTICAL_DEBLOCKING);
        (this->*DeblockChroma[color_format])(HORIZONTAL_DEBLOCKING);
    }

    // perform deblocking
    DeblockLuma(VERTICAL_DEBLOCKING);
    DeblockLuma(HORIZONTAL_DEBLOCKING);

} // void H264SegmentDecoder::DeblockMacroblockBSlice()

void H264SegmentDecoder::DeblockLuma(Ipp32u dir)
{
    if (!m_deblockingParams.DeblockingFlag[dir])
        return;

    Ipp32s AlphaC0Offset = m_deblockingParams.nAlphaC0Offset;
    Ipp32s BetaOffset = m_deblockingParams.nBetaOffset;
    Ipp32s pmq_QP = m_cur_mb.LocalMacroblockInfo->QP;

    m_deblockingParams.deblockInfo.pBs = m_deblockingParams.Strength[dir];
    m_deblockingParams.deblockInfo.srcDstStep = m_uPitchLuma;
    m_deblockingParams.deblockInfo.pSrcDstPlane = m_deblockingParams.pLuma;

    Ipp8u *pClipTab;
    Ipp32s QP;
    Ipp32s index;
    Ipp8u *pStrength = m_deblockingParams.Strength[dir];
    //
    // correct strengths for high profile
    //
    if (GetMB8x8TSFlag(*m_cur_mb.GlobalMacroblockInfo))
    {
        SetEdgeStrength(pStrength + 4, 0);
        SetEdgeStrength(pStrength + 12, 0);
    }

    if (m_deblockingParams.ExternalEdgeFlag[dir])
    {
        Ipp32s pmp_QP;

        // get neighbour block QP
        pmp_QP = m_mbinfo.mbs[m_deblockingParams.nNeighbour[dir]].QP;

        // luma variables
        QP = (pmp_QP + pmq_QP + 1) >> 1 ;

        // external edge variables
        index = IClip(0, 51, QP + BetaOffset);
        m_deblockingParams.deblockInfo.pBeta[0] = (Ipp8u) (BETA_TABLE[index]);

        index = IClip(0, 51, QP + AlphaC0Offset);
        m_deblockingParams.deblockInfo.pAlpha[0] = (Ipp8u) (ALPHA_TABLE[index]);

        pClipTab = CLIP_TAB[index];

        // create clipping values
        m_deblockingParams.deblockInfo.pThresholds[0] = (Ipp8u) (pClipTab[pStrength[0]]);
        m_deblockingParams.deblockInfo.pThresholds[1] = (Ipp8u) (pClipTab[pStrength[1]]);
        m_deblockingParams.deblockInfo.pThresholds[2] = (Ipp8u) (pClipTab[pStrength[2]]);
        m_deblockingParams.deblockInfo.pThresholds[3] = (Ipp8u) (pClipTab[pStrength[3]]);
    }

    // internal edge variables
    QP = pmq_QP;

    index = IClip(0, 51, QP + BetaOffset);
    m_deblockingParams.deblockInfo.pBeta[1] = (Ipp8u) (BETA_TABLE[index]);

    index = IClip(0, 51, QP + AlphaC0Offset);
    m_deblockingParams.deblockInfo.pAlpha[1] = (Ipp8u) (ALPHA_TABLE[index]);

    pClipTab = CLIP_TAB[index];

    // create clipping values
    {
        Ipp32s edge;

        for (edge = 1;edge < 4;edge += 1)
        {
            if (*((Ipp32u *) (pStrength + edge * 4)))
            {
                // create clipping values
                m_deblockingParams.deblockInfo.pThresholds[edge * 4 + 0] = (Ipp8u) (pClipTab[pStrength[edge * 4 + 0]]);
                m_deblockingParams.deblockInfo.pThresholds[edge * 4 + 1] = (Ipp8u) (pClipTab[pStrength[edge * 4 + 1]]);
                m_deblockingParams.deblockInfo.pThresholds[edge * 4 + 2] = (Ipp8u) (pClipTab[pStrength[edge * 4 + 2]]);
                m_deblockingParams.deblockInfo.pThresholds[edge * 4 + 3] = (Ipp8u) (pClipTab[pStrength[edge * 4 + 3]]);
            }
        }
    }

    if (bit_depth_luma > 8)
    {
        IppDeblocking16u[dir]((Ipp16u*)m_deblockingParams.pLuma,
                              m_uPitchLuma,
                              m_deblockingParams.deblockInfo.pAlpha,
                              m_deblockingParams.deblockInfo.pBeta,
                              m_deblockingParams.deblockInfo.pThresholds,
                              pStrength,
                              bit_depth_luma);
    }
    else
    {
        IppDeblocking[dir](&m_deblockingParams.deblockInfo);
    }
} // void H264SegmentDecoder::DeblockLuma(Ipp32u dir)

void H264SegmentDecoder::DeblockChroma400(Ipp32u)
{
    // there is nothing to deblock

} // void H264SegmentDecoder::DeblockChroma400(Ipp32u)

void H264SegmentDecoder::DeblockChroma420(Ipp32u dir)
{
    if (!m_deblockingParams.DeblockingFlag[dir])
        return;

    bool nv12_support = m_pCurrentFrame->GetColorFormat() == NV12;

    Ipp32s AlphaC0Offset = m_deblockingParams.nAlphaC0Offset;
    Ipp32s BetaOffset = m_deblockingParams.nBetaOffset;
    Ipp32s pmq_QP = m_cur_mb.LocalMacroblockInfo->QP;
    Ipp8u *pClipTab;
    Ipp32s QP;
    Ipp32s index;
    Ipp8u *pStrength = m_deblockingParams.Strength[dir];
    Ipp32s nPlane;
    Ipp32s chroma_qp_offset = ~(m_pPicParamSet->chroma_qp_index_offset[0]);

    m_deblockingParams.deblockInfo.pBs = m_deblockingParams.Strength[dir];
    m_deblockingParams.deblockInfo.srcDstStep = m_uPitchChroma;
    m_deblockingParams.deblockInfo.pSrcDstPlane = m_deblockingParams.pChroma[0];

    if (nv12_support)
    {
        for (nPlane = 0; nPlane < 2; nPlane += 1)
        {
            if (chroma_qp_offset != m_pPicParamSet->chroma_qp_index_offset[nPlane])
            {
                chroma_qp_offset = m_pPicParamSet->chroma_qp_index_offset[nPlane];

                if (m_deblockingParams.ExternalEdgeFlag[dir])
                {
                    Ipp32s pmp_QP;

                    // get left block QP
                    pmp_QP = m_mbinfo.mbs[m_deblockingParams.nNeighbour[dir]].QP;

                    // external edge variables
                    QP = (QP_SCALE_CR[IClip(0, 51, pmp_QP + chroma_qp_offset)] +
                          QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)] + 1) >> 1;

                    index = IClip(0, 51, QP + BetaOffset);
                    m_deblockingParams.deblockInfo.pBeta[0 + 2*nPlane] = (Ipp8u) (BETA_TABLE[index]);

                    index = IClip(0, 51, QP + AlphaC0Offset);
                    m_deblockingParams.deblockInfo.pAlpha[0 + 2*nPlane] = (Ipp8u) (ALPHA_TABLE[index]);

                    pClipTab = CLIP_TAB[index];

                    // create clipping values
                    m_deblockingParams.deblockInfo.pThresholds[0 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[0]]);
                    m_deblockingParams.deblockInfo.pThresholds[1 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[1]]);
                    m_deblockingParams.deblockInfo.pThresholds[2 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[2]]);
                    m_deblockingParams.deblockInfo.pThresholds[3 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[3]]);
                }

                // create clipping values
                if (*((Ipp32u *) (pStrength + 8)))
                {
                    // internal edge variables
                    QP = QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];

                    index = IClip(0, 51, QP + BetaOffset);
                    m_deblockingParams.deblockInfo.pBeta[1 + 2*nPlane] = (Ipp8u) (BETA_TABLE[index]);

                    index = IClip(0, 51, QP + AlphaC0Offset);
                    m_deblockingParams.deblockInfo.pAlpha[1 + 2*nPlane] = (Ipp8u) (ALPHA_TABLE[index]);

                    pClipTab = CLIP_TAB[index];

                    m_deblockingParams.deblockInfo.pThresholds[4 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[8]]);
                    m_deblockingParams.deblockInfo.pThresholds[5 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[9]]);
                    m_deblockingParams.deblockInfo.pThresholds[6 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[10]]);
                    m_deblockingParams.deblockInfo.pThresholds[7 + 8*nPlane] = (Ipp8u) (pClipTab[pStrength[11]]);
                }
            }
            else
            {
                if (*((Ipp32u *) (pStrength)))
                {
                    ippsCopy_8u(&m_deblockingParams.deblockInfo.pThresholds[0], &m_deblockingParams.deblockInfo.pThresholds[0 + 8],
                        4 * sizeof(m_deblockingParams.deblockInfo.pThresholds[0]));
                    m_deblockingParams.deblockInfo.pAlpha[2] = m_deblockingParams.deblockInfo.pAlpha[0];
                    m_deblockingParams.deblockInfo.pBeta[2] = m_deblockingParams.deblockInfo.pBeta[0];
                }

                if (*((Ipp32u *) (pStrength + 8)))
                {
                    ippsCopy_8u(&m_deblockingParams.deblockInfo.pThresholds[4], &m_deblockingParams.deblockInfo.pThresholds[0 + 12],
                        4 * sizeof(m_deblockingParams.deblockInfo.pThresholds[0]));
                    m_deblockingParams.deblockInfo.pAlpha[1 + 2] = m_deblockingParams.deblockInfo.pAlpha[1];
                    m_deblockingParams.deblockInfo.pBeta[1 + 2] = m_deblockingParams.deblockInfo.pBeta[1];
                }
            }
        } // for plane

        IppDeblocking[10 + dir](&m_deblockingParams.deblockInfo);
    }
    else
    {
        for (nPlane = 0; nPlane < 2; nPlane += 1)
        {
            if (chroma_qp_offset != m_pPicParamSet->chroma_qp_index_offset[nPlane])
            {
                chroma_qp_offset = m_pPicParamSet->chroma_qp_index_offset[nPlane];

                if (m_deblockingParams.ExternalEdgeFlag[dir])
                {
                    Ipp32s pmp_QP;

                    // get left block QP
                    pmp_QP = m_mbinfo.mbs[m_deblockingParams.nNeighbour[dir]].QP;

                    // external edge variables
                    QP = (QP_SCALE_CR[IClip(0, 51, pmp_QP + chroma_qp_offset)] +
                          QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)] + 1) >> 1;

                    index = IClip(0, 51, QP + BetaOffset);
                    m_deblockingParams.deblockInfo.pBeta[0] = (Ipp8u) (BETA_TABLE[index]);

                    index = IClip(0, 51, QP + AlphaC0Offset);
                    m_deblockingParams.deblockInfo.pAlpha[0] = (Ipp8u) (ALPHA_TABLE[index]);
                    pClipTab = CLIP_TAB[index];

                    // create clipping values
                    m_deblockingParams.deblockInfo.pThresholds[0] = (Ipp8u) (pClipTab[pStrength[0]]);
                    m_deblockingParams.deblockInfo.pThresholds[1] = (Ipp8u) (pClipTab[pStrength[1]]);
                    m_deblockingParams.deblockInfo.pThresholds[2] = (Ipp8u) (pClipTab[pStrength[2]]);
                    m_deblockingParams.deblockInfo.pThresholds[3] = (Ipp8u) (pClipTab[pStrength[3]]);

                }

                // internal edge variables
                QP = QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];

                index = IClip(0, 51, QP + BetaOffset);
                m_deblockingParams.deblockInfo.pBeta[1] = (Ipp8u) (BETA_TABLE[index]);

                index = IClip(0, 51, QP + AlphaC0Offset);
                m_deblockingParams.deblockInfo.pAlpha[1] = (Ipp8u) (ALPHA_TABLE[index]);
                pClipTab = CLIP_TAB[index];

                // create clipping values
                m_deblockingParams.deblockInfo.pThresholds[4] = (Ipp8u) (pClipTab[pStrength[8]]);
                m_deblockingParams.deblockInfo.pThresholds[5] = (Ipp8u) (pClipTab[pStrength[9]]);
                m_deblockingParams.deblockInfo.pThresholds[6] = (Ipp8u) (pClipTab[pStrength[10]]);
                m_deblockingParams.deblockInfo.pThresholds[7] = (Ipp8u) (pClipTab[pStrength[11]]);
            }

            m_deblockingParams.deblockInfo.pSrcDstPlane = m_deblockingParams.pChroma[nPlane];

            if (bit_depth_chroma > 8)
            {
                IppDeblocking16u[2 + dir]((Ipp16u*)m_deblockingParams.pChroma[nPlane],
                                          m_deblockingParams.deblockInfo.srcDstStep,
                                          m_deblockingParams.deblockInfo.pAlpha,
                                          m_deblockingParams.deblockInfo.pBeta,
                                          m_deblockingParams.deblockInfo.pThresholds,
                                          m_deblockingParams.deblockInfo.pBs,
                                          bit_depth_chroma);
            }
            else
            {
                IppDeblocking[2 + dir](&m_deblockingParams.deblockInfo);
            }
        } // for plane
    }

} // void H264SegmentDecoder::DeblockChroma420(Ipp32u dir, DeblockingParameters *pParams)

void H264SegmentDecoder::DeblockChroma422(Ipp32u dir)
{
    if (m_deblockingParams.DeblockingFlag[dir])
    {
        Ipp32s pic_pitch = m_uPitchChroma;
        Ipp32s AlphaC0Offset = m_deblockingParams.nAlphaC0Offset;
        Ipp32s BetaOffset = m_deblockingParams.nBetaOffset;
        Ipp32s pmq_QP = m_cur_mb.LocalMacroblockInfo->QP;
        Ipp8u *pClipTab;
        Ipp32s QP;
        Ipp32s index;
        Ipp8u *pStrength = m_deblockingParams.Strength[dir];
        Ipp32s nPlane;
        Ipp32s chroma_qp_offset = ~(m_pPicParamSet->chroma_qp_index_offset[0]);

        m_deblockingParams.deblockInfo.pBs = m_deblockingParams.Strength[dir];
        m_deblockingParams.deblockInfo.srcDstStep = pic_pitch;

        for (nPlane = 0; nPlane < 2; nPlane += 1)
        {
            if (chroma_qp_offset != m_pPicParamSet->chroma_qp_index_offset[nPlane])
            {
                chroma_qp_offset = m_pPicParamSet->chroma_qp_index_offset[nPlane];

                if (m_deblockingParams.ExternalEdgeFlag[dir])
                {
                    Ipp32s pmp_QP;

                    // get left block QP
                    pmp_QP = m_mbinfo.mbs[m_deblockingParams.nNeighbour[dir]].QP;

                    // external edge variables
                    QP = (QP_SCALE_CR[IClip(0, 51, pmp_QP + chroma_qp_offset)] +
                          QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)] + 1) >> 1;

                    index = IClip(0, 51, QP + BetaOffset);
                    m_deblockingParams.deblockInfo.pBeta[0] = (Ipp8u) (BETA_TABLE[index]);

                    index = IClip(0, 51, QP + AlphaC0Offset);
                    m_deblockingParams.deblockInfo.pAlpha[0] = (Ipp8u) (ALPHA_TABLE[index]);
                    pClipTab = CLIP_TAB[index];

                    // create clipping values
                    m_deblockingParams.deblockInfo.pThresholds[0] = (Ipp8u) (pClipTab[pStrength[0]]);
                    m_deblockingParams.deblockInfo.pThresholds[1] = (Ipp8u) (pClipTab[pStrength[1]]);
                    m_deblockingParams.deblockInfo.pThresholds[2] = (Ipp8u) (pClipTab[pStrength[2]]);
                    m_deblockingParams.deblockInfo.pThresholds[3] = (Ipp8u) (pClipTab[pStrength[3]]);

                }

                // internal edge variables
                QP = QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];

                index = IClip(0, 51, QP + BetaOffset);
                m_deblockingParams.deblockInfo.pBeta[1] = (Ipp8u) (BETA_TABLE[index]);

                index = IClip(0, 51, QP + AlphaC0Offset);
                m_deblockingParams.deblockInfo.pAlpha[1] = (Ipp8u) (ALPHA_TABLE[index]);
                pClipTab = CLIP_TAB[index];

                if (HORIZONTAL_DEBLOCKING == dir)
                {
                    // create clipping values
                    m_deblockingParams.deblockInfo.pThresholds[4] = (Ipp8u) (pClipTab[pStrength[4]]);
                    m_deblockingParams.deblockInfo.pThresholds[5] = (Ipp8u) (pClipTab[pStrength[5]]);
                    m_deblockingParams.deblockInfo.pThresholds[6] = (Ipp8u) (pClipTab[pStrength[6]]);
                    m_deblockingParams.deblockInfo.pThresholds[7] = (Ipp8u) (pClipTab[pStrength[7]]);
                    m_deblockingParams.deblockInfo.pThresholds[8] = (Ipp8u) (pClipTab[pStrength[8]]);
                    m_deblockingParams.deblockInfo.pThresholds[9] = (Ipp8u) (pClipTab[pStrength[9]]);
                    m_deblockingParams.deblockInfo.pThresholds[10] = (Ipp8u) (pClipTab[pStrength[10]]);
                    m_deblockingParams.deblockInfo.pThresholds[11] = (Ipp8u) (pClipTab[pStrength[11]]);
                    m_deblockingParams.deblockInfo.pThresholds[12] = (Ipp8u) (pClipTab[pStrength[12]]);
                    m_deblockingParams.deblockInfo.pThresholds[13] = (Ipp8u) (pClipTab[pStrength[13]]);
                    m_deblockingParams.deblockInfo.pThresholds[14] = (Ipp8u) (pClipTab[pStrength[14]]);
                    m_deblockingParams.deblockInfo.pThresholds[15] = (Ipp8u) (pClipTab[pStrength[15]]);
                }
                else
                {
                    // create clipping values
                    m_deblockingParams.deblockInfo.pThresholds[4] = (Ipp8u) (pClipTab[pStrength[8]]);
                    m_deblockingParams.deblockInfo.pThresholds[5] = (Ipp8u) (pClipTab[pStrength[9]]);
                    m_deblockingParams.deblockInfo.pThresholds[6] = (Ipp8u) (pClipTab[pStrength[10]]);
                    m_deblockingParams.deblockInfo.pThresholds[7] = (Ipp8u) (pClipTab[pStrength[11]]);
                }
            }

            m_deblockingParams.deblockInfo.pSrcDstPlane = m_deblockingParams.pChroma[nPlane];

            if (bit_depth_chroma > 8)
            {
                IppDeblocking16u[4 + dir]((Ipp16u*)m_deblockingParams.pChroma[nPlane],
                                          pic_pitch,
                                          m_deblockingParams.deblockInfo.pAlpha,
                                          m_deblockingParams.deblockInfo.pBeta,
                                          m_deblockingParams.deblockInfo.pThresholds,
                                          m_deblockingParams.deblockInfo.pBs,
                                          bit_depth_chroma);
            }
            else
            {
                IppDeblocking[4 + dir](&m_deblockingParams.deblockInfo);
            }

        }
    }

} // void H264SegmentDecoder::DeblockChroma422(Ipp32u dir)

void H264SegmentDecoder::DeblockChroma444(Ipp32u dir)
{
    if (m_deblockingParams.DeblockingFlag[dir])
    {
        Ipp32s pic_pitch = m_uPitchChroma;
        Ipp32s AlphaC0Offset = m_deblockingParams.nAlphaC0Offset;
        Ipp32s BetaOffset = m_deblockingParams.nBetaOffset;
        Ipp32s pmq_QP = m_cur_mb.LocalMacroblockInfo->QP;
        Ipp8u *pClipTab;
        Ipp32s QP;
        Ipp32s index;
        Ipp8u *pStrength = m_deblockingParams.Strength[dir];
        Ipp32s nPlane;
        Ipp32s chroma_qp_offset = ~(m_pPicParamSet->chroma_qp_index_offset[0]);

        m_deblockingParams.deblockInfo.pBs = m_deblockingParams.Strength[dir];
        m_deblockingParams.deblockInfo.srcDstStep = pic_pitch;

        for (nPlane = 0; nPlane < 2; nPlane += 1)
        {
            if (chroma_qp_offset != m_pPicParamSet->chroma_qp_index_offset[nPlane])
            {
                chroma_qp_offset = m_pPicParamSet->chroma_qp_index_offset[nPlane];

                if (m_deblockingParams.ExternalEdgeFlag[dir])
                {
                    Ipp32s pmp_QP;

                    // get left block QP
                    pmp_QP = m_mbinfo.mbs[m_deblockingParams.nNeighbour[dir]].QP;

                    // external edge variables
                    QP = (QP_SCALE_CR[IClip(0, 51, pmp_QP + chroma_qp_offset)] +
                          QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)] + 1) >> 1;

                    index = IClip(0, 51, QP + BetaOffset);
                    m_deblockingParams.deblockInfo.pBeta[0] = (Ipp8u) (BETA_TABLE[index]);

                    index = IClip(0, 51, QP + AlphaC0Offset);
                    m_deblockingParams.deblockInfo.pAlpha[0] = (Ipp8u) (ALPHA_TABLE[index]);
                    pClipTab = CLIP_TAB[index];

                    // create clipping values
                    m_deblockingParams.deblockInfo.pThresholds[0] = (Ipp8u) (pClipTab[pStrength[0]]);
                    m_deblockingParams.deblockInfo.pThresholds[1] = (Ipp8u) (pClipTab[pStrength[1]]);
                    m_deblockingParams.deblockInfo.pThresholds[2] = (Ipp8u) (pClipTab[pStrength[2]]);
                    m_deblockingParams.deblockInfo.pThresholds[3] = (Ipp8u) (pClipTab[pStrength[3]]);
                }

                // internal edge variables
                QP = QP_SCALE_CR[IClip(0, 51, pmq_QP + chroma_qp_offset)];

                index = IClip(0, 51, QP + BetaOffset);
                m_deblockingParams.deblockInfo.pBeta[1] = (Ipp8u) (BETA_TABLE[index]);

                index = IClip(0, 51, QP + AlphaC0Offset);
                m_deblockingParams.deblockInfo.pAlpha[1] = (Ipp8u) (ALPHA_TABLE[index]);
                pClipTab = CLIP_TAB[index];

                // create clipping values
                m_deblockingParams.deblockInfo.pThresholds[4] = (Ipp8u) (pClipTab[pStrength[4]]);
                m_deblockingParams.deblockInfo.pThresholds[5] = (Ipp8u) (pClipTab[pStrength[5]]);
                m_deblockingParams.deblockInfo.pThresholds[6] = (Ipp8u) (pClipTab[pStrength[6]]);
                m_deblockingParams.deblockInfo.pThresholds[7] = (Ipp8u) (pClipTab[pStrength[7]]);
                m_deblockingParams.deblockInfo.pThresholds[8] = (Ipp8u) (pClipTab[pStrength[8]]);
                m_deblockingParams.deblockInfo.pThresholds[9] = (Ipp8u) (pClipTab[pStrength[9]]);
                m_deblockingParams.deblockInfo.pThresholds[10] = (Ipp8u) (pClipTab[pStrength[10]]);
                m_deblockingParams.deblockInfo.pThresholds[11] = (Ipp8u) (pClipTab[pStrength[11]]);
                m_deblockingParams.deblockInfo.pThresholds[12] = (Ipp8u) (pClipTab[pStrength[12]]);
                m_deblockingParams.deblockInfo.pThresholds[13] = (Ipp8u) (pClipTab[pStrength[13]]);
                m_deblockingParams.deblockInfo.pThresholds[14] = (Ipp8u) (pClipTab[pStrength[14]]);
                m_deblockingParams.deblockInfo.pThresholds[15] = (Ipp8u) (pClipTab[pStrength[15]]);
            }

            m_deblockingParams.deblockInfo.pSrcDstPlane = m_deblockingParams.pChroma[nPlane];

            if (bit_depth_chroma > 8)
            {
                IppDeblocking16u[6 + dir]((Ipp16u*)m_deblockingParams.pChroma[nPlane],
                                          pic_pitch,
                                          m_deblockingParams.deblockInfo.pAlpha,
                                          m_deblockingParams.deblockInfo.pBeta,
                                          m_deblockingParams.deblockInfo.pThresholds,
                                          pStrength,
                                          bit_depth_chroma);
            }
            else
            {
                IppDeblocking[6 + dir](&m_deblockingParams.deblockInfo);
            }
        }
    }

} // void H264SegmentDecoder::DeblockChroma444(Ipp32u dir)

void H264SegmentDecoder::InitDeblockingOnce()
{
    // load slice header
    const H264SliceHeader *pHeader = (m_bFrameDeblocking) ?
              (m_pCurrentFrame->GetAU(m_pCurrentFrame->GetNumberByParity(m_field_index))->GetSliceByNumber(m_gmbinfo->mbs[m_CurMBAddr].slice_id)->GetSliceHeader()) :
              (m_pSliceHeader);

    m_deblockingParams.nMaxMVector = (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec) ? (2) : (4);
    m_deblockingParams.MBFieldCoded = (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec);

    // set slice's variables
    m_deblockingParams.nAlphaC0Offset = pHeader->slice_alpha_c0_offset;
    m_deblockingParams.nBetaOffset = pHeader->slice_beta_offset;

    m_pRefPicList[0] = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[m_CurMBAddr].slice_id, 0)->m_RefPicList;
    m_pRefPicList[1] = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[m_CurMBAddr].slice_id, 1)->m_RefPicList;
    m_pFields[0] = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[m_CurMBAddr].slice_id, 0)->m_Flags;
    m_pFields[1] = m_pCurrentFrame->GetRefPicList(m_gmbinfo->mbs[m_CurMBAddr].slice_id, 1)->m_Flags;

    m_pSliceHeader = pHeader;

    m_deblockingParams.deblockInfo.pAlpha = m_deblockingParams.Alpha;
    m_deblockingParams.deblockInfo.pBeta = m_deblockingParams.Beta;
    m_deblockingParams.deblockInfo.pThresholds = m_deblockingParams.Clipping;
    m_deblockingParams.deblockInfo.srcDstStep = m_uPitchChroma;

    m_cur_mb.isInited = false;
    m_cur_mb.GlobalMacroblockInfo = &m_gmbinfo->mbs[m_CurMBAddr];
    m_cur_mb.LocalMacroblockInfo = &m_mbinfo.mbs[m_CurMBAddr];
    m_cur_mb.MVs[0] = &m_gmbinfo->MV[0][m_CurMBAddr];
    m_cur_mb.MVs[1] = &m_gmbinfo->MV[1][m_CurMBAddr];
    m_cur_mb.RefIdxs[0] = &m_gmbinfo->mbs[m_CurMBAddr].refIdxs[0];
    m_cur_mb.RefIdxs[1] = &m_gmbinfo->mbs[m_CurMBAddr].refIdxs[1];

    InitDeblockingSliceBoundaryInfo();
}

void H264SegmentDecoder::InitDeblockingSliceBoundaryInfo()
{
    m_deblockingParams.m_isSameSlice = m_isSliceGroups ? 0 : 1;

    if (!m_isSliceGroups && DEBLOCK_FILTER_ON_NO_SLICE_EDGES != m_pSliceHeader->disable_deblocking_filter_idc && m_pSliceHeader->slice_type != INTRASLICE)
    {
        m_deblockingParams.m_isSameSlice = 0;

        Ipp32s MBYAdjust = 0;
        if (FRM_STRUCTURE > m_pCurrentFrame->m_PictureStructureForDec && m_field_index)
            MBYAdjust = mb_height/2;

        Ipp32s prevRow = m_CurMB_Y ? (m_CurMB_Y  + MBYAdjust - 1)*mb_width : MBYAdjust*mb_width;

        if (m_isMBAFF)
            prevRow = m_CurMB_Y > 3 ? (m_CurMB_Y  - 3)*mb_width : 0;

        if (m_iSliceNumber == m_gmbinfo->mbs[prevRow].slice_id)
        {
            m_deblockingParams.m_isSameSlice = 1;
        }
        else
        {
            H264DecoderFrameInfo * info = m_pCurrentFrame->GetAU(m_pCurrentFrame->GetNumberByParity(m_field_index));
            m_deblockingParams.m_isSameSlice = (info->m_isBExist && info->m_isPExist) ? 0 : 1;
        }
    }
}

void H264SegmentDecoder::ResetDeblockingVariables()
{
    if (m_isSliceGroups)
    {
        Ipp32s MBYAdjust = 0;
        if (m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE && m_field_index)
            MBYAdjust = mb_height/2;

        m_CurMB_X = (m_CurMBAddr % mb_width);
        m_CurMB_Y = (m_CurMBAddr / mb_width) - MBYAdjust;

        InitDeblockingOnce();
    }
    else
    {
        if (m_cur_mb.isInited)
        {
            m_cur_mb.GlobalMacroblockInfo++;
            m_cur_mb.LocalMacroblockInfo++;
            m_cur_mb.MVs[0]++;
            m_cur_mb.MVs[1]++;
            m_cur_mb.RefIdxs[0] = &m_cur_mb.GlobalMacroblockInfo->refIdxs[0];
            m_cur_mb.RefIdxs[1] = &m_cur_mb.GlobalMacroblockInfo->refIdxs[1];
        }

        m_cur_mb.isInited = true;
    }

    bool nv12_support = (m_pCurrentFrame->GetColorFormat() == NV12);

    Ipp32s pixel_luma_sz    = bit_depth_luma > 8 ? 2 : 1;
    Ipp32s pixel_chroma_sz  = bit_depth_chroma > 8 ? 2 : 1;

    // prepare macroblock variables
    Ipp32s mbXOffset = m_CurMB_X * 16;
    Ipp32s mbYOffset = m_CurMB_Y * 16;

    Ipp32s color_format = m_pCurrentFrame->m_chroma_format;
    Ipp32u offsetY = mbXOffset + (mbYOffset * m_uPitchLuma);
    Ipp32u offsetC = (mbXOffset >> (Ipp32s)(nv12_support ? 0 : (color_format < 3))) +  ((mbYOffset >> (Ipp32s)(color_format <= 1)) * m_uPitchChroma);

    // set external edge variables
    m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = (m_CurMB_X != 0);
    m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = (m_CurMB_Y != 0);

    if (DEBLOCK_FILTER_ON_NO_SLICE_EDGES == m_pSliceHeader->disable_deblocking_filter_idc)
    {
        // don't filter at slice boundaries
        if (m_CurMB_X)
        {
            if (m_gmbinfo->mbs[m_CurMBAddr].slice_id !=
                m_gmbinfo->mbs[m_CurMBAddr - 1].slice_id)
                m_deblockingParams.ExternalEdgeFlag[VERTICAL_DEBLOCKING] = 0;
        }

        if (m_CurMB_Y)
        {
            if (m_gmbinfo->mbs[m_CurMBAddr].slice_id !=
                m_gmbinfo->mbs[m_CurMBAddr - mb_width].slice_id)
                m_deblockingParams.ExternalEdgeFlag[HORIZONTAL_DEBLOCKING] = 0;
        }
    }

    // reset external edges strength
    SetEdgeStrength(m_deblockingParams.Strength[VERTICAL_DEBLOCKING], 0);
    SetEdgeStrength(m_deblockingParams.Strength[HORIZONTAL_DEBLOCKING], 0);

    // set neighbour addreses
    m_deblockingParams.nNeighbour[VERTICAL_DEBLOCKING] = m_CurMBAddr - 1;
    m_deblockingParams.nNeighbour[HORIZONTAL_DEBLOCKING] = m_CurMBAddr - mb_width;

    // set deblocking flag(s)
    m_deblockingParams.DeblockingFlag[VERTICAL_DEBLOCKING] = 0;
    m_deblockingParams.DeblockingFlag[HORIZONTAL_DEBLOCKING] = 0;

    // save variables
    m_deblockingParams.pLuma = m_pYPlane + offsetY*pixel_luma_sz;
    if (nv12_support)
    {
        m_deblockingParams.pChroma[0] = m_pUVPlane + offsetC*pixel_chroma_sz;
    }
    else
    {
        m_deblockingParams.pChroma[0] = m_pUPlane + offsetC*pixel_chroma_sz;
        m_deblockingParams.pChroma[1] = m_pVPlane + offsetC*pixel_chroma_sz;
    }

} // void H264SegmentDecoder::ResetDeblockingVariables(DeblockingParameters *pParams)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
