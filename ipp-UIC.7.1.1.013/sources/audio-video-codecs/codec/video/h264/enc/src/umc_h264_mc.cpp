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
#include "umc_h264_video_encoder.h"
#include "umc_h264_tables.h"
#include "umc_h264_bme.h"

// pitch of the prediction buffer which is the destination buffer for the
// functions in this file

#define DEST_PITCH 16

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct{
    Ipp32s block_index;
    Ipp32s block_w;
    Ipp32s block_h;
    Direction_t direction;
} BlocksBidirInfo;

BlocksBidirInfo bidir_blocks_infos[] =
{
    {0, 4, 4, D_DIR_FWD}, // 0 - MBTYPE_INTER

    {0, 4, 4, D_DIR_BWD}, // 1 - MBTYPE_BACKWARD

    {0, 4, 2, D_DIR_FWD}, // 2 - MBTYPE_FWD_FWD_16x8, MBTYPE_INTER_16x8
    {8, 4, 2, D_DIR_FWD}, // 3 - MBTYPE_FWD_FWD_16x8, MBTYPE_INTER_16x8

    {0, 2, 4, D_DIR_FWD}, // 4 - MBTYPE_FWD_FWD_8x16, MBTYPE_INTER_8x16
    {2, 2, 4, D_DIR_FWD}, // 5 - MBTYPE_FWD_FWD_8x16, MBTYPE_INTER_8x16

    {0, 4, 2, D_DIR_BWD}, // 6 - MBTYPE_BWD_BWD_16x8
    {8, 4, 2, D_DIR_BWD}, // 7 - MBTYPE_BWD_BWD_16x8

    {0, 2, 4, D_DIR_BWD}, // 8 - MBTYPE_BWD_BWD_8x16
    {2, 2, 4, D_DIR_BWD}, // 9 - MBTYPE_BWD_BWD_8x16

    {0, 4, 2, D_DIR_FWD}, // 10 - MBTYPE_FWD_BWD_16x8
    {8, 4, 2, D_DIR_BWD}, // 11 - MBTYPE_FWD_BWD_16x8

    {0, 2, 4, D_DIR_FWD}, // 12 - MBTYPE_FWD_BWD_8x16
    {2, 2, 4, D_DIR_BWD}, // 13 - MBTYPE_FWD_BWD_8x16

    {8, 4, 2, D_DIR_FWD}, // 14 - MBTYPE_BWD_FWD_16x8
    {0, 4, 2, D_DIR_BWD}, // 15 - MBTYPE_BWD_FWD_16x8

    {2, 2, 4, D_DIR_FWD}, // 16 - MBTYPE_BWD_FWD_8x16
    {0, 2, 4, D_DIR_BWD}, // 17 - MBTYPE_BWD_FWD_8x16

    {0, 4, 2, D_DIR_BIDIR}, // 18 - MBTYPE_BIDIR_FWD_16x8
    {8, 4, 2, D_DIR_FWD},   // 19 - MBTYPE_BIDIR_FWD_16x8

    {0, 4, 2, D_DIR_FWD},   // 20 - MBTYPE_FWD_BIDIR_16x8
    {8, 4, 2, D_DIR_BIDIR}, // 21 - MBTYPE_FWD_BIDIR_16x8

    {0, 4, 2, D_DIR_BIDIR}, // 22 - MBTYPE_BIDIR_BWD_16x8
    {8, 4, 2, D_DIR_BWD},   // 23 - MBTYPE_BIDIR_BWD_16x8

    {0, 4, 2, D_DIR_BWD},   // 24 - MBTYPE_BWD_BIDIR_16x8
    {8, 4, 2, D_DIR_BIDIR}, // 25 - MBTYPE_BWD_BIDIR_16x8

    {0, 4, 2, D_DIR_BIDIR}, // 26 - MBTYPE_BIDIR_BIDIR_16x8
    {8, 4, 2, D_DIR_BIDIR}, // 27 - MBTYPE_BIDIR_BIDIR_16x8

    {0, 2, 4, D_DIR_BIDIR}, // 28 - MBTYPE_BIDIR_FWD_8x16
    {2, 2, 4, D_DIR_FWD},   // 29 - MBTYPE_BIDIR_FWD_8x16

    {0, 2, 4, D_DIR_FWD},   // 30 - MBTYPE_FWD_BIDIR_8x16
    {2, 2, 4, D_DIR_BIDIR}, // 31 - MBTYPE_FWD_BIDIR_8x16

    {0, 2, 4, D_DIR_BIDIR}, // 32 - MBTYPE_BIDIR_BWD_8x16
    {2, 2, 4, D_DIR_BWD},   // 33 - MBTYPE_BIDIR_BWD_8x16

    {0, 2, 4, D_DIR_BWD},   // 34 - MBTYPE_BWD_BIDIR_8x16
    {2, 2, 4, D_DIR_BIDIR}, // 35 - MBTYPE_BWD_BIDIR_8x16

    {0, 2, 4, D_DIR_BIDIR}, // 36 - MBTYPE_BIDIR_BIDIR_8x16
    {2, 2, 4, D_DIR_BIDIR}, // 37 - MBTYPE_BIDIR_BIDIR_8x16

    {0, 4, 4, D_DIR_BIDIR}, // 38 - MBTYPE_BIDIR

    {0, 2, 2, D_DIR_FWD}, // 39 - MBTYPE_B_8x8 / SBTYPE_FORWARD_8x8
    {0, 2, 1, D_DIR_FWD}, // 40 - MBTYPE_B_8x8 / SBTYPE_FORWARD_8x4
    {0, 1, 2, D_DIR_FWD}, // 41 - MBTYPE_B_8x8 / SBTYPE_FORWARD_4x8
    {0, 1, 1, D_DIR_FWD}, // 42 - MBTYPE_B_8x8 / SBTYPE_FORWARD_4x4

    {0, 2, 2, D_DIR_BWD}, // 43 - MBTYPE_B_8x8 / SBTYPE_BACKWARD_8x8
    {0, 2, 1, D_DIR_BWD}, // 44 - MBTYPE_B_8x8 / SBTYPE_BACKWARD_8x4
    {0, 1, 2, D_DIR_BWD}, // 45 - MBTYPE_B_8x8 / SBTYPE_BACKWARD_4x8
    {0, 1, 1, D_DIR_BWD}, // 46 - MBTYPE_B_8x8 / SBTYPE_BACKWARD_4x4

    {0, 2, 2, D_DIR_BIDIR}, // 47 - MBTYPE_B_8x8 / SBTYPE_BIDIR_8x8
    {0, 2, 1, D_DIR_BIDIR}, // 48 - MBTYPE_B_8x8 / SBTYPE_BIDIR_8x4
    {0, 1, 2, D_DIR_BIDIR}, // 49 - MBTYPE_B_8x8 / SBTYPE_BIDIR_4x8
    {0, 1, 1, D_DIR_BIDIR}, // 50 - MBTYPE_B_8x8 / SBTYPE_BIDIR_4x4

    {-1, 4, 4, D_DIR_DIRECT}, // 51 - MBTYPE_DIRECT
    {0, 2, 2, D_DIR_DIRECT},  // 52 - MBTYPE_DIRECT_8x8
};

typedef struct{
    Ipp32s size;
    BlocksBidirInfo *blocks;
} Bidir_Blocks_Info;

void get_bidir_info(MBTypeValue mb_type, SBTypeValue *sub_type, Bidir_Blocks_Info & bidir_info)
{
    const Ipp32s blockpos[4] = {0, 2, 8, 10};

    switch (mb_type)
    {
    case MBTYPE_DIRECT:
        bidir_info.blocks = &bidir_blocks_infos[51];
        bidir_info.size = 1;
        break;

    case MBTYPE_INTER:
    case MBTYPE_FORWARD:
    case MBTYPE_SKIPPED:  //only for P frames
        bidir_info.blocks = &bidir_blocks_infos[0];
        bidir_info.size = 1;
        break;

    case MBTYPE_BACKWARD:
        bidir_info.blocks = &bidir_blocks_infos[1];
        bidir_info.size = 1;
        break;

    case MBTYPE_FWD_FWD_16x8:
    case MBTYPE_INTER_16x8:
        bidir_info.blocks = &bidir_blocks_infos[2];
        bidir_info.size = 2;
        break;

    case MBTYPE_FWD_FWD_8x16:
    case MBTYPE_INTER_8x16:
        bidir_info.blocks = &bidir_blocks_infos[4];
        bidir_info.size = 2;
        break;

    case MBTYPE_BWD_BWD_16x8:
        bidir_info.blocks = &bidir_blocks_infos[6];
        bidir_info.size = 2;
        break;

    case MBTYPE_BWD_BWD_8x16:
        bidir_info.blocks = &bidir_blocks_infos[8];
        bidir_info.size = 2;
        break;

    case MBTYPE_FWD_BWD_16x8:
        bidir_info.blocks = &bidir_blocks_infos[10];
        bidir_info.size = 2;
        break;

    case MBTYPE_FWD_BWD_8x16:
        bidir_info.blocks = &bidir_blocks_infos[12];
        bidir_info.size = 2;
        break;

    case MBTYPE_BWD_FWD_16x8:
        bidir_info.blocks = &bidir_blocks_infos[14];
        bidir_info.size = 2;
        break;

    case MBTYPE_BWD_FWD_8x16:
        bidir_info.blocks = &bidir_blocks_infos[16];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR_FWD_16x8:
        bidir_info.blocks = &bidir_blocks_infos[18];
        bidir_info.size = 2;
        break;

    case MBTYPE_FWD_BIDIR_16x8:
        bidir_info.blocks = &bidir_blocks_infos[20];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR_BWD_16x8:
        bidir_info.blocks = &bidir_blocks_infos[22];
        bidir_info.size = 2;
        break;

    case MBTYPE_BWD_BIDIR_16x8:
        bidir_info.blocks = &bidir_blocks_infos[24];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR_BIDIR_16x8:
        bidir_info.blocks = &bidir_blocks_infos[26];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR_FWD_8x16:
        bidir_info.blocks = &bidir_blocks_infos[28];
        bidir_info.size = 2;
        break;

    case MBTYPE_FWD_BIDIR_8x16:
        bidir_info.blocks = &bidir_blocks_infos[30];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR_BWD_8x16:
        bidir_info.blocks = &bidir_blocks_infos[32];
        bidir_info.size = 2;
        break;

    case MBTYPE_BWD_BIDIR_8x16:
        bidir_info.blocks = &bidir_blocks_infos[34];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR_BIDIR_8x16:
        bidir_info.blocks = &bidir_blocks_infos[36];
        bidir_info.size = 2;
        break;

    case MBTYPE_BIDIR:
        bidir_info.blocks = &bidir_blocks_infos[38];
        bidir_info.size = 1;
        break;

    case MBTYPE_INTER_8x8_REF0:
    case MBTYPE_INTER_8x8:
        {
        BlocksBidirInfo *inf = bidir_info.blocks;
        bidir_info.size = 0;
        for (Ipp32s block = 0; block < 4; block++)
        {
            switch (*sub_type)
            {
                case SBTYPE_8x8:
                    *inf = bidir_blocks_infos[39];
                    inf->block_index = blockpos[block];
                    inf++;
                    bidir_info.size++;
                    break;

                case SBTYPE_8x4:
                    *inf = bidir_blocks_infos[40];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[40];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_4x8:
                    *inf = bidir_blocks_infos[41];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[41];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_4x4:
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block] + 5;
                    inf++;
                    bidir_info.size += 4;
                    break;

                default:
                    VM_ASSERT(false);
                    break;
            }

            sub_type++;
        };
        }
        break;

    case MBTYPE_B_8x8:
        {
        BlocksBidirInfo *inf = bidir_info.blocks;
        bidir_info.size = 0;

        SBTypeValue *stype = sub_type;
        for (Ipp32s block = 0; block < 4; block++)
        {
            switch (*stype)
            {
                case SBTYPE_DIRECT:
                    *inf = bidir_blocks_infos[52];
                    // inf->block_index = blockpos[block];
                    inf->block_index = block; // Special case !
                    inf++;
                    bidir_info.size++;
                    break;

                case SBTYPE_FORWARD_8x8:
                    *inf = bidir_blocks_infos[39];
                    inf->block_index = blockpos[block];
                    inf++;
                    bidir_info.size++;
                    break;

                case SBTYPE_BIDIR_8x8:
                    *inf = bidir_blocks_infos[47];
                    inf->block_index = blockpos[block];
                    inf++;
                    bidir_info.size++;
                    break;

                case SBTYPE_BACKWARD_8x8:
                    *inf = bidir_blocks_infos[43];
                    inf->block_index = blockpos[block];
                    inf++;
                    bidir_info.size++;
                    break;

                case SBTYPE_FORWARD_8x4:
                    *inf = bidir_blocks_infos[40];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[40];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_BACKWARD_8x4:
                    *inf = bidir_blocks_infos[44];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[44];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_BIDIR_8x4:
                    *inf = bidir_blocks_infos[48];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[48];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_FORWARD_4x8:
                    *inf = bidir_blocks_infos[41];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[41];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_BACKWARD_4x8:
                    *inf = bidir_blocks_infos[45];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[45];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_BIDIR_4x8:
                    *inf = bidir_blocks_infos[49];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[49];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    bidir_info.size += 2;
                    break;

                case SBTYPE_FORWARD_4x4:
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    *inf = bidir_blocks_infos[42];
                    inf->block_index = blockpos[block] + 5;
                    inf++;
                    bidir_info.size += 4;
                    break;

                case SBTYPE_BIDIR_4x4:
                    *inf = bidir_blocks_infos[50];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[50];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    *inf = bidir_blocks_infos[50];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    *inf = bidir_blocks_infos[50];
                    inf->block_index = blockpos[block] + 5;
                    inf++;
                    bidir_info.size += 4;
                    break;

                case SBTYPE_BACKWARD_4x4:
                    *inf = bidir_blocks_infos[46];
                    inf->block_index = blockpos[block];
                    inf++;
                    *inf = bidir_blocks_infos[46];
                    inf->block_index = blockpos[block] + 1;
                    inf++;
                    *inf = bidir_blocks_infos[46];
                    inf->block_index = blockpos[block] + 4;
                    inf++;
                    *inf = bidir_blocks_infos[46];
                    inf->block_index = blockpos[block] + 5;
                    inf++;
                    bidir_info.size += 4;
                    break;

                default:
                    break;
            }

            stype++;
        }
        }
        break;

    default:
        VM_ASSERT(false);
        bidir_info.size = 0;
        break;
    }
}

#define TRUNCATE_LO(val, lim)      \
    { Ipp32s (tmp) = (lim);                      \
    if ((tmp) < (val))                  \
        (val) = (Ipp16s) (tmp);}

#define TRUNCATE_HI(val, lim)      \
    {Ipp32s (tmp) = (lim);                      \
    if ((tmp) > (val))                  \
        (val) = (Ipp16s) (tmp);}





//////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////
// C_DirectB_PredictOneMB_Cr
//
// Predict chroma values of current direct B mode macroblock by interpolating
// from previous and future references.
//
////////////////////////////////////////////////////////////////////////////////
template<typename PIXTYPE>
inline void DirectB_PredictOneMB_Cr(
    PIXTYPE *const        pDirB,      // pointer to current direct mode MB buffer
    const PIXTYPE *const  pPrev,      // pointer to previous ref plane buffer
    const PIXTYPE *const  pFutr,      // pointer to future ref plane buffer
    const Ipp32s          pitchPixels,  // reference buffers pitch in pixels
    const Ipp32u        uInterpType,// 0 = Skip, 1 = Default, 2 = Implicit Weighted
    const Ipp32s        W0,
    const Ipp32s        W1,
    const IppiSize & roiSize)
{
    if (!uInterpType)
    {
        for (Ipp32s i = 0, k = 0; i < roiSize.height; i ++, k += pitchPixels)
            memcpy(pDirB + i * 16, pPrev + k, roiSize.width * sizeof(PIXTYPE));
    }
    else if (uInterpType == 1)
    {
        ownInterpolateBlock_H264(pPrev, pFutr, pDirB, roiSize.width, roiSize.height, pitchPixels);

    } else  {
        for (Ipp32s i = 0, k = 0; i < roiSize.height; i ++, k += pitchPixels)
            for (Ipp32s j = 0; j < roiSize.width; j ++)
            {
                pDirB[i * 16 + j] = (PIXTYPE) ((pPrev[k + j] * W0 + pFutr[k + j] * W1 + 32) >> 6);
            }
    }
} // C_DirectB_PredictOneMB_Cr

template<typename PIXTYPE>
inline void GetImplicitBidirPred(
    PIXTYPE *const        pDirB,      // pointer to current direct mode MB buffer
    const PIXTYPE *const  pPrev,      // pointer to previous ref plane buffer
    const PIXTYPE *const  pFutr,      // pointer to future ref plane buffer
    const Ipp32u          pitchPixels,  // reference buffers pitch in pixels
    const IppiSize& roiSize)
{
    ownInterpolateBlock_H264(pPrev, pFutr, pDirB, roiSize.width, roiSize.height, pitchPixels);
} // C_DirectB_PredictOneMB_Cr

////////////////////////////////////////////////////////////////////////////////
// MCOneMBLuma
//
//  Copy all 16 4x4 luma blocks from reference source using the input motion
//  vectors to locate and compute the reference pels using interpolation as
//  required. The vectors are in subpel units. Results are stored in the output
//  buffer as a 16x16 block with a row pitch of DEST_PITCH.
//
////////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_MCOneMBLuma(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    const H264MotionVector *pMVFwd,   // motion vectors in subpel units
    const H264MotionVector *pMVBwd,   // motion vectors in subpel units
    PIXTYPE* pDst)                 // put the resulting block here
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u uBlock;
    PIXTYPE *pMCDst;
    const H264MotionVector *pMCMV;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMB = cur_mb.uMB;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    MB_Type uMBType = (MB_Type)(cur_mb.GlobalMacroblockInfo->mbtype);  // type of MB
    T_RefIdx *pRefIdxL0 = cur_mb.RefIdxs[LIST_0]->RefIdxs;
    T_RefIdx *pRefIdxL1 = cur_mb.RefIdxs[LIST_1]->RefIdxs;
    H264EncoderFrame<PIXTYPE> **pRefPicList0 = GetRefPicList(curr_slice, LIST_0, is_cur_mb_field,uMB&1)->m_RefPicList;
    H264EncoderFrame<PIXTYPE> **pRefPicList1 = GetRefPicList(curr_slice, LIST_1, is_cur_mb_field,uMB&1)->m_RefPicList;
    Ipp8s *pFields0 = GetRefPicList(curr_slice, LIST_0,is_cur_mb_field,uMB&1)->m_Prediction;
    Ipp8s *pFields1 = GetRefPicList(curr_slice, LIST_1,is_cur_mb_field,uMB&1)->m_Prediction;
    Ipp32u uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    T_RefIdx block_ref;
    PIXTYPE* prev_frame = 0;
    PIXTYPE* futr_frame = 0;

#ifdef FRAME_INTERPOLATION
        Ipp32s planeSize = core_enc->m_pCurrentFrame->m_PlaneSize;
#else
        Ipp32s planeSize = 0;
#endif
    switch (uMBType)
    {
    case MBTYPE_DIRECT:
        Copy16x16(curr_slice->m_pPred4DirectB, 16, pDst, 16);
        break;

    case MBTYPE_BIDIR:
    case MBTYPE_BIDIR_BIDIR_16x8:
    case MBTYPE_BIDIR_BIDIR_8x16:
        Copy16x16(curr_slice->m_pPred4BiPred, 16, pDst, 16);
        break;

    case MBTYPE_INTER:
    case MBTYPE_SKIPPED:
    case MBTYPE_FORWARD:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels, pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BACKWARD:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels, pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_INTER_16x8:
    case MBTYPE_FWD_FWD_16x8:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVFwd += 4*2;      // Reposition MV pointer to next 16x8
        pDst += DEST_PITCH*8;
        block_ref = pRefIdxL0[4 * 2];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]] + pitchPixels*8;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_FWD_BWD_16x8:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVBwd += 4*2;      // Reposition MV pointer to next 16x8
        pDst += DEST_PITCH*8;
        block_ref = pRefIdxL1[4 * 2];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]] + pitchPixels*8;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BWD_FWD_16x8:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVFwd += 4 * 2;      // Reposition MV pointer to next 16x8
        pDst += DEST_PITCH*8;
        block_ref = pRefIdxL0[4 * 2];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]] + pitchPixels*8;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BWD_BWD_16x8:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVBwd += 4 * 2;      // Reposition MV pointer to next 16x8
        pDst += DEST_PITCH*8;
        block_ref = pRefIdxL1[4 * 2];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]] + pitchPixels*8;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BIDIR_FWD_16x8:
        // Copy the stored 16x8 prediction data from the BiPred Mode Buffer
        Copy16x8( curr_slice->m_pPred4BiPred, 16,  pDst, 16 );
        // Do other 16x8 block
        pMVFwd += 4*2;      // Reposition MV pointer to next 16x8
        pDst += DEST_PITCH*8;
        block_ref = pRefIdxL0[4 * 2];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]] + pitchPixels*8;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_FWD_BIDIR_16x8:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + curr_slice->m_InitialOffset[pFields0[block_ref]] + uOffset;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        // Copy the stored 2nd 16x8 prediction data from the BiPred Mode Buffer
        Copy16x8( curr_slice->m_pPred4BiPred + 128, 16,  pDst + DEST_PITCH*8, 16 );
        break;

    case MBTYPE_BIDIR_BWD_16x8:
        // Copy the stored 16x8 prediction data from the BiPred Mode Buffer
        Copy16x8( curr_slice->m_pPred4BiPred, 16,  pDst, 16 );
        // Do other 16x8 block
        pMVBwd += 4 * 2;      // Reposition MV pointer to next 16x8
        pDst += DEST_PITCH*8;
        block_ref = pRefIdxL1[4 * 2];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]] + pitchPixels*8;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BWD_BIDIR_16x8:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + curr_slice->m_InitialOffset[pFields1[block_ref]] + uOffset;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size16x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        // Copy the stored 2nd 16x8 prediction data from the BiPred Mode Buffer
        Copy16x8( curr_slice->m_pPred4BiPred + 128, 16,  pDst + DEST_PITCH*8, 16 );
        break;

    case MBTYPE_INTER_8x16:
    case MBTYPE_FWD_FWD_8x16:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        // Do other 8x16 block
        pMVFwd += 2;                    // Reposition MV pointer to next 8x16
        pDst += 8;
        block_ref = pRefIdxL0[2];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]]+ 8;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_FWD_BWD_8x16:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVBwd += 2;                    // Reposition MV pointer to next 8x16
        pDst += 8;
        block_ref = pRefIdxL1[2];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]]+ 8;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BWD_FWD_8x16:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVFwd += 2;                    // Reposition MV pointer to next 8x16
        pDst += 8;
        block_ref = pRefIdxL0[2];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]]+ 8;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BWD_BWD_8x16:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        pMVBwd += 2;                    // Reposition MV pointer to next 8x16
        pDst += 8;
        block_ref = pRefIdxL1[2];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]]+ 8;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BIDIR_FWD_8x16:
        Copy8x16( curr_slice->m_pPred4BiPred, 16, pDst, 16 );
        // Do other 8x16 block
        pMVFwd += 2;                    // Reposition MV pointer to next 8x16
        pDst += 8;
        block_ref = pRefIdxL0[2];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]]+ 8;
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_FWD_BIDIR_8x16:
        block_ref = pRefIdxL0[0];
        prev_frame = pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        Copy8x16( curr_slice->m_pPred4BiPred + 8, 16, pDst + 8, 16 );
        break;

    case MBTYPE_BIDIR_BWD_8x16:
        Copy8x16( curr_slice->m_pPred4BiPred, 16, pDst, 16 );
        pMVBwd += 2;                    // Reposition MV pointer to next 8x16
        pDst += 8;
        block_ref = pRefIdxL1[2];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]]+ 8;
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        break;

    case MBTYPE_BWD_BIDIR_8x16:
        block_ref = pRefIdxL1[0];
        futr_frame = pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]];
        InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x16, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
        Copy8x16( curr_slice->m_pPred4BiPred + 8, 16, pDst + 8, 16 );
        break;

    case MBTYPE_INTER_8x8_REF0:
    case MBTYPE_INTER_8x8:
    case MBTYPE_B_8x8:
        {
        Ipp32s ref_off = 0;
        Ipp32s sb_pos = 0;

        // 4 8x8 blocks
        for (uBlock=0; uBlock < 4; uBlock++)
        {
            block_ref = pRefIdxL0[ref_off];
            prev_frame = block_ref >= 0 ? pRefPicList0[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields0[block_ref]]: 0;
            block_ref = pRefIdxL1[ref_off];
            futr_frame = block_ref >= 0 ? pRefPicList1[block_ref] ?
                pRefPicList1[block_ref]->m_pYPlane + uOffset + curr_slice->m_InitialOffset[pFields1[block_ref]]: 0 : 0;

            switch (cur_mb.GlobalMacroblockInfo->sbtype[uBlock])
            {
                case SBTYPE_8x8:
                case SBTYPE_FORWARD_8x8:
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_BACKWARD_8x8:
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_8x4:
                case SBTYPE_FORWARD_8x4:
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size8x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    pMCMV = pMVFwd + 4;
                    pMCDst = pDst + DEST_PITCH*4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + pitchPixels*4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size8x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_BACKWARD_8x4:
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size8x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 1
                    pMCMV = pMVBwd + 4;
                    pMCDst = pDst + DEST_PITCH*4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + pitchPixels*4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size8x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_4x8:
                case SBTYPE_FORWARD_4x8:
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size4x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 1
                    pMCMV = pMVFwd + 1;
                    pMCDst = pDst + 4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + 4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_BACKWARD_4x8:
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size4x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 1
                    pMCMV = pMVBwd + 1;
                    pMCDst = pDst + 4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + 4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x8, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_4x4:
                case SBTYPE_FORWARD_4x4:
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMVFwd->mvx>>SUB_PEL_SHIFT,pMVFwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVFwd->mvx&3, pMVFwd->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 1
                    pMCMV = pMVFwd + 1;
                    pMCDst = pDst + 4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + 4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 2
                    pMCMV = pMVFwd + 4;
                    pMCDst = pDst + DEST_PITCH*4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + pitchPixels*4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 3
                    pMCMV = pMVFwd + 4 + 1;
                    pMCDst = pDst + DEST_PITCH*4 + 4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(prev_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + pitchPixels*4 + 4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_BACKWARD_4x4:
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMVBwd->mvx>>SUB_PEL_SHIFT,pMVBwd->mvy>>SUB_PEL_SHIFT), pitchPixels,pDst, 16, pMVBwd->mvx&3, pMVBwd->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 1
                    pMCMV = pMVBwd + 1;
                    pMCDst = pDst + 4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + 4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 2
                    pMCMV = pMVBwd + 4;
                    pMCDst = pDst + DEST_PITCH*4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + pitchPixels*4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    // Update MV, Ref & Dst pointers for block 3
                    pMCMV = pMVBwd + 4 + 1;
                    pMCDst = pDst + DEST_PITCH*4 + 4;
                    InterpolateLuma<PIXTYPE>(MVADJUST(futr_frame,pitchPixels,pMCMV->mvx>>SUB_PEL_SHIFT,pMCMV->mvy>>SUB_PEL_SHIFT) + pitchPixels*4 + 4, pitchPixels,pMCDst, 16, pMCMV->mvx&3, pMCMV->mvy&3, size4x4, core_enc->m_PicParamSet.bit_depth_luma, planeSize);
                    break;

                case SBTYPE_BIDIR_8x8:
                case SBTYPE_BIDIR_8x4:
                case SBTYPE_BIDIR_4x8:
                case SBTYPE_BIDIR_4x4:
                    {
                        PIXTYPE* pBiPred = curr_slice->m_pPred4BiPred + (uBlock&1)*8 + (uBlock&2)*16*4;
                        Copy8x8( pBiPred, 16, pDst, 16);
                    }
                break;

                case SBTYPE_DIRECT:
                    {
                        PIXTYPE* pDirect = curr_slice->m_pPred4DirectB + (uBlock&1)*8 + (uBlock&2)*16*4;
                        Copy8x8(pDirect, 16, pDst, 16);
                    }
                break;

                default:
                    break;

            }

            // advance MV pointers and source ptrs to next 8x8 block
            if (1 == uBlock) {
                // next block is lower left
                ref_off += 4*2 - 2;
                uOffset += pitchPixels*8 - 8;
                pMVFwd += 4*2 - 2;
                pMVBwd += 4*2 - 2;
                pDst += DEST_PITCH*8 - 8;
                sb_pos += 8;
            } else {
                // next block is to the right (except when uBlock==3, then
                // don't care
                ref_off += 2;
                uOffset += 8;
                pMVFwd += 2;
                pMVBwd += 2;
                pDst += 8;
            }
        }   // for uBlock
        }
        break;

    default:
        // not supposed to be here
        break;
    }   // switch
}   // MCOneMBLuma

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_MCOneMBChroma(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, PIXTYPE* pDst)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMB = cur_mb.uMB;
    H264MotionVector *pMVFwd = cur_mb.MVs[LIST_0]->MotionVectors;
    H264MotionVector *pMVBwd = cur_mb.MVs[LIST_1]->MotionVectors;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    bool is_cur_mb_bottom_field = curr_slice->m_is_cur_mb_bottom_field;
    MBTypeValue mb_type = cur_mb.GlobalMacroblockInfo->mbtype;
    T_RefIdx *pRefIdxL0 = cur_mb.RefIdxs[LIST_0]->RefIdxs;
    T_RefIdx *pRefIdxL1 = cur_mb.RefIdxs[LIST_1]->RefIdxs;
    H264EncoderFrame<PIXTYPE> **pRefPicList0 = GetRefPicList(curr_slice, LIST_0,is_cur_mb_field,uMB&1)->m_RefPicList;
    H264EncoderFrame<PIXTYPE> **pRefPicList1 = GetRefPicList(curr_slice, LIST_1,is_cur_mb_field,uMB&1)->m_RefPicList;
    Ipp8s *pFields0 = GetRefPicList(curr_slice, LIST_0,is_cur_mb_field,uMB&1)->m_Prediction;
    Ipp8s *pFields1 = GetRefPicList(curr_slice, LIST_1,is_cur_mb_field,uMB&1)->m_Prediction;
    BlocksBidirInfo bidir_info_buf[32];
    Ipp32s mb_offset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    Bidir_Blocks_Info info;
    info.blocks = bidir_info_buf;
    PIXTYPE *ref_futr, *ref_prev;

    if (mb_type == MBTYPE_DIRECT) {
        H264CoreEncoder_CDirectBOneMB_Interp_Cr(state, curr_slice, pMVFwd, pMVBwd, pFields0, pFields1, pDst, -1, size4x4);
        return;
    }
    get_bidir_info(mb_type, cur_mb.GlobalMacroblockInfo->sbtype, info);
    Ipp32s uMBx = cur_mb.uMBx * 16;
    Ipp32s uMBy = cur_mb.uMBy * 16;
    Ipp32s uFrmWidth = core_enc->m_WidthInMBs * 16;
    Ipp32s uFrmHeight = core_enc->m_HeightInMBs * 16;
    Ipp32u predPitchPixels = 16;
    // Ipp32s uInterpType = use_implicit_weighted_bipred ? 2 : 1;
    Ipp32s blocks_num = info.size;
    BlocksBidirInfo *bidir_blocks = info.blocks;

    for(Ipp32s i = 0; i < blocks_num; i++, bidir_blocks++)
    {
        Ipp32u block_idx = bidir_blocks->block_index;
        IppiSize size = {0, 0};
        Ipp32s dpx, dpy;
        Ipp32s shift_y;

        Ipp32s src_block_offset = 0;// = (((block_idx&3)<<2) + (block_idx&12)*pitch )>>1;
        Ipp32s dst_block_offset = 0;// = (((block_idx&3)<<2) + (block_idx&12)*uPredPitch )>>1;

        switch( core_enc->m_PicParamSet.chroma_format_idc ){
            case 1:
                size.width = bidir_blocks->block_w*2;
                size.height = bidir_blocks->block_h*2;
                src_block_offset = (((block_idx&3)<<2) + (block_idx&12)*pitchPixels )>>1;
                dst_block_offset = (((block_idx&3)<<2) + (block_idx&12)*predPitchPixels )>>1;
                shift_y = 0;
                break;
            case 2:
                size.width = bidir_blocks->block_w*2;
                size.height = bidir_blocks->block_h*4;
                src_block_offset = ((block_idx&3)<<1) + (block_idx&12)*pitchPixels;
                dst_block_offset = ((block_idx&3)<<1) + (block_idx&12)*predPitchPixels;
                shift_y=1;
                break;
            case 3:
                size.width = bidir_blocks->block_w*4;
                size.height = bidir_blocks->block_h*4;
                src_block_offset = ((block_idx&3)<<2) + (block_idx&12)*pitchPixels ;
                dst_block_offset = ((block_idx&3)<<2) + (block_idx&12)*predPitchPixels;
                shift_y=1;
                break;
        }

        Ipp32s posx = uMBx + ((block_idx&3)<<2);
        Ipp32s posy = uMBy + (block_idx&12);

        switch (bidir_blocks->direction)
        {
        case D_DIR_FWD:
            {
            Ipp32s block_ref = pRefIdxL0[block_idx];
            H264MotionVector mv_f = pMVFwd[block_idx];
            if( core_enc->m_PicParamSet.chroma_format_idc == 1 ){
                if(!is_cur_mb_bottom_field && pFields0[block_ref]) mv_f.mvy += - 2;
                else if(is_cur_mb_bottom_field && !pFields0[block_ref]) mv_f.mvy += 2;
            }
            TRUNCATE_LO(mv_f.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
            TRUNCATE_HI(mv_f.mvx, (-16 - posx - 1) * SubPelFactor)
            TRUNCATE_LO(mv_f.mvy, (uFrmHeight - posy + 1) * (SubPelFactor)) // >>shift_y
            TRUNCATE_HI(mv_f.mvy, (-16 - posy - 1) * (SubPelFactor))        // >>shift_y
            // get the previous reference block (with interpolation as needed)
            Ipp32s n = SubpelChromaMVAdjust(&mv_f, pitchPixels, dpx, dpy, core_enc->m_PicParamSet.chroma_format_idc) + src_block_offset;
            ref_prev = pRefPicList0[block_ref]->m_pUPlane + mb_offset + curr_slice->m_InitialOffset[pFields0[block_ref]];
            ownInterpolateChroma_H264(
                ref_prev + n,
                pitchPixels,
                pDst + dst_block_offset,
                DEST_PITCH,
                dpx,
                dpy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            ref_prev = pRefPicList0[block_ref]->m_pVPlane + mb_offset + curr_slice->m_InitialOffset[pFields0[block_ref]];
            ownInterpolateChroma_H264(
                ref_prev + n,
                pitchPixels,
                pDst + 8 + dst_block_offset,
                DEST_PITCH,
                dpx,
                dpy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            }
            break;

        case D_DIR_BWD:
            {
            Ipp32s block_ref = pRefIdxL1[block_idx];
            H264MotionVector mv_b = pMVBwd[block_idx];
            if( core_enc->m_PicParamSet.chroma_format_idc == 1 ){
                if(!is_cur_mb_bottom_field && pFields1[block_ref]) mv_b.mvy += - 2;
                else if(is_cur_mb_bottom_field && !pFields1[block_ref]) mv_b.mvy += 2;
            }
            TRUNCATE_LO(mv_b.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
            TRUNCATE_HI(mv_b.mvx, (-16 - posx - 1) * SubPelFactor)
            TRUNCATE_LO(mv_b.mvy, (uFrmHeight - posy + 1) * (SubPelFactor))  // >>shift_y
            TRUNCATE_HI(mv_b.mvy, (-16 - posy - 1) * (SubPelFactor))         // >>shift_y
            // get the previous reference block (with interpolation as needed)
            Ipp32s n = SubpelChromaMVAdjust(&mv_b, pitchPixels, dpx, dpy, core_enc->m_PicParamSet.chroma_format_idc) + src_block_offset;
            ref_futr = pRefPicList1[block_ref]->m_pUPlane + mb_offset + curr_slice->m_InitialOffset[pFields1[block_ref]];
            ownInterpolateChroma_H264(
                ref_futr + n,
                pitchPixels,
                pDst + dst_block_offset,
                DEST_PITCH,
                dpx,
                dpy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            ref_futr = pRefPicList1[block_ref]->m_pVPlane + mb_offset + curr_slice->m_InitialOffset[pFields1[block_ref]];
            ownInterpolateChroma_H264(
                ref_futr + n,
                pitchPixels,
                pDst + 8 + dst_block_offset,
                DEST_PITCH,
                dpx,
                dpy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            }
            break;

        case D_DIR_BIDIR:
            {
            Ipp32s block_ref_l1 = pRefIdxL1[block_idx];
            Ipp32s block_ref_l0 = pRefIdxL0[block_idx];

            // Clip the backward vector
            H264MotionVector mv_f = pMVFwd[block_idx];
            if( core_enc->m_PicParamSet.chroma_format_idc == 1 ){
                if(!is_cur_mb_bottom_field && pFields0[block_ref_l0]) mv_f.mvy += - 2;
                else if(is_cur_mb_bottom_field && !pFields0[block_ref_l0]) mv_f.mvy += 2;
            }
            TRUNCATE_LO(mv_f.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
            TRUNCATE_HI(mv_f.mvx, (-16 - posx - 1) * SubPelFactor)
            TRUNCATE_LO(mv_f.mvy, (uFrmHeight - posy + 1) * (SubPelFactor)) // >>shift_y
            TRUNCATE_HI(mv_f.mvy, (-16 - posy - 1) * (SubPelFactor))        // >>shift_y
            H264MotionVector mv_b = pMVBwd[block_idx];
            if( core_enc->m_PicParamSet.chroma_format_idc == 1 ){
                if(!is_cur_mb_bottom_field && pFields1[block_ref_l1]) mv_b.mvy += - 2;
                else if(is_cur_mb_bottom_field && !pFields1[block_ref_l1]) mv_b.mvy += 2;
            }
            TRUNCATE_LO(mv_b.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
            TRUNCATE_HI(mv_b.mvx, (-16 - posx - 1) * SubPelFactor)
            TRUNCATE_LO(mv_b.mvy, (uFrmHeight - posy + 1) * (SubPelFactor)) // >>shift_y
            TRUNCATE_HI(mv_b.mvy, (-16 - posy - 1) * (SubPelFactor))        // >>shift_y
            Ipp32s dfx, dfy;

            // get the previous reference block (with interpolation as needed)
            Ipp32s m = SubpelChromaMVAdjust(&mv_f, pitchPixels, dpx, dpy, core_enc->m_PicParamSet.chroma_format_idc) + src_block_offset;
            Ipp32s n = SubpelChromaMVAdjust(&mv_b,  pitchPixels, dfx, dfy, core_enc->m_PicParamSet.chroma_format_idc) + src_block_offset;
            ref_futr = pRefPicList1[block_ref_l1]->m_pUPlane + mb_offset + curr_slice->m_InitialOffset[pFields1[block_ref_l1]];
            ref_prev = pRefPicList0[block_ref_l0]->m_pUPlane + mb_offset + curr_slice->m_InitialOffset[pFields0[block_ref_l0]];
            ownInterpolateChroma_H264(
                ref_prev + m,
                pitchPixels,
                curr_slice->m_pTempBuff4DirectB + dst_block_offset,
                DEST_PITCH,
                dpx,
                dpy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            ownInterpolateChroma_H264(
                ref_futr + n,
                pitchPixels,
                pDst + dst_block_offset,
                DEST_PITCH,
                dfx,
                dfy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            // Ipp32s w1 = curr_slice->DistScaleFactor[block_ref_l0][0] >> 2;
            GetImplicitBidirPred(pDst + dst_block_offset, curr_slice->m_pTempBuff4DirectB + dst_block_offset,
                pDst + dst_block_offset, predPitchPixels, size);
            ref_futr = pRefPicList1[block_ref_l1]->m_pVPlane + mb_offset + curr_slice->m_InitialOffset[pFields1[block_ref_l1]];
            ref_prev = pRefPicList0[block_ref_l0]->m_pVPlane + mb_offset + curr_slice->m_InitialOffset[pFields0[block_ref_l0]];
            ownInterpolateChroma_H264(
                ref_prev + m,
                pitchPixels,
                curr_slice->m_pTempBuff4DirectB + 8 + dst_block_offset,
                DEST_PITCH,
                dpx,
                dpy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            ownInterpolateChroma_H264(
                ref_futr + n,
                pitchPixels,
                pDst + 8 + dst_block_offset,
                DEST_PITCH,
                dfx,
                dfy,
                size,
                core_enc->m_SeqParamSet.bit_depth_chroma);
            // Ipp32s w1 = curr_slice->DistScaleFactor[block_ref_l0][0] >> 2;
            GetImplicitBidirPred(pDst + 8 + dst_block_offset, curr_slice->m_pTempBuff4DirectB + 8 + dst_block_offset,
                pDst + 8 + dst_block_offset, predPitchPixels, size);
            }
            break;

        case D_DIR_DIRECT:
            H264CoreEncoder_CDirectBOneMB_Interp_Cr(state, curr_slice, pMVFwd, pMVBwd,  pFields0, pFields1, pDst, block_idx, size);
            break;

        default:
            VM_ASSERT(false);
            break;
        }
    }
}   // MCOneMBChroma

////////////////////////////////////////////////////////////////////////////////
// CDirectBOneMB_Interp_Cr
//
//  Copy all 4 4x4 chroma blocks from reference source using the input motion
//  vectors to locate and compute the reference pels using interpolation as
//  required. The vectors are in subpel units. Results are stored in the output
//  buffer in a 16x16 block, with a row pitch of DEST_PITCH.
//
////////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_CDirectBOneMB_Interp_Cr(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    const H264MotionVector *pMVL0,// Fwd motion vectors in subpel units
    const H264MotionVector *pMVL1,// Bwd motion vectors in subpel units
    Ipp8s *pFields0, Ipp8s *pFields1,
    PIXTYPE* pDst,                // put the resulting block here with pitch of 16
    Ipp32s block_offset, IppiSize size)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMB = cur_mb.uMB;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;
    Ipp32s max_sb, numx, numy, shift, cols_factor, rows_factor, w1, w0, intMVx, intMVy;
    Ipp32u uMBx = cur_mb.uMBx * 16;
    Ipp32u uMBy = cur_mb.uMBy * 16;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    bool is_cur_mb_bottom_field = curr_slice->m_is_cur_mb_bottom_field;
    Ipp32s uFrmWidth = core_enc->m_WidthInMBs << 4;
    Ipp32s uFrmHeight = core_enc->m_HeightInMBs << 4;
    PIXTYPE *pInterpBuf1 = NULL, *pInterpBuf2 = NULL;
    T_EncodeMBOffsets* pMBOffset = &core_enc->m_pMBOffsets[uMB];
    T_RefIdx *pRefIdxL0 = cur_mb.RefIdxs[LIST_0]->RefIdxs;
    T_RefIdx *pRefIdxL1 = cur_mb.RefIdxs[LIST_1]->RefIdxs;
    Ipp32u uInterpType = core_enc->use_implicit_weighted_bipred ? 2 : 1;
    PIXTYPE *pPrevU = 0, *pPrevV = 0, *pFutrU = 0, *pFutrV = 0;
    Ipp32s offset = pMBOffset->uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    H264EncoderFrame<PIXTYPE> **pRefPicList1 = GetRefPicList(curr_slice, LIST_1,is_cur_mb_field,uMB&1)->m_RefPicList;
    H264EncoderFrame<PIXTYPE> **pRefPicList0 = GetRefPicList(curr_slice, LIST_0,is_cur_mb_field,uMB&1)->m_RefPicList;
    IppiSize sz, copySize;
    Ipp32s sb = 0;

    if (core_enc->m_PicParamSet.chroma_format_idc == 1) {
        max_sb = 4;
        sz = size2x2;
        copySize = size4x4;
        numx = numy = 2;
        if (block_offset != -1) {
            sb = block_offset;
            numy = (size.height) / sz.height;
            numx = (size.width) / sz.width;
            copySize = size;
            max_sb = sb + 1;
        }
        for (; sb < max_sb; sb ++) {
            Ipp32s sb_row = (sb & 14);
            Ipp32s sb_col = (sb & 1) * 2;
            Ipp32s sb_pos = sb_row * 4 + sb_col;
            Ipp32s sb_offset = sb_row * 32 + 2 * sb_col;
            Direction_t direction;
            Ipp32s pxoffset = sb_col*2;
            pxoffset += offset + sb_row*pitchPixels*2;
            T_RefIdx ref_idx_l0 = pRefIdxL0[sb_pos];
            Ipp16s f_mvy_add = 0;
            if (ref_idx_l0 != -1){
                pPrevU = pRefPicList0[ref_idx_l0]->m_pUPlane + pxoffset + curr_slice->m_InitialOffset[pFields0[ref_idx_l0]];
                pPrevV = pRefPicList0[ref_idx_l0]->m_pVPlane + pxoffset + curr_slice->m_InitialOffset[pFields0[ref_idx_l0]];
                if (!is_cur_mb_bottom_field && pFields0[ref_idx_l0])
                    f_mvy_add += - 2;
                else if (is_cur_mb_bottom_field && !pFields0[ref_idx_l0])
                    f_mvy_add += 2;
            } else {
                pPrevU = pPrevV = 0;
                w1 = 0;
                uInterpType = 0;
            }
            T_RefIdx ref_idx_l1 = pRefIdxL1[sb_pos];
            Ipp16s b_mvy_add = 0;
            if (ref_idx_l1 != -1){
                pFutrU = pRefPicList1[ref_idx_l1]->m_pUPlane + pxoffset + curr_slice->m_InitialOffset[pFields1[ref_idx_l1]];
                pFutrV = pRefPicList1[ref_idx_l1]->m_pVPlane + pxoffset + curr_slice->m_InitialOffset[pFields1[ref_idx_l1]];
                if (!is_cur_mb_bottom_field && pFields1[ref_idx_l1])
                    b_mvy_add += - 2;
                else if (is_cur_mb_bottom_field && !pFields1[ref_idx_l1])
                    b_mvy_add += 2;
                if (pPrevU) {
                    direction = D_DIR_BIDIR;
                    w1 = curr_slice->DistScaleFactor[ref_idx_l0][ref_idx_l1]>> 2;
                    w0 = 64 - w1;
                    pInterpBuf2 = curr_slice->m_pTempBuff4DirectB;
                    pInterpBuf1 = pDst;
                } else {
                    direction =  D_DIR_BWD;
                    pInterpBuf2 = pDst;
                }
            } else {
                pFutrU = pFutrV = 0;
                direction = D_DIR_FWD;
                uInterpType = 0;
                pInterpBuf1 = pDst;
            }
            bool is8x8;
            if (direction == D_DIR_FWD) {
                is8x8 = (pMVL0[sb_pos] == pMVL0[sb_pos + 1]) && (pMVL0[sb_pos] == pMVL0[sb_pos + 4]) && (pMVL0[sb_pos] == pMVL0[sb_pos + 5]);
            } else if (direction == D_DIR_BWD) {
                is8x8 = (pMVL1[sb_pos] == pMVL1[sb_pos + 1]) && (pMVL1[sb_pos] == pMVL1[sb_pos + 4]) && (pMVL1[sb_pos] == pMVL1[sb_pos + 5]);
            } else {
                is8x8 = (pMVL0[sb_pos] == pMVL0[sb_pos + 1]) && (pMVL0[sb_pos] == pMVL0[sb_pos + 4]) && (pMVL0[sb_pos] == pMVL0[sb_pos + 5]) &&
                        (pMVL1[sb_pos] == pMVL1[sb_pos + 1]) && (pMVL1[sb_pos] == pMVL1[sb_pos + 4]) && (pMVL1[sb_pos] == pMVL1[sb_pos + 5]);
            }
            if (is8x8) {
                Ipp32s posx, posy;
                posx = uMBx + sb_col * 4;
                posy = uMBy + sb_row * 4;
                if (direction == D_DIR_FWD || direction == D_DIR_BIDIR) {
                    H264MotionVector mv_f = pMVL0[sb_pos];
                    mv_f.mvy = Ipp16s(mv_f.mvy + f_mvy_add);
                    TRUNCATE_LO(mv_f.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
                    TRUNCATE_HI(mv_f.mvx, (-16 - posx - 1) * SubPelFactor)
                    TRUNCATE_LO(mv_f.mvy, (uFrmHeight - posy + 1) * SubPelFactor)
                    TRUNCATE_HI(mv_f.mvy, (-16 - posy - 1) * SubPelFactor)
                    Ipp32s rOff = SubpelChromaMVAdjust(&mv_f, pitchPixels, intMVx, intMVy, 1);
                    PIXTYPE *pPrevBlk = pInterpBuf1 + sb_offset;
                    ownInterpolateChroma_H264(pPrevU + rOff, pitchPixels, pPrevBlk, 16, intMVx, intMVy, copySize, core_enc->m_SeqParamSet.bit_depth_chroma);
                    ownInterpolateChroma_H264(pPrevV + rOff, pitchPixels, pPrevBlk + 8, 16, intMVx, intMVy, copySize, core_enc->m_SeqParamSet.bit_depth_chroma);
                }
                if (direction == D_DIR_BWD || direction == D_DIR_BIDIR) {
                    H264MotionVector mv_b = pMVL1[sb_pos];
                    mv_b.mvy = Ipp16s(mv_b.mvy + b_mvy_add);
                    TRUNCATE_LO(mv_b.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
                    TRUNCATE_HI(mv_b.mvx, (-16 - posx - 1) * SubPelFactor)
                    TRUNCATE_LO(mv_b.mvy, (uFrmHeight - posy + 1) * (SubPelFactor))
                    TRUNCATE_HI(mv_b.mvy, (-16 - posy - 1) * (SubPelFactor))
                    Ipp32s rOff = SubpelChromaMVAdjust(&mv_b, pitchPixels, intMVx, intMVy, 1);
                    PIXTYPE *pFutrBlk = pInterpBuf2 + sb_offset;
                    ownInterpolateChroma_H264(pFutrU + rOff, pitchPixels, pFutrBlk, 16, intMVx, intMVy, copySize, core_enc->m_SeqParamSet.bit_depth_chroma);
                    ownInterpolateChroma_H264(pFutrV + rOff, pitchPixels, pFutrBlk + 8, 16, intMVx, intMVy, copySize, core_enc->m_SeqParamSet.bit_depth_chroma);
                }
            } else
                for (Ipp32s ypos = 0; ypos < numy; ypos++) {
                    for (Ipp32s xpos = 0; xpos < numx; xpos++){
                        Ipp32s posx, posy, mv_pos = sb_pos + ypos*4 + xpos;
                        posx = uMBx + sb_col * 4;
                        posy = uMBy + sb_row * 4;
                        if (direction == D_DIR_FWD || direction == D_DIR_BIDIR) {
                            H264MotionVector mv_f = pMVL0[mv_pos];
                            mv_f.mvy = Ipp16s(mv_f.mvy + f_mvy_add);
                            TRUNCATE_LO(mv_f.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
                            TRUNCATE_HI(mv_f.mvx, (-16 - posx - 1) * SubPelFactor)
                            TRUNCATE_LO(mv_f.mvy, (uFrmHeight - posy + 1) * SubPelFactor)
                            TRUNCATE_HI(mv_f.mvy, (-16 - posy - 1) * SubPelFactor)
                            Ipp32s rOff = ypos*pitchPixels*sz.height + SubpelChromaMVAdjust(&mv_f, pitchPixels, intMVx, intMVy, 1);
                            rOff += xpos*2;
                            PIXTYPE *pPrevBlk = pInterpBuf1 + sb_offset + ypos*16*sz.height + xpos*2;
                            ownInterpolateChroma_H264(pPrevU + rOff, pitchPixels, pPrevBlk, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                            ownInterpolateChroma_H264(pPrevV + rOff, pitchPixels, pPrevBlk + 8, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                        }
                        if (direction == D_DIR_BWD || direction == D_DIR_BIDIR) {
                            H264MotionVector mv_b = pMVL1[mv_pos];
                            mv_b.mvy = Ipp16s(mv_b.mvy + b_mvy_add);
                            TRUNCATE_LO(mv_b.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
                            TRUNCATE_HI(mv_b.mvx, (-16 - posx - 1) * SubPelFactor)
                            TRUNCATE_LO(mv_b.mvy, (uFrmHeight - posy + 1) * (SubPelFactor))
                            TRUNCATE_HI(mv_b.mvy, (-16 - posy - 1) * (SubPelFactor))
                            Ipp32s rOff = ypos*pitchPixels*sz.height + SubpelChromaMVAdjust(&mv_b, pitchPixels, intMVx, intMVy, 1);
                            rOff += xpos*2;
                            PIXTYPE *pFutrBlk = pInterpBuf2 + sb_offset + ypos*16*sz.height + xpos*2;
                            ownInterpolateChroma_H264(pFutrU + rOff, pitchPixels, pFutrBlk, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                            ownInterpolateChroma_H264(pFutrV + rOff, pitchPixels, pFutrBlk + 8, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                        }
                    }
                }
            if (direction == D_DIR_BIDIR) {
                GetImplicitBidirPred(pDst + sb_offset, pInterpBuf1 + sb_offset, pInterpBuf2 + sb_offset, 16, copySize);
                GetImplicitBidirPred(pDst + 8 + sb_offset, pInterpBuf1 + 8 + sb_offset, pInterpBuf2 + 8 + sb_offset, 16, copySize);
            }
        }
        return;
    }
    switch( core_enc->m_PicParamSet.chroma_format_idc ){
        case 1: // 420
            cols_factor = rows_factor = 1;
            shift = 0;
            max_sb = 4;
            sz = size2x2;
            copySize = size4x4;
            numx = numy = 2;
            break;
        case 2: // 422
            shift = 1;
            cols_factor = 1;
            rows_factor = 2;
            max_sb = 8;
            sz = size2x4;
            copySize = size4x4;
            numx = 2;
            numy = 1;
            break;
        default: // 444
            shift = 1;
            cols_factor = rows_factor = 2;
            max_sb = 16;
            sz = size4x4;
            copySize = size8x8;
            numx = numy = 1;
            break;
    }
    if (block_offset != -1) {
        sb = block_offset;
        numy = (size.height) / sz.height;
        numx = (size.width) / sz.width;
        copySize.width = size.width;
        copySize.height = size.height;
        if( core_enc->m_PicParamSet.chroma_format_idc == 2 ){
            if( block_offset == 2 || block_offset == 3) sb += 2;
            numy = numy<<1;
            copySize.height = copySize.height<<1;
        }
        max_sb = sb + 1;
    }
    for (; sb < max_sb; sb ++) {
        Ipp32s sb_row = (sb & 14) >> shift;
        Ipp32s sb_col = (sb & 1) * 2;
        Ipp32s sb_pos = sb_row * 4 + sb_col;
        Ipp32s sb_offset = sb_row * 32 * rows_factor + 2 * sb_col * cols_factor;
        Direction_t direction;
        T_RefIdx ref_idx_l0 = pRefIdxL0[sb_pos];
        Ipp16s f_mvy_add = 0;
        if (ref_idx_l0 != -1){
            pPrevU = pRefPicList0[ref_idx_l0]->m_pUPlane + offset + sb_row*pitchPixels*2*rows_factor + sb_col*2*cols_factor + curr_slice->m_InitialOffset[pFields0[ref_idx_l0]];
            pPrevV = pRefPicList0[ref_idx_l0]->m_pVPlane + offset + sb_row*pitchPixels*2*rows_factor + sb_col*2*cols_factor + curr_slice->m_InitialOffset[pFields0[ref_idx_l0]];
            if( core_enc->m_PicParamSet.chroma_format_idc == 1){
                if(!is_cur_mb_bottom_field && pFields0[ref_idx_l0]) f_mvy_add += - 2;
                else if(is_cur_mb_bottom_field && !pFields0[ref_idx_l0]) f_mvy_add += 2;
            }
        } else {
            pPrevU = pPrevV = 0;
            w1 = 0;
            uInterpType = 0;
        }
        T_RefIdx ref_idx_l1 = pRefIdxL1[sb_pos];
        Ipp16s b_mvy_add = 0;
        if (ref_idx_l1 != -1){
            pFutrU = pRefPicList1[ref_idx_l1]->m_pUPlane + offset + sb_row*pitchPixels*2*rows_factor + sb_col*2*cols_factor + curr_slice->m_InitialOffset[pFields1[ref_idx_l1]];
            pFutrV = pRefPicList1[ref_idx_l1]->m_pVPlane + offset + sb_row*pitchPixels*2*rows_factor + sb_col*2*cols_factor + curr_slice->m_InitialOffset[pFields1[ref_idx_l1]];
            if( core_enc->m_PicParamSet.chroma_format_idc == 1 ){
                if(!is_cur_mb_bottom_field && pFields1[ref_idx_l1]) b_mvy_add += - 2;
                else if(is_cur_mb_bottom_field && !pFields1[ref_idx_l1]) b_mvy_add += 2;
            }
            if (pPrevU) {
                direction = D_DIR_BIDIR;
                w1 = curr_slice->DistScaleFactor[ref_idx_l0][ref_idx_l1]>> 2;
                w0 = 64 - w1;
                pInterpBuf2 = curr_slice->m_pTempBuff4DirectB;
                pInterpBuf1 = pDst;
            } else {
                direction =  D_DIR_BWD;
                pInterpBuf2 = pDst;
            }
        } else {
            pFutrU = pFutrV = 0;
            direction = D_DIR_FWD;
            uInterpType = 0;
            pInterpBuf1 = pDst;
        }
        for (Ipp32s ypos = 0; ypos < numy; ypos++){ // 4 2x2 blocks
            for (Ipp32s xpos = 0; xpos < numx; xpos++){
                Ipp32s posx, posy, mv_pos = sb_pos + ypos*4 + xpos;
                posx = uMBx + sb_col * 4;
                posy = uMBy + sb_row * 4;
                if (direction == D_DIR_FWD || direction == D_DIR_BIDIR) {
                    H264MotionVector mv_f = pMVL0[mv_pos];
                    mv_f.mvy = Ipp16s(mv_f.mvy + f_mvy_add);
                    TRUNCATE_LO(mv_f.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
                    TRUNCATE_HI(mv_f.mvx, (-16 - posx - 1) * SubPelFactor)
                    TRUNCATE_LO(mv_f.mvy, (uFrmHeight - posy + 1) * SubPelFactor)
                    TRUNCATE_HI(mv_f.mvy, (-16 - posy - 1) * SubPelFactor)
                    Ipp32s rOff = ypos*pitchPixels*sz.height + xpos*2 + SubpelChromaMVAdjust(&mv_f, pitchPixels, intMVx, intMVy, core_enc->m_PicParamSet.chroma_format_idc);
                    PIXTYPE *pPrevBlk = pInterpBuf1 + sb_offset + ypos*16*sz.height + xpos*2;
                    ownInterpolateChroma_H264(pPrevU + rOff, pitchPixels, pPrevBlk, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                    ownInterpolateChroma_H264(pPrevV + rOff, pitchPixels, pPrevBlk + 8, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                }
                if (direction == D_DIR_BWD || direction == D_DIR_BIDIR) {
                    H264MotionVector mv_b = pMVL1[mv_pos];
                    mv_b.mvy = Ipp16s(mv_b.mvy + b_mvy_add);
                    TRUNCATE_LO(mv_b.mvx, (uFrmWidth - posx + 1) * SubPelFactor)
                    TRUNCATE_HI(mv_b.mvx, (-16 - posx - 1) * SubPelFactor)
                    TRUNCATE_LO(mv_b.mvy, (uFrmHeight - posy + 1) * (SubPelFactor)) // >>shift))
                    TRUNCATE_HI(mv_b.mvy, (-16 - posy - 1) * (SubPelFactor))        // >>shift))
                    Ipp32s rOff = ypos*pitchPixels*sz.height + xpos*2 + SubpelChromaMVAdjust(&mv_b, pitchPixels, intMVx, intMVy, core_enc->m_PicParamSet.chroma_format_idc);
                    PIXTYPE *pFutrBlk = pInterpBuf2 + sb_offset + ypos*16*sz.height + xpos*2;
                    ownInterpolateChroma_H264(pFutrU + rOff, pitchPixels, pFutrBlk, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                    ownInterpolateChroma_H264(pFutrV + rOff, pitchPixels, pFutrBlk + 8, 16, intMVx, intMVy, sz, core_enc->m_SeqParamSet.bit_depth_chroma);
                }
            }
        }
        if (direction == D_DIR_BIDIR) {
            GetImplicitBidirPred(pDst + sb_offset, pInterpBuf1 + sb_offset, pInterpBuf2 + sb_offset, 16, copySize);
            GetImplicitBidirPred(pDst + 8 + sb_offset, pInterpBuf1 + 8 + sb_offset, pInterpBuf2 + 8 + sb_offset, 16, copySize);
        }
    }
}

// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template void H264CoreEncoder_MCOneMBLuma<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, const H264MotionVector*, const H264MotionVector*, PIXTYPE*);
template void H264CoreEncoder_CDirectBOneMB_Interp_Cr<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, const H264MotionVector*, const H264MotionVector*, Ipp8s*, Ipp8s*, PIXTYPE*, Ipp32s, IppiSize);
template void H264CoreEncoder_MCOneMBChroma<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template void H264CoreEncoder_MCOneMBLuma<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, const H264MotionVector*, const H264MotionVector*, PIXTYPE*);
template void H264CoreEncoder_CDirectBOneMB_Interp_Cr<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, const H264MotionVector*, const H264MotionVector*, Ipp8s*, Ipp8s*, PIXTYPE*, Ipp32s, IppiSize);
template void H264CoreEncoder_MCOneMBChroma<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, PIXTYPE*);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER
