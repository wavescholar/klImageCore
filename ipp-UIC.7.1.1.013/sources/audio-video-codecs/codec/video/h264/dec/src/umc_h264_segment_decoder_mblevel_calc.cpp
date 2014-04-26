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


namespace UMC
{

Ipp32s H264SegmentDecoder::GetColocatedLocation(H264DecoderFrame *pRefFrame,
                                                Ipp32s Field,
                                                Ipp32s &block,
                                                Ipp32s *scale)
{
    Ipp32s location = -1;
    Ipp32u cur_pic_struct = m_pCurrentFrame->m_PictureStructureForDec;
    Ipp32u ref_pic_struct = pRefFrame->m_PictureStructureForDec;
    Ipp32s xCol = block & 3;
    Ipp32s yCol = block - xCol;

    if (cur_pic_struct==FRM_STRUCTURE && ref_pic_struct==FRM_STRUCTURE)
    {
        if (scale)
            *scale = 0;
        return m_CurMBAddr;
    }
    else if (cur_pic_struct==AFRM_STRUCTURE && ref_pic_struct==AFRM_STRUCTURE)
    {
        Ipp32s preColMBAddr=m_CurMBAddr;
        H264DecoderMacroblockGlobalInfo *preColMB = &pRefFrame->m_mbinfo.mbs[preColMBAddr];
        Ipp32s cur_mbfdf = pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo);
        Ipp32s ref_mbfdf = pGetMBFieldDecodingFlag(preColMB);

        if (cur_mbfdf == ref_mbfdf)
        {
            if (scale)
                *scale = 0;
            return m_CurMBAddr;
        }
        else if (cur_mbfdf > ref_mbfdf) //current - field reference - frame
        {
            if ((preColMBAddr & 1))
            {
                preColMBAddr-=1;//get pair
                if (yCol >= 8)
                    preColMBAddr += 1;//get pair again
            }
            else
            {
                if (yCol >= 8)
                    preColMBAddr += 1;//get pair
            }

            yCol *= 2;
            yCol &= 15;
            if (scale)
                *scale = 1;
        }
        else
        {
            Ipp32s curPOC = m_pCurrentFrame->PicOrderCnt(0,3);
            Ipp32s topPOC = pRefFrame->PicOrderCnt(0,1);
            Ipp32s bottomPOC = pRefFrame->PicOrderCnt(1,1);

            preColMBAddr &= -2; // == (preColMBAddr/2)*2;
            if (abs(topPOC - curPOC) >= abs(bottomPOC - curPOC))
                preColMBAddr += 1;

            yCol = (m_CurMBAddr & 1)*8 + 4*(yCol/8);
            if (scale)
                *scale = -1;
        }

        block = yCol + xCol;
        return preColMBAddr;
    }
    else if (cur_pic_struct==FLD_STRUCTURE && ref_pic_struct==FLD_STRUCTURE)
    {
        if (scale)
            *scale = 0;
        Ipp32s RefField = Field;

        if(RefField > m_field_index)
        {
            return (m_CurMBAddr + m_pCurrentFrame->totalMBs);
        }

        return RefField < m_field_index ? (m_CurMBAddr - m_pCurrentFrame->totalMBs) : m_CurMBAddr;
    }
    else if (cur_pic_struct == FLD_STRUCTURE && ref_pic_struct == FRM_STRUCTURE)
    {
        Ipp32u PicWidthInMbs = mb_width;
        Ipp32u CurrMbAddr = m_field_index ? m_CurMBAddr - m_pCurrentFrame->totalMBs : m_CurMBAddr;
        if(scale)
            *scale = 1;
        yCol = ((2*yCol)&15);
        block = yCol+xCol;
        return 2*PicWidthInMbs*(CurrMbAddr/PicWidthInMbs) +
            (CurrMbAddr%PicWidthInMbs) + PicWidthInMbs*(yCol/8);
    }
    else if (cur_pic_struct == FRM_STRUCTURE && ref_pic_struct == FLD_STRUCTURE)
    {
        if (scale)
            *scale=-1;

        Ipp32u PicWidthInMbs = mb_width;
        Ipp32u CurrMbAddr = m_CurMBAddr;
        yCol = 8*((CurrMbAddr/PicWidthInMbs)&1) + 4 * (yCol/8);
        block = yCol+xCol;

        Ipp32s curPOC = m_pCurrentFrame->PicOrderCnt(0,3);
        Ipp32s topPOC = pRefFrame->PicOrderCnt(pRefFrame->GetNumberByParity(0), 1);
        Ipp32s bottomPOC = pRefFrame->PicOrderCnt(pRefFrame->GetNumberByParity(1), 1);

        Ipp32s add = 0;
        if (abs(curPOC - topPOC) >= abs(curPOC - bottomPOC))
        {
            add = pRefFrame->totalMBs;
        }

        return (PicWidthInMbs*(CurrMbAddr/(2*PicWidthInMbs))+(CurrMbAddr%PicWidthInMbs)) + add;
    }
    else if (cur_pic_struct == FLD_STRUCTURE && ref_pic_struct == AFRM_STRUCTURE)
    {
        Ipp32u CurrMbAddr = m_CurMBAddr;
        if (m_field_index)
            CurrMbAddr -= m_pCurrentFrame->totalMBs;
        Ipp32s bottom_field_flag = m_field_index;
        Ipp32s preColMBAddr = 2*CurrMbAddr;

        H264DecoderMacroblockGlobalInfo *preColMB = &pRefFrame->m_mbinfo.mbs[preColMBAddr];
        Ipp32s col_mbfdf = pGetMBFieldDecodingFlag(preColMB);

        if (!col_mbfdf)
        {
            if (yCol >= 8)
                preColMBAddr += 1;
            yCol = ((2*yCol)&15);
            if(scale)
                *scale=1;
        }
        else
        {
            if (bottom_field_flag)
                preColMBAddr += 1;
            if(scale)
                *scale=0;
        }

        block = yCol + xCol;
        return preColMBAddr;
    }
    else if (cur_pic_struct == AFRM_STRUCTURE && ref_pic_struct == FLD_STRUCTURE)
    {
        Ipp32u CurrMbAddr = m_CurMBAddr;
        Ipp32s preColMBAddr = CurrMbAddr;

        Ipp32s cur_mbfdf = pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo);
        Ipp32s cur_mbbf = (m_CurMBAddr & 1);
        Ipp32s curPOC = m_pCurrentFrame->PicOrderCnt(0,3);
        Ipp32s topPOC = pRefFrame->PicOrderCnt(pRefFrame->GetNumberByParity(0), 1);
        Ipp32s bottomPOC = pRefFrame->PicOrderCnt(pRefFrame->GetNumberByParity(1), 1);

        Ipp32s bottom_field_flag = cur_mbfdf ? cur_mbbf :
                    abs(curPOC - topPOC) >= abs(curPOC - bottomPOC);

        if (cur_mbbf)
            preColMBAddr-=1;
        preColMBAddr = preColMBAddr/2;

        if (!cur_mbfdf)
        {
            yCol = 8*cur_mbbf + 4*(yCol/8);
            if (scale)
                *scale = -1;
        }
        else
        {
            if(scale)
                *scale = 0;
        }

        block = yCol + xCol;
        VM_ASSERT(preColMBAddr +(bottom_field_flag)*pRefFrame->totalMBs < m_pCurrentFrame->totalMBs);
        return preColMBAddr + (bottom_field_flag)*pRefFrame->totalMBs;
    }
    else // ARFM and FRM it's non-standard case.
    {
        VM_ASSERT(0);

        Ipp32s preColMBAddr=m_CurMBAddr;
        H264DecoderMacroblockGlobalInfo *preColMB = &pRefFrame->m_mbinfo.mbs[preColMBAddr];
        Ipp32s cur_mbfdf = pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo);
        Ipp32s ref_mbfdf = pGetMBFieldDecodingFlag(preColMB);

        if (cur_mbfdf == ref_mbfdf)
        {
            if (scale)
                *scale = 0;
            return m_CurMBAddr;
        }
        else if (cur_mbfdf > ref_mbfdf) //current - field reference - frame
        {
            if ((preColMBAddr & 1))
            {
                preColMBAddr-=1;//get pair
                if (yCol >= 8)
                    preColMBAddr += 1;//get pair again
            }
            else
            {
                if (yCol >= 8)
                    preColMBAddr += 1;//get pair
            }

            yCol *= 2;
            yCol &= 15;
            if (scale)
                *scale = 1;
        }
        else
        {
            Ipp32s curPOC = m_pCurrentFrame->PicOrderCnt(0,3);
            Ipp32s topPOC = pRefFrame->PicOrderCnt(0,1);
            Ipp32s bottomPOC = pRefFrame->PicOrderCnt(1,1);

            preColMBAddr &= -2; // == (preColMBAddr/2)*2;
            if (abs(topPOC - curPOC) >= abs(bottomPOC - curPOC))
                preColMBAddr += 1;

            yCol = (m_CurMBAddr & 1)*8 + 4*(yCol/8);
            if (scale)
                *scale = -1;
        }

        block = yCol + xCol;
        location = preColMBAddr;
    }

    return location;

} // Ipp32s H264SegmentDecoder::GetColocatedLocation(DecodedFrame *pRefFrame, Ipp8u Field, Ipp32s &block, Ipp8s *scale)

void H264SegmentDecoder::AdjustIndex(Ipp32s ref_mb_is_bottom, Ipp32s ref_mb_is_field, Ipp8s &RefIdx)
{
    if (RefIdx<0)
    {
        RefIdx=0;
        return;
    }

    if (ref_mb_is_field) //both are AFRM
    {
        Ipp32s cur_mb_is_bottom = (m_CurMBAddr & 1);
        Ipp32s cur_mb_is_field = pGetMBFieldDecodingFlag(m_cur_mb.GlobalMacroblockInfo);
        if (cur_mb_is_field)
        {
            bool same_parity = (((RefIdx&1) ^ ref_mb_is_bottom) == cur_mb_is_bottom);
            if (same_parity)
            {
                RefIdx&=-2;
            }
            else
            {
                RefIdx|=1;
            }
        }
        else if (m_pCurrentFrame->m_PictureStructureForDec!=AFRM_STRUCTURE)
        {
            RefIdx>>=1;
        }
    }
} // void H264SegmentDecoder::AdjustIndex(Ipp8u ref_mb_is_bottom, Ipp8s ref_mb_is_field, Ipp8s &RefIdx)

} // namespace UMC
#endif // UMC_ENABLE_H264_VIDEO_DECODER
