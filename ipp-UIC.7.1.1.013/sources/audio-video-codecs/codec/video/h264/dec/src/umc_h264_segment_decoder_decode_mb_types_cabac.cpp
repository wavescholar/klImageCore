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
#include "umc_h264_video_decoder.h"
#include "umc_h264_bitstream_inlines.h"
#include "umc_h264_dec_internal_cabac.h"

namespace UMC
{

void H264SegmentDecoder::DecodeMBFieldDecodingFlag_CABAC(void)
{
    Ipp32s condTermFlagA = 0, condTermFlagB = 0;
    Ipp32s mbAddr, ctxIdxInc;

    // get left MB pair info
    mbAddr = m_CurMBAddr - 2;
    if ((m_pSliceHeader->first_mb_in_slice <= mbAddr) &&
        (m_CurMB_X))
        condTermFlagA = GetMBFieldDecodingFlag(m_gmbinfo->mbs[mbAddr]);

    // get above MB pair info
    mbAddr = m_CurMBAddr - mb_width * 2;
    if ((m_pSliceHeader->first_mb_in_slice <= mbAddr) &&
        (m_CurMB_Y))
        condTermFlagB = GetMBFieldDecodingFlag(m_gmbinfo->mbs[mbAddr]);

    // decode flag
    ctxIdxInc = condTermFlagA + condTermFlagB;
    {
        Ipp32s binVal = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_FIELD_DECODING_FLAG] +
                                                            ctxIdxInc);

        pSetPairMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo,
                                    m_cur_mb.GlobalMacroblockPairInfo,
                                    binVal);
    }

} // void H264SegmentDecoder::DecodeMBFieldDecodingFlag_CABAC(void)

#if 0
static
Ipp8u block4_block8[16] =
{
    0,  0,  1,  1,
    0,  0,  1,  1,
    2,  2,  3,  3,
    2,  2,  3,  3
};

static
Ipp32s bit_mask[] =
{
    1<<0,1<<1,1<<2,1<<3,
    1<<4,1<<5,1<<6,1<<7,
    1<<8,1<<9,1<<10,1<<11,
    1<<12,1<<13,1<<14,1<<15,
    1<<16,1<<17,1<<18,1<<19,
    1<<20,1<<21,1<<22,1<<23,
    1<<24,1<<25,1<<26,1<<27,
    1<<28,1<<29,1<<30,1<<31,
};
#endif

Ipp32u H264SegmentDecoder::DecodeCBP_CABAC(Ipp32u color_format)
{
    H264DecoderMacroblockLocalInfo *pTop, *pLeft[2];
    H264DecoderMacroblockGlobalInfo *pGTop, *pGLeft[2];
    Ipp32u cbp = 0;
    bool bOk = true;

    // obtain pointers to neighbouring MBs
    {
        Ipp32s nNum;

        nNum = m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num;
        if (0 <= nNum)
        {
            pTop = &m_mbinfo.mbs[nNum];
            pGTop = &m_gmbinfo->mbs[nNum];
        }
        else
        {
            pTop = 0;
            pGTop = 0;
            bOk = false;
        }
        nNum = m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num;
        if (0 <= nNum)
        {
            pLeft[0] = &m_mbinfo.mbs[nNum];
            pGLeft[0] = &m_gmbinfo->mbs[nNum];
        }
        else
        {
            pLeft[0] = 0;
            pGLeft[0] = 0;
            bOk = false;
        }
        nNum = m_cur_mb.CurrentBlockNeighbours.mbs_left[2].mb_num;
        if (0 <= nNum)
        {
            pLeft[1] = &m_mbinfo.mbs[nNum];
            pGLeft[1] = &m_gmbinfo->mbs[nNum];
        }
        else
        {
            pLeft[1] = 0;
            pGLeft[1] = 0;
            bOk = false;
        }
    }

    // neightbouring MBs are present
    if ((bOk) &&
        (MBTYPE_PCM != pGTop->mbtype) &&
        (MBTYPE_PCM != pGLeft[0]->mbtype) &&
        (MBTYPE_PCM != pGLeft[1]->mbtype))
    {
        Ipp32u condTermFlagA, condTermFlagB;
        Ipp32u ctxIdxInc;
        Ipp32u mask;

        // decode first luma bit
        mask = (m_cur_mb.CurrentBlockNeighbours.mbs_left[0].block_num <= 7) ? (2) : (8);
        condTermFlagA = (pLeft[0]->cbp & mask) ? (0) : (1);
        condTermFlagB = (pTop->cbp & 4) ? (0) : (1);
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
        cbp |= m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                   ctxIdxInc);

        // decode second luma bit
        condTermFlagA = cbp ^ 1;
        condTermFlagB = (pTop->cbp & 8) ? (0) : (1);
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
        cbp |= 2 * (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                        ctxIdxInc));

        // decode third luma bit
        mask = (m_cur_mb.CurrentBlockNeighbours.mbs_left[2].block_num <= 7) ? (2) : (8);
        condTermFlagA = (pLeft[1]->cbp & mask) ? (0) : (1);
        condTermFlagB = (cbp & 1) ^ 1;
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
        cbp |= 4 * (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                        ctxIdxInc));

        // decode fourth luma bit
        condTermFlagA = ((cbp & 4) >> 2) ^ 1;
        condTermFlagB = (cbp & 2) ^ 2;
        // condTermFlagB has been already multiplyed on 2
        ctxIdxInc = condTermFlagA + condTermFlagB;
        cbp |= 8 * (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                        ctxIdxInc));

        // decode chroma CBP
        if (color_format)
        {
            Ipp32u bin;

            condTermFlagA = (pLeft[0]->cbp & 0x30) ? (1) : (0);
            condTermFlagB = (pTop->cbp & 0x30) ? (1) : (0);
            ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
            bin = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_CHROMA] +
                                                      ctxIdxInc);

            if (bin)
            {
                condTermFlagA = (pLeft[0]->cbp >> 5);
                condTermFlagB = (pTop->cbp >> 5);
                ctxIdxInc = condTermFlagA + 2 * condTermFlagB + 4;
                bin = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_CHROMA] +
                                                          ctxIdxInc);
                cbp |= (bin + 1) << 4;
            }
        }
    }
    // some of macroblocks may be absent or have I_PCM type
    else
    {
        Ipp32u condTermFlagA, condTermFlagB;
        Ipp32u ctxIdxInc;

        // decode first luma bit
        if (pLeft[0])
        {
            Ipp32u mask;
            mask = (m_cur_mb.CurrentBlockNeighbours.mbs_left[0].block_num <= 7) ? (2) : (8);
            condTermFlagA = (pLeft[0]->cbp & mask) ? (0) : (MBTYPE_PCM != pGLeft[0]->mbtype);
        }
        else
            condTermFlagA = 0;
        if (pTop)
            condTermFlagB = (pTop->cbp & 4) ? (0) : (MBTYPE_PCM != pGTop->mbtype);
        else
            condTermFlagB = 0;
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
        cbp |= m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                   ctxIdxInc);

        // decode second luma bit
        condTermFlagA = cbp ^ 1;
        if (pTop)
            condTermFlagB = (pTop->cbp & 8) ? (0) : (MBTYPE_PCM != pGTop->mbtype);
        else
            condTermFlagB = 0;
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
        cbp |= 2 * (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                        ctxIdxInc));

        // decode third luma bit
        if (pLeft[1])
        {
            Ipp32u mask;
            mask = (m_cur_mb.CurrentBlockNeighbours.mbs_left[2].block_num <= 7) ? (2) : (8);
            condTermFlagA = (pLeft[1]->cbp & mask) ? (0) : (MBTYPE_PCM != pGLeft[1]->mbtype);
        }
        else
            condTermFlagA = 0;
        condTermFlagB = (cbp & 1) ^ 1;
        ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
        cbp |= 4 * (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                        ctxIdxInc));

        // decode fourth luma bit
        condTermFlagA = ((cbp & 4) >> 2) ^ 1;
        condTermFlagB = (cbp & 2) ^ 2;
        // condTermFlagB has been already multiplyed on 2
        ctxIdxInc = condTermFlagA + condTermFlagB;
        cbp |= 8 * (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_LUMA] +
                                                        ctxIdxInc));

        // decode chroma CBP
        if (color_format)
        {
            Ipp32u bin;

            if (pLeft[0])
                condTermFlagA = (pLeft[0]->cbp & 0x30) ? (1) : (MBTYPE_PCM == pGLeft[0]->mbtype);
            else
                condTermFlagA = 0;
            if (pTop)
                condTermFlagB = (pTop->cbp & 0x30) ? (1) : (MBTYPE_PCM == pGTop->mbtype);
            else
                condTermFlagB = 0;
            ctxIdxInc = condTermFlagA + 2 * condTermFlagB;
            bin = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_CHROMA] +
                                                      ctxIdxInc);

            if (bin)
            {
                if (pLeft[0])
                    condTermFlagA = (pLeft[0]->cbp & 0x20) ? (1) : (MBTYPE_PCM == pGLeft[0]->mbtype);
                else
                    condTermFlagA = 0;
                if (pTop)
                    condTermFlagB = (pTop->cbp & 0x20) ? (1) : (MBTYPE_PCM == pGTop->mbtype);
                else
                    condTermFlagB = 0;
                ctxIdxInc = condTermFlagA + 2 * condTermFlagB + 4;
                bin = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[CODED_BLOCK_PATTERN_CHROMA] +
                                                          ctxIdxInc);
                cbp |= (bin + 1) << 4;
            }
        }
    }

    return cbp;

} // Ipp32u H264SegmentDecoder::DecodeCBP_CABAC(Ipp32u color_format)

void H264SegmentDecoder::DecodeIntraTypes4x4_CABAC(IntraType *pMBIntraTypes, bool bUseConstrainedIntra)
{
    Ipp32u block;
    // Temp arrays for modes from above and left, initially filled from
    // outside the MB, then updated with modes within the MB
    Ipp32s uModeAbove[4];
    Ipp32s uModeLeft[4];
    Ipp32s uPredMode;        // predicted mode for current 4x4 block

    IntraType *pRefIntraTypes;
    Ipp32u uLeftIndex;      // indexes into mode arrays, dependent on 8x8 block
    Ipp32u uAboveIndex;
    H264DecoderMacroblockGlobalInfo *gmbinfo=m_gmbinfo->mbs;

    // Get modes of blocks above and to the left, substituting 0
    // when above or to left is outside this MB slice. Substitute mode 2
    // when the adjacent macroblock is not 4x4 INTRA. Add 1 to actual
    // modes, so mode range is 1..9.
    if ((m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num<0) ||
        ((!IS_INTRA_MBTYPE(gmbinfo[m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num].mbtype)
                                                && bUseConstrainedIntra)))
    {
        memset(uModeAbove, 0, sizeof(uModeAbove));
    }
    else
    {
        if (gmbinfo[m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num].mbtype == MBTYPE_INTRA)
        {
            pRefIntraTypes = m_pMBIntraTypes +
                m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num * NUM_INTRA_TYPE_ELEMENTS;
            uModeAbove[0] = pRefIntraTypes[10] + 1;
            uModeAbove[1] = pRefIntraTypes[11] + 1;
            uModeAbove[2] = pRefIntraTypes[14] + 1;
            uModeAbove[3] = pRefIntraTypes[15] + 1;
        }
        else
        {
            // MB above in slice but not INTRA, use mode 2 (+1)
            uModeAbove[0] = uModeAbove[1] = uModeAbove[2] = uModeAbove[3] = 3;
        }
    }

    H264DecoderBlockLocation *pMbsLeft = m_cur_mb.CurrentBlockNeighbours.mbs_left;
    Ipp32s mb_num;
    Ipp32s i;
    for(i = 0; i < 4; i++)
    {
        mb_num = pMbsLeft[i].mb_num;
        if ((mb_num<0) ||
           ((!IS_INTRA_MBTYPE(gmbinfo[mb_num].mbtype) && bUseConstrainedIntra)))
        {
            uModeLeft[i] = 0;
        }
        else
        {
            if (gmbinfo[mb_num].mbtype  == MBTYPE_INTRA)
            {
                pRefIntraTypes = m_pMBIntraTypes + mb_num*NUM_INTRA_TYPE_ELEMENTS;
                uModeLeft[i]   = pRefIntraTypes[NIT2LIN[pMbsLeft[i].block_num]] + 1;
            }
            else
            {                 // MB left in slice but not INTRA, use mode 2 (+1)
                uModeLeft[i] = 2+1;
            }
        }
    }

    Ipp32s predModeValues[16];
    Ipp32s flag_context = ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG];
    Ipp32s pred_context = ctxIdxOffset[REM_INTRA4X4_PRED_MODE];
    for(i = 0; i < 16; i++)
    {
        if (0 == m_pBitStream->DecodeSingleBin_CABAC(flag_context))
        {
            predModeValues[i]  = 0;
            predModeValues[i] |= (m_pBitStream->DecodeSingleBin_CABAC(pred_context));
            predModeValues[i] |= (m_pBitStream->DecodeSingleBin_CABAC(pred_context) << 1);
            predModeValues[i] |= (m_pBitStream->DecodeSingleBin_CABAC(pred_context) << 2);
        }
        else
            predModeValues[i] = -1;
    }

    // Loop over the 4 8x8 blocks
    for (block=0; block<4; block++)
    {
        uAboveIndex = (block & 1) * 2;        // 0,2,0,2
        uLeftIndex = (block & 2);            // 0,0,2,2

        // upper left 4x4

        // Predicted mode is minimum of the above and left modes, or
        // mode 2 if above or left is outside slice, indicated by 0 in
        // mode array.
        uPredMode = IPP_MIN(uModeLeft[uLeftIndex], uModeAbove[uAboveIndex]);
        if (uPredMode)
            uPredMode--;
        else
            uPredMode = 2;

        // use_most_probable_mode

        // remaining_mode_selector
        if(predModeValues[4*block + 0] != -1)
        {
            if(predModeValues[4*block + 0] >= uPredMode)
                predModeValues[4*block + 0]++;
            uPredMode = predModeValues[4*block + 0];
        }

        // Save mode
        pMBIntraTypes[0] = (Ipp8u)uPredMode;
        uModeAbove[uAboveIndex] = uPredMode + 1;

        // upper right 4x4
        uPredMode = IPP_MIN(uPredMode+1, uModeAbove[uAboveIndex+1]);
        if (uPredMode)
            uPredMode--;
        else
            uPredMode = 2;

        if(predModeValues[4*block + 1] != -1)
        {
            if(predModeValues[4*block + 1] >= uPredMode)
                predModeValues[4*block + 1]++;
            uPredMode = predModeValues[4*block + 1];
        }

        pMBIntraTypes[1] = (IntraType)uPredMode;
        uModeAbove[uAboveIndex+1] = uPredMode + 1;
        uModeLeft[uLeftIndex] = uPredMode + 1;

        // lower left 4x4
        uPredMode = IPP_MIN(uModeLeft[uLeftIndex+1], uModeAbove[uAboveIndex]);
        if (uPredMode)
            uPredMode--;
        else
            uPredMode = 2;

        if(predModeValues[4*block + 2] != -1)
        {
         if(predModeValues[4*block + 2] >= uPredMode)
             predModeValues[4*block + 2]++;
            uPredMode = predModeValues[4*block + 2];
        }

        pMBIntraTypes[2] = (IntraType)uPredMode;
        uModeAbove[uAboveIndex] = uPredMode + 1;

        // lower right 4x4 (above and left must always both be in slice)
        uPredMode = IPP_MIN(uPredMode+1, uModeAbove[uAboveIndex+1]) - 1;

        if(predModeValues[4*block + 3] != -1)
        {
          if(predModeValues[4*block + 3] >= uPredMode)
              predModeValues[4*block + 3]++;
            uPredMode = predModeValues[4*block + 3];
        }

        pMBIntraTypes[3] = (IntraType)uPredMode;
        uModeAbove[uAboveIndex+1] = uPredMode + 1;
        uModeLeft[uLeftIndex+1] = uPredMode + 1;

        pMBIntraTypes += 4;
    }    // block

} // void H264SegmentDecoder::DecodeIntraTypes4x4_CABAC(Ipp32u *pMBIntraTypes, bool bUseConstrainedIntra)

void H264SegmentDecoder::DecodeIntraTypes8x8_CABAC(IntraType *pMBIntraTypes, bool bUseConstrainedIntra)
{
    Ipp32s uModeAbove[2];
    Ipp32s uModeLeft[2];
    Ipp32s uPredMode;        // predicted mode for current 4x4 block

    IntraType *pRefIntraTypes;
    Ipp32s val;

    H264DecoderMacroblockGlobalInfo *gmbinfo=m_gmbinfo->mbs;
    Ipp32u predictors=7;//5 lsb bits set
    //new version
    {
        // above, left MB available only if they are INTRA
        if ((m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num<0) || ((!IS_INTRA_MBTYPE(gmbinfo[m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num].mbtype) && bUseConstrainedIntra)))
            predictors &= (~1);//clear 1-st bit
        if ((m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num<0) || ((!IS_INTRA_MBTYPE(gmbinfo[m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num].mbtype) && bUseConstrainedIntra)))
            predictors &= (~2); //clear 2-nd bit
        if ((m_cur_mb.CurrentBlockNeighbours.mbs_left[2].mb_num<0) || ((!IS_INTRA_MBTYPE(gmbinfo[m_cur_mb.CurrentBlockNeighbours.mbs_left[2].mb_num].mbtype) && bUseConstrainedIntra)))
            predictors &= (~4); //clear 4-th bit
    }

    // Get modes of blocks above and to the left, substituting 0
    // when above or to left is outside this MB slice. Substitute mode 2
    // when the adjacent macroblock is not 4x4 INTRA. Add 1 to actual
    // modes, so mode range is 1..9.

    if (predictors&1)
    {
        if (gmbinfo[m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num].mbtype == MBTYPE_INTRA)
        {
            pRefIntraTypes = m_pMBIntraTypes + m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num * NUM_INTRA_TYPE_ELEMENTS;
            uModeAbove[0] = pRefIntraTypes[10] + 1;
            uModeAbove[1] = pRefIntraTypes[14] + 1;
        }
        else
        {   // MB above in slice but not INTRA, use mode 2 (+1)
            uModeAbove[0] = uModeAbove[1] = 2 + 1;
        }
    }
    else
    {
        uModeAbove[0] = uModeAbove[1]= 0;
    }
    if (predictors&2)
    {
        if (gmbinfo[m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num].mbtype  == MBTYPE_INTRA)
        {
            pRefIntraTypes = m_pMBIntraTypes + m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num*NUM_INTRA_TYPE_ELEMENTS;
            uModeLeft[0] = pRefIntraTypes[NIT2LIN[m_cur_mb.CurrentBlockNeighbours.mbs_left[0].block_num]] + 1;
        }
        else
        {
            // MB left in slice but not INTRA, use mode 2 (+1)
            uModeLeft[0] = 2+1;
        }
    }
    else
    {
        uModeLeft[0] = 0;
    }

    if (predictors&4)
    {
        if (gmbinfo[m_cur_mb.CurrentBlockNeighbours.mbs_left[2].mb_num].mbtype == MBTYPE_INTRA)
        {
            pRefIntraTypes = m_pMBIntraTypes + m_cur_mb.CurrentBlockNeighbours.mbs_left[2].mb_num*NUM_INTRA_TYPE_ELEMENTS;
            uModeLeft[1] = pRefIntraTypes[NIT2LIN[m_cur_mb.CurrentBlockNeighbours.mbs_left[2].block_num]] + 1;
        }
        else
        {
            // MB left in slice but not INTRA, use mode 2 (+1)
            uModeLeft[1] = 2+1;
        }
    }
    else
    {
        uModeLeft[1] = 0;
    }

    // upper left 8x8

    // Predicted mode is minimum of the above and left modes, or
    // mode 2 if above or left is outside slice, indicated by 0 in
    // mode array.
    uPredMode = IPP_MIN(uModeLeft[0], uModeAbove[0]);
    if (uPredMode)
        uPredMode--;
    else
        uPredMode = 2;

    // If next bitstream bit is 1, use predicted mode, else read new mode
    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG]))
    {
        val  = 0;
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]));
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 1);
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 2);

        uPredMode = val + (val >= uPredMode);
    }
    // Save mode
    pMBIntraTypes[0] =
    pMBIntraTypes[1] =
    pMBIntraTypes[2] =
    pMBIntraTypes[3] =
        (IntraType)uPredMode;
    uModeAbove[0] = uPredMode + 1;

    // upper right 8x8
    uPredMode = IPP_MIN(uPredMode+1, uModeAbove[1]);
    if (uPredMode)
        uPredMode--;
    else
        uPredMode = 2;

    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG]))
    {
        val  = 0;
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]));
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 1);
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 2);

        uPredMode = val + (val >= uPredMode);
    }

    pMBIntraTypes[4] =
    pMBIntraTypes[5] =
    pMBIntraTypes[6] =
    pMBIntraTypes[7] =
        (IntraType)uPredMode;
    uModeAbove[1] = uPredMode + 1;

    // lower left 4x4
    uPredMode = IPP_MIN(uModeLeft[1], uModeAbove[0]);
    if (uPredMode)
        uPredMode--;
    else
        uPredMode = 2;

    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG]))
    {
        val  = 0;
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]));
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 1);
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 2);

        uPredMode = val + (val >= uPredMode);
    }
    pMBIntraTypes[8] =
    pMBIntraTypes[9] =
    pMBIntraTypes[10] =
    pMBIntraTypes[11] =
        (IntraType)uPredMode;
    uModeAbove[0] = uPredMode + 1;

    // lower right 4x4 (above and left must always both be in slice)
    uPredMode = IPP_MIN(uPredMode+1, uModeAbove[1]) - 1;

    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG]))
    {
        val  = 0;
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]));
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 1);
        val |= (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[REM_INTRA4X4_PRED_MODE]) << 2);

        uPredMode = val + (val >= uPredMode);
    }
    pMBIntraTypes[12] =
    pMBIntraTypes[13] =
    pMBIntraTypes[14] =
    pMBIntraTypes[15] =
        (IntraType)uPredMode;

    // copy last IntraTypes to first 4 for reconstruction since they're not used for further prediction
    pMBIntraTypes[1] = pMBIntraTypes[4];
    pMBIntraTypes[2] = pMBIntraTypes[8];
    pMBIntraTypes[3] = pMBIntraTypes[12];

}  // void H264SegmentDecoder::DecodeIntraTypes8x8_CABAC( Ipp32u *pMBIntraTypes,bool bUseConstrainedIntra)

void H264SegmentDecoder::DecodeIntraPredChromaMode_CABAC(void)
{
    Ipp32u ctxIdxInc;

    {
        Ipp32u condTermFlagA = 0, condTermFlagB = 0;
        Ipp32s nNum;

        // to obtain more details
        // see subclause 9.3.3.1.1.8 of the h264 standard

        // get left macrobock condition
        nNum = m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num;
        if (0 <= nNum)
        {
            if (IS_INTRA_MBTYPE(m_gmbinfo->mbs[nNum].mbtype) && m_mbinfo.mbs[nNum].IntraTypes.intra_chroma_mode)
                condTermFlagA = 1;
        }

        // get above macroblock condition
        nNum = m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num;
        if (0 <= nNum)
        {
            if (IS_INTRA_MBTYPE(m_gmbinfo->mbs[nNum].mbtype) && m_mbinfo.mbs[nNum].IntraTypes.intra_chroma_mode)
                condTermFlagB = 1;
        }

        ctxIdxInc = condTermFlagA + condTermFlagB;
    }

    // decode chroma mode
    {
        Ipp32u nVal;

        nVal = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[INTRA_CHROMA_PRED_MODE] +
                                                   ctxIdxInc);

        if (nVal)
        {
            nVal = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[INTRA_CHROMA_PRED_MODE] +
                                                       3);

            if (nVal)
            {
                nVal = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[INTRA_CHROMA_PRED_MODE] +
                                                           3) + 1;
            }

            nVal += 1;
        }

        m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode = (Ipp8u) nVal;
    }

} // void H264SegmentDecoder::DecodeIntraPredChromaMode_CABAC(void)

Ipp32u H264SegmentDecoder::DecodeMBSkipFlag_CABAC(Ipp32s ctxIdx)
{
    Ipp32s mbAddrA = -1, mbAddrB = -1;
    Ipp32s iFirstMB = m_iFirstSliceMb;

    // obtain neigbouring blocks
    if (false == m_isMBAFF)
    {
        // obtain left macroblock addres
        if ((m_CurMB_X) &&
            (iFirstMB <= m_CurMBAddr - 1))
            mbAddrA = m_CurMBAddr - 1;

        // obtain above macroblock addres
        if ((m_CurMB_Y) &&
            (iFirstMB <= m_CurMBAddr - mb_width))
            mbAddrB = m_CurMBAddr - mb_width;
    }
    else
    {
        Ipp32s iCurrentField = 0;

        //
        // determine type of current macroblock
        // See subclause 7.4.4 of H.264 standard
        //

        if (0 == (m_CurMBAddr & 1))
        {
            Ipp32s mbAddr;

            // try to get left MB pair info
            mbAddr = m_CurMBAddr - 2;
            if ((iFirstMB <= mbAddr) &&
                (m_CurMB_X))
            {
                iCurrentField = GetMBFieldDecodingFlag(m_gmbinfo->mbs[mbAddr]);
            }
            else
            {
                // get above MB pair info
                mbAddr = m_CurMBAddr - mb_width * 2;
                if ((iFirstMB <= mbAddr) &&
                    (m_CurMB_Y))
                    iCurrentField = GetMBFieldDecodingFlag(m_gmbinfo->mbs[mbAddr]);
            }

            pSetPairMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo,
                                        m_cur_mb.GlobalMacroblockPairInfo,
                                        iCurrentField);
        }
        else
            iCurrentField = GetMBFieldDecodingFlag(m_gmbinfo->mbs[m_CurMBAddr]);

        //
        // calculate neighbouring blocks
        // See table 6-4 of H.264 standard
        //

        // top MB from macroblock pair
        if (0 == (m_CurMBAddr & 1))
        {
            // determine A macroblock
            if ((m_CurMB_X) &&
                (iFirstMB <= m_CurMBAddr - 2))
                mbAddrA = m_CurMBAddr - 2;

            // determine B macroblock
            if ((m_CurMB_Y) &&
                (iFirstMB <= m_CurMBAddr - mb_width * 2))
            {
                if (iCurrentField &
                    GetMBFieldDecodingFlag(m_gmbinfo->mbs[m_CurMBAddr - mb_width * 2]))
                    mbAddrB = m_CurMBAddr - mb_width * 2;
                else
                    mbAddrB = m_CurMBAddr - mb_width * 2 + 1;
            }
        }
        // bottom MB from macroblock pair
        else
        {
            // determine A macroblock
            if ((m_CurMB_X) &&
                (iFirstMB <= m_CurMBAddr - 2))
            {
                if (iCurrentField == GetMBFieldDecodingFlag(m_gmbinfo->mbs[m_CurMBAddr - 2]))
                    mbAddrA = m_CurMBAddr - 2;
                else
                    mbAddrA = m_CurMBAddr - 3;
            }

            // determine B macroblock
            if (0 == iCurrentField)
                mbAddrB = m_CurMBAddr - 1;
            else
            {
                if ((m_CurMB_Y) &&
                    (iFirstMB <= m_CurMBAddr - mb_width * 2))
                {
                    mbAddrB = m_CurMBAddr - mb_width * 2;
                }
            }
        }
    }

    // decode skip flag
    {
        Ipp32s condTermFlagA = 0, condTermFlagB = 0;
        Ipp32s ctxIdxInc;

        // obtain left macroblock info
        if ((0 <= mbAddrA) &&
            (!GetMBSkippedFlag(m_gmbinfo->mbs[mbAddrA])))
            condTermFlagA = 1;

        // obtain above macroblock info
        if ((0 <= mbAddrB) &&
            (!GetMBSkippedFlag(m_gmbinfo->mbs[mbAddrB])))
            condTermFlagB = 1;

        ctxIdxInc = condTermFlagA + condTermFlagB;

        return m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[ctxIdx] + ctxIdxInc);
    }

} // Ipp32u H264SegmentDecoder::DecodeMBSkipFlag_CABAC(Ipp32s ctxIdx)

static
Ipp32s SBTYPETBL[] =
{
    SBTYPE_8x8, SBTYPE_8x4, SBTYPE_4x8, SBTYPE_4x4
};

void H264SegmentDecoder::DecodeMBTypeISlice_CABAC(void)
{
    Ipp32s condTermFlagA = 0, condTermFlagB = 0;
    Ipp32s mbAddr;
    Ipp32s ctxIdxInc;
    Ipp32s mb_type;

    // get left macroblock type
    mbAddr = m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num;
    if (0 <= mbAddr)
    {
        if (MBTYPE_INTRA != m_gmbinfo->mbs[mbAddr].mbtype)
            condTermFlagA = 1;
    }

    // get abouve macroblock type
    mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num;
    if (0 <= mbAddr)
    {
        if (MBTYPE_INTRA != m_gmbinfo->mbs[mbAddr].mbtype)
            condTermFlagB = 1;
    }

    ctxIdxInc = condTermFlagA + condTermFlagB;
    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_I] + ctxIdxInc))
    {
        // we have I_NxN macroblock type
        m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA;
    }
    else
    {
        if (0 == m_pBitStream->DecodeSymbolEnd_CABAC())
        {
            Ipp32u code;

            // luma CBP bit
            if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_I] + 3))
            {
                m_cur_mb.LocalMacroblockInfo->cbp = 0x0f;
                mb_type = 16;
            }
            else
            {
                m_cur_mb.LocalMacroblockInfo->cbp = 0x00;
                mb_type = 0;
            }

            // chroma CBP bits
            if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_I] + 4))
            {
                if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_I] + 5))
                {
                    m_cur_mb.LocalMacroblockInfo->cbp |= 0x20;
                    mb_type |= 12;
                }
                else
                {
                    m_cur_mb.LocalMacroblockInfo->cbp |= 0x10;
                    mb_type |= 8;
                }
            }

            // prediction bits
            code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_I] + 6);
            code = (code << 1) |
                    m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_I] + 7);
            mb_type |= code;

            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA_16x16;
            IntraType *pMBIntraTypes = m_pMBIntraTypes + m_CurMBAddr*NUM_INTRA_TYPE_ELEMENTS;

            pMBIntraTypes[0] =
            pMBIntraTypes[1] =
            pMBIntraTypes[2] =
            pMBIntraTypes[3] = (mb_type) & 0x03;
        }
        else
        {
            // we have I_PCM macroblock type
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_PCM;
        }
    }

} // void H264SegmentDecoder::DecodeMBTypeISlice_CABAC(Ipp32u *pMBIntraTypes,

static Ipp8u MBTypesInPSlice[] =
{
    MBTYPE_FORWARD,
    MBTYPE_INTER_8x8,
    MBTYPE_INTER_8x16,
    MBTYPE_INTER_16x8
};

void H264SegmentDecoder::DecodeMBTypePSlice_CABAC(void)
{
    // macroblock has P type
    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 0))
    {
        Ipp32u code;

        code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 1);
        code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + code + 2) +
               code * 2;

        // See table 9-28 of H.264 standard
        // for translating bin string (code) to mb_type
        m_cur_mb.GlobalMacroblockInfo->mbtype = (Ipp8u) MBTypesInPSlice[code];

        // decode subblock types
        if (MBTYPE_INTER_8x8 == m_cur_mb.GlobalMacroblockInfo->mbtype)
        {
            Ipp32s subblock;
            Ipp32u uCodeNum;

            for (subblock = 0; subblock < 4; subblock++)
            {
                // block type decoding
                if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_P_SP] + 0))
                    uCodeNum = 0;
                else
                {
                    if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_P_SP] + 1))
                        uCodeNum = 3 - m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_P_SP] + 2);
                    else
                        uCodeNum = 1;
                }

                m_cur_mb.GlobalMacroblockInfo->sbtype[subblock] = (Ipp8u) SBTYPETBL[uCodeNum];
            }
        }
    }
    // decode intra macroblock type in P slice
    else
    {
        // macroblock has I_NxN type
        if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 3))
        {
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA;
        }
        else
        {
            // macroblock has I_16x16 type
            if (0 == m_pBitStream->DecodeSymbolEnd_CABAC())
            {
                Ipp32u code, mb_type;

                // luma CBP bit
                if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 4))
                {
                    m_cur_mb.LocalMacroblockInfo->cbp = 0x0f;
                    mb_type = 16;
                }
                else
                {
                    m_cur_mb.LocalMacroblockInfo->cbp = 0x00;
                    mb_type = 0;
                }

                // chroma CBP bits
                if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 5))
                {
                    if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 5))
                    {
                        m_cur_mb.LocalMacroblockInfo->cbp |= 0x20;
                        mb_type |= 12;
                    }
                    else
                    {
                        m_cur_mb.LocalMacroblockInfo->cbp |= 0x10;
                        mb_type |= 8;
                    }
                }

                // prediction bits
                code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 6);
                code = (code << 1) |
                       m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_P_SP] + 6);
                mb_type |= code;

                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA_16x16;
                IntraType *pMBIntraTypes = m_pMBIntraTypes + m_CurMBAddr*NUM_INTRA_TYPE_ELEMENTS;

                pMBIntraTypes[0] =
                pMBIntraTypes[1] =
                pMBIntraTypes[2] =
                pMBIntraTypes[3] = (mb_type) & 0x03;
            }
            else
            {
                // we have I_PCM macroblock type
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_PCM;
            }

        }
    }

} // void H264SegmentDecoder::DecodeMBTypePSlice_CABAC(void)

static
Ipp8u LargeSubMBSubDirBSlice[][2] =
{
    {D_DIR_FWD, D_DIR_FWD},
    {D_DIR_BWD, D_DIR_BWD},
    {D_DIR_FWD, D_DIR_BWD},
    {D_DIR_BWD, D_DIR_FWD},

    {D_DIR_FWD, D_DIR_BIDIR},
    {D_DIR_BWD, D_DIR_BIDIR},
    {D_DIR_BIDIR, D_DIR_FWD},
    {D_DIR_BIDIR, D_DIR_BWD},
    {D_DIR_BIDIR, D_DIR_BIDIR}
};

static
Ipp8u SmallSubMBSubDirBSlice[16] =
{
    (Ipp8u) -1,
    (Ipp8u) -1,
    D_DIR_FWD,
    D_DIR_FWD,
    D_DIR_FWD,
    D_DIR_FWD,
    D_DIR_BWD,
    D_DIR_BWD,
    D_DIR_BWD,
    D_DIR_BIDIR,
    D_DIR_BIDIR,
    D_DIR_FWD,
    D_DIR_BWD,
    D_DIR_BWD,
    D_DIR_BIDIR,
    D_DIR_BIDIR
};

static
Ipp8u SmallSubMBSubDivBSlice[16] =
{
    (Ipp8u) -1,
    (Ipp8u) -1,
    SBTYPE_8x4,
    SBTYPE_8x4,
    SBTYPE_4x8,
    SBTYPE_4x8,
    SBTYPE_8x4,
    SBTYPE_8x4,
    SBTYPE_4x8,
    SBTYPE_8x4,
    SBTYPE_4x8,
    SBTYPE_4x4,
    SBTYPE_4x4,
    SBTYPE_4x4,
    SBTYPE_4x4,
    SBTYPE_4x4
};

void H264SegmentDecoder::DecodeMBTypeBSlice_CABAC(void)
{
    Ipp32s ctxIdxInc;

    // calculate context index increment
    {
        Ipp32s condTermFlagA = 0, condTermFlagB = 0;
        Ipp32s nMBNum;

        // obtain left macroblock information
        nMBNum = m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num;
        if (0 <= nMBNum)
        {
            if (!GetMBDirectSkipFlag(m_gmbinfo->mbs[nMBNum]))
                condTermFlagA = 1;
        }

        // obtain above macroblock information
        nMBNum = m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num;
        if (0 <= nMBNum)
        {
            if (!GetMBDirectSkipFlag(m_gmbinfo->mbs[nMBNum]))
                condTermFlagB = 1;
        }

        ctxIdxInc = condTermFlagA + condTermFlagB;
    }

    // for translation bin string into mb type
    // see table 9-28 of the h264 standard

    // decode the type
    if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] +
                                                 ctxIdxInc))
    {
        // B_Direct_16x16 type
        m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_DIRECT;
        pSetMBDirectFlag(m_cur_mb.GlobalMacroblockInfo);
        memset(m_cur_mb.GlobalMacroblockInfo->sbtype, 0, sizeof(m_cur_mb.GlobalMacroblockInfo->sbtype));
    }
    else if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] +
                                                      3))
    {
        // B_L0_16x16 & B_L1_16x16 types
        if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 5))
        {
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_BACKWARD;
        }
        else
        {
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_FORWARD;
        }
    }
    else
    {
        Ipp32u code;

        // get next 4 bins
        code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 4);
        code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 5) +
               code * 2;
        code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 5) +
               code * 2;
        code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 5) +
               code * 2;

        if (0 == code)
        {
            // B_Bi_16x16 type
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_BIDIR;
        }
        else if (7 >= code)
        {
            if (code & 1)
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_16x8;
            else
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_8x16;

            // set the sub directions
            code = (code - 1) >> 1;
            m_cur_mb.LocalMacroblockInfo->sbdir[0] = LargeSubMBSubDirBSlice[code][0];
            m_cur_mb.LocalMacroblockInfo->sbdir[1] = LargeSubMBSubDirBSlice[code][1];
        }
        else if (14 == code)
        {
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_8x16;
            m_cur_mb.LocalMacroblockInfo->sbdir[0] = D_DIR_BWD;
            m_cur_mb.LocalMacroblockInfo->sbdir[1] = D_DIR_FWD;
        }
        else if (15 == code)
        {
            Ipp32s iSubBlock;

            // block has 8x8 subdivision
            m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_8x8;

            // we need to decode sub directions
            for (iSubBlock = 0; iSubBlock < 4; iSubBlock += 1)
            {
                // for translation bin string into sub-macroblock type
                // see table 9-29 of the h264 standard

                // the subblock has the direct type
                if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B]))
                {
                    m_cur_mb.GlobalMacroblockInfo->sbtype[iSubBlock] = SBTYPE_DIRECT;
                    m_cur_mb.LocalMacroblockInfo->sbdir[iSubBlock] = D_DIR_DIRECT;
                }
                // the subblock has 8x8 subdivision
                else if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B] + 1))
                {
                    m_cur_mb.GlobalMacroblockInfo->sbtype[iSubBlock] = SBTYPE_8x8;
                    if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B] + 3))
                        m_cur_mb.LocalMacroblockInfo->sbdir[iSubBlock] = D_DIR_BWD;
                    else
                        m_cur_mb.LocalMacroblockInfo->sbdir[iSubBlock] = D_DIR_FWD;
                }
                else
                {
                    code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B] + 2);
                    code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B] + 3) +
                           code * 2;
                    code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B] + 3) +
                           code * 2;

                    if (0 == code)
                    {
                        m_cur_mb.GlobalMacroblockInfo->sbtype[iSubBlock] = SBTYPE_8x8;
                        m_cur_mb.LocalMacroblockInfo->sbdir[iSubBlock] = D_DIR_BIDIR;
                    }
                    else
                    {
                        if ((4 == code) || (5 == code))
                        {
                            code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[SUB_MB_TYPE_B] + 3) +
                                   code * 2;
                        }
                        else
                            code += code;

                        // get subdivision and directions
                        m_cur_mb.GlobalMacroblockInfo->sbtype[iSubBlock] = SmallSubMBSubDivBSlice[code];
                        m_cur_mb.LocalMacroblockInfo->sbdir[iSubBlock] = SmallSubMBSubDirBSlice[code];
                    }
                }
            }
        }
        else if (13 != code)
        {
            code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 5) +
                   code * 2;

            if (code & 1)
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_8x16;
            else
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_16x8;

            // set the sub directions
            code = (code >> 1) & 0x07;
            m_cur_mb.LocalMacroblockInfo->sbdir[0] = LargeSubMBSubDirBSlice[4 + code][0];
            m_cur_mb.LocalMacroblockInfo->sbdir[1] = LargeSubMBSubDirBSlice[4 + code][1];
        }
        else
        {
            // macroblock has I_NxN type
            if (0 == m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] +
                                                         5))
            {
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA;
            }
            // macroblock has I_16x16 type
            else if (0 == m_pBitStream->DecodeSymbolEnd_CABAC())
            {
                Ipp32u mb_type;

                // luma CBP bit
                if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 6))
                {
                    m_cur_mb.LocalMacroblockInfo->cbp = 0x0f;
                    mb_type = 16;
                }
                else
                {
                    m_cur_mb.LocalMacroblockInfo->cbp = 0x00;
                    mb_type = 0;
                }

                // chroma CBP bits
                if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 7))
                {
                    if (m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 7))
                    {
                        m_cur_mb.LocalMacroblockInfo->cbp |= 0x20;
                        mb_type |= 12;
                    }
                    else
                    {
                        m_cur_mb.LocalMacroblockInfo->cbp |= 0x10;
                        mb_type |= 8;
                    }
                }

                // prediction bits
                code = m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 8);
                code = (code << 1) |
                       m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[MB_TYPE_B] + 8);
                mb_type |= code;

                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA_16x16;
                IntraType *pMBIntraTypes = m_pMBIntraTypes + m_CurMBAddr*NUM_INTRA_TYPE_ELEMENTS;

                pMBIntraTypes[0] =
                pMBIntraTypes[1] =
                pMBIntraTypes[2] =
                pMBIntraTypes[3] = (mb_type) & 0x03;
            }
            // macroblock has PCM type
            else
            {
                // we have I_PCM macroblock type
                m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_PCM;
            }
        }
    }

} // void H264SegmentDecoder::DecodeMBTypeBSlice_CABAC(void)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
