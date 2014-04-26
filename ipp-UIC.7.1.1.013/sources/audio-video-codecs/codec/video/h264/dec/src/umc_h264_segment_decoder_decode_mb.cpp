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


namespace UMC
{

// 0 1 4 5
// 2 3 6 7
// 8 9 c d
// a b e f

const
Ipp8u block2lin[16] =
{
     0, 1, 4, 5,
     2, 3, 6, 7,
     8, 9,12,13,
     10,11,14,15
};

void H264SegmentDecoder::ComputeMotionVectorPredictorsMBAFF(const Ipp8u ListNum,
                                                       Ipp8s RefIndex, // reference index for this part
                                                       const Ipp32s block, // block or subblock number, depending on mbtype
                                                       H264DecoderMotionVector * mv)
{
    Ipp32s  px0, px1, px2;
    Ipp32s  py0, py1, py2;
    //    Ipp32s     diff;
    Ipp32s  isRightUnavailable=false;
    // Indicates whether the (above) right block, subblock or
    // macroblock can be used for motion vector prediction.
    // This is not a true boolean, in that we use bitwise operations
    // so that any non-zero value, not just the value true,
    // is considered true.
    Ipp32s  isLeftUnavailable=false;
    // Indicates whether the (above) left block, subblock or
    // macroblock can be used for motion vector prediction.
    // Only used when isRightUnavailable is non-zero.

    H264DecoderMotionVector *sl; // pointer to sb to the left
    H264DecoderMotionVector *sa; // pointer to sb above
    H264DecoderMotionVector *sr; // pointer to sb above right
    const RefIndexType *pRefIxl;                    // pointer to corresponding RefIndex sb
    const RefIndexType *pRefIxa;
    const RefIndexType *pRefIxr;
    H264DecoderMotionVector *sonly = NULL; // pointer to only MV this ref
    H264DecoderMotionVector null = {0,0};
    RefIndexType nullRefIx = -1;
    Ipp32u uSameRefPicCount = 3;
    // To code the rule that if only one of the three reference MV is to
    // the same reference picture as the MV being computed, then that one
    // MV is used as the MV predictor. Initialize to all same, then decrement
    // as "different reference picture" are found.
    H264DecoderBlockLocation Left={-1,-1},Top={-1,-1},TopRight={-1,-1},TopLeft={-1,-1};
    H264DecoderMacroblockGlobalInfo * gmbs = m_gmbinfo->mbs;

    Ipp32u lbls=0, lbrs=0, tbls=0, tbrs=0, rbls=0, rbrs=0;
    switch (m_cur_mb.GlobalMacroblockInfo->mbtype)
    {
    case MBTYPE_FORWARD:
    case MBTYPE_BACKWARD:
    case MBTYPE_BIDIR:
    case MBTYPE_SKIPPED:
        VM_ASSERT(block == 0);

        Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
        Top = m_cur_mb.CurrentBlockNeighbours.mb_above;
        TopRight = m_cur_mb.CurrentBlockNeighbours.mb_above_right;
        TopLeft= m_cur_mb.CurrentBlockNeighbours.mb_above_left;

        isRightUnavailable = (TopRight.mb_num<0);
        isLeftUnavailable = (TopLeft.mb_num<0);

        break;
    case MBTYPE_INTER_16x8:
        VM_ASSERT(block >= 0 && block <= 1);
        // First check for availability of directional predictor which is
        // just used if available.
        if (block == 0)
        {
            // upper half, use predictor from above
            Top= m_cur_mb.CurrentBlockNeighbours.mb_above;
            if (Top.mb_num>=0)
            {
                tbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Top.mb_num]))>0;
                tbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Top.mb_num]))<0;

                if (IS_INTER_MBTYPE(gmbs[Top.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Top.mb_num, Top.block_num)<<tbls)>>tbrs) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Top.mb_num, Top.block_num);
                    mv->mvy = (Ipp16s)(((mv->mvy+((mv->mvy < 0)&&tbls))<<tbrs)>>tbls);
                    return;
                }
                else
                {
                    goto median16x8_0_aff;
                }
            }
            else
            {
median16x8_0_aff:
                Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
                TopRight = m_cur_mb.CurrentBlockNeighbours.mb_above_right;
                TopLeft = m_cur_mb.CurrentBlockNeighbours.mb_above_left;

                // init vars for median prediction
                isRightUnavailable = (TopRight.mb_num<0);
                if (isRightUnavailable)
                    isLeftUnavailable = (TopLeft.mb_num<0);
            }
        }
        else
        {
            Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[2];
            // lower half, use predictor from left
            if ( Left.mb_num>=0)
            {
                lbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Left.mb_num]))>0;
                lbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Left.mb_num]))<0;

                if (IS_INTER_MBTYPE(gmbs[Left.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Left.mb_num, Left.block_num)<<lbls)>>lbrs) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
                    mv->mvy = (Ipp16s)(((mv->mvy + ((mv->mvy < 0)&&lbls))<<lbrs)>>lbls);
                    return;
                }
                else
                {
                    goto median_16x8_1_aff;
                }
            }
            else
            {
median_16x8_1_aff:

                Top.mb_num = m_CurMBAddr;
                Top.block_num = 4;

                TopLeft.block_num = 8;
                GetTopLeftLocationForCurrentMBLumaMBAFF(&TopLeft);

                // init vars for median prediction
                isRightUnavailable = 1;
                isLeftUnavailable = (Left.mb_num<0);
            }
        }
        break;
    case MBTYPE_INTER_8x16:
        VM_ASSERT(block >= 0 && block <= 1);
        // First check for availability of directional predictor which is
        // just used if available.
        if (block == 0)
        {
            // left half, use predictor from left
            //LeftBlockNum = block;
            //LeftMB=GetLeftBlock(pMBInfo,LeftBlockNum);
            Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
            if (Left.mb_num>=0)
            {
                lbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Left.mb_num]))>0;
                lbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Left.mb_num]))<0;
                if (IS_INTER_MBTYPE(gmbs[Left.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Left.mb_num, Left.block_num)<<lbls)>>lbrs) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
                    mv->mvy = (Ipp16s)(((mv->mvy + ((mv->mvy < 0)&&lbls))<<lbrs)>>lbls);
                    return;
                }
                else
                {
                    goto median_8x16_0_aff;
                }
            }
            else
            {
median_8x16_0_aff:
                /*                TopBlockNum=0;
                TopLeftBlockNum=0;
                TopRightBlockNum=1;

                TopMB=GetTopBlock(pMBInfo,TopBlockNum);
                TopLeftMB=GetTopLeftBlock(pMBInfo,TopLeftBlockNum);
                TopRightMB=GetTopRightBlock(pMBInfo,TopRightBlockNum);*/

                Top = m_cur_mb.CurrentBlockNeighbours.mb_above;
                TopRight = m_cur_mb.CurrentBlockNeighbours.mb_above;
                TopLeft= m_cur_mb.CurrentBlockNeighbours.mb_above_left;
                TopRight.block_num+=2;
                // init vars for median prediction
                isRightUnavailable = (Top.mb_num<0);
                if (isRightUnavailable)
                    isLeftUnavailable = (TopLeft.mb_num<0);
            }
        }
        else
        {
            // right half, use predictor from above right unless unavailable,
            // then try above left
            //TopRightBlockNum=3;
            //TopRightMB=GetTopRightBlock(pMBInfo,TopRightBlockNum);
            //TopBlockNum=2;
            //TopMB=GetTopBlock(pMBInfo,TopBlockNum);
            TopRight= m_cur_mb.CurrentBlockNeighbours.mb_above_right;
            Top= m_cur_mb.CurrentBlockNeighbours.mb_above;
            Top.block_num+=2;

            if ( TopRight.mb_num>=0)
            {
                rbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[TopRight.mb_num]))>0;
                rbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[TopRight.mb_num]))<0;

                if (IS_INTER_MBTYPE(gmbs[TopRight.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num)<<rbls)>>rbrs) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num);
                    mv->mvy = (Ipp16s)(((mv->mvy + ((mv->mvy < 0)&&rbls))<<rbrs)>>rbls);
                    return;
                }
            }
            else  if ( Top.mb_num>=0)
            {
                tbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Top.mb_num]))>0;
                tbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Top.mb_num]))<0;

                if (IS_INTER_MBTYPE(gmbs[Top.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Top.mb_num, Top.block_num-1)<<tbls)>>tbrs) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Top.mb_num, Top.block_num-1);
                    mv->mvy = (Ipp16s)(((mv->mvy + ((mv->mvy < 0)&&tbls))<<tbrs)>>tbls);
                    return;
                }
            }


            //LeftBlockNum=2;
            //LeftMB=GetLeftBlock(pMBInfo,LeftBlockNum);
            Left.mb_num=m_CurMBAddr;
            Left.block_num=1;
            TopLeft=m_cur_mb.CurrentBlockNeighbours.mb_above;
            TopLeft.block_num++;
            // init vars for median prediction
            isRightUnavailable = (TopRight.mb_num<0);
            if (isRightUnavailable)
                isLeftUnavailable = (Top.mb_num<0);
        }
        //        diff = 2;
        break;
    case MBTYPE_INTER_8x8:
    case MBTYPE_INTER_8x8_REF0:
        {
            // Each 8x8 block of a macroblock can be subdivided into subblocks,
            // each having its own MV. The parameter 'block' has block and
            // subblock information:
            //  block 0..3, bits 2-3
            //  subblock 0..3, bits 0-1
            Ipp32s  left_edge_block = 0, top_edge_block = 0, right_edge_block = 0;

            switch (m_cur_mb.GlobalMacroblockInfo->sbtype[block>>2])
            {
            case SBTYPE_8x8:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    block2lin[block];
                TopRight.block_num=
                    block2lin[block]+1;
                    GetLeftLocationForCurrentMBLumaMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaMBAFF(&Top,0);
                    GetTopRightLocationForCurrentMBLumaMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaMBAFF(&TopLeft);
                switch (block>>2)
                {
                case 0:
                    left_edge_block = 1;
                    top_edge_block = 1;
                    isRightUnavailable = (Top.mb_num<0);
                    isLeftUnavailable = (TopLeft.mb_num<0);
                    break;
                case 1:
                    left_edge_block = 0;
                    top_edge_block = 1;
                    isRightUnavailable = (TopRight.mb_num<0);
                    isLeftUnavailable = (Top.mb_num<0);
                    break;
                case 2:
                    left_edge_block = 1;
                    top_edge_block = 0;
                    isRightUnavailable = 0;
                    break;
                case 3:
                    left_edge_block = 0;
                    top_edge_block = 0;
                    isRightUnavailable = 1;
                    isLeftUnavailable = 0;
                    break;
                }   // block
                right_edge_block = left_edge_block == 0 ? 1 : 0;
                break;
            case SBTYPE_8x4:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    block2lin[block&(-4)]+4*(block&1);
                TopRight.block_num=
                    block2lin[block&(-4)]+4*(block&1)+1;

                    GetLeftLocationForCurrentMBLumaMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaMBAFF(&Top,0);
                    GetTopRightLocationForCurrentMBLumaMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaMBAFF(&TopLeft);

                left_edge_block = left_edge_tab16_8x4[block];
                top_edge_block = top_edge_tab16_8x4[block];
                right_edge_block = right_edge_tab16_8x4[block];

                if (!top_edge_block)
                {
                    isRightUnavailable = (above_right_avail_8x4[block] == 0);
                    if (isRightUnavailable)
                    {
                        if (left_edge_block)
                            isLeftUnavailable = (Left.mb_num<0);
                        else
                            isLeftUnavailable = 0;
                    }
                }
                else
                {
                    // top edge of 8x4
                    if (!right_edge_block)
                    {
                        isRightUnavailable = (Top.mb_num<0);
                        isLeftUnavailable = (TopLeft.mb_num<0);
                    }
                    else
                    {
                        isRightUnavailable = (TopRight.mb_num<0);
                        isLeftUnavailable = (Top.mb_num<0);
                    }
                }
                break;
            case SBTYPE_4x8:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    block2lin[block&(-4)]+(block&1);
                TopRight.block_num=
                    block2lin[block&(-4)]+(block&1);

                    GetLeftLocationForCurrentMBLumaMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaMBAFF(&Top,0);
                    GetTopRightLocationForCurrentMBLumaMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaMBAFF(&TopLeft);
                //            diff = 1;
                left_edge_block = left_edge_tab16_4x8[block];
                top_edge_block = top_edge_tab16_4x8[block];
                right_edge_block = right_edge_tab16_4x8[block];
                if (!top_edge_block)
                {
                    isRightUnavailable = (above_right_avail_4x8[block] == 0);
                    isLeftUnavailable = 0;  // always, when above right not available
                }
                else
                {
                    if (!right_edge_block)
                        isRightUnavailable = (Top.mb_num<0);
                    else
                        isRightUnavailable = (TopRight.mb_num<0);
                    if (isRightUnavailable)
                    {
                        if (!left_edge_block)
                            isLeftUnavailable = (Top.mb_num<0);
                        else
                            isLeftUnavailable = (TopLeft.mb_num<0);
                    }
                }
                break;
            case SBTYPE_4x4:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    TopRight.block_num=
                    block2lin[block];

                    GetLeftLocationForCurrentMBLumaMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaMBAFF(&Top,0);
                    GetTopRightLocationForCurrentMBLumaMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaMBAFF(&TopLeft);
                VM_ASSERT(block >= 0 && block <= 15);
                //            diff = 1;
                left_edge_block = left_edge_tab16_[block];
                top_edge_block = top_edge_tab16_[block];
                right_edge_block = right_edge_tab16[block];
                if (!top_edge_block)
                    isRightUnavailable = (above_right_avail_4x4_[block] == 0);
                else
                {
                    if (!right_edge_block)
                        isRightUnavailable = (Top.mb_num<0);
                    else
                        isRightUnavailable = (TopRight.mb_num<0);
                }
                if (isRightUnavailable)
                {
                    // When not on top edge of MB, for blocks for which right may not
                    // be available, left is always available.
                    if (top_edge_block == 0)
                        isLeftUnavailable = 0;
                    else
                    {
                        if (!left_edge_block)
                            isLeftUnavailable = (Top.mb_num<0);
                        else
                            isLeftUnavailable = (TopLeft.mb_num<0);
                    }
                }
                break;
            }
        }
        break;
    default:
        *mv = zeroVector;
        return;  // DEBUG : may be throw ???
    }

    // correct for left edge
    if (Left.mb_num<0)
    {
        sl = &null;
        pRefIxl = &nullRefIx;
    }
    else
    {
        sl = &GetMV(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
        pRefIxl = GetReferenceIndexPtr(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
        lbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Left.mb_num]))>0;
        lbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Left.mb_num]))<0;
    }

    // correct for top edge
    if (Top.mb_num<0)
    {
        sa = &null;
        pRefIxa = &nullRefIx;
    }
    else
    {
        sa = &GetMV(m_gmbinfo, ListNum, Top.mb_num, Top.block_num);
        pRefIxa = GetReferenceIndexPtr(m_gmbinfo, ListNum, Top.mb_num, Top.block_num);
        tbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Top.mb_num]))>0;
        tbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[Top.mb_num]))<0;

    }

    // correct for upper right
    if (isRightUnavailable)
    {
        // replace with block above left if available
        if (isLeftUnavailable)
        {
            sr = &null;
            pRefIxr = &nullRefIx;
        }
        else
        {
            sr = &GetMV(m_gmbinfo, ListNum, TopLeft.mb_num, TopLeft.block_num);
            pRefIxr = GetReferenceIndexPtr(m_gmbinfo, ListNum, TopLeft.mb_num, TopLeft.block_num);
            rbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[TopLeft.mb_num]))>0;
            rbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[TopLeft.mb_num]))<0;
        }
    }
    else
    {
        sr = &GetMV(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num);
        pRefIxr = GetReferenceIndexPtr(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num);
        rbls=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[TopRight.mb_num]))>0;
        rbrs=(pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo)-GetMBFieldDecodingFlag(gmbs[TopRight.mb_num]))<0;

    }

    // If above AND right are unavailable AND left is available, use left
    if ((sa == &null) && (sr == &null) && (sl != &null))
    {
        // return MV at sl
        mv->mvx = sl->mvx;
        mv->mvy = (Ipp16s)(((sl->mvy+((sl->mvy<0)&&lbls))<<lbrs)>>lbls);
    }
    else
    {
        // Check for more than one predictor from different reference frame
        // If there is only one predictor from this ref frame, then sonly will
        // be pointing to it.
        Ipp32s ls = 0, rs = 0;
        if (((*pRefIxl<<lbls)>>lbrs) != RefIndex)
            uSameRefPicCount--;
        else
        {
            sonly = sl;
            ls=lbls;
            rs=lbrs;
        }
        if (((*pRefIxa<<tbls)>>tbrs) != RefIndex)
            uSameRefPicCount--;
        else
        {
            sonly = sa;
            ls=tbls;
            rs=tbrs;
        }
        if (((*pRefIxr<<rbls)>>rbrs) != RefIndex)
            uSameRefPicCount--;
        else
        {
            sonly = sr;
            ls=rbls;
            rs=rbrs;
        }

        if (uSameRefPicCount != 1)
        {
            // "normal" median prediction
            px0 = sl->mvx;
            px1 = sa->mvx;
            px2 = sr->mvx;

#define MEDIAN_OF_3(a, b, c) (IPP_MIN((a),(b))) ^ (IPP_MIN((b),(c))) ^ (IPP_MIN((c),(a)))

            mv->mvx = (Ipp16s)(MEDIAN_OF_3(px0, px1, px2));

            py0 = ((sl->mvy+((sl->mvy<0)&&lbls))<<lbrs)>>lbls;
            py1 = ((sa->mvy+((sa->mvy<0)&&tbls))<<tbrs)>>tbls;
            py2 = ((sr->mvy+((sr->mvy<0)&&rbls))<<rbrs)>>rbls;

            mv->mvy = (Ipp16s)(MEDIAN_OF_3(py0, py1, py2));
        }
        else
        {
            // return MV at sonly
            mv->mvx = sonly->mvx;
            mv->mvy = (Ipp16s)(((sonly->mvy+((sonly->mvy<0)&&ls))<<rs)>>ls);
        }
    }
}

void H264SegmentDecoder::ComputeMotionVectorPredictors(const Ipp8u ListNum,
                                                       Ipp8s RefIndex, // reference index for this part
                                                       const Ipp32s block, // block or subblock number, depending on mbtype
                                                       H264DecoderMotionVector * mv)
{
    if (m_isMBAFF)
    {
        ComputeMotionVectorPredictorsMBAFF(ListNum, RefIndex, block, mv);
        return;
    }

    Ipp32s  px0, px1, px2;
    Ipp32s  py0, py1, py2;
    //    Ipp32s     diff;
    Ipp32s  isRightUnavailable=false;
    // Indicates whether the (above) right block, subblock or
    // macroblock can be used for motion vector prediction.
    // This is not a true boolean, in that we use bitwise operations
    // so that any non-zero value, not just the value true,
    // is considered true.
    Ipp32s  isLeftUnavailable=false;
    // Indicates whether the (above) left block, subblock or
    // macroblock can be used for motion vector prediction.
    // Only used when isRightUnavailable is non-zero.

    H264DecoderMotionVector *sl; // pointer to sb to the left
    H264DecoderMotionVector *sa; // pointer to sb above
    H264DecoderMotionVector *sr; // pointer to sb above right
    const RefIndexType *pRefIxl;                    // pointer to corresponding RefIndex sb
    const RefIndexType *pRefIxa;
    const RefIndexType *pRefIxr;
    H264DecoderMotionVector *sonly = NULL; // pointer to only MV this ref
    H264DecoderMotionVector null = {0,0};
    RefIndexType nullRefIx = -1;
    Ipp32u uSameRefPicCount = 3;
    // To code the rule that if only one of the three reference MV is to
    // the same reference picture as the MV being computed, then that one
    // MV is used as the MV predictor. Initialize to all same, then decrement
    // as "different reference picture" are found.
    H264DecoderBlockLocation Left={-1,-1},Top={-1,-1},TopRight={-1,-1},TopLeft={-1,-1};
    H264DecoderMacroblockGlobalInfo * gmbs = m_gmbinfo->mbs;

    switch (m_cur_mb.GlobalMacroblockInfo->mbtype)
    {
    case MBTYPE_FORWARD:
    case MBTYPE_BACKWARD:
    case MBTYPE_BIDIR:
    case MBTYPE_SKIPPED:
        VM_ASSERT(block == 0);

        Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
        Top = m_cur_mb.CurrentBlockNeighbours.mb_above;
        TopRight = m_cur_mb.CurrentBlockNeighbours.mb_above_right;
        TopLeft= m_cur_mb.CurrentBlockNeighbours.mb_above_left;

        isRightUnavailable = (TopRight.mb_num<0);
        isLeftUnavailable = (TopLeft.mb_num<0);

        break;
    case MBTYPE_INTER_16x8:
        VM_ASSERT(block >= 0 && block <= 1);
        // First check for availability of directional predictor which is
        // just used if available.
        if (block == 0)
        {
            // upper half, use predictor from above
            Top= m_cur_mb.CurrentBlockNeighbours.mb_above;
            if (Top.mb_num>=0)
            {

                if (IS_INTER_MBTYPE(gmbs[Top.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Top.mb_num, Top.block_num))) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Top.mb_num, Top.block_num);
                    return;
                }
                else
                {
                    goto median16x8_0;
                }
            }
            else
            {
median16x8_0:
                Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
                TopRight = m_cur_mb.CurrentBlockNeighbours.mb_above_right;
                TopLeft = m_cur_mb.CurrentBlockNeighbours.mb_above_left;

                // init vars for median prediction
                isRightUnavailable = (TopRight.mb_num<0);
                if (isRightUnavailable)
                    isLeftUnavailable = (TopLeft.mb_num<0);
            }
        }
        else
        {
            Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[2];
            // lower half, use predictor from left
            if ( Left.mb_num>=0)
            {

                if (IS_INTER_MBTYPE(gmbs[Left.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Left.mb_num, Left.block_num))) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
                    return;
                }
                else
                {
                    goto median_16x8_1;
                }
            }
            else
            {
median_16x8_1:

                Top.mb_num = m_CurMBAddr;
                Top.block_num = 4;

                TopLeft.block_num = 8;
                GetTopLeftLocationForCurrentMBLumaNonMBAFF(&TopLeft);

                // init vars for median prediction
                isRightUnavailable = 1;
                isLeftUnavailable = (Left.mb_num<0);
            }
        }
        break;
    case MBTYPE_INTER_8x16:
        VM_ASSERT(block >= 0 && block <= 1);
        // First check for availability of directional predictor which is
        // just used if available.
        if (block == 0)
        {
            // left half, use predictor from left
            //LeftBlockNum = block;
            //LeftMB=GetLeftBlock(pMBInfo,LeftBlockNum);
            Left = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
            if (Left.mb_num>=0)
            {
                if (IS_INTER_MBTYPE(gmbs[Left.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Left.mb_num, Left.block_num))) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
                    return;
                }
                else
                {
                    goto median_8x16_0;
                }
            }
            else
            {
median_8x16_0:
                /*                TopBlockNum=0;
                TopLeftBlockNum=0;
                TopRightBlockNum=1;

                TopMB=GetTopBlock(pMBInfo,TopBlockNum);
                TopLeftMB=GetTopLeftBlock(pMBInfo,TopLeftBlockNum);
                TopRightMB=GetTopRightBlock(pMBInfo,TopRightBlockNum);*/

                Top = m_cur_mb.CurrentBlockNeighbours.mb_above;
                TopRight = m_cur_mb.CurrentBlockNeighbours.mb_above;
                TopLeft= m_cur_mb.CurrentBlockNeighbours.mb_above_left;
                TopRight.block_num+=2;
                // init vars for median prediction
                isRightUnavailable = (Top.mb_num<0);
                if (isRightUnavailable)
                    isLeftUnavailable = (TopLeft.mb_num<0);
            }
        }
        else
        {
            // right half, use predictor from above right unless unavailable,
            // then try above left
            //TopRightBlockNum=3;
            //TopRightMB=GetTopRightBlock(pMBInfo,TopRightBlockNum);
            //TopBlockNum=2;
            //TopMB=GetTopBlock(pMBInfo,TopBlockNum);
            TopRight= m_cur_mb.CurrentBlockNeighbours.mb_above_right;
            Top= m_cur_mb.CurrentBlockNeighbours.mb_above;
            Top.block_num+=2;

            if ( TopRight.mb_num>=0)
            {
                if (IS_INTER_MBTYPE(gmbs[TopRight.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num))) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num);
                    return;
                }
            }
            else  if ( Top.mb_num>=0)
            {
                if (IS_INTER_MBTYPE(gmbs[Top.mb_num].mbtype) &&
                    (((GetReferenceIndex(m_gmbinfo, ListNum, Top.mb_num, Top.block_num-1))) == RefIndex))
                {
                    *mv = GetMV(m_gmbinfo, ListNum, Top.mb_num, Top.block_num-1);
                    return;
                }
            }

            //LeftBlockNum=2;
            //LeftMB=GetLeftBlock(pMBInfo,LeftBlockNum);
            Left.mb_num=m_CurMBAddr;
            Left.block_num=1;
            TopLeft=m_cur_mb.CurrentBlockNeighbours.mb_above;
            TopLeft.block_num++;
            // init vars for median prediction
            isRightUnavailable = (TopRight.mb_num<0);
            if (isRightUnavailable)
                isLeftUnavailable = (Top.mb_num<0);
        }
        //        diff = 2;
        break;
    case MBTYPE_INTER_8x8:
    case MBTYPE_INTER_8x8_REF0:
        {
            // Each 8x8 block of a macroblock can be subdivided into subblocks,
            // each having its own MV. The parameter 'block' has block and
            // subblock information:
            //  block 0..3, bits 2-3
            //  subblock 0..3, bits 0-1
            Ipp32s  left_edge_block = 0, top_edge_block = 0, right_edge_block = 0;

            switch (m_cur_mb.GlobalMacroblockInfo->sbtype[block>>2])
            {
            case SBTYPE_8x8:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    block2lin[block];
                TopRight.block_num=
                    block2lin[block]+1;
                    GetLeftLocationForCurrentMBLumaNonMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaNonMBAFF(&Top);
                    GetTopRightLocationForCurrentMBLumaNonMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaNonMBAFF(&TopLeft);
                switch (block>>2)
                {
                case 0:
                    left_edge_block = 1;
                    top_edge_block = 1;
                    isRightUnavailable = (Top.mb_num<0);
                    isLeftUnavailable = (TopLeft.mb_num<0);
                    break;
                case 1:
                    left_edge_block = 0;
                    top_edge_block = 1;
                    isRightUnavailable = (TopRight.mb_num<0);
                    isLeftUnavailable = (Top.mb_num<0);
                    break;
                case 2:
                    left_edge_block = 1;
                    top_edge_block = 0;
                    isRightUnavailable = 0;
                    break;
                case 3:
                    left_edge_block = 0;
                    top_edge_block = 0;
                    isRightUnavailable = 1;
                    isLeftUnavailable = 0;
                    break;
                }   // block
                right_edge_block = left_edge_block == 0 ? 1 : 0;
                break;
            case SBTYPE_8x4:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    block2lin[block&(-4)]+4*(block&1);
                TopRight.block_num=
                    block2lin[block&(-4)]+4*(block&1)+1;

                    GetLeftLocationForCurrentMBLumaNonMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaNonMBAFF(&Top);
                    GetTopRightLocationForCurrentMBLumaNonMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaNonMBAFF(&TopLeft);

                left_edge_block = left_edge_tab16_8x4[block];
                top_edge_block = top_edge_tab16_8x4[block];
                right_edge_block = right_edge_tab16_8x4[block];

                if (!top_edge_block)
                {
                    isRightUnavailable = (above_right_avail_8x4[block] == 0);
                    if (isRightUnavailable)
                    {
                        if (left_edge_block)
                            isLeftUnavailable = (Left.mb_num<0);
                        else
                            isLeftUnavailable = 0;
                    }
                }
                else
                {
                    // top edge of 8x4
                    if (!right_edge_block)
                    {
                        isRightUnavailable = (Top.mb_num<0);
                        isLeftUnavailable = (TopLeft.mb_num<0);
                    }
                    else
                    {
                        isRightUnavailable = (TopRight.mb_num<0);
                        isLeftUnavailable = (Top.mb_num<0);
                    }
                }
                break;
            case SBTYPE_4x8:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    block2lin[block&(-4)]+(block&1);
                TopRight.block_num=
                    block2lin[block&(-4)]+(block&1);

                    GetLeftLocationForCurrentMBLumaNonMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaNonMBAFF(&Top);
                    GetTopRightLocationForCurrentMBLumaNonMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaNonMBAFF(&TopLeft);
                //            diff = 1;
                left_edge_block = left_edge_tab16_4x8[block];
                top_edge_block = top_edge_tab16_4x8[block];
                right_edge_block = right_edge_tab16_4x8[block];
                if (!top_edge_block)
                {
                    isRightUnavailable = (above_right_avail_4x8[block] == 0);
                    isLeftUnavailable = 0;  // always, when above right not available
                }
                else
                {
                    if (!right_edge_block)
                        isRightUnavailable = (Top.mb_num<0);
                    else
                        isRightUnavailable = (TopRight.mb_num<0);
                    if (isRightUnavailable)
                    {
                        if (!left_edge_block)
                            isLeftUnavailable = (Top.mb_num<0);
                        else
                            isLeftUnavailable = (TopLeft.mb_num<0);
                    }
                }
                break;
            case SBTYPE_4x4:
                Top.block_num=
                    Left.block_num=
                    TopLeft.block_num=
                    TopRight.block_num=
                    block2lin[block];

                    GetLeftLocationForCurrentMBLumaNonMBAFF(&Left);
                    GetTopLocationForCurrentMBLumaNonMBAFF(&Top);
                    GetTopRightLocationForCurrentMBLumaNonMBAFF(&TopRight);
                    GetTopLeftLocationForCurrentMBLumaNonMBAFF(&TopLeft);
                VM_ASSERT(block >= 0 && block <= 15);
                //            diff = 1;
                left_edge_block = left_edge_tab16_[block];
                top_edge_block = top_edge_tab16_[block];
                right_edge_block = right_edge_tab16[block];
                if (!top_edge_block)
                    isRightUnavailable = (above_right_avail_4x4_[block] == 0);
                else
                {
                    if (!right_edge_block)
                        isRightUnavailable = (Top.mb_num<0);
                    else
                        isRightUnavailable = (TopRight.mb_num<0);
                }
                if (isRightUnavailable)
                {
                    // When not on top edge of MB, for blocks for which right may not
                    // be available, left is always available.
                    if (top_edge_block == 0)
                        isLeftUnavailable = 0;
                    else
                    {
                        if (!left_edge_block)
                            isLeftUnavailable = (Top.mb_num<0);
                        else
                            isLeftUnavailable = (TopLeft.mb_num<0);
                    }
                }
                break;
            }
        }

        break;
    default:
        *mv = zeroVector;
        return; // DEBUG : may be throw ???
    }

    // correct for left edge
    if (Left.mb_num<0)
    {
        sl = &null;
        pRefIxl = &nullRefIx;
    }
    else
    {
        sl = &GetMV(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
        pRefIxl = GetReferenceIndexPtr(m_gmbinfo, ListNum, Left.mb_num, Left.block_num);
    }

    // correct for top edge
    if (Top.mb_num<0)
    {
        sa = &null;
        pRefIxa = &nullRefIx;
    }
    else
    {
        sa = &GetMV(m_gmbinfo, ListNum, Top.mb_num, Top.block_num);
        pRefIxa = GetReferenceIndexPtr(m_gmbinfo, ListNum, Top.mb_num, Top.block_num);

    }

    // correct for upper right
    if (isRightUnavailable)
    {
        // replace with block above left if available
        if (isLeftUnavailable)
        {
            sr = &null;
            pRefIxr = &nullRefIx;
        }
        else
        {
            sr = &GetMV(m_gmbinfo, ListNum, TopLeft.mb_num, TopLeft.block_num);
            pRefIxr = GetReferenceIndexPtr(m_gmbinfo, ListNum, TopLeft.mb_num, TopLeft.block_num);
        }
    }
    else
    {
        sr = &GetMV(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num);
        pRefIxr = GetReferenceIndexPtr(m_gmbinfo, ListNum, TopRight.mb_num, TopRight.block_num);
    }

    // If above AND right are unavailable AND left is available, use left
    if ((sa == &null) && (sr == &null) && (sl != &null))
    {
        // return MV at sl
        mv->mvx = sl->mvx;
        mv->mvy = sl->mvy;
    }
    else
    {
        // Check for more than one predictor from different reference frame
        // If there is only one predictor from this ref frame, then sonly will
        // be pointing to it.
        if ((*pRefIxl) != RefIndex)
            uSameRefPicCount--;
        else
        {
            sonly = sl;
        }
        if ((*pRefIxa) != RefIndex)
            uSameRefPicCount--;
        else
        {
            sonly = sa;
        }
        if ((*pRefIxr) != RefIndex)
            uSameRefPicCount--;
        else
        {
            sonly = sr;
        }

        if (uSameRefPicCount != 1)
        {
            // "normal" median prediction
            px0 = sl->mvx;
            px1 = sa->mvx;
            px2 = sr->mvx;

#define MEDIAN_OF_3(a, b, c) (IPP_MIN((a),(b))) ^ (IPP_MIN((b),(c))) ^ (IPP_MIN((c),(a)))

            mv->mvx = (Ipp16s)(MEDIAN_OF_3(px0, px1, px2));

            py0 = sl->mvy;
            py1 = sa->mvy;
            py2 = sr->mvy;

            mv->mvy = (Ipp16s)(MEDIAN_OF_3(py0, py1, py2));
        }
        else
        {
            // return MV at sonly
            *mv = *sonly;
        }
    }
} // void H264SegmentDecoder::ComputeMotionVectorPredictors(const Ipp8u ListNum,

// Used to obtain colocated motion vector and reference index for temporal
// direct B. Called only when the colocated MB is not INTRA. Uses the
// L0 and L1 reference indices of colocated MB to choose MV. Also translates
// the colocated reference index to be used into the correct L0 index
// for this slice.
void H264SegmentDecoder::GetDirectTemporalMV(Ipp32s MBCol,
                                             Ipp32u ipos, // offset into MV and RefIndex storage
                                             H264DecoderMotionVector  *&MVL0, // return colocated MV here
                                             Ipp8s &RefIndexL0) // return ref index here
{
    //I'm not sure about this function correctness
    VM_ASSERT(m_pRefPicList[1][0]);
    H264DecoderMotionVector   *pRefMVL0;
    H264DecoderMotionVector   *pRefMVL1;
    const RefIndexType *pRefRefIndexL0;
    const RefIndexType *pRefRefIndexL1;
    H264DecoderFrame **pRefRefPicList;

    // Set pointers to colocated list 0 ref index and MV
    pRefRefIndexL0 = GetReferenceIndexPtr(&m_pRefPicList[1][0]->m_mbinfo, 0, MBCol, ipos);
    pRefMVL0 = &GetMV(&m_pRefPicList[1][0]->m_mbinfo, 0, MBCol, 0);
    Ipp16u uRefSliceNum=m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol].slice_id;

    VM_ASSERT(pRefRefIndexL0);
    VM_ASSERT(pRefMVL0);

    // Get ref index and MV from L0 of colocated ref MB if the
    // colocated L0 ref index is >=0. Else use L1.
    if (*pRefRefIndexL0 >= 0)
    {
        // Use colocated L0
        MVL0 = pRefMVL0;
        RefIndexL0 = *pRefRefIndexL0;

        // Get pointer to ref pic list 0 of colocated
        pRefRefPicList = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 0)->m_RefPicList;
    }
    else
    {
        // Use Ref L1

        // Set pointers to colocated list 1 ref index and MV
        pRefRefIndexL1 = GetReferenceIndexPtr(&m_pRefPicList[1][0]->m_mbinfo, 1, MBCol, ipos);
        pRefMVL1 = &GetMV(&m_pRefPicList[1][0]->m_mbinfo, 1, MBCol, 0);
        VM_ASSERT(pRefRefIndexL1);
        VM_ASSERT(pRefMVL1);
        RefIndexL0 = *pRefRefIndexL1;

        // Use colocated L1
        MVL0 = pRefMVL1;

        // Get pointer to ref pic list 1 of colocated
        pRefRefPicList = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 1)->m_RefPicList;

    }
    if (m_pRefPicList[1][0]->m_PictureStructureForDec==AFRM_STRUCTURE)
    {
        VM_ASSERT(0);//can't happen
        AdjustIndex((MBCol & 1), GetMBFieldDecodingFlag(m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol]), RefIndexL0);
    }

    VM_ASSERT(pRefRefPicList[RefIndexL0]);

    Ipp32s index = pRefRefPicList[RefIndexL0]->m_index;

    // find matching reference frame on current slice list 0
    RefIndexL0 = 0;
    for (; m_pRefPicList[0][RefIndexL0]; RefIndexL0++)
    {
        if (m_pRefPicList[0][RefIndexL0]->m_index == index)
        {
            return;
        }
    }

    // can't happen
    RefIndexL0 = 0;

} // void H264SegmentDecoder::GetDirectTemporalMV(Ipp32s MBCol,

void H264SegmentDecoder::GetDirectTemporalMVFLD(Ipp32s MBCol,
                                                Ipp32u ipos, // offset into MV and RefIndex storage
                                                H264DecoderMotionVector *& MVL0, // return colocated MV here
                                                Ipp8s &RefIndexL0) // return ref index here
{
    //I'm not sure about this function correctness
    VM_ASSERT(m_pRefPicList[1][0]);
    H264DecoderMotionVector   *pRefMVL0;
    H264DecoderMotionVector   *pRefMVL1;
    const RefIndexType *pRefRefIndexL0;
    const RefIndexType *pRefRefIndexL1;
    H264DecoderFrame **pRefRefPicList;
    // Set pointers to colocated list 0 ref index and MV
    pRefRefIndexL0 = GetReferenceIndexPtr(&m_pRefPicList[1][0]->m_mbinfo, 0, MBCol, ipos);
    pRefMVL0 = &GetMV(&m_pRefPicList[1][0]->m_mbinfo, 0, MBCol, 0);
    Ipp32s uRefSliceNum = m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol].slice_id;
    ReferenceFlags *pRefFields;
    VM_ASSERT(pRefRefIndexL0);
    VM_ASSERT(pRefMVL0);

    // Get ref index and MV from L0 of colocated ref MB if the
    // colocated L0 ref index is >=0. Else use L1.
    if (*pRefRefIndexL0 >= 0)
    {
        // Use colocated L0
        MVL0 = pRefMVL0;
        RefIndexL0 = *pRefRefIndexL0;

        // Get pointer to ref pic list 0 of colocated
        pRefRefPicList = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 0)->m_RefPicList;
        pRefFields = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 0)->m_Flags;
    }
    else
    {
        // Use Ref L1

        // Set pointers to colocated list 1 ref index and MV
        pRefRefIndexL1 = GetReferenceIndexPtr(&m_pRefPicList[1][0]->m_mbinfo, 1, MBCol, ipos);
        pRefMVL1 = &GetMV(&m_pRefPicList[1][0]->m_mbinfo, 1, MBCol, 0);
        VM_ASSERT(pRefRefIndexL1);
        VM_ASSERT(pRefMVL1);
        RefIndexL0 = *pRefRefIndexL1;

        // Use colocated L1
        MVL0 = pRefMVL1;

        // Get pointer to ref pic list 1 of colocated
        pRefRefPicList = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 1)->m_RefPicList;
        pRefFields = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 1)->m_Flags;
    }

    // Translate the reference index of the colocated to current
    // L0 index to the same reference picture, using picNum or
    // LongTermPicNum as id criteria.
    Ipp32s num_ref;
    Ipp32s force_value;
    if (m_pRefPicList[1][0]->m_PictureStructureForDec == FRM_STRUCTURE)
    {
        num_ref = m_field_index;
        force_value = 1;
    }
    else if (m_pRefPicList[1][0]->m_PictureStructureForDec == AFRM_STRUCTURE)
    {
        if (GetMBFieldDecodingFlag(m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol]))
        {
            Ipp32s field_selector = RefIndexL0&1;
            RefIndexL0>>=1;
            num_ref = (field_selector^(MBCol & 1));
            force_value = 1;
        }
        else
        {
            num_ref = m_field_index;
            force_value = 1;
        }
    }
    else
    {
        num_ref = GetReferenceField(pRefFields, RefIndexL0);
        force_value = 1;
    }
    VM_ASSERT(pRefRefPicList[RefIndexL0]);

    Ipp32s index = pRefRefPicList[RefIndexL0]->m_index;

    // find matching reference frame on current slice list 0
    RefIndexL0 = 0;
    for (; m_pRefPicList[0][RefIndexL0] != NULL; RefIndexL0++)
    {
        if (m_pRefPicList[0][RefIndexL0]->m_index == index &&
            (force_value == 3 || GetReferenceField(m_pFields[0], RefIndexL0) == num_ref))
        {
            return;
        }
    }

    // can't happen
    RefIndexL0 = 0;

} // void H264SegmentDecoder::GetDirectTemporalMVFLD(Ipp32s MBCol,

void H264SegmentDecoder::GetDirectTemporalMVMBAFF(Ipp32s MBCol,
                                                  Ipp32u ipos, // offset into MV and RefIndex storage
                                                  H264DecoderMotionVector *& MVL0, // return colocated MV here
                                                  Ipp8s &RefIndexL0) // return ref index here
{
    //I'm not sure about this function correctness
    VM_ASSERT(m_pRefPicList[1][0]);
    H264DecoderMotionVector   *pRefMVL0;
    H264DecoderMotionVector   *pRefMVL1;
    const RefIndexType *pRefRefIndexL0;
    const RefIndexType *pRefRefIndexL1;
    H264DecoderFrame **pRefRefPicList;
    // Set pointers to colocated list 0 ref index and MV
    pRefRefIndexL0 = GetReferenceIndexPtr(&m_pRefPicList[1][0]->m_mbinfo, 0, MBCol, ipos);
    pRefMVL0 = &GetMV(&m_pRefPicList[1][0]->m_mbinfo, 0, MBCol, 0);
    Ipp16u uRefSliceNum=m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol].slice_id;
    Ipp32u scale_idx=GetMBFieldDecodingFlag(m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol]);
    Ipp32u back_scale_idx=pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo);
    Ipp32u field_selector = 0;
    ReferenceFlags *pFields;

    VM_ASSERT(pRefRefIndexL0);
    VM_ASSERT(pRefMVL0);

    // Get ref index and MV from L0 of colocated ref MB if the
    // colocated L0 ref index is >=0. Else use L1.
    RefIndexL0 = *pRefRefIndexL0;
    if (RefIndexL0 >= 0)
    {
        // Use colocated L0
        MVL0 = pRefMVL0;

        // Get pointer to ref pic list 0 of colocated
        pRefRefPicList = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 0)->m_RefPicList;
        pFields = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 0)->m_Flags;
    }
    else
    {
        // Use Ref L1

        // Set pointers to colocated list 1 ref index and MV
        pRefRefIndexL1 = GetReferenceIndexPtr(&m_pRefPicList[1][0]->m_mbinfo, 1, MBCol, ipos);
        pRefMVL1 = &GetMV(&m_pRefPicList[1][0]->m_mbinfo, 1, MBCol, 0);
        pFields = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 1)->m_Flags;
        VM_ASSERT(pRefRefIndexL1);
        VM_ASSERT(pRefMVL1);
        RefIndexL0 = *pRefRefIndexL1;

        // Use colocated L1
        MVL0 = pRefMVL1;

        // Get pointer to ref pic list 1 of colocated
        pRefRefPicList = m_pRefPicList[1][0]->GetRefPicList(uRefSliceNum, 1)->m_RefPicList;
    }

    if (m_pRefPicList[1][0]->m_PictureStructureForDec == AFRM_STRUCTURE)
    {
        AdjustIndex((MBCol & 1), GetMBFieldDecodingFlag(m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol]), RefIndexL0);
        field_selector = RefIndexL0&scale_idx;
        RefIndexL0>>=scale_idx;
    }
    else if (m_pRefPicList[1][0]->m_PictureStructureForDec < FRM_STRUCTURE)
    {
        Ipp8s ref1field = (MBCol >= m_pRefPicList[1][0]->totalMBs);
        field_selector = (ref1field != GetReferenceField(pFields, RefIndexL0));
    }

    VM_ASSERT(pRefRefPicList[RefIndexL0]);
    VM_ASSERT(m_pRefPicList[1][0]->m_PictureStructureForDec != FRM_STRUCTURE);

    Ipp32s index = pRefRefPicList[RefIndexL0]->m_index;

    // find matching reference frame on current slice list 0
    RefIndexL0 = 0;
    for (; m_pRefPicList[0][RefIndexL0]; RefIndexL0++)
    {
        if (m_pRefPicList[0][RefIndexL0]->m_index == index)
        {
            RefIndexL0 <<= back_scale_idx;
            RefIndexL0 = (Ipp8s)(RefIndexL0 | (field_selector&back_scale_idx));
            return;
        }
    }

    // can't happen
    RefIndexL0 = 0;

} // void H264SegmentDecoder::GetDirectTemporalMVMBAFF(Ipp32s MBCol,

inline void GetScaledMV(Ipp32s pos,
                        H264DecoderMotionVector *directMVs,
                        Ipp32s *pDistScaleFactorMV,
                        Ipp32s RefIndexL0,
                        Ipp32s scale,
                        Ipp32s scale_idx,
                        H264DecoderMotionVector &pFwdMV,
                        H264DecoderMotionVector &pBwdMV,
                        Ipp32s mvDistortion[2])
{
    H264DecoderMotionVector MV = directMVs[pos];

    switch(scale)
    {
    case 1:
        MV.mvy /= 2;
        break;
    case -1:
        MV.mvy *= 2;
        break;
    }

    pFwdMV.mvx = (Ipp16s)((MV.mvx * pDistScaleFactorMV[RefIndexL0 >> scale_idx] + 128) >> 8);
    pFwdMV.mvy = (Ipp16s)((MV.mvy * pDistScaleFactorMV[RefIndexL0 >> scale_idx] + 128) >> 8);
    pBwdMV.mvx = (Ipp16s)(pFwdMV.mvx - MV.mvx);
    pBwdMV.mvy = (Ipp16s)(pFwdMV.mvy - MV.mvy);

    if (pFwdMV.mvy > mvDistortion[0])
        mvDistortion[0] = pFwdMV.mvy;

    if (pBwdMV.mvy > mvDistortion[1])
        mvDistortion[1] = pBwdMV.mvy;
}

void H264SegmentDecoder::DecodeDirectMotionVectorsTemporal_8x8Inference()
{
    // Use forward and backward ratios to scale the reference vectors to
    // produce the forward and backward vectors, for either a 16x16 macroblock
    // (16 vectors) or an 8x8 block (4 vectors)

    // When bRefMBIsInter is false, set the MV to zero.

    FactorArrayValue *pDistScaleFactorMV;
    Ipp32u sb;
    Ipp32s ref_mvoffset, sboffset = 0;
    Ipp8s RefIndexL0 = 0, RefIndexL1 = 0;
    H264DecoderMotionVector  MV = {0};
    Ipp32s mvxf, mvyf, mvxb, mvyb;

    // set up pointers to where MV and RefIndex will be stored
    H264DecoderMotionVector *pFwdMV = m_cur_mb.MVs[0]->MotionVectors;
    H264DecoderMotionVector *pBwdMV = m_cur_mb.MVs[1]->MotionVectors;
    RefIndexType *pRefIndexL0 = m_cur_mb.GetReferenceIndexStruct(0)->refIndexs;
    RefIndexType *pRefIndexL1 = m_cur_mb.GetReferenceIndexStruct(1)->refIndexs;
    Ipp32u scale_idx = pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo);

    bool isAll8x8Same = true;
    H264DecoderMotionVector * first_mv_fwd_all = &(pFwdMV[0]);
    H264DecoderMotionVector * first_mv_bwd_all = &(pBwdMV[0]);
    RefIndexType *first_refL0_all = &pRefIndexL0[0];
    RefIndexType *first_refL1_all = &pRefIndexL1[0];

    for (sb = 0; sb<4; sb++)
    {
        if (m_cur_mb.GlobalMacroblockInfo->sbtype[sb] != SBTYPE_DIRECT)
        {
            isAll8x8Same = false;
            continue;
        }

        switch (sb)
        {
        case 0:
            ref_mvoffset = 0;   // upper left corner
            sboffset = 0;
            break;
        case 1:
            ref_mvoffset = 3;   // upper right corner
            sboffset = 2;
            break;
        case 2:
            ref_mvoffset = 12;   // lower left corner
            sboffset = 8;
            break;
        case 3:
            ref_mvoffset = 3 + 12;   // lower right corner
            sboffset = 2 + 8;
            break;
        }
        Ipp32s MBCol;
        Ipp32s scale;
        MBCol = GetColocatedLocation(m_pRefPicList[1][0], GetReferenceField(m_pFields[1], 0), ref_mvoffset, &scale);
        bool bRefMBIsInter = IS_INTER_MBTYPE(m_pRefPicList[1][0]->m_mbinfo.mbs[MBCol].mbtype);
        RefIndexL1 = 0;
        if (bRefMBIsInter)
        {
            H264DecoderMotionVector * directMVs;

            // Get colocated MV and translated ref index
            switch(m_pCurrentFrame->m_PictureStructureForDec)
            {
            case FLD_STRUCTURE:
                GetDirectTemporalMVFLD(MBCol, ref_mvoffset, directMVs, RefIndexL0);
                break;
            case FRM_STRUCTURE:
                GetDirectTemporalMV(MBCol,ref_mvoffset, directMVs, RefIndexL0);
                break;
            case AFRM_STRUCTURE:
                GetDirectTemporalMVMBAFF(MBCol,ref_mvoffset, directMVs, RefIndexL0);
                break;
            default:
                throw h264_exception(UMC_ERR_INVALID_STREAM);
            }

            if (scale_idx)
            {
                Ipp32s curfield = (m_CurMBAddr & 1);
                Ipp32s ref0field = curfield ^ (RefIndexL0&1);
                pDistScaleFactorMV = m_pSlice->GetDistScaleFactorMVAFF()->values[curfield][ref0field][curfield];
            }
            else
            {
                pDistScaleFactorMV = m_pSlice->GetDistScaleFactorMV()->values;
            }

            MV = directMVs[ref_mvoffset];
            switch(scale)
            {
            case 1: MV.mvy/=2;
                break;
            case -1:MV.mvy*=2;
                break;
            }
            // Reference MV from outside corner 4x4
            mvxf = Ipp32s
                ((MV.mvx * pDistScaleFactorMV[RefIndexL0>>scale_idx] + 128) >> 8);
            mvxb = mvxf - MV.mvx;
            mvyf = Ipp32s
                ((MV.mvy * pDistScaleFactorMV[RefIndexL0>>scale_idx] + 128) >> 8);
            mvyb = mvyf - MV.mvy;
        }
        else
        {
            mvxf = 0;
            mvyf = 0;
            mvxb = 0;
            mvyb = 0;
            RefIndexL0 = 0;
        }

        if (mvyf > m_MVDistortion[0])
            m_MVDistortion[0] = mvyf;

        if (mvyb > m_MVDistortion[1])
            m_MVDistortion[1] = mvyb;

        // Save MV to all 4 4x4's for this 8x8.
        pFwdMV[sboffset].mvx = (Ipp16s) mvxf;
        pFwdMV[sboffset].mvy = (Ipp16s) mvyf;
        pBwdMV[sboffset].mvx = (Ipp16s) mvxb;
        pBwdMV[sboffset].mvy = (Ipp16s) mvyb;

        pFwdMV[sboffset+1].mvx = (Ipp16s) mvxf;
        pFwdMV[sboffset+1].mvy = (Ipp16s) mvyf;
        pBwdMV[sboffset+1].mvx = (Ipp16s) mvxb;
        pBwdMV[sboffset+1].mvy = (Ipp16s) mvyb;

        pFwdMV[sboffset+4].mvx = (Ipp16s) mvxf;
        pFwdMV[sboffset+4].mvy = (Ipp16s) mvyf;
        pBwdMV[sboffset+4].mvx = (Ipp16s) mvxb;
        pBwdMV[sboffset+4].mvy = (Ipp16s) mvyb;

        pFwdMV[sboffset+4+1].mvx = (Ipp16s) mvxf;
        pFwdMV[sboffset+4+1].mvy = (Ipp16s) mvyf;
        pBwdMV[sboffset+4+1].mvx = (Ipp16s) mvxb;
        pBwdMV[sboffset+4+1].mvy = (Ipp16s) mvyb;

        pRefIndexL0[sb] = RefIndexL0;
        pRefIndexL1[sb] = RefIndexL1;

        m_cur_mb.LocalMacroblockInfo->sbdir[sb]=D_DIR_DIRECT;
        m_cur_mb.GlobalMacroblockInfo->sbtype[sb]=SBTYPE_8x8;


        if (isAll8x8Same)
        {
            isAll8x8Same &=
                (pFwdMV[sboffset].mvx == first_mv_fwd_all->mvx) &
                (pFwdMV[sboffset].mvy == first_mv_fwd_all->mvy) &
                (pBwdMV[sboffset].mvx == first_mv_bwd_all->mvx) &
                (pBwdMV[sboffset].mvy == first_mv_bwd_all->mvy) &
                (RefIndexL0 == *first_refL0_all) &
                (RefIndexL1 == *first_refL1_all);
        }
    }   // for sb


    // set mbtype to 8x8 if it was not; use larger type if possible
    if (isAll8x8Same)
    {
        m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_BIDIR;
    }
    else
    {
        m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTER_8x8;
    }

} // void H264SegmentDecoder::DecodeDirectMotionVectorsTemporal_8x8Inference(H264DecoderFrame **pRefPicList0,

void H264SegmentDecoder::ComputeDirectSpatialRefIdx(Ipp32s *pRefIndexL0,
                                                    Ipp32s *pRefIndexL1)
{
    Ipp32s refIdxL0 = -1;
    Ipp32s refIdxL1 = -1;

    // usual case
    if (false == m_isMBAFF)
    {
        H264DecoderBlockLocation mbAddr;

        // get left block location
        mbAddr = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
        if (0 <= mbAddr.mb_num)
        {
            refIdxL0 = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
            refIdxL1 = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
        }

        // get aboue location
        mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above;
        if (0 <= mbAddr.mb_num)
        {
            Ipp32u tmp;

            tmp = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
            refIdxL0 = (((Ipp32u) refIdxL0) <= tmp) ? (refIdxL0) : (tmp);
            tmp = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
            refIdxL1 = (((Ipp32u) refIdxL1) <= tmp) ? (refIdxL1) : (tmp);
        }

        // get above left (right) location
        mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above_right;
        if (0 <=  mbAddr.mb_num)
        {
            Ipp32u tmp;

            tmp = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
            refIdxL0 = (((Ipp32u) refIdxL0) <= tmp) ? (refIdxL0) : (tmp);
            tmp = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
            refIdxL1 = (((Ipp32u) refIdxL1) <= tmp) ? (refIdxL1) : (tmp);
        }
        else
        {
            mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above_left;
            if (0 <= mbAddr.mb_num)
            {
                Ipp32u tmp;

                tmp = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
                refIdxL0 = (((Ipp32u) refIdxL0) <= tmp) ? (refIdxL0) : (tmp);
                tmp = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
                refIdxL1 = (((Ipp32u) refIdxL1) <= tmp) ? (refIdxL1) : (tmp);
            }
        }
    }
    // MBAFF case
    else
    {
        H264DecoderBlockLocation mbAddr;
        H264DecoderMacroblockGlobalInfo *pInfo;
        Ipp32s cur_field;

        pInfo = m_gmbinfo->mbs;
        cur_field = GetMBFieldDecodingFlag(pInfo[m_CurMBAddr]);

        //
        // do not try to find corresponding place in the standard
        // we use optimized calculations
        //

        // get left block location
        mbAddr = m_cur_mb.CurrentBlockNeighbours.mbs_left[0];
        if (0 <= mbAddr.mb_num)
        {
            Ipp32s neighbour_frame = GetMBFieldDecodingFlag(pInfo[mbAddr.mb_num]) ^ 1;

            refIdxL0 = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
            refIdxL1 = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
            refIdxL0 <<= (cur_field + neighbour_frame);
            refIdxL1 <<= (cur_field + neighbour_frame);
        }

        // get aboue location
        mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above;
        if (0 <= mbAddr.mb_num)
        {
            Ipp32u tmp;
            Ipp32s neighbour_frame = GetMBFieldDecodingFlag(pInfo[mbAddr.mb_num]) ^ 1;

            tmp = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
            tmp <<= (cur_field + neighbour_frame);
            refIdxL0 = (((Ipp32u) refIdxL0) <= tmp) ? (refIdxL0) : (tmp);
            tmp = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
            tmp <<= (cur_field + neighbour_frame);
            refIdxL1 = (((Ipp32u) refIdxL1) <= tmp) ? (refIdxL1) : (tmp);
        }

        // get above left (right) location
        mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above_right;
        if (0 <=  mbAddr.mb_num)
        {
            Ipp32u tmp;
            Ipp32s neighbour_frame = GetMBFieldDecodingFlag(pInfo[mbAddr.mb_num]) ^ 1;

            tmp = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
            tmp <<= (cur_field + neighbour_frame);
            refIdxL0 = (((Ipp32u) refIdxL0) <= tmp) ? (refIdxL0) : (tmp);
            tmp = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
            tmp <<= (cur_field + neighbour_frame);
            refIdxL1 = (((Ipp32u) refIdxL1) <= tmp) ? (refIdxL1) : (tmp);
        }
        else
        {
            mbAddr = m_cur_mb.CurrentBlockNeighbours.mb_above_left;
            if (0 <= mbAddr.mb_num)
            {
                Ipp32u tmp;
                Ipp32s neighbour_frame = GetMBFieldDecodingFlag(pInfo[mbAddr.mb_num]) ^ 1;

                tmp = GetReferenceIndex(m_gmbinfo, 0, mbAddr.mb_num, mbAddr.block_num);
                tmp <<= (cur_field + neighbour_frame);
                refIdxL0 = (((Ipp32u) refIdxL0) <= tmp) ? (refIdxL0) : (tmp);
                tmp = GetReferenceIndex(m_gmbinfo, 1, mbAddr.mb_num, mbAddr.block_num);
                tmp <<= (cur_field + neighbour_frame);
                refIdxL1 = (((Ipp32u) refIdxL1) <= tmp) ? (refIdxL1) : (tmp);
            }
        }

        // correct reference indexes
        refIdxL0 >>= 1;
        refIdxL1 >>= 1;
    }

    // save reference indexes
    *pRefIndexL0 = refIdxL0;
    *pRefIndexL1 = refIdxL1;

} // void H264SegmentDecoder::ComputeDirectSpatialRefIdx(Ipp32s *pRefIndexL0,

void H264SegmentDecoder::DecodeMBQPDelta_CAVLC(void)
{
    // Update QP with delta from bitstream
    Ipp32s qpdelta = m_pBitStream->GetVLCElement(true);
    Ipp32s bitdepth_luma_qp_scale;

    m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s)(m_cur_mb.LocalMacroblockInfo->QP + qpdelta);

    bitdepth_luma_qp_scale = 6 * (bit_depth_luma - 8);
    m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s)( ((m_cur_mb.LocalMacroblockInfo->QP +
                                                  52 +
                                                  2 * bitdepth_luma_qp_scale) %
                                                 (bitdepth_luma_qp_scale + 52)) -
                                                bitdepth_luma_qp_scale);

    if (qpdelta > QP_MAX/2 || qpdelta < (-QP_MAX-1)/2)
    {
        // following type of h.264 standard
        // we should return from function with error.
        // but we don't return to be compatible with
        // old h.264 files.
    }
} // void H264SegmentDecoder::DecodeMBQPDelta_CAVLC(void)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
