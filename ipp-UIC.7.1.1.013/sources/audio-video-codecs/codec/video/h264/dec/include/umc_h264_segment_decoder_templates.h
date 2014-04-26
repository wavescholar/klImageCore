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

#ifndef __UMC_H264_SEGMENT_DECODER_TEMPLATES_H
#define __UMC_H264_SEGMENT_DECODER_TEMPLATES_H

#include "umc_h264_dec_internal_cabac.h"
#include "umc_h264_reconstruct_templates.h"
#include "umc_h264_redual_decoder_templates.h"

#include "umc_h264_timing.h"

namespace UMC
{

#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

template <Ipp32s color_format, typename Coeffs, typename PlaneY, typename PlaneUV, Ipp32s is_field>
class ColorSpecific
{
public:

    static inline void UpdateNeighbouringBlocks(H264SegmentDecoderMultiThreaded * sd)
    {
        switch(color_format)
        {
        case 3:
            sd->UpdateNeighbouringBlocksH4();
            break;
        case 2:
            sd->UpdateNeighbouringBlocksH2();
            break;
        default:
            // field of frame case
            if (!sd->m_isMBAFF)
            {
                H264DecoderBlockNeighboursInfo *p = &(sd->m_cur_mb.CurrentBlockNeighbours);
                H264DecoderMacroblockNeighboursInfo *pMBs = &(sd->m_cur_mb.CurrentMacroblockNeighbours);

                p->mbs_left[0].mb_num =
                p->mbs_left[1].mb_num =
                p->mbs_left[2].mb_num =
                p->mbs_left[3].mb_num = pMBs->mb_A;
                p->mb_above.mb_num = pMBs->mb_B;
                p->mb_above_right.mb_num = pMBs->mb_C;
                p->mb_above_left.mb_num = pMBs->mb_D;
                p->mbs_left_chroma[0][0].mb_num =
                p->mbs_left_chroma[0][1].mb_num =
                p->mbs_left_chroma[1][0].mb_num =
                p->mbs_left_chroma[1][1].mb_num = pMBs->mb_A;
                p->mb_above_chroma[0].mb_num =
                p->mb_above_chroma[1].mb_num = pMBs->mb_B;

                if (0 == p->m_bInited)
                {
                    p->mbs_left[0].block_num = 3;
                    p->mbs_left[1].block_num = 7;
                    p->mbs_left[2].block_num = 11;
                    p->mbs_left[3].block_num = 15;
                    p->mb_above.block_num = 12;
                    p->mb_above_right.block_num = 12;
                    p->mb_above_left.block_num = 15;
                    p->mbs_left_chroma[0][0].block_num = 17;
                    p->mbs_left_chroma[0][1].block_num = 19;
                    p->mbs_left_chroma[1][0].block_num = 21;
                    p->mbs_left_chroma[1][1].block_num = 23;
                    p->mb_above_chroma[0].block_num = 18;
                    p->mb_above_chroma[1].block_num = 22;

                    // set the init flag
                    p->m_bInited = 1;
                }
            }
            else
            {
                sd->UpdateNeighbouringBlocksBMEH();
            }
            break;
        }
    }

    static inline Ipp32u GetChromaAC()
    {
        switch(color_format)
        {
        case 3:
            return D_CBP_CHROMA_AC_444;
        case 2:
            return D_CBP_CHROMA_AC_422;
        default:
            return D_CBP_CHROMA_AC_420;
        }
    }

    static inline IppStatus ReconstructChromaIntra4x4MB_Swit(Coeffs **ppSrcDstCoeff,
                                                             PlaneUV *pSrcDstUPlane,
                                                             PlaneUV *pSrcDstVPlane,
                                                             Ipp32u srcdstUVStep,
                                                             IppIntraChromaPredMode_H264 intra_chroma_mode,
                                                             Ipp32u cbpU,
                                                             Ipp32u cbpV,
                                                             Ipp32u chromaQPU,
                                                             Ipp32u chromaQPV,
                                                             Ipp32u levelScaleDCU,
                                                             Ipp32u levelScaleDCV,
                                                             Ipp8u  edge_type,
                                                             Ipp16s *pQuantTableU,
                                                             Ipp16s *pQuantTableV,
                                                             Ipp8u  bypass_flag,
                                                             Ipp32s bit_depth = 8)
    {
        switch(color_format)
        {
        case 3:
            return ReconstructChromaIntra4x4MB444(ppSrcDstCoeff,
                                                  pSrcDstUPlane,
                                                  pSrcDstVPlane,
                                                  srcdstUVStep,
                                                  intra_chroma_mode,
                                                  cbpU,
                                                  cbpV,
                                                  chromaQPU,
                                                  chromaQPV,
                                                  edge_type,
                                                  pQuantTableU,
                                                  pQuantTableV,
                                                  bypass_flag,
                                                  bit_depth);
        case 2:
            return ReconstructChromaIntra4x4MB422(ppSrcDstCoeff,
                                                  pSrcDstUPlane,
                                                  pSrcDstVPlane,
                                                  srcdstUVStep,
                                                  intra_chroma_mode,
                                                  cbpU,
                                                  cbpV,
                                                  chromaQPU,
                                                  chromaQPV,
                                                  levelScaleDCU,
                                                  levelScaleDCV,
                                                  edge_type,
                                                  pQuantTableU,
                                                  pQuantTableV,
                                                  bypass_flag,
                                                  bit_depth);
        default:
            return ReconstructChromaIntra4x4MB(ppSrcDstCoeff,
                                               pSrcDstUPlane,
                                               pSrcDstVPlane,
                                               srcdstUVStep,
                                               intra_chroma_mode,
                                               cbpU,
                                               cbpV,
                                               chromaQPU,
                                               chromaQPV,
                                               edge_type,
                                               pQuantTableU,
                                               pQuantTableV,
                                               bypass_flag,
                                               bit_depth);
        }
    }

    static inline IppStatus ReconstructChromaInter4x4MB_Swit(Coeffs **ppSrcDstCoeff,
                                                             PlaneUV *pSrcDstUPlane,
                                                             PlaneUV *pSrcDstVPlane,
                                                             Ipp32u srcdstUVStep,
                                                             Ipp32u cbpU,
                                                             Ipp32u cbpV,
                                                             Ipp32u chromaQPU,
                                                             Ipp32u chromaQPV,
                                                             Ipp32u levelScaleDCU,
                                                             Ipp32u levelScaleDCV,
                                                             Ipp16s *pQuantTableU,
                                                             Ipp16s *pQuantTableV,
                                                             Ipp8u  bypass_flag,
                                                             Ipp32s bit_depth = 8)
    {
        switch(color_format)
        {
        case 3:
            return ReconstructChromaInter4x4MB444(ppSrcDstCoeff,
                                                  pSrcDstUPlane,
                                                  pSrcDstVPlane,
                                                  srcdstUVStep,
                                                  cbpU,
                                                  cbpV,
                                                  chromaQPU,
                                                  chromaQPV,
                                                  pQuantTableU,
                                                  pQuantTableV,
                                                  bypass_flag,
                                                  bit_depth);
        case 2:
            return ReconstructChromaInter4x4MB422(ppSrcDstCoeff,
                                                  pSrcDstUPlane,
                                                  pSrcDstVPlane,
                                                  srcdstUVStep,
                                                  cbpU,
                                                  cbpV,
                                                  chromaQPU,
                                                  chromaQPV,
                                                  levelScaleDCU,
                                                  levelScaleDCV,
                                                  pQuantTableU,
                                                  pQuantTableV,
                                                  bypass_flag,
                                                  bit_depth);
        default:
            return ReconstructChromaInter4x4MB(ppSrcDstCoeff,
                                               pSrcDstUPlane,
                                               pSrcDstVPlane,
                                               srcdstUVStep,
                                               cbpU,
                                               cbpV,
                                               chromaQPU,
                                               chromaQPV,
                                               pQuantTableU,
                                               pQuantTableV,
                                               bypass_flag,
                                               bit_depth);
        }
    }
};

class SegmentDecoderHPBase
{
public:

    virtual ~SegmentDecoderHPBase() {}

    virtual Status DecodeSegmentCAVLC(Ipp32u curMB, Ipp32u nMaxMBNumber,
        H264SegmentDecoderMultiThreaded * sd) = 0;

    virtual Status DecodeSegmentCAVLC_Single(Ipp32s curMB, Ipp32s nMacroBlocksToDecode,
        H264SegmentDecoderMultiThreaded * sd) = 0;

    virtual Status DecodeSegmentCABAC(Ipp32u curMB, Ipp32u nMaxMBNumber, H264SegmentDecoderMultiThreaded * sd) = 0;

    virtual Status DecodeSegmentCABAC_Single(Ipp32s curMB, Ipp32s nMacroBlocksToDecode,
        H264SegmentDecoderMultiThreaded * sd) = 0;

    virtual Status ReconstructSegment(Ipp32u curMB,Ipp32u nMaxMBNumber,
        H264SegmentDecoderMultiThreaded * sd) = 0;

    virtual void RestoreErrorRect(Ipp32s startMb, Ipp32s endMb, H264DecoderFrame *pRefFrame,
        H264SegmentDecoderMultiThreaded * sd) = 0;
};

template <typename Coeffs, typename PlaneY, typename PlaneUV, Ipp32s color_format, Ipp32s is_field, bool is_high_profile>
class MBDecoder :
    public ResidualDecoderCABAC<Coeffs, color_format, is_field>,
    public ResidualDecoderCAVLC<Coeffs, color_format, is_field>,
    public ResidualDecoderPCM<Coeffs, PlaneY, PlaneUV, color_format, is_field>
{
public:

    void DecodeMacroblock_ISlice_CABAC(H264SegmentDecoderMultiThreaded *sd)
    {
        // reset macroblock info
        ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.LocalMacroblockInfo, sizeof(H264DecoderMacroblockLocalInfo));
        sd->m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s) sd->m_QuantPrev;
        sd->m_cur_mb.GlobalMacroblockInfo->slice_id = (Ipp16s) sd->m_iSliceNumber;

        // decode macroblock field flag
        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
            {
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockPairInfo->mbflags) = 0;
                sd->DecodeMBFieldDecodingFlag_CABAC();
            }
        }
        else
        {
            *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
            pSetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo, 0);
        }

        // update neighbouring addresses
        sd->UpdateNeighbouringAddresses();
        // update neighbouring block positions
        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types.
        sd->DecodeMBTypeISlice_CABAC();

        // decode macroblock having I type
        if (MBTYPE_PCM != sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
            DecodeMacroblock_I_CABAC(sd);
        // macroblock has PCM type
        else
            DecodeMacroblock_PCM(sd);

    } // void DecodeMacroblock_ISlice_CABAC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_PSlice_CABAC(H264SegmentDecoderMultiThreaded *sd)
    {
        Ipp32s iSkip;

        // reset macroblock info
        ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.LocalMacroblockInfo->sbdir, sizeof(sd->m_cur_mb.LocalMacroblockInfo->sbdir));
        sd->m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s) sd->m_QuantPrev;
        sd->m_cur_mb.GlobalMacroblockInfo->slice_id = (Ipp16s) sd->m_iSliceNumber;
        ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[1], sizeof(H264DecoderMacroblockRefIdxs));

        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
            {
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockPairInfo->mbflags) = 0;
            }
        }
        else
        {
            *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
        }

        // decode skip flag
        // sometimes we have decoded the flag,
        // when we were doing decoding of the previous macroblock
        if ((!sd->m_isMBAFF) ||
            (0 == (sd->m_CurMBAddr & 1)) ||
            (!pGetMBSkippedFlag(sd->m_cur_mb.GlobalMacroblockPairInfo)))
            iSkip = sd->DecodeMBSkipFlag_CABAC(MB_SKIP_FLAG_P_SP);
        else
            iSkip = sd->m_iSkipNextMacroblock;

        if (iSkip)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp = 0;

            // reset macroblock variables
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.RefIdxs[0], sizeof(H264DecoderMacroblockRefIdxs));
            sd->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_SKIPPED;
            pSetMBSkippedFlag(sd->m_cur_mb.GlobalMacroblockInfo);
            sd->m_prev_dquant = 0;

            sd->m_iSkipNextMacroblock = 0;
            if (sd->m_isMBAFF)
            {
                // but we don't know the spatial structure of the current macroblock.
                // we need to decode next macroblock for obtaining the structure.
                if (0 == (sd->m_CurMBAddr & 1))
                {
                    sd->m_CurMBAddr += 1;
                    sd->m_CurMB_Y += 1;
                    // the next macroblock isn't skipped, obtain the structure
                    if (0 == sd->DecodeMBSkipFlag_CABAC(MB_SKIP_FLAG_P_SP))
                        sd->DecodeMBFieldDecodingFlag_CABAC();
                    // the next macroblock is skipped too
                    else
                        sd->m_iSkipNextMacroblock = 1;
                    sd->m_CurMBAddr -= 1;
                    sd->m_CurMB_Y -= 1;
                }
            }

            sd->UpdateNeighbouringAddresses();
            ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

            sd->ReconstructSkipMotionVectors();

            return;
        }
        else
            sd->m_iSkipNextMacroblock = 0;

        // decode macroblock field flag
        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
                sd->DecodeMBFieldDecodingFlag_CABAC();
        }
        else
            pSetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo, 0);

        // update neighbouring addresses
        sd->UpdateNeighbouringAddresses();
        // update neighbouring block positions
        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types.
        sd->DecodeMBTypePSlice_CABAC();

        // decode macroblock having P type
        if (MBTYPE_PCM < sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
        {
            DecodeMacroblock_P_CABAC(sd);
        }
        else
        {
            ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[0], sizeof(H264DecoderMacroblockRefIdxs));
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.MVs[0], sizeof(H264DecoderMacroblockMVs));

            // decode macroblock having I type
            if (MBTYPE_PCM > sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
                DecodeMacroblock_I_CABAC(sd);
            // macroblock has PCM type
            else
                DecodeMacroblock_PCM(sd);
        }

    } // void DecodeMacroblock_PSlice_CABAC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_BSlice_CABAC(H264SegmentDecoderMultiThreaded * sd)
    {
        Ipp32s iSkip;

        // reset macroblock info
        ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.LocalMacroblockInfo->sbdir, sizeof(sd->m_cur_mb.LocalMacroblockInfo->sbdir));
        sd->m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s) sd->m_QuantPrev;
        sd->m_cur_mb.GlobalMacroblockInfo->slice_id = (Ipp16s) sd->m_iSliceNumber;

        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
            {
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockPairInfo->mbflags) = 0;
            }
        }
        else
        {
            *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
        }

        // decode skip flag
        // sometimes we have decoded the flag,
        // when we were doing decoding of the previous macroblock
        if ((!sd->m_isMBAFF) ||
            (0 == (sd->m_CurMBAddr & 1)) ||
            (!pGetMBSkippedFlag(sd->m_cur_mb.GlobalMacroblockPairInfo)))
            iSkip = sd->DecodeMBSkipFlag_CABAC(MB_SKIP_FLAG_B);
        else
            iSkip = sd->m_iSkipNextMacroblock;

        if (iSkip)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp = 0;

            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.GlobalMacroblockInfo->sbtype, sizeof(sd->m_cur_mb.GlobalMacroblockInfo->sbtype));

            // reset macroblock variables
            sd->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_SKIPPED;
            pSetMBSkippedFlag(sd->m_cur_mb.GlobalMacroblockInfo);
            sd->m_prev_dquant = 0;

            sd->m_iSkipNextMacroblock = 0;
            if (sd->m_isMBAFF)
            {
                // but we don't know the spatial structure of the current macroblock.
                // we need to decode next macroblock for obtaining the structure.
                if (0 == (sd->m_CurMBAddr & 1))
                {
                    sd->m_CurMBAddr += 1;
                    sd->m_CurMB_Y += 1;
                    // the next macroblock isn't skipped, obtain the structure
                    if (0 == sd->DecodeMBSkipFlag_CABAC(MB_SKIP_FLAG_B))
                        sd->DecodeMBFieldDecodingFlag_CABAC();
                    // the next macroblock is skipped too
                    else
                        sd->m_iSkipNextMacroblock = 1;
                    sd->m_CurMBAddr -= 1;
                    sd->m_CurMB_Y -= 1;

                    sd->UpdateNeighbouringAddresses();
                }
            }
            else
            {
                if (sd->m_IsUseSpatialDirectMode)
                {
                    sd->UpdateNeighbouringAddresses();
                }
            }

            if (sd->m_IsUseSpatialDirectMode)
            {
                ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);
            }

            sd->DecodeDirectMotionVectors(true);
            return;
        }
        else
            sd->m_iSkipNextMacroblock = 0;

        // decode macroblock field flag
        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
                sd->DecodeMBFieldDecodingFlag_CABAC();
        }
        else
            pSetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo, 0);

        sd->UpdateNeighbouringAddresses();
        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types.
        sd->DecodeMBTypeBSlice_CABAC();

        // decode macroblock having P type
        if (MBTYPE_PCM < sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
        {
            DecodeMacroblock_B_CABAC(sd);
        }
        else
        {
            ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[0], sizeof(H264DecoderMacroblockRefIdxs));
            ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[1], sizeof(H264DecoderMacroblockRefIdxs));
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.MVs[0], sizeof(H264DecoderMacroblockMVs));
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.MVs[1], sizeof(H264DecoderMacroblockMVs));

            // decode macroblock having I type
            if (MBTYPE_PCM > sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
                DecodeMacroblock_I_CABAC(sd);
            // macroblock has PCM type
            else
                DecodeMacroblock_PCM(sd);
        }

    } // void DecodeMacroblock_BSlice_CABAC(H264SegmentDecoderMultiThreaded * sd)

    void DecodeMacroblock_ISlice_CAVLC(H264SegmentDecoderMultiThreaded *sd)
    {
        sd->m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s) sd->m_QuantPrev;
        sd->m_cur_mb.GlobalMacroblockInfo->slice_id = (Ipp16s) sd->m_iSliceNumber;

        // decode macroblock field flag
        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
            {
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockPairInfo->mbflags) = 0;
                sd->DecodeMBFieldDecodingFlag_CAVLC();
            }
        }
        else
        {
            *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
        }

        // update neighbouring addresses
        sd->UpdateNeighbouringAddresses();
        // update neighbouring block positions
        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types.
        sd->DecodeMBTypeISlice_CAVLC();

        // decode macroblock having I type
        if (MBTYPE_PCM != sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
            DecodeMacroblock_I_CAVLC(sd);
        // macroblock has PCM type
        else
            DecodeMacroblock_PCM(sd);
    } // void DecodeMacroblock_ISlice_CAVLC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_PSlice_CAVLC(H264SegmentDecoderMultiThreaded *sd)
    {
        sd->m_cur_mb.GlobalMacroblockInfo->slice_id = (Ipp16s) sd->m_iSliceNumber;
        sd->m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s) sd->m_QuantPrev;
        ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.LocalMacroblockInfo->sbdir, sizeof(sd->m_cur_mb.LocalMacroblockInfo->sbdir));
        ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[1], sizeof(H264DecoderMacroblockRefIdxs));

        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
            {
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockPairInfo->mbflags) = 0;
            }
        }
        else
        {
            *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
        }

        // reset macroblock info
        if (0 == sd->m_MBSkipCount)
            sd->m_MBSkipCount = sd->DecodeMBSkipRun_CAVLC();
        else
            sd->m_MBSkipCount -= 1;

        if (0 < sd->m_MBSkipCount)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;

            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;
            ippsSet_8u(0, (Ipp8u*)pNumCoeffsArray, sizeof(H264DecoderMacroblockCoeffsInfo));

            // reset macroblock variables
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.RefIdxs[0], sizeof(H264DecoderMacroblockRefIdxs));
            sd->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_SKIPPED;
            pSetMBSkippedFlag(sd->m_cur_mb.GlobalMacroblockInfo);
            sd->m_prev_dquant = 0;

            if ((sd->m_isMBAFF) &&
                (0 == (sd->m_CurMBAddr & 1)))
            {
                // but we don't know the spatial structure of the current macroblock.
                // we need to decode next macroblock for obtaining the structure.
                if (1 == sd->m_MBSkipCount)
                {
                    sd->m_CurMBAddr += 1;
                    sd->m_CurMB_Y += 1;
                    // the next macroblock isn't skipped, obtain the structure
                    sd->DecodeMBFieldDecodingFlag_CAVLC();
                    // the next macroblock is skipped too
                    sd->m_CurMBAddr -= 1;
                    sd->m_CurMB_Y -= 1;
                }
                else
                    sd->DecodeMBFieldDecodingFlag();
            }

            sd->UpdateNeighbouringAddresses();
            ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

            sd->ReconstructSkipMotionVectors();

            return;
        }

        // decode macroblock field flag
        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
                sd->DecodeMBFieldDecodingFlag_CAVLC();
        }
        else
            pSetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo, 0);

        sd->UpdateNeighbouringAddresses();
        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types.
        sd->DecodeMBTypePSlice_CAVLC();

        // decode macroblock having P type
        if (MBTYPE_PCM < sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
        {
            DecodeMacroblock_P_CAVLC(sd);
        }
        else
        {
            ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[0], sizeof(H264DecoderMacroblockRefIdxs));
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.MVs[0], sizeof(H264DecoderMacroblockMVs));

            // decode macroblock having I type
            if (MBTYPE_PCM > sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
                DecodeMacroblock_I_CAVLC(sd);
            // macroblock has PCM type
            else
                DecodeMacroblock_PCM(sd);
        }

    } // void DecodeMacroblock_PSlice_CAVLC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_BSlice_CAVLC(H264SegmentDecoderMultiThreaded *sd)
    {
        // reset macroblock info
        ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.LocalMacroblockInfo->sbdir, sizeof(sd->m_cur_mb.LocalMacroblockInfo->sbdir));
        sd->m_cur_mb.LocalMacroblockInfo->QP = (Ipp8s) sd->m_QuantPrev;
        sd->m_cur_mb.GlobalMacroblockInfo->slice_id = (Ipp16s) sd->m_iSliceNumber;

        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
            {
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
                *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockPairInfo->mbflags) = 0;
            }
        }
        else
        {
            *((Ipp8u*)&sd->m_cur_mb.GlobalMacroblockInfo->mbflags) = 0;
        }

        if (0 == sd->m_MBSkipCount)
            sd->m_MBSkipCount = sd->DecodeMBSkipRun_CAVLC();
        else
            sd->m_MBSkipCount -= 1;

        if (0 < sd->m_MBSkipCount)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;

            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.GlobalMacroblockInfo->sbtype, sizeof(sd->m_cur_mb.GlobalMacroblockInfo->sbtype));

            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;
            ippsSet_8u(0, (Ipp8u*)pNumCoeffsArray, sizeof(H264DecoderMacroblockCoeffsInfo));

            // reset macroblock variables
            sd->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_SKIPPED;
            pSetMBSkippedFlag(sd->m_cur_mb.GlobalMacroblockInfo);
            sd->m_prev_dquant = 0;

            if ((sd->m_isMBAFF) &&
                (0 == (sd->m_CurMBAddr & 1)))
            {
                // but we don't know the spatial structure of the current macroblock.
                // we need to decode next macroblock for obtaining the structure.
                if (1 == sd->m_MBSkipCount)
                {
                    sd->m_CurMBAddr += 1;
                    sd->m_CurMB_Y += 1;
                    // the next macroblock isn't skipped, obtain the structure
                    sd->DecodeMBFieldDecodingFlag_CAVLC();
                    // the next macroblock is skipped too
                    sd->m_CurMBAddr -= 1;
                    sd->m_CurMB_Y -= 1;
                }
                else
                    sd->DecodeMBFieldDecodingFlag();

                sd->UpdateNeighbouringAddresses();
            }
            else
            {
                if (sd->m_IsUseSpatialDirectMode)
                {
                    sd->UpdateNeighbouringAddresses();
                }
            }

            if (sd->m_IsUseSpatialDirectMode)
            {
                ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);
            }

            sd->DecodeDirectMotionVectors(true);
            return;
        }

        // decode macroblock field flag
        if (sd->m_isMBAFF)
        {
            if (0 == (sd->m_CurMBAddr & 1))
                sd->DecodeMBFieldDecodingFlag_CAVLC();
        }
        else
            pSetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo, 0);

        sd->UpdateNeighbouringAddresses();
        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::UpdateNeighbouringBlocks(sd);

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types.
        sd->DecodeMBTypeBSlice_CAVLC();

        // decode macroblock having P type
        if (MBTYPE_PCM < sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
        {
            DecodeMacroblock_B_CAVLC(sd);
        }
        else
        {
            ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[0], sizeof(H264DecoderMacroblockRefIdxs));
            ippsSet_8u(0xFF, (Ipp8u*)sd->m_cur_mb.RefIdxs[1], sizeof(H264DecoderMacroblockRefIdxs));
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.MVs[0], sizeof(H264DecoderMacroblockMVs));
            ippsSet_8u(0, (Ipp8u*)sd->m_cur_mb.MVs[1], sizeof(H264DecoderMacroblockMVs));

            // decode macroblock having I type
            if (MBTYPE_PCM > sd->m_cur_mb.GlobalMacroblockInfo->mbtype)
                DecodeMacroblock_I_CAVLC(sd);
            // macroblock has PCM type
            else
                DecodeMacroblock_PCM(sd);
        }

    } // void DecodeMacroblock_BSlice_CAVLC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_I_CABAC(H264SegmentDecoderMultiThreaded *sd)
    {
        bool noSubMbPartSizeLessThan8x8Flag;

        // Reset buffer pointers to start
        // This works only as long as "batch size" for VLD and reconstruct
        // is the same. When/if want to make them different, need to change this.
        IntraType *pMBIntraTypes = sd->m_pMBIntraTypes + sd->m_CurMBAddr*NUM_INTRA_TYPE_ELEMENTS;

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types, motion vectors and CBP.

        // Get MB type, possibly change MBSKipCount to non-zero
        if (is_high_profile)
            noSubMbPartSizeLessThan8x8Flag = false;

        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (mbtype == MBTYPE_INTRA)
        {
            Ipp8u transform_size_8x8_mode_flag = 0;

            if ((is_high_profile) &&
                (sd->m_pPicParamSet->transform_8x8_mode_flag))
            {
                Ipp32s left_inc = sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num>=0?
                    GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num]):0;
                Ipp32s top_inc = sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num>=0?
                    GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num]):0;
                Ipp32u ctxIdxInc = top_inc + left_inc;
                transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[TRANSFORM_SIZE_8X8_FLAG] + ctxIdxInc);
                pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo,transform_size_8x8_mode_flag);
            }

            if (transform_size_8x8_mode_flag)
                sd->DecodeIntraTypes8x8_CABAC(pMBIntraTypes, sd->m_IsUseConstrainedIntra);
            else
                sd->DecodeIntraTypes4x4_CABAC(pMBIntraTypes, sd->m_IsUseConstrainedIntra);
        }

        // decode chroma intra prediction mode
        if (color_format)
            sd->DecodeIntraPredChromaMode_CABAC();

        sd->DecodeEdgeType();

        // decode CBP
        if (mbtype != MBTYPE_INTRA_16x16)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp = (Ipp8u) sd->DecodeCBP_CABAC(color_format);

            if (0 == sd->m_cur_mb.LocalMacroblockInfo->cbp)
            {
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
                sd->m_prev_dquant = 0;
            }
        }

        // decode delta QP
        if ((sd->m_cur_mb.LocalMacroblockInfo->cbp) ||
            (mbtype == MBTYPE_INTRA_16x16))
        {
            if (is_high_profile && noSubMbPartSizeLessThan8x8Flag && (sd->m_cur_mb.LocalMacroblockInfo->cbp&15))
            {
                Ipp8u transform_size_8x8_mode_flag = 0;
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    Ipp32u ctxIdxInc;
                    Ipp32s left_inc = sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num>=0?
                        GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num]):0;
                    Ipp32s top_inc = sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num>=0?
                        GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num]):0;
                    ctxIdxInc = top_inc+left_inc;
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[TRANSFORM_SIZE_8X8_FLAG] + ctxIdxInc);
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo,transform_size_8x8_mode_flag);
                }
            }

            // decode delta for quant value
            {
                sd->DecodeMBQPDelta_CABAC();
                sd->m_QuantPrev = sd->m_cur_mb.LocalMacroblockInfo->QP;
            }

            // Now, decode the coefficients
            if (MBTYPE_INTRA_16x16 != mbtype)
            {
                if (is_high_profile && pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                    this->DecodeCoefficients8x8_CABAC(sd);
                else
                    this->DecodeCoefficients4x4_CABAC(sd);
            }
            else
                this->DecodeCoefficients16x16_CABAC(sd);
        }
    } // void DecodeMacroblock_I_CABAC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_P_CABAC(H264SegmentDecoderMultiThreaded *sd)
    {
        bool noSubMbPartSizeLessThan8x8Flag;

        if (is_high_profile)
            noSubMbPartSizeLessThan8x8Flag = false;

        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (is_high_profile)
        {
            if (mbtype==MBTYPE_INTER_8x8 || mbtype==MBTYPE_INTER_8x8_REF0)
            {
                Ipp32s sum_partnum =
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[0]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[1]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[2]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[3]];

                if (sum_partnum == 0)
                    noSubMbPartSizeLessThan8x8Flag = true;
            }
            else
                noSubMbPartSizeLessThan8x8Flag = true;
        }

        // Motion Vector Computation
        sd->DecodeMotionVectors_CABAC();

        // cbp
        sd->m_cur_mb.LocalMacroblockInfo->cbp = (Ipp8u) sd->DecodeCBP_CABAC(color_format);

        if (0 == sd->m_cur_mb.LocalMacroblockInfo->cbp)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
            sd->m_prev_dquant = 0;
        }
        else
        { // delta QP
            if (is_high_profile && noSubMbPartSizeLessThan8x8Flag && (sd->m_cur_mb.LocalMacroblockInfo->cbp&15))
            {
                Ipp8u transform_size_8x8_mode_flag = 0;
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    Ipp32u ctxIdxInc;
                    Ipp32s left_inc = sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num>=0?
                        GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num]):0;
                    Ipp32s top_inc = sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num>=0?
                        GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num]):0;
                    ctxIdxInc = top_inc+left_inc;
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[TRANSFORM_SIZE_8X8_FLAG] + ctxIdxInc);
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo,transform_size_8x8_mode_flag);
                }
            }

            // decode delta for quant value
            sd->DecodeMBQPDelta_CABAC();
            sd->m_QuantPrev = sd->m_cur_mb.LocalMacroblockInfo->QP;

            // Now, decode the coefficients
            if (is_high_profile && pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                this->DecodeCoefficients8x8_CABAC(sd);
            else
                this->DecodeCoefficients4x4_CABAC(sd);
        }
    } // void DecodeMacroblock_P_CABAC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_B_CABAC(H264SegmentDecoderMultiThreaded *sd)
    {
        bool noSubMbPartSizeLessThan8x8Flag;

        if (is_high_profile)
            noSubMbPartSizeLessThan8x8Flag = false;

        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (is_high_profile)
        {
            if ((MBTYPE_INTER_8x8 == mbtype) || (MBTYPE_DIRECT == mbtype))
            {
                Ipp32s sum_partnum =
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[0]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[1]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[2]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[3]];

                if (sum_partnum==0)
                    noSubMbPartSizeLessThan8x8Flag = true;
            }
            else
                noSubMbPartSizeLessThan8x8Flag = true;
        }

        if (mbtype != MBTYPE_DIRECT)
        {
            // Motion Vector Computation
            if (mbtype == MBTYPE_INTER_8x8)
            {
                // First, if B slice and MB is 8x8, set the MV for any DIRECT
                // 8x8 partitions. The MV for the 8x8 DIRECT partition need to
                // be properly set before the MV for subsequent 8x8 partitions
                // can be computed, due to prediction. The DIRECT MV are computed
                // by a separate function and do not depend upon block neighbors for
                // predictors, so it is done here first.
                if (sd->m_cur_mb.GlobalMacroblockInfo->sbtype[0] == SBTYPE_DIRECT ||
                    sd->m_cur_mb.GlobalMacroblockInfo->sbtype[1] == SBTYPE_DIRECT ||
                    sd->m_cur_mb.GlobalMacroblockInfo->sbtype[2] == SBTYPE_DIRECT ||
                    sd->m_cur_mb.GlobalMacroblockInfo->sbtype[3] == SBTYPE_DIRECT)
                {
                    sd->DecodeDirectMotionVectors(false);
                }
            }

            sd->DecodeMotionVectors_CABAC();
        }
        else
        {
            sd->DecodeDirectMotionVectors(true);
        }

        // cbp
        sd->m_cur_mb.LocalMacroblockInfo->cbp = (Ipp8u)sd->DecodeCBP_CABAC(color_format);

        if (0 == sd->m_cur_mb.LocalMacroblockInfo->cbp)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
            sd->m_prev_dquant = 0;
        }
        else
        { // // delta QP

            if (is_high_profile && noSubMbPartSizeLessThan8x8Flag && (sd->m_cur_mb.LocalMacroblockInfo->cbp&15))
            {
                Ipp8u transform_size_8x8_mode_flag = 0;
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    Ipp32u ctxIdxInc;
                    Ipp32s left_inc = sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num>=0?
                        GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mbs_left[0].mb_num]):0;
                    Ipp32s top_inc = sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num>=0?
                        GetMB8x8TSFlag(sd->m_gmbinfo->mbs[sd->m_cur_mb.CurrentBlockNeighbours.mb_above.mb_num]):0;
                    ctxIdxInc = top_inc+left_inc;
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->DecodeSingleBin_CABAC(ctxIdxOffset[TRANSFORM_SIZE_8X8_FLAG] + ctxIdxInc);
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo,transform_size_8x8_mode_flag);
                }
            }

            // decode delta for quant value
            {
                sd->DecodeMBQPDelta_CABAC();
                sd->m_QuantPrev = sd->m_cur_mb.LocalMacroblockInfo->QP;
            }

            // Now, decode the coefficients
            if (is_high_profile && pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                this->DecodeCoefficients8x8_CABAC(sd);
            else
                this->DecodeCoefficients4x4_CABAC(sd);
        }
    } // void DecodeMacroblock_B_CABAC(H264SegmentDecoderMultiThreaded *sd)


    void DecodeMacroblock_I_CAVLC(H264SegmentDecoderMultiThreaded *sd)
    {
        bool noSubMbPartSizeLessThan8x8Flag;

        // Reset buffer pointers to start
        // This works only as long as "batch size" for VLD and reconstruct
        // is th same. When/if want to make them different, need to change this.
        IntraType * pMBIntraTypes = sd->m_pMBIntraTypes + sd->m_CurMBAddr*NUM_INTRA_TYPE_ELEMENTS;

        // First decode the "macroblock header", e.g., macroblock type,
        // intra-coding types, motion vectors and CBP.

        // Get MB type, possibly change MBSKipCount to non-zero
        if (is_high_profile)
            noSubMbPartSizeLessThan8x8Flag = false;

        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (mbtype == MBTYPE_INTRA)
        {
            Ipp8u transform_size_8x8_mode_flag = 0;
            if (is_high_profile)
            {
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->Get1Bit();
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo,transform_size_8x8_mode_flag);
                }
            }

            if (transform_size_8x8_mode_flag)
                sd->DecodeIntraTypes8x8_CAVLC(pMBIntraTypes, sd->m_IsUseConstrainedIntra);
            else
                sd->DecodeIntraTypes4x4_CAVLC(pMBIntraTypes, sd->m_IsUseConstrainedIntra);
        }

        if (color_format)
        {
            // Get chroma prediction mode
            sd->m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode = (Ipp8u) sd->m_pBitStream->GetVLCElement(false);
            if (sd->m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode > 3)
                throw h264_exception(UMC_ERR_INVALID_STREAM);
        }

        sd->DecodeEdgeType();

        // cbp
        if (mbtype != MBTYPE_INTRA_16x16)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp = (Ipp8u) sd->DecodeCBP_CAVLC(color_format);

            if (0 == sd->m_cur_mb.LocalMacroblockInfo->cbp)
            {
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
                sd->m_prev_dquant = 0;
            }
        }

        if ((sd->m_cur_mb.LocalMacroblockInfo->cbp) ||
            (mbtype == MBTYPE_INTRA_16x16))
        {
            if (is_high_profile && noSubMbPartSizeLessThan8x8Flag && (sd->m_cur_mb.LocalMacroblockInfo->cbp&15))
            {
                Ipp8u transform_size_8x8_mode_flag = 0;
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->Get1Bit();
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo, transform_size_8x8_mode_flag);
                }
            }

            // decode delta for quant value
            if (!sd->m_pBitStream->NextBit())
            {
                sd->DecodeMBQPDelta_CAVLC();
                sd->m_QuantPrev = sd->m_cur_mb.LocalMacroblockInfo->QP;
            }

            // Now, decode the coefficients
            if (MBTYPE_INTRA_16x16 != mbtype)
            {
                if (is_high_profile && pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                    this->DecodeCoefficients8x8_CAVLC(sd);
                else
                    this->DecodeCoefficients4x4_CAVLC(sd);
            }
            else
                this->DecodeCoefficients16x16_CAVLC(sd);
        }
        else
        {
            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;
            ippsSet_8u(0, pNumCoeffsArray, sizeof(H264DecoderMacroblockCoeffsInfo));
        }
    } // void DecodeMacroblock_I_CAVLC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_P_CAVLC(H264SegmentDecoderMultiThreaded *sd)
    {
        bool noSubMbPartSizeLessThan8x8Flag;

        // Get MB type, possibly change MBSKipCount to non-zero
        if (is_high_profile)
            noSubMbPartSizeLessThan8x8Flag = false;

        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (is_high_profile)
        {
            if (mbtype==MBTYPE_INTER_8x8 || mbtype==MBTYPE_INTER_8x8_REF0)
            {
                Ipp32s sum_partnum =
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[0]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[1]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[2]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[3]];

                if (sum_partnum == 0)
                    noSubMbPartSizeLessThan8x8Flag = true;
            }
            else
                noSubMbPartSizeLessThan8x8Flag = true;
        }

        // MV and Ref Index
        sd->DecodeMotionVectorsPSlice_CAVLC();
        //sd->ReconstructMotionVectors();

        // cbp
        sd->m_cur_mb.LocalMacroblockInfo->cbp = (Ipp8u) sd->DecodeCBP_CAVLC(color_format);

        if (0 == sd->m_cur_mb.LocalMacroblockInfo->cbp)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
            sd->m_prev_dquant = 0;
            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;
            ippsSet_8u(0, pNumCoeffsArray, sizeof(H264DecoderMacroblockCoeffsInfo));
        }
        else
        {
            if (is_high_profile && noSubMbPartSizeLessThan8x8Flag && (sd->m_cur_mb.LocalMacroblockInfo->cbp&15))
            {
                Ipp8u transform_size_8x8_mode_flag = 0;
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->Get1Bit();
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo, transform_size_8x8_mode_flag);
                }
            }

            // check for usual case of zero QP delta
            if (!sd->m_pBitStream->NextBit())
            {
                sd->DecodeMBQPDelta_CAVLC();
                sd->m_QuantPrev = sd->m_cur_mb.LocalMacroblockInfo->QP;
            }

            // Now, decode the coefficients
            if (is_high_profile && pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                this->DecodeCoefficients8x8_CAVLC(sd);
            else
                this->DecodeCoefficients4x4_CAVLC(sd);
        }
    } // void DecodeMacroblock_P_CAVLC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_B_CAVLC(H264SegmentDecoderMultiThreaded *sd)
    {
        bool noSubMbPartSizeLessThan8x8Flag;

        // Get MB type, possibly change MBSKipCount to non-zero
        if (is_high_profile)
            noSubMbPartSizeLessThan8x8Flag = false;

        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (is_high_profile)
        {
            if ((MBTYPE_INTER_8x8 == mbtype) ||
                (MBTYPE_DIRECT == mbtype))
            {
                Ipp32s sum_partnum =
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[0]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[1]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[2]]+
                    SbPartNumMinus1[sd->m_IsUseDirect8x8Inference][sd->m_cur_mb.GlobalMacroblockInfo->sbtype[3]];

                if (sum_partnum == 0)
                    noSubMbPartSizeLessThan8x8Flag = true;
            }
            else
                noSubMbPartSizeLessThan8x8Flag = true;
        }

        if (mbtype != MBTYPE_DIRECT)
        {
            // Motion Vector Computation

            if (mbtype == MBTYPE_INTER_8x8)
            {
                // First, if B slice and MB is 8x8, set the MV for any DIRECT
                // 8x8 partitions. The MV for the 8x8 DIRECT partition need to
                // be properly set before the MV for subsequent 8x8 partitions
                // can be computed, due to prediction. The DIRECT MV are computed
                // by a separate function and do not depend upon block neighbors for
                // predictors, so it is done here first.
                if (sd->m_cur_mb.GlobalMacroblockInfo->sbtype[0] == SBTYPE_DIRECT ||
                    sd->m_cur_mb.GlobalMacroblockInfo->sbtype[1] == SBTYPE_DIRECT ||
                    sd->m_cur_mb.GlobalMacroblockInfo->sbtype[2] == SBTYPE_DIRECT ||
                    sd->m_cur_mb.GlobalMacroblockInfo->sbtype[3] == SBTYPE_DIRECT)
                {
                    sd->DecodeDirectMotionVectors(false);
                }
            }

            // MV and Ref Index
            sd->DecodeMotionVectors_CAVLC(true);
            //sd->ReconstructMotionVectors();
        }
        else
        {
            sd->DecodeDirectMotionVectors(true);
        }

        sd->m_cur_mb.LocalMacroblockInfo->cbp = (Ipp8u) sd->DecodeCBP_CAVLC(color_format);

        if (0 == sd->m_cur_mb.LocalMacroblockInfo->cbp)
        {
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] = 0;
            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = 0;
            sd->m_prev_dquant = 0;

            Ipp8u *pNumCoeffsArray = sd->m_cur_mb.GetNumCoeffs()->numCoeffs;
            ippsSet_8u(0, pNumCoeffsArray, sizeof(H264DecoderMacroblockCoeffsInfo));
        }
        else
        {
            if (is_high_profile && noSubMbPartSizeLessThan8x8Flag && (sd->m_cur_mb.LocalMacroblockInfo->cbp&15))
            {
                Ipp8u transform_size_8x8_mode_flag = 0;
                if (sd->m_pPicParamSet->transform_8x8_mode_flag)
                {
                    transform_size_8x8_mode_flag  = (Ipp8u) sd->m_pBitStream->Get1Bit();
                    pSetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo, transform_size_8x8_mode_flag);
                }
            }

            // check for usual case of zero QP delta
            if (!sd->m_pBitStream->NextBit())
            {
                sd->DecodeMBQPDelta_CAVLC();
                sd->m_QuantPrev = sd->m_cur_mb.LocalMacroblockInfo->QP;
            }

            // Now, decode the coefficients
            if (is_high_profile && pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                this->DecodeCoefficients8x8_CAVLC(sd);
            else
                this->DecodeCoefficients4x4_CAVLC(sd);
        }
    } // void DecodeMacroblock_B_CAVLC(H264SegmentDecoderMultiThreaded *sd)

    void DecodeMacroblock_PCM(H264SegmentDecoderMultiThreaded *sd)
    {
        // Reset buffer pointers to start
        // This works only as long as "batch size" for VLD and reconstruct
        // is the same. When/if want to make them different, need to change this.

        for (;;) // not really a loop, while used to enable use of break
        {
            // First decode the "macroblock header", e.g., macroblock type,
            // intra-coding types, motion vectors and CBP.

            {
                this->DecodeCoefficients_PCM(sd);
                // For PCM type MB, num coeffs are set by above call, cbp is
                // set to all blocks coded (for deblock filter), MV are set to zero,
                // QP is unchanged.
                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma = D_CBP_LUMA_DC | D_CBP_LUMA_AC;
                if (color_format)
                {
                    sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0] =
                    sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1] = D_CBP_CHROMA_DC | ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::GetChromaAC();
                }
                sd->m_prev_dquant = 0;
            }

            break; // no more to read from bitstream
        } // while 1

    } // void DecodeMacroblock_PCM(H264SegmentDecoderMultiThreaded *sd)
};

template <typename Coeffs, typename PlaneY, typename PlaneUV, Ipp32s color_format, Ipp32s is_field, bool is_high_profile>
class MBNullDecoder //:
    /*public ResidualDecoderCABAC<Coeffs, color_format, is_field>,
    public ResidualDecoderCAVLC<Coeffs, color_format, is_field>,
    public ResidualDecoderPCM<Coeffs, PlaneY, PlaneUV, color_format, is_field>*/
{
public:
    virtual
    ~MBNullDecoder(void)
    {
    } // MBNullDecoder(void)

    void DecodeMacroblock_ISlice_CABAC(H264SegmentDecoderMultiThreaded *)
    {
    } // void DecodeMacroblock_ISlice_CABAC(

    void DecodeMacroblock_PSlice_CABAC(H264SegmentDecoderMultiThreaded *)
    {
    } // void DecodeMacroblock_PSlice_CABAC(

    void DecodeMacroblock_BSlice_CABAC(H264SegmentDecoderMultiThreaded * )
    {
    } // void DecodeMacroblock_BSlice_CABAC(

    void DecodeMacroblock_ISlice_CAVLC(H264SegmentDecoderMultiThreaded *)
    {
    } // void DecodeMacroblock_ISlice_CAVLC(

    void DecodeMacroblock_PSlice_CAVLC(H264SegmentDecoderMultiThreaded *)
    {
    } // void DecodeMacroblock_PSlice_CAVLC(

    void DecodeMacroblock_BSlice_CAVLC(H264SegmentDecoderMultiThreaded *)
    {
    } // void DecodeMacroblock_BSlice_CAVLC(
};

template <typename Coeffs, typename PlaneY, typename PlaneUV, Ipp32s color_format, Ipp32s is_field, bool is_high_profile, bool nv12_support = false>
class MBReconstructor
{
public:

    enum
    {
        width_chroma_div = nv12_support ? 0 : (color_format < 3),  // for plane
        height_chroma_div = (color_format < 2),  // for plane
    };

    typedef PlaneY * PlanePtrY;
    typedef PlaneUV * PlanePtrUV;
    typedef Coeffs *  CoeffsPtr;

    virtual ~MBReconstructor() {}

    void ReconstructMacroblock_ISlice(H264SegmentDecoderMultiThreaded * sd)
    {
        // per-macroblock variables

        Ipp32s fdf = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo);
        // reconstruct Data
        IntraType *pMBIntraTypes = sd->m_pMBIntraTypes + sd->m_CurMBAddr*NUM_INTRA_TYPE_ELEMENTS;
        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        Ipp32s mbXOffset = sd->m_CurMB_X * 16;
        Ipp32s mbYOffset = sd->m_CurMB_Y * 16;
        VM_ASSERT(mbXOffset < sd->m_pCurrentFrame->lumaSize().width);
        VM_ASSERT(mbYOffset < sd->m_pCurrentFrame->lumaSize().height);

        // reconstruct starts here
        // Perform motion compensation to reconstruct the YUV data
        //
        Ipp32u offsetY = mbXOffset + (mbYOffset * sd->m_uPitchLuma);
        Ipp32u offsetC = (mbXOffset >> width_chroma_div) +  ((mbYOffset >> height_chroma_div) * sd->m_uPitchChroma);

        {
            Ipp32u rec_pitch_luma = sd->m_uPitchLuma; // !!! adjust rec_pitch to MBAFF
            Ipp32u rec_pitch_chroma = sd->m_uPitchChroma; // !!! adjust rec_pitch to MBAFF

            if (sd->m_isMBAFF)
            {
                Ipp32s currmb_fdf = fdf;
                Ipp32s currmb_bf = (sd->m_CurMBAddr & 1);

                if (currmb_fdf) //current mb coded as field MB
                {
                    if (currmb_bf)
                    {
                        offsetY -= 15 * rec_pitch_luma;
                        offsetC -= (color_format == 1 ? 7 : 15) * rec_pitch_chroma;
                    }
                    rec_pitch_luma *= 2;
                    rec_pitch_chroma *= 2;
                }
            }

            if (mbtype != MBTYPE_PCM)
            {
                Ipp32s special_MBAFF_case = 0; // !!!
                Ipp8u edge_type = 0;
                Ipp8u edge_type_2t = 0;
                Ipp8u edge_type_2b = 0;

                if (sd->m_isMBAFF)
                {
                    special_MBAFF_case = 0; // adjust to MBAFF

                    sd->ReconstructEdgeType(edge_type_2t, edge_type_2b, special_MBAFF_case);
                    edge_type = (Ipp8u) (edge_type_2t | edge_type_2b);
                }
                else
                {
                    edge_type = (Ipp8u)sd->m_mbinfo.mbs[sd->m_CurMBAddr].IntraTypes.edge_type;
                }

                Ipp32s bitdepth_luma_qp_scale = 6*(sd->bit_depth_luma - 8);
                Ipp32s lumaQP = sd->m_cur_mb.LocalMacroblockInfo->QP + bitdepth_luma_qp_scale;

                // reconstruct luma block(s)
                if (mbtype == MBTYPE_INTRA_16x16)
                {
                    if (is_high_profile)
                    {
                        ReconstructLumaIntra_16x16MB(
                            (CoeffsPtr*)(&sd->m_pCoeffBlocksRead),
                            (PlanePtrY)sd->m_pYPlane + offsetY,
                            rec_pitch_luma,
                            (IppIntra16x16PredMode_H264) pMBIntraTypes[0],
                            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma,
                            lumaQP,
                            edge_type,
                            sd->m_pPicParamSet->m_LevelScale4x4[0].LevelScaleCoeffs[lumaQP],
                            sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                            sd->bit_depth_luma);
                    }
                    else
                    {
                        ReconstructLumaIntra16x16MB(
                            (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                            (PlanePtrY)sd->m_pYPlane + offsetY,
                            rec_pitch_luma,
                            (IppIntra16x16PredMode_H264) pMBIntraTypes[0],
                            (Ipp32u) sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma,
                            lumaQP,
                            edge_type,
                            sd->bit_depth_luma);
                    }
                }
                else // if (intra16x16)
                {
                    if (is_high_profile)
                    {
                        switch (special_MBAFF_case)
                        {
                        default:
                            if (pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                            {
                                ReconstructLumaIntraHalf8x8MB(
                                    (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                    (PlanePtrY)sd->m_pYPlane + offsetY,
                                    rec_pitch_luma,
                                    (IppIntra8x8PredMode_H264 *) pMBIntraTypes,
                                    sd->m_cur_mb.LocalMacroblockInfo->cbp,
                                    lumaQP,
                                    edge_type_2t,
                                    sd->m_pPicParamSet->m_LevelScale8x8[0].LevelScaleCoeffs[lumaQP],
                                    sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag);
                                ReconstructLumaIntraHalf8x8MB(
                                    (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                    (PlanePtrY)sd->m_pYPlane + offsetY+8*rec_pitch_luma,
                                    rec_pitch_luma,
                                    (IppIntra8x8PredMode_H264 *) pMBIntraTypes+2,
                                    sd->m_cur_mb.LocalMacroblockInfo->cbp>>2,
                                    lumaQP,
                                    edge_type_2b,
                                    sd->m_pPicParamSet->m_LevelScale8x8[0].LevelScaleCoeffs[lumaQP],
                                    sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag);
                            }
                            else
                            {
                                ReconstructLumaIntraHalf4x4MB(
                                    (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                    (PlanePtrY)sd->m_pYPlane + offsetY,
                                    rec_pitch_luma,
                                    (IppIntra4x4PredMode_H264 *) pMBIntraTypes,
                                    sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma>>1,
                                    lumaQP,
                                    edge_type_2t,
                                    sd->m_pPicParamSet->m_LevelScale4x4[0].LevelScaleCoeffs[lumaQP],
                                    sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag);
                                ReconstructLumaIntraHalf4x4MB(
                                    (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                    (PlanePtrY)sd->m_pYPlane + offsetY+8*rec_pitch_luma,
                                    rec_pitch_luma,
                                    (IppIntra4x4PredMode_H264 *) pMBIntraTypes+8,
                                    sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma>>9,
                                    lumaQP,
                                    edge_type_2b,
                                    sd->m_pPicParamSet->m_LevelScale4x4[0].LevelScaleCoeffs[lumaQP],
                                    sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag);
                            }
                            break;
                        case 0:
                            if (pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                                ReconstructLumaIntra8x8MB (
                                    (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                    (PlanePtrY)sd->m_pYPlane + offsetY,
                                    rec_pitch_luma,
                                    (IppIntra8x8PredMode_H264 *) pMBIntraTypes,
                                    sd->m_cur_mb.LocalMacroblockInfo->cbp,
                                    lumaQP,
                                    edge_type,
                                    sd->m_pPicParamSet->m_LevelScale8x8[0].LevelScaleCoeffs[lumaQP],
                                    sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                                    sd->bit_depth_luma);
                            else
                                ReconstructLumaIntra4x4MB (
                                    (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                    (PlanePtrY)sd->m_pYPlane + offsetY,
                                    rec_pitch_luma,
                                    (IppIntra4x4PredMode_H264 *) pMBIntraTypes,
                                    sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma,
                                    lumaQP,
                                    edge_type,
                                    sd->m_pPicParamSet->m_LevelScale4x4[0].LevelScaleCoeffs[lumaQP],
                                    sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                                    sd->bit_depth_luma);
                            break;
                        }
                    }
                    else
                    {
                        switch (special_MBAFF_case)
                        {
                        default:
                            ReconstructLumaIntraHalfMB(
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrY)sd->m_pYPlane + offsetY,
                                rec_pitch_luma,
                                (IppIntra4x4PredMode_H264 *) pMBIntraTypes,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma>>1,
                                lumaQP,
                                edge_type_2t,
                                sd->bit_depth_luma);
                            ReconstructLumaIntraHalfMB(
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrY)sd->m_pYPlane + offsetY+8*rec_pitch_luma,
                                rec_pitch_luma,
                                (IppIntra4x4PredMode_H264 *) pMBIntraTypes+8,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma>>9,
                                lumaQP,
                                edge_type_2b,
                                sd->bit_depth_luma);
                            break;
                        case 0:
                            ReconstructLumaIntraMB(
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrY)sd->m_pYPlane + offsetY,
                                rec_pitch_luma,
                                (IppIntra4x4PredMode_H264 *) pMBIntraTypes,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma,
                                lumaQP,
                                edge_type,
                                sd->bit_depth_luma);
                            break;
                        }
                    }
                }

                if (color_format)
                {
                    Ipp32s bitdepth_chroma_qp_scale = 6*(sd->bit_depth_chroma - 8
                        + sd->m_pSeqParamSet->residual_colour_transform_flag);

                    Ipp32s QPChromaU, QPChromaV;
                    Ipp32s QPChromaIndexU, QPChromaIndexV;
                    QPChromaIndexU = sd->m_cur_mb.LocalMacroblockInfo->QP + sd->m_pPicParamSet->chroma_qp_index_offset[0];

                    QPChromaIndexU = IPP_MIN(QPChromaIndexU, (Ipp32s)QP_MAX);
                    QPChromaIndexU = IPP_MAX(-bitdepth_chroma_qp_scale, QPChromaIndexU);
                    QPChromaU = QPChromaIndexU < 0 ? QPChromaIndexU : QPtoChromaQP[QPChromaIndexU];
                    QPChromaU += bitdepth_chroma_qp_scale;

                    if (is_high_profile)
                    {
                        QPChromaIndexV = sd->m_cur_mb.LocalMacroblockInfo->QP + sd->m_pPicParamSet->chroma_qp_index_offset[1];
                        QPChromaIndexV = IPP_MIN(QPChromaIndexV, (Ipp32s)QP_MAX);
                        QPChromaIndexV = IPP_MAX(-bitdepth_chroma_qp_scale, QPChromaIndexV);
                        QPChromaV = QPChromaIndexV < 0 ? QPChromaIndexV : QPtoChromaQP[QPChromaIndexV];
                        QPChromaV += bitdepth_chroma_qp_scale;
                    }

                    // reconstruct chroma block(s)
                    if(is_high_profile)
                    {
                        switch (special_MBAFF_case)
                        {
                        default:
                            ReconstructChromaIntraHalfs4x4MB (
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrUV)sd->m_pUPlane + offsetC,
                                (PlanePtrUV)sd->m_pVPlane + offsetC,
                                rec_pitch_chroma,
                                (IppIntraChromaPredMode_H264) sd->m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0],
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1],
                                QPChromaU,
                                QPChromaV,
                                edge_type_2t,
                                edge_type_2b,
                                sd->m_pPicParamSet->m_LevelScale4x4[1].LevelScaleCoeffs[QPChromaU],
                                sd->m_pPicParamSet->m_LevelScale4x4[2].LevelScaleCoeffs[QPChromaV],
                                sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag);
                                break;
                        case 0:
                            ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::ReconstructChromaIntra4x4MB_Swit(
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrUV)sd->m_pUPlane + offsetC,
                                (PlanePtrUV)sd->m_pVPlane + offsetC,
                                rec_pitch_chroma,
                                (IppIntraChromaPredMode_H264) sd->m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0],
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1],
                                QPChromaU,
                                QPChromaV,
                                sd->m_pPicParamSet->m_LevelScale4x4[1].LevelScaleCoeffs[QPChromaU+3][0],
                                sd->m_pPicParamSet->m_LevelScale4x4[2].LevelScaleCoeffs[QPChromaV+3][0],
                                edge_type,
                                sd->m_pPicParamSet->m_LevelScale4x4[1].LevelScaleCoeffs[QPChromaU],
                                sd->m_pPicParamSet->m_LevelScale4x4[2].LevelScaleCoeffs[QPChromaV],
                                sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                                sd->bit_depth_chroma);
                                break;
                        }
                    }
                    else
                    {
                        switch (special_MBAFF_case)
                        {
                        default:
                            ReconstructChromaIntraHalfsMB (
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrUV)sd->m_pUPlane + offsetC,
                                (PlanePtrUV)sd->m_pVPlane + offsetC,
                                rec_pitch_chroma,
                                (IppIntraChromaPredMode_H264) sd->m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0],
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1],
                                QPChromaU,
                                edge_type_2t,
                                edge_type_2b,
                                sd->bit_depth_chroma);
                            break;
                        case 0:
                            ReconstructChromaIntraMB (
                                (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                (PlanePtrUV)sd->m_pUPlane + offsetC,
                                (PlanePtrUV)sd->m_pVPlane + offsetC,
                                rec_pitch_chroma,
                                (IppIntraChromaPredMode_H264) sd->m_cur_mb.LocalMacroblockInfo->IntraTypes.intra_chroma_mode,
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0],
                                sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1],
                                QPChromaU,
                                edge_type,
                                sd->bit_depth_chroma);
                            break;
                        }
                    } // if (is_high_profile)
                }
            }
            else
            {
                // reconstruct PCM block(s)
                ReconstructMB<PlaneY, PlaneUV, color_format, is_field, 1>  mb;
                mb.ReconstructPCMMB(offsetY, offsetC, rec_pitch_luma, rec_pitch_chroma, sd);
            }
        }
    } // void ReconstructMacroblock_ISlice(H264SegmentDecoderMultiThreaded * sd)

    void ReconstructMacroblock_PSlice(H264SegmentDecoderMultiThreaded * sd)
    {
        // reconstruct Data
        Ipp8u mbtype = sd->m_cur_mb.GlobalMacroblockInfo->mbtype;

        if (MBTYPE_PCM >= mbtype)
        {
            ReconstructMacroblock_ISlice(sd);
            return;
        }

        Ipp32s mbXOffset = sd->m_CurMB_X * 16;
        Ipp32s mbYOffset = sd->m_CurMB_Y * 16;

        VM_ASSERT(mbXOffset < sd->m_pCurrentFrame->lumaSize().width);
        VM_ASSERT(mbYOffset < sd->m_pCurrentFrame->lumaSize().height);

        // reconstruct starts here
        // Perform motion compensation to reconstruct the YUV data
        //
        Ipp32u offsetY = mbXOffset + (mbYOffset * sd->m_uPitchLuma);
        Ipp32u offsetC = (mbXOffset >> width_chroma_div) +  ((mbYOffset >> height_chroma_div) * sd->m_uPitchChroma);

        Ipp32s pitch_luma = sd->m_uPitchLuma;
        Ipp32s pitch_chroma = sd->m_uPitchChroma;

        {
            if (sd->m_isMBAFF)
            {
                Ipp32s fdf = pGetMBFieldDecodingFlag(sd->m_cur_mb.GlobalMacroblockInfo);

                Ipp32u offsetY_1 = offsetY;
                Ipp32u offsetC_1 = offsetC;
                bool need_adjust = (sd->m_CurMBAddr & 1) && fdf;
                if (need_adjust)
                {
                    offsetY -= 15 * sd->m_uPitchLuma;
                    offsetC -= (color_format == 1 ? 7 : 15) * sd->m_uPitchChroma;
                    mbYOffset -= 16;

                    offsetY_1 = offsetY - sd->m_uPitchLuma;
                    offsetC_1 = offsetC - sd->m_uPitchChroma;
                }

                pitch_luma <<= fdf;
                pitch_chroma <<= fdf;

                if(sd->m_pPicParamSet->weighted_bipred_idc!= 0 ||
                    sd->m_pPicParamSet->weighted_pred_flag!= 0)
                {
                    if(fdf)
                    {
                        ReconstructMB<PlaneY, PlaneUV, color_format, true, 1>  mb;
                        mb.CompensateMotionMacroBlock((PlanePtrY)sd->m_pYPlane + offsetY,
                                                        (PlanePtrUV)sd->m_pVPlane + offsetC,
                                                        (PlanePtrUV)sd->m_pUPlane + offsetC,
                                                        mbXOffset,
                                                        mbYOffset >> fdf,
                                                        offsetY_1,
                                                        offsetC_1,
                                                        pitch_luma,
                                                        pitch_chroma,
                                                        sd);
                    } else {
                        ReconstructMB<PlaneY, PlaneUV, color_format, false, 1>  mb;
                        mb.CompensateMotionMacroBlock((PlanePtrY)sd->m_pYPlane + offsetY,
                                                        (PlanePtrUV)sd->m_pVPlane + offsetC,
                                                        (PlanePtrUV)sd->m_pUPlane + offsetC,
                                                        mbXOffset,
                                                        mbYOffset >> fdf,
                                                        offsetY_1,
                                                        offsetC_1,
                                                        pitch_luma,
                                                        pitch_chroma,
                                                        sd);
                    }
                } else {
                    if(fdf)
                    {
                        ReconstructMB<PlaneY, PlaneUV, color_format, true, 0>  mb;
                        mb.CompensateMotionMacroBlock((PlanePtrY)sd->m_pYPlane + offsetY,
                                                        (PlanePtrUV)sd->m_pVPlane + offsetC,
                                                        (PlanePtrUV)sd->m_pUPlane + offsetC,
                                                        mbXOffset,
                                                        mbYOffset >> fdf,
                                                        offsetY_1,
                                                        offsetC_1,
                                                        pitch_luma,
                                                        pitch_chroma,
                                                        sd);
                    }
                    else
                    {
                        ReconstructMB<PlaneY, PlaneUV, color_format, false, 0>  mb;
                        mb.CompensateMotionMacroBlock((PlanePtrY)sd->m_pYPlane + offsetY_1,
                                                        (PlanePtrUV)sd->m_pVPlane + offsetC,
                                                        (PlanePtrUV)sd->m_pUPlane + offsetC,
                                                        mbXOffset,
                                                        mbYOffset >> fdf,
                                                        offsetY_1,
                                                        offsetC_1,
                                                        pitch_luma,
                                                        pitch_chroma,
                                                        sd);
                    }
                }
            }
            else
            {
                ReconstructMB<PlaneY, PlaneUV, color_format, is_field, 1>  mb;
                mb.CompensateMotionMacroBlock((PlanePtrY)sd->m_pYPlane + offsetY,
                                              (PlanePtrUV)sd->m_pVPlane + offsetC,
                                              (PlanePtrUV)sd->m_pUPlane + offsetC,
                                              mbXOffset,
                                              mbYOffset,
                                              offsetY,
                                              offsetC,
                                              pitch_luma,
                                              pitch_chroma,
                                              sd);
            }

            if (sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma & D_CBP_LUMA_AC)
            {
                Ipp32s bitdepth_luma_qp_scale = 6*(sd->bit_depth_luma - 8);
                Ipp32s lumaQP = sd->m_cur_mb.LocalMacroblockInfo->QP + bitdepth_luma_qp_scale;

                if (is_high_profile)
                {
                    if (pGetMB8x8TSFlag(sd->m_cur_mb.GlobalMacroblockInfo))
                    {
                        ReconstructLumaInter8x8MB(
                            (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                            (PlanePtrY)sd->m_pYPlane + offsetY,
                            pitch_luma,
                            sd->m_cur_mb.LocalMacroblockInfo->cbp,
                            lumaQP,
                            sd->m_pPicParamSet->m_LevelScale8x8[1].LevelScaleCoeffs[lumaQP],
                            sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                            sd->bit_depth_luma);
                    }
                    else
                    {
                        ReconstructLumaInter4x4MB(
                            (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                            (PlanePtrY)sd->m_pYPlane + offsetY,
                            pitch_luma,
                            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma,
                            lumaQP,
                            sd->m_pPicParamSet->m_LevelScale4x4[3].LevelScaleCoeffs[lumaQP],
                            sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                            sd->bit_depth_luma);
                    }
                }
                else
                {
                    ReconstructLumaInterMB(
                                        (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                                        (PlanePtrY)sd->m_pYPlane + offsetY,
                                        pitch_luma,
                                        sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_luma,
                                        lumaQP,
                                        sd->bit_depth_luma);
                }
            }

            if (color_format && (sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0]
                || sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1]))
            {
                Ipp32s bitdepth_chroma_qp_scale = 6*(sd->bit_depth_chroma - 8
                    + sd->m_pSeqParamSet->residual_colour_transform_flag);

                Ipp32s QPChromaU, QPChromaV;
                Ipp32s QPChromaIndexU, QPChromaIndexV;
                QPChromaIndexU = sd->m_cur_mb.LocalMacroblockInfo->QP + sd->m_pPicParamSet->chroma_qp_index_offset[0];

                QPChromaIndexU = IPP_MIN(QPChromaIndexU, (Ipp32s)QP_MAX);
                QPChromaIndexU = IPP_MAX(-bitdepth_chroma_qp_scale, QPChromaIndexU);
                QPChromaU = QPChromaIndexU < 0 ? QPChromaIndexU : QPtoChromaQP[QPChromaIndexU];
                QPChromaU += bitdepth_chroma_qp_scale;

                if (is_high_profile)
                {
                    QPChromaIndexV = sd->m_cur_mb.LocalMacroblockInfo->QP + sd->m_pPicParamSet->chroma_qp_index_offset[1];
                    QPChromaIndexV = IPP_MIN(QPChromaIndexV, (Ipp32s)QP_MAX);
                    QPChromaIndexV = IPP_MAX(-bitdepth_chroma_qp_scale, QPChromaIndexV);
                    QPChromaV = QPChromaIndexV < 0 ? QPChromaIndexV : QPtoChromaQP[QPChromaIndexV];
                    QPChromaV += bitdepth_chroma_qp_scale;
                }

                {
                    if (is_high_profile)
                    {
                        ColorSpecific<color_format, Coeffs, PlaneY, PlaneUV, is_field>::ReconstructChromaInter4x4MB_Swit(
                            (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                            (PlanePtrUV)sd->m_pUPlane + offsetC,
                            (PlanePtrUV)sd->m_pVPlane + offsetC,
                            pitch_chroma,
                            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0],
                            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1],
                            QPChromaU,
                            QPChromaV,
                            sd->m_pPicParamSet->m_LevelScale4x4[4].LevelScaleCoeffs[QPChromaU+3][0],
                            sd->m_pPicParamSet->m_LevelScale4x4[5].LevelScaleCoeffs[QPChromaV+3][0],
                            sd->m_pPicParamSet->m_LevelScale4x4[4].LevelScaleCoeffs[QPChromaU],
                            sd->m_pPicParamSet->m_LevelScale4x4[5].LevelScaleCoeffs[QPChromaV],
                            sd->m_pSeqParamSet->qpprime_y_zero_transform_bypass_flag,
                            sd->bit_depth_chroma);
                    }
                    else
                    {
                        ReconstructChromaInterMB(
                            (CoeffsPtr*)&sd->m_pCoeffBlocksRead,
                            (PlanePtrUV)sd->m_pUPlane + offsetC,
                            (PlanePtrUV)sd->m_pVPlane + offsetC,
                            pitch_chroma,
                            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[0],
                            sd->m_cur_mb.LocalMacroblockInfo->cbp4x4_chroma[1],
                            QPChromaU,
                            sd->bit_depth_chroma);
                    }
                } // if (nv12_support)
            }
        }
    } // void ReconstructMacroblock_PSlice(H264SegmentDecoderMultiThreaded * sd)

    void ReconstructMacroblock_BSlice(H264SegmentDecoderMultiThreaded * sd)
    {
        ReconstructMacroblock_PSlice(sd);
    }
     // void ReconstructMacroblock_BSlice(H264SegmentDecoderMultiThreaded * sd)
};

template <class Decoder, class Reconstructor, typename Coeffs, typename PlaneY, typename PlaneUV, Ipp32s color_format, Ipp32s is_field, bool is_high_profile>
class SegmentDecoderHP : public SegmentDecoderHPBase,
    public Decoder,
    public Reconstructor
{
public:
    typedef PlaneY * PlanePtrY;
    typedef PlaneUV * PlanePtrUV;
    typedef Coeffs *  CoeffsPtr;

    typedef UMC::SegmentDecoderHP<Decoder, Reconstructor, Coeffs,
        PlaneY, PlaneUV, color_format, is_field, is_high_profile> ThisClassType;

    Status DecodeSegmentCAVLC(Ipp32u curMB,
                              Ipp32u nBorder,
                              H264SegmentDecoderMultiThreaded * sd)
    {
        Status umcRes = UMC_OK;
        void (ThisClassType::*pDecFunc)(H264SegmentDecoderMultiThreaded *sd);

        Ipp32s MBYAdjust = 0;
        if (is_field && sd->m_field_index)
            MBYAdjust  = sd->mb_height/2;

        sd->m_CurMBAddr = curMB;

        // select decoding function
        switch (sd->m_pSliceHeader->slice_type)
        {
            // intra coded slice
        case INTRASLICE:
        case S_INTRASLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_ISlice_CAVLC;
            break;

            // predicted slice
        case PREDSLICE:
        case S_PREDSLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_PSlice_CAVLC;
            break;

            // bidirectional predicted slice
        default:
            pDecFunc = &ThisClassType::DecodeMacroblock_BSlice_CAVLC;
            break;
        };

        // set initial macroblock coordinates
        sd->m_CurMB_X = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) % sd->mb_width);
        sd->m_CurMB_Y = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) / sd->mb_width) - MBYAdjust;
        sd->m_CurMB_Y <<= (Ipp32s) sd->m_isMBAFF;

        for (; curMB < nBorder; curMB += 1)
        {
            // align pointer to improve performance
            sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon) (align_pointer<CoeffsPtr> (sd->m_pCoeffBlocksWrite, 16));

            sd->UpdateCurrentMBInfo();

            (this->*pDecFunc)(sd);

            // search for end of stream in case
            // of unknown slice sizes
            if (sd->m_MBSkipCount <= 1)
            {
                // Check for new slice header, which can change the next MB
                // to be decoded
                if (false == sd->m_pBitStream->More_RBSP_Data())
                {
                    // align bit stream to start code
                    sd->m_CurMBAddr += 1;
                    umcRes = UMC_ERR_END_OF_STREAM;
                    break;
                }

            }   // check for new slice header

            // set next MB coordinates
            if (0 == sd->m_isMBAFF)
                sd->m_CurMB_X += 1;
            else
            {
                sd->m_CurMB_X += sd->m_CurMBAddr & 1;
                sd->m_CurMB_Y ^= 1;
            }
            // set next MB addres
            sd->m_CurMBAddr += 1;
        }

        // save bitstream variables
        sd->m_pSlice->SetStateVariables(sd->m_MBSkipCount, sd->m_QuantPrev, sd->m_prev_dquant);
        return umcRes;
    }

    virtual Status DecodeSegmentCAVLC_Single(Ipp32s curMB, Ipp32s nBorder,
                                             H264SegmentDecoderMultiThreaded * sd)
    {
        Status umcRes = UMC_OK;
        void (ThisClassType::*pDecFunc)(H264SegmentDecoderMultiThreaded *sd);
        void (ThisClassType::*pRecFunc)(H264SegmentDecoderMultiThreaded *sd);

        Ipp32s MBYAdjust = 0;
        if (is_field && sd->m_field_index)
            MBYAdjust  = sd->mb_height/2;

        sd->m_CurMBAddr = curMB;

        // select decoding function
        switch (sd->m_pSliceHeader->slice_type)
        {
            // intra coded slice
        case INTRASLICE:
        case S_INTRASLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_ISlice_CAVLC;
            pRecFunc = &ThisClassType::ReconstructMacroblock_ISlice;
            break;

            // predicted slice
        case PREDSLICE:
        case S_PREDSLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_PSlice_CAVLC;
            pRecFunc = &ThisClassType::ReconstructMacroblock_PSlice;
            break;

            // bidirectional predicted slice
        default:
            pDecFunc = &ThisClassType::DecodeMacroblock_BSlice_CAVLC;
            pRecFunc = &ThisClassType::ReconstructMacroblock_BSlice;
            break;
        };

        // set initial macroblock coordinates
        sd->m_CurMB_X = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) % sd->mb_width);
        sd->m_CurMB_Y = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) / sd->mb_width) - MBYAdjust;
        sd->m_CurMB_Y <<= (Ipp32s) sd->m_isMBAFF;

        for (; curMB < nBorder; curMB += 1)
        {
            // align pointer to improve performance
            sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon) sd->GetCoefficientsBuffer();
            sd->m_pCoeffBlocksRead = (UMC::CoeffsPtrCommon) sd->GetCoefficientsBuffer();

            sd->UpdateCurrentMBInfo();

            START_TICK
            (this->*pDecFunc)(sd);
            END_TICK(decode_time)

            START_TICK1
            (this->*pRecFunc)(sd);
            END_TICK1(reconstruction_time)

            // search for end of stream in case
            // of unknown slice sizes
            if (sd->m_MBSkipCount <= 1)
            {
                // Check for new slice header, which can change the next MB
                // to be decoded
                if (false == sd->m_pBitStream->More_RBSP_Data())
                {
                    // align bit stream to start code
                    sd->m_CurMBAddr += 1;
                    umcRes = UMC_ERR_END_OF_STREAM;
                    break;
                }

            }   // check for new slice header

            if (false == sd->m_isSliceGroups)
            {
                // set next MB coordinates
                if (0 == sd->m_isMBAFF)
                {
                    sd->m_CurMB_X += 1;
                }
                else
                {
                    sd->m_CurMB_X += sd->m_CurMBAddr & 1;
                    sd->m_CurMB_Y ^= 1;
                }
                // set next MB addres
                sd->m_CurMBAddr += 1;
            }
            else
            {
                sd->m_CurMBAddr = sd->m_mbinfo.active_next_mb_table[sd->m_CurMBAddr];
                sd->m_CurMB_X = (sd->m_CurMBAddr % sd->mb_width);
                sd->m_CurMB_Y = (sd->m_CurMBAddr / sd->mb_width) - MBYAdjust;
            }
        }

        // save bitstream variables
        sd->m_pSlice->SetStateVariables(sd->m_MBSkipCount, sd->m_QuantPrev, sd->m_prev_dquant);
        return umcRes;
    }

    Status DecodeSegmentCABAC(Ipp32u curMB,
                              Ipp32u nBorder,
                              H264SegmentDecoderMultiThreaded *sd)
    {
        Status umcRes = UMC_OK;
        void (ThisClassType::*pDecFunc)(H264SegmentDecoderMultiThreaded *sd);

        Ipp32s MBYAdjust = 0;
        if (is_field && sd->m_field_index)
            MBYAdjust  = sd->mb_height / 2;

        sd->m_CurMBAddr = curMB;

        // select decoding function
        switch (sd->m_pSliceHeader->slice_type)
        {
            // intra coded slice
        case INTRASLICE:
        case S_INTRASLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_ISlice_CABAC;
            break;

            // predicted slice
        case PREDSLICE:
        case S_PREDSLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_PSlice_CABAC;
            break;

            // bidirectional predicted slice
        default:
            pDecFunc = &ThisClassType::DecodeMacroblock_BSlice_CABAC;
            break;
        };

        // set initial macroblock coordinates
        sd->m_CurMB_X = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) % sd->mb_width);
        sd->m_CurMB_Y = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) / sd->mb_width) - MBYAdjust;
        sd->m_CurMB_Y <<= (Ipp32s) sd->m_isMBAFF;

        for (; curMB < nBorder; curMB += 1)
        {
            // align pointer to improve performance
            sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon) (align_pointer<CoeffsPtr> (sd->m_pCoeffBlocksWrite, 16));

            sd->UpdateCurrentMBInfo();

            (this->*pDecFunc)(sd);

            // decode end of slice
            {
                Ipp32s end_of_slice;

                if ((0 == sd->m_isMBAFF) ||
                    (sd->m_CurMBAddr & 1))
                    end_of_slice = sd->m_pBitStream->DecodeSymbolEnd_CABAC();
                else
                    end_of_slice = 0;

                if (end_of_slice)
                {
                    sd->m_pBitStream->TerminateDecode_CABAC();

                    sd->m_CurMBAddr += 1;
                    umcRes = UMC_ERR_END_OF_STREAM;
                    break;
                }
            }

            // set next MB coordinates
            if (0 == sd->m_isMBAFF)
                sd->m_CurMB_X += 1;
            else
            {
                sd->m_CurMB_X += sd->m_CurMBAddr & 1;
                sd->m_CurMB_Y ^= 1;
            }
            // set next MB addres
            sd->m_CurMBAddr += 1;
        }

        // save bitstream variables
        sd->m_pSlice->SetStateVariables(sd->m_MBSkipCount, sd->m_QuantPrev, sd->m_prev_dquant);
        return umcRes;
    }

    virtual Status DecodeSegmentCABAC_Single(Ipp32s curMB, Ipp32s nBorder,
                                             H264SegmentDecoderMultiThreaded * sd)
    {
        Status umcRes = UMC_OK;
        void (ThisClassType::*pDecFunc)(H264SegmentDecoderMultiThreaded *sd);
        void (ThisClassType::*pRecFunc)(H264SegmentDecoderMultiThreaded *sd);

        Ipp32s MBYAdjust = 0;
        if (is_field && sd->m_field_index)
            MBYAdjust  = sd->mb_height / 2;

        sd->m_CurMBAddr = curMB;

        // select decoding function
        switch (sd->m_pSliceHeader->slice_type)
        {
            // intra coded slice
        case INTRASLICE:
        case S_INTRASLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_ISlice_CABAC;
            pRecFunc = &ThisClassType::ReconstructMacroblock_ISlice;
            break;

            // predicted slice
        case PREDSLICE:
        case S_PREDSLICE:
            pDecFunc = &ThisClassType::DecodeMacroblock_PSlice_CABAC;
            pRecFunc = &ThisClassType::ReconstructMacroblock_PSlice;
            break;

            // bidirectional predicted slice
        default:
            pDecFunc = &ThisClassType::DecodeMacroblock_BSlice_CABAC;
            pRecFunc = &ThisClassType::ReconstructMacroblock_BSlice;
            break;
        };

        // set initial macroblock coordinates
        sd->m_CurMB_X = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) % sd->mb_width);
        sd->m_CurMB_Y = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) / sd->mb_width) - MBYAdjust;
        sd->m_CurMB_Y <<= (Ipp32s) sd->m_isMBAFF;

        for (; curMB < nBorder; curMB += 1)
        {
            // reset buffer pointers to start
            // in anyways we write this buffer consistently
            // align pointer to improve performance
            sd->m_pCoeffBlocksRead  = sd->m_pCoeffBlocksWrite = (UMC::CoeffsPtrCommon)sd->GetCoefficientsBuffer();
            sd->UpdateCurrentMBInfo();

            START_TICK
            (this->*pDecFunc)(sd);
            END_TICK(decode_time)

            START_TICK1
            (this->*pRecFunc)(sd);
            END_TICK1(reconstruction_time)

            // decode end of slice
            {
                Ipp32s end_of_slice;

                if ((0 == sd->m_isMBAFF) ||
                    (sd->m_CurMBAddr & 1))
                    end_of_slice = sd->m_pBitStream->DecodeSymbolEnd_CABAC();
                else
                    end_of_slice = 0;

                if (end_of_slice)
                {
                    sd->m_pBitStream->TerminateDecode_CABAC();

                    sd->m_CurMBAddr += 1;
                    umcRes = UMC_ERR_END_OF_STREAM;
                    break;
                }
            }

            // set next MB coordinates
            if (0 == sd->m_isMBAFF)
                sd->m_CurMB_X += 1;
            else
            {
                sd->m_CurMB_X += sd->m_CurMBAddr & 1;
                sd->m_CurMB_Y ^= 1;
            }
            // set next MB addres
            sd->m_CurMBAddr += 1;
        }

        // save bitstream variables
        sd->m_pSlice->SetStateVariables(sd->m_MBSkipCount, sd->m_QuantPrev, sd->m_prev_dquant);

        return umcRes;
    }

    virtual Status ReconstructSegment(Ipp32u curMB, Ipp32u nBorder,
                                      H264SegmentDecoderMultiThreaded * sd)
    {
        Status umcRes = UMC_OK;
        void (ThisClassType::*pRecFunc)(H264SegmentDecoderMultiThreaded *sd);

        Ipp32s MBYAdjust = 0;
        if (is_field && sd->m_field_index)
            MBYAdjust  = sd->mb_height/2;

        sd->m_CurMBAddr = curMB;


        // select reconstruct function
        switch (sd->m_pSliceHeader->slice_type)
        {
            // intra coded slice
        case INTRASLICE:
        case S_INTRASLICE:
            pRecFunc = &ThisClassType::ReconstructMacroblock_ISlice;
            break;

            // predicted slice
        case PREDSLICE:
        case S_PREDSLICE:
            pRecFunc = &ThisClassType::ReconstructMacroblock_PSlice;
            break;

            // bidirectional predicted slice
        default:
            pRecFunc = &ThisClassType::ReconstructMacroblock_BSlice;
            break;
        };

        // set initial macroblock coordinates
        sd->m_CurMB_X = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) % sd->mb_width);
        sd->m_CurMB_Y = ((sd->m_CurMBAddr >> (Ipp32s) sd->m_isMBAFF) / sd->mb_width) - MBYAdjust;
        sd->m_CurMB_Y <<= (Ipp32s) sd->m_isMBAFF;

        // reconstruct starts here
        for (;curMB < nBorder; curMB += 1)
        {
            // align pointer to improve performance
            sd->m_pCoeffBlocksRead = (UMC::CoeffsPtrCommon) (align_pointer<CoeffsPtr> (sd->m_pCoeffBlocksRead, 16));

            sd->UpdateCurrentMBInfo();

            (this->*pRecFunc)(sd);

            // set next MB coordinates
            if (0 == sd->m_isMBAFF)
                sd->m_CurMB_X += 1;
            else
            {
                sd->m_CurMB_X += sd->m_CurMBAddr & 1;
                sd->m_CurMB_Y ^= 1;
            }
            // set next MB addres
            sd->m_CurMBAddr += 1;
        }

        return umcRes;
    }

    virtual void RestoreErrorPlane(PlanePtrY pRefPlane, PlanePtrY pCurrentPlane, Ipp32s pitch,
        Ipp32s offsetX, Ipp32s offsetY, Ipp32s offsetXL, Ipp32s offsetYL,
        Ipp32s mb_width, Ipp32s fieldOffset, IppiSize mbSize)
    {
            IppiSize roiSize;

            roiSize.height = mbSize.height;
            roiSize.width = mb_width * mbSize.width - offsetX;
            Ipp32s offset = offsetX + offsetY*pitch;

            if (offsetYL == offsetY)
            {
                roiSize.width = offsetXL - offsetX + mbSize.width;
            }

            if (pRefPlane)
            {
                CopyPlane(pRefPlane + offset + (fieldOffset*pitch >> 1),
                            pitch,
                            pCurrentPlane + offset + (fieldOffset*pitch >> 1),
                            pitch,
                            roiSize);
            }
            else
            {
                SetPlane(128, pCurrentPlane + offset + (fieldOffset*pitch >> 1), pitch, roiSize);
            }

            if (offsetYL > offsetY)
            {
                roiSize.height = mbSize.height;
                roiSize.width = offsetXL + mbSize.width;
                offset = offsetYL*pitch;

                if (pRefPlane)
                {
                    CopyPlane(pRefPlane + offset + (fieldOffset*pitch >> 1),
                                pitch,
                                pCurrentPlane + offset + (fieldOffset*pitch >> 1),
                                pitch,
                                roiSize);
                }
                else
                {
                    SetPlane(128, pCurrentPlane + offset + (fieldOffset*pitch >> 1), pitch, roiSize);
                }
            }

            if (offsetYL - offsetY > mbSize.height)
            {
                roiSize.height = offsetYL - offsetY - mbSize.height;
                roiSize.width = mb_width * mbSize.width;
                offset = (offsetY + mbSize.height)*pitch;

                if (pRefPlane)
                {
                    CopyPlane(pRefPlane + offset + (fieldOffset*pitch >> 1),
                                pitch,
                                pCurrentPlane + offset + (fieldOffset*pitch >> 1),
                                pitch,
                                roiSize);
                }
                else
                {
                    SetPlane(128, pCurrentPlane + offset + (fieldOffset*pitch >> 1), pitch, roiSize);
                }
            }
    }

    virtual void RestoreErrorRect(Ipp32s startMb, Ipp32s endMb, H264DecoderFrame *pRefFrame,
        H264SegmentDecoderMultiThreaded * sd)
    {
        if (startMb > 0)
            startMb--;

        if (startMb >= endMb || sd->m_isSliceGroups)
            return;

        H264DecoderFrame * pCurrentFrame = sd->m_pSlice->GetCurrentFrame();
        sd->mb_height = sd->m_pSlice->GetMBHeight();
        sd->mb_width = sd->m_pSlice->GetMBWidth();

        Ipp32s pitch_luma = pCurrentFrame->pitch_luma();
        Ipp32s pitch_chroma = pCurrentFrame->pitch_chroma();

        Ipp32s fieldOffset = 0;

        Ipp32s MBYAdjust = 0;

        if (FRM_STRUCTURE > pCurrentFrame->m_PictureStructureForDec)
        {
            if (sd->m_pSlice->IsBottomField())
            {
                fieldOffset = 1;
                startMb += sd->mb_width*sd->mb_height / 2;
                endMb += sd->mb_width*sd->mb_height / 2;
                MBYAdjust = sd->mb_height / 2;
            }

            pitch_luma *= 2;
            pitch_chroma *= 2;
        }

        Ipp32s offsetX, offsetY;
        offsetX = (startMb % sd->mb_width) * 16;
        offsetY = ((startMb / sd->mb_width) - MBYAdjust) * 16;

        Ipp32s offsetXL = ((endMb - 1) % sd->mb_width) * 16;
        Ipp32s offsetYL = (((endMb - 1) / sd->mb_width) - MBYAdjust) * 16;

        IppiSize mbSize;

        mbSize.width = 16;
        mbSize.height = 16;

        if (pRefFrame && pRefFrame->m_pYPlane)
        {
            RestoreErrorPlane((PlanePtrY)pRefFrame->m_pYPlane, (PlanePtrY)pCurrentFrame->m_pYPlane, pitch_luma,
                    offsetX, offsetY, offsetXL, offsetYL,
                    sd->mb_width, fieldOffset, mbSize);
        }
        else
        {
            RestoreErrorPlane(0, (PlanePtrY)pCurrentFrame->m_pYPlane, pitch_luma,
                    offsetX, offsetY, offsetXL, offsetYL,
                    sd->mb_width, fieldOffset, mbSize);
        }

        switch (pCurrentFrame->m_chroma_format)
        {
        case CHROMA_FORMAT_420: // YUV420
            offsetX >>= 1;
            offsetY >>= 1;
            offsetXL >>= 1;
            offsetYL >>= 1;

            mbSize.width >>= 1;
            mbSize.height >>= 1;

            break;
        case CHROMA_FORMAT_422: // YUV422
            offsetX >>= 1;
            offsetXL >>= 1;

            mbSize.width >>= 1;
            break;
        case CHROMA_FORMAT_444: // YUV444
            break;

        case CHROMA_FORMAT_400: // YUV400
            return;
        default:
            VM_ASSERT(false);
            return;
        }

        if (pRefFrame && pRefFrame->m_pUPlane && pRefFrame->m_pVPlane)
        {
            RestoreErrorPlane((PlanePtrUV)pRefFrame->m_pUPlane, (PlanePtrUV)pCurrentFrame->m_pUPlane, pitch_chroma,
                    offsetX, offsetY, offsetXL, offsetYL,
                    sd->mb_width, fieldOffset, mbSize);
            RestoreErrorPlane((PlanePtrUV)pRefFrame->m_pVPlane, (PlanePtrUV)pCurrentFrame->m_pVPlane, pitch_chroma,
                    offsetX, offsetY, offsetXL, offsetYL,
                    sd->mb_width, fieldOffset, mbSize);
        }
        else
        {
            RestoreErrorPlane(0, (PlanePtrUV)pCurrentFrame->m_pUPlane, pitch_chroma,
                    offsetX, offsetY, offsetXL, offsetYL,
                    sd->mb_width, fieldOffset, mbSize);
            RestoreErrorPlane(0, (PlanePtrUV)pCurrentFrame->m_pVPlane, pitch_chroma,
                    offsetX, offsetY, offsetXL, offsetYL,
                    sd->mb_width, fieldOffset, mbSize);
        }
    }
};

template <typename Coeffs, typename PlaneY, typename PlaneUV, bool is_field, Ipp32s color_format, bool is_high_profile>
class CreateSoftSegmentDecoderWrapper
{
public:

    static SegmentDecoderHPBase* CreateSegmentDecoder()
    {
        static SegmentDecoderHP<
            MBDecoder<Coeffs, PlaneY, PlaneUV, color_format, is_field, is_high_profile>,
            MBReconstructor<Coeffs, PlaneY, PlaneUV, color_format, is_field, is_high_profile>,
            Coeffs, PlaneY, PlaneUV, color_format, is_field, is_high_profile> k;
        return &k;
    }
};

template <typename Coeffs, typename PlaneY, typename PlaneUV, bool is_field>
class CreateSegmentDecoderWrapper
{
public:

    static SegmentDecoderHPBase* CreateSoftSegmentDecoder(Ipp32s color_format, bool is_high_profile)
    {
        static SegmentDecoderHPBase* global_sds_array[4][2] = {0};

        if (global_sds_array[0][0] == 0)
        {
#undef INIT
#define INIT(cf, hp) global_sds_array[cf][hp] = CreateSoftSegmentDecoderWrapper<Coeffs, PlaneY, PlaneUV, is_field, cf, hp>::CreateSegmentDecoder();

            INIT(3, true);
            INIT(2, true);
            INIT(1, true);
            INIT(0, true);
            INIT(3, false);
            INIT(2, false);
            INIT(1, false);
            INIT(0, false);
        }

        return global_sds_array[color_format][is_high_profile];
    }
};

#if defined(_MSC_VER)
#pragma warning(default: 4127)
#endif

// declare functions for creating proper decoders
extern
SegmentDecoderHPBase* CreateSD(Ipp32s bit_depth_luma,
                               Ipp32s bit_depth_chroma,
                               bool is_field,
                               Ipp32s color_format,
                               bool is_high_profile);
extern
SegmentDecoderHPBase* CreateSD_ManyBits(Ipp32s bit_depth_luma,
                                        Ipp32s bit_depth_chroma,
                                        bool is_field,
                                        Ipp32s color_format,
                                        bool is_high_profile);

} // end namespace UMC

#endif // __UMC_H264_SEGMENT_DECODER_TEMPLATES_H
