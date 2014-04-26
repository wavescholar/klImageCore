//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include <string.h>
#include <math.h>
#include "vm_debug.h"
#include "umc_h264_core_enc.h"
#include "umc_h264_video_encoder.h"
#include "umc_h264_tables.h"
#include "umc_h264_bme.h"


////////////////////////////////////////////////////////////////////////////////
// Uncomment the following #define to remove the left MB dependency in the
// code to select the 4x4 Intra mode for a given block.

// The pels of the 4x4 block are labelled a..p. The predictor pels above
// are labelled A..H, from the left I..L, and from above left M, as follows:
//
//  M A B C D E F G H
//  I a b c d
//  J e f g h
//  K i j k l
//  L m n o p

// Predictor array index definitions
#define P_Z PredPel[0]
#define P_A PredPel[1]
#define P_B PredPel[2]
#define P_C PredPel[3]
#define P_D PredPel[4]
#define P_E PredPel[5]
#define P_F PredPel[6]
#define P_G PredPel[7]
#define P_H PredPel[8]
#define P_I PredPel[9]
#define P_J PredPel[10]
#define P_K PredPel[11]
#define P_L PredPel[12]
#define P_M PredPel[13]
#define P_N PredPel[14]
#define P_O PredPel[15]
#define P_P PredPel[16]
#define P_Q PredPel[17]
#define P_R PredPel[18]
#define P_S PredPel[19]
#define P_T PredPel[20]
#define P_U PredPel[21]
#define P_V PredPel[22]
#define P_W PredPel[23]
#define P_X PredPel[24]

// Predicted pixel array offset macros
#define P_a pPredBuf[0]
#define P_b pPredBuf[1]
#define P_c pPredBuf[2]
#define P_d pPredBuf[3]
#define P_e pPredBuf[0+1*16]
#define P_f pPredBuf[1+1*16]
#define P_g pPredBuf[2+1*16]
#define P_h pPredBuf[3+1*16]
#define P_i pPredBuf[0+2*16]
#define P_j pPredBuf[1+2*16]
#define P_k pPredBuf[2+2*16]
#define P_l pPredBuf[3+2*16]
#define P_m pPredBuf[0+3*16]
#define P_n pPredBuf[1+3*16]
#define P_o pPredBuf[2+3*16]
#define P_p pPredBuf[3+3*16]

#define NUM_AI_MODES 9

Ipp32s intra_modes[4][8] = {
    { 0, 1, 3, 7, 8,-1,-1,-1},
    { 0, 1, 3, 4, 5, 6, 7, 8},
    { 0, 3, 7,-1,-1,-1,-1,-1},
    { 1, 8,-1,-1,-1,-1,-1,-1}
};

static const Ipp8u uBlockURPredOK[] = {
    // luma
    0xff, 0xff, 0xff, 0,
    0xff, 0xff, 0xff, 0,
    0xff, 0xff, 0xff, 0,
    0xff, 0,    0xff, 0
};

typedef enum {
    PRED16x16_VERT   = 0,
    PRED16x16_HORZ   = 1,
    PRED16x16_DC     = 2,
    PRED16x16_PLANAR = 3
} Enum16x16PredType;

typedef enum {
    PRED8x8_DC     = 0,
    PRED8x8_HORZ   = 1,
    PRED8x8_VERT   = 2,
    PRED8x8_PLANAR = 3
} Enum8x8PredType;

typedef enum {
    LEFT_AVAILABLE = 1, // Pixels to the left from the MB are available.
    TOP_AVAILABLE  = 2  // Pixels above the MB are available.
} PixelsAvailabilityType;

inline void MemorySet(Ipp8u *dst, Ipp32s val, Ipp32s length)
{
    ippsSet_8u(val, dst, length);
}

inline void MemorySet(Ipp16u *dst, Ipp32s val, Ipp32s length)
{
    ippsSet_16s((Ipp16s)val, (Ipp16s*)dst, length);
}




///////////////////////////////////////////////



template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32u uBestSAD, Ipp32u *puAIMBSAD)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMB = cur_mb.uMB;
    Ipp32s BestMode = 0;
    *puAIMBSAD = 0;//rd_quant_intra[curr_slice->m_cur_mb.LocalMacroblockInfo->QP]; //TODO ADB

    // pointer to upper left pel of first block
    PIXTYPE *pBlock = cur_mb.mbPtr;
    PIXTYPE *rBlock = core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field];
    Ipp32s uBlock;
    cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA;
    if( core_enc->m_Analyse & ANALYSE_RD_OPT ){
        H264BsBase_CopyContextCABAC_I4x4(
            &curr_slice->fBitstreams[2]->m_base,
            curr_slice->m_pbitstream,
            !curr_slice->m_is_cur_mb_field);
        //curr_slice->fBitstreams[2]->m_lcodIRange = 256;
        BestMode = 2;
        cur_mb.LocalMacroblockInfo->cbp_bits = 0;
    }
    for (uBlock = 0; uBlock < 16; uBlock++){
        PIXTYPE *pPredBuf = 0;
        if (curr_slice->m_use_transform_for_intra_decision)
            pPredBuf = cur_mb.mb4x4.prediction + xoff[uBlock] + yoff[uBlock]*16;
        if((core_enc->m_Analyse & ANALYSE_RD_OPT) && pPredBuf ){
            H264BsBase* pBitstream = curr_slice->m_pbitstream;
            curr_slice->m_cur_mb.intra_types[uBlock] = BestMode;
            *puAIMBSAD += H264CoreEncoder_Intra4x4SelectRD(state, curr_slice, pBlock, rBlock, uBlock, curr_slice->m_cur_mb.intra_types, pPredBuf);
            BestMode = curr_slice->m_cur_mb.intra_types[uBlock];
            curr_slice->m_pbitstream = pBitstream;
        }else{
            *puAIMBSAD += H264CoreEncoder_AIModeSelectOneBlock(state, curr_slice, pBlock, rBlock, uBlock, curr_slice->m_cur_mb.intra_types, pPredBuf);
        }
        if( *puAIMBSAD > uBestSAD ) return;
        // next block
        pBlock += core_enc->m_EncBlockOffsetInc[curr_slice->m_is_cur_mb_field][uBlock];
        rBlock += core_enc->m_EncBlockOffsetInc[curr_slice->m_is_cur_mb_field][uBlock];
    }
    if ((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE)) {
        // for ANALYSE_RD_OPT it is need to calc Chroma for non INTRA slices
        H264BsBase *pBitstream = curr_slice->m_pbitstream;
        H264BsFake_Reset(curr_slice->fakeBitstream);
        H264BsFake_CopyContext_CABAC(curr_slice->fakeBitstream, pBitstream, !curr_slice->m_is_cur_mb_field, 0);
        curr_slice->m_pbitstream = (H264BsBase* )curr_slice->fakeBitstream;
        cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        H264MacroblockLocalInfo sLocalMBinfo = *cur_mb.LocalMacroblockInfo;
        H264MacroblockGlobalInfo sGlobalMBinfo = *cur_mb.GlobalMacroblockInfo;
        pSetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo, false);
        cur_mb.LocalMacroblockInfo->cbp_luma = cur_mb.m_uIntraCBP4x4;
        H264CoreEncoder_TransQuantIntra_RD(state, curr_slice);
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        Ipp32s bs = H264BsFake_GetBsOffset(curr_slice->fakeBitstream);
        Ipp32s d = SSD16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, cur_mb.mb4x4.reconstruct, 16)<<5;
        //Ipp32s d = SSD16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field], cur_mb.mbPitchPixels)<<5;
        *puAIMBSAD = d + cur_mb.lambda * bs;
        curr_slice->m_pbitstream = pBitstream;
        *cur_mb.LocalMacroblockInfo = sLocalMBinfo;
        *cur_mb.GlobalMacroblockInfo = sGlobalMBinfo;
    } else {
        Ipp32s iQP = cur_mb.lumaQP51;
        *puAIMBSAD += BITS_COST(24, glob_RDQM[iQP]);
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock8x8(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32u uBestSAD, Ipp32u *puAIMBSAD)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMB = cur_mb.uMB;
    Ipp32s BestMode = 0;
    *puAIMBSAD = 0;//rd_quant_intra[curr_slice->m_cur_mb.LocalMacroblockInfo->QP]; //TODO ADB

    // pointer to upper left pel of first block
    PIXTYPE *pBlock = cur_mb.mbPtr;
    PIXTYPE *rBlock = core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field];
    Ipp32s uBlock;
    cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA;
    pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 1);
    if( core_enc->m_Analyse & ANALYSE_RD_OPT ){
        H264BsBase_CopyContextCABAC_I8x8(
            &curr_slice->fBitstreams[2]->m_base,
            curr_slice->m_pbitstream,
            !curr_slice->m_is_cur_mb_field);
        //curr_slice->fBitstreams[2]->m_lcodIRange = 256;
        BestMode = 2;
        cur_mb.LocalMacroblockInfo->cbp_bits = 0;
    }
    for (uBlock = 0; uBlock < 4; uBlock++){
        PIXTYPE *pPredBuf = NULL;
        if (curr_slice->m_use_transform_for_intra_decision)
            pPredBuf = cur_mb.mb8x8.prediction + xoff[4*uBlock] + yoff[4*uBlock]*16;
        if((core_enc->m_Analyse & ANALYSE_RD_OPT) && pPredBuf ){
            H264BsBase* pBitstream = curr_slice->m_pbitstream;
            curr_slice->m_cur_mb.intra_types[uBlock<<2] = BestMode;
            *puAIMBSAD += H264CoreEncoder_Intra8x8SelectRD(state, curr_slice, pBlock, rBlock, uBlock, curr_slice->m_cur_mb.intra_types, pPredBuf);
            BestMode = curr_slice->m_cur_mb.intra_types[uBlock<<2];
            curr_slice->m_pbitstream = pBitstream;
        }else{
            *puAIMBSAD += H264CoreEncoder_AIModeSelectOneMB_8x8(state, curr_slice, pBlock, rBlock, uBlock, curr_slice->m_cur_mb.intra_types, pPredBuf);
        }
        if( *puAIMBSAD > uBestSAD ){
            pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 0);
            return;  //Don't count anymore because previous SAD is better.
        }
        pBlock += core_enc->m_EncBlockOffsetInc[curr_slice->m_is_cur_mb_field][uBlock]*2;
        rBlock += core_enc->m_EncBlockOffsetInc[curr_slice->m_is_cur_mb_field][uBlock]*2;
    }
    if ((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE)) {
        // for ANALYSE_RD_OPT it is need to calc Chroma for non INTRA slices
        H264BsBase* pBitstream = curr_slice->m_pbitstream;
        cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        H264BsFake_Reset(curr_slice->fakeBitstream);
        H264BsFake_CopyContext_CABAC(curr_slice->fakeBitstream, pBitstream, !curr_slice->m_is_cur_mb_field, 1);
        curr_slice->m_pbitstream = (H264BsBase *)curr_slice->fakeBitstream;
        H264MacroblockLocalInfo sLocalMBinfo = *cur_mb.LocalMacroblockInfo;
        H264MacroblockGlobalInfo sGlobalMBinfo = *cur_mb.GlobalMacroblockInfo;
        pSetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo, true);
        cur_mb.LocalMacroblockInfo->cbp_luma = cur_mb.m_uIntraCBP8x8;
        H264CoreEncoder_TransQuantIntra_RD(state, curr_slice);
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        Ipp32s bs = H264BsFake_GetBsOffset(curr_slice->fakeBitstream);
        //Ipp32s d = SSD16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field], cur_mb.mbPitchPixels)<<5;
        Ipp32s d = SSD16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, cur_mb.mb8x8.reconstruct, 16)<<5;
        *puAIMBSAD = d + cur_mb.lambda * bs;
        curr_slice->m_pbitstream = pBitstream;
        *cur_mb.LocalMacroblockInfo = sLocalMBinfo;
        *cur_mb.GlobalMacroblockInfo = sGlobalMBinfo;
    } else {
        Ipp32s iQP = cur_mb.lumaQP51;
        *puAIMBSAD += BITS_COST(12, glob_RDQM[iQP]);
    }
    pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 0);
}

//  Obtains the above and left prediction pels for the 4x4 block, following edge rules.
//  The prediction pel buffer is ordered as follows:
//  [0]:    M
//  [1..8]: A..H
//  [9..12] I..L
//  Predictors M and E..H are used only for the diagonal modes (modes 3 and greater).
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetBlockPredPels(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pLeftRefBlock,         // pointer to block in reference picture
    Ipp32u uLeftPitch,              // of source data. Pitch in pixels.
    PIXTYPE* pAboveRefBlock,        // pointer to block in reference picture
    Ipp32u uAbovePitch,             // of source data. Pitch in pixels.
    PIXTYPE* pAboveLeftRefBlock,    // pointer to block in reference picture
    Ipp32u uAboveLeftPitch,         // of source data. Pitch in pixels.
    Ipp32u uBlock,                  // 0..15 for luma blocks only
    PIXTYPE* PredPel)               // result here
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s raster_block = block_subblock_mapping_[uBlock];

    bool bLeft = false;
    if (BLOCK_IS_ON_LEFT_EDGE(raster_block))
        bLeft = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[raster_block / 4].mb_num < 0;
    bool bTop = false;
    if (BLOCK_IS_ON_TOP_EDGE(raster_block))
        bTop = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num < 0;
    if (bTop && bLeft) {
        P_A = P_B = P_C = P_D = P_I = P_J = P_K = P_L = 1<<(core_enc->m_PicParamSet.bit_depth_luma - 1);
    } else if (bTop && !bLeft) {
        for (Ipp32s i = 0; i < 4; i++) {
            PredPel[i+1] = PredPel[i+9] = (pLeftRefBlock + i*uLeftPitch)[-1];
        }
    } else if (!bTop && bLeft) {
        for (Ipp32s i = 0; i < 4; i++) {
            PredPel[i+1] = PredPel[i+9] = (pAboveRefBlock - uAbovePitch)[i];
        }
    } else {
        for (Ipp32s i = 0; i < 4; i++) {
            PredPel[i+1] = (pAboveRefBlock - uAbovePitch)[i];
            PredPel[i+9] = (pLeftRefBlock + i*uLeftPitch)[-1];
        }
        // for diagonal modes
        P_Z = (pAboveLeftRefBlock - uAboveLeftPitch)[-1];
    }
    if (!bTop) {
        // Get EFGH, predictors pels above and to the right of the block.
        // Use D when EFGH are not valid, which is when the block is on
        // the right edge of the MB and the MB is at the right edge of
        // the picture; or when the block is on the right edge of the MB
        // but not in the top row.
        bool bRight = raster_block == 3 ? curr_slice->m_cur_mb.BlockNeighbours.mb_above_right.mb_num < 0 : false;
        if (uBlockURPredOK[uBlock] == 0 || bRight) {
            P_E = P_F = P_G = P_H = P_D;
        } else {
            P_E = (pAboveRefBlock - uAbovePitch)[4];
            P_F = (pAboveRefBlock - uAbovePitch)[5];
            P_G = (pAboveRefBlock - uAbovePitch)[6];
            P_H = (pAboveRefBlock - uAbovePitch)[7];
        }
    }
}

// Find advanced intra prediction block, store in PredBuf, which has a pitch = 16
template<typename PIXTYPE>
void H264CoreEncoder_GetPredBlock(
    Ipp32u uMode,           // advanced intra mode of the block
    PIXTYPE *pPredBuf,
    PIXTYPE* PredPel)       // predictor pels
{
    Ipp32u sum;
    Ipp32s i, j;
    PIXTYPE PredVal;

    switch (uMode)
    {
    case 0:
        // mode 0: vertical prediction (from above)
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                pPredBuf[i] = PredPel[i+1]; // ABCD
            }
            pPredBuf += 16;
        }
        break;
    case 1:
        // mode 1: horizontal prediction (from left)
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                pPredBuf[i] = PredPel[j+9]; // IJKL
            }
            pPredBuf += 16;
        }
        break;
    case 2:
        // mode 2: DC prediction
        sum = 0;
        for (i = 0; i < 4; i++) {
            sum += PredPel[i+9];    // IJKL
            sum += PredPel[i+1];    // ABCD
        }
        PredVal = (PIXTYPE)((sum + 4) >> 3);

        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                // Warning: The WinCE compiler generated incorrect code (storing predictor in the first byte only) when the following line of code was: pPredBuf[i] = (PIXTYPE)sum;
                pPredBuf[i] = PredVal;
            }
            pPredBuf += 16;
        }
        break;
    case 3:
        // mode 3: diagonal down/left prediction
        P_a =                   (P_A + ((P_B) << 1) + P_C + 2) >> 2;
        P_b = P_e =             (P_B + ((P_C) << 1) + P_D + 2) >> 2;
        P_c = P_f = P_i =       (P_C + ((P_D) << 1) + P_E + 2) >> 2;
        P_d = P_g = P_j = P_m = (P_D + ((P_E) << 1) + P_F + 2) >> 2;
        P_h = P_k = P_n =       (P_E + ((P_F) << 1) + P_G + 2) >> 2;
        P_l = P_o =             (P_F + ((P_G) << 1) + P_H + 2) >> 2;
        P_p =                   (P_G + ((P_H) << 1) + P_H + 2) >> 2;
        break;
    case 4:
        // mode 4: diagonal down/right prediction
        P_m =                   (P_J + ((P_K) << 1) + P_L + 2) >> 2;
        P_i = P_n =             (P_I + ((P_J) << 1) + P_K + 2) >> 2;
        P_e = P_j = P_o =       (P_Z + ((P_I) << 1) + P_J + 2) >> 2;
        P_a = P_f = P_k = P_p = (P_A + ((P_Z) << 1) + P_I + 2) >> 2;
        P_b = P_g = P_l =       (P_Z + ((P_A) << 1) + P_B + 2) >> 2;
        P_c = P_h =             (P_A + ((P_B) << 1) + P_C + 2) >> 2;
        P_d =                   (P_B + ((P_C) << 1) + P_D + 2) >> 2;
        break;
    case 5 :
        // mode 5: vertical-right prediction
        P_a = P_j = (P_Z + P_A + 1) >> 1;
        P_b = P_k = (P_A + P_B + 1) >> 1;
        P_c = P_l = (P_B + P_C + 1) >> 1;
        P_d =       (P_C + P_D + 1) >> 1;
        P_e = P_n = (P_I + ((P_Z) << 1) + P_A + 2) >> 2;
        P_f = P_o = (P_Z + ((P_A) << 1) + P_B + 2) >> 2;
        P_g = P_p = (P_A + ((P_B) << 1) + P_C + 2) >> 2;
        P_h =       (P_B + ((P_C) << 1) + P_D + 2) >> 2;
        P_i =       (P_Z + ((P_I) << 1) + P_J + 2) >> 2;
        P_m =       (P_I + ((P_J) << 1) + P_K + 2) >> 2;
        break;
    case 6 :
        // mode 6: horizontal-down prediction
        P_a = P_g = (P_Z + P_I + 1) >> 1;
        P_b = P_h = (P_I + ((P_Z) << 1) + P_A + 2) >> 2;
        P_c =       (P_Z + ((P_A) << 1) + P_B + 2) >> 2;
        P_d =       (P_A + ((P_B) << 1) + P_C + 2) >> 2;
        P_e = P_k = (P_I + P_J + 1) >> 1;
        P_f = P_l = (P_Z + ((P_I) << 1) + P_J + 2) >> 2;
        P_i = P_o = (P_J + P_K + 1) >> 1;
        P_j = P_p = (P_I + ((P_J) << 1) + P_K + 2) >> 2;
        P_m =       (P_K + P_L + 1) >> 1;
        P_n =       (P_J + ((P_K) << 1) + P_L + 2) >> 2;
        break;
    case 7 :
        // mode 7: vertical-left prediction
        P_a =       (P_A + P_B + 1) >> 1;
        P_b = P_i = (P_B + P_C + 1) >> 1;
        P_c = P_j = (P_C + P_D + 1) >> 1;
        P_d = P_k = (P_D + P_E + 1) >> 1;
        P_l =       (P_E + P_F + 1) >> 1;
        P_e =       (P_A + ((P_B) << 1) + P_C + 2) >> 2;
        P_f = P_m = (P_B + ((P_C) << 1) + P_D + 2) >> 2;
        P_g = P_n = (P_C + ((P_D) << 1) + P_E + 2) >> 2;
        P_h = P_o = (P_D + ((P_E) << 1) + P_F + 2) >> 2;
        P_p =       (P_E + ((P_F) << 1) + P_G + 2) >> 2;
        break;
    case 8 :
        // mode 8: horizontal-up prediction
        P_a =       (P_I + P_J + 1) >> 1;
        P_b =       (P_I + ((P_J) << 1) + P_K + 2) >> 2;
        P_c = P_e = (P_J + P_K + 1) >> 1;
        P_d = P_f = (P_J + ((P_K) << 1) + P_L + 2) >> 2;
        P_g = P_i = (P_K + P_L + 1) >> 1;
        P_h = P_j = (P_K + ((P_L) << 1) + P_L + 2) >> 2;
        P_k = P_l = P_m = P_n = P_o = P_p = (P_L);
        break;
    default:
        break;
    }
}

#ifdef BITDEPTH_9_12
void SATD4x4_2(
    const Ipp16u *pSrc1,
    Ipp32s src1Step,
    const Ipp16u *pSrc2_1,
    const Ipp16u *pSrc2_2,
    Ipp32u* pSATD_1,
    Ipp32u* pSATD_2)
{
    *pSATD_1 = SATD_16u_C1R(pSrc1, src1Step, pSrc2_1, 16, 4, 4);
    *pSATD_2 = SATD_16u_C1R(pSrc1, src1Step, pSrc2_2, 16, 4, 4);
}

void SAD4x4_2(
    const Ipp16u *pSrc1,
    Ipp32s src1Step,
    const Ipp16u *pSrc2_1,
    const Ipp16u *pSrc2_2,
    Ipp32u* pSATD_1,
    Ipp32u* pSATD_2)
{
    *pSATD_1 = SAD4x4(pSrc1, src1Step, pSrc2_1, 16);
    *pSATD_2 = SAD4x4(pSrc1, src1Step, pSrc2_2, 16);
}
#endif

void SATD4x4_2(
    const Ipp8u *pSrc1,
    Ipp32s src1Step,
    const Ipp8u *pSrc2_1,
    const Ipp8u *pSrc2_2,
    Ipp32u* pSATD_1,
    Ipp32u* pSATD_2)
{
    *pSATD_1 = SATD4x4_8u_C1R(pSrc1, src1Step, pSrc2_1, 16);
    *pSATD_2 = SATD4x4_8u_C1R(pSrc1, src1Step, pSrc2_2, 16);
}

void SAD4x4_2(
    const Ipp8u *pSrc1,
    Ipp32s src1Step,
    const Ipp8u *pSrc2_1,
    const Ipp8u *pSrc2_2,
    Ipp32u* pSATD_1,
    Ipp32u* pSATD_2)
{
    *pSATD_1 = SAD4x4(pSrc1, src1Step, pSrc2_1, 16);
    *pSATD_2 = SAD4x4(pSrc1, src1Step, pSrc2_2, 16);
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectOneBlock(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pSrcBlock,     // pointer to upper left pel of source block
    PIXTYPE* pReconBlock,   // pointer to same block in reconstructed picture
    Ipp32u uBlock,          // which 4x4 of the MB (0..15)
    T_AIMode *intra_types,  // selected mode goes here
    PIXTYPE *pPred)        // predictor pels for selected mode goes here
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMinSAD;
    T_AIMode Mode, ModeAbove, ModeLeft, ProbMode;
    __ALIGN16 PIXTYPE uPred[16*NUM_AI_MODES*4]; // predictor pels, all modes, each 4 rows, pitch=16
    PIXTYPE PredPel[13];
    T_AIMode *pMode = &intra_types[uBlock];

    ModeLeft = -1;
    if (BLOCK_IS_ON_LEFT_EDGE(block_subblock_mapping_[uBlock])) {
        H264BlockLocation block = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[block_subblock_mapping_[uBlock] >> 2];
        if (block.mb_num >= 0)
            ModeLeft = core_enc->m_mbinfo.intra_types[block.mb_num].intra_types[block_subblock_mapping_[block.block_num]];
    } else {
        Ipp32s block_num = block_subblock_mapping_[uBlock] - 1; // neighbour in non-raster order
        block_num = block_subblock_mapping_[block_num];
        ModeLeft = intra_types[block_num];
    }
    ModeAbove = -1;
    if (BLOCK_IS_ON_TOP_EDGE(block_subblock_mapping_[uBlock])) {
        H264BlockLocation block = curr_slice->m_cur_mb.BlockNeighbours.mb_above;
        if (block.mb_num >= 0) {
            block.block_num += block_subblock_mapping_[uBlock];
            ModeAbove = core_enc->m_mbinfo.intra_types[block.mb_num].intra_types[block_subblock_mapping_[block.block_num]];
        }
    } else {
        Ipp32s block_num = block_subblock_mapping_[uBlock] - 4; // neighbour in non-raster order
        block_num = block_subblock_mapping_[block_num];
        ModeAbove = intra_types[block_num];
    }

    Ipp32s pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels<<curr_slice->m_is_cur_mb_field;
    PIXTYPE* pLeftRefBlock;
    Ipp32u uLeftPitch;
    PIXTYPE* pAboveRefBlock;
    Ipp32u uAbovePitch;
    PIXTYPE* pAboveLeftRefBlock;
    Ipp32u uAboveLeftPitch;

    if (curr_slice->m_use_transform_for_intra_decision) {
        uLeftPitch = uAbovePitch = uAboveLeftPitch = 16;
        pLeftRefBlock = pAboveRefBlock = pAboveLeftRefBlock = pPred + 256;
        if (ModeAbove == -1 || BLOCK_IS_ON_TOP_EDGE(block_subblock_mapping_[uBlock])) {
            pAboveRefBlock = pReconBlock;
            uAbovePitch = pitchPixels;
            pAboveLeftRefBlock = pReconBlock;
            uAboveLeftPitch = pitchPixels;
        }
        if (BLOCK_IS_ON_LEFT_EDGE(block_subblock_mapping_[uBlock])) {
            pLeftRefBlock = pReconBlock;
            uLeftPitch = pitchPixels;
            pAboveLeftRefBlock = pReconBlock;
            uAboveLeftPitch = pitchPixels;
        }
    } else {
        pLeftRefBlock = pReconBlock;
        uLeftPitch = pitchPixels;
        pAboveRefBlock = pReconBlock;
        uAbovePitch = pitchPixels;
        pAboveLeftRefBlock = pReconBlock;
        uAboveLeftPitch = pitchPixels;
    }
    H264CoreEncoder_GetBlockPredPels(state, curr_slice, pLeftRefBlock, uLeftPitch, pAboveRefBlock, uAbovePitch, pAboveLeftRefBlock, uAboveLeftPitch, uBlock, PredPel);
    Ipp32s iQP = cur_mb.lumaQP51;
    ProbMode = MIN(ModeAbove, ModeLeft);
    if (ProbMode == -1) ProbMode = 2;
    H264CoreEncoder_GetPredBlock(2, &uPred[2*64], PredPel);
    Ipp32s useSAD = core_enc->m_Analyse & ANALYSE_SAD;
    if (useSAD)
        uMinSAD = BITS_COST((ProbMode == 2)? 1:4, glob_RDQM[iQP]) + SAD4x4(pSrcBlock, pitchPixels, &uPred[2*64], 16);
    else
        uMinSAD = BITS_COST((ProbMode == 2)? 1:4, glob_RDQM[iQP]) + SATD4x4(pSrcBlock, pitchPixels, &uPred[2*64], 16);
    *pMode = 2;
    Ipp32s* modes = 0;
    Ipp32s nmodes=0;
    Ipp32u ModeSAD[9];
    if( ( ModeAbove >= 0 ) && ( ModeLeft >= 0 ) ){
        if (!uBlock && (curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num < 0)){
            modes = &intra_modes[0][0];
            nmodes=5;
            //check modes 0,1,2 first
            if (useSAD) {
                for (Ipp32s i=0; i < 3; i++){
                    Mode = modes[i];
                    H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                    ModeSAD[Mode] = SAD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
                }
            }else{
                for (Ipp32s i=0; i < 3; i++){
                    Mode = modes[i];
                    H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                    ModeSAD[Mode] = SATD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
                }
            }
            if( ModeSAD[0] < ModeSAD[1] ){
                ModeSAD[8] = MAX_SAD;
                H264CoreEncoder_GetPredBlock(7, &uPred[7*64], PredPel);
                if (useSAD){
                    ModeSAD[7] = SAD4x4(pSrcBlock, pitchPixels, &uPred[7*64], 16);
                }else{
                    ModeSAD[7] = SATD4x4(pSrcBlock, pitchPixels, &uPred[7*64], 16);
                }
            }else{
                ModeSAD[7] = MAX_SAD;
                H264CoreEncoder_GetPredBlock(8, &uPred[8*64], PredPel);
                if (useSAD){
                    ModeSAD[8] = SAD4x4(pSrcBlock, pitchPixels, &uPred[8*64], 16);
                }else{
                    ModeSAD[8] = SATD4x4(pSrcBlock, pitchPixels, &uPred[8*64], 16);
                }
            }
        }else{
            modes = &intra_modes[1][0];
            nmodes=8;
            //check modes 0,1 first
            if (useSAD){
                //for (Ipp32s i=0; i < 2; i++){
                //     Mode = modes[i];
                //     H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                //     ModeSAD[Mode] = SAD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
                //}
                H264CoreEncoder_GetPredBlock(0, &uPred[0*64], PredPel);
                H264CoreEncoder_GetPredBlock(1, &uPred[1*64], PredPel);
                SAD4x4_2(pSrcBlock, pitchPixels, &uPred[0*64], &uPred[1*64], &ModeSAD[0], &ModeSAD[1]);
            }else{
                //for (Ipp32s i=0; i < 2; i++){
                //    Mode = modes[i];
                //    H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                //    ModeSAD[Mode] = SATD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
                //}
                H264CoreEncoder_GetPredBlock(0, &uPred[0*64], PredPel);
                H264CoreEncoder_GetPredBlock(1, &uPred[1*64], PredPel);
                SATD4x4_2(pSrcBlock, pitchPixels, &uPred[0*64], &uPred[1*64], &ModeSAD[0], &ModeSAD[1]);
            }
            if( ModeSAD[0] < ModeSAD[1] ){
                ModeSAD[6] = ModeSAD[8] = MAX_SAD;
                H264CoreEncoder_GetPredBlock(5, &uPred[5*64], PredPel);
                H264CoreEncoder_GetPredBlock(7, &uPred[7*64], PredPel);
                if (useSAD){
                    //ModeSAD[5] = SAD4x4(pSrcBlock, pitchPixels, &uPred[5*64], 16);
                    //ModeSAD[7] = SAD4x4(pSrcBlock, pitchPixels, &uPred[7*64], 16);
                    SAD4x4_2(pSrcBlock, pitchPixels, &uPred[5*64], &uPred[7*64], &ModeSAD[5], &ModeSAD[7]);
                }else{
                    //ModeSAD[5] = SATD4x4(pSrcBlock, pitchPixels, &uPred[5*64], 16);
                    //ModeSAD[7] = SATD4x4(pSrcBlock, pitchPixels, &uPred[7*64], 16);
                    SATD4x4_2(pSrcBlock, pitchPixels, &uPred[5*64], &uPred[7*64], &ModeSAD[5], &ModeSAD[7]);
                }
            }else{
                ModeSAD[5] = ModeSAD[7] = MAX_SAD;
                H264CoreEncoder_GetPredBlock(6, &uPred[6*64], PredPel);
                H264CoreEncoder_GetPredBlock(8, &uPred[8*64], PredPel);
                if (useSAD){
                    //ModeSAD[6] = SAD4x4(pSrcBlock, pitchPixels, &uPred[6*64], 16);
                    //ModeSAD[8] = SAD4x4(pSrcBlock, pitchPixels, &uPred[8*64], 16);
                    SAD4x4_2(pSrcBlock, pitchPixels, &uPred[6*64], &uPred[8*64], &ModeSAD[6], &ModeSAD[8]);
                }else{
                    //ModeSAD[6] = SATD4x4(pSrcBlock, pitchPixels, &uPred[6*64], 16);
                    //ModeSAD[8] = SATD4x4(pSrcBlock, pitchPixels, &uPred[8*64], 16);
                    SATD4x4_2(pSrcBlock, pitchPixels, &uPred[6*64], &uPred[8*64], &ModeSAD[6], &ModeSAD[8]);
                }
            }

            if( ModeSAD[5] < ModeSAD[7] ||  ModeSAD[6] < ModeSAD[8] ){
                ModeSAD[3] = MAX_SAD;
                H264CoreEncoder_GetPredBlock(4, &uPred[4*64], PredPel);
                if (useSAD){
                    ModeSAD[4] = SAD4x4(pSrcBlock, pitchPixels, &uPred[4*64], 16);
                }else{
                    ModeSAD[4] = SATD4x4(pSrcBlock, pitchPixels, &uPred[4*64], 16);
                }
            }else{
                ModeSAD[4] = MAX_SAD;
                H264CoreEncoder_GetPredBlock(3, &uPred[3*64], PredPel);
                if (useSAD){
                    ModeSAD[3] = SAD4x4(pSrcBlock, pitchPixels, &uPred[3*64], 16);
                }else{
                    ModeSAD[3] = SATD4x4(pSrcBlock, pitchPixels, &uPred[3*64], 16);
                }
            }
        }
    }else if( ModeAbove >= 0){
        modes = &intra_modes[2][0];
        nmodes=3;
        if (useSAD){
           for (Ipp32s i=0; i < nmodes; i++){
                Mode = modes[i];
                H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                ModeSAD[Mode] = SAD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
            }
        }else{
            for (Ipp32s i=0; i < nmodes; i++){
                Mode = modes[i];
                H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                ModeSAD[Mode] = SATD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
            }
        }
    }else if( ModeLeft >= 0 ){
        modes = &intra_modes[3][0];
        nmodes=2;
        if (useSAD){
           for (Ipp32s i=0; i < nmodes; i++){
                Mode = modes[i];
                H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                ModeSAD[Mode] = SAD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
            }
        }else{
            for (Ipp32s i=0; i < nmodes; i++){
                Mode = modes[i];
                H264CoreEncoder_GetPredBlock(Mode, &uPred[Mode*64], PredPel);
                ModeSAD[Mode] = SATD4x4(pSrcBlock, pitchPixels, &uPred[Mode*64], 16);
            }
        }
    }
    if( nmodes ){
        Ipp32s BIT_COST_4 = BITS_COST(4, glob_RDQM[iQP]);
        if( ProbMode != 2 && ModeSAD[ProbMode] != MAX_SAD) ModeSAD[ProbMode] += BITS_COST(1, glob_RDQM[iQP])-BIT_COST_4;
        for(Ipp32s i=0;i<nmodes;i++){
            Mode = modes[i];
            Ipp32u cost = ModeSAD[Mode] + BIT_COST_4;
            if (cost <= uMinSAD) {
                 if( cost != uMinSAD || Mode < *pMode ){ //Prefer mode with less number
                    uMinSAD = cost;
                    *pMode = Mode;
                 }
            }
        }
    }
    if (pPred) {
        Ipp32s i;
        PIXTYPE *pSrc = (PIXTYPE*)&uPred[(*pMode)*64];
        PIXTYPE *pDst = (PIXTYPE*)pPred;
        for(i = 0; i < 4; i++) {
            pDst[0] = pSrc[0];
            pDst[1] = pSrc[1];
            pDst[2] = pSrc[2];
            pDst[3] = pSrc[3];
            pDst += 16;
            pSrc += 16;
        }
        if (curr_slice->m_use_transform_for_intra_decision)
            H264CoreEncoder_Encode4x4IntraBlock(state, curr_slice, uBlock);
    }
    return uMinSAD;
}

void PredictIntraLuma16x16(
    Ipp16u* pRef,
    Ipp32s pitchPixels,
    Ipp16u *pPredBuf,
    Ipp32s bitDepth,
    bool leftAvail,
    bool topAvail,
    bool lefttopAvail)
{
    if (!leftAvail && !topAvail) {
        MemorySet(pPredBuf + 256 * PRED16x16_DC, 1 << (bitDepth - 1), 256);
        return;
    } else if (!leftAvail) {
        Ipp16u *pAbove = pRef - pitchPixels;
        Ipp32s uSum = 0;
        for (int i = 0; i < 16; i ++) {
            memcpy(pPredBuf + 256 * PRED16x16_VERT + i*16, pAbove, 16*sizeof(Ipp16u));
            uSum += pAbove[i];
        }
        uSum = (uSum + 8) >> 4;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp16u)uSum, 256);
        return;
    } else if (!topAvail) {
        Ipp16u *pLeft = pRef - 1;
        Ipp32s uSum = 0;
        for (int i = 0; i < 16; i ++) {
            MemorySet(pPredBuf + 256 * PRED16x16_HORZ + i*16, *pLeft, 16);
            uSum += *pLeft;
            pLeft += pitchPixels;
        }
        uSum = (uSum + 8) >> 4;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp16u)uSum, 256);
        return;
    } else if (!lefttopAvail) {
        Ipp16u *pAbove = pRef - pitchPixels;
        Ipp16u *pLeft = pRef - 1;
        Ipp32s uSum = 0;
        for (int i = 0; i < 16; i ++) {
            memcpy(pPredBuf + 256 * PRED16x16_VERT + i*16, pAbove, 16*sizeof(Ipp16u));
            MemorySet(pPredBuf + 256 * PRED16x16_HORZ + i*16, *pLeft, 16);
            uSum += pAbove[i];
            uSum += *pLeft;
            pLeft += pitchPixels;
        }
        uSum = (uSum + 16) >> 5;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp16u)uSum, 256);
        return;
    } else {
        Ipp32s iH, iV, x, a, b, c;

        Ipp16u *pAbove = pRef - pitchPixels;
        Ipp16u *pLeft = pRef - 1;
        Ipp32s uSum = 0;
        iH = 0;
        iV = 0;
        for (x = 1; x <= 8; x ++) {
            iH += x*((pRef-pitchPixels-1)[8+x] - (pRef-pitchPixels-1)[8-x]);
            iV += x*((pRef+pitchPixels*(8+x-1))[-1] - (pRef+pitchPixels*(8-x-1))[-1]);
        }
        a = ((pRef-pitchPixels-1)[16] + (pRef+pitchPixels*(16-1))[-1])*16;
        b = (5 * iH + 32) >> 6;
        c = (5 * iV + 32) >> 6;
        Ipp32s max_pix_value = (1 << bitDepth) - 1;
        for (int i = 0; i < 16; i ++) {
            memcpy(pPredBuf + 256 * PRED16x16_VERT + i*16, pAbove, 16*sizeof(Ipp16u));
            MemorySet(pPredBuf + 256 * PRED16x16_HORZ + i*16, *pLeft, 16);
            uSum += pAbove[i];
            uSum += *pLeft;
            pLeft += pitchPixels;
            for (int x = 0; x < 16; x ++) {
                Ipp32s temp= (a+(x-7)*b+(i-7)*c+16)>>5;
                temp= (temp>max_pix_value)?max_pix_value:temp;
                temp= (temp<0)?0:temp;
                pPredBuf[256 * PRED16x16_PLANAR + x + i*16]=(Ipp16u)temp;
            }
        }
        uSum = (uSum + 16) >> 5;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp16u)uSum, 256);
        return;
    }
}

void PlanarPredictLuma(
    Ipp16u* pBlock,
    Ipp32u uPitch,
    Ipp16u* pPredBuf,
    Ipp32s bitDepth)
{
    Ipp32s iH, iV, x, y, a, b, c, temp;
    Ipp32s max_pix_value = (1 << bitDepth) - 1;

    iH = 0;
    iV = 0;
    for (x = 1; x <= 8; x ++) {
        iH += x*((pBlock-Ipp32s(uPitch)-1)[8+x] - (pBlock-Ipp32s(uPitch)-1)[8-x]);
        iV += x*((pBlock+Ipp32s(uPitch)*(8+x-1))[-1] - (pBlock+Ipp32s(uPitch)*(8-x-1))[-1]);
    }
    a = ((pBlock-Ipp32s(uPitch)-1)[16] + (pBlock+Ipp32s(uPitch)*(16-1))[-1])*16;
    b = (5 * iH + 32) >> 6;
    c = (5 * iV + 32) >> 6;
    for (y = 0; y < 16; y ++) {
        for (x = 0; x < 16; x ++) {
            temp= (a+(x-7)*b+(y-7)*c+16)>>5;
            temp= (temp>max_pix_value)?max_pix_value:temp;
            temp= (temp<0)?0:temp;
            pPredBuf[x + y*16]=(Ipp16u)temp;
        }
    }
}

void PredictIntraLuma16x16(
    Ipp8u* pRef,
    Ipp32s pitchPixels,
    Ipp8u *pPredBuf,
    Ipp32s bitDepth,
    bool leftAvail,
    bool topAvail,
    bool lefttopAvail)
{
    bitDepth = bitDepth;
    if (!leftAvail && !topAvail) {
#ifndef INTRINSIC_OPT
        MemorySet(pPredBuf + 256 * PRED16x16_DC, 1 << (bitDepth - 1), 256);
#else
        __ALIGN16 __m128i  _p_0;
        _p_0 = _mm_set1_epi8(-128);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  0*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  1*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  2*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  3*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  4*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  5*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  6*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  7*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  8*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  9*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 10*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 11*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 12*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 13*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 14*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 15*16), _p_0);
#endif
        return;
    } else if (!leftAvail) {
        Ipp8u *pAbove = pRef - pitchPixels;
        Ipp32s i, uSum = 0;
#ifndef INTRINSIC_OPT
        for (i = 0; i < 16; i ++) {
            memcpy(pPredBuf + 256 * PRED16x16_VERT + i*16, pAbove, 16*sizeof(Ipp8u));
            uSum += pAbove[i];
        }
        uSum = (uSum + 8) >> 4;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp8u)uSum, 256);
#else
        __ALIGN16 __m128i  _p_0, _p_above;

        _p_above = _mm_load_si128((__m128i*)(pAbove));
        for (i = 0; i < 16; i ++) {
            _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_VERT + i*16), _p_above);
            uSum += pAbove[i];
        }
        uSum = (uSum + 8) >> 4;
        _p_0 = _mm_set1_epi8(uSum);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  0*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  1*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  2*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  3*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  4*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  5*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  6*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  7*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  8*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  9*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 10*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 11*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 12*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 13*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 14*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 15*16), _p_0);
#endif
        return;
    } else if (!topAvail) {
        Ipp8u *pLeft = pRef - 1;
        Ipp32s i, uSum = 0;
#ifndef INTRINSIC_OPT
        for (i = 0; i < 16; i ++) {
            MemorySet(pPredBuf + 256 * PRED16x16_HORZ + i*16, *pLeft, 16);
            uSum += *pLeft;
            pLeft += pitchPixels;
        }
        uSum = (uSum + 8) >> 4;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp8u)uSum, 256);
#else
        __ALIGN16 __m128i  _p_0;
        Ipp32s *pH, j;

        pH = (Ipp32s*)(pPredBuf + 256 * PRED16x16_HORZ);
        for (i = 0; i < 16; i ++) {
            j = *pLeft;
            uSum += *pLeft;
            pLeft += pitchPixels;
            j = j + (j << 8) + (j << 16) + (j << 24);
            pH[0] = pH[1] = pH[2] = pH[3] = j;
            pH += 4;
        }
        uSum = (uSum + 8) >> 4;
        _p_0 = _mm_set1_epi8(uSum);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  0*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  1*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  2*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  3*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  4*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  5*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  6*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  7*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  8*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  9*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 10*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 11*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 12*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 13*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 14*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 15*16), _p_0);
#endif
        return;
    } else if (!lefttopAvail) {
        Ipp8u *pAbove = pRef - pitchPixels;
        Ipp8u *pLeft = pRef - 1;
        Ipp32s uSum = 0;
        for (int i = 0; i < 16; i ++) {
            memcpy(pPredBuf + 256 * PRED16x16_VERT + i*16, pAbove, 16*sizeof(Ipp8u));
            MemorySet(pPredBuf + 256 * PRED16x16_HORZ + i*16, *pLeft, 16);
            uSum += pAbove[i];
            uSum += *pLeft;
            pLeft += pitchPixels;
        }
        uSum = (uSum + 16) >> 5;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp8u)uSum, 256);
        return;
    } else {
        Ipp32s iH, iV, x, a, b, c;

        Ipp8u *pAbove = pRef - pitchPixels;
        Ipp8u *pLeft = pRef - 1;
        Ipp32s uSum = 0;
        iH = 0;
        iV = 0;
        for (x = 1; x <= 8; x ++) {
            iH += x*((pRef-pitchPixels-1)[8+x] - (pRef-pitchPixels-1)[8-x]);
            iV += x*((pRef+pitchPixels*(8+x-1))[-1] - (pRef+pitchPixels*(8-x-1))[-1]);
        }
        a = ((pRef-pitchPixels-1)[16] + (pRef+pitchPixels*(16-1))[-1])*16;
        b = (5 * iH + 32) >> 6;
        c = (5 * iV + 32) >> 6;
#ifndef INTRINSIC_OPT
        Ipp32s max_pix_value = (1 << bitDepth) - 1;
        for (int i = 0; i < 16; i ++) {
            memcpy(pPredBuf + 256 * PRED16x16_VERT + i*16, pAbove, 16*sizeof(Ipp8u));
            MemorySet(pPredBuf + 256 * PRED16x16_HORZ + i*16, *pLeft, 16);
            uSum += pAbove[i];
            uSum += *pLeft;
            pLeft += pitchPixels;
            for (int x = 0; x < 16; x ++) {
                Ipp32s temp= (a+(x-7)*b+(i-7)*c+16)>>5;
                temp= (temp>max_pix_value)?max_pix_value:temp;
                temp= (temp<0)?0:temp;
                pPredBuf[256 * PRED16x16_PLANAR + x + i*16]=(Ipp8u)temp;
            }
        }
        uSum = (uSum + 16) >> 5;
        MemorySet(pPredBuf + 256 * PRED16x16_DC, (Ipp8u)uSum, 256);
#else
        __ALIGN16 __m128i  _p_0, _p_1, _p_c, _p_y_0, _p_y_1, _p_above;
        __ALIGN16 Ipp16s _p_x_0[8] = {-7, -6, -5, -4, -3, -2, -1, 0};
        __ALIGN16 Ipp16s _p_x_1[8] = { 1,  2,  3,  4,  5,  6,  7, 8};
        int  i, j, *pH;
        Ipp8u *pV;

        _p_y_0 = _mm_set1_epi16(a - 7 * c + 16);
        _p_0 = _mm_set1_epi16(b);
        _p_y_1 = _p_y_0;
        _p_1 = _p_0;
        _p_0 = _mm_mullo_epi16(_p_0, *(__m128i*)_p_x_0);
        _p_1 = _mm_mullo_epi16(_p_1, *(__m128i*)_p_x_1);
        _p_c = _mm_set1_epi16(c);
        _p_y_0 = _mm_adds_epi16(_p_y_0, _p_0);
        _p_y_1 = _mm_adds_epi16(_p_y_1, _p_1);
        _p_above = _mm_load_si128((__m128i*)(pAbove));
        pH = (Ipp32s*)(pPredBuf + 256 * PRED16x16_HORZ);
        pV = pPredBuf + 256 * PRED16x16_VERT;
        for (i = 0; i < 16; i ++) {
            _p_0 = _mm_srai_epi16(_p_y_0, 5);
            _p_1 = _mm_srai_epi16(_p_y_1, 5);
            _p_y_0 = _mm_adds_epi16(_p_y_0, _p_c);
            _p_y_1 = _mm_adds_epi16(_p_y_1, _p_c);
            _mm_store_si128((__m128i*)(pV), _p_above);
            pV += 16;
            _p_0 = _mm_packus_epi16(_p_0, _p_1);
            j = *pLeft;
            uSum += *pLeft;
            pLeft += pitchPixels;
            j = j + (j << 8) + (j << 16) + (j << 24);
            pH[0] = pH[1] = pH[2] = pH[3] = j;
            pH += 4;
            uSum += pAbove[i];
            _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_PLANAR + i * 16), _p_0);
        }
        uSum = (uSum + 16) >> 5;
        _p_0 = _mm_set1_epi8(uSum);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  0*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  1*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  2*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  3*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  4*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  5*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  6*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  7*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  8*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC +  9*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 10*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 11*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 12*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 13*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 14*16), _p_0);
        _mm_store_si128((__m128i*)(pPredBuf + 256 * PRED16x16_DC + 15*16), _p_0);
#endif
        return;
    }
}

void PlanarPredictLuma(
    Ipp8u* pBlock,
    Ipp32u uPitch,
    Ipp8u* pPredBuf,
    Ipp32s bitDepth)
{
    Ipp32s iH, iV, x, a, b, c;

    iH = 0;
    iV = 0;
    for (x = 1; x <= 8; x ++) {
        iH += x*((pBlock-Ipp32s(uPitch)-1)[8+x] - (pBlock-Ipp32s(uPitch)-1)[8-x]);
        iV += x*((pBlock+Ipp32s(uPitch)*(8+x-1))[-1] - (pBlock+Ipp32s(uPitch)*(8-x-1))[-1]);
    }
    a = ((pBlock-Ipp32s(uPitch)-1)[16] + (pBlock+Ipp32s(uPitch)*(16-1))[-1])*16;
    b = (5 * iH + 32) >> 6;
    c = (5 * iV + 32) >> 6;
#ifndef INTRINSIC_OPT
    Ipp32s max_pix_value = (1 << bitDepth) - 1;
    for (int y = 0; y < 16; y ++) {
        for (x = 0; x < 16; x ++) {
            Ipp32s temp= (a+(x-7)*b+(y-7)*c+16)>>5;
            temp= (temp>max_pix_value)?max_pix_value:temp;
            temp= (temp<0)?0:temp;
            pPredBuf[x + y*16]=(Ipp8u)temp;
        }
    }
#else
    // disable warning unref param
    bitDepth = bitDepth;
#undef xx
#define xx(d) \
    _p_0 = _mm_srai_epi16(_p_y_0, 5); \
    _p_1 = _mm_srai_epi16(_p_y_1, 5); \
    _p_y_0 = _mm_adds_epi16(_p_y_0, _p_c); \
    _p_y_1 = _mm_adds_epi16(_p_y_1, _p_c); \
    _p_0 = _mm_packus_epi16(_p_0, _p_0); \
    _p_1 = _mm_packus_epi16(_p_1, _p_1); \
    _mm_storel_epi64((__m128i*)(pPredBuf+d*16), _p_0); \
    _mm_storel_epi64((__m128i*)(pPredBuf+d*16+8), _p_1)

    __ALIGN16 __m128i  _p_0, _p_1, _p_6, _p_7, _p_c, _p_y_0, _p_y_1;
    __ALIGN16 Ipp16s _p_x_0[8] = {-7, -6, -5, -4, -3, -2, -1, 0};
    __ALIGN16 Ipp16s _p_x_1[8] = { 1,  2,  3,  4,  5,  6,  7, 8};

    _p_y_0 = _mm_set1_epi16(a - 7 * c + 16);
    _p_6 = _mm_set1_epi16(b);
    _p_y_1 = _p_y_0;
    _p_7 = _p_6;
    _p_6 = _mm_mullo_epi16(_p_6, *(__m128i*)_p_x_0);
    _p_7 = _mm_mullo_epi16(_p_7, *(__m128i*)_p_x_1);
    _p_c = _mm_set1_epi16(c);
    _p_y_0 = _mm_adds_epi16(_p_y_0, _p_6);
    _p_y_1 = _mm_adds_epi16(_p_y_1, _p_7);
//    for (int y = 0; y < 16; y ++) {
//        xx(y);
//    }
    xx(0);
    xx(1);
    xx(2);
    xx(3);
    xx(4);
    xx(5);
    xx(6);
    xx(7);
    xx(8);
    xx(9);
    xx(10);
    xx(11);
    xx(12);
    xx(13);
    xx(14);
    xx(15);
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
//  PlanarPredictChroma [This function conforms with the JVT FCD, but the #if 0'd
//                       version above is probably correct.]
//
//  Find the planar prediction for this 8x8 MB, store at pPredBuf.
//
//  Uses 7 pels from the reconstructed macroblock above and 7 pels from the
//  reconstructed macroblock to the left and 1 pel from the macroblock to the NW
//  to form a 8x8 prediction macroblock.
//
//  L0,a0 a1 a2 a3 -- a5 a6 a7 a8
//  L1    x  x  x  x  x  x  x  x
//  L2    x  x  x  x  x  x  x  x
//  L3    x  x  x  x  x  x  x  x
//  --    x  x  x  x  x  x  x  x
//  L5    x  x  x  x  x  x  x  x
//  L6    x  x  x  x  x  x  x  x
//  L7    x  x  x  x  x  x  x  x
//  L8    x  x  x  x  x  x  x  x
//
//  Above predictors are a0..a3 & a5..a8, left predictors are L0..L3 & L5..L8,
//  x indicates pels to be predicted.
//
//  1. Obtain weighted sum of differences of above predictors:
//   H = 4*(a8-a0) + 3*(a7-a1) + 2*(a6-a2) + 1*(a5-a3)
//  2. Obtain weighted sum of differences of left predictors:
//   V = 4*(L8-L0) + 3*(L7-L1) + 2*(L6-L2) + 1*(L5-L3)
//  3. Calculate a,b,c terms (level, horiz slope factor, vert slope factor):
//   a = (a8 + L8) *16
//   b = (17*H + 16) >> 5
//   c = (17*V + 16) >> 5
//  4. Use a,b,c to calculate a predictor at each i, j of the 8x8 macroblock,
//   where x is horizontal index and y is vertical index:
//   pred = ((a + (i-3)*b + (j-3)*c) + 16)>>5
//
//  Note this function will reference pels above and to the left of the
//  current macroblock so should not be called at top or left picture edge.
//
////////////////////////////////////////////////////////////////////////////////

template<typename PIXTYPE>
void PlanarPredictChroma(
    PIXTYPE* pBlock,
    Ipp32u uPitch,
    PIXTYPE* pPredBuf,
    Ipp32s bitDepth,
    Ipp32s idc)
{
    PIXTYPE ap[17], lp[17];
    Ipp32s iH;
    Ipp32s iV;
    Ipp32s i, j;
    Ipp32s a,b,c;
    Ipp32s temp;
    Ipp32s xCF,yCF;
    PIXTYPE max_pix_value = (1 << bitDepth) - 1;

    xCF = 4*(idc == 3);
    yCF = 4*(idc != 1);
    for (i=0;i<(9+(xCF<<1));i++)
        ap[i]=(pBlock-Ipp32s(uPitch)-1)[i];
    for (i=0;i<(9+(yCF<<1));i++)
        lp[i]=(pBlock+Ipp32s(uPitch)*(i-1))[-1];

    iH = 0;
    iV = 0;

    for (i=1; i<= 4; i++){
        iH += i*(ap[4+xCF+i] - ap[4+xCF-i]);
        iV += i*(lp[4+yCF+i] - lp[4+yCF-i]);
    }
    for( i=5; i<= 4+xCF; i++ )
        iH += i*(ap[4+xCF+i] - ap[4+xCF-i]);
    for( i=5; i<= 4+yCF; i++ )
        iV += i*(lp[4+yCF+i] - lp[4+yCF-i]);

    a = (ap[8+(xCF<<1)] + lp[8+(yCF<<1)])*16;
    b = ((34-29*(idc == 3))*iH + 32)>>6;
    c = ((34-29*(idc != 1))*iV + 32)>>6;

    for (j=0; j<(8+(yCF<<1)); j++)
        for (i=0; i<(8+(xCF<<1)); i++)
        {
            temp= (a+(i-3-xCF)*b+(j-3-yCF)*c+16)>>5;
            temp= (temp>max_pix_value)? max_pix_value:temp;
            temp= (temp<0)?0:temp;
            pPredBuf[i + j*16]=(PIXTYPE) temp;
        }
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectOneMB_16x16(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE *pSrc, PIXTYPE *pRef, Ipp32s pitchPixels, T_AIMode *pMode, PIXTYPE *pPredBuf)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32u uSAD[4], uSmallestSAD;
    __ALIGN16 PIXTYPE uPred[256*4];
    Enum16x16PredType Best16x16Type;
    bool topAvailable = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
    bool leftAvailable = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
    bool left_above_aval = curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
    Ipp32u iQP = cur_mb.lumaQP51;

    PredictIntraLuma16x16(pRef, pitchPixels, uPred, core_enc->m_PicParamSet.bit_depth_luma, leftAvailable, topAvailable, left_above_aval);
    // only for 8 bit
    if(sizeof(PIXTYPE) == 1 && leftAvailable && topAvailable && !(core_enc->m_Analyse & ANALYSE_SAD))
    {
        __ALIGN16 Ipp16s transf[16][16], trans_V[16], trans_H[16];
        Ipp32s x, y;
        Ipp32u satd = 0;
        PIXTYPE *pS = pSrc;

#ifndef INTRINSIC_OPT
        Ipp32s b;
        __ALIGN16 Ipp16s tmpBuff[4][4];
        for (y = 0; y < 16; y += 4) {
            for (x = 0; x < 16; x += 4) {
                for (b = 0; b < 4; b ++) {
                    Ipp32s a01, a23, b01, b23;

                    a01 = pS[b * pitchPixels + x + 0] + pS[b * pitchPixels + x + 1];
                    a23 = pS[b * pitchPixels + x + 2] + pS[b * pitchPixels + x + 3];
                    b01 = pS[b * pitchPixels + x + 0] - pS[b * pitchPixels + x + 1];
                    b23 = pS[b * pitchPixels + x + 2] - pS[b * pitchPixels + x + 3];
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
                    transf[y+0][x+b] = a01 + a23;
                    transf[y+1][x+b] = a01 - a23;
                    transf[y+2][x+b] = b01 - b23;
                    transf[y+3][x+b] = b01 + b23;
                    satd += ABS(a01 + a23) + ABS(a01 - a23) + ABS(b01 - b23) + ABS(b01 + b23);
                }
            }
            pS += 4 * pitchPixels;
        }
#else
        Ipp32s s;
        __ALIGN16 __m128i  _p_0, _p_1, _p_2, _p_3, _p_4, _p_5, _p_6, _p_7, _p_zero;
        __ALIGN16 __m128i  _r_0, _r_1, _r_2, _r_3, _r_5, _r_7, _p_satd;
        static __ALIGN16 Ipp16s _p_one[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        _p_zero = _mm_setzero_si128();
        _p_satd = _mm_setzero_si128();
        for (y = 0; y < 16; y += 4) {
            for (x = 0; x < 16; x += 8) {
                _p_0 = _mm_loadl_epi64((__m128i*)(pS+x));
                _p_1 = _mm_loadl_epi64((__m128i*)(pS+pitchPixels+x));
                _p_0 = _mm_unpacklo_epi8(_p_0, _p_zero);
                _p_1 = _mm_unpacklo_epi8(_p_1, _p_zero);
                _p_2 = _mm_loadl_epi64((__m128i*)(pS+2*pitchPixels+x));
                _p_3 = _mm_loadl_epi64((__m128i*)(pS+3*pitchPixels+x));
                _p_2 = _mm_unpacklo_epi8(_p_2, _p_zero);
                _p_3 = _mm_unpacklo_epi8(_p_3, _p_zero);

                _p_5 = _mm_subs_epi16(_p_0, _p_1);
                _p_0 = _mm_adds_epi16(_p_0, _p_1);
                _p_7 = _mm_subs_epi16(_p_2, _p_3);
                _p_2 = _mm_adds_epi16(_p_2, _p_3);
                _p_1 = _mm_subs_epi16(_p_0, _p_2);
                _p_0 = _mm_adds_epi16(_p_0, _p_2);
                _p_3 = _mm_adds_epi16(_p_5, _p_7);
                _p_5 = _mm_subs_epi16(_p_5, _p_7);

                _r_0 = _mm_unpackhi_epi16(_p_0, _p_1);
                _r_5 = _mm_unpackhi_epi16(_p_5, _p_3);

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

                _mm_storel_epi64((__m128i*)(&transf[y+0][x]), _p_0);
                _p_3 = _mm_srli_si128(_p_0, 8);
                _mm_storel_epi64((__m128i*)(&transf[y+1][x]), _p_3);
                _mm_storel_epi64((__m128i*)(&transf[y+2][x]), _p_2);
                _p_5 = _mm_srli_si128(_p_2, 8);
                _mm_storel_epi64((__m128i*)(&transf[y+3][x]), _p_5);

                _p_3 = _mm_srai_epi16(_p_2, 15);
                _p_1 = _mm_srai_epi16(_p_0, 15);
                _p_2 = _mm_xor_si128(_p_2, _p_3);
                _p_0 = _mm_xor_si128(_p_0, _p_1);
                _p_2 = _mm_sub_epi16(_p_2, _p_3);
                _p_0 = _mm_sub_epi16(_p_0, _p_1);
                _p_0 = _mm_add_epi16(_p_0, _p_2);
                _p_0 = _mm_madd_epi16(_p_0, *(__m128i*)_p_one);
                _p_satd = _mm_add_epi32(_p_satd, _p_0);

                _r_7 = _mm_unpackhi_epi32(_r_0, _r_5);
                _r_0 = _mm_unpacklo_epi32(_r_0, _r_5);
                _r_1 = _mm_srli_si128(_r_0, 8);
                _r_3 = _mm_srli_si128(_r_7, 8);
                _r_5 = _mm_subs_epi16(_r_0, _r_1);
                _r_0 = _mm_adds_epi16(_r_0, _r_1);
                _r_2 = _mm_subs_epi16(_r_7, _r_3);
                _r_7 = _mm_adds_epi16(_r_7, _r_3);
                _r_1 = _mm_subs_epi16(_r_0, _r_7);
                _r_0 = _mm_adds_epi16(_r_0, _r_7);
                _r_3 = _mm_adds_epi16(_r_5, _r_2);
                _r_5 = _mm_subs_epi16(_r_5, _r_2);
                _r_0 = _mm_unpacklo_epi16(_r_0, _r_1);
                _r_5 = _mm_unpacklo_epi16(_r_5, _r_3);
                _r_2 = _mm_unpackhi_epi32(_r_0, _r_5);
                _r_0 = _mm_unpacklo_epi32(_r_0, _r_5);

                _mm_storel_epi64((__m128i*)(&transf[y+0][x+4]), _r_0);
                _r_3 = _mm_srli_si128(_r_0, 8);
                _mm_storel_epi64((__m128i*)(&transf[y+1][x+4]), _r_3);
                _mm_storel_epi64((__m128i*)(&transf[y+2][x+4]), _r_2);
                _r_5 = _mm_srli_si128(_r_2, 8);
                _mm_storel_epi64((__m128i*)(&transf[y+3][x+4]), _r_5);

                _r_3 = _mm_srai_epi16(_r_2, 15);
                _r_1 = _mm_srai_epi16(_r_0, 15);
                _r_2 = _mm_xor_si128(_r_2, _r_3);
                _r_0 = _mm_xor_si128(_r_0, _r_1);
                _r_2 = _mm_sub_epi16(_r_2, _r_3);
                _r_0 = _mm_sub_epi16(_r_0, _r_1);
                _r_0 = _mm_add_epi16(_r_0, _r_2);
                _r_0 = _mm_madd_epi16(_r_0, *(__m128i*)_p_one);
                _p_satd = _mm_add_epi32(_p_satd, _r_0);
            }
            pS += 4 * pitchPixels;
        }
        _p_1 = _mm_srli_si128(_p_satd, 8);
        _p_satd = _mm_add_epi32(_p_satd, _p_1);
        _p_1 = _mm_srli_si128(_p_satd, 4);
        _p_satd = _mm_add_epi32(_p_satd, _p_1);
        satd = _mm_cvtsi128_si32(_p_satd);
#endif
        Ipp32u satd_DC = satd;
        Ipp32s uSum = uPred[256 * PRED16x16_DC] * 16;
        for (y = 0; y < 16; y += 4)
            for (x = 0; x < 16; x += 4)
                satd_DC += ABS(transf[y][x] - uSum) - ABS(transf[y][x]);
        satd_DC >>= 1;
        satd_DC += BITS_COST(3, glob_RDQM[iQP]);

        Ipp32u satd_V = satd;
        for (x = 0; x < 16; x += 4) {
            Ipp32s a01, a23, b01, b23;

            a01 = uPred[256 * PRED16x16_VERT + x + 0] + uPred[256 * PRED16x16_VERT + x + 1];
            a23 = uPred[256 * PRED16x16_VERT + x + 2] + uPred[256 * PRED16x16_VERT + x + 3];
            b01 = uPred[256 * PRED16x16_VERT + x + 0] - uPred[256 * PRED16x16_VERT + x + 1];
            b23 = uPred[256 * PRED16x16_VERT + x + 2] - uPred[256 * PRED16x16_VERT + x + 3];
            trans_V[x + 0] = (a01 + a23) * 4;
            trans_V[x + 1] = (a01 - a23) * 4;
            trans_V[x + 2] = (b01 - b23) * 4;
            trans_V[x + 3] = (b01 + b23) * 4;
        }
#ifndef INTRINSIC_OPT
        for (y = 0; y < 16; y += 4)
            for (x = 0; x < 16; x ++)
                satd_V += ABS(transf[y][x] - trans_V[x]) - ABS(transf[y][x]);
#else
        for (y = 0; y < 16; y += 4) {
            _p_0 = _mm_load_si128((__m128i*)(&transf[y][0]));
            _p_1 = _mm_load_si128((__m128i*)(&transf[y][8]));
            _p_2 = _mm_load_si128((__m128i*)(&transf[y][0]));
            _p_3 = _mm_load_si128((__m128i*)(&transf[y][8]));
            _p_0 = _mm_sub_epi16(_p_0, *(__m128i*)(&trans_V[0]));
            _p_1 = _mm_sub_epi16(_p_1, *(__m128i*)(&trans_V[8]));
            _p_4 = _mm_srai_epi16(_p_2, 15);
            _p_5 = _mm_srai_epi16(_p_3, 15);
            _p_2 = _mm_xor_si128(_p_2, _p_4);
            _p_3 = _mm_xor_si128(_p_3, _p_5);
            _p_2 = _mm_sub_epi16(_p_2, _p_4);
            _p_3 = _mm_sub_epi16(_p_3, _p_5);
            _p_6 = _mm_srai_epi16(_p_0, 15);
            _p_7 = _mm_srai_epi16(_p_1, 15);
            _p_0 = _mm_xor_si128(_p_0, _p_6);
            _p_1 = _mm_xor_si128(_p_1, _p_7);
            _p_0 = _mm_sub_epi16(_p_0, _p_6);
            _p_1 = _mm_sub_epi16(_p_1, _p_7);
            _p_0 = _mm_sub_epi16(_p_0, _p_2);
            _p_1 = _mm_sub_epi16(_p_1, _p_3);
            _p_0 = _mm_madd_epi16(_p_0, *(__m128i*)_p_one);
            _p_1 = _mm_madd_epi16(_p_1, *(__m128i*)_p_one);
            _p_zero = _mm_add_epi32(_p_zero, _p_0);
            _p_zero = _mm_add_epi32(_p_zero, _p_1);
        }
        _p_1 = _mm_srli_si128(_p_zero, 8);
        _p_zero = _mm_add_epi32(_p_zero, _p_1);
        _p_1 = _mm_srli_si128(_p_zero, 4);
        _p_zero = _mm_add_epi32(_p_zero, _p_1);
        s = _mm_cvtsi128_si32(_p_zero);
        satd_V += s;
#endif
        satd_V >>= 1;
        satd_V += BITS_COST(1, glob_RDQM[iQP]);

        Ipp32u satd_H = satd;
        for (y = 0; y < 16; y += 4) {
            Ipp32s a01, a23, b01, b23;

            a01 = uPred[256 * PRED16x16_HORZ + (y + 0) * 16] + uPred[256 * PRED16x16_HORZ + (y + 1) * 16];
            a23 = uPred[256 * PRED16x16_HORZ + (y + 2) * 16] + uPred[256 * PRED16x16_HORZ + (y + 3) * 16];
            b01 = uPred[256 * PRED16x16_HORZ + (y + 0) * 16] - uPred[256 * PRED16x16_HORZ + (y + 1) * 16];
            b23 = uPred[256 * PRED16x16_HORZ + (y + 2) * 16] - uPred[256 * PRED16x16_HORZ + (y + 3) * 16];
            trans_H[y + 0] = (a01 + a23) * 4;
            trans_H[y + 1] = (a01 - a23) * 4;
            trans_H[y + 2] = (b01 - b23) * 4;
            trans_H[y + 3] = (b01 + b23) * 4;
        }
        for (y = 0; y < 16; y ++)
            for (x = 0; x < 16; x += 4)
                satd_H += ABS(transf[y][x] - trans_H[y]) - ABS(transf[y][x]);
        satd_H >>= 1;
        satd_H += BITS_COST(3, glob_RDQM[iQP]);

        uSAD[PRED16x16_VERT] = satd_V;
        uSAD[PRED16x16_HORZ] = satd_H;
        uSAD[PRED16x16_DC] = satd_DC;
        uSAD[PRED16x16_PLANAR] = MAX_SAD;
    }
    else
    {
    if (!leftAvailable && !topAvailable){
        *pMode = PRED16x16_DC;
        memcpy(pPredBuf, uPred + 256 * PRED16x16_DC, 256*sizeof(PIXTYPE));
        if (core_enc->m_Analyse & ANALYSE_SAD)
            return BITS_COST(3, glob_RDQM[iQP]) + SAD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_DC, 16);
        else
            return BITS_COST(3, glob_RDQM[iQP]) + SATD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_DC, 16);
    }
    uSAD[PRED16x16_VERT] = uSAD[PRED16x16_HORZ] = uSAD[PRED16x16_PLANAR] = MAX_SAD;
    if (topAvailable) {
        uSAD[PRED16x16_VERT] =  BITS_COST(1, glob_RDQM[iQP]);
        if (core_enc->m_Analyse & ANALYSE_SAD)
            uSAD[PRED16x16_VERT] += SAD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_VERT, 16);
        else
            uSAD[PRED16x16_VERT] += SATD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_VERT, 16);
    }
    if (leftAvailable) {
        uSAD[PRED16x16_HORZ] =  BITS_COST(3, glob_RDQM[iQP]);
        if (core_enc->m_Analyse & ANALYSE_SAD)
            uSAD[PRED16x16_HORZ] += SAD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_HORZ, 16);
        else
            uSAD[PRED16x16_HORZ] += SATD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_HORZ, 16);
    }
    uSAD[PRED16x16_DC] =  BITS_COST(3, glob_RDQM[iQP]);
    if (core_enc->m_Analyse & ANALYSE_SAD)
        uSAD[PRED16x16_DC] += SAD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_DC, 16);
    else
        uSAD[PRED16x16_DC] += SATD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_DC, 16);
    }
    if (leftAvailable && topAvailable && left_above_aval) {
        uSAD[PRED16x16_PLANAR] =  BITS_COST(5, glob_RDQM[iQP]);
        if (core_enc->m_Analyse & ANALYSE_SAD)
            uSAD[PRED16x16_PLANAR] += SAD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_PLANAR, 16);
        else
            uSAD[PRED16x16_PLANAR] += SATD16x16(pSrc, pitchPixels, uPred + 256 * PRED16x16_PLANAR, 16);
    }
    uSmallestSAD = uSAD[PRED16x16_DC];
    Best16x16Type = PRED16x16_DC;
    if (uSAD[PRED16x16_VERT] < uSmallestSAD) {
        uSmallestSAD = uSAD[PRED16x16_VERT];
        Best16x16Type = PRED16x16_VERT;
    }
    if (uSAD[PRED16x16_HORZ] < uSmallestSAD) {
        uSmallestSAD = uSAD[PRED16x16_HORZ];
        Best16x16Type = PRED16x16_HORZ;
    }
    if (uSAD[PRED16x16_PLANAR] < uSmallestSAD) {
        uSmallestSAD = uSAD[PRED16x16_PLANAR];
        Best16x16Type = PRED16x16_PLANAR;
    }
    memcpy(pPredBuf, uPred + 256 * Best16x16Type, 256*sizeof(PIXTYPE));
    *pMode = (T_AIMode)Best16x16Type;
    return uSmallestSAD;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectChromaMBs_8x8(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pUSrc, PIXTYPE* pURef, PIXTYPE* pVSrc, PIXTYPE* pVRef,
    Ipp32u uPitch, Ipp8u* pMode, PIXTYPE *pUPredBuf, PIXTYPE *pVPredBuf)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;

    if (core_enc->m_PicParamSet.chroma_format_idc == 1)
    {
        H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
        Ipp32u uSAD[4];
        __ALIGN16 PIXTYPE uVertPred[2][64];
        __ALIGN16 PIXTYPE uHorizPred[2][64];
        __ALIGN16 PIXTYPE uDCPred[2][64];
        Ipp32u uSum[2][16] = {{0},{0}};
        Enum8x8PredType Best8x8Type;
        Ipp32u i, uSmallestSAD, idx;
        bool topAvailable = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
        bool leftAvailable = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
        bool left_above_aval = curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
        Ipp32u iQP = cur_mb.chromaQP;
        if (!topAvailable && !leftAvailable) {
            Ipp32s dc = 1 << (core_enc->m_params.bit_depth_chroma - 1);
            PIXTYPE* UPlane = pUPredBuf;
            PIXTYPE* VPlane = pVPredBuf;
            for (i = 0; i < 8; i ++) {
                UPlane[0] = UPlane[1] = UPlane[2] = UPlane[3] = UPlane[4] = UPlane[5] = UPlane[6] = UPlane[7] = dc;
                VPlane[0] = VPlane[1] = VPlane[2] = VPlane[3] = VPlane[4] = VPlane[5] = VPlane[6] = VPlane[7] = dc;
                UPlane += 16;
                VPlane += 16;
            }
            Ipp32s SAD = BITS_COST(1, glob_RDQM[iQP]);
            if (core_enc->m_Analyse & ANALYSE_SAD) {
                SAD += SAD8x8(pUSrc, uPitch, pUPredBuf, 16);
                SAD += SAD8x8(pVSrc, uPitch, pVPredBuf, 16);
            } else {
                SAD += SATD8x8(pUSrc, uPitch, pUPredBuf, 16);
                SAD += SATD8x8(pVSrc, uPitch, pVPredBuf, 16);
            }
            *pMode = PRED8x8_DC;
            return SAD;
        }
        if (!topAvailable)
            uSAD[PRED8x8_VERT] = MAX_SAD;
        else {
            PIXTYPE *pAboveU = pURef - uPitch;
            PIXTYPE *pAboveV = pVRef - uPitch;
            for (i = 0; i < 8; i++) {
                memcpy(&uVertPred[0][i*8], pAboveU, 8*sizeof(PIXTYPE));
                memcpy(&uVertPred[1][i*8], pAboveV, 8*sizeof(PIXTYPE));
                uSum[0][i>>2] += pAboveU[i];
                uSum[1][i>>2] += pAboveV[i];
            }
            uSum[0][0+4] = uSum[0][0];
            uSum[0][1+4] = uSum[0][1];
            uSum[1][0+4] = uSum[1][0];
            uSum[1][1+4] = uSum[1][1];
            uSAD[PRED8x8_VERT] = BITS_COST(3, glob_RDQM[iQP]);
            if (core_enc->m_Analyse & ANALYSE_SAD) {
                uSAD[PRED8x8_VERT] += SAD8x8(pUSrc, uPitch, &uVertPred[0][0], 8);
                uSAD[PRED8x8_VERT] += SAD8x8(pVSrc, uPitch, &uVertPred[1][0], 8);
            } else {
                uSAD[PRED8x8_VERT] += SATD8x8(pUSrc, uPitch, &uVertPred[0][0], 8);
                uSAD[PRED8x8_VERT] += SATD8x8(pVSrc, uPitch, &uVertPred[1][0], 8);
            }
        }
        if (!leftAvailable)
            uSAD[PRED8x8_HORZ] = MAX_SAD;
        else {
            Ipp32u tmpSumU = uSum[0][1];
            Ipp32u tmpSumV = uSum[1][1];
            uSum[0][4] = 0;
            uSum[1][4] = 0;
            PIXTYPE *pLeftU = pURef - 1;
            PIXTYPE *pLeftV = pVRef - 1;
            for (i = 0; i < 8; i++) {
                idx = i & ~0x03;
                uSum[0][idx] += *pLeftU;
                uSum[0][idx+1] += *pLeftU;
                uSum[1][idx] += *pLeftV;
                uSum[1][idx+1] += *pLeftV;
                MemorySet(&uHorizPred[0][i*8], *pLeftU, 8);
                MemorySet(&uHorizPred[1][i*8], *pLeftV, 8);
                pLeftU += uPitch;
                pLeftV += uPitch;
            }
            if (topAvailable) {
                uSum[0][1] = tmpSumU;
                uSum[1][1] = tmpSumV;
            }
            uSAD[PRED8x8_HORZ] = BITS_COST(3, glob_RDQM[iQP]);
            if (core_enc->m_Analyse & ANALYSE_SAD) {
                uSAD[PRED8x8_HORZ] += SAD8x8(pUSrc, uPitch, &uHorizPred[0][0], 8);
                uSAD[PRED8x8_HORZ] += SAD8x8(pVSrc, uPitch, &uHorizPred[1][0], 8);
            } else {
                uSAD[PRED8x8_HORZ] += SATD8x8(pUSrc, uPitch, &uHorizPred[0][0], 8);
                uSAD[PRED8x8_HORZ] += SATD8x8(pVSrc, uPitch, &uHorizPred[1][0], 8);
            }
        }
        if (topAvailable && leftAvailable) {
            // 8 Pixels
            uSum[0][0] = (uSum[0][0] + 4) >> 3;
            uSum[0][5] = (uSum[0][5] + 4) >> 3;
            uSum[1][0] = (uSum[1][0] + 4) >> 3;
            uSum[1][5] = (uSum[1][5] + 4) >> 3;
        } else {
            // 4 pixels
            uSum[0][0] = (uSum[0][0] + 2) >> 2;
            uSum[0][5] = (uSum[0][5] + 2) >> 2;
            uSum[1][0] = (uSum[1][0] + 2) >> 2;
            uSum[1][5] = (uSum[1][5] + 2) >> 2;
        }
        // Always 4 pixels
        uSum[0][1] = (uSum[0][1] + 2) >> 2;
        uSum[0][4] = (uSum[0][4] + 2) >> 2;
        uSum[1][1] = (uSum[1][1] + 2) >> 2;
        uSum[1][4] = (uSum[1][4] + 2) >> 2;
        for (i = 0; i < 4; i ++) {
            uDCPred[0][i*8+ 0] = uDCPred[0][i*8+ 1] = uDCPred[0][i*8+ 2] = uDCPred[0][i*8+ 3] = (PIXTYPE)uSum[0][0];
            uDCPred[0][i*8+ 4] = uDCPred[0][i*8+ 5] = uDCPred[0][i*8+ 6] = uDCPred[0][i*8+ 7] = (PIXTYPE)uSum[0][1];
            uDCPred[0][i*8+32] = uDCPred[0][i*8+33] = uDCPred[0][i*8+34] = uDCPred[0][i*8+35] = (PIXTYPE)uSum[0][4];
            uDCPred[0][i*8+36] = uDCPred[0][i*8+37] = uDCPred[0][i*8+38] = uDCPred[0][i*8+39] = (PIXTYPE)uSum[0][5];
            uDCPred[1][i*8+ 0] = uDCPred[1][i*8+ 1] = uDCPred[1][i*8+ 2] = uDCPred[1][i*8+ 3] = (PIXTYPE)uSum[1][0];
            uDCPred[1][i*8+ 4] = uDCPred[1][i*8+ 5] = uDCPred[1][i*8+ 6] = uDCPred[1][i*8+ 7] = (PIXTYPE)uSum[1][1];
            uDCPred[1][i*8+32] = uDCPred[1][i*8+33] = uDCPred[1][i*8+34] = uDCPred[1][i*8+35] = (PIXTYPE)uSum[1][4];
            uDCPred[1][i*8+36] = uDCPred[1][i*8+37] = uDCPred[1][i*8+38] = uDCPred[1][i*8+39] = (PIXTYPE)uSum[1][5];
        }
        uSAD[PRED8x8_DC] = BITS_COST(1, glob_RDQM[iQP]);
        if (core_enc->m_Analyse & ANALYSE_SAD) {
            uSAD[PRED8x8_DC] += SAD8x8(pUSrc, uPitch, &uDCPred[0][0], 8);
            uSAD[PRED8x8_DC] += SAD8x8(pVSrc, uPitch, &uDCPred[1][0], 8);
        } else {
            uSAD[PRED8x8_DC] += SATD8x8(pUSrc, uPitch, &uDCPred[0][0], 8);
            uSAD[PRED8x8_DC] += SATD8x8(pVSrc, uPitch, &uDCPred[1][0], 8);
        }
        if (topAvailable && leftAvailable && left_above_aval) {
            PIXTYPE apU[9], lpU[9], apV[9], lpV[9];
            Ipp32s iHU, iVU, iHV, iVV, i, j, aU, bU, cU, aV, bV, cV, tempU, tempV;
            PIXTYPE max_pix_value = (1 << core_enc->m_SeqParamSet.bit_depth_chroma) - 1;
            PIXTYPE *pBlockU = pURef, *pBlockV = pVRef;

            for (i = 0; i < 9; i++) {
                apU[i] = (pBlockU-Ipp32s(uPitch)-1)[i];
                apV[i] = (pBlockV-Ipp32s(uPitch)-1)[i];
                lpU[i] = (pBlockU+Ipp32s(uPitch)*(i-1))[-1];
                lpV[i] = (pBlockV+Ipp32s(uPitch)*(i-1))[-1];
            }
            iHU = iVU = iHV = iVV = 0;
            for (i = 1; i <= 4; i++) {
                iHU += i * (apU[4+i] - apU[4-i]);
                iVU += i * (lpU[4+i] - lpU[4-i]);
                iHV += i * (apV[4+i] - apV[4-i]);
                iVV += i * (lpV[4+i] - lpV[4-i]);
            }
            aU = (apU[8] + lpU[8]) * 16;
            bU = (34 * iHU + 32) >> 6;
            cU = (34 * iVU + 32) >> 6;
            aV = (apV[8] + lpV[8]) * 16;
            bV = (34 * iHV + 32) >> 6;
            cV = (34 * iVV + 32) >> 6;
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    tempU = (aU + (i-3) * bU + (j-3) * cU + 16) >> 5;
                    if (tempU > max_pix_value)
                        tempU = max_pix_value;
                    else if (tempU < 0)
                        tempU = 0;
                    pUPredBuf[i+j*16] = (PIXTYPE)tempU;
                    tempV = (aV + (i-3) * bV + (j-3) * cV + 16) >> 5;
                    if (tempV > max_pix_value)
                        tempV = max_pix_value;
                    else if (tempV < 0)
                        tempV = 0;
                    pVPredBuf[i+j*16] = (PIXTYPE)tempV;
                }
            }
            uSAD[PRED8x8_PLANAR] = BITS_COST(5, glob_RDQM[iQP]);
            if (core_enc->m_Analyse & ANALYSE_SAD) {
                uSAD[PRED8x8_PLANAR] += SAD8x8(pUSrc, uPitch, pUPredBuf, 16);
                uSAD[PRED8x8_PLANAR] += SAD8x8(pVSrc, uPitch, pVPredBuf, 16);
            } else {
                uSAD[PRED8x8_PLANAR] += SATD8x8(pUSrc, uPitch, pUPredBuf, 16);
                uSAD[PRED8x8_PLANAR] += SATD8x8(pVSrc, uPitch, pVPredBuf, 16);
            }
        } else
            uSAD[PRED8x8_PLANAR] = MAX_SAD;
        // choose smallest
        uSmallestSAD = uSAD[PRED8x8_DC];
        Best8x8Type = PRED8x8_DC;
        if (uSAD[PRED8x8_VERT] < uSmallestSAD) {
            uSmallestSAD = uSAD[PRED8x8_VERT];
            Best8x8Type = PRED8x8_VERT;
        }
        if (uSAD[PRED8x8_HORZ] < uSmallestSAD) {
            uSmallestSAD = uSAD[PRED8x8_HORZ];
            Best8x8Type = PRED8x8_HORZ;
        }
        if (uSAD[PRED8x8_PLANAR] < uSmallestSAD) {
            uSmallestSAD = uSAD[PRED8x8_PLANAR];
            Best8x8Type = PRED8x8_PLANAR;
        }
        switch (Best8x8Type) {
        case PRED8x8_VERT:
            for (i = 0; i < 8; i ++) {
                memcpy(pUPredBuf+i*16, &uVertPred[0][i*8], 8*sizeof(PIXTYPE));
                memcpy(pVPredBuf+i*16, &uVertPred[1][i*8], 8*sizeof(PIXTYPE));
            }
            break;
        case PRED8x8_HORZ:
            for (i = 0; i < 8; i ++) {
                memcpy(pUPredBuf+i*16, &uHorizPred[0][i*8], 8*sizeof(PIXTYPE));
                memcpy(pVPredBuf+i*16, &uHorizPred[1][i*8], 8*sizeof(PIXTYPE));
            }
            break;
        case PRED8x8_DC:
            for (i = 0; i < 8; i ++) {
                memcpy(pUPredBuf+i*16, &uDCPred[0][i*8], 8*sizeof(PIXTYPE));
                memcpy(pVPredBuf+i*16, &uDCPred[1][i*8], 8*sizeof(PIXTYPE));
            }
            break;
        case PRED8x8_PLANAR:
            //  nothing to do, planar prediction already filled PredBuf
            break;
        default:
            break;
        }
        *pMode = (Ipp8u)Best8x8Type;
        return uSmallestSAD;
    }
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32u uSAD[4]; // MB SAD accumulators
    __ALIGN16 PIXTYPE uVertPred[2][64];   // predictors from above, 4 4x4 Predictor blocks, U & V //first up lines for each block, then line+1 for each block and so on
    __ALIGN16 PIXTYPE uHorizPred[2][64];  // predictors from left, 4 4x4 Predictor blocks, U & V
    __ALIGN16 PIXTYPE uDCPred[2][256];    // DC prediction, 1 8x8 predictor block, U & V
    Ipp32u uSum[2][16] =  {{0},{0}};     // for DC, U & V - 16 predictors (a - d) each
    PIXTYPE *pAbove, *pLeft, *pPred = 0, *pSrcBlock = 0;
    Ipp32u *pCopySrc, *pCopyDst;
    Enum8x8PredType Best8x8Type;
    Ipp32u i, j, uBlock, plane, uSmallestSAD, num_rows, num_cols, wide, blocks,idx;
    bool topAvailable = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
    bool leftAvailable = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
    bool left_above_aval = curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
    Ipp32u iQP = cur_mb.chromaQP;

    wide = ((core_enc->m_PicParamSet.chroma_format_idc-1) & 0x2)>>1;
    num_cols = (wide+1)<<3;
    num_rows = (((core_enc->m_PicParamSet.chroma_format_idc&0x2)>>1)+1)<<3;
    blocks = 2<<core_enc->m_PicParamSet.chroma_format_idc;

    if (!topAvailable && !leftAvailable) {
        Ipp32s dc = 1<<(core_enc->m_params.bit_depth_chroma - 1);
        PIXTYPE* UPlane = pUPredBuf;
        PIXTYPE* VPlane = pVPredBuf;
        for( i = 0; i < num_rows; ++i ){
            MemorySet(UPlane, dc, 8);  // Fill the block, both Planes
            MemorySet(VPlane, dc, 8);  // Fill the block, both Planes
            UPlane += 16;
            VPlane += 16;
        }
        Ipp32s SAD = BITS_COST(1, glob_RDQM[iQP]);
        if (core_enc->m_Analyse & ANALYSE_SAD){
            SAD += SAD8x8(pUSrc, uPitch, pUPredBuf, 16);
            SAD += SAD8x8(pVSrc, uPitch, pVPredBuf, 16);
            if( core_enc->m_PicParamSet.chroma_format_idc == 2 ){
                SAD += SAD8x8(pUSrc+uPitch*8, uPitch, pUPredBuf, 16);
                SAD += SAD8x8(pVSrc+uPitch*8, uPitch, pVPredBuf, 16);
            }
        }else{
            SAD += SATD8x8(pUSrc, uPitch, pUPredBuf, 16);
            SAD += SATD8x8(pVSrc, uPitch, pVPredBuf, 16);
            if( core_enc->m_PicParamSet.chroma_format_idc == 2 ){
                SAD += SATD8x8(pUSrc+uPitch*8, uPitch, pUPredBuf, 16);
                SAD += SATD8x8(pVSrc+uPitch*8, uPitch, pVPredBuf, 16);
            }
        }
         *pMode = PRED8x8_DC;
         return SAD;
    }

    // Initialize uVertPred with prediction from above
    if (!topAvailable)
        uSAD[PRED8x8_VERT] = MAX_SAD;
    else {
        uSAD[PRED8x8_VERT] = BITS_COST(3, glob_RDQM[iQP]);
        for (plane = 0; plane < 2; plane++) {
            pAbove = plane ? pVRef - uPitch : pURef - uPitch;
            // Get predictors from above and copy into 4x4 blocks for SAD calculations
            for (i=0; i<num_cols; i++) {
                uVertPred[plane][((i>>2)<<4)+(i&0x3)] = pAbove[i]; //fill the up row
                uSum[plane][i>>2] += pAbove[i];     // accumulate to A & B for DC predictor
            }
            //Copy down
            if( core_enc->m_PicParamSet.chroma_format_idc == 1){
               for( i=0; i<2; i++ )
                 uSum[plane][i+4] = uSum[plane][i];
            }else{
               for( i=0; i<(num_cols>>2); i++ )
                 uSum[plane][i+4] = uSum[plane][i+8] = uSum[plane][i+12] = uSum[plane][i];
            }
            for (i=0; i < (num_cols>>2); ++i) {
                memcpy(&uVertPred[plane][i*16+4],&uVertPred[plane][i*16],4*sizeof(PIXTYPE));
                memcpy(&uVertPred[plane][i*16+8],&uVertPred[plane][i*16],4*sizeof(PIXTYPE));
                memcpy(&uVertPred[plane][i*16+12],&uVertPred[plane][i*16],4*sizeof(PIXTYPE));
            }
        }
    }

    // Initialize uHorizPred with prediction from left
    if (!leftAvailable)
        uSAD[PRED8x8_HORZ] = MAX_SAD;
    else {
        uSAD[PRED8x8_HORZ] = BITS_COST(3, glob_RDQM[iQP]);
        for (plane = 0; plane < 2; plane++) {
            Ipp32u tmpSum = uSum[plane][1];
            uSum[plane][4] = 0;     // Reset Block C to zero in this case.
            if( core_enc->m_PicParamSet.chroma_format_idc == 2){
                    uSum[plane][8] = 0;
                    uSum[plane][12] = 0;
            }
            pLeft = plane ? pVRef - 1 : pURef - 1;
            // Get predictors from the left and copy into 4x4 blocks for SAD calculations
            for (i=0; i<num_rows; i++)
            {
                idx = i & ~0x03; //Reset low 2 bits
                uSum[plane][idx] += *pLeft;    // accumulate to A or C for DC predictor
                uSum[plane][idx+1] += *pLeft;  // accumulate to B or D for DC predictor
                MemorySet(&uHorizPred[plane][i<<2], *pLeft, 4);
                if( core_enc->m_PicParamSet.chroma_format_idc == 3 ){
                    uSum[plane][idx+2] += *pLeft;
                    uSum[plane][idx+3] += *pLeft;
                }
                pLeft += uPitch;
            }
            if (topAvailable){                // Conditionally restore the previous sum
                uSum[plane][1] = tmpSum;    // unless this is on the top edge
            }
        }
    }
    // DC prediction, store predictor in all 64 pels of the predictor block for each plane.
    uSAD[PRED8x8_DC] = BITS_COST(1, glob_RDQM[iQP]);
        for (plane = 0; plane < 2; plane++) {
            // Divide & round A & D properly, depending on how many terms are in the sum.
            if (topAvailable && leftAvailable) {
                // 8 Pixels
                uSum[plane][0] = (uSum[plane][0] + 4) >> 3;
                uSum[plane][5] = (uSum[plane][5] + 4) >> 3;
            } else {
                // 4 pixels
                uSum[plane][0] = (uSum[plane][0] + 2) >> 2;
                uSum[plane][5] = (uSum[plane][5] + 2) >> 2;
            }
            // Always 4 pixels
            uSum[plane][1] = (uSum[plane][1] + 2) >> 2;
            uSum[plane][4] = (uSum[plane][4] + 2) >> 2;
            if( core_enc->m_PicParamSet.chroma_format_idc == 2){
                    uSum[plane][8] = (uSum[plane][8] + 2) >> 2;
                    uSum[plane][12] = (uSum[plane][12] + 2) >> 2;
                    if (topAvailable && leftAvailable) {
                        uSum[plane][9] = (uSum[plane][9] + 4) >> 3;
                        uSum[plane][13] = (uSum[plane][13] + 4) >> 3;
                    }else{
                        uSum[plane][9] = (uSum[plane][9] + 2) >> 2;
                        uSum[plane][13] = (uSum[plane][13] + 2) >> 2;
                    }
            }
            // Fill the correct pixel values into the uDCPred buffer
            if( core_enc->m_PicParamSet.chroma_format_idc == 3 ){
                for( j = 0; j<blocks; j++ )
                    MemorySet(&uDCPred[plane][j*16], (PIXTYPE)uSum[plane][j], 16);
            }else{
                for( j = 0; j<blocks; j++ )
                   MemorySet(&uDCPred[plane][j*16], (PIXTYPE)uSum[plane][((j>>1)<<2) + (j&0x01)], 16);
            }
        }

    // Get planar prediction, save 8x8 PIXTYPE result at pPredBuf,
    if (topAvailable && leftAvailable && left_above_aval) {
        PlanarPredictChroma(pURef, uPitch, pUPredBuf, core_enc->m_params.bit_depth_chroma, core_enc->m_PicParamSet.chroma_format_idc);
        PlanarPredictChroma(pVRef, uPitch, pVPredBuf, core_enc->m_params.bit_depth_chroma, core_enc->m_PicParamSet.chroma_format_idc);
        uSAD[PRED8x8_PLANAR] = BITS_COST(5, glob_RDQM[iQP]);
    } else
        uSAD[PRED8x8_PLANAR] = MAX_SAD;

    // Mode select: Loop through all chroma blocks, accumulate a MB SAD for each mode.
    if (core_enc->m_Analyse & ANALYSE_SAD){
        if (topAvailable && leftAvailable && left_above_aval){
            for (plane=0; plane<2; plane++) {
                for (uBlock=0; uBlock<blocks; uBlock++) {
                  if ((uBlock & 1) == 0) {
                        // init pPlanarPred for new row of blocks
                        pPred = plane ? pVPredBuf + uBlock*32 : pUPredBuf + uBlock*32;
                        pSrcBlock = plane ? pVSrc + (uBlock<<1)*uPitch : pUSrc + (uBlock<<1)*uPitch;
                    }
                    uSAD[PRED8x8_DC] += SAD4x4(pSrcBlock, uPitch, &uDCPred[plane][uBlock<<4], 4);
                    uSAD[PRED8x8_VERT] += SAD4x4(pSrcBlock, uPitch, &uVertPred[plane][(uBlock&1)<<4], 4);
                    uSAD[PRED8x8_HORZ] += SAD4x4(pSrcBlock, uPitch, &uHorizPred[plane][(uBlock>>1)<<4], 4);
                    uSAD[PRED8x8_PLANAR] += SAD4x4(pSrcBlock, uPitch, pPred, 16);
                    // next block
                    pSrcBlock += 4;
                    pPred += 4;
                }
            }
        }else{
            for (plane=0; plane<2; plane++) {
                for (uBlock=0; uBlock<blocks; uBlock++) {
                  if ((uBlock & 1) == 0) {
                        // init pPlanarPred for new row of blocks
                        pPred = plane ? pVPredBuf + uBlock*32 : pUPredBuf + uBlock*32;
                        pSrcBlock = plane ? pVSrc + (uBlock<<1)*uPitch : pUSrc + (uBlock<<1)*uPitch;
                    }
                    uSAD[PRED8x8_DC] += SAD4x4(pSrcBlock, uPitch, &uDCPred[plane][uBlock<<4], 4);
                    if (topAvailable)
                        uSAD[PRED8x8_VERT] += SAD4x4(pSrcBlock, uPitch, &uVertPred[plane][(uBlock&1)<<4], 4);
                    if (leftAvailable)
                        uSAD[PRED8x8_HORZ] += SAD4x4(pSrcBlock, uPitch, &uHorizPred[plane][(uBlock>>1)<<4], 4);
                    // next block
                    pSrcBlock += 4;
                    pPred += 4;
                }
            }
        }
    }else{
        if (topAvailable && leftAvailable && left_above_aval){
            for (plane=0; plane<2; plane++) {
                for (uBlock=0; uBlock<blocks; uBlock++) {
                    if ((uBlock & 1) == 0) {
                        // init pPlanarPred for new row of blocks
                        pPred = plane ? pVPredBuf + uBlock*32 : pUPredBuf + uBlock*32;
                        pSrcBlock = plane ? pVSrc + (uBlock<<1)*uPitch : pUSrc + (uBlock<<1)*uPitch;
                    }
                    uSAD[PRED8x8_DC] += SATD4x4(pSrcBlock, uPitch, &uDCPred[plane][uBlock<<4], 4);
                    uSAD[PRED8x8_VERT] += SATD4x4(pSrcBlock, uPitch, &uVertPred[plane][(uBlock&1)<<4], 4);
                    uSAD[PRED8x8_HORZ] += SATD4x4(pSrcBlock, uPitch, &uHorizPred[plane][(uBlock>>1)<<4], 4);
                    uSAD[PRED8x8_PLANAR] += SATD4x4(pSrcBlock, uPitch, pPred, 16);
                    // next block
                    pSrcBlock += 4;
                    pPred += 4;
                }
            }
        }else{
            for (plane=0; plane<2; plane++) {
                for (uBlock=0; uBlock<blocks; uBlock++) {
                    if ((uBlock & 1) == 0) {
                        // init pPlanarPred for new row of blocks
                        pPred = plane ? pVPredBuf + uBlock*32 : pUPredBuf + uBlock*32;
                        pSrcBlock = plane ? pVSrc + (uBlock<<1)*uPitch : pUSrc + (uBlock<<1)*uPitch;
                    }
                    uSAD[PRED8x8_DC] += SATD4x4(pSrcBlock, uPitch, &uDCPred[plane][uBlock<<4], 4);
                    if (topAvailable)
                        uSAD[PRED8x8_VERT] += SATD4x4(pSrcBlock, uPitch, &uVertPred[plane][(uBlock&1)<<4], 4);
                    if (leftAvailable)
                        uSAD[PRED8x8_HORZ] += SATD4x4(pSrcBlock, uPitch, &uHorizPred[plane][(uBlock>>1)<<4], 4);
                    // next block
                    pSrcBlock += 4;
                    pPred += 4;
                }
            }
        }
    }
    // choose smallest
    uSmallestSAD = uSAD[PRED8x8_DC];
    Best8x8Type = PRED8x8_DC;
    if (uSAD[PRED8x8_VERT] < uSmallestSAD) {
        uSmallestSAD = uSAD[PRED8x8_VERT];
        Best8x8Type = PRED8x8_VERT;
    }
    if (uSAD[PRED8x8_HORZ] < uSmallestSAD) {
        uSmallestSAD = uSAD[PRED8x8_HORZ];
        Best8x8Type = PRED8x8_HORZ;
    }
    if (uSAD[PRED8x8_PLANAR] < uSmallestSAD) {
        uSmallestSAD = uSAD[PRED8x8_PLANAR];
        Best8x8Type = PRED8x8_PLANAR;
    }
    // Set MB type for smallest, fill PredBuf with predictors
    switch (Best8x8Type) {
    case PRED8x8_VERT:
        for (plane=0; plane<2; plane++) {
            // Prediction from above, fill rows of prediction blocks with the corresponding pels from above.
            pCopySrc = (Ipp32u *)uVertPred[plane];
            pCopyDst = plane ? (Ipp32u *)pVPredBuf : (Ipp32u *)pUPredBuf;

            for (i=0; i<num_rows; i++) // Rewritten for 16 bits.
            {
                Ipp32u *srcPtr = pCopySrc;
                Ipp32u *dstPtr = pCopyDst;
                for(j = 0; j < sizeof(PIXTYPE); j++) {
                    *dstPtr = *srcPtr;
                    *(dstPtr+sizeof(PIXTYPE)) = *(srcPtr + 4*sizeof(PIXTYPE));
                    if(core_enc->m_PicParamSet.chroma_format_idc == 3) {
                        *(dstPtr + 2*sizeof(PIXTYPE)) = *(pCopySrc + 8*sizeof(PIXTYPE));
                        *(dstPtr + 3*sizeof(PIXTYPE)) = *(pCopySrc + 12*sizeof(PIXTYPE));
                    }
                    dstPtr++;
                    srcPtr++;
                }
                pCopyDst += 4*sizeof(PIXTYPE);
            }
        }
        break;
    case PRED8x8_HORZ:
        for (plane=0; plane<2; plane++) {
            // Prediction from left. Fill rows of prediction blocks with the corresponding pels from the left.
            pCopySrc = (Ipp32u *)uHorizPred[plane];
            pCopyDst = plane ? (Ipp32u *)pVPredBuf : (Ipp32u *)pUPredBuf;

            for (i=0; i<num_rows; i++)  // Rewritten for 16 bits.
            {
                Ipp32u *dstPtr = pCopyDst;
                Ipp32u *srcPtr = pCopySrc;
                for(Ipp32u j = 0; j < sizeof(PIXTYPE); j++) {
                    *dstPtr = *(dstPtr + sizeof(PIXTYPE)) = *srcPtr;
                    dstPtr++;
                    srcPtr++;
                }
                pCopySrc += sizeof(PIXTYPE);
                pCopyDst += 4*sizeof(PIXTYPE);                  // pitch is 16*sizeof(PIXTYPE)
            }
        }
        break;
    case PRED8x8_DC:
        for (plane=0; plane<2; plane++) {
            // top row
            pPred = plane ? pVPredBuf : pUPredBuf;
            // Copy 8 rows
            pCopySrc = (Ipp32u *)&uDCPred[plane][0]; // Rewrite for 16 bits.
            pCopyDst = (Ipp32u *)pPred;
            for (i=0; i<num_rows; i++)
            {
                Ipp32u *srcPtr = pCopySrc;
                Ipp32u *dstPtr = pCopyDst;
                for(Ipp32u j = 0; j < sizeof(PIXTYPE); j++) {
                    *dstPtr = *srcPtr;
                    *(dstPtr + sizeof(PIXTYPE)) = *(srcPtr + 4*sizeof(PIXTYPE));
                    dstPtr++;
                    srcPtr++;
                }
                if((i & 0x03) == 0x03) pCopySrc += 5*sizeof(PIXTYPE);
                else                   pCopySrc +=   sizeof(PIXTYPE);
                pCopyDst += 4*sizeof(PIXTYPE);
            }
        }
        break;
    case PRED8x8_PLANAR:
        //  nothing to do, planar prediction already filled PredBuf
        break;
    default:
        VM_ASSERT( 0 /*Can't find the best intra prediction 8x8!!!*/);
        break;
    }
    *pMode = (Ipp8u)Best8x8Type;

    return uSmallestSAD;
}

////////////////////////////////////////////////////////////////////////////////
//
// AIModeSelectOneMB_8x8
//
// Choose the best advanced intra mode for coding the MB, return at
// *pMode. Also return the SAD for the chosen mode. Also
// save predictor pels for the MB in provided buffer.
//
////////////////////////////////////////////////////////////////////////////////

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_AIModeSelectOneMB_8x8(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pSrc,            // pointer to upper left pel of source MB
    PIXTYPE* pRef,            // pointer to same MB in reference picture
    Ipp32s   uBlock,          // 8x8 block number
    T_AIMode* pMode,          // selected mode goes here
    PIXTYPE* pPredBuf)        // predictor pels for selected mode goes here
                              //pPredBuf should be not equal to 0 if use_transform_for_intra_decision is set
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    PIXTYPE pred_pels[25]; //Sources for prediction
    Ipp32u pred_pels_mask;
    Ipp32u best_SAD;
    Ipp32s i, n;
    bool dcPredModePredictedFlag = false;
    T_AIMode intra_pred_mode_A, intra_pred_mode_B, intra_pred_mode, intra_actual_mode, mode;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;

    /* First fill out pred_pels
    ** pred_pels
    ** [0] - top-left
    ** [1..16] - top 16th values
    ** [17..24] - left 8-th values
    */

    pred_pels_mask=0; //all is not available; 25 bits used
    bool top_avbl;
    bool left_avbl;
    bool left_above_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
    bool right_above_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above_right.mb_num >= 0;
    //bool right_avbl = curr_slice->m_cur_mb.BlockNeighbours..mb_num >= 0;
    H264BlockLocation mb_ALoc;
    H264BlockLocation mb_BLoc;
    H264MacroblockGlobalInfo mb_A;
    H264MacroblockGlobalInfo mb_B;

    //Check availability
    if( uBlock & 0x2 ){
        top_avbl = true;
        mb_BLoc.block_num = uBlock - 2;
        mb_BLoc.mb_num = cur_mb.uMB;
        mb_B = *(cur_mb.GlobalMacroblockInfo);
    }else{
        top_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
        mb_BLoc = cur_mb.BlockNeighbours.mb_above;
        mb_BLoc.block_num = uBlock + 2;
        mb_B = core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb_BLoc.mb_num];
    }

    if( uBlock & 0x1 ){
        left_avbl = true;
        mb_ALoc.block_num = uBlock - 1;
        mb_ALoc.mb_num = cur_mb.uMB;
        mb_A = *(cur_mb.GlobalMacroblockInfo);
    }else{
        left_avbl = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
        mb_ALoc = cur_mb.BlockNeighbours.mbs_left[0];
        mb_ALoc.block_num = uBlock + 1;
        mb_A = core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb_ALoc.mb_num];
    }

    if (!top_avbl){
        dcPredModePredictedFlag = true;
        //Reset mask
    }else if( mb_B.mbtype < 8 && mb_B.mbtype > 2 && core_enc->m_PicParamSet.constrained_intra_pred_flag )
            dcPredModePredictedFlag = true;

    if(!left_avbl){
        dcPredModePredictedFlag = true;
    }else if( mb_A.mbtype < 8 && mb_A.mbtype > 2 && core_enc->m_PicParamSet.constrained_intra_pred_flag )
        dcPredModePredictedFlag = true;

    PIXTYPE* pLeftRef;
    Ipp32u uLeftPitch;
    PIXTYPE* pAboveRef;
    Ipp32u uAbovePitch;
    PIXTYPE* pAboveLeftRef;
    Ipp32u uAboveLeftPitch;
    PIXTYPE* pAboveRightRef;
    Ipp32u uAboveRightPitch;

    if (curr_slice->m_use_transform_for_intra_decision){

        uLeftPitch =  uAbovePitch = uAboveLeftPitch = uAboveRightPitch = 16;
        pLeftRef = pAboveRef = pAboveLeftRef = pAboveRightRef = pPredBuf + 256;

        if ( !(uBlock & 0x2) ) //block on top edge
        {
            pAboveRef = pRef;
            uAbovePitch = pitchPixels;
            pAboveLeftRef = pRef;
            uAboveLeftPitch = pitchPixels;
            pAboveRightRef = pRef;
            uAboveRightPitch = pitchPixels;
        }

        if ( !(uBlock & 0x1) ) //block on left edge
        {
            pLeftRef = pRef;
            uLeftPitch = pitchPixels;
            pAboveLeftRef = pRef;
            uAboveLeftPitch = pitchPixels;
        }
    } else {
        pLeftRef = pRef;
        uLeftPitch = pitchPixels;
        pAboveRef = pRef;
        uAbovePitch = pitchPixels;
        pAboveLeftRef = pRef;
        uAboveLeftPitch = pitchPixels;
        pAboveRightRef = pRef;
        uAboveRightPitch = pitchPixels;
    }

    //Copy pels
    //TOP
    if( top_avbl ){
        for( i=0; i<8; i++ )
            pred_pels[1+i] = *(pAboveRef-uAbovePitch+i);
        pred_pels_mask |= 0x000001fe;
    }

    //LEFT
    if( left_avbl ){
        for( i=0; i<8; i++ )
            pred_pels[17+i] = *(pLeftRef+i*uLeftPitch-1);
            pred_pels_mask |= 0x1fe0000;
    }

    //LEFT_ABOVE
    if( (uBlock == 0 && left_above_avbl) || uBlock == 3 ||
        (uBlock == 1 && top_avbl) || ( uBlock == 2 && left_avbl)){
        pred_pels[0] = *(pAboveLeftRef-uAboveLeftPitch-1);
        pred_pels_mask |= 0x01;
    }

    //RIGHT_ABOVE
    if( (uBlock == 2) || (uBlock == 0 && top_avbl) ||
        (uBlock == 1 && right_above_avbl) ){
        for( i=0; i<8; i++ )
            pred_pels[9+i] = *(pAboveRightRef-uAboveRightPitch+i+8);
        pred_pels_mask |= 0x0001fe00;
    }

    if( !((pred_pels_mask & 0x0001FE00)==0x0001FE00) && (pred_pels_mask & 0x00000100) ){
        pred_pels_mask |= 0x0001FE00;
        for( i=0; i<8; i++ ) pred_pels[9+i] = pred_pels[1+7];
    }

    H264CoreEncoder_Filter8x8Pels(pred_pels, pred_pels_mask);

    if( dcPredModePredictedFlag ){
        intra_pred_mode_A = intra_pred_mode_B = 2; //Check only DC mode
    }else{
        //Derive modes according previously coded macroblocks
        if( mb_A.mbtype != MBTYPE_INTRA ) intra_pred_mode_A = 2;
        else{
            if( pGetMB8x8TSFlag(&mb_A)){
                intra_pred_mode_A = core_enc->m_mbinfo.intra_types[mb_ALoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[mb_ALoc.block_num<<2]]];
            }else{
                if( uBlock != 2 || !core_enc->m_SliceHeader.MbaffFrameFlag  || curr_slice->m_is_cur_mb_field ) //TODO Add frame coded mb_A check
                    n=1;
                else n=3;
                intra_pred_mode_A = core_enc->m_mbinfo.intra_types[mb_ALoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[(mb_ALoc.block_num<<2)+n]]];
            }
        }

        if( mb_B.mbtype != MBTYPE_INTRA ) intra_pred_mode_B = 2;
        else{
            if( pGetMB8x8TSFlag(&mb_B)){
                intra_pred_mode_B = core_enc->m_mbinfo.intra_types[mb_BLoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[mb_BLoc.block_num<<2]]];
            }else{
                intra_pred_mode_B = core_enc->m_mbinfo.intra_types[mb_BLoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[(mb_BLoc.block_num<<2)+2]]];
            }
        }
    }

    intra_pred_mode = MIN( intra_pred_mode_A, intra_pred_mode_B );

    //Check all prediction modes for the current MB and take the one with the best SAD
    PIXTYPE uPred[9][64];
    if( intra_pred_mode < 0 ) intra_pred_mode = 2; //use DC
    Ipp32s  iQP = cur_mb.lumaQP51;

    //Make prediction, check all possible modes
    Ipp32s* modes = 0;
    Ipp32s nmodes=0;

    //DC mode prediction, always available
     H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 2, pred_pels, pred_pels_mask, uPred[2] );
     if (core_enc->m_Analyse & ANALYSE_SAD)
        best_SAD = BITS_COST((intra_pred_mode == 2)? 1:4, glob_RDQM[iQP]) + SAD8x8(pSrc, pitchPixels, uPred[2], 8);
     else
        best_SAD = BITS_COST((intra_pred_mode == 2)? 1:4, glob_RDQM[iQP]) + SAT8x8D(pSrc, pitchPixels, uPred[2], 8);
     intra_actual_mode = 2;

/*
    if( ( top_avbl ) && ( left_avbl ) ){
        if (!uBlock && (curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num < 0)){
            modes = &intra_modes[0][0];
            nmodes=5;
        }else{
            modes = &intra_modes[1][0];
            nmodes=8;
        }
    }else if( top_avbl ){
        modes = &intra_modes[2][0];
        nmodes=3;
    }else if( left_avbl ){
        modes = &intra_modes[3][0];
        nmodes=2;
    }

    if( nmodes ){
        Ipp32u ModeSAD[9];
        if (core_enc->m_Analyse & ANALYSE_SAD){
            for( i=0; i<nmodes; i++ ){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8(state, mode, pred_pels, pred_pels_mask, uPred[mode] );
                ModeSAD[mode] = BITS_COST(4, glob_RDQM[iQP]) + SAD8x8(pSrc, pitchPixels, uPred[mode], 8);
            }
        }else{
            for( i=0; i<nmodes; i++ ){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8(state, mode, pred_pels, pred_pels_mask, uPred[mode] );
                ModeSAD[mode] = BITS_COST(4, glob_RDQM[iQP]) + SAT8x8D(pSrc, pitchPixels, uPred[mode], 8);
            }
        }
*/
    Ipp32u ModeSAD[9];
    if( top_avbl && left_avbl ){
        if (!uBlock && (curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num < 0)){
            modes = &intra_modes[0][0];
            nmodes=5;
            //check modes 0,1,2 first
            if (core_enc->m_Analyse & ANALYSE_SAD){
               for (Ipp32s i=0; i < 3; i++){
                    mode = modes[i];
                    H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                    ModeSAD[mode] = SAD8x8(pSrc, pitchPixels, uPred[mode], 8);
                }
            }else{
                for (Ipp32s i=0; i < 3; i++){
                    mode = modes[i];
                    H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                    ModeSAD[mode] = SAT8x8D(pSrc, pitchPixels, uPred[mode], 8);
                }
            }
            if( ModeSAD[0] < ModeSAD[1] ){
                ModeSAD[8] = MAX_SAD;
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 7, pred_pels, pred_pels_mask, uPred[7]);
                if (core_enc->m_Analyse & ANALYSE_SAD){
                    ModeSAD[7] = SAD8x8(pSrc, pitchPixels, uPred[7], 8);
                }else{
                    ModeSAD[7] = SAT8x8D(pSrc, pitchPixels, uPred[7], 8);
                }
            }else{
                ModeSAD[7] = MAX_SAD;
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 8, pred_pels, pred_pels_mask, uPred[8]);
                if (core_enc->m_Analyse & ANALYSE_SAD){
                    ModeSAD[8] = SAD8x8(pSrc, pitchPixels, uPred[8], 8);
                }else{
                    ModeSAD[8] = SAT8x8D(pSrc, pitchPixels, uPred[8], 8);
                }
            }
        }else{
            modes = &intra_modes[1][0];
            nmodes=8;
            //check modes 0,1 first
            if (core_enc->m_Analyse & ANALYSE_SAD){
               for (Ipp32s i=0; i < 2; i++){
                    mode = modes[i];
                    H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                    ModeSAD[mode] = SAD8x8(pSrc, pitchPixels, uPred[mode], 8);
                }
            }else{
                for (Ipp32s i=0; i < 2; i++){
                    mode = modes[i];
                    H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                    ModeSAD[mode] = SAT8x8D(pSrc, pitchPixels, uPred[mode], 8);
                }
            }
            if( ModeSAD[0] < ModeSAD[1] ){
                ModeSAD[6] = ModeSAD[8] = MAX_SAD;
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 5, pred_pels, pred_pels_mask, uPred[5]);
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 7, pred_pels, pred_pels_mask, uPred[7]);
                if (core_enc->m_Analyse & ANALYSE_SAD){
                    ModeSAD[5] = SAD8x8(pSrc, pitchPixels, uPred[5], 8);
                    ModeSAD[7] = SAD8x8(pSrc, pitchPixels, uPred[7], 8);
                }else{
                    ModeSAD[5] = SAT8x8D(pSrc, pitchPixels, uPred[5], 8);
                    ModeSAD[7] = SAT8x8D(pSrc, pitchPixels, uPred[7], 8);
                }
            }else{
                ModeSAD[5] = ModeSAD[7] = MAX_SAD;
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 6, pred_pels, pred_pels_mask, uPred[6]);
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 8, pred_pels, pred_pels_mask, uPred[8]);
                if (core_enc->m_Analyse & ANALYSE_SAD){
                    ModeSAD[6] = SAD8x8(pSrc, pitchPixels, uPred[6], 8);
                    ModeSAD[8] = SAD8x8(pSrc, pitchPixels, uPred[8], 8);
                }else{
                    ModeSAD[6] = SAT8x8D(pSrc, pitchPixels, uPred[6], 8);
                    ModeSAD[8] = SAT8x8D(pSrc, pitchPixels, uPred[8], 8);
                }
             }

             if( ModeSAD[5] < ModeSAD[7] ||  ModeSAD[6] < ModeSAD[8] ){
                    ModeSAD[3] = MAX_SAD;
                    H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 4, pred_pels, pred_pels_mask, uPred[4]);
                    if (core_enc->m_Analyse & ANALYSE_SAD){
                        ModeSAD[4] = SAD8x8(pSrc, pitchPixels, uPred[4], 8);
                    }else{
                        ModeSAD[4] = SAT8x8D(pSrc, pitchPixels, uPred[4], 8);
                    }
             }else{
                    ModeSAD[4] = MAX_SAD;
                    H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 3, pred_pels, pred_pels_mask, uPred[3]);
                    if (core_enc->m_Analyse & ANALYSE_SAD){
                        ModeSAD[3] = SAD8x8(pSrc, pitchPixels, uPred[3], 8);
                    }else{
                        ModeSAD[3] = SAT8x8D(pSrc, pitchPixels, uPred[3], 8);
                    }
             }
        }
    }else if( top_avbl ){
        modes = &intra_modes[2][0];
        nmodes=3;
        if (core_enc->m_Analyse & ANALYSE_SAD){
           for (Ipp32s i=0; i < nmodes; i++){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                ModeSAD[mode] = SAD8x8(pSrc, pitchPixels, uPred[mode], 8);
            }
        }else{
            for (Ipp32s i=0; i < nmodes; i++){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                ModeSAD[mode] = SAT8x8D(pSrc, pitchPixels, uPred[mode],8);
            }
        }
    }else if( left_avbl ){
        modes = &intra_modes[3][0];
        nmodes=2;
        if (core_enc->m_Analyse & ANALYSE_SAD){
           for (Ipp32s i=0; i < nmodes; i++){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                ModeSAD[mode] = SAD8x8(pSrc, pitchPixels, uPred[mode],8);
            }
        }else{
            for (Ipp32s i=0; i < nmodes; i++){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode]);
                ModeSAD[mode] = SAT8x8D(pSrc, pitchPixels, uPred[mode], 8);
            }
        }
    }

    if( nmodes ){
        Ipp32s BIT_COST_4 = BITS_COST(4, glob_RDQM[iQP]);
        if( intra_pred_mode != 2 ) ModeSAD[intra_pred_mode] += BITS_COST(1, glob_RDQM[iQP]) - BIT_COST_4;
        for(i=0;i<nmodes;i++){
            mode = modes[i];
            Ipp32u cost = ModeSAD[mode] + BIT_COST_4;
            if (cost <= best_SAD) {
                if( cost != best_SAD || mode < intra_actual_mode ){ //Prefer mode with less number
                    best_SAD = cost;
                    intra_actual_mode = mode;
                }
            }
        }
    }

    //Copy prediction
    if( pPredBuf ){
        for( i=0; i<8; i++){
            memcpy(pPredBuf, &uPred[intra_actual_mode][i*8], 8*sizeof(PIXTYPE));
            pPredBuf += 16; //pitch = 16
        }
        //Save the final mode
        for( i=0; i<4; i++ )
            pMode[ (uBlock<<2)+i ] = intra_actual_mode;

        if (curr_slice->m_use_transform_for_intra_decision){
            H264CoreEncoder_Encode8x8IntraBlock(state, curr_slice, uBlock);
        }
    }

    return best_SAD;
}

template<typename PIXTYPE>
void H264CoreEncoder_Filter8x8Pels(
    PIXTYPE* pred_pels,
    Ipp32u pred_pels_mask)
{
    PIXTYPE pred_pels_filtered[25];
    Ipp32s i;

    if( (pred_pels_mask & 0x1FE) == 0x1FE){ /* 0..7 */
        if( pred_pels_mask & 0x01 ){
            pred_pels_filtered[1+0] = (pred_pels[0]+2*pred_pels[1+0]+pred_pels[1+1]+2)>>2;
        }else{
            pred_pels_filtered[1+0] = (3*pred_pels[1+0]+pred_pels[1+1]+2)>>2;
        }

        for( i=1; i<8; i++ )
            pred_pels_filtered[1+i] = (pred_pels[1+i-1]+2*pred_pels[1+i]+pred_pels[1+i+1]+2)>>2;
    }

    if( (pred_pels_mask & 0x0001FF00) == 0x0001FF00 ){ /* 7..15 */
        for( i=0; i<7; i++ )
            pred_pels_filtered[9+i] = (pred_pels[9+i-1]+2*pred_pels[9+i]+pred_pels[9+i+1]+2)>>2;
        pred_pels_filtered[9+7]=(pred_pels[9+6]+3*pred_pels[9+7]+2)>>2;
    }

    if( pred_pels_mask & 0x01 ){ /* 0 */
        if( (pred_pels_mask & 0x00020002) == 0x00020002 ){
            pred_pels_filtered[0] = (pred_pels[1+0]+2*pred_pels[0]+pred_pels[17+0]+2)>>2;
        }else{
            if( pred_pels_mask & 0x02 ){
                pred_pels_filtered[0] = (3*pred_pels[0]+pred_pels[1+0]+2)>>2;
            }else if( pred_pels_mask & 0x00020000 ){
                pred_pels_filtered[0] = (3*pred_pels[0]+pred_pels[17+0]+2)>>2;
            }
        }
    }

    if( (pred_pels_mask & 0x01FE0000) ==  0x01FE0000){ /* y 0..7 */
        if( pred_pels_mask & 0x01 ){
            pred_pels_filtered[17+0] = (pred_pels[0]+2*pred_pels[17+0]+pred_pels[17+1]+2)>>2;
        }else{
            pred_pels_filtered[17+0] = (3*pred_pels[17+0]+pred_pels[17+1]+2)>>2;
        }
        for( i=1; i<7; i++ )
            pred_pels_filtered[17+i] = (pred_pels[17+i-1]+2*pred_pels[17+i]+pred_pels[17+i+1]+2)>>2;
        pred_pels_filtered[17+7] = (pred_pels[17+6]+3*pred_pels[17+7]+2)>>2;
    }

    memcpy( pred_pels, pred_pels_filtered, 25*sizeof(PIXTYPE));
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetPrediction8x8(void* state, T_AIMode mode, PIXTYPE* PredPel, Ipp32u pred_pels_mask, PIXTYPE* pels)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s value;

    switch( mode ){
    case 0: /*Vertical*/
        *(pels+56)=*(pels+48)=*(pels+40)=*(pels+32)=*(pels+24)=*(pels+16)=*(pels+8)=*(pels+0)=P_A;
        *(pels+57)=*(pels+49)=*(pels+41)=*(pels+33)=*(pels+25)=*(pels+17)=*(pels+9)=*(pels+1)=P_B;
        *(pels+58)=*(pels+50)=*(pels+42)=*(pels+34)=*(pels+26)=*(pels+18)=*(pels+10)=*(pels+2)=P_C;
        *(pels+59)=*(pels+51)=*(pels+43)=*(pels+35)=*(pels+27)=*(pels+19)=*(pels+11)=*(pels+3)=P_D;
        *(pels+60)=*(pels+52)=*(pels+44)=*(pels+36)=*(pels+28)=*(pels+20)=*(pels+12)=*(pels+4)=P_E;
        *(pels+61)=*(pels+53)=*(pels+45)=*(pels+37)=*(pels+29)=*(pels+21)=*(pels+13)=*(pels+5)=P_F;
        *(pels+62)=*(pels+54)=*(pels+46)=*(pels+38)=*(pels+30)=*(pels+22)=*(pels+14)=*(pels+6)=P_G;
        *(pels+63)=*(pels+55)=*(pels+47)=*(pels+39)=*(pels+31)=*(pels+23)=*(pels+15)=*(pels+7)=P_H;
        break;
    case 1: /* Horizontal */
        *(pels+7)=*(pels+6)=*(pels+5)=*(pels+4)=*(pels+3)=*(pels+2)=*(pels+1)=*(pels+0)=P_Q;
        *(pels+15)=*(pels+14)=*(pels+13)=*(pels+12)=*(pels+11)=*(pels+10)=*(pels+9)=*(pels+8)=P_R;
        *(pels+23)=*(pels+22)=*(pels+21)=*(pels+20)=*(pels+19)=*(pels+18)=*(pels+17)=*(pels+16)=P_S;
        *(pels+31)=*(pels+30)=*(pels+29)=*(pels+28)=*(pels+27)=*(pels+26)=*(pels+25)=*(pels+24)=P_T;
        *(pels+39)=*(pels+38)=*(pels+37)=*(pels+36)=*(pels+35)=*(pels+34)=*(pels+33)=*(pels+32)=P_U;
        *(pels+47)=*(pels+46)=*(pels+45)=*(pels+44)=*(pels+43)=*(pels+42)=*(pels+41)=*(pels+40)=P_V;
        *(pels+55)=*(pels+54)=*(pels+53)=*(pels+52)=*(pels+51)=*(pels+50)=*(pels+49)=*(pels+48)=P_W;
        *(pels+63)=*(pels+62)=*(pels+61)=*(pels+60)=*(pels+59)=*(pels+58)=*(pels+57)=*(pels+56)=P_X;
        break;
    case 2: /* DC */
        value = 0;
        if( (pred_pels_mask & 0x01fe01fe) == 0x01fe01fe ){
            value = (P_A + P_B + P_C + P_D + P_E + P_F + P_G + P_H + P_Q + P_R + P_S + P_T + P_U + P_V + P_W + P_X + 8)>>4;
        }else if( (pred_pels_mask & 0x01fe0000) == 0x01fe0000 ){
            value = (P_Q + P_R + P_S + P_T + P_U + P_V + P_W + P_X + 4)>>3;
        }else if( (pred_pels_mask & 0x000001fe) == 0x000001fe ){
            value = (P_A + P_B + P_C + P_D + P_E + P_F + P_G + P_H + 4)>>3;
        }else value = 1<<(core_enc->m_PicParamSet.bit_depth_luma - 1);
        MemorySet(pels, (PIXTYPE)value, 64);
        break;
    case 3: /* Diagonal Down Left */
        *(pels+0)=(P_A + 2*P_B + P_C + 2)>>2;
        *(pels+1)=*(pels+8)=(P_B + 2*P_C + P_D + 2)>>2;
        *(pels+2)=*(pels+9)=*(pels+16)=(P_C + 2*P_D + P_E + 2)>>2;
        *(pels+3)=*(pels+10)=*(pels+17)=*(pels+24)=(P_D + 2*P_E + P_F + 2)>>2;
        *(pels+4)=*(pels+11)=*(pels+18)=*(pels+25)=*(pels+32)=(P_E + 2*P_F + P_G + 2)>>2;
        *(pels+5)=*(pels+12)=*(pels+19)=*(pels+26)=*(pels+33)=*(pels+40)=(P_F + 2*P_G + P_H + 2)>>2;
        *(pels+6)=*(pels+13)=*(pels+20)=*(pels+27)=*(pels+34)=*(pels+41)=*(pels+48)=(P_G + 2*P_H + P_I + 2)>>2;
        *(pels+7)=*(pels+14)=*(pels+21)=*(pels+28)=*(pels+35)=*(pels+42)=*(pels+49)=*(pels+56)=(P_H + 2*P_I + P_J + 2)>>2;
        *(pels+15)=*(pels+22)=*(pels+29)=*(pels+36)=*(pels+43)=*(pels+50)=*(pels+57)=(P_I + 2*P_J + P_K + 2)>>2;
        *(pels+23)=*(pels+30)=*(pels+37)=*(pels+44)=*(pels+51)=*(pels+58)=(P_J + 2*P_K + P_L + 2)>>2;
        *(pels+31)=*(pels+38)=*(pels+45)=*(pels+52)=*(pels+59)=(P_K + 2*P_L + P_M + 2)>>2;
        *(pels+39)=*(pels+46)=*(pels+53)=*(pels+60)=(P_L + 2*P_M + P_N + 2)>>2;
        *(pels+47)=*(pels+54)=*(pels+61)=(P_M + 2*P_N + P_O + 2)>>2;
        *(pels+55)=*(pels+62)=(P_N + 2*P_O + P_P + 2)>>2;
        *(pels+63)=(P_O + 3*P_P + 2)>>2;
        break;
    case 4: /* Diagonal Down Right */
        *(pels+55)=*(pels+46)=*(pels+37)=*(pels+28)=*(pels+19)=*(pels+10)=*(pels+1)=(P_Z + 2*P_A + P_B +2)>>2;
        *(pels+62)=*(pels+53)=*(pels+44)=*(pels+35)=*(pels+26)=*(pels+17)=*(pels+8)=(P_Z + 2*P_Q + P_R +2)>>2;
        *(pels+63)=*(pels+54)=*(pels+45)=*(pels+36)=*(pels+27)=*(pels+18)=*(pels+9)=*(pels+0)=(P_A + 2*P_Z + P_Q +2)>>2;
        *(pels+47)=*(pels+38)=*(pels+29)=*(pels+20)=*(pels+11)=*(pels+2)=(P_A + 2*P_B + P_C +2)>>2;
        *(pels+39)=*(pels+30)=*(pels+21)=*(pels+12)=*(pels+3)=(P_B + 2*P_C + P_D +2)>>2;
        *(pels+31)=*(pels+22)=*(pels+13)=*(pels+4)=(P_C + 2*P_D + P_E +2)>>2;
        *(pels+23)=*(pels+14)=*(pels+5)=(P_D + 2*P_E + P_F +2)>>2;
        *(pels+15)=*(pels+6)=(P_E + 2*P_F + P_G +2)>>2;
        *(pels+7)=(P_F + 2*P_G + P_H +2)>>2;
        *(pels+61)=*(pels+52)=*(pels+43)=*(pels+34)=*(pels+25)=*(pels+16)=(P_Q + 2*P_R + P_S +2)>>2;
        *(pels+60)=*(pels+51)=*(pels+42)=*(pels+33)=*(pels+24)=(P_R + 2*P_S + P_T +2)>>2;
        *(pels+59)=*(pels+50)=*(pels+41)=*(pels+32)=(P_S + 2*P_T + P_U +2)>>2;
        *(pels+58)=*(pels+49)=*(pels+40)=(P_T + 2*P_U + P_V +2)>>2;
        *(pels+57)=*(pels+48)=(P_U + 2*P_V + P_W +2)>>2;
        *(pels+56)=(P_V + 2*P_W + P_X +2)>>2;
        break;
    case 5: /* Vertical Right */
        *(pels+60)=*(pels+43)=*(pels+26)=*(pels+9)=(P_Z + 2*P_A + P_B + 2)>>2;
        *(pels+51)=*(pels+34)=*(pels+17)=*(pels+0)=(P_Z + P_A + 1)>>1;
        *(pels+61)=*(pels+44)=*(pels+27)=*(pels+10)=(P_A + 2*P_B + P_C + 2)>>2;
        *(pels+52)=*(pels+35)=*(pels+18)=*(pels+1)=(P_A + P_B + 1)>>1;
        *(pels+62)=*(pels+45)=*(pels+28)=*(pels+11)=(P_B + 2*P_C + P_D + 2)>>2;
        *(pels+53)=*(pels+36)=*(pels+19)=*(pels+2)=(P_B + P_C + 1)>>1;
        *(pels+63)=*(pels+46)=*(pels+29)=*(pels+12)=(P_C + 2*P_D + P_E + 2)>>2;
        *(pels+54)=*(pels+37)=*(pels+20)=*(pels+3)=(P_C + P_D + 1)>>1;
        *(pels+47)=*(pels+30)=*(pels+13)=(P_D + 2*P_E + P_F + 2)>>2;
        *(pels+55)=*(pels+38)=*(pels+21)=*(pels+4)=(P_D + P_E + 1)>>1;
        *(pels+31)=*(pels+14)=(P_E + 2*P_F + P_G + 2)>>2;
        *(pels+39)=*(pels+22)=*(pels+5)=(P_E + P_F + 1)>>1;
        *(pels+15)=(P_F + 2*P_G + P_H + 2)>>2;
        *(pels+23)=*(pels+6)=(P_F + P_G + 1)>>1;
        *(pels+7)=(P_G + P_H + 1)>>1;
        *(pels+59)=*(pels+42)=*(pels+25)=*(pels+8)=(P_Q + 2*P_Z + P_A + 2)>>2;
        *(pels+50)=*(pels+33)=*(pels+16)=(P_R + 2*P_Q + P_Z + 2)>>2;
        *(pels+58)=*(pels+41)=*(pels+24)=(P_S + 2*P_R + P_Q + 2)>>2;
        *(pels+49)=*(pels+32)=(P_T + 2*P_S + P_R + 2)>>2;
        *(pels+57)=*(pels+40)=(P_U + 2*P_T + P_S + 2)>>2;
        *(pels+48)=(P_V + 2*P_U + P_T + 2)>>2;
        *(pels+56)=(P_W + 2*P_V + P_U + 2)>>2;
        break;
    case 6: /* Horizontal Down */
        *(pels+39)=*(pels+29)=*(pels+19)=*(pels+9)=(P_Z + 2*P_Q + P_R + 2)>>2;
        *(pels+30)=*(pels+20)=*(pels+10)=*(pels+0)=(P_Z + P_Q + 1)>>1;
        *(pels+22)=*(pels+12)=*(pels+2)=(P_B + 2*P_A + P_Z + 2)>>2;
        *(pels+23)=*(pels+13)=*(pels+3)=(P_C + 2*P_B + P_A + 2)>>2;
        *(pels+14)=*(pels+4)=(P_D + 2*P_C + P_B + 2)>>2;
        *(pels+15)=*(pels+5)=(P_E + 2*P_D + P_C + 2)>>2;
        *(pels+6)=(P_F + 2*P_E + P_D + 2)>>2;
        *(pels+7)=(P_G + 2*P_F + P_E + 2)>>2;
        *(pels+31)=*(pels+21)=*(pels+11)=*(pels+1)=(P_Q + 2*P_Z + P_A + 2)>>2;
        *(pels+47)=*(pels+37)=*(pels+27)=*(pels+17)=(P_Q + 2*P_R + P_S + 2)>>2;
        *(pels+38)=*(pels+28)=*(pels+18)=*(pels+8)=(P_Q + P_R + 1)>>1;
        *(pels+55)=*(pels+45)=*(pels+35)=*(pels+25)=(P_R + 2*P_S + P_T + 2)>>2;
        *(pels+46)=*(pels+36)=*(pels+26)=*(pels+16)=(P_R + P_S + 1)>>1;
        *(pels+63)=*(pels+53)=*(pels+43)=*(pels+33)=(P_S + 2*P_T + P_U + 2)>>2;
        *(pels+54)=*(pels+44)=*(pels+34)=*(pels+24)=(P_S + P_T + 1)>>1;
        *(pels+61)=*(pels+51)=*(pels+41)=(P_T + 2*P_U + P_V + 2)>>2;
        *(pels+62)=*(pels+52)=*(pels+42)=*(pels+32)=(P_T + P_U + 1)>>1;
        *(pels+59)=*(pels+49)=(P_U + 2*P_V + P_W + 2)>>2;
        *(pels+60)=*(pels+50)=*(pels+40)=(P_U + P_V + 1)>>1;
        *(pels+57)=(P_V + 2*P_W + P_X + 2)>>2;
        *(pels+58)=*(pels+48)=(P_V + P_W + 1)>>1;
        *(pels+56)=(P_W + P_X + 1)>>1;
        break;
    case 7: /* Vertical Left */
        *(pels+8)=(P_A + 2*P_B + P_C + 2)>>2;
        *(pels+0)=(P_A + P_B + 1)>>1;
        *(pels+9)=*(pels+24)=(P_B + 2*P_C + P_D + 2)>>2;
        *(pels+1)=*(pels+16)=(P_B + P_C + 1)>>1;
        *(pels+10)=*(pels+25)=*(pels+40)=(P_C + 2*P_D + P_E + 2)>>2;
        *(pels+2)=*(pels+17)=*(pels+32)=(P_C + P_D + 1)>>1;
        *(pels+11)=*(pels+26)=*(pels+41)=*(pels+56)=(P_D + 2*P_E + P_F + 2)>>2;
        *(pels+3)=*(pels+18)=*(pels+33)=*(pels+48)=(P_D + P_E + 1)>>1;
        *(pels+12)=*(pels+27)=*(pels+42)=*(pels+57)=(P_E + 2*P_F + P_G + 2)>>2;
        *(pels+4)=*(pels+19)=*(pels+34)=*(pels+49)=(P_E + P_F + 1)>>1;
        *(pels+13)=*(pels+28)=*(pels+43)=*(pels+58)=(P_F + 2*P_G + P_H + 2)>>2;
        *(pels+5)=*(pels+20)=*(pels+35)=*(pels+50)=(P_F + P_G + 1)>>1;
        *(pels+14)=*(pels+29)=*(pels+44)=*(pels+59)=(P_G + 2*P_H + P_I + 2)>>2;
        *(pels+6)=*(pels+21)=*(pels+36)=*(pels+51)=(P_G + P_H + 1)>>1;
        *(pels+15)=*(pels+30)=*(pels+45)=*(pels+60)=(P_H + 2*P_I + P_J + 2)>>2;
        *(pels+7)=*(pels+22)=*(pels+37)=*(pels+52)=(P_H + P_I + 1)>>1;
        *(pels+31)=*(pels+46)=*(pels+61)=(P_I + 2*P_J + P_K + 2)>>2;
        *(pels+23)=*(pels+38)=*(pels+53)=(P_I + P_J + 1)>>1;
        *(pels+47)=*(pels+62)=(P_J + 2*P_K + P_L + 2)>>2;
        *(pels+39)=*(pels+54)=(P_J + P_K + 1)>>1;
        *(pels+63)=(P_K + 2*P_L + P_M + 2)>>2;
        *(pels+55)=(P_K + P_L + 1)>>1;
        break;
    case 8: /* Horizontal Up */
        *(pels+1)=(P_Q + 2*P_R + P_S + 2)>>2;
        *(pels+0)=(P_Q + P_R + 1)>>1;
        *(pels+3)=*(pels+9)=(P_R + 2*P_S + P_T + 2)>>2;
        *(pels+2)=*(pels+8)=(P_R + P_S + 1)>>1;
        *(pels+5)=*(pels+11)=*(pels+17)=(P_S + 2*P_T + P_U + 2)>>2;
        *(pels+4)=*(pels+10)=*(pels+16)=(P_S + P_T + 1)>>1;
        *(pels+7)=*(pels+13)=*(pels+19)=*(pels+25)=(P_T + 2*P_U + P_V + 2)>>2;
        *(pels+6)=*(pels+12)=*(pels+18)=*(pels+24)=(P_T + P_U + 1)>>1;
        *(pels+15)=*(pels+21)=*(pels+27)=*(pels+33)=(P_U + 2*P_V + P_W + 2)>>2;
        *(pels+14)=*(pels+20)=*(pels+26)=*(pels+32)=(P_U + P_V + 1)>>1;
        *(pels+23)=*(pels+29)=*(pels+35)=*(pels+41)=(P_V + 2*P_W + P_X + 2)>>2;
        *(pels+22)=*(pels+28)=*(pels+34)=*(pels+40)=(P_V + P_W + 1)>>1;
        *(pels+30)=*(pels+36)=*(pels+42)=*(pels+48)=(P_W + P_X + 1)>>1;
        *(pels+63)=*(pels+55)=*(pels+47)=*(pels+39)=*(pels+62)=*(pels+54)=*(pels+46)=*(pels+38)=*(pels+61)=*(pels+53)=*(pels+45)=*(pels+60)=*(pels+52)=*(pels+44)=*(pels+59)=*(pels+51)=*(pels+58)=*(pels+50)=*(pels+57)=*(pels+56)=P_X;
        *(pels+31)=*(pels+37)=*(pels+43)=*(pels+49)=(P_W + 3*P_X + 2)>>2;
        break;
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_Intra4x4SelectRD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE*  pSrcBlock,         // pointer to upper left pel of source block
    PIXTYPE*  pReconBlock,         // pointer to same block in reconstructed picture
    Ipp32u    uBlock,            // which 4x4 of the MB (0..15)
    T_AIMode* intra_types,       // selected mode goes here
    PIXTYPE*  pPred)             // predictor pels for selected mode goes here
    // if not NULL
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMinSAD;
    T_AIMode Mode, ModeAbove, ModeLeft, ProbMode;
    PIXTYPE PredPel[13];
    H264BsFake** fBitstreams = curr_slice->fBitstreams;
    Ipp32s isField = curr_slice->m_is_cur_mb_field;
    Ipp32u cbp_bits = curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits;
    Ipp32u cbp_bits_best = 0;

    T_AIMode *pMode = &intra_types[uBlock];


    // get AI mode of block above and block to left to use to add bit cost
    // of signaling each mode for this block to SAD. Take care not to go
    // beyond the AIMode array, at top and left picture edges.
    ModeLeft = -1;
    if (BLOCK_IS_ON_LEFT_EDGE(block_subblock_mapping_[uBlock]))
    {
        H264BlockLocation block = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[block_subblock_mapping_[uBlock] >> 2];
        if (block.mb_num >= 0)
        {
            ModeLeft = core_enc->m_mbinfo.intra_types[block.mb_num].intra_types[block_subblock_mapping_[block.block_num]];
        }
    } else {
        Ipp32s block_num = block_subblock_mapping_[uBlock] - 1; // neighbour in non-raster order
        block_num = block_subblock_mapping_[block_num];
        ModeLeft = intra_types[block_num];
    }

    ModeAbove = -1;
    if (BLOCK_IS_ON_TOP_EDGE(block_subblock_mapping_[uBlock]))
    {
        H264BlockLocation block = curr_slice->m_cur_mb.BlockNeighbours.mb_above;
        if (block.mb_num >= 0)
        {
            block.block_num += block_subblock_mapping_[uBlock];
            ModeAbove = core_enc->m_mbinfo.intra_types[block.mb_num].intra_types[block_subblock_mapping_[block.block_num]];
        }
    }else {
        Ipp32s block_num = block_subblock_mapping_[uBlock] - 4; // neighbour in non-raster order
        block_num = block_subblock_mapping_[block_num];
        ModeAbove = intra_types[block_num];
    }

    Ipp32s pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels<<curr_slice->m_is_cur_mb_field;

    PIXTYPE* pLeftRefBlock;
    Ipp32u uLeftPitch;
    PIXTYPE* pAboveRefBlock;
    Ipp32u uAbovePitch;
    PIXTYPE* pAboveLeftRefBlock;
    Ipp32u uAboveLeftPitch;

    if (curr_slice->m_use_transform_for_intra_decision){
        uLeftPitch = uAbovePitch = uAboveLeftPitch = 16;
        pLeftRefBlock = pAboveRefBlock = pAboveLeftRefBlock = pPred + 256;
        if (ModeAbove == -1 || BLOCK_IS_ON_TOP_EDGE(block_subblock_mapping_[uBlock])){
            pAboveRefBlock = pReconBlock;
            uAbovePitch = pitchPixels;
            pAboveLeftRefBlock = pReconBlock;
            uAboveLeftPitch = pitchPixels;
        }

        if (BLOCK_IS_ON_LEFT_EDGE(block_subblock_mapping_[uBlock])){
            pLeftRefBlock = pReconBlock;
            uLeftPitch = pitchPixels;
            pAboveLeftRefBlock = pReconBlock;
            uAboveLeftPitch = pitchPixels;
        }
    } else {
        pLeftRefBlock = pReconBlock;
        uLeftPitch = pitchPixels;
        pAboveRefBlock = pReconBlock;
        uAbovePitch = pitchPixels;
        pAboveLeftRefBlock = pReconBlock;
        uAboveLeftPitch = pitchPixels;
    }

    // Get the above and left predictor pels to PredPel
    H264CoreEncoder_GetBlockPredPels(state, curr_slice, pLeftRefBlock, uLeftPitch, pAboveRefBlock, uAbovePitch, pAboveLeftRefBlock, uAboveLeftPitch, uBlock, PredPel);

    // If all the predictor pels are equal, all modes will give the same results.
    ProbMode = MIN(ModeAbove, ModeLeft);
    if (ProbMode == -1) ProbMode = 2;

    Ipp32s* modes = 0;
    Ipp32s nmodes=0;

    if( ( ModeAbove >= 0 ) && ( ModeLeft >= 0 ) ){
        if (!uBlock && (cur_mb.BlockNeighbours.mb_above_left.mb_num < 0)){
            modes = &intra_modes[0][0];
            nmodes=5;
        }else{
            modes = &intra_modes[1][0];
            nmodes=8;
        }
    }else if( ModeAbove >= 0){
        modes = &intra_modes[2][0];
        nmodes=3;
    }else if( ModeLeft >= 0 ){
        modes = &intra_modes[3][0];
        nmodes=2;
    }

    Ipp32s m;
    Ipp32s BestMode = *pMode;

    if ((core_enc->m_Analyse & ANALYSE_FAST_INTRA) && (nmodes == 8)) {
        Ipp32u ModeSAD[10];
        Ipp32s i, iQP = cur_mb.lumaQP51, BITS_COST_4 = BITS_COST(4, glob_RDQM[iQP]);
        for (i = 0; i < 9; i ++) {
            H264CoreEncoder_GetPredBlock(i, pPred, PredPel);
            ModeSAD[i] = SATD4x4(pSrcBlock, pitchPixels, pPred, 16) + BITS_COST_4;
        }
        ModeSAD[ProbMode] += BITS_COST(1, glob_RDQM[iQP]) - BITS_COST_4;
        ModeSAD[9] = MAX_SAD;
        T_AIMode bestMode[3];
        bestMode[0] = 0; bestMode[1] = bestMode[2] = 9;
        for (i = 1; i < 9; i ++) {
            if (ModeSAD[i] >= ModeSAD[bestMode[2]])
                continue;
            if (ModeSAD[i] < ModeSAD[bestMode[0]]) {
                bestMode[2] = bestMode[1]; bestMode[1] = bestMode[0]; bestMode[0] = i;
            } else if (ModeSAD[i] < ModeSAD[bestMode[1]]) {
                bestMode[2] = bestMode[1]; bestMode[1] = i;
            } else
                bestMode[2] = i;
        }
        for (m = 0; m < 3; m++) {
            Mode = bestMode[m];
            if (BestMode != Mode)
                H264BsBase_CopyContextCABAC_I4x4(&fBitstreams[Mode]->m_base, &fBitstreams[BestMode]->m_base, !isField);
        }
        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
        c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs4x4[uBlock]);
        c_data->uLastSignificant = cur_mb.m_iLastCoeff4x4[uBlock];
        c_data->CtxBlockCat = BLOCK_LUMA_LEVELS;
        c_data->uFirstCoeff = 0;
        c_data->uLastCoeff = 15;
        Ipp32u cost;
        const Ipp32s* scan = dec_single_scan[isField];
        COEFFSTYPE* transform = &cur_mb.mb4x4.transform[uBlock*16];
        const PIXTYPE*    recon = cur_mb.mb4x4.reconstruct + xoff[uBlock] + yoff[uBlock]*16;
        uMinSAD = MAX_SAD;
        for (i = 0; i < 3; i ++) {
            Mode = bestMode[i];
            H264CoreEncoder_GetPredBlock(Mode, pPred, PredPel);
            cur_mb.m_uIntraCBP4x4 |= CBP4x4Mask[uBlock];
            H264CoreEncoder_Encode4x4IntraBlock(state, curr_slice, uBlock);
            cost = SSD4x4(pSrcBlock, pitchPixels, recon, 16 )<<5;
            if( cost > uMinSAD ) continue;
            Ipp32s prob0;
            if (ProbMode == Mode) {
                prob0 = -1;
            } else if (Mode < ProbMode ) {    // Otherwise, the mode probability increases
                prob0 = Mode;                      // (the opposite of intuitive notion of probability)
            } else {                                    // with the order of the remaining modes.
                prob0 = Mode-1;
            }
            c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs4x4[uBlock]);
            c_data->uLastSignificant = cur_mb.m_iLastCoeff4x4[uBlock];
            H264CoreEncoder_MakeSignificantLists_CABAC(transform, scan, c_data);
            curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits = cbp_bits;
            H264BsFake* stream = fBitstreams[Mode];
            curr_slice->m_pbitstream = (H264BsBase *)stream;
            H264BsFake_Reset(stream);
            H264BsFake_IntraPredMode_CABAC(stream, prob0);
            H264CoreEncoder_PackSubBlockLuma_Fake(state, curr_slice, uBlock);
            cost += cur_mb.lambda * H264BsFake_GetBsOffset(stream);
            if( cost <= uMinSAD ){
                 if( cost != uMinSAD || Mode < *pMode ){ //Prefer mode with less number
                    uMinSAD = cost;
                    *pMode = Mode;
                    cbp_bits_best = curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits;
                 }
            }
        }
    } else {
        if (BestMode != 2)
            H264BsBase_CopyContextCABAC_I4x4(
                &fBitstreams[2]->m_base,
                &fBitstreams[BestMode]->m_base,
                !isField);

        for (m = 0; m < nmodes; m++)
        {
            Mode = modes[m];
            if (BestMode != Mode)
                H264BsBase_CopyContextCABAC_I4x4(
                    &fBitstreams[Mode]->m_base,
                    &fBitstreams[BestMode]->m_base,
                    !isField);
        }
        H264CoreEncoder_GetPredBlock(2, pPred, PredPel);
        cur_mb.m_uIntraCBP4x4 |= CBP4x4Mask[uBlock];
        H264CoreEncoder_Encode4x4IntraBlock(state, curr_slice, uBlock);
        uMinSAD = SSD4x4(pSrcBlock, pitchPixels, cur_mb.mb4x4.reconstruct + xoff[uBlock] + yoff[uBlock]*16, 16 )<<5;
        Ipp32s prob0;
        if (ProbMode == 2) {
            prob0 = -1;
        } else if (2 < ProbMode ) {    // Otherwise, the mode probability increases
            prob0 = 2;                      // (the opposite of intuitive notion of probability)
        } else {                                    // with the order of the remaining modes.
            prob0 = 2-1;
        }
        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
        c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs4x4[uBlock]);
        c_data->uLastSignificant = cur_mb.m_iLastCoeff4x4[uBlock];
        c_data->CtxBlockCat = BLOCK_LUMA_LEVELS;
        c_data->uFirstCoeff = 0;
        c_data->uLastCoeff = 15;
        H264CoreEncoder_MakeSignificantLists_CABAC(&cur_mb.mb4x4.transform[uBlock*16],dec_single_scan[isField], c_data);

        curr_slice->m_pbitstream = (H264BsBase *)fBitstreams[2];
        H264BsFake_Reset(fBitstreams[2]);
        H264BsFake_IntraPredMode_CABAC(fBitstreams[2], prob0);
        H264CoreEncoder_PackSubBlockLuma_Fake(state, curr_slice, uBlock);
        Ipp32s bits = H264BsFake_GetBsOffset(fBitstreams[2]);
        uMinSAD += cur_mb.lambda*bits;
        *pMode = 2;
        cbp_bits_best = curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits;

        if( nmodes ){
            Ipp32u cost;
            Ipp32s i;
            const Ipp32s* scan = dec_single_scan[isField];
            COEFFSTYPE* transform = &cur_mb.mb4x4.transform[uBlock*16];
            const PIXTYPE*    recon = cur_mb.mb4x4.reconstruct + xoff[uBlock] + yoff[uBlock]*16;

            for (i=0; i < nmodes; i++){
                Mode = modes[i];
                H264CoreEncoder_GetPredBlock(Mode, pPred, PredPel);
                cur_mb.m_uIntraCBP4x4 |= CBP4x4Mask[uBlock];
                H264CoreEncoder_Encode4x4IntraBlock(state, curr_slice, uBlock);
                cost = SSD4x4(pSrcBlock, pitchPixels, recon, 16 )<<5;
                if( cost > uMinSAD ) continue;
                Ipp32s prob0;
                if (ProbMode == Mode) {
                    prob0 = -1;
                } else if (Mode < ProbMode ) {    // Otherwise, the mode probability increases
                    prob0 = Mode;                      // (the opposite of intuitive notion of probability)
                } else {                                    // with the order of the remaining modes.
                    prob0 = Mode-1;
                }
                c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs4x4[uBlock]);
                c_data->uLastSignificant = cur_mb.m_iLastCoeff4x4[uBlock];
                H264CoreEncoder_MakeSignificantLists_CABAC(transform, scan, c_data);
                curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits = cbp_bits;

                H264BsFake* stream = fBitstreams[Mode];
                curr_slice->m_pbitstream = (H264BsBase *)stream;
                H264BsFake_Reset(stream);
                H264BsFake_IntraPredMode_CABAC(stream, prob0);
                H264CoreEncoder_PackSubBlockLuma_Fake(state, curr_slice, uBlock);
                cost += cur_mb.lambda * H264BsFake_GetBsOffset(stream);
                if( cost <= uMinSAD ){
                     if( cost != uMinSAD || Mode < *pMode ){ //Prefer mode with less number
                        uMinSAD = cost;
                        *pMode = Mode;
                        cbp_bits_best = curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits;
                     }
                }

            }
        }
    }
    H264CoreEncoder_GetPredBlock(*pMode, pPred, PredPel);
    if (curr_slice->m_use_transform_for_intra_decision){
        cur_mb.m_uIntraCBP4x4 |= CBP4x4Mask[uBlock];
        H264CoreEncoder_Encode4x4IntraBlock(state, curr_slice, uBlock);
    }
    curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_bits = cbp_bits_best;
    return uMinSAD;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_Intra8x8SelectRD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pSrc,            // pointer to upper left pel of source MB
    PIXTYPE* pRef,            // pointer to same MB in reference picture
    Ipp32s   uBlock,          // 8x8 block number
    T_AIMode* pMode,          // selected mode goes here
    PIXTYPE* pPredBuf)        // predictor pels for selected mode goes here
                              //pPredBuf should be not equal to 0 if use_transform_for_intra_decision is set
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    PIXTYPE pred_pels[25]; //Sources for prediction
    Ipp32u pred_pels_mask;
    Ipp32u best_SAD;
    Ipp32s i, n;
    bool dcPredModePredictedFlag = false;
    T_AIMode intra_pred_mode_A, intra_pred_mode_B, intra_pred_mode, intra_actual_mode, mode;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;
    H264BsFake** fBitstreams = curr_slice->fBitstreams;
    Ipp32s isField = curr_slice->m_is_cur_mb_field;

    /* First fill out pred_pels
    ** pred_pels
    ** [0] - top-left
    ** [1..16] - top 16th values
    ** [17..24] - left 8-th values
    */

    pred_pels_mask=0; //all is not available; 25 bits used
    bool top_avbl;
    bool left_avbl;
    bool left_above_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
    bool right_above_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above_right.mb_num >= 0;
    //bool right_avbl = curr_slice->m_cur_mb.BlockNeighbours..mb_num >= 0;
    H264BlockLocation mb_ALoc;
    H264BlockLocation mb_BLoc;
    H264MacroblockGlobalInfo mb_A;
    H264MacroblockGlobalInfo mb_B;

    //Check availability
    if( uBlock & 0x2 ){
        top_avbl = true;
        mb_BLoc.block_num = uBlock - 2;
        mb_BLoc.mb_num = cur_mb.uMB;
        mb_B = *(cur_mb.GlobalMacroblockInfo);
    }else{
        top_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
        mb_BLoc = cur_mb.BlockNeighbours.mb_above;
        mb_BLoc.block_num = uBlock + 2;
        mb_B = core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb_BLoc.mb_num];
    }

    if( uBlock & 0x1 ){
        left_avbl = true;
        mb_ALoc.block_num = uBlock - 1;
        mb_ALoc.mb_num = cur_mb.uMB;
        mb_A = *(cur_mb.GlobalMacroblockInfo);
    }else{
        left_avbl = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
        mb_ALoc = cur_mb.BlockNeighbours.mbs_left[0];
        mb_ALoc.block_num = uBlock + 1;
        mb_A = core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb_ALoc.mb_num];
    }

    if (!top_avbl){
        dcPredModePredictedFlag = true;
        //Reset mask
    }else if( mb_B.mbtype < 8 && mb_B.mbtype > 2 && core_enc->m_PicParamSet.constrained_intra_pred_flag )
            dcPredModePredictedFlag = true;

    if(!left_avbl){
        dcPredModePredictedFlag = true;
    }else if( mb_A.mbtype < 8 && mb_A.mbtype > 2 && core_enc->m_PicParamSet.constrained_intra_pred_flag )
        dcPredModePredictedFlag = true;

    PIXTYPE* pLeftRef;
    Ipp32u uLeftPitch;
    PIXTYPE* pAboveRef;
    Ipp32u uAbovePitch;
    PIXTYPE* pAboveLeftRef;
    Ipp32u uAboveLeftPitch;
    PIXTYPE* pAboveRightRef;
    Ipp32u uAboveRightPitch;

    if (curr_slice->m_use_transform_for_intra_decision){

        uLeftPitch =  uAbovePitch = uAboveLeftPitch = uAboveRightPitch = 16;
        pLeftRef = pAboveRef = pAboveLeftRef = pAboveRightRef = pPredBuf + 256;

        if ( !(uBlock & 0x2) ) //block on top edge
        {
            pAboveRef = pRef;
            uAbovePitch = pitchPixels;
            pAboveLeftRef = pRef;
            uAboveLeftPitch = pitchPixels;
            pAboveRightRef = pRef;
            uAboveRightPitch = pitchPixels;
        }

        if ( !(uBlock & 0x1) ) //block on left edge
        {
            pLeftRef = pRef;
            uLeftPitch = pitchPixels;
            pAboveLeftRef = pRef;
            uAboveLeftPitch = pitchPixels;
        }
    } else {
        pLeftRef = pRef;
        uLeftPitch = pitchPixels;
        pAboveRef = pRef;
        uAbovePitch = pitchPixels;
        pAboveLeftRef = pRef;
        uAboveLeftPitch = pitchPixels;
        pAboveRightRef = pRef;
        uAboveRightPitch = pitchPixels;
    }

    //Copy pels
    //TOP
    if( top_avbl ){
        for( i=0; i<8; i++ )
            pred_pels[1+i] = *(pAboveRef-uAbovePitch+i);
        pred_pels_mask |= 0x000001fe;
    }

    //LEFT
    if( left_avbl ){
        for( i=0; i<8; i++ )
            pred_pels[17+i] = *(pLeftRef+i*uLeftPitch-1);
            pred_pels_mask |= 0x1fe0000;
    }

    //LEFT_ABOVE
    if( (uBlock == 0 && left_above_avbl) || uBlock == 3 ||
        (uBlock == 1 && top_avbl) || ( uBlock == 2 && left_avbl)){
        pred_pels[0] = *(pAboveLeftRef-uAboveLeftPitch-1);
        pred_pels_mask |= 0x01;
    }

    //RIGHT_ABOVE
    if( (uBlock == 2) || (uBlock == 0 && top_avbl) ||
        (uBlock == 1 && right_above_avbl) ){
        for( i=0; i<8; i++ )
            pred_pels[9+i] = *(pAboveRightRef-uAboveRightPitch+i+8);
        pred_pels_mask |= 0x0001fe00;
    }

    if( !((pred_pels_mask & 0x0001FE00)==0x0001FE00) && (pred_pels_mask & 0x00000100) ){
        pred_pels_mask |= 0x0001FE00;
        for( i=0; i<8; i++ ) pred_pels[9+i] = pred_pels[1+7];
    }

    H264CoreEncoder_Filter8x8Pels(pred_pels, pred_pels_mask);

    if( dcPredModePredictedFlag ){
        intra_pred_mode_A = intra_pred_mode_B = 2; //Check only DC mode
    }else{
        //Derive modes according previously coded macroblocks
        if( mb_A.mbtype != MBTYPE_INTRA ) intra_pred_mode_A = 2;
        else{
            if( pGetMB8x8TSFlag(&mb_A)){
                intra_pred_mode_A = core_enc->m_mbinfo.intra_types[mb_ALoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[mb_ALoc.block_num<<2]]];
            }else{
                if( uBlock != 2 || !core_enc->m_SliceHeader.MbaffFrameFlag  || curr_slice->m_is_cur_mb_field ) //TODO Add frame coded mb_A check
                    n=1;
                else n=3;
                intra_pred_mode_A = core_enc->m_mbinfo.intra_types[mb_ALoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[(mb_ALoc.block_num<<2)+n]]];
            }
        }

        if( mb_B.mbtype != MBTYPE_INTRA ) intra_pred_mode_B = 2;
        else{
            if( pGetMB8x8TSFlag(&mb_B)){
                intra_pred_mode_B = core_enc->m_mbinfo.intra_types[mb_BLoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[mb_BLoc.block_num<<2]]];
            }else{
                intra_pred_mode_B = core_enc->m_mbinfo.intra_types[mb_BLoc.mb_num].intra_types[block_subblock_mapping_[block_subblock_mapping_[(mb_BLoc.block_num<<2)+2]]];
            }
        }
    }

    intra_pred_mode = MIN( intra_pred_mode_A, intra_pred_mode_B );

    //Check all prediction modes for the current MB and take the one with the best SAD
    PIXTYPE uPred[9][64];
    if( intra_pred_mode < 0 ) intra_pred_mode = 2; //use DC

    //Make prediction, check all possible modes
    Ipp32s* modes = NULL;
    Ipp32s nmodes = 0;
    if( ( top_avbl ) && ( left_avbl ) ){
        if (!uBlock && (curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num < 0)){
            modes = &intra_modes[0][0];
            nmodes=5;
        }else{
            modes = &intra_modes[1][0];
            nmodes=8;
        }
    }else if( top_avbl ){
        modes = &intra_modes[2][0];
        nmodes=3;
    }else if( left_avbl ){
        modes = &intra_modes[3][0];
        nmodes=2;
    }

    PIXTYPE* pPred = pPredBuf;
    if ((core_enc->m_Analyse & ANALYSE_FAST_INTRA) && (nmodes == 8)) {
        Ipp32u ModeSAD[10];
        Ipp32s i, k, iQP = cur_mb.lumaQP51, BITS_COST_4 = BITS_COST(4, glob_RDQM[iQP]);
        for (i = 0; i < 9; i ++) {
            H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, i, pred_pels, pred_pels_mask, uPred[i]);
            ModeSAD[i] = SAT8x8D(pSrc, pitchPixels, uPred[i], 8) + BITS_COST_4;
        }
        ModeSAD[intra_pred_mode] += BITS_COST(1, glob_RDQM[iQP]) - BITS_COST_4;
        ModeSAD[9] = MAX_SAD;
        T_AIMode bestMode[3];
        bestMode[0] = 0; bestMode[1] = bestMode[2] = 9;
        for (i = 1; i < 9; i ++) {
            if (ModeSAD[i] >= ModeSAD[bestMode[2]])
                continue;
            if (ModeSAD[i] < ModeSAD[bestMode[0]]) {
                bestMode[2] = bestMode[1]; bestMode[1] = bestMode[0]; bestMode[0] = i;
            } else if (ModeSAD[i] < ModeSAD[bestMode[1]]) {
                bestMode[2] = bestMode[1]; bestMode[1] = i;
            } else
                bestMode[2] = i;
        }
        Ipp32s BestMode = pMode[uBlock<<2];
        for (k = 0; k < 3; k ++) {
            mode = bestMode[k];
            if (BestMode != mode)
                H264BsBase_CopyContextCABAC_I8x8(&fBitstreams[mode]->m_base, &fBitstreams[BestMode]->m_base, !isField);
        }
        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
        c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs8x8[uBlock]);
        c_data->uLastSignificant = cur_mb.m_iLastCoeff8x8[uBlock];
        c_data->CtxBlockCat = BLOCK_LUMA_64_LEVELS;
        c_data->uFirstCoeff = 0;
        c_data->uLastCoeff = 63;
        Ipp32u cost;
        const Ipp32s* scan = dec_single_scan_8x8[isField];
        COEFFSTYPE* transform = &cur_mb.mb8x8.transform[uBlock*64];
        const Ipp32s block4x4 = uBlock*4;
        const PIXTYPE*    recon = cur_mb.mb8x8.reconstruct + xoff[block4x4] + yoff[block4x4]*16;
        best_SAD = MAX_SAD;
        intra_actual_mode = 8;
        for (i = 0; i < 3; i ++) {
            mode = bestMode[i];
            Ipp32s k;
            pPred = pPredBuf;
            for (k = 0; k < 8; k ++) {
                memcpy(pPred, &uPred[mode][k*8], 8*sizeof(PIXTYPE));
                pPred += 16; //pitch = 16
            }
            cur_mb.m_uIntraCBP8x8 |= CBP8x8Mask[uBlock];
            H264CoreEncoder_Encode8x8IntraBlock(state, curr_slice, uBlock);
            cost = SSD8x8(pSrc, pitchPixels, recon, 16 )<<5;
            if( cost > best_SAD ) continue;
            Ipp32s prob0;
            if (intra_pred_mode == mode) {
                prob0 = -1;
            } else if (mode < intra_pred_mode ) {    // Otherwise, the mode probability increases
                prob0 = mode;                      // (the opposite of intuitive notion of probability)
            } else {                                    // with the order of the remaining modes.
                prob0 = mode-1;
            }
            c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs8x8[uBlock]);
            c_data->uLastSignificant = cur_mb.m_iLastCoeff8x8[uBlock];
            H264CoreEncoder_MakeSignificantLists_CABAC(transform, scan, c_data);
            H264BsFake* stream = fBitstreams[mode];
            curr_slice->m_pbitstream = (H264BsBase *)stream;
            H264BsFake_Reset(stream);
            H264BsFake_IntraPredMode_CABAC(stream, prob0);
            if (c_data->uNumSigCoeffs)
                H264BsFake_ResidualBlock_CABAC(stream, c_data, !isField);
            cost += cur_mb.lambda * H264BsFake_GetBsOffset(stream);
            if (cost <= best_SAD) {
                if( cost != best_SAD || mode < intra_actual_mode ){ //Prefer mode with less number
                    best_SAD = cost;
                    intra_actual_mode = mode;
                }
            }
        }
    } else {
        Ipp32s BestMode = pMode[uBlock<<2];
        if (BestMode != 2)
            H264BsBase_CopyContextCABAC_I8x8(
                &fBitstreams[2]->m_base,
                &fBitstreams[BestMode]->m_base,
                !isField);

        for(i = 0; i < nmodes; i++)
        {
            mode = modes[i];
            if (BestMode != mode)
                H264BsBase_CopyContextCABAC_I8x8(
                    &fBitstreams[mode]->m_base,
                    &fBitstreams[BestMode]->m_base,
                    !isField);
        }

        //DC mode prediction, always available
        H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, 2, pred_pels, pred_pels_mask, uPred[2] );
        intra_actual_mode = 2;

        for( i=0; i<8; i++){
             memcpy(pPred, &uPred[intra_actual_mode][i*8], 8*sizeof(PIXTYPE));
             pPred += 16; //pitch = 16
         }
        cur_mb.m_uIntraCBP8x8 |= CBP8x8Mask[uBlock];
        H264CoreEncoder_Encode8x8IntraBlock(state, curr_slice, uBlock);
        best_SAD = SSD8x8(pSrc, pitchPixels, cur_mb.mb8x8.reconstruct + xoff[uBlock*4] + yoff[uBlock*4]*16, 16 )<<5;
        Ipp32s prob0;
        if (intra_pred_mode == 2) {
            prob0 = -1;
        } else if (2 < intra_pred_mode ) {    // Otherwise, the mode probability increases
            prob0 = 2;                      // (the opposite of intuitive notion of probability)
        } else {                                    // with the order of the remaining modes.
            prob0 = 2-1;
        }
        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
        c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs8x8[uBlock]);
        c_data->uLastSignificant = cur_mb.m_iLastCoeff8x8[uBlock];
        c_data->CtxBlockCat = BLOCK_LUMA_64_LEVELS;
        c_data->uFirstCoeff = 0;
        c_data->uLastCoeff = 63;
        H264CoreEncoder_MakeSignificantLists_CABAC(&cur_mb.mb8x8.transform[uBlock*64],dec_single_scan_8x8[isField], c_data);

        curr_slice->m_pbitstream = (H264BsBase *)fBitstreams[2];
        H264BsFake_Reset(fBitstreams[2]);
        H264BsFake_IntraPredMode_CABAC(fBitstreams[2], prob0);
        if (c_data->uNumSigCoeffs)
            H264BsFake_ResidualBlock_CABAC(fBitstreams[2], c_data, !isField);
        Ipp32s bits = H264BsFake_GetBsOffset(fBitstreams[2]);
        best_SAD += cur_mb.lambda*bits;
        if( nmodes ){
            Ipp32u cost;
            const Ipp32s* scan = dec_single_scan_8x8[isField];
            COEFFSTYPE* transform = &cur_mb.mb8x8.transform[uBlock*64];
            const Ipp32s block4x4 = uBlock*4;
            const PIXTYPE*    recon = cur_mb.mb8x8.reconstruct + xoff[block4x4] + yoff[block4x4]*16;

            for( i=0; i<nmodes; i++ ){
                mode = modes[i];
                H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, mode, pred_pels, pred_pels_mask, uPred[mode] );
                Ipp32s k;
                pPred = pPredBuf;
                for( k=0; k<8; k++){
                    memcpy(pPred, &uPred[mode][k*8], 8*sizeof(PIXTYPE));
                    pPred += 16; //pitch = 16
                }
                cur_mb.m_uIntraCBP8x8 |= CBP8x8Mask[uBlock];
                H264CoreEncoder_Encode8x8IntraBlock(state, curr_slice, uBlock);
                cost = SSD8x8(pSrc, pitchPixels, recon, 16 )<<5;
                if( cost > best_SAD ) continue;
                Ipp32s prob0;
                if (intra_pred_mode == mode) {
                    prob0 = -1;
                } else if (mode < intra_pred_mode ) {    // Otherwise, the mode probability increases
                    prob0 = mode;                      // (the opposite of intuitive notion of probability)
                } else {                                    // with the order of the remaining modes.
                    prob0 = mode-1;
                }
                c_data->uNumSigCoeffs = (T_NumCoeffs)ABS(cur_mb.m_iNumCoeffs8x8[uBlock]);
                c_data->uLastSignificant = cur_mb.m_iLastCoeff8x8[uBlock];
                H264CoreEncoder_MakeSignificantLists_CABAC(transform, scan, c_data);

                H264BsFake* stream = fBitstreams[mode];
                curr_slice->m_pbitstream = (H264BsBase *)stream;
                H264BsFake_Reset(stream);
                H264BsFake_IntraPredMode_CABAC(stream, prob0);
                if (c_data->uNumSigCoeffs)
                    H264BsFake_ResidualBlock_CABAC(stream, c_data, !isField);
                cost += cur_mb.lambda * H264BsFake_GetBsOffset(stream);
                if (cost <= best_SAD) {
                    if( cost != best_SAD || mode < intra_actual_mode ){ //Prefer mode with less number
                        best_SAD = cost;
                        intra_actual_mode = mode;
                    }
                }
            }
        }
    }
    //Copy prediction
    if( pPredBuf ){
        pPred = pPredBuf;
        for( i=0; i<8; i++){
            memcpy(pPred, &uPred[intra_actual_mode][i*8], 8*sizeof(PIXTYPE));
            pPred += 16; //pitch = 16
        }
        if (curr_slice->m_use_transform_for_intra_decision){
            cur_mb.m_uIntraCBP8x8 |= CBP8x8Mask[uBlock];
            H264CoreEncoder_Encode8x8IntraBlock(state, curr_slice, uBlock);
        }
    }
    //Save the final mode
    for( i=0; i<4; i++ )  pMode[ (uBlock<<2)+i ] = intra_actual_mode;
    return best_SAD;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_Intra16x16SelectRD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE  *pSrc,            // pointer to upper left pel of source MB
    PIXTYPE  *pRef,            // pointer to same MB in reference picture
    Ipp32s    pitchPixels,     // of source and ref data
    T_AIMode *pMode,           // selected mode goes here
    PIXTYPE  *pPredBuf)        // predictor pels for selected mode goes here
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    PIXTYPE* pAbove;
    PIXTYPE* pLeft;
    __ALIGN16 PIXTYPE uVertPred[64];    // 4 4x4 predictor blocks
    __ALIGN16 PIXTYPE uHorizPred[64];   // 4 4x4 predictor blocks
    Ipp32u i, row;
    Ipp32u uSum = 0;            // to get DC predictor
    Ipp64u uSmallestSAD;
    Ipp64u uSAD;
    Ipp32s tmp_chroma_format_idc;
    Enum16x16PredType Best16x16Type = PRED16x16_VERT;
// !!!!!!!!!!!!!!!!!!!!!!
    H264BsFake* fakeBitstream = curr_slice->fakeBitstream;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32s isFrame = !curr_slice->m_is_cur_mb_field;

    bool topAvailable = cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
    bool leftAvailable = cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
    bool left_above_aval = cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;

    Ipp32s bits;
    H264BsBase* pBitstream = curr_slice->m_pbitstream;
    curr_slice->m_pbitstream = (H264BsBase *)fakeBitstream;
    tmp_chroma_format_idc = cur_mb.chroma_format_idc;
    cur_mb.chroma_format_idc = 0;
/*    if( curr_slice->m_slice_type != INTRASLICE ){
        cnc = cur_mb.MacroblockCoeffsInfo->chromaNC;
        cur_mb.MacroblockCoeffsInfo->chromaNC = 0;
    }
*/
    if (!leftAvailable && !topAvailable){ //only DC available
        *pMode = PRED16x16_DC;
        uSum = 1<<(core_enc->m_PicParamSet.bit_depth_luma - 1);
        MemorySet(pPredBuf, (PIXTYPE)uSum, 256);
        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        H264CoreEncoder_TransQuantIntra16x16_RD(state,  curr_slice );
        uSAD = SSD16x16( pSrc, pitchPixels, cur_mb.mb16x16.reconstruct, 16)<<5;
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_I16x16(&fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->intra_16x16_mode = PRED16x16_DC;
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        curr_slice->m_pbitstream = pBitstream;
        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        cur_mb.chroma_format_idc = tmp_chroma_format_idc;
        bits = H264BsFake_GetBsOffset(fakeBitstream);
        return uSAD+cur_mb.lambda*bits;
    }

    // Initialize uVertPred with prediction from above
    if (topAvailable){
        pAbove = pRef - pitchPixels;
        memcpy(uVertPred, pAbove, 16*sizeof(PIXTYPE));
        for(i = 0; i < 16; i++) {
            // Accumulate sum for DC predictor
            uSum += pAbove[i];
        }

        //Vertical
        for (row=0; row<16; row++) memcpy(pPredBuf + row*16, uVertPred, 16*sizeof(PIXTYPE));
        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        H264CoreEncoder_TransQuantIntra16x16_RD(state,  curr_slice );
        uSAD = SSD16x16( pSrc, pitchPixels, cur_mb.mb16x16.reconstruct, 16)<<5;
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_I16x16(&fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->intra_16x16_mode = PRED16x16_VERT;
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        bits = H264BsFake_GetBsOffset(fakeBitstream);
        uSAD += cur_mb.lambda*bits;
        uSmallestSAD = uSAD;
        Best16x16Type = PRED16x16_VERT;
    }else{
        uSmallestSAD = (((Ipp64u)MAX_SAD)<<8);
    }

    // Initialize uHorizPred with prediction from left
    if (leftAvailable){
        pLeft = pRef - 1;
        // fill each of 4 predictor blocks from left
        for (i=0; i<16; i++, pLeft += pitchPixels) {
            uHorizPred[i*4] = *pLeft;
            uSum += *pLeft;     // accumulate for DC predictor
        }
        //Horizontal
        for (row=0; row<16; row++) MemorySet(pPredBuf+row*16, uHorizPred[row*4], 16);
        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        H264CoreEncoder_TransQuantIntra16x16_RD(state, curr_slice );
        uSAD = SSD16x16( pSrc, pitchPixels, cur_mb.mb16x16.reconstruct, 16 )<<5;
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_I16x16(&fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->intra_16x16_mode = PRED16x16_HORZ;
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        bits = H264BsFake_GetBsOffset(fakeBitstream);
        uSAD += cur_mb.lambda*bits;
        if(uSAD < uSmallestSAD ){
             uSmallestSAD = uSAD;
             Best16x16Type = PRED16x16_HORZ;
        }
    }

    // Initialize uDCPred with average of above and left divide by 32 to get average
    if (!topAvailable || !leftAvailable) uSum <<= 1;
    uSum = (uSum + 16) >> 5;
    MemorySet(pPredBuf, (PIXTYPE)uSum, 256);
    cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
    cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
    H264CoreEncoder_TransQuantIntra16x16_RD(state,  curr_slice );
    uSAD = SSD16x16( pSrc, pitchPixels, cur_mb.mb16x16.reconstruct, 16 )<<5;
    H264BsFake_Reset(fakeBitstream);
    H264BsBase_CopyContextCABAC_I16x16(&fakeBitstream->m_base, pBitstream, isFrame);
    cur_mb.LocalMacroblockInfo->intra_16x16_mode = PRED16x16_DC;
    H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
    H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
    bits = H264BsFake_GetBsOffset(fakeBitstream);
    uSAD += cur_mb.lambda*bits;
    if(uSAD < uSmallestSAD ){
        uSmallestSAD = uSAD;
        Best16x16Type = PRED16x16_DC;
    }

    // Get planar prediction, save 16x16 PIXTYPE result at pPredBuf
    if (leftAvailable && topAvailable && left_above_aval) {
        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        PlanarPredictLuma(pRef, pitchPixels, pPredBuf, core_enc->m_PicParamSet.bit_depth_luma);
        H264CoreEncoder_TransQuantIntra16x16_RD(state,  curr_slice );
        uSAD = SSD16x16( pSrc, pitchPixels, cur_mb.mb16x16.reconstruct, 16 )<<5;
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_I16x16(&fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->intra_16x16_mode = PRED16x16_PLANAR;
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        bits = H264BsFake_GetBsOffset(fakeBitstream);
        uSAD += cur_mb.lambda*bits;
        if(uSAD < uSmallestSAD ){
            uSmallestSAD = uSAD;
            Best16x16Type = PRED16x16_PLANAR;
        }
    }
    // Set MB type for smallest, fill PredBuf with predictors
    switch (Best16x16Type) {
    case PRED16x16_VERT:
        // copy from uVertPred to PredBuf, duplicating for each row of the MB
        for (row=0; row<16; row++) memcpy(pPredBuf + row*16, uVertPred, 16*sizeof(PIXTYPE));
        break;
    case PRED16x16_HORZ:
        // copy from uHorizPred to PredBuf
        for (row=0; row<16; row++) MemorySet(pPredBuf+row*16, uHorizPred[row*4], 16);
        break;
    case PRED16x16_DC:
        // set all prediction pels to the single predictor
        MemorySet(pPredBuf, uSum, 256);
        break;
    case PRED16x16_PLANAR: //Last predicted
        //PlanarPredictLuma(pRef, pitchPixels, pPredBuf, core_enc->m_PicParamSet.bit_depth_luma);
        break;
    default:
        VM_ASSERT(0); // Can't find a suitable intra prediction mode!!!
        break;
    }
    *pMode = (T_AIMode)Best16x16Type;
    curr_slice->m_pbitstream = pBitstream;
    cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
    cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
    cur_mb.chroma_format_idc = tmp_chroma_format_idc;
/*    if( curr_slice->m_slice_type != INTRASLICE ){
        cur_mb.MacroblockCoeffsInfo->chromaNC = cnc;
    }
*/
 //f   uSmallestSAD += SATD16x16(pSrc, pitchPixels, pPredBuf, 16);
    return uSmallestSAD;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_IntraSelectChromaRD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    PIXTYPE* pUSrc,             // pointer to upper left pel of U source MB
    PIXTYPE* pURef,             // pointer to same MB in U reference picture
    PIXTYPE* pVSrc,             // pointer to upper left pel of V source MB
    PIXTYPE* pVRef,             // pointer to same MB in V reference picture
    Ipp32u   uPitch,            // of source and ref data
    Ipp8u*   pMode,             // selected mode goes here
    PIXTYPE *pUPredBuf,         // U predictor pels for selected mode go here
    PIXTYPE *pVPredBuf)         // V predictor pels for selected mode go here
{
    pUSrc = pUSrc;
    pVSrc = pVSrc;
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp64u uSAD,uSmallestSAD;
    __ALIGN16 PIXTYPE uVertPred[2][64];   // predictors from above, 4 4x4 Predictor blocks, U & V //first up lines for each block, then line+1 for each block and so on
    __ALIGN16 PIXTYPE uHorizPred[2][64];  // predictors from left, 4 4x4 Predictor blocks, U & V
    Ipp32u uSum[2][16] =  {{0},{0}};     // for DC, U & V - 16 predictors (a - d) each
    PIXTYPE *pAbove, *pLeft;
    Enum8x8PredType Best8x8Type = PRED8x8_DC;
    Ipp32u i, j, plane, num_rows, num_cols, wide, blocks,idx;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    bool topAvailable = cur_mb.BlockNeighbours.mb_above.mb_num >= 0;
    bool leftAvailable = cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;
    bool left_above_aval = cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
    H264BsFake* fakeBitstream = curr_slice->fakeBitstream;
    H264BsBase* pBitstream = curr_slice->m_pbitstream;
    curr_slice->m_pbitstream = (H264BsBase *)fakeBitstream;
    Ipp32s isFrame = !curr_slice->m_is_cur_mb_field;
    Ipp32s uOffset = core_enc->m_pMBOffsets[cur_mb.uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field];
    Ipp32u chromaNC = 0;

    //f
    Ipp32u iQP = cur_mb.chromaQP;
//    Ipp32s l = int( (pow(2.0,MAX(0,(iQP-12))/3.0)*0.85) + 0.5);
    const Ipp32s l = lambda_sq[iQP];

    wide = ((core_enc->m_PicParamSet.chroma_format_idc-1) & 0x2)>>1;
    num_cols = (wide+1)<<3;
    num_rows = (((core_enc->m_PicParamSet.chroma_format_idc&0x2)>>1)+1)<<3;
    blocks = 2<<core_enc->m_PicParamSet.chroma_format_idc;
    cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA;
    Ipp32s left_p = (cur_mb.BlockNeighbours.mbs_left[0].mb_num < 0 ? 0 : core_enc->m_mbinfo.mbs[cur_mb.BlockNeighbours.mbs_left[0].mb_num].intra_chroma_mode) != 0;
    Ipp32s top_p = (cur_mb.BlockNeighbours.mb_above.mb_num < 0 ? 0 : core_enc->m_mbinfo.mbs[cur_mb.BlockNeighbours.mb_above.mb_num].intra_chroma_mode) != 0;


//FIXME 420 only at the moment

    if (!topAvailable && !leftAvailable) { //Only DC available
//        Ipp32s size = 32<<m_PicParamSet.chroma_format_idc;
        Ipp32s row;
        for (row=0; row<8; row++) MemorySet(pUPredBuf+row*16, 1<<(core_enc->m_params.bit_depth_chroma - 1), 8);
        for (row=0; row<8; row++) MemorySet(pVPredBuf+row*16, 1<<(core_enc->m_params.bit_depth_chroma - 1), 8);
        H264CoreEncoder_TransQuantChromaIntra_RD(state, curr_slice );
//        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels);
//        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels);
        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct, 16);
        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct+8, 16);
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_Chroma(&curr_slice->fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
        cur_mb.LocalMacroblockInfo->intra_chroma_mode = PRED8x8_DC;
        H264BsFake_ChromaIntraPredMode_CABAC(
            fakeBitstream,
            cur_mb.LocalMacroblockInfo->intra_chroma_mode,
            left_p,
            top_p);

        H264CoreEncoder_Put_MBChroma_Fake(state, curr_slice );
        curr_slice->m_pbitstream = pBitstream;
        *pMode = PRED8x8_DC;
        return (uSAD<<5)+cur_mb.lambda * H264BsFake_GetBsOffset(fakeBitstream);
    }

    // Initialize uVertPred with prediction from above
    if (topAvailable){
        for (plane = 0; plane < 2; plane++) {
            pAbove = plane ? pVRef - uPitch : pURef - uPitch;
            // Get predictors from above and copy into 4x4 blocks for SAD calculations
            memcpy(uVertPred[plane], pAbove, 8*sizeof(PIXTYPE));
            for (i=0; i<num_cols; i++) {
                uSum[plane][i>>2] += pAbove[i];     // accumulate to A & B for DC predictor
            }
            //Copy down
            if( core_enc->m_PicParamSet.chroma_format_idc == 1){
               for( i=0; i<2; i++ )
                 uSum[plane][i+4] = uSum[plane][i];
            }else{
               for( i=0; i<(num_cols>>2); i++ )
                 uSum[plane][i+4] = uSum[plane][i+8] = uSum[plane][i+12] = uSum[plane][i];
            }
        }

        //Vertical
        Ipp32s row;
        for (row=0; row<8; row++) memcpy(pUPredBuf + row*16, uVertPred[0], 8*sizeof(PIXTYPE));
        for (row=0; row<8; row++) memcpy(pVPredBuf + row*16, uVertPred[1], 8*sizeof(PIXTYPE));
        H264CoreEncoder_TransQuantChromaIntra_RD(state, curr_slice );
//        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels);
//        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels);
        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct, 16);
        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct+8, 16);
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_Chroma(&curr_slice->fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
        cur_mb.LocalMacroblockInfo->intra_chroma_mode = PRED8x8_VERT;
        H264BsFake_ChromaIntraPredMode_CABAC(
            fakeBitstream,
            cur_mb.LocalMacroblockInfo->intra_chroma_mode,
            left_p,
            top_p);

        H264CoreEncoder_Put_MBChroma_Fake(state, curr_slice );
        uSAD <<= 5;
        uSAD += l * H264BsFake_GetBsOffset(fakeBitstream);
        uSmallestSAD = uSAD;
        chromaNC = cur_mb.MacroblockCoeffsInfo->chromaNC;
        Best8x8Type = PRED8x8_VERT;
    }else{
        uSmallestSAD = (Ipp64u)MAX_SAD<<8;
    }

    // Initialize uHorizPred with prediction from left
    if (leftAvailable){
        //PRED8x8_HORZ
        for (plane = 0; plane < 2; plane++) {
            Ipp32u tmpSum = uSum[plane][1];
            uSum[plane][4] = 0;     // Reset Block C to zero in this case.
            if( core_enc->m_PicParamSet.chroma_format_idc == 2){
                    uSum[plane][8] = 0;
                    uSum[plane][12] = 0;
            }
            pLeft = plane ? pVRef - 1 : pURef - 1;
            // Get predictors from the left and copy into 4x4 blocks for SAD calculations
            for (i=0; i<num_rows; i++){
                idx = i & ~0x03; //Reset low 2 bits
                uSum[plane][idx] += *pLeft;    // accumulate to A or C for DC predictor
                uSum[plane][idx+1] += *pLeft;  // accumulate to B or D for DC predictor
                uHorizPred[plane][i] = *pLeft;
                pLeft += uPitch;
            }
            if (topAvailable){                // Conditionally restore the previous sum
                uSum[plane][1] = tmpSum;    // unless this is on the top edge
            }
        }
        //Horizontal
        Ipp32s row;
        for (row=0; row<8; row++) MemorySet(pUPredBuf+row*16, uHorizPred[0][row], 8);
        for (row=0; row<8; row++) MemorySet(pVPredBuf+row*16, uHorizPred[1][row], 8);
        H264CoreEncoder_TransQuantChromaIntra_RD(state, curr_slice );
//        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels);
//        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels);
        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct, 16);
        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct+8, 16);
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_Chroma(&curr_slice->fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
        cur_mb.LocalMacroblockInfo->intra_chroma_mode = PRED8x8_HORZ;
        H264BsFake_ChromaIntraPredMode_CABAC(
            fakeBitstream,
            cur_mb.LocalMacroblockInfo->intra_chroma_mode,
            left_p,
            top_p);

        H264CoreEncoder_Put_MBChroma_Fake(state, curr_slice );
        uSAD <<= 5;
        uSAD += l*H264BsFake_GetBsOffset(fakeBitstream);
        if(uSAD < uSmallestSAD ){
             chromaNC = cur_mb.MacroblockCoeffsInfo->chromaNC;
             uSmallestSAD = uSAD;
             Best8x8Type = PRED8x8_HORZ;
        }
    }

    //DC prediction
       for (plane = 0; plane < 2; plane++) {
            // Divide & round A & D properly, depending on how many terms are in the sum.
            if (topAvailable && leftAvailable) {
                // 8 Pixels
                uSum[plane][0] = (uSum[plane][0] + 4) >> 3;
                uSum[plane][5] = (uSum[plane][5] + 4) >> 3;
            } else {
                // 4 pixels
                uSum[plane][0] = (uSum[plane][0] + 2) >> 2;
                uSum[plane][5] = (uSum[plane][5] + 2) >> 2;
            }
            // Always 4 pixels
            uSum[plane][1] = (uSum[plane][1] + 2) >> 2;
            uSum[plane][4] = (uSum[plane][4] + 2) >> 2;
            if( core_enc->m_PicParamSet.chroma_format_idc == 2){
                    uSum[plane][8] = (uSum[plane][8] + 2) >> 2;
                    uSum[plane][12] = (uSum[plane][12] + 2) >> 2;
                    if (topAvailable && leftAvailable) {
                        uSum[plane][9] = (uSum[plane][9] + 4) >> 3;
                        uSum[plane][13] = (uSum[plane][13] + 4) >> 3;
                    }else{
                        uSum[plane][9] = (uSum[plane][9] + 2) >> 2;
                        uSum[plane][13] = (uSum[plane][13] + 2) >> 2;
                    }
            }
        }

        for( j = 0; j<blocks; j++ ){
            Ipp32s row;
            for (row=0; row<4; row++) MemorySet(pUPredBuf+row*16 + ((j>>1)<<6) + ((j&1)<<2),  (PIXTYPE)uSum[0][((j>>1)<<2) + (j&0x01)], 4);
            for (row=0; row<4; row++) MemorySet(pVPredBuf+row*16 + ((j>>1)<<6) + ((j&1)<<2),  (PIXTYPE)uSum[1][((j>>1)<<2) + (j&0x01)], 4);
        }

        H264CoreEncoder_TransQuantChromaIntra_RD(state, curr_slice );
//        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels);
//        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels);
        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct, 16);
        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct+8, 16);
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_Chroma(&curr_slice->fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
        cur_mb.LocalMacroblockInfo->intra_chroma_mode = PRED8x8_DC;
        H264BsFake_ChromaIntraPredMode_CABAC(
            fakeBitstream,
            cur_mb.LocalMacroblockInfo->intra_chroma_mode,
            left_p,
            top_p);

        H264CoreEncoder_Put_MBChroma_Fake(state, curr_slice );
        uSAD <<= 5;
        uSAD += l*H264BsFake_GetBsOffset(fakeBitstream);
        if(uSAD < uSmallestSAD ){
             chromaNC = cur_mb.MacroblockCoeffsInfo->chromaNC;
             uSmallestSAD = uSAD;
             Best8x8Type = PRED8x8_DC;
        }


    // Get planar prediction, save 8x8 PIXTYPE result at pPredBuf,
    if (topAvailable && leftAvailable && left_above_aval) {
        PlanarPredictChroma(pURef, uPitch, pUPredBuf, core_enc->m_params.bit_depth_chroma, core_enc->m_PicParamSet.chroma_format_idc);
        PlanarPredictChroma(pVRef, uPitch, pVPredBuf, core_enc->m_params.bit_depth_chroma, core_enc->m_PicParamSet.chroma_format_idc);
        H264CoreEncoder_TransQuantChromaIntra_RD(state, curr_slice );
//        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels);
//        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels);
        uSAD = SSD8x8(core_enc->m_pCurrentFrame->m_pUPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct, 16);
        uSAD += SSD8x8(core_enc->m_pCurrentFrame->m_pVPlane + uOffset, cur_mb.mbPitchPixels, cur_mb.mbChromaIntra.reconstruct+8, 16);
        H264BsFake_Reset(fakeBitstream);
        H264BsBase_CopyContextCABAC_Chroma(&curr_slice->fakeBitstream->m_base, pBitstream, isFrame);
        cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
        cur_mb.LocalMacroblockInfo->intra_chroma_mode = PRED8x8_PLANAR;
        H264BsFake_ChromaIntraPredMode_CABAC(
            fakeBitstream,
            cur_mb.LocalMacroblockInfo->intra_chroma_mode,
            left_p,
            top_p);

        H264CoreEncoder_Put_MBChroma_Fake(state, curr_slice );
        uSAD <<= 5;
        uSAD += l*H264BsFake_GetBsOffset(fakeBitstream);
        if(uSAD < uSmallestSAD ){
             chromaNC = cur_mb.MacroblockCoeffsInfo->chromaNC;
             uSmallestSAD = uSAD;
             Best8x8Type = PRED8x8_PLANAR;
        }
    }

    // Set MB type for smallest, fill PredBuf with predictors
    switch (Best8x8Type) {
    case PRED8x8_VERT:
        for (i=0; i<8; i++) memcpy(pUPredBuf + i*16, uVertPred[0], 8*sizeof(PIXTYPE));
        for (i=0; i<8; i++) memcpy(pVPredBuf + i*16, uVertPred[1], 8*sizeof(PIXTYPE));
        break;
    case PRED8x8_HORZ:
        for (i=0; i<num_rows; i++){
           MemorySet(pUPredBuf+i*16, uHorizPred[0][i], 8);
           MemorySet(pVPredBuf+i*16, uHorizPred[1][i], 8);
        }
        break;
    case PRED8x8_DC:
        for( j = 0; j<blocks; j++ ){
            Ipp32s row;
            for (row=0; row<4; row++) MemorySet(pUPredBuf+row*16 + ((j>>1)<<6) + ((j&1)<<2),  (PIXTYPE)uSum[0][((j>>1)<<2) + (j&0x01)], 4);
            for (row=0; row<4; row++) MemorySet(pVPredBuf+row*16 + ((j>>1)<<6) + ((j&1)<<2),  (PIXTYPE)uSum[1][((j>>1)<<2) + (j&0x01)], 4);
        }
        break;
    case PRED8x8_PLANAR:
        //  nothing to do, planar prediction already filled PredBuf
        break;
    default:
        VM_ASSERT( 0 /*Can't find the best intra prediction 8x8!!!*/);
        break;
    }

    cur_mb.MacroblockCoeffsInfo->chromaNC = chromaNC;
    curr_slice->m_pbitstream = pBitstream;
    *pMode = (Ipp8u)Best8x8Type;
    return uSmallestSAD;
}

// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template void   H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u, Ipp32u*);
template void   H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock8x8<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u, Ipp32u*);
template Ipp32u H264CoreEncoder_AIModeSelectChromaMBs_8x8<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, PIXTYPE*, PIXTYPE*, Ipp32u, Ipp8u*, PIXTYPE*, PIXTYPE*);
template Ipp32u H264CoreEncoder_IntraSelectChromaRD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, PIXTYPE*, PIXTYPE*, Ipp32u, Ipp8u*, PIXTYPE*, PIXTYPE*);
template Ipp32u H264CoreEncoder_AIModeSelectOneMB_16x16<COEFFSTYPE, PIXTYPE>(void*,H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, Ipp32s, T_AIMode*, PIXTYPE*);
template Ipp32u H264CoreEncoder_Intra16x16SelectRD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, Ipp32s, T_AIMode*, PIXTYPE*);
template void   H264CoreEncoder_Filter8x8Pels<PIXTYPE>(PIXTYPE*, Ipp32u);
template void   H264CoreEncoder_GetBlockPredPels<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, Ipp32u, PIXTYPE*, Ipp32u, PIXTYPE*, Ipp32u, Ipp32u, PIXTYPE*);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template void   H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u, Ipp32u*);
template void   H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock8x8<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u, Ipp32u*);
template Ipp32u H264CoreEncoder_AIModeSelectChromaMBs_8x8<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, PIXTYPE*, PIXTYPE*, Ipp32u, Ipp8u*, PIXTYPE*, PIXTYPE*);
template Ipp32u H264CoreEncoder_IntraSelectChromaRD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, PIXTYPE*, PIXTYPE*, Ipp32u, Ipp8u*, PIXTYPE*, PIXTYPE*);
template Ipp32u H264CoreEncoder_AIModeSelectOneMB_16x16<COEFFSTYPE, PIXTYPE>(void*,H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, Ipp32s, T_AIMode*, PIXTYPE*);
template Ipp32u H264CoreEncoder_Intra16x16SelectRD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, PIXTYPE*, Ipp32s, T_AIMode*, PIXTYPE*);
template void   H264CoreEncoder_Filter8x8Pels<PIXTYPE>(PIXTYPE*, Ipp32u);
template void   H264CoreEncoder_GetBlockPredPels<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*, Ipp32u, PIXTYPE*, Ipp32u, PIXTYPE*, Ipp32u, Ipp32u, PIXTYPE*);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER

