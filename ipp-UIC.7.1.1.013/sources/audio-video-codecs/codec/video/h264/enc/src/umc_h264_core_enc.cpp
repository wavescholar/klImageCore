//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include <math.h>
#include <string.h>
#include "vm_strings.h"
#include "vm_time.h"
#include "umc_h264_config.h"
#include "umc_h264_video_encoder.h"
#include "umc_h264_core_enc.h"
#include "umc_video_data.h"
#include "umc_video_processing.h"
#include "umc_h264_to_ipp.h"
#include "umc_h264_bme.h"

#ifdef FRAME_QP_FROM_FILE
#include <list>
#endif

#include "umc_h264_tables.h"

#ifdef FRAME_QP_FROM_FILE
static std::list<char> frame_type;
static std::list<int> frame_qp;
#endif

#if defined (_OPENMP)
#include "vm_thread.h"
#endif

#ifdef STORE_PICLIST
FILE *refpic;
#endif

//////////////////////////////////////////////////////////////////////////////
// InitDistScaleFactor
//  Calculates the scaling factor used for B slice temporal motion vector
//  scaling and for B slice bidir predictin weighting using the picordercnt
//  values from the current and both reference frames, saving the result
//  to the DistScaleFactor array for future use. The array is initialized
//  with out of range values whenever a bitstream unit is received that
//  might invalidate the data (for example a B slice header resulting in
//  modified reference picture lists). For scaling, the list1 [0] entry
//    is always used.
//////////////////////////////////////////////////////////////////////////////
#define CalculateDSF(index)                                                     \
    /* compute scaling ratio for temporal direct and implicit weighting*/   \
    tb = picCntCur - picCntRef0;    /* distance from previous */            \
    td = picCntRef1 - picCntRef0;    /* distance between ref0 and ref1 */   \
    \
    /* special rule: if td is 0 or if L0 is long-term reference, use */     \
    /* L0 motion vectors and equal weighting.*/                             \
    if (td == 0 ||                                                          \
        H264EncoderFrame_isLongTermRef1(                 \
            pRefPicList0[index],                                            \
            core_enc->m_field_index)){                                      \
        /* These values can be used directly in scaling calculations */     \
        /* to get back L0 or can use conditional test to choose L0.    */   \
        curr_slice->DistScaleFactor[L0Index][L1Index] = 128;    /* for equal weighting    */    \
        curr_slice->DistScaleFactorMV[L0Index][L1Index] = 256;                                  \
    }else{                                                                  \
        \
        tb = MAX(-128,tb);                                                  \
        tb = MIN(127,tb);                                                   \
        td = MAX(-128,td);                                                  \
        td = MIN(127,td);                                                   \
        \
        VM_ASSERT(td != 0);                                                    \
        \
        tx = (16384 + abs(td/2))/td;                                        \
        \
        DistScaleFactor = (tb*tx + 32)>>6;                                  \
        DistScaleFactor = MAX(-1024, DistScaleFactor);                      \
        DistScaleFactor = MIN(1023, DistScaleFactor);                       \
        \
        if (DistScaleFactor < -256 || DistScaleFactor > 512)                \
            curr_slice->DistScaleFactor[L0Index][L1Index] = 128;    /* equal weighting     */   \
        else                                                                \
            curr_slice->DistScaleFactor[L0Index][L1Index] = DistScaleFactor;                    \
        \
        curr_slice->DistScaleFactorMV[L0Index][L1Index] = DistScaleFactor;                      \
    }

//
// Constructor for the EncoderH264 class.
//
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264Slice_Create(
    void* state)
{
    H264Slice<COEFFSTYPE, PIXTYPE>* slice = (H264Slice<COEFFSTYPE, PIXTYPE>*)state;
    memset(slice, 0, sizeof(H264Slice<COEFFSTYPE, PIXTYPE>));
    return UMC_OK;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264Slice_Init(void* state, H264EncoderParams &info)
{
    H264Slice<COEFFSTYPE, PIXTYPE>* slice_enc = (H264Slice<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s status;
    Ipp32s allocSize = 256*sizeof(PIXTYPE) // pred for direct
                     + 256*sizeof(PIXTYPE) // temp working for direct
                     + 256*sizeof(PIXTYPE) // pred for BiPred
                     + 256*sizeof(PIXTYPE) // temp buf for BiPred
                     + 256*sizeof(PIXTYPE) // temp buf for ChromaPred
                     + 256*sizeof(PIXTYPE) // MC
                     + 256*sizeof(PIXTYPE) // ME
                     + 6*512*sizeof(PIXTYPE) // MB prediction & reconstruct
                     + 6*256*sizeof(COEFFSTYPE) //MB transform result
                     + 64*sizeof(Ipp16s)     // Diff
                     + 64*sizeof(COEFFSTYPE) // TransformResults
                     + 64*sizeof(COEFFSTYPE) // QuantResult
                     + 64*sizeof(COEFFSTYPE) // DequantResult
                     + 16*sizeof(COEFFSTYPE) // luma dc
                     + 256*sizeof(Ipp16s)    // MassDiff
                     + 512 + ALIGN_VALUE
                     + 3 * (16 * 16 * 3 + 100);  //Bitstreams
    slice_enc->m_pAllocatedMBEncodeBuffer = (Ipp8u*)H264_Malloc(allocSize);
    if (!slice_enc->m_pAllocatedMBEncodeBuffer)
        return(UMC_ERR_ALLOC);

    // 16-byte align buffer start
    slice_enc->m_pPred4DirectB = (PIXTYPE*)align_pointer<Ipp8u*>(slice_enc->m_pAllocatedMBEncodeBuffer, ALIGN_VALUE);
    slice_enc->m_pTempBuff4DirectB = slice_enc->m_pPred4DirectB + 256;
    slice_enc->m_pPred4BiPred = slice_enc->m_pTempBuff4DirectB + 256;
    slice_enc->m_pTempBuff4BiPred = slice_enc->m_pPred4BiPred + 256;
    slice_enc->m_pTempChromaPred = slice_enc->m_pTempBuff4BiPred + 256;

    slice_enc->m_cur_mb.mb4x4.prediction = slice_enc->m_pTempChromaPred + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mb4x4.reconstruct = slice_enc->m_cur_mb.mb4x4.prediction + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mb4x4.transform = (COEFFSTYPE*)(slice_enc->m_cur_mb.mb4x4.reconstruct + 256); // 256 for pred_intra and 256 for reconstructed blocks

    slice_enc->m_cur_mb.mb8x8.prediction = (PIXTYPE*)(slice_enc->m_cur_mb.mb4x4.transform + 256); // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mb8x8.reconstruct = slice_enc->m_cur_mb.mb8x8.prediction + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mb8x8.transform = (COEFFSTYPE*)(slice_enc->m_cur_mb.mb8x8.reconstruct + 256); // 256 for pred_intra and 256 for reconstructed blocks

    slice_enc->m_cur_mb.mb16x16.prediction = (PIXTYPE*)(slice_enc->m_cur_mb.mb8x8.transform + 256); // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mb16x16.reconstruct = slice_enc->m_cur_mb.mb16x16.prediction + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mb16x16.transform = (COEFFSTYPE*)(slice_enc->m_cur_mb.mb16x16.reconstruct + 256); // 256 for pred_intra and 256 for reconstructed blocks

    slice_enc->m_cur_mb.mbInter.prediction = (PIXTYPE*)(slice_enc->m_cur_mb.mb16x16.transform + 256); // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mbInter.reconstruct = slice_enc->m_cur_mb.mbInter.prediction + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mbInter.transform = (COEFFSTYPE*)(slice_enc->m_cur_mb.mbInter.reconstruct + 256); // 256 for pred_intra and 256 for reconstructed blocks

    slice_enc->m_cur_mb.mbChromaIntra.prediction = (PIXTYPE*)(slice_enc->m_cur_mb.mbInter.transform + 256); // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mbChromaIntra.reconstruct = slice_enc->m_cur_mb.mbChromaIntra.prediction + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mbChromaIntra.transform = (COEFFSTYPE*)(slice_enc->m_cur_mb.mbChromaIntra.reconstruct + 256); // 256 for pred_intra and 256 for reconstructed blocks

    slice_enc->m_cur_mb.mbChromaInter.prediction = (PIXTYPE*)(slice_enc->m_cur_mb.mbChromaIntra.transform + 256); // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mbChromaInter.reconstruct = slice_enc->m_cur_mb.mbChromaInter.prediction + 256; // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_cur_mb.mbChromaInter.transform = (COEFFSTYPE*)(slice_enc->m_cur_mb.mbChromaInter.reconstruct + 256); // 256 for pred_intra and 256 for reconstructed blocks
    slice_enc->m_pMBEncodeBuffer = (PIXTYPE*)(slice_enc->m_cur_mb.mbChromaInter.transform + 256);

    //Init bitstreams
    if (slice_enc->fakeBitstream == NULL)
    {
        slice_enc->fakeBitstream = (H264BsFake *)H264_Malloc(sizeof(H264BsFake));
        if (!slice_enc->fakeBitstream)
            return UMC_ERR_ALLOC;
    }

    H264BsFake_Create(slice_enc->fakeBitstream, 0, 0, info.chroma_format_idc, status);

    Ipp32s i;
    for (i = 0; i < 9; i++)
    {
        if (slice_enc->fBitstreams[i] == NULL)
        {
            slice_enc->fBitstreams[i] = (H264BsFake *)H264_Malloc(sizeof(H264BsFake));
            if (!slice_enc->fBitstreams[i])
                return UMC_ERR_ALLOC;
        }

        H264BsFake_Create(slice_enc->fBitstreams[i], 0, 0, info.chroma_format_idc, status);
    }

    return(UMC_OK);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264Slice_Destroy(void* state)
{
    H264Slice<COEFFSTYPE, PIXTYPE>* slice_enc = (H264Slice<COEFFSTYPE, PIXTYPE> *)state;
    if(slice_enc->m_pAllocatedMBEncodeBuffer != NULL)
    {
        H264_Free(slice_enc->m_pAllocatedMBEncodeBuffer);
        slice_enc->m_pAllocatedMBEncodeBuffer = NULL;
    }
    slice_enc->m_pPred4DirectB = NULL;
    slice_enc->m_pPred4BiPred = NULL;
    slice_enc->m_pTempBuff4DirectB = NULL;
    slice_enc->m_pTempBuff4BiPred = NULL;
    slice_enc->m_pMBEncodeBuffer = NULL;
    slice_enc->m_cur_mb.mb4x4.prediction = NULL;
    slice_enc->m_cur_mb.mb4x4.reconstruct = NULL;
    slice_enc->m_cur_mb.mb4x4.transform = NULL;
    slice_enc->m_cur_mb.mb8x8.prediction = NULL;
    slice_enc->m_cur_mb.mb8x8.reconstruct = NULL;
    slice_enc->m_cur_mb.mb8x8.transform = NULL;

    if (slice_enc->fakeBitstream != NULL)
    {
        H264_Free(slice_enc->fakeBitstream);
        slice_enc->fakeBitstream = NULL;
    }

    Ipp32s i;
    for (i = 0; i < 9; i++)
    {
        if(slice_enc->fBitstreams[i] != NULL)
        {
            H264_Free(slice_enc->fBitstreams[i]);
            slice_enc->fBitstreams[i] = NULL;
        }
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Create(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264EncoderParams *params = &core_enc->m_params;
    params = new (params) H264EncoderParams();

    core_enc->m_pBitStream = NULL;
    core_enc->memAlloc = NULL;
    core_enc->profile_frequency = 1;
    core_enc->m_iProfileIndex = 0;
    core_enc->m_is_mb_data_initialized = false;
    core_enc->m_pAllocEncoderInst = NULL;
    core_enc->m_pbitstreams = NULL;

    core_enc->m_bs1 = NULL;
    H264EncoderFrameList_Create<PIXTYPE>(&core_enc->m_dpb, NULL);
    H264EncoderFrameList_Create<PIXTYPE>(&core_enc->m_cpb, NULL);
    core_enc->m_uIntraFrameInterval = 0;
    core_enc->m_uIDRFrameInterval = 0;
    core_enc->m_PicOrderCnt = 0;
    core_enc->m_PicOrderCnt_Accu = 0;
    core_enc->m_pParsedDataNew = 0;
    core_enc->m_pReconstructFrame = NULL;
    core_enc->m_l1_cnt_to_start_B = 0;
    core_enc->m_pMBOffsets = NULL;
    core_enc->m_EmptyThreshold = NULL;
    core_enc->m_DirectBSkipMEThres = NULL;
    core_enc->m_PSkipMEThres = NULL;
    core_enc->m_BestOf5EarlyExitThres = NULL;
    core_enc->use_implicit_weighted_bipred = false;
    core_enc->m_is_cur_pic_afrm = false;
    core_enc->m_Slices = NULL;
    core_enc->m_total_bits_encoded = 0;
    core_enc->m_MaxSliceSize = 0;

    H264_AVBR_Create(&core_enc->avbr);
    core_enc->m_PaddedSize.width = 0;
    core_enc->m_PaddedSize.height = 0;

    // Initialize the BRCState local variables based on the default
    // settings in core_enc->m_params.

    // If these assertions fail, then uTargetFrmSize needs to be set to
    // something other than 0.
    // Initialize the sequence parameter set structure.

    memset(&core_enc->m_SeqParamSet, 0, sizeof(H264SeqParamSet));
    core_enc->m_SeqParamSet.profile_idc = H264_PROFILE_MAIN;
    core_enc->m_SeqParamSet.chroma_format_idc = 1;
    core_enc->m_SeqParamSet.bit_depth_luma = 8;
    core_enc->m_SeqParamSet.bit_depth_chroma = 8;
    core_enc->m_SeqParamSet.bit_depth_aux = 8;
    core_enc->m_SeqParamSet.alpha_opaque_value = 8;

    // Initialize the picture parameter set structure.
    memset(&core_enc->m_PicParamSet, 0, sizeof(H264PicParamSet));

    // Initialize the slice header structure.
    memset(&core_enc->m_SliceHeader, 0, sizeof(H264SliceHeader));
    core_enc->m_SliceHeader.direct_spatial_mv_pred_flag = 1;

    core_enc->m_DirectTypeStat[0] = 0;
    core_enc->m_DirectTypeStat[1] = 0;

    core_enc->eFrameSeq = (H264EncoderFrame<PIXTYPE>**)H264_Malloc(1 * sizeof(H264EncoderFrame<PIXTYPE>*));
    if (!core_enc->eFrameSeq)
    {
        H264CoreEncoder_Destroy<COEFFSTYPE, PIXTYPE>(state);
        return UMC_ERR_ALLOC;
    }

    core_enc->eFrameType = (EnumPicCodType*)H264_Malloc(1 * sizeof(EnumPicCodType));
    if (!core_enc->eFrameType)
    {
        H264CoreEncoder_Destroy<COEFFSTYPE, PIXTYPE>(state);
        return UMC_ERR_ALLOC;
    }

    core_enc->eFrameType[0] = PREDPIC;
    return UMC_OK;
}

//
// move all frames in WaitingForRef to ReadyToEncode
//
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_MoveFromCPBToDPB(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
 //   EnumPicCodType  ePictureType;
    H264EncoderFrameList_RemoveFrame<PIXTYPE>(&core_enc->m_cpb, core_enc->m_pCurrentFrame);
    H264EncoderFrameList_insertAtCurrent<PIXTYPE>(&core_enc->m_dpb, core_enc->m_pCurrentFrame);
    return UMC_OK;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_CleanDPB(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264EncoderFrame<PIXTYPE> *pFrm = H264EncoderFrameList_findNextDisposable<PIXTYPE>(&core_enc->m_dpb);
    //   EnumPicCodType  ePictureType;
    Status      ps = UMC_OK;
    while (pFrm != NULL)
    {
        H264EncoderFrameList_RemoveFrame<PIXTYPE>(&core_enc->m_dpb, pFrm);
        H264EncoderFrameList_insertAtCurrent<PIXTYPE>(&core_enc->m_cpb, pFrm);
        pFrm = H264EncoderFrameList_findNextDisposable<PIXTYPE>(&core_enc->m_dpb);
    }
    return ps;
}

/*************************************************************
 *  Name:         encodeFrameHeader
 *  Description:  Write out the frame header to the bit stream.
 ************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_encodeFrameHeader(void* state, H264BsReal* bs, MediaData* dst, bool bIDR_Pic, bool& startPicture)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status ps = UMC_OK;

    // First, write a access unit delimiter for the frame.
    if (core_enc->m_params.write_access_unit_delimiters)
    {
        ps = H264BsReal_PutPicDelimiter(bs, core_enc->m_PicType);

        H264BsBase_WriteTrailingBits(&bs->m_base);

        // Copy PicDelimiter RBSP to the end of the output buffer after
        // Adding start codes and SC emulation prevention.
        dst->SetDataSize(
            dst->GetDataSize() +
                H264BsReal_EndOfNAL(
                    bs,
                    (Ipp8u*)dst->GetDataPointer() + dst->GetDataSize(),
                    0,
                    NAL_UT_AUD,
                    startPicture));
    }

    // If this is an IDR picture, write the seq and pic parameter sets
    if (bIDR_Pic)
    {
        // Write the seq_parameter_set_rbsp
        ps = H264BsReal_PutSeqParms(bs, core_enc->m_SeqParamSet);

        H264BsBase_WriteTrailingBits(&bs->m_base);

        // Copy Sequence Parms RBSP to the end of the output buffer after
        // Adding start codes and SC emulation prevention.
        dst->SetDataSize(
            dst->GetDataSize() +
                H264BsReal_EndOfNAL(
                    bs,
                    (Ipp8u*)dst->GetDataPointer() + dst->GetDataSize(),
                    1,
                    NAL_UT_SPS,
                    startPicture));

        if(core_enc->m_SeqParamSet.pack_sequence_extension)
        {
            // Write the seq_parameter_set_extension_rbsp when needed.
            ps = H264BsReal_PutSeqExParms(bs, core_enc->m_SeqParamSet);

            H264BsBase_WriteTrailingBits(&bs->m_base);

            // Copy Sequence Parms RBSP to the end of the output buffer after
            // Adding start codes and SC emulation prevention.
            dst->SetDataSize(
                dst->GetDataSize() +
                    H264BsReal_EndOfNAL(
                        bs,
                        (Ipp8u*)dst->GetDataPointer() + dst->GetDataSize(),
                        1,
                        NAL_UT_SEQEXT,
                        startPicture));
        }

        ps = H264BsReal_PutPicParms(bs, core_enc->m_PicParamSet, core_enc->m_SeqParamSet);

        H264BsBase_WriteTrailingBits(&bs->m_base);

        // Copy Picture Parms RBSP to the end of the output buffer after
        // Adding start codes and SC emulation prevention.
        dst->SetDataSize(
            dst->GetDataSize() +
                H264BsReal_EndOfNAL(
                    bs,
                    (Ipp8u*)dst->GetDataPointer() + dst->GetDataSize(),
                    1,
                    NAL_UT_PPS,
                    startPicture));
    }

    return ps;
}

/*************************************************************
 *  Name:         EncodeDummyFrame
 *  Description:  Writes out a blank frame to the bitstream in
                  case of buffer overflow.
 ************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_EncodeDummyFrame(void* state, MediaData* dst)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s uMBRow, uMB;
    Status status = UMC_OK;
    bool startAccessUnit = true;

    H264BsReal_Reset(core_enc->m_bs1);

    H264CoreEncoder_encodeFrameHeader<COEFFSTYPE, PIXTYPE>(state, core_enc->m_bs1, dst, false, startAccessUnit);

    for (uMBRow=0; uMBRow < core_enc->m_HeightInMBs; uMBRow++)
    {
        for (uMB=0; uMB < core_enc->m_WidthInMBs; uMB++)
        {
            if (core_enc->m_PicType == INTRAPIC)
            {
                H264BsReal_PutBit(core_enc->m_bs1, 1); // MBTYPE: INTRA_16x16
                H264BsReal_PutBits(core_enc->m_bs1, 0, 2); // AIC 16x16: DC
                H264BsReal_PutBit(core_enc->m_bs1, 1); // CBP: 0
                H264BsReal_PutBit(core_enc->m_bs1, 1); // 16x16 Coef: EOB
            }
            else if (core_enc->m_PicType == PREDPIC ||  core_enc->m_PicType == BPREDPIC)
            {
                H264BsReal_PutBit(core_enc->m_bs1, 1); // skipped MB
            }
            else
            {
                status = UMC_ERR_FAILED;
                goto done;
            }

        }
    }

done:
    return status;
}

#ifdef FRAME_INTERPOLATION
#ifdef INTERPOLATE_FRAME_THREAD
template<typename PIXTYPE>
void InterpolateHP_Threaded(PIXTYPE *pY, Ipp32s mbw, Ipp32s mbh, Ipp32s step, Ipp32s planeSize, Ipp32s bitDepth)
{
    static IppiSize sz = {16, 16};
    Ipp32s k,mbw2;
    PIXTYPE  *pB, *pH, *pJ;
    pB = pY + planeSize;
    pH = pB + planeSize;
    pJ = pH + planeSize;
    
    mbw2 = mbw+2;
#pragma omp parallel for private(k)
    for(k=0; k<(mbh+2)*mbw2; k++ )
    {
        Ipp32s mbs = k/mbw2;
        Ipp32s i = -16 + mbs*16;
        Ipp32s j = -16 + (k-mbw2*mbs)*16;
        Ipp32s offset = i * step + j;
        ownInterpolateLuma_H264(pY + offset, step, pB + offset, step, 2, 0, sz, bitDepth);
        ownInterpolateLuma_H264(pY + offset, step, pH + offset, step, 0, 2, sz, bitDepth);
        ownInterpolateLuma_H264(pY + offset, step, pJ + offset, step, 2, 2, sz, bitDepth);
    }
    
#pragma omp parallel sections
    {
#pragma omp section
        {
            ExpandPlane(pB - 16 * step - 16, (mbw + 2)* 16, (mbh + 2)* 16, step, LUMA_PADDING - 16);
        }
#pragma omp section
        {
            ExpandPlane(pH - 16 * step - 16, (mbw + 2)* 16, (mbh + 2)* 16, step, LUMA_PADDING - 16);
        }
#pragma omp section
        {
            ExpandPlane(pJ - 16 * step - 16, (mbw + 2)* 16, (mbh + 2)* 16, step, LUMA_PADDING - 16);
        }
    }
}
#endif

template<typename PIXTYPE>
void InterpolateHP(PIXTYPE *pY, Ipp32s mbw, Ipp32s mbh, Ipp32s step, Ipp32s planeSize, Ipp32s bitDepth)
{
    static IppiSize sz = {16, 16};
    Ipp32s   i, j;
    PIXTYPE  *pB, *pH, *pJ;
    pB = pY + planeSize;
    pH = pB + planeSize;
    pJ = pH + planeSize;
    for (i = -1; i <= mbh; i ++) {
        for (j = -1; j <= mbw; j ++) {
            ownInterpolateLuma_H264(pY + i * 16 * step + j * 16, step, pB + i * 16 * step + j * 16, step, 2, 0, sz, bitDepth);
            ownInterpolateLuma_H264(pY + i * 16 * step + j * 16, step, pH + i * 16 * step + j * 16, step, 0, 2, sz, bitDepth);
            ownInterpolateLuma_H264(pY + i * 16 * step + j * 16, step, pJ + i * 16 * step + j * 16, step, 2, 2, sz, bitDepth);
        }
    }
    ExpandPlane(pB - 16 * step - 16, (mbw + 2)* 16, (mbh + 2)* 16, step, LUMA_PADDING - 16);
    ExpandPlane(pH - 16 * step - 16, (mbw + 2)* 16, (mbh + 2)* 16, step, LUMA_PADDING - 16);
    ExpandPlane(pJ - 16 * step - 16, (mbw + 2)* 16, (mbh + 2)* 16, step, LUMA_PADDING - 16);
}
#endif

///////////////////////////////////////////////////////////////////////////
//
//  End_Picture
//
// Any processing needed after each picture
//
///////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_End_Picture(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    // deblock filter just completed
    Ipp32s shift_x = 0;
    Ipp32s shift_y = 0;

    switch(core_enc->m_PicParamSet.chroma_format_idc)
    {
        case 1:
            shift_x=shift_y=1;
            break;
        case 2:
            shift_x=1;
            shift_y=0;
            break;
        case 3:
            shift_x=shift_y=0;
            break;
    }
    Ipp32s chromaPadding = LUMA_PADDING >> 1;
    if(core_enc->m_pCurrentFrame->m_data.m_colorFormat == YUV422
        || core_enc->m_pCurrentFrame->m_data.m_colorFormat == YUV422A
        )
        chromaPadding = LUMA_PADDING;
    switch (core_enc->m_pCurrentFrame->m_PictureStructureForDec)
    {
    case FRM_STRUCTURE:
            if(core_enc->m_PicParamSet.chroma_format_idc){
#ifdef EXPAND_PLANE_THREAD
#pragma omp parallel sections
{
#pragma omp section
{
                ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16, core_enc->m_pReconstructFrame->m_pitchPixels, LUMA_PADDING);
}
#pragma omp section
{
                ExpandPlane(
                    core_enc->m_pReconstructFrame->m_pUPlane,
                    core_enc->m_WidthInMBs * 16 >> shift_x,
                    core_enc->m_HeightInMBs * 16 >> shift_y,
                    core_enc->m_pReconstructFrame->m_pitchPixels,
                    chromaPadding);
}
#pragma omp section
{
                ExpandPlane(
                    core_enc->m_pReconstructFrame->m_pVPlane,
                    core_enc->m_WidthInMBs * 16 >> shift_x,
                    core_enc->m_HeightInMBs * 16 >> shift_y,
                    core_enc->m_pReconstructFrame->m_pitchPixels,
                    chromaPadding);
}
}
#else
            ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16, core_enc->m_pReconstructFrame->m_pitchPixels, LUMA_PADDING);
            ExpandPlane(
                core_enc->m_pReconstructFrame->m_pUPlane,
                core_enc->m_WidthInMBs * 16 >> shift_x,
                core_enc->m_HeightInMBs * 16 >> shift_y,
                core_enc->m_pReconstructFrame->m_pitchPixels,
                chromaPadding);
            ExpandPlane(
                core_enc->m_pReconstructFrame->m_pVPlane,
                core_enc->m_WidthInMBs * 16 >> shift_x,
                core_enc->m_HeightInMBs * 16 >> shift_y,
                core_enc->m_pReconstructFrame->m_pitchPixels,
                chromaPadding);
#endif
        }
        else
            ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16, core_enc->m_pReconstructFrame->m_pitchPixels, LUMA_PADDING);

#ifdef FRAME_INTERPOLATION
#ifdef INTERPOLATE_FRAME_THREAD
            if( core_enc->m_params.m_iThreads > 1 )
                InterpolateHP_Threaded(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs, core_enc->m_HeightInMBs, core_enc->m_pReconstructFrame->m_pitchPixels, core_enc->m_pReconstructFrame->m_PlaneSize, core_enc->m_PicParamSet.bit_depth_luma);
            else
                InterpolateHP(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs, core_enc->m_HeightInMBs, core_enc->m_pReconstructFrame->m_pitchPixels, core_enc->m_pReconstructFrame->m_PlaneSize, core_enc->m_PicParamSet.bit_depth_luma);
#else
            InterpolateHP(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs, core_enc->m_HeightInMBs, core_enc->m_pReconstructFrame->m_pitchPixels, core_enc->m_pReconstructFrame->m_PlaneSize, core_enc->m_PicParamSet.bit_depth_luma);
#endif
#endif
        break;
    case AFRM_STRUCTURE:
        if(!shift_y)
            shift_y = 1;
        else
            shift_y <<= 1;
        ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane, core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16>>1, core_enc->m_pReconstructFrame->m_pitchPixels*2, LUMA_PADDING);
        if(core_enc->m_PicParamSet.chroma_format_idc)
        {
            ExpandPlane(core_enc->m_pReconstructFrame->m_pUPlane, core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
            ExpandPlane(core_enc->m_pReconstructFrame->m_pVPlane, core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
        }
        ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels, core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16>>1, core_enc->m_pReconstructFrame->m_pitchPixels*2, LUMA_PADDING);
        if(core_enc->m_PicParamSet.chroma_format_idc)
        {
            ExpandPlane(core_enc->m_pReconstructFrame->m_pUPlane + core_enc->m_pReconstructFrame->m_pitchPixels, core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pCurrentFrame->m_pitchPixels*2, chromaPadding);
            ExpandPlane(core_enc->m_pReconstructFrame->m_pVPlane + core_enc->m_pReconstructFrame->m_pitchPixels, core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
        }
        break;
    case FLD_STRUCTURE:
            if(core_enc->m_PicParamSet.chroma_format_idc) {
#ifdef EXPAND_PLANE_THREAD
#pragma omp parallel sections
{
#pragma omp section
{
                ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                        core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16, core_enc->m_pReconstructFrame->m_pitchPixels*2, LUMA_PADDING);
}
#pragma omp section
{
                ExpandPlane(core_enc->m_pReconstructFrame->m_pUPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                            core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
}
#pragma omp section
{
                ExpandPlane(core_enc->m_pReconstructFrame->m_pVPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                            core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
}
}
#else
                ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                        core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16, core_enc->m_pReconstructFrame->m_pitchPixels*2, LUMA_PADDING);
                ExpandPlane(core_enc->m_pReconstructFrame->m_pUPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                            core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
                ExpandPlane(core_enc->m_pReconstructFrame->m_pVPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                            core_enc->m_WidthInMBs*16>>shift_x, core_enc->m_HeightInMBs*16>>shift_y, core_enc->m_pReconstructFrame->m_pitchPixels*2, chromaPadding);
#endif
            }else{
            ExpandPlane(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index],
                    core_enc->m_WidthInMBs*16, core_enc->m_HeightInMBs*16, core_enc->m_pReconstructFrame->m_pitchPixels*2, LUMA_PADDING);
            }
#ifdef FRAME_INTERPOLATION
#ifdef INTERPOLATE_FRAME_THREAD
            if( core_enc->m_params.m_iThreads > 1 )
                InterpolateHP_Threaded(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index], core_enc->m_WidthInMBs, core_enc->m_HeightInMBs, core_enc->m_pReconstructFrame->m_pitchPixels * 2, core_enc->m_pReconstructFrame->m_PlaneSize, core_enc->m_PicParamSet.bit_depth_luma);
            else
                InterpolateHP(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index], core_enc->m_WidthInMBs, core_enc->m_HeightInMBs, core_enc->m_pReconstructFrame->m_pitchPixels * 2, core_enc->m_pReconstructFrame->m_PlaneSize, core_enc->m_PicParamSet.bit_depth_luma);
#else
            InterpolateHP(core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pReconstructFrame->m_pitchPixels*core_enc->m_pReconstructFrame->m_bottom_field_flag[core_enc->m_field_index], core_enc->m_WidthInMBs, core_enc->m_HeightInMBs, core_enc->m_pReconstructFrame->m_pitchPixels * 2, core_enc->m_pReconstructFrame->m_PlaneSize, core_enc->m_PicParamSet.bit_depth_luma);
#endif
#endif
        break;
    }
}   // End_Picture

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_SetDPBSize(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u iMaxDPB;
    Ipp32u iWidthMBs, iHeightMBs;

    // MaxDPB, per Table A-1, Level Limits
    switch (core_enc->m_params.level_idc)
    {
    case 9:
    case 10:
        iMaxDPB = 396;
        break;
    case 11:
        iMaxDPB = 900;
        break;
    case 12:
    case 13:
    case 20:
        iMaxDPB = 2376;
        break;
    case 21:
        iMaxDPB = 4752;
        break;
    case 22:
    case 30:
        iMaxDPB = 8100;
        break;
    case 31:
        iMaxDPB = 18000;
        break;
    case 32:
        iMaxDPB = 20480;
        break;
    case 40:
    case 41:
        iMaxDPB = 32768;
        break;
    case 42:
        iMaxDPB = 34816;
        break;
    case 50:
        iMaxDPB = 110400;
        break;
    case 51:
        iMaxDPB = 184320;
        break;
    default:
        iMaxDPB = 184320;
    }

    iWidthMBs  = (core_enc->m_params.m_info.videoInfo.m_iWidth + 15)/16;
    iHeightMBs = (core_enc->m_params.m_info.videoInfo.m_iHeight + 15)/16;
    if(core_enc->m_params.coding_type)
        iHeightMBs *= 2;

    core_enc->m_dpbSize = IPP_MIN(16, iMaxDPB/(iWidthMBs*iHeightMBs));
    if(core_enc->m_params.num_ref_frames)
        core_enc->m_dpbSize = MIN(core_enc->m_params.num_ref_frames, core_enc->m_dpbSize);
    else
        core_enc->m_dpbSize = 1;
}

/*************************************************************
 *  Name: SetSequenceParameters
 *  Description:  Fill in the Sequence Parameter Set for this
 *  sequence.  Can only change at an IDR picture.
 *************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_SetSequenceParameters(
    void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    core_enc->m_SeqParamSet.profile_idc = core_enc->m_params.profile_idc;

    // We don't meet any of these contraints yet
    core_enc->m_SeqParamSet.constraint_set0_flag = core_enc->m_params.constraint_set_flag[0];
    core_enc->m_SeqParamSet.constraint_set1_flag = core_enc->m_params.constraint_set_flag[1];
    core_enc->m_SeqParamSet.constraint_set2_flag = core_enc->m_params.constraint_set_flag[2];
    core_enc->m_SeqParamSet.constraint_set3_flag = core_enc->m_params.constraint_set_flag[3];
    core_enc->m_SeqParamSet.constraint_set4_flag = core_enc->m_params.constraint_set_flag[4];
    core_enc->m_SeqParamSet.constraint_set5_flag = core_enc->m_params.constraint_set_flag[5];

    core_enc->m_SeqParamSet.seq_parameter_set_id = 0;

    // Frame numbers are mod 256.
    core_enc->m_SeqParamSet.log2_max_frame_num = 8;

    // Setup pic_order_cnt_type based on use of B frames.
    // Note! pic_order_cnt_type == 1 is not implemented

    // The following are not transmitted in either case below, and are
    // just initialized here to be nice.
    core_enc->m_SeqParamSet.delta_pic_order_always_zero_flag = 0;
    core_enc->m_SeqParamSet.offset_for_non_ref_pic = 0;
    core_enc->m_SeqParamSet.poffset_for_ref_frame = NULL;
    core_enc->m_SeqParamSet.num_ref_frames_in_pic_order_cnt_cycle = 0;

    if (core_enc->m_params.B_frame_rate == 0 && core_enc->m_params.coding_type == 0)
    {
        core_enc->m_SeqParamSet.pic_order_cnt_type = 2;
        core_enc->m_SeqParamSet.log2_max_pic_order_cnt_lsb = 0;
        // Right now this only supports simple P frame patterns (e.g. H264PPPP...)
    }
    else
    {
        //Ipp32s log2_max_poc = (Ipp32u)log(((Ipp64f)core_enc->m_params.B_frame_rate +
        //    core_enc->m_params.num_ref_to_start_code_B_slice)/log((Ipp64f)2) + 1) << 1;
        Ipp32s log2_max_poc = (Ipp32s) (log((Ipp64f)((core_enc->m_params.B_frame_rate<<((core_enc->m_params.B_reference_mode == 2)?1:0))+ core_enc->m_params.num_ref_frames))
                               / log(2.0)) + 3; // 3=1+1+1=round+multiply by 2 in counting+devide by 2 in comparison

        core_enc->m_SeqParamSet.log2_max_pic_order_cnt_lsb = IPP_MAX(log2_max_poc, 4);

        if (core_enc->m_SeqParamSet.log2_max_pic_order_cnt_lsb > 16)
        {
            VM_ASSERT(false);
            core_enc->m_SeqParamSet.log2_max_pic_order_cnt_lsb = 16;
        }

        core_enc->m_SeqParamSet.pic_order_cnt_type = 0;
        // Right now this only supports simple B frame patterns (e.g. IBBPBBP...)
    }
    core_enc->m_SeqParamSet.num_ref_frames = core_enc->m_params.num_ref_frames;

    // Note!  NO code after this point supports pic_order_cnt_type == 1
    // Always zero because we don't support field encoding
    core_enc->m_SeqParamSet.offset_for_top_to_bottom_field = 0;

    core_enc->m_SeqParamSet.frame_mbs_only_flag = (core_enc->m_params.coding_type)? 0: 1;

    core_enc->m_SeqParamSet.gaps_in_frame_num_value_allowed_flag = 0;
    core_enc->m_SeqParamSet.mb_adaptive_frame_field_flag = core_enc->m_params.coding_type>1;

    // If set to 1, 8x8 blocks in Direct Mode always use 1 MV,
    // obtained from the "outer corner" 4x4 block, regardless
    // of how the CoLocated 8x8 is split into subblocks.  If this
    // is 0, then the 8x8 in Direct Mode is subdivided exactly as
    // the Colocated 8x8, with the appropriate number of derived MVs.
    core_enc->m_SeqParamSet.direct_8x8_inference_flag = (core_enc->m_params.direct_8x8_inference_flag || !core_enc->m_SeqParamSet.frame_mbs_only_flag);

    // Picture Dimensions in MBs
    core_enc->m_SeqParamSet.frame_width_in_mbs = ((core_enc->m_params.m_info.videoInfo.m_iWidth+15)>>4);
    core_enc->m_SeqParamSet.frame_height_in_mbs = ((core_enc->m_params.m_info.videoInfo.m_iHeight+(16<<(1 - core_enc->m_SeqParamSet.frame_mbs_only_flag)) - 1)>>4) >> (1 - core_enc->m_SeqParamSet.frame_mbs_only_flag);
    Ipp32u frame_height_in_mbs = core_enc->m_SeqParamSet.frame_height_in_mbs << (1 - core_enc->m_SeqParamSet.frame_mbs_only_flag);

    // If the width & height in MBs doesn't match the image dimensions then do
    // some cropping in the decoder
    if (((core_enc->m_SeqParamSet.frame_width_in_mbs<<4) != core_enc->m_params.m_info.videoInfo.m_iWidth) ||
        ((frame_height_in_mbs << 4) != core_enc->m_params.m_info.videoInfo.m_iHeight))
    {
        core_enc->m_SeqParamSet.frame_cropping_flag = 1;
        core_enc->m_SeqParamSet.frame_crop_left_offset = 0;
        core_enc->m_SeqParamSet.frame_crop_right_offset =
            ((core_enc->m_SeqParamSet.frame_width_in_mbs<<4) - core_enc->m_params.m_info.videoInfo.m_iWidth)/SubWidthC[core_enc->m_SeqParamSet.chroma_format_idc];
        core_enc->m_SeqParamSet.frame_crop_top_offset = 0;
        core_enc->m_SeqParamSet.frame_crop_bottom_offset =
            ((frame_height_in_mbs<<4) - core_enc->m_params.m_info.videoInfo.m_iHeight)/(SubHeightC[core_enc->m_SeqParamSet.chroma_format_idc]*(2 - core_enc->m_SeqParamSet.frame_mbs_only_flag));
    }
    else
    {
        core_enc->m_SeqParamSet.frame_cropping_flag = 0;
        core_enc->m_SeqParamSet.frame_crop_left_offset = 0;
        core_enc->m_SeqParamSet.frame_crop_right_offset = 0;
        core_enc->m_SeqParamSet.frame_crop_top_offset = 0;
        core_enc->m_SeqParamSet.frame_crop_bottom_offset = 0;
    }

    core_enc->m_SeqParamSet.vui_parameters_present_flag = 0;

    core_enc->m_SeqParamSet.level_idc = core_enc->m_params.level_idc;

    core_enc->m_SeqParamSet.profile_idc                    = core_enc->m_params.profile_idc;
    core_enc->m_SeqParamSet.chroma_format_idc              = (Ipp8s)core_enc->m_params.chroma_format_idc;
    core_enc->m_SeqParamSet.bit_depth_luma                 = core_enc->m_params.bit_depth_luma;
    core_enc->m_SeqParamSet.bit_depth_chroma               = core_enc->m_params.bit_depth_chroma;
    core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag = core_enc->m_params.qpprime_y_zero_transform_bypass_flag;
    core_enc->m_SeqParamSet.seq_scaling_matrix_present_flag = false;

    core_enc->m_SeqParamSet.bit_depth_aux                  = core_enc->m_params.bit_depth_aux;
    core_enc->m_SeqParamSet.alpha_incr_flag                = core_enc->m_params.alpha_incr_flag;
    core_enc->m_SeqParamSet.alpha_opaque_value             = core_enc->m_params.alpha_opaque_value;
    core_enc->m_SeqParamSet.alpha_transparent_value        = core_enc->m_params.alpha_transparent_value;
    core_enc->m_SeqParamSet.aux_format_idc                 = core_enc->m_params.aux_format_idc;

    if(   core_enc->m_SeqParamSet.bit_depth_aux != 8
       || core_enc->m_SeqParamSet.alpha_incr_flag != 0
       || core_enc->m_SeqParamSet.alpha_opaque_value != 0
       || core_enc->m_SeqParamSet.alpha_transparent_value != 0
       || core_enc->m_SeqParamSet.aux_format_idc != 0)
    {
        core_enc->m_SeqParamSet.pack_sequence_extension = 1;
    }

    // Precalculate these values so we have them for later (repeated) use.
    core_enc->m_SeqParamSet.MaxMbAddress = (core_enc->m_SeqParamSet.frame_width_in_mbs * frame_height_in_mbs) - 1;
    H264CoreEncoder_SetDPBSize<COEFFSTYPE, PIXTYPE>(state);

    //Scaling matrices
    if(core_enc->m_params.use_default_scaling_matrix)
    {
        Ipp32s i;
        //setup matrices that will be used
        core_enc->m_SeqParamSet.seq_scaling_matrix_present_flag = true;
        // 4x4 matrices
        for( i=0; i<6; i++ )
            core_enc->m_SeqParamSet.seq_scaling_list_present_flag[i] = true;
        //Copy default
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[0], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[1], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[2], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[3], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[4], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[5], FlatScalingList4x4, 16*sizeof(Ipp8u));

        // 8x8 matrices
        core_enc->m_SeqParamSet.seq_scaling_list_present_flag[6] = true;
        core_enc->m_SeqParamSet.seq_scaling_list_present_flag[7] = true;

        //Copy default scaling matrices
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_8x8[0], DefaultScalingList8x8[1], 64*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_8x8[1], DefaultScalingList8x8[1], 64*sizeof(Ipp8u));
    }
    else
    {
        core_enc->m_SeqParamSet.seq_scaling_matrix_present_flag = false;
        //Copy default
/*            memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[0], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[1], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[2], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[3], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[4], FlatScalingList4x4, 16*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_4x4[5], FlatScalingList4x4, 16*sizeof(Ipp8u));
*/
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_8x8[0], FlatScalingList8x8, 64*sizeof(Ipp8u));
        memcpy(core_enc->m_SeqParamSet.seq_scaling_list_8x8[1], FlatScalingList8x8, 64*sizeof(Ipp8u));
    }

     //Generate new scaling matrices for use in transform
    Ipp32s qp_rem,i;
/*     for( i=0; i < 6; i++ )
         for( qp_rem = 0; qp_rem<6; qp_rem++ ) {
             ownGenScaleLevel4x4_H264_8u16s_D2( core_enc->m_SeqParamSet.seq_scaling_list_4x4[i],
                                     core_enc->m_SeqParamSet.seq_scaling_inv_matrix_4x4[i][qp_rem],
                                     core_enc->m_SeqParamSet.seq_scaling_matrix_4x4[i][qp_rem],
                                     qp_rem );

        }
*/
    for( i=0; i<2; i++)
    {
        for( qp_rem=0; qp_rem<6; qp_rem++ )
        {
            ippiGenScaleLevel8x8_H264_8u16s_D2(core_enc->m_SeqParamSet.seq_scaling_list_8x8[i],
                                    8,
                                    core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[i][qp_rem],
                                    core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[i][qp_rem],
                                    qp_rem);
        }
    }

    //VUI parameters
    core_enc->m_SeqParamSet.vui_parameters_present_flag = 1;
    core_enc->m_SeqParamSet.vui_parameters.aspect_ratio_info_present_flag = 1;
    core_enc->m_SeqParamSet.vui_parameters.aspect_ratio_idc = 1;
    if(core_enc->m_params.m_info.fFramerate != 0)
    {
        core_enc->m_SeqParamSet.vui_parameters.timing_info_present_flag = 1;
        core_enc->m_SeqParamSet.vui_parameters.num_units_in_tick = 1;
        core_enc->m_SeqParamSet.vui_parameters.time_scale =
        2*core_enc->m_SeqParamSet.vui_parameters.num_units_in_tick * (Ipp32u)core_enc->m_params.m_info.fFramerate;
        core_enc->m_SeqParamSet.vui_parameters.fixed_frame_rate_flag = 1;
    }

} // SetSequenceParameters

/*************************************************************
 *  Name: SetPictureParameters
 *  Description:  Fill in the Picture Parameter Set for this
 *  sequence.  Can only change at an IDR picture.
 *************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_SetPictureParameters(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    core_enc->m_PicParamSet.pic_parameter_set_id = 0;

    // Assumes that there is only one sequence param set to choose from
    core_enc->m_PicParamSet.seq_parameter_set_id = core_enc->m_SeqParamSet.seq_parameter_set_id;

    core_enc->m_PicParamSet.entropy_coding_mode = core_enc->m_params.entropy_coding_mode_flag;

    core_enc->m_PicParamSet.pic_order_present_flag = (core_enc->m_SeqParamSet.frame_mbs_only_flag == 0);

    core_enc->m_PicParamSet.weighted_pred_flag = core_enc->m_params.weighted_pred_flag;

    // We use implicit weighted prediction (2) when B frame rate can
    // benefit from it.  When B_Frame_rate == 0 or 1, it doesn't matter,
    // so we do what is faster (0).

    core_enc->m_PicParamSet.weighted_bipred_idc = core_enc->m_params.weighted_bipred_idc;

    // Default to P frame constant quality at time of an IDR
    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR ||
        core_enc->m_params.rate_controls.method == H264_RCM_CBR ||
        core_enc->m_params.rate_controls.method == H264_RCM_VBR_SLICE ||
        core_enc->m_params.rate_controls.method == H264_RCM_CBR_SLICE
        )
    {
        //core_enc->m_PicParamSet.pic_init_qp = (Ipp8s)H264_AVBR_GetQP(&core_enc->avbr, INTRAPIC);
        core_enc->m_PicParamSet.pic_init_qp = 26;
    }
    else if(core_enc->m_params.rate_controls.method == H264_RCM_QUANT)
        core_enc->m_PicParamSet.pic_init_qp = core_enc->m_params.rate_controls.quantP;

    core_enc->m_PicParamSet.pic_init_qs = 26;     // Not used

    core_enc->m_PicParamSet.chroma_qp_index_offset = 0;
    core_enc->m_PicParamSet.deblocking_filter_variables_present_flag = 1;

    core_enc->m_PicParamSet.constrained_intra_pred_flag = 0;

    // We don't do redundant slices...
    core_enc->m_PicParamSet.redundant_pic_cnt_present_flag = 0;
    core_enc->m_PicParamSet.pic_scaling_matrix_present_flag = 0;
    core_enc->m_PicParamSet.transform_8x8_mode_flag = false;
    // In the future, if flexible macroblock ordering is
    // desired, then a macroblock allocation map will need
    // to be coded and the value below updated accordingly.
    core_enc->m_PicParamSet.num_slice_groups = 1;     // Hard coded for now
    core_enc->m_PicParamSet.SliceGroupInfo.slice_group_map_type = 0;
    core_enc->m_PicParamSet.SliceGroupInfo.t3.pic_size_in_map_units = 0;
    core_enc->m_PicParamSet.SliceGroupInfo.t3.pSliceGroupIDMap = NULL;

    // I guess these need to be 1 or greater since they are written as "minus1".
    core_enc->m_PicParamSet.num_ref_idx_l0_active = 1;
    core_enc->m_PicParamSet.num_ref_idx_l1_active = 1;

    core_enc->m_PicParamSet.transform_8x8_mode_flag = core_enc->m_params.transform_8x8_mode_flag;

} // SetPictureParameters

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_UpdateRefPicListCommon(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264EncoderFrame<PIXTYPE> *pFrm;
    H264EncoderFrame<PIXTYPE> *pHead = core_enc->m_dpb.m_pHead;

    Ipp32u uMaxFrameNum = (1<<core_enc->m_SeqParamSet.log2_max_frame_num);
    //Ipp32u uMaxPicNum = (core_enc->m_SliceHeader.field_pic_flag == 0) ? uMaxFrameNum : uMaxFrameNum<<1;

    for (pFrm = pHead; pFrm; pFrm = pFrm->m_pFutureFrame)
    {
        // update FrameNumWrap and PicNum if frame number wrap occurred,
        // for short-term frames
        // TBD: modify for fields
        H264EncoderFrame_UpdateFrameNumWrap(
            pFrm,
            (Ipp32s)core_enc->m_SliceHeader.frame_num,
            uMaxFrameNum,
            core_enc->m_pCurrentFrame->m_PictureStructureForRef+
            core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index]);

        // For long-term references, update LongTermPicNum. Note this
        // could be done when LongTermFrameIdx is set, but this would
        // only work for frames, not fields.
        // TBD: modify for fields
        H264EncoderFrame_UpdateLongTermPicNum(
            pFrm,
            core_enc->m_pCurrentFrame->m_PictureStructureForRef +
                core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index]);
    }
}

//////////////////////////////////////////////////////////////////////////////
// updateRefPicMarking
//  Called at the completion of decoding a frame to update the marking of the
//  reference pictures in the decoded frames buffer.
//////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_UpdateRefPicMarking(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status ps = UMC_OK;
    Ipp32u arpmmf_idx;
    Ipp32s PicNum;
    Ipp32s LongTermFrameIdx;
    bool bCurrentisST = true;
    Ipp32u NumShortTermRefs, NumLongTermRefs;

    if (core_enc->m_pCurrentFrame->m_bIsIDRPic)
    {
        // mark all reference pictures as unused
        H264EncoderFrameList_removeAllRef(&core_enc->m_dpb);
        H264EncoderFrameList_IncreaseRefPicListResetCount(&core_enc->m_dpb, core_enc->m_pCurrentFrame);

        if (core_enc->m_SliceHeader.long_term_reference_flag)
        {
            H264EncoderFrame_SetisLongTermRef(core_enc->m_pCurrentFrame, core_enc->m_field_index);
            core_enc->m_MaxLongTermFrameIdx = 0;
        }
        else
        {
            H264EncoderFrame_SetisShortTermRef(core_enc->m_pCurrentFrame, core_enc->m_field_index);
            core_enc->m_MaxLongTermFrameIdx = -1;        // no long term frame indices
        }
    }
    else
    {
        Ipp32s LastLongTermFrameIdx = -1;
        // not IDR picture
        if (core_enc->m_SliceHeader.adaptive_ref_pic_marking_mode_flag == 0)
        {
            // sliding window ref pic marking
            // find out how many active reference frames currently in decoded frames buffer
            H264EncoderFrameList_countActiveRefs(
                &core_enc->m_dpb,
                NumShortTermRefs,
                NumLongTermRefs);
            if (((NumShortTermRefs + NumLongTermRefs) >= (Ipp32u)core_enc->m_SeqParamSet.num_ref_frames) && !core_enc->m_field_index )
            {
                // mark oldest short term reference as unused
                VM_ASSERT(NumShortTermRefs > 0);
                H264EncoderFrameList_freeOldestShortTermRef(&core_enc->m_dpb);
            }
        }
        else
        {
            // adaptive ref pic marking
            if (core_enc->m_AdaptiveMarkingInfo.num_entries > 0)
            {
                for (arpmmf_idx=0; arpmmf_idx<core_enc->m_AdaptiveMarkingInfo.num_entries; arpmmf_idx++)
                {
                    switch (core_enc->m_AdaptiveMarkingInfo.mmco[arpmmf_idx])
                    {
                        case 1:
                            // mark a short-term picture as unused for reference
                            // Value is difference_of_pic_nums_minus1
                            PicNum = H264EncoderFrame_PicNum(
                                core_enc->m_pCurrentFrame,
                                core_enc->m_field_index,
                                0) - (core_enc->m_AdaptiveMarkingInfo.value[arpmmf_idx * 2] + 1);
                            H264EncoderFrameList_freeShortTermRef(
                                &core_enc->m_dpb,
                                PicNum);
                            break;
                        case 2:
                            // mark a long-term picture as unused for reference
                            // value is long_term_pic_num
                            PicNum = core_enc->m_AdaptiveMarkingInfo.value[arpmmf_idx*2];
                            H264EncoderFrameList_freeLongTermRef(
                                &core_enc->m_dpb,
                                PicNum);
                            break;
                        case 3:
                            // Assign a long-term frame idx to a short-term picture
                            // Value is difference_of_pic_nums_minus1 followed by
                            // long_term_frame_idx. Only this case uses 2 value entries.
                            PicNum = H264EncoderFrame_PicNum(core_enc->m_pCurrentFrame, core_enc->m_field_index, 0) - (core_enc->m_AdaptiveMarkingInfo.value[arpmmf_idx*2] + 1);
                            LongTermFrameIdx = core_enc->m_AdaptiveMarkingInfo.value[arpmmf_idx*2+1];

                            // First free any existing LT reference with the LT idx
                            if (LastLongTermFrameIdx !=LongTermFrameIdx) //this is needed since both fields may have equal Idx
                                H264EncoderFrameList_freeLongTermRefIdx(&core_enc->m_dpb, LongTermFrameIdx);

                            H264EncoderFrameList_changeSTtoLTRef(&core_enc->m_dpb, PicNum, LongTermFrameIdx);
                            LastLongTermFrameIdx = LongTermFrameIdx;
                            break;
                        case 4:
                            // Specify IPP_MAX long term frame idx
                            // Value is max_long_term_frame_idx_plus1
                            // Set to "no long-term frame indices" (-1) when value == 0.
                            core_enc->m_MaxLongTermFrameIdx = core_enc->m_AdaptiveMarkingInfo.value[arpmmf_idx*2] - 1;

                            // Mark any long-term reference frames with a larger LT idx
                            // as unused for reference.
                            H264EncoderFrameList_freeOldLongTermRef(
                                &core_enc->m_dpb,
                                core_enc->m_MaxLongTermFrameIdx);
                            break;
                        case 5:
                            // Mark all as unused for reference
                            // no value
                            H264EncoderFrameList_removeAllRef(&core_enc->m_dpb);
                            H264EncoderFrameList_IncreaseRefPicListResetCount<PIXTYPE>(&core_enc->m_dpb, NULL);
                            core_enc->m_MaxLongTermFrameIdx = -1;        // no long term frame indices
                            // set "previous" picture order count vars for future
                            //m_PicOrderCntMsb = 0;
                            //m_PicOrderCntLsb = 0;
                            core_enc->m_FrameNumOffset = 0;
                            core_enc->m_FrameNum = 0;
                            // set frame_num to zero for this picture, for correct
                            // FrameNumWrap
                            core_enc->m_pCurrentFrame->m_FrameNum = 0;
                            core_enc->m_pCurrentFrame->m_FrameNum = 0;
                            // set POC to zero for this picture, for correct display order
                            //m_pCurrentFrame->setPicOrderCnt(core_enc->m_PicOrderCnt,0);
                            //m_pCurrentFrame->setPicOrderCnt(core_enc->m_PicOrderCnt,1);
                            break;
                        case 6:
                            // Assign long term frame idx to current picture
                            // Value is long_term_frame_idx
                            LongTermFrameIdx = core_enc->m_AdaptiveMarkingInfo.value[arpmmf_idx*2];

                            // First free any existing LT reference with the LT idx
                            H264EncoderFrameList_freeLongTermRefIdx(&core_enc->m_dpb, LongTermFrameIdx);

                            // Mark current
                            H264EncoderFrame_SetisLongTermRef(core_enc->m_pCurrentFrame, core_enc->m_field_index);
                            core_enc->m_pCurrentFrame->m_LongTermFrameIdx = LongTermFrameIdx;
                            bCurrentisST = false;
                            break;
                        case 0:
                        default:
                            // invalid mmco command in bitstream
                            VM_ASSERT(0);
                            ps = UMC_ERR_INVALID_STREAM;
                    }    // switch
                }    // for arpmmf_idx
            }
        }    // adaptive ref pic marking
    }    // not IDR picture

    if (bCurrentisST)   // set current as short term
        H264EncoderFrame_SetisShortTermRef(core_enc->m_pCurrentFrame, core_enc->m_field_index);

    return ps;
}    // updateRefPicMarking

//#include "vm_time.h"
//extern vm_tick t_slices[10000][8];
//extern int frame_count;
//
// CompressFrame
//
#define FRAME_SIZE_RATIO_THRESH_MAX 16.0
#define FRAME_SIZE_RATIO_THRESH_MIN 8.0
#define TOTAL_SIZE_RATIO_THRESH 1.5

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_CompressFrame(void* state, EnumPicCodType& ePictureType, EnumPicClass& ePic_Class, MediaData* dst)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status status = UMC_OK;
    Ipp32s slice;
    bool bufferOverflowFlag = false;
    bool buffersNotFull = true;
    Ipp32s bitsPerFrame = 0;
    bool brcRecode = false;
    EnumPicClass firstFieldClass;
    Ipp32s i;

    Ipp32s numSliceMB;
    Ipp32s data_size = (Ipp32s)dst->GetDataSize();

    if (core_enc->m_Analyse & ANALYSE_RECODE_FRAME /*&& core_enc->m_pReconstructFrame == NULL //could be size change*/) //make init reconstruct buffer
    {
        core_enc->m_pReconstructFrame = (H264EncoderFrame<PIXTYPE>*)H264_Malloc(sizeof(H264EncoderFrame<PIXTYPE>));
        if (!core_enc->m_pReconstructFrame)
            return UMC_ERR_ALLOC;

        H264EncoderFrame_Create<PIXTYPE>(core_enc->m_pReconstructFrame, &core_enc->m_pCurrentFrame->m_data, core_enc->memAlloc, core_enc->m_SeqParamSet.aux_format_idc, 0);
        if (H264EncoderFrame_allocate<PIXTYPE>(core_enc->m_pReconstructFrame, core_enc->m_PaddedSize, core_enc->m_params.num_slices))
            return UMC_ERR_ALLOC;

        core_enc->m_pReconstructFrame->m_bottom_field_flag[0] = core_enc->m_pCurrentFrame->m_bottom_field_flag[0];
        core_enc->m_pReconstructFrame->m_bottom_field_flag[1] = core_enc->m_pCurrentFrame->m_bottom_field_flag[1];
    }

    // recode frame loop
    for(;;)
    {
        brcRecode = false;
        core_enc->m_is_cur_pic_afrm = (Ipp32s)(core_enc->m_pCurrentFrame->m_PictureStructureForDec==AFRM_STRUCTURE);
        firstFieldClass = ePic_Class;

        for(core_enc->m_field_index = 0; core_enc->m_field_index <= (Ipp8u)(core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE); core_enc->m_field_index++)
        {
            core_enc->m_NeedToCheckMBSliceEdges = (core_enc->m_params.num_slices > 1 || core_enc->m_field_index > 0);
            EnumSliceType default_slice_type = INTRASLICE;
            bool startPicture = true;
            numSliceMB = 0;
            bool alpha = true; // Is changed to the opposite at the beginning

            do // First iteration primary picture, second -- alpha (if present)
            {
                alpha = !alpha;
                if(!alpha)
                {
                    if (ePic_Class == IDR_PIC)
                    {
                        if (core_enc->m_field_index)
                            ePic_Class = REFERENCE_PIC;
                        else
                        {
                            //if (core_enc->m_uFrames_Num == 0) //temoporaly disabled
                            {
                                H264CoreEncoder_SetSequenceParameters<COEFFSTYPE, PIXTYPE>(state);
                                H264CoreEncoder_SetPictureParameters<COEFFSTYPE, PIXTYPE>(state);
                            }
                            // Toggle the idr_pic_id on and off so that adjacent IDRs will have different values
                            // This is done here because it is done per frame and not per slice.
    //FPV                            core_enc->m_SliceHeader.idr_pic_id ^= 0x1;
                            core_enc->m_SliceHeader.idr_pic_id++;
                            core_enc->m_SliceHeader.idr_pic_id &= 0xff; //Restrict to 255 to reduce number of bits(max value 65535 in standard)
                        }
                    }
                    core_enc->m_PicType = ePictureType;
                    default_slice_type = (core_enc->m_PicType == PREDPIC) ? PREDSLICE : (core_enc->m_PicType == INTRAPIC) ? INTRASLICE : BPREDSLICE;
                    core_enc->m_PicParamSet.chroma_format_idc = core_enc->m_SeqParamSet.chroma_format_idc;
                    core_enc->m_PicParamSet.bit_depth_luma = core_enc->m_SeqParamSet.bit_depth_luma;
                }
                else
                {
                    H264EncoderFrameList_switchToAuxiliary(&core_enc->m_cpb);
                    H264EncoderFrameList_switchToAuxiliary(&core_enc->m_dpb);
                    core_enc->m_PicParamSet.chroma_format_idc = 0;
                    core_enc->m_PicParamSet.bit_depth_luma = core_enc->m_SeqParamSet.bit_depth_aux;
                }
                if(!(core_enc->m_Analyse & ANALYSE_RECODE_FRAME))
                    core_enc->m_pReconstructFrame = core_enc->m_pCurrentFrame;
                // reset bitstream object before begin compression

                for (i = 0; i < core_enc->m_params.num_slices*((core_enc->m_params.coding_type == 1) + 1); i++)   //TODO fix for PicAFF/AFRM
                    H264BsReal_Reset(core_enc->m_pbitstreams[i]);

                if(!alpha)
                {
                    H264CoreEncoder_SetSliceHeaderCommon<COEFFSTYPE, PIXTYPE>(state, core_enc->m_pCurrentFrame);
                    if( default_slice_type == BPREDSLICE )
                    {
                        if( core_enc->m_Analyse & ANALYSE_ME_AUTO_DIRECT)
                        {
                            if( core_enc->m_SliceHeader.direct_spatial_mv_pred_flag )
                                core_enc->m_SliceHeader.direct_spatial_mv_pred_flag = core_enc->m_DirectTypeStat[0] > ((545*core_enc->m_DirectTypeStat[1])>>9) ? 0:1;
                            else
                                core_enc->m_SliceHeader.direct_spatial_mv_pred_flag = core_enc->m_DirectTypeStat[1] > ((545*core_enc->m_DirectTypeStat[0])>>9) ? 1:0;
                            core_enc->m_DirectTypeStat[0]=core_enc->m_DirectTypeStat[1]=0;
                        }
                        else
                            core_enc->m_SliceHeader.direct_spatial_mv_pred_flag = core_enc->m_params.direct_pred_mode & 0x1;
                    }
                    status = H264CoreEncoder_encodeFrameHeader<COEFFSTYPE, PIXTYPE>(state, core_enc->m_bs1, dst, (ePic_Class == IDR_PIC), startPicture);
                    if (status != UMC_OK)
                        goto done;
                }

                status = H264CoreEncoder_Start_Picture<COEFFSTYPE, PIXTYPE>(state, &ePic_Class, ePictureType);
                if (status != UMC_OK)
                    goto done;
                Ipp32s slice_qp_delta_default = core_enc->m_Slices[0].m_slice_qp_delta;
                H264CoreEncoder_UpdateRefPicListCommon<COEFFSTYPE, PIXTYPE>(state);

#if defined (_OPENMP)
                vm_thread_priority mainTreadPriority = vm_get_current_thread_priority();
#pragma omp parallel for private(slice)
#endif
                for (slice = (Ipp32s)core_enc->m_params.num_slices*core_enc->m_field_index; slice < core_enc->m_params.num_slices*(core_enc->m_field_index+1); slice++)
                {
#if defined (_OPENMP)
                    vm_set_current_thread_priority(mainTreadPriority);
#endif
//                    t_slices[frame_count][slice - core_enc->m_params.num_slices * core_enc->m_field_index] = vm_time_get_tick();
                    core_enc->m_Slices[slice].m_slice_qp_delta = (Ipp8s)slice_qp_delta_default;
                    core_enc->m_Slices[slice].m_slice_number = slice;
                    core_enc->m_Slices[slice].m_slice_type = default_slice_type; // Pass to core encoder
                    H264CoreEncoder_UpdateRefPicList(state, core_enc->m_Slices + slice, &core_enc->m_pCurrentFrame->m_pRefPicList[slice], core_enc->m_SliceHeader, &core_enc->m_ReorderInfoL0, &core_enc->m_ReorderInfoL1);
                    //if (core_enc->m_SliceHeader.MbaffFrameFlag)
                    //    H264CoreEncoder_UpdateRefPicList(state, &core_enc->m_pRefPicList[slice], core_enc->m_SliceHeader, &core_enc->m_ReorderInfoL0, &core_enc->m_ReorderInfoL1);
re_encode_slice:
                    Ipp32s slice_bits = H264BsBase_GetBsOffset(core_enc->m_Slices[slice].m_pbitstream);
                    EnumPicCodType pic_type = INTRAPIC;
                    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR_SLICE || core_enc->m_params.rate_controls.method == H264_RCM_CBR_SLICE)
                    {
                        pic_type = (core_enc->m_Slices[slice].m_slice_type == INTRASLICE) ? INTRAPIC : (core_enc->m_Slices[slice].m_slice_type == PREDSLICE) ? PREDPIC : BPREDPIC;
                        core_enc->m_Slices[slice].m_slice_qp_delta = (Ipp8s)(H264_AVBR_GetQP(&core_enc->avbr, pic_type) - core_enc->m_PicParamSet.pic_init_qp);
                        core_enc->m_Slices[slice].m_iLastXmittedQP = core_enc->m_PicParamSet.pic_init_qp + core_enc->m_Slices[slice].m_slice_qp_delta;
                    }
                    // Compress one slice
                    if (core_enc->m_is_cur_pic_afrm)
                        core_enc->m_Slices[slice].status = H264CoreEncoder_Compress_Slice_MBAFF(state, core_enc->m_Slices + slice);
                    else
                    {
                        core_enc->m_Slices[slice].status = H264CoreEncoder_Compress_Slice(state, core_enc->m_Slices + slice, core_enc->m_Slices[slice].m_slice_number == core_enc->m_params.num_slices*core_enc->m_field_index);
                        if( core_enc->m_MaxSliceSize)
                        {
                            Ipp32s numMBs = core_enc->m_HeightInMBs*core_enc->m_WidthInMBs;
                            numSliceMB += core_enc->m_Slices[slice].m_MB_Counter;
                            dst->SetDataSize(dst->GetDataSize() + H264BsReal_EndOfNAL(core_enc->m_Slices[slice].m_pbitstream, (Ipp8u*)dst->GetDataPointer() + dst->GetDataSize(), (ePic_Class != DISPOSABLE_PIC),
                                (alpha) ? NAL_UT_LAYERNOPART :
                                ((ePic_Class == IDR_PIC) ? NAL_UT_IDR_SLICE : NAL_UT_SLICE), startPicture));
                            if (numSliceMB != numMBs)
                            {
                                core_enc->m_NeedToCheckMBSliceEdges = true;
                                H264BsReal_Reset(core_enc->m_Slices[slice].m_pbitstream);
                                core_enc->m_Slices[slice].m_slice_number++;
                                goto re_encode_slice;
                            }
                            else
                                core_enc->m_Slices[slice].m_slice_number = slice;
                        }
                    }

                    slice_bits = H264BsBase_GetBsOffset(core_enc->m_Slices[slice].m_pbitstream) - slice_bits;
                    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR_SLICE || core_enc->m_params.rate_controls.method == H264_RCM_CBR_SLICE)
                        H264_AVBR_PostFrame(&core_enc->avbr, pic_type, slice_bits);

    //                t_slices[frame_count][slice - core_enc->m_params.num_slices * core_enc->m_field_index] =
    //                    vm_time_get_tick() - t_slices[frame_count][slice - core_enc->m_params.num_slices * core_enc->m_field_index];
                }

#ifdef SLICE_THREADING_LOAD_BALANCING
                Ipp64s ticks_total = 0;
                for (slice = (Ipp32s)core_enc->m_params.num_slices*core_enc->m_field_index; slice < core_enc->m_params.num_slices*(core_enc->m_field_index+1); slice++)
                    ticks_total += core_enc->m_Slices[slice].m_ticks_per_slice;
                if (core_enc->m_pCurrentFrame->m_PicCodType == INTRAPIC)
                {
                    core_enc->m_B_ticks_data_available = 0;
                    core_enc->m_P_ticks_data_available = 0;
                    core_enc->m_P_ticks_per_frame = ticks_total;
                    core_enc->m_B_ticks_per_frame = ticks_total;
                }
                else if (core_enc->m_pCurrentFrame->m_PicCodType == PREDPIC)
                {
                    core_enc->m_P_ticks_data_available = 1;
                    core_enc->m_P_ticks_per_frame = ticks_total;
                }
                else
                {
                    core_enc->m_B_ticks_data_available = 1;
                    core_enc->m_B_ticks_per_frame = ticks_total;
                }
#endif

                //Write slice to the stream in order, copy Slice RBSP to the end of the output buffer after adding start codes and SC emulation prevention.
                if(!core_enc->m_MaxSliceSize)
                {
                    for (slice = (Ipp32s)core_enc->m_params.num_slices*core_enc->m_field_index; slice < core_enc->m_params.num_slices*(core_enc->m_field_index+1); slice++)
                    {
                        if(dst->GetDataSize() + (Ipp32s)H264BsBase_GetBsSize(core_enc->m_Slices[slice].m_pbitstream) + 5 /* possible extra bytes */ > dst->GetBufferSize())
                            bufferOverflowFlag = true;
                        else
                        {   //Write to output bitstream
                            dst->SetDataSize(dst->GetDataSize() + H264BsReal_EndOfNAL(core_enc->m_Slices[slice].m_pbitstream, (Ipp8u*)dst->GetDataPointer() + dst->GetDataSize(), (ePic_Class != DISPOSABLE_PIC),
                                (alpha) ? NAL_UT_LAYERNOPART :
                                ((ePic_Class == IDR_PIC) ? NAL_UT_IDR_SLICE : NAL_UT_SLICE), startPicture));
                        }
                        buffersNotFull = buffersNotFull && H264BsBase_CheckBsLimit(core_enc->m_Slices[slice].m_pbitstream);
                    }
                    if (bufferOverflowFlag)
                    {
                        if(core_enc->m_Analyse & ANALYSE_RECODE_FRAME)
                            goto recode_check; //Output buffer overflow
                        else
                        {
                            status = UMC_ERR_NOT_ENOUGH_BUFFER;
                            goto done;
                        }
                    }
                    // check for buffer overrun on some of slices
                    if (!buffersNotFull )
                    {
                        if( core_enc->m_Analyse & ANALYSE_RECODE_FRAME )
                            goto recode_check;
                        else
                        {
                            status = H264CoreEncoder_EncodeDummyFrame<COEFFSTYPE, PIXTYPE>(state, dst);
                            core_enc->m_bMakeNextFrameKey = true;
                            if (status == UMC_OK)
                                goto end_of_frame;
                            else
                                goto done;
                        }
                    }

                    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR || core_enc->m_params.rate_controls.method == H264_RCM_CBR)
                    {
                        bitsPerFrame = (Ipp32s)((dst->GetDataSize() - data_size) << 3);
                        if (core_enc->m_Analyse & ANALYSE_RECODE_FRAME)
                        {
                            Ipp64s totEncoded, totTarget;
                            totEncoded = core_enc->avbr.mBitsEncodedTotal + bitsPerFrame;
                            totTarget = core_enc->avbr.mBitsDesiredTotal + core_enc->avbr.mBitsDesiredFrame;
                            Ipp64f thratio, thratio_tot = (Ipp64f)((totEncoded - totTarget) / totTarget);
                            thratio = FRAME_SIZE_RATIO_THRESH_MAX * (1.0 - thratio_tot);
                            h264_Clip(thratio, FRAME_SIZE_RATIO_THRESH_MIN, FRAME_SIZE_RATIO_THRESH_MAX);

                            if (bitsPerFrame > core_enc->avbr.mBitsDesiredFrame * thratio)
                            {
                                Ipp32s qp = H264_AVBR_GetQP(&core_enc->avbr, core_enc->m_PicType);
                                if (qp < 51)
                                {
                                    Ipp32s qp_new = (Ipp32s)(qp * sqrt((Ipp64f)bitsPerFrame / (thratio * core_enc->avbr.mBitsDesiredFrame)));
                                    if (qp_new <= qp)
                                        qp_new++;
                                    h264_Clip(qp_new, 1, 51);
                                    H264_AVBR_SetQP(&core_enc->avbr, core_enc->m_PicType, qp_new);
                                    brcRecode = true;
                                    goto recode_check;
                                }
                            }
                        }
                        H264_AVBR_PostFrame(&core_enc->avbr, core_enc->m_PicType, bitsPerFrame);
                    }
                }
                else
                {
                    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR || core_enc->m_params.rate_controls.method == H264_RCM_CBR)
                    {
                        bitsPerFrame = (Ipp32s)((dst->GetDataSize() - data_size) << 3);
                        H264_AVBR_PostFrame(&core_enc->avbr, core_enc->m_PicType, bitsPerFrame);
                    }
                }

                //Deblocking all frame/field
#ifdef DEBLOCK_THREADING
                if( ePic_Class != DISPOSABLE_PIC )
                {
                    //Current we assume the same slice type for all slices
                    H264CoreEncoder_DeblockingFunction pDeblocking = NULL;
                    switch (default_slice_type){
                        case INTRASLICE:
                            pDeblocking = &H264CoreEncoder_DeblockMacroblockISlice<COEFFSTYPE, PIXTYPE>;
                            break;
                        case PREDSLICE:
                            pDeblocking = &H264CoreEncoder_DeblockMacroblockPSlice<COEFFSTYPE, PIXTYPE>;
                            break;
                        case BPREDSLICE:
                            pDeblocking = &H264CoreEncoder_DeblockMacroblockBSlice<COEFFSTYPE, PIXTYPE>;
                            break;
                        default:
                            pDeblocking = NULL;
                            break;
                    }

                    Ipp32s mbstr;
                    //Reset table
#pragma omp parallel for private(mbstr)
                    for(mbstr = 0; mbstr < core_enc->m_HeightInMBs; mbstr++ )
                        memset( (void*)(core_enc->mbs_deblock_ready + mbstr*core_enc->m_WidthInMBs), 0, sizeof(Ipp8u)*core_enc->m_WidthInMBs);
#pragma omp parallel for private(mbstr) schedule(static,1)
                    for( mbstr=0; mbstr<core_enc->m_HeightInMBs; mbstr++ )
                    {
                        //Lock string
                        Ipp32s first_mb = mbstr*core_enc->m_WidthInMBs;
                        Ipp32s last_mb = first_mb + core_enc->m_WidthInMBs;
                        for(Ipp32s mb = first_mb; mb < last_mb; mb++ )
                        {
                            //Check up mb is ready
                            Ipp32s upmb = mb - core_enc->m_WidthInMBs + 1;
                            if( mb == last_mb-1 ) upmb = mb - core_enc->m_WidthInMBs;
                            if( upmb >= 0 )
                                while( *(core_enc->mbs_deblock_ready + upmb) == 0 );
                             (*pDeblocking)(state, mb);
                             //Unlock ready MB
                             *(core_enc->mbs_deblock_ready + mb) = 1;
                        }
                    }
                }
#else
                for (slice = (Ipp32s)core_enc->m_params.num_slices*core_enc->m_field_index; slice < core_enc->m_params.num_slices*(core_enc->m_field_index+1); slice++)
                {
                    if (core_enc->m_Slices[slice].status != UMC_OK)
                    {
                        // It is unreachable in the current implementation, so there is no problem!!!
                        core_enc->m_bMakeNextFrameKey = true;
                        VM_ASSERT(0);// goto done;
                    }
                    else if(ePic_Class != DISPOSABLE_PIC)
                    {
                        H264CoreEncoder_DeblockSlice(state, core_enc->m_Slices + slice, core_enc->m_Slices[slice].m_first_mb_in_slice + core_enc->m_WidthInMBs*core_enc->m_HeightInMBs*core_enc->m_field_index, core_enc->m_Slices[slice].m_MB_Counter);
                    }
                }
#endif
end_of_frame:
                if (ePic_Class != DISPOSABLE_PIC)
                    H264CoreEncoder_End_Picture<COEFFSTYPE, PIXTYPE>(state);
                core_enc->m_HeightInMBs <<= (Ipp8u)(core_enc->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE); //Do we need it here?

            } while(!alpha && core_enc->m_SeqParamSet.aux_format_idc );

            if(alpha)
            {
//                core_enc->m_pCurrentFrame->usePrimary();
                H264EncoderFrameList_switchToPrimary<PIXTYPE>(&core_enc->m_cpb);
                H264EncoderFrameList_switchToPrimary<PIXTYPE>(&core_enc->m_dpb);
            }
//            for(slice = 0; slice < core_enc->m_params.num_slices*((core_enc->m_params.coding_type == 1) + 1); slice++)  //TODO fix for PicAFF/AFRM
//                bitsPerFrame += core_enc->m_pbitstreams[slice]->GetBsOffset();
            if (ePic_Class != DISPOSABLE_PIC)
                H264CoreEncoder_UpdateRefPicMarking<COEFFSTYPE, PIXTYPE>(state);
        }

recode_check:
        if( (core_enc->m_Analyse & ANALYSE_RECODE_FRAME) && ( bufferOverflowFlag || !buffersNotFull || brcRecode) ) //check frame size to be less than output buffer otherwise change qp and recode frame
        {
            if (!brcRecode)
            {
                Ipp32s qp = H264_AVBR_GetQP(&core_enc->avbr, core_enc->m_PicType);
                if( qp == 51 )
                {
                    status = UMC_ERR_NOT_ENOUGH_BUFFER;
                    goto done;
                }
                H264_AVBR_SetQP(&core_enc->avbr, core_enc->m_PicType, ++qp);
            }
            bufferOverflowFlag = false;
            buffersNotFull = true;
            brcRecode = false;
            if(core_enc->m_field_index)
            {
                if(firstFieldClass == IDR_PIC) // restore IDR flag or SPS header will be lost
                {
                    core_enc->m_SliceHeader.idr_pic_id--;
                    ePic_Class = IDR_PIC;
                }
                core_enc->m_HeightInMBs <<= 1;
            }
            else
            {
                if(ePic_Class == IDR_PIC)
                {
                    core_enc->m_SliceHeader.idr_pic_id--;
                    core_enc->m_SliceHeader.idr_pic_id &= 0xff; //Restrict to 255 to reduce number of bits(max value 65535 in standard)
                }
            }
            dst->SetDataSize(data_size);
        }
        else
            break;
    }
    bitsPerFrame = (Ipp32s)((dst->GetDataSize() - data_size) << 3);
    core_enc->m_total_bits_encoded += bitsPerFrame;

/*
    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR || core_enc->m_params.rate_controls.method == H264_RCM_CBR)
        H264_AVBR_PostFrame(&core_enc->avbr, core_enc->m_PicType, bitsPerFrame);
*/
    if( core_enc->m_Analyse & ANALYSE_RECODE_FRAME )
    {
        H264EncoderFrame_exchangeFrameYUVPointers(core_enc->m_pReconstructFrame, core_enc->m_pCurrentFrame);
        if(core_enc->m_pReconstructFrame)
        {
            H264EncoderFrame_Destroy<PIXTYPE>(core_enc->m_pReconstructFrame);
            H264_Free(core_enc->m_pReconstructFrame);
            core_enc->m_pReconstructFrame = NULL;

        }
    }
    if (dst->GetDataSize() == 0)
    {
        core_enc->m_bMakeNextFrameKey = true;
        status = UMC_ERR_FAILED;
        goto done;
    }
    core_enc->m_uFrames_Num++;

done:
    return status;
}

// ===========================================================================
//
// Apply the "Profile Rules" to this profile frame type
//
//
//      Option         Single Layer
// ------------   -------------------------
// Progressive
//      Stills
//                 Purge frames in the
//                 Ready and Wait queues.
//                 Reset the profile index.
//
//                -------------------------
// Key frame
//  interval       Wait for next 'P' in the
//                 profile to make an I frame
//                 Do process intervening B
//                 frames in the profile.
//                 No profile index reset.
//                 The key frame interval
//                 counter restarts after
//                 the key frame is sent.
//                -------------------------
// Forced Key
//      Frame      Regardless of the profile
//                 frame type; purge the
//                 the Ready queue.
//                 If destuctive key frame request
//                 then purge the Wait queue
//                 Reset profile index.
//
//                -------------------------
// switch B
// frame to P      If this profile frame type
//                 indicates a 'B' it will be
//                 en-queued, encoded and
//                 emitted as a P frame. Prior
//                 frames (P|B) in the queue
//                 are correctly emitted using
//                 the (new) P as a ref frame
//
// ------------   -------------------------
template<typename COEFFSTYPE, typename PIXTYPE>
EnumPicCodType H264CoreEncoder_DetermineFrameType(void* state, Ipp32s)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    EnumPicCodType ePictureType = core_enc->eFrameType[core_enc->m_iProfileIndex];

    if (core_enc->m_uFrames_Num == 0)
    {
        core_enc->m_iProfileIndex++;
        if (core_enc->m_iProfileIndex == core_enc->profile_frequency)
            core_enc->m_iProfileIndex = 0;

        core_enc->m_bMakeNextFrameIDR = true;
        core_enc->m_bMakeNextFrameKey = false;
        return INTRAPIC;
    }

    core_enc->m_uIntraFrameInterval--;
    if (0 == core_enc->m_uIntraFrameInterval)
        core_enc->m_bMakeNextFrameKey = true;

    // change to an I frame
    if (core_enc->m_bMakeNextFrameKey)
    {
        ePictureType = INTRAPIC;

        core_enc->m_uIntraFrameInterval = core_enc->m_params.key_interval + 1;
        core_enc->m_uIDRFrameInterval--;
        if (0 == core_enc->m_uIDRFrameInterval)
        {
            if (core_enc->m_iProfileIndex != 1 && core_enc->profile_frequency > 1) // if sequence PBBB was completed
            {   // no
                ePictureType = PREDPIC;
                core_enc->m_uIDRFrameInterval = 1;
                core_enc->m_uIntraFrameInterval = 1;
                core_enc->m_l1_cnt_to_start_B = 1;
            }
            else
            {
                core_enc->m_bMakeNextFrameIDR = true;
                core_enc->m_uIDRFrameInterval = core_enc->m_params.idr_interval + 1;
                core_enc->m_iProfileIndex = 0;
            }
        }
        else
        {
            if (!core_enc->m_params.m_do_weak_forced_key_frames)
                core_enc->m_iProfileIndex = 0;
        }

        core_enc->m_bMakeNextFrameKey = false;
    }

    core_enc->m_iProfileIndex++;
    if (core_enc->m_iProfileIndex == core_enc->profile_frequency)
        core_enc->m_iProfileIndex = 0;

    return ePictureType;
}

//
// Encode - drives the compression of a "Temporal Reference"
//
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Encode(void* state, VideoData* src, MediaData* dst, const H264_Encoder_Compression_Flags flags, H264_Encoder_Compression_Notes& notes)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status ps = UMC_OK;
    core_enc->m_pCurrentFrame = 0;
    core_enc->m_field_index = 0;
    Ipp32s isRef = 0;
    Ipp32s frame_index = 0;

    if (!(flags & H264_ECF_LAST_FRAME))
    {
        Ipp32u nShortTerm, nLongTerm;
        EnumPicCodType  ePictureType;
        FrameType ft = src->m_frameType;
        switch(ft){
            case I_PICTURE:
                ePictureType = INTRAPIC;
                break;
            case P_PICTURE:
                //Check for available frames in dpb
                H264EncoderFrameList_countActiveRefs(
                    &core_enc->m_dpb,
                    nShortTerm,
                    nLongTerm);
                if ((nShortTerm + nLongTerm) == 0)
                    return UMC_ERR_NOT_ENOUGH_DATA; //May be it should be  UMC_ERR_INVALID_PARAMS
                ePictureType = PREDPIC;
                break;
            case B_PICTURE:
                //Check for available frames in dpb
                H264EncoderFrameList_countActiveRefs(
                    &core_enc->m_dpb,
                    nShortTerm,
                    nLongTerm);
                if( (nShortTerm + nLongTerm) == 0 ) return UMC_ERR_NOT_ENOUGH_DATA; //May be it should be  UMC_ERR_INVALID_PARAMS
                ePictureType = BPREDPIC;
                break;
            case NONE_PICTURE:
            default:
#ifdef FRAME_QP_FROM_FILE
                char ft = frame_type.front();
                frame_type.pop_front();
                isRef = 0;
                switch( ft ){
                    case 'i':
                    case 'I':
                        ePictureType = INTRAPIC;
                        isRef = 1;
                        core_enc->m_bMakeNextFrameIDR = true;
                        break;
                    case 'p':
                    case 'P':
                        ePictureType = PREDPIC;
                        isRef = 1;
                        break;
                    case 'b':
                    case 'B':
                        ePictureType = BPREDPIC;
                        break;
                }
                //fprintf(stderr,"frame: %d %c ",core_enc->m_uFrames_Num,ft);
#else
                frame_index = core_enc->m_iProfileIndex;
                ePictureType = H264CoreEncoder_DetermineFrameType<COEFFSTYPE, PIXTYPE>(state, core_enc->m_iProfileIndex);
                if (ePictureType == BPREDPIC)
                {
                    switch( core_enc->m_params.B_reference_mode)
                    {
                        case 0:
                            isRef = 0; //No B references
                            break;
                        case 1:
                            if(((Ipp32u)(core_enc->m_iProfileIndex - 1)) == ((core_enc->m_params.B_frame_rate + 1) >> 1)) //Only middle B is reference, better to use round to low in case of even number of B frames
                                isRef = 1;
                            else
                                isRef = 0;
                            break;
                        case 2: //All B frames are refereces
                            isRef = 1;
                            break;
                    }
                }
#endif
                break;
        }

        core_enc->m_pLastFrame = core_enc->m_pCurrentFrame = H264EncoderFrameList_InsertFrame(
            &core_enc->m_cpb,
            src,
            ePictureType,
            isRef,
            core_enc->m_params.num_slices * ((core_enc->m_params.coding_type == 1) + 1),
            core_enc->m_PaddedSize
            , core_enc->m_SeqParamSet.aux_format_idc
            );
            // pad frame to MB boundaries
        {
            Ipp32s padW = core_enc->m_PaddedSize.width - core_enc->m_pCurrentFrame->uWidth;
            if (padW > 0) {
                Ipp32s  i, j;
                PIXTYPE *py = core_enc->m_pCurrentFrame->m_pYPlane + core_enc->m_pCurrentFrame->uWidth;
                for (i = 0; i < (Ipp32s)core_enc->m_pCurrentFrame->uHeight; i ++) {
                    for (j = 0; j < padW; j ++)
                        py[j] = py[-1];
                        // py[j] = 0;
                    py += core_enc->m_pCurrentFrame->m_pitchPixels;
                }
                if (core_enc->m_params.chroma_format_idc != 0) {
                    Ipp32s h = core_enc->m_pCurrentFrame->uHeight;
                    if (core_enc->m_params.chroma_format_idc == 1)
                        h >>= 1;
                    padW >>= 1;
                    PIXTYPE *pu = core_enc->m_pCurrentFrame->m_pUPlane + (core_enc->m_pCurrentFrame->uWidth >> 1);
                    PIXTYPE *pv = core_enc->m_pCurrentFrame->m_pVPlane + (core_enc->m_pCurrentFrame->uWidth >> 1);
                    for (i = 0; i < h; i ++) {
                        for (j = 0; j < padW; j ++) {
                            pu[j] = pu[-1];
                            pv[j] = pv[-1];
                            // pu[j] = 0;
                            // pv[j] = 0;
                        }
                        pu += core_enc->m_pCurrentFrame->m_pitchPixels;
                        pv += core_enc->m_pCurrentFrame->m_pitchPixels;
                    }
                }
            }
            Ipp32s padH = core_enc->m_PaddedSize.height - core_enc->m_pCurrentFrame->uHeight;
            if (padH > 0)
            {
                Ipp32s  i;
                PIXTYPE *pyD = core_enc->m_pCurrentFrame->m_pYPlane + core_enc->m_pCurrentFrame->uHeight * core_enc->m_pCurrentFrame->m_pitchPixels;
                PIXTYPE *pyS = pyD - core_enc->m_pCurrentFrame->m_pitchPixels;
                for (i = 0; i < padH; i ++)
                {
                    memcpy(pyD, pyS, core_enc->m_PaddedSize.width * sizeof(PIXTYPE));
                    //memset(pyD, 0, core_enc->m_PaddedSize.width * sizeof(PIXTYPE));
                    pyD += core_enc->m_pCurrentFrame->m_pitchPixels;
                }
                if (core_enc->m_params.chroma_format_idc != 0) {
                    Ipp32s h = core_enc->m_pCurrentFrame->uHeight;
                    if (core_enc->m_params.chroma_format_idc == 1)
                    {
                        h >>= 1;
                        padH >>= 1;
                    }
                    PIXTYPE *puD = core_enc->m_pCurrentFrame->m_pUPlane + h * core_enc->m_pCurrentFrame->m_pitchPixels;
                    PIXTYPE *pvD = core_enc->m_pCurrentFrame->m_pVPlane + h * core_enc->m_pCurrentFrame->m_pitchPixels;
                    PIXTYPE *puS = puD - core_enc->m_pCurrentFrame->m_pitchPixels;
                    PIXTYPE *pvS = pvD - core_enc->m_pCurrentFrame->m_pitchPixels;
                    for (i = 0; i < padH; i ++)
                    {
                        memcpy(puD, puS, (core_enc->m_PaddedSize.width >> 1) * sizeof(PIXTYPE));
                        memcpy(pvD, pvS, (core_enc->m_PaddedSize.width >> 1) * sizeof(PIXTYPE));
                        //memset(puD, 0, (core_enc->m_PaddedSize.width >> 1) * sizeof(PIXTYPE));
                        //memset(pvD, 0, (core_enc->m_PaddedSize.width >> 1) * sizeof(PIXTYPE));
                        puD += core_enc->m_pCurrentFrame->m_pitchPixels;
                        pvD += core_enc->m_pCurrentFrame->m_pitchPixels;
                    }
                }
            }
        }
        if(core_enc->m_pCurrentFrame)
        {
#ifdef FRAME_QP_FROM_FILE
            int qp = frame_qp.front();
            frame_qp.pop_front();
            core_enc->m_pCurrentFrame->frame_qp = qp;
            //fprintf(stderr,"qp: %d\n",qp);
#endif
            core_enc->m_pCurrentFrame->m_bIsIDRPic = false;
            if (core_enc->m_bMakeNextFrameIDR && ePictureType == INTRAPIC)
            {
                core_enc->m_pCurrentFrame->m_bIsIDRPic = true;
                core_enc->m_PicOrderCnt_Accu += core_enc->m_PicOrderCnt;
                core_enc->m_PicOrderCnt = 0;
                core_enc->m_bMakeNextFrameIDR = false;
            }
            switch (core_enc->m_params.coding_type)
            {
            case 0:
                core_enc->m_pCurrentFrame->m_PictureStructureForDec = core_enc->m_pCurrentFrame->m_PictureStructureForRef = FRM_STRUCTURE;
                core_enc->m_pCurrentFrame->m_PicOrderCnt[0] = core_enc->m_PicOrderCnt * 2;
                core_enc->m_pCurrentFrame->m_PicOrderCnt[1] = core_enc->m_PicOrderCnt * 2 + 1;
                core_enc->m_pCurrentFrame->m_PicOrderCounterAccumulated = 2*(core_enc->m_PicOrderCnt + core_enc->m_PicOrderCnt_Accu);
                core_enc->m_pCurrentFrame->m_bottom_field_flag[0] = 0;
                core_enc->m_pCurrentFrame->m_bottom_field_flag[1] = 0;
                break;
            case 1:
                core_enc->m_pCurrentFrame->m_PictureStructureForDec = core_enc->m_pCurrentFrame->m_PictureStructureForRef = FLD_STRUCTURE;
                core_enc->m_pCurrentFrame->m_PicOrderCnt[0] = core_enc->m_PicOrderCnt * 2;
                core_enc->m_pCurrentFrame->m_PicOrderCnt[1] = core_enc->m_PicOrderCnt * 2 + 1;
                core_enc->m_pCurrentFrame->m_PicOrderCounterAccumulated = 2*(core_enc->m_PicOrderCnt + core_enc->m_PicOrderCnt_Accu);
                core_enc->m_pCurrentFrame->m_bottom_field_flag[0] = 0;
                core_enc->m_pCurrentFrame->m_bottom_field_flag[1] = 1;
                break;
            case 2:
                core_enc->m_pCurrentFrame->m_PictureStructureForDec = core_enc->m_pCurrentFrame->m_PictureStructureForRef = AFRM_STRUCTURE;
                core_enc->m_pCurrentFrame->m_PicOrderCnt[0] = core_enc->m_PicOrderCnt * 2;
                core_enc->m_pCurrentFrame->m_PicOrderCnt[1] = core_enc->m_PicOrderCnt * 2 + 1;
                core_enc->m_pCurrentFrame->m_PicOrderCounterAccumulated = 2*(core_enc->m_PicOrderCnt + core_enc->m_PicOrderCnt_Accu);
                core_enc->m_pCurrentFrame->m_bottom_field_flag[0] = 0;
                core_enc->m_pCurrentFrame->m_bottom_field_flag[1] = 0;
                break;
            default:
                return UMC_ERR_UNSUPPORTED;
            }
            H264EncoderFrame_InitRefPicListResetCount(core_enc->m_pCurrentFrame, 0);
            H264EncoderFrame_InitRefPicListResetCount(core_enc->m_pCurrentFrame, 1);
            core_enc->m_PicOrderCnt++;
            if (core_enc->m_pCurrentFrame->m_bIsIDRPic)
                H264EncoderFrameList_IncreaseRefPicListResetCount<PIXTYPE>(&core_enc->m_cpb, core_enc->m_pCurrentFrame);
            if (core_enc->m_Analyse & ANALYSE_FRAME_TYPE)
                H264CoreEncoder_FrameTypeDetect<COEFFSTYPE, PIXTYPE>(state);
        }
        else
            return UMC_ERR_INVALID_STREAM;
    }
    else
    {
        if (core_enc->m_pLastFrame && core_enc->m_pLastFrame->m_PicCodType == BPREDPIC)
        {
            core_enc->m_pLastFrame->m_PicCodType = PREDPIC;
            core_enc->m_l1_cnt_to_start_B = 1;
        }
    }

    if (UMC_OK == ps)
    {
        core_enc->m_pCurrentFrame = H264EncoderFrameList_findOldestToEncode(
            &core_enc->m_cpb,
            &core_enc->m_dpb,
            core_enc->m_l1_cnt_to_start_B,
            core_enc->m_params.B_reference_mode);

        if (core_enc->m_pCurrentFrame)
        {

            if (core_enc->m_pCurrentFrame->m_bIsIDRPic)
                core_enc->m_uFrameCounter = 0;
            core_enc->m_pCurrentFrame->m_FrameNum = core_enc->m_uFrameCounter;

//            if( core_enc->m_pCurrentFrame->m_PicCodType != BPREDPIC || core_enc->m_params.treat_B_as_reference )
            if( core_enc->m_pCurrentFrame->m_PicCodType != BPREDPIC || core_enc->m_pCurrentFrame->m_RefPic )
                core_enc->m_uFrameCounter++;

            H264CoreEncoder_MoveFromCPBToDPB<COEFFSTYPE, PIXTYPE>(state);

            notes &= ~H264_ECN_NO_FRAME;
        }
        else
        {
            core_enc->cnotes |= H264_ECN_NO_FRAME;
            return UMC_ERR_NOT_ENOUGH_DATA;
        }

        EnumPicCodType ePictureType = core_enc->m_pCurrentFrame->m_PicCodType;
        EnumPicClass    ePic_Class;
        switch (ePictureType)
        {
            case INTRAPIC:
                if (core_enc->m_pCurrentFrame->m_bIsIDRPic || core_enc->m_uFrames_Num == 0)
                {
                    // Right now, only the first INTRAPIC in a sequence is an IDR Frame
                    ePic_Class = IDR_PIC;
                    core_enc->m_l1_cnt_to_start_B = core_enc->m_params.num_ref_to_start_code_B_slice;
                }
                else
                    ePic_Class = REFERENCE_PIC;
                break;

            case PREDPIC:
                ePic_Class = REFERENCE_PIC;
                break;

            case BPREDPIC:
                ePic_Class = ((core_enc->m_params.B_reference_mode && core_enc->m_pCurrentFrame->m_RefPic) ? REFERENCE_PIC : DISPOSABLE_PIC);
                break;

            default:
                VM_ASSERT(false);
                ePic_Class = IDR_PIC;
                break;
        }

        // ePictureType is a reference variable.  It is updated by CompressFrame if the frame is internally forced to a key frame.
        size_t data_size = dst->GetDataSize();
        ps = H264CoreEncoder_CompressFrame<COEFFSTYPE, PIXTYPE>(state, ePictureType, ePic_Class, dst);

        dst->m_fPTSStart = core_enc->m_pCurrentFrame->pts_start;
        dst->m_fPTSEnd   = core_enc->m_pCurrentFrame->pts_end;
        data_size = dst->GetDataSize() - data_size;
        if (data_size > 0)
        {
            if (ePictureType == INTRAPIC)       // Tell the environment we just generated a key frame
                notes |= H264_ECN_KEY_FRAME;
            else if (ePictureType == BPREDPIC) // Tell the environment we just generated a B frame
                notes |= H264_ECN_B_FRAME;
        }

        if (ps == UMC_OK && data_size == 0 && (flags & H264_ECF_LAST_FRAME))
            notes |= H264_ECN_NO_FRAME;
    }

    if (ps == UMC_OK)
        core_enc->m_pCurrentFrame->m_wasEncoded = true;
    H264CoreEncoder_CleanDPB<COEFFSTYPE, PIXTYPE>(state);
    if (ps == UMC_OK)
        core_enc->m_params.m_iFramesCounter++;
    return ps;
}


/**********************************************************************
 *  EncoderH264 destructor.
 **********************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Destroy(void* state)
{
    if(state)
    {
        H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
        // release the noise reduction prefilter
        H264CoreEncoder_Close<COEFFSTYPE, PIXTYPE>(state);
        H264_AVBR_Destroy(&core_enc->avbr);
        ((H264EncoderParams*)&core_enc->m_params)->~H264EncoderParams();
        H264EncoderFrameList_Destroy(&core_enc->m_dpb);
        H264EncoderFrameList_Destroy(&core_enc->m_cpb);
    }
}

/*************************************************************
 *  Name: SetSliceHeaderCommon
 *  Description:  Given the ePictureType and core_enc->m_params
 *                fill in the slice header for this slice
 ************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_SetSliceHeaderCommon(void* state, H264EncoderFrame<PIXTYPE>* pCurrentFrame)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s frame_num = pCurrentFrame->m_FrameNum;
    // Right now, we always work with frame nums modulo 256
    core_enc->m_SliceHeader.frame_num = frame_num % core_enc->m_uTRWrapAround;

    // Assumes there is only one picture parameter set to choose from
    core_enc->m_SliceHeader.pic_parameter_set_id = core_enc->m_PicParamSet.pic_parameter_set_id;

    core_enc->m_SliceHeader.field_pic_flag = (Ipp8u)(pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE);
    core_enc->m_SliceHeader.bottom_field_flag = (Ipp8u)(pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index]);
    core_enc->m_SliceHeader.MbaffFrameFlag = (core_enc->m_SeqParamSet.mb_adaptive_frame_field_flag)&&(!core_enc->m_SliceHeader.field_pic_flag);
    core_enc->m_SliceHeader.delta_pic_order_cnt_bottom = core_enc->m_SeqParamSet.frame_mbs_only_flag == 0;

    //core_enc->m_TopPicOrderCnt = core_enc->m_PicOrderCnt;
    //core_enc->m_BottomPicOrderCnt = core_enc->m_TopPicOrderCnt + 1; // ????

    if (core_enc->m_SeqParamSet.pic_order_cnt_type == 0)
    {
        core_enc->m_SliceHeader.pic_order_cnt_lsb = H264EncoderFrame_PicOrderCnt(
            core_enc->m_pCurrentFrame,
            core_enc->m_field_index,
            0) & ~(0xffffffff << core_enc->m_SeqParamSet.log2_max_pic_order_cnt_lsb);
    }
    core_enc->m_SliceHeader.adaptive_ref_pic_marking_mode_flag = 0;

    for(Ipp32s i = 0; i < core_enc->m_params.num_slices*((core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE)+1); i++)
    { //TODO fix for PicAFF/AFRM
        H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice = core_enc->m_Slices + i;
        curr_slice->num_ref_idx_l0_active = core_enc->m_PicParamSet.num_ref_idx_l0_active;
        curr_slice->num_ref_idx_l1_active = core_enc->m_PicParamSet.num_ref_idx_l1_active;

        curr_slice->num_ref_idx_active_override_flag = (
            (curr_slice->num_ref_idx_l0_active != core_enc->m_PicParamSet.num_ref_idx_l0_active)
            || (curr_slice->num_ref_idx_l1_active != core_enc->m_PicParamSet.num_ref_idx_l1_active)
        );

        curr_slice->m_disable_deblocking_filter_idc = core_enc->m_params.deblocking_filter_idc;
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_CheckEncoderParameters(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status status = UMC_OK;

    if (core_enc->m_params.coding_type > 1)
        core_enc->m_params.coding_type = 0;
    if (core_enc->m_params.m_info.iBitrate <= 0)
        core_enc->m_params.rate_controls.method = H264_RCM_QUANT;
    if (core_enc->m_params.m_info.videoInfo.m_iWidth < 1 || core_enc->m_params.m_info.videoInfo.m_iHeight < 1)
        return UMC_ERR_INIT;
    if (core_enc->m_params.key_interval == 0) // key frames only
    {
        core_enc->m_params.idr_interval   = 0;
        core_enc->m_params.B_frame_rate   = 0;
        core_enc->m_params.num_ref_frames = 0;
    }
    if (core_enc->m_params.B_reference_mode > 2)
        core_enc->m_params.B_reference_mode = 2;
    if(core_enc->m_params.key_interval > 0 && core_enc->m_params.num_ref_frames < 1)
        core_enc->m_params.num_ref_frames = 1;
    if (core_enc->m_params.num_ref_frames > 16)
        core_enc->m_params.num_ref_frames = 16;
    if (core_enc->m_params.B_frame_rate > 0)
    {
        if(core_enc->m_params.num_ref_to_start_code_B_slice < 1)
            core_enc->m_params.num_ref_to_start_code_B_slice = 1;
        if(core_enc->m_params.num_ref_to_start_code_B_slice >= core_enc->m_params.num_ref_frames)
            core_enc->m_params.num_ref_frames = core_enc->m_params.num_ref_to_start_code_B_slice + 1;
    }
#if defined (_OPENMP)
    if(core_enc->m_params.m_iThreads < 1)
        core_enc->m_params.m_iThreads = omp_get_max_threads();
#else
    core_enc->m_params.m_iThreads = 1;
#endif // _OPENMP
    if(core_enc->m_params.max_slice_size)
    {
        core_enc->m_MaxSliceSize = core_enc->m_params.max_slice_size;
        core_enc->m_params.m_iThreads = 1;
        core_enc->m_params.num_slices = 1;
    }
    if(core_enc->m_params.num_slices == 0)
        core_enc->m_params.num_slices = (Ipp16s)IPP_MIN(core_enc->m_params.m_iThreads, 0x7FFF);

    Ipp32s nMB = ((core_enc->m_params.m_info.videoInfo.m_iHeight + 15) >> 4) * ((core_enc->m_params.m_info.videoInfo.m_iWidth + 15) >> 4);
    if (core_enc->m_params.num_slices > nMB)
        core_enc->m_params.num_slices = (Ipp16s)IPP_MIN(nMB, 0x7FFF);
    if (core_enc->m_params.num_slices < core_enc->m_params.m_iThreads)
        core_enc->m_params.m_iThreads = core_enc->m_params.num_slices;
    switch (core_enc->m_params.level_idc)
    {
        case 0:
        case 10: case 11: case 12: case 13:
        case 20: case 21: case 22:
        case 30: case 31: case 32:
        case 40: case 41: case 42:
        case 50: case 51:
        break;
    default:
        //Set default level to autoselect
        core_enc->m_params.level_idc = 0;
    }

    // Calculate the Level Based on encoding parameters .
    Ipp32u MB_per_frame = ((core_enc->m_params.m_info.videoInfo.m_iWidth+15)>>4) * ((core_enc->m_params.m_info.videoInfo.m_iHeight+15)>>4);
    Ipp32u MB_per_sec = (Ipp32u)(MB_per_frame * core_enc->m_params.m_info.fFramerate);
    if (core_enc->m_params.level_idc==0)
    {
        if ((MB_per_frame <= 99) && (MB_per_sec <= 1485))
            core_enc->m_params.level_idc = 10;   // Level 1
        else if ((MB_per_frame <= 396) && (MB_per_sec <= 3000))
            core_enc->m_params.level_idc = 11;   // Level 1.1
        else if ((MB_per_frame <= 396) && (MB_per_sec <= 6000))
            core_enc->m_params.level_idc = 12;   // Level 1.2
        else if ((MB_per_frame <= 396) && (MB_per_sec <= 11880))
            core_enc->m_params.level_idc = 20;   // Level 2
        else if ((MB_per_frame <= 792) && (MB_per_sec <= 19800))
            core_enc->m_params.level_idc = 21;   // Level 2.1
        else if ((MB_per_frame <= 1620) && (MB_per_sec <= 20250))
            core_enc->m_params.level_idc = 22;   // Level 2.2
        else if ((MB_per_frame <= 1620) && (MB_per_sec <= 40500))
            core_enc->m_params.level_idc = 30;   // Level 3
        else if ((MB_per_frame <= 3600) && (MB_per_sec <= 108000))
            core_enc->m_params.level_idc = 31;   // Level 3.1
        else if ((MB_per_frame <= 5120) && (MB_per_sec <= 216000))
            core_enc->m_params.level_idc = 32;   // Level 3.2
        else if ((MB_per_frame <= 8192) && (MB_per_sec <= 245760))
            core_enc->m_params.level_idc = 40;   // Level 4
//        else if ((MB_per_frame <= 8192) && (MB_per_sec <= 245760))
//            core_enc->m_SeqParamSet.level_idc = 41;   // Level 4.1
        else if ((MB_per_frame <= 8704) && (MB_per_sec <= 522240))
            core_enc->m_params.level_idc = 42;   // Level 4.2
        else if ((MB_per_frame <= 22080) && (MB_per_sec <= 589824))
            core_enc->m_params.level_idc= 50;    // Level 5
        else
            core_enc->m_params.level_idc= 51;    // Level 5.1
    }

    // Profile selection
    if(core_enc->m_params.profile_idc == 0) // Auto detect
    {
#ifdef BITDEPTH_9_12
        if(core_enc->m_params.bit_depth_chroma > 14 || core_enc->m_params.bit_depth_luma > 14 || core_enc->m_params.bit_depth_aux > 12)
            return UMC_ERR_INVALID_PARAMS;
#else
        if(core_enc->m_params.bit_depth_chroma > 8 || core_enc->m_params.bit_depth_luma > 8 || core_enc->m_params.bit_depth_aux > 8)
            return UMC_ERR_UNSUPPORTED;
#endif
        if(core_enc->m_params.bit_depth_chroma < 8 || core_enc->m_params.bit_depth_luma < 8 || core_enc->m_params.bit_depth_aux < 8)
            return UMC_ERR_INVALID_PARAMS;
        if(core_enc->m_params.chroma_format_idc > 2) // YUV444 not supported
            return UMC_ERR_UNSUPPORTED;

        for(;;)
        {
            if(core_enc->m_params.bit_depth_chroma > 10 ||
                core_enc->m_params.bit_depth_luma > 10 ||
                core_enc->m_params.bit_depth_aux > 10 ||
                core_enc->m_params.chroma_format_idc > 2 ||
                core_enc->m_params.qpprime_y_zero_transform_bypass_flag)
            {
                core_enc->m_params.profile_idc = H264_PROFILE_HIGH444;
                if(core_enc->m_params.key_interval == 0)
                {
                    if(!core_enc->m_params.entropy_coding_mode_flag)
                    {
                        core_enc->m_params.profile_idc = H264_PROFILE_CAVLC444; // cavlc444_intra profile
                        break;
                    }
                    core_enc->m_params.constraint_set_flag[3] = true; // high444_intra profile
                }
                break;
            }

            if(core_enc->m_params.bit_depth_chroma > 8 ||
                core_enc->m_params.bit_depth_luma > 8 ||
                core_enc->m_params.bit_depth_aux > 8)
            {
                if(core_enc->m_params.chroma_format_idc > 1)
                    core_enc->m_params.profile_idc = H264_PROFILE_HIGH422;
                else
                    core_enc->m_params.profile_idc = H264_PROFILE_HIGH10;
                if(core_enc->m_params.key_interval == 0)
                    core_enc->m_params.constraint_set_flag[3] = true; // high422_intra, high10_intra profiles
                break;
            }

            if(core_enc->m_params.aux_format_idc ||
                core_enc->m_params.chroma_format_idc != 1 ||
                core_enc->m_params.transform_8x8_mode_flag ||
                core_enc->m_params.use_default_scaling_matrix)
            {
                core_enc->m_params.profile_idc = H264_PROFILE_HIGH;
                break;
            }

            if(core_enc->m_params.B_frame_rate ||
                core_enc->m_params.entropy_coding_mode_flag ||
                core_enc->m_params.coding_type ||
                core_enc->m_params.weighted_pred_flag ||
                core_enc->m_params.weighted_bipred_idc)
            {
                core_enc->m_params.profile_idc = H264_PROFILE_MAIN;
                break;
            }

            core_enc->m_params.profile_idc = H264_PROFILE_BASELINE;
            break;
        }
    }
    else
    {
        switch(core_enc->m_params.profile_idc)
        {
        case H264_PROFILE_BASELINE:
            core_enc->m_params.B_frame_rate                         = 0;
            core_enc->m_params.bit_depth_aux                        = 8;
            core_enc->m_params.bit_depth_chroma                     = 8;
            core_enc->m_params.bit_depth_luma                       = 8;
            core_enc->m_params.aux_format_idc                       = 0;
            core_enc->m_params.chroma_format_idc                    = 1;
            core_enc->m_params.transform_8x8_mode_flag              = false;
            core_enc->m_params.qpprime_y_zero_transform_bypass_flag = false;
            core_enc->m_params.coding_type                          = 0; // frame only
            core_enc->m_params.use_default_scaling_matrix           = 0;
            core_enc->m_params.weighted_pred_flag                   = false;
            core_enc->m_params.weighted_bipred_idc                  = 0;
            core_enc->m_params.entropy_coding_mode_flag             = false;
            if(!core_enc->m_params.constraint_set_flag[1]) // also obey main profile restrictions if any
                break;

        case H264_PROFILE_MAIN:
            core_enc->m_params.bit_depth_aux                        = 8;
            core_enc->m_params.bit_depth_chroma                     = 8;
            core_enc->m_params.bit_depth_luma                       = 8;
            core_enc->m_params.aux_format_idc                       = 0;
            core_enc->m_params.chroma_format_idc                    = 1;
            core_enc->m_params.transform_8x8_mode_flag              = false;
            core_enc->m_params.qpprime_y_zero_transform_bypass_flag = false;
            core_enc->m_params.use_default_scaling_matrix           = 0;
            break;

        case H264_PROFILE_EXTENDED:
            core_enc->m_params.direct_8x8_inference_flag            = true;
            core_enc->m_params.bit_depth_aux                        = 8;
            core_enc->m_params.bit_depth_chroma                     = 8;
            core_enc->m_params.bit_depth_luma                       = 8;
            core_enc->m_params.aux_format_idc                       = 0;
            core_enc->m_params.chroma_format_idc                    = 1;
            core_enc->m_params.transform_8x8_mode_flag              = false;
            core_enc->m_params.qpprime_y_zero_transform_bypass_flag = false;
            core_enc->m_params.use_default_scaling_matrix           = 0;
            core_enc->m_params.entropy_coding_mode_flag             = false;
            break;

        case H264_PROFILE_HIGH:
            if(core_enc->m_params.chroma_format_idc > 1)
                core_enc->m_params.chroma_format_idc = 1;
            core_enc->m_params.bit_depth_aux                        = 8;
            core_enc->m_params.bit_depth_chroma                     = 8;
            core_enc->m_params.bit_depth_luma                       = 8;
            core_enc->m_params.qpprime_y_zero_transform_bypass_flag = false;
            if(core_enc->m_params.constraint_set_flag[3]) // also high10_intra
            {
                core_enc->m_params.key_interval = 0;
                core_enc->m_params.idr_interval = 0;
            }
            break;

        case H264_PROFILE_HIGH10:
            if(core_enc->m_params.chroma_format_idc > 1)
                core_enc->m_params.chroma_format_idc = 1;
            if(core_enc->m_params.bit_depth_aux > 10)
                core_enc->m_params.bit_depth_aux = 10;
            if(core_enc->m_params.bit_depth_chroma > 10)
                core_enc->m_params.bit_depth_chroma = 10;
            if(core_enc->m_params.bit_depth_luma > 10)
                core_enc->m_params.bit_depth_luma = 10;
            core_enc->m_params.qpprime_y_zero_transform_bypass_flag = false;
            if(core_enc->m_params.constraint_set_flag[3]) // high10_intra
            {
                core_enc->m_params.key_interval = 0;
                core_enc->m_params.idr_interval = 0;
            }
            break;

        case H264_PROFILE_HIGH422:
            if(core_enc->m_params.chroma_format_idc > 2)
                core_enc->m_params.chroma_format_idc = 2;
            if(core_enc->m_params.bit_depth_aux > 10)
                core_enc->m_params.bit_depth_aux = 10;
            if(core_enc->m_params.bit_depth_chroma > 10)
                core_enc->m_params.bit_depth_chroma = 10;
            if(core_enc->m_params.bit_depth_luma > 10)
                core_enc->m_params.bit_depth_luma = 10;
            core_enc->m_params.qpprime_y_zero_transform_bypass_flag = false;
            if(core_enc->m_params.constraint_set_flag[3]) // high422_intra
            {
                core_enc->m_params.key_interval = 0;
                core_enc->m_params.idr_interval = 0;
            }
            break;

        case H264_PROFILE_CAVLC444:
            core_enc->m_params.entropy_coding_mode_flag = false;
            core_enc->m_params.key_interval             = 0;
            core_enc->m_params.idr_interval             = 0;
        case H264_PROFILE_HIGH444:
            if(core_enc->m_params.chroma_format_idc > 2)
                core_enc->m_params.chroma_format_idc = 2; // YUV444 not supported
            if(core_enc->m_params.bit_depth_aux > 12)
                core_enc->m_params.bit_depth_aux = 12;
            if(core_enc->m_params.bit_depth_chroma > 14)
                core_enc->m_params.bit_depth_chroma = 14;
            if(core_enc->m_params.bit_depth_luma > 14)
                core_enc->m_params.bit_depth_luma = 14;
            if(core_enc->m_params.constraint_set_flag[3]) // high444_intra
            {
                core_enc->m_params.key_interval = 0;
                core_enc->m_params.idr_interval = 0;
            }
            break;
        }
    }

    switch(core_enc->m_params.rate_controls.method) {
        case H264_RCM_QUANT:
        case H264_RCM_CBR:
        case H264_RCM_VBR:
        case H264_RCM_CBR_SLICE:
        case H264_RCM_VBR_SLICE:
            break;
        default:
            core_enc->m_params.rate_controls.method = H264_RCM_VBR;
            break;
    }

    H264CoreEncoder_SetDPBSize<COEFFSTYPE, PIXTYPE>(state);

    core_enc->m_params.weighted_pred_flag = false;
    core_enc->m_params.weighted_bipred_idc = 0;
    if (core_enc->m_params.num_ref_frames > core_enc->m_dpbSize)
        core_enc->m_params.num_ref_frames = core_enc->m_dpbSize;
    if (core_enc->m_params.rate_controls.quantI < -6*(core_enc->m_params.bit_depth_luma - 8) || core_enc->m_params.rate_controls.quantI > 51)
        core_enc->m_params.rate_controls.quantI = 0;
    if (core_enc->m_params.rate_controls.quantP < -6*(core_enc->m_params.bit_depth_luma - 8) || core_enc->m_params.rate_controls.quantP > 51)
        core_enc->m_params.rate_controls.quantP = 0;
    if (core_enc->m_params.rate_controls.quantB < -6*(core_enc->m_params.bit_depth_luma - 8) || core_enc->m_params.rate_controls.quantB > 51)
        core_enc->m_params.rate_controls.quantB = 0;
    if (core_enc->m_params.mv_search_method > 31)
        core_enc->m_params.mv_search_method = 2;
    if (core_enc->m_params.me_split_mode > 3)
        core_enc->m_params.me_split_mode = 1;
    if (core_enc->m_params.direct_pred_mode > 2)
        core_enc->m_params.direct_pred_mode = 1;
    if (core_enc->m_params.deblocking_filter_idc > 2)
        core_enc->m_params.deblocking_filter_idc = 0;
    if (core_enc->m_params.deblocking_filter_alpha < -12 || core_enc->m_params.deblocking_filter_alpha > 12)
        core_enc->m_params.deblocking_filter_alpha = 6;
    if (core_enc->m_params.deblocking_filter_beta < -12 || core_enc->m_params.deblocking_filter_beta > 12)
        core_enc->m_params.deblocking_filter_beta = 6;
    if (core_enc->m_params.cabac_init_idc > 2)
        core_enc->m_params.cabac_init_idc = 0;
    return status;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Init(void* state, BaseCodecParams* init, MemoryAllocator* pMemAlloc)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264EncoderParams *info = DynamicCast<H264EncoderParams, BaseCodecParams> (init);
    if(info == NULL)
    {
        VideoEncoderParams *VideoParams = DynamicCast<VideoEncoderParams, BaseCodecParams> (init);
        if(VideoParams == NULL)
            return UMC_ERR_INIT;

        core_enc->m_params.m_info.videoInfo.m_iWidth       = VideoParams->m_info.videoInfo.m_iWidth;
        core_enc->m_params.m_info.videoInfo.m_iHeight      = VideoParams->m_info.videoInfo.m_iHeight;
        core_enc->m_params.m_info.iBitrate         = VideoParams->m_info.iBitrate;
        core_enc->m_params.m_info.fFramerate       = VideoParams->m_info.fFramerate;
        core_enc->m_params.m_iFramesCounter  = 0;
        core_enc->m_params.m_iThreads      = VideoParams->m_iThreads;
        core_enc->m_params.m_iQuality = VideoParams->m_iQuality;
        info = &core_enc->m_params;
    }
    else
        core_enc->m_params = *info;

    Status status = H264CoreEncoder_CheckEncoderParameters<COEFFSTYPE, PIXTYPE>(state);
    if (status != UMC_OK)
        return status;

    H264CoreEncoder_SetSequenceParameters<COEFFSTYPE, PIXTYPE>(state);
    core_enc->m_SubME_Algo = (core_enc->m_params.mv_search_method == 0) ? 0 : 2;
    core_enc->m_Analyse = ANALYSE_I_4x4 | ANALYSE_ME_SUBPEL | ANALYSE_CHECK_SKIP_INTPEL | ANALYSE_CHECK_SKIP_BESTCAND | ANALYSE_CHECK_SKIP_SUBPEL;
    if (core_enc->m_params.transform_8x8_mode_flag)
        core_enc->m_Analyse |= ANALYSE_I_8x8;
    core_enc->m_Analyse |= ANALYSE_FRAME_TYPE;
    //core_enc->m_Analyse |= ANALYSE_ME_CONTINUED_SEARCH;
    //core_enc->m_Analyse |= ANALYSE_FAST_INTRA; // QS <= 3
    //core_enc->m_Analyse |= ANALYSE_ME_SUBPEL_SAD; // QS <= 3
    //core_enc->m_Analyse |= ANALYSE_FLATNESS; // QS <= 1
    //core_enc->m_Analyse |= ANALYSE_INTRA_IN_ME; // QS <= 1
    //core_enc->m_Analyse |= ANALYSE_ME_FAST_MULTIREF; // QS <= 1
    //core_enc->m_Analyse |= ANALYSE_ME_EARLY_EXIT;
    //core_enc->m_Analyse |= ANALYSE_CBP_EMPTY;
    //core_enc->m_Analyse |= ANALYSE_SPLIT_SMALL_RANGE;
    //core_enc->m_Analyse |= ANALYSE_ME_EXT_CANDIDATES;
    core_enc->m_Analyse |= ANALYSE_RECODE_FRAME;
    if (core_enc->m_params.me_split_mode != 0)
        core_enc->m_Analyse |= ANALYSE_P_8x8 | ANALYSE_B_8x8;
    if (core_enc->m_params.me_split_mode > 1)
        core_enc->m_Analyse |= ANALYSE_P_4x4 | ANALYSE_B_4x4;
    if (core_enc->m_params.direct_pred_mode == 2)
        core_enc->m_Analyse |= ANALYSE_ME_AUTO_DIRECT;

    if (core_enc->m_params.m_QualitySpeed == 0)
        core_enc->m_Analyse |= ANALYSE_SAD;
    if (core_enc->m_params.m_QualitySpeed <= 1)
        core_enc->m_Analyse |= ANALYSE_FLATNESS | ANALYSE_INTRA_IN_ME | ANALYSE_ME_FAST_MULTIREF | ANALYSE_ME_PRESEARCH;
    if (core_enc->m_params.m_QualitySpeed >= 2 && core_enc->m_params.chroma_format_idc > 0)
        core_enc->m_Analyse |= ANALYSE_ME_CHROMA;
    if ((core_enc->m_params.m_QualitySpeed >= 2) && core_enc->m_params.entropy_coding_mode_flag)
        core_enc->m_Analyse |= ANALYSE_RD_MODE;
    if ((core_enc->m_params.m_QualitySpeed >= 3) && core_enc->m_params.entropy_coding_mode_flag)
        core_enc->m_Analyse |= ANALYSE_RD_OPT | ANALYSE_B_RD_OPT;
    if (core_enc->m_params.m_QualitySpeed <= 3)
        core_enc->m_Analyse |= ANALYSE_ME_SUBPEL_SAD | ANALYSE_FAST_INTRA;
    if (core_enc->m_params.m_QualitySpeed >= 4)
      core_enc->m_Analyse |= /*ANALYSE_B_RD_OPT |*/ ANALYSE_ME_BIDIR_REFINE;
    if (core_enc->m_params.m_QualitySpeed >= 5)
        core_enc->m_Analyse |= ANALYSE_ME_ALL_REF;

    if (core_enc->m_params.coding_type > 0) {
        if (core_enc->m_params.direct_pred_mode == 2) {
            core_enc->m_Analyse &= ~ANALYSE_ME_AUTO_DIRECT;
            core_enc->m_params.direct_pred_mode = 1;
        }
        //core_enc->m_Analyse &= ~ANALYSE_FRAME_TYPE;
    }
    //External memory allocator
    core_enc->memAlloc = pMemAlloc;
    core_enc->m_dpb.memAlloc = pMemAlloc;
    core_enc->m_cpb.memAlloc = pMemAlloc;

    Ipp32s numOfSliceEncs = core_enc->m_params.num_slices*((core_enc->m_params.coding_type == 1) + 1);
    status = UMC_ERR_ALLOC;
    size_t* _alloc_ptr = (size_t*)H264_Malloc(numOfSliceEncs * sizeof(H264Slice<COEFFSTYPE, PIXTYPE>) + sizeof(size_t));
    core_enc->m_Slices = (H264Slice<COEFFSTYPE, PIXTYPE>*)(_alloc_ptr + 1);
    _alloc_ptr[0] = (size_t)numOfSliceEncs;
    if (core_enc->m_Slices)
    {
        for (size_t i = 0; i < (size_t)numOfSliceEncs; i++)
        {
            status = (H264Slice_Create<COEFFSTYPE, PIXTYPE>)((H264Slice<COEFFSTYPE, PIXTYPE>*)core_enc->m_Slices + i);
            if(status != UMC_OK)
            {
                for (i++; i > 0; i--)
                    (H264Slice_Destroy<COEFFSTYPE, PIXTYPE>)((H264Slice<COEFFSTYPE, PIXTYPE>*)core_enc->m_Slices + i - 1);
                H264_Free(_alloc_ptr);
                core_enc->m_Slices = NULL;
                break;
            }
        }
    }
    if (status != UMC_OK)
        return status;

    Ipp32s i;
    for (i = 0; i < numOfSliceEncs; i++)
    { // TODO fix for PicAFF/AFRM
        status = H264Slice_Init<COEFFSTYPE, PIXTYPE>(&core_enc->m_Slices[i], core_enc->m_params);
        if (status != UMC_OK)
            return status;
    }
    core_enc->profile_frequency = core_enc->m_params.B_frame_rate + 1;
    if (core_enc->eFrameSeq)
        H264_Free(core_enc->eFrameSeq);
    core_enc->eFrameSeq = (H264EncoderFrame<PIXTYPE> **)H264_Malloc((core_enc->profile_frequency + 1) * sizeof(H264EncoderFrame<PIXTYPE>*));
    for (i = 0; i <= core_enc->profile_frequency; i++)
        core_enc->eFrameSeq[i] = NULL;
    if (core_enc->eFrameType != NULL)
        H264_Free(core_enc->eFrameType);
    core_enc->eFrameType = (EnumPicCodType *)H264_Malloc(core_enc->profile_frequency * sizeof(EnumPicCodType));
    core_enc->eFrameType[0] = PREDPIC;
    for (i = 1; i < core_enc->profile_frequency; i++)
        core_enc->eFrameType[i] = BPREDPIC;

    // Set up for the 8 bit/default frame rate
    core_enc->m_uTRWrapAround = TR_WRAP;
    core_enc->m_uFrames_Num = 0;
    core_enc->m_uFrameCounter = 0;
    core_enc->m_PaddedSize.width  = (core_enc->m_params.m_info.videoInfo.m_iWidth  + 15) & ~15;
    core_enc->m_PaddedSize.height = (core_enc->m_params.m_info.videoInfo.m_iHeight + (16<<(1 - core_enc->m_SeqParamSet.frame_mbs_only_flag)) - 1) & ~((16<<(1 - core_enc->m_SeqParamSet.frame_mbs_only_flag)) - 1);
    core_enc->m_WidthInMBs = core_enc->m_PaddedSize.width >> 4;
    core_enc->m_HeightInMBs  = core_enc->m_PaddedSize.height >> 4;
    core_enc->m_Pitch = CalcPitchFromWidth(core_enc->m_PaddedSize.width, sizeof(PIXTYPE)) / sizeof(PIXTYPE);
    core_enc->m_bMakeNextFrameKey = true; // Ensure that we always start with a key frame.
    core_enc->m_bMakeNextFrameIDR = false;
    core_enc->m_uIntraFrameInterval = core_enc->m_params.key_interval + 1;
    core_enc->m_uIDRFrameInterval = core_enc->m_params.idr_interval + 1;

    memset(&core_enc->m_AdaptiveMarkingInfo,0,sizeof(core_enc->m_AdaptiveMarkingInfo));
    memset(&core_enc->m_ReorderInfoL0,0,sizeof(core_enc->m_ReorderInfoL0));
    memset(&core_enc->m_ReorderInfoL1,0,sizeof(core_enc->m_ReorderInfoL1));
    if ((core_enc->m_EmptyThreshold = (Ipp32u*)H264_Malloc(52*sizeof(Ipp32u))) == NULL)
        return UMC_ERR_ALLOC;
    if ((core_enc->m_DirectBSkipMEThres = (Ipp32u*)H264_Malloc(52*sizeof(Ipp32u))) == NULL)
        return UMC_ERR_ALLOC;
    if ((core_enc->m_PSkipMEThres = (Ipp32u*)H264_Malloc(52*sizeof(Ipp32u))) == NULL)
        return UMC_ERR_ALLOC;
    if ((core_enc->m_BestOf5EarlyExitThres = (Ipp32s*)H264_Malloc(52*sizeof(Ipp32s))) == NULL)
        return UMC_ERR_ALLOC;
    if (core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag)
    {
        ippsSet_8u(0, (Ipp8u*)core_enc->m_EmptyThreshold, 52 * sizeof(*core_enc->m_EmptyThreshold));
        ippsSet_8u(0, (Ipp8u*)core_enc->m_DirectBSkipMEThres, 52 * sizeof(*core_enc->m_DirectBSkipMEThres));
        ippsSet_8u(0, (Ipp8u*)core_enc->m_PSkipMEThres, 52 * sizeof(*core_enc->m_PSkipMEThres));
        ippsSet_8u(0, (Ipp8u*)core_enc->m_BestOf5EarlyExitThres, 52 * sizeof(*core_enc->m_BestOf5EarlyExitThres));
    }
    else
    {
        ippsCopy_8u((const Ipp8u*)EmptyThreshold, (Ipp8u*)core_enc->m_EmptyThreshold, 52 * sizeof(*core_enc->m_EmptyThreshold));
        ippsCopy_8u((const Ipp8u*)DirectBSkipMEThres, (Ipp8u*)core_enc->m_DirectBSkipMEThres, 52 * sizeof(*core_enc->m_DirectBSkipMEThres));
        ippsCopy_8u((const Ipp8u*)PSkipMEThres, (Ipp8u*)core_enc->m_PSkipMEThres, 52 * sizeof(*core_enc->m_PSkipMEThres));
        ippsCopy_8u((const Ipp8u*)BestOf5EarlyExitThres, (Ipp8u*)core_enc->m_BestOf5EarlyExitThres, 52 * sizeof(*core_enc->m_BestOf5EarlyExitThres));
    }

    Ipp32u bsSize = core_enc->m_PaddedSize.width * core_enc->m_PaddedSize.height * sizeof(PIXTYPE);
    bsSize += (bsSize >> 1) + 4096;
    // TBD: see if buffer size can be reduced

    core_enc->m_pAllocEncoderInst = (Ipp8u*)H264_Malloc(numOfSliceEncs * bsSize + DATA_ALIGN);
    if (core_enc->m_pAllocEncoderInst == NULL)
        return UMC_ERR_ALLOC;
    core_enc->m_pBitStream = align_pointer<Ipp8u*>(core_enc->m_pAllocEncoderInst, DATA_ALIGN);

    core_enc->m_pbitstreams = (H264BsReal**)H264_Malloc(numOfSliceEncs * sizeof(H264BsReal*));
    if (core_enc->m_pbitstreams == NULL)
        return UMC_ERR_ALLOC;


    for (i = 0; i < numOfSliceEncs; i++)
    {
        core_enc->m_pbitstreams[i] = (H264BsReal*)H264_Malloc(sizeof(H264BsReal));
        if (!core_enc->m_pbitstreams[i])
            return UMC_ERR_ALLOC;
        H264BsReal_Create(core_enc->m_pbitstreams[i], core_enc->m_pBitStream + i * bsSize, bsSize, core_enc->m_params.chroma_format_idc, status);
        if (status != UMC_OK)
            return status;
        core_enc->m_Slices[i].m_pbitstream = (H264BsBase*)core_enc->m_pbitstreams[i];
    }
    core_enc->m_bs1 = core_enc->m_pbitstreams[0]; // core_enc->m_bs1 is the main stream.

    Ipp32s nMBCount = core_enc->m_WidthInMBs * core_enc->m_HeightInMBs;
#ifdef DEBLOCK_THREADING
    core_enc->mbs_deblock_ready = (Ipp8u*)H264_Malloc(sizeof(Ipp8u) * nMBCount);
    memset((void*)core_enc->mbs_deblock_ready, 0, sizeof( Ipp8u )*nMBCount);
#endif

#ifdef SLICE_THREADING_LOAD_BALANCING
    // Load balancing for slice level multithreading
    core_enc->m_B_ticks_per_macroblock = (Ipp64s*)H264_Malloc(sizeof(Ipp64s) * nMBCount);
    if (!core_enc->m_B_ticks_per_macroblock)
        return UMC_ERR_ALLOC;
    core_enc->m_P_ticks_per_macroblock = (Ipp64s*)H264_Malloc(sizeof(Ipp64s) * nMBCount);
    if (!core_enc->m_P_ticks_per_macroblock)
        return UMC_ERR_ALLOC;
    core_enc->m_B_ticks_data_available = 0;
    core_enc->m_P_ticks_data_available = 0;
#endif // SLICE_THREADING_LOAD_BALANCING

    Ipp32s len = (sizeof(H264MacroblockMVs) +
                  sizeof(H264MacroblockMVs) +
                  sizeof(H264MacroblockCoeffsInfo) +
                  sizeof(H264MacroblockLocalInfo) +
                  sizeof(H264MacroblockIntraTypes) +
                  sizeof(T_EncodeMBOffsets)
                 ) * nMBCount + ALIGN_VALUE * 6;
    core_enc->m_pParsedDataNew = (Ipp8u*)H264_Malloc(len);
    if (NULL == core_enc->m_pParsedDataNew)
        return UMC_ERR_ALLOC;
    core_enc->m_mbinfo.MVDeltas[0] = align_pointer<H264MacroblockMVs *> (core_enc->m_pParsedDataNew, ALIGN_VALUE);
    core_enc->m_mbinfo.MVDeltas[1] = align_pointer<H264MacroblockMVs *> (core_enc->m_mbinfo.MVDeltas[0] + nMBCount, ALIGN_VALUE);
    core_enc->m_mbinfo.MacroblockCoeffsInfo = align_pointer<H264MacroblockCoeffsInfo *> (core_enc->m_mbinfo.MVDeltas[1] + nMBCount, ALIGN_VALUE);
    core_enc->m_mbinfo.mbs = align_pointer<H264MacroblockLocalInfo *> (core_enc->m_mbinfo.MacroblockCoeffsInfo + nMBCount, ALIGN_VALUE);
    core_enc->m_mbinfo.intra_types = align_pointer<H264MacroblockIntraTypes *> (core_enc->m_mbinfo.mbs + nMBCount, ALIGN_VALUE);
    core_enc->m_pMBOffsets = align_pointer<T_EncodeMBOffsets*> (core_enc->m_mbinfo.intra_types  + nMBCount, ALIGN_VALUE);
    // Block offset -- initialize table, indexed by current block (0-23)
    // with the value to add to the offset of the block into the plane
    // to advance to the next block
    for (i = 0; i < 16; i++) {
        // 4 Cases to cover:
        if (!(i & 1)) {   // Even # blocks, next block to the right
            core_enc->m_EncBlockOffsetInc[0][i] = 4;
            core_enc->m_EncBlockOffsetInc[1][i] = 4;
        } else if (!(i & 2)) {  // (1,5,9 & 13), down and one to the left
            core_enc->m_EncBlockOffsetInc[0][i] = (core_enc->m_Pitch<<2) - 4;
            core_enc->m_EncBlockOffsetInc[1][i] = (core_enc->m_Pitch<<3) - 4;
        } else if (i == 7) { // beginning of next row
            core_enc->m_EncBlockOffsetInc[0][i] = (core_enc->m_Pitch<<2) - 12;
            core_enc->m_EncBlockOffsetInc[1][i] = (core_enc->m_Pitch<<3) - 12;
        } else { // (3 & 11) up and one to the right
            core_enc->m_EncBlockOffsetInc[0][i] = -(Ipp32s)((core_enc->m_Pitch<<2) - 4);
            core_enc->m_EncBlockOffsetInc[1][i] = -(Ipp32s)((core_enc->m_Pitch<<3) - 4);
        }
    }
    Ipp32s last_block = 16+(4<<core_enc->m_params.chroma_format_idc) - 1; // - last increment
    core_enc->m_EncBlockOffsetInc[0][last_block] = 0;
    core_enc->m_EncBlockOffsetInc[1][last_block] = 0;
    switch (core_enc->m_params.chroma_format_idc)
    {
        case 1:
        case 2:
            for (i = 16; i < last_block; i++)
            {
                if (i & 1)
                {
                    core_enc->m_EncBlockOffsetInc[0][i] = (core_enc->m_Pitch<<2) - 4;
                    core_enc->m_EncBlockOffsetInc[1][i] = (core_enc->m_Pitch<<3) - 4;
                } else
                {
                    core_enc->m_EncBlockOffsetInc[0][i] = 4;
                    core_enc->m_EncBlockOffsetInc[1][i] = 4;
                }
            }
            break;
        case 3:
            //Copy from luma
            memcpy(&core_enc->m_EncBlockOffsetInc[0][16], &core_enc->m_EncBlockOffsetInc[0][0], 16*sizeof(Ipp32s));
            memcpy(&core_enc->m_EncBlockOffsetInc[0][32], &core_enc->m_EncBlockOffsetInc[0][0], 16*sizeof(Ipp32s));
            memcpy(&core_enc->m_EncBlockOffsetInc[1][16], &core_enc->m_EncBlockOffsetInc[1][0], 16*sizeof(Ipp32s));
            memcpy(&core_enc->m_EncBlockOffsetInc[1][32], &core_enc->m_EncBlockOffsetInc[1][0], 16*sizeof(Ipp32s));
            break;
    }
    core_enc->m_InitialOffsets[0][0] = core_enc->m_InitialOffsets[1][1] = 0;
    core_enc->m_InitialOffsets[0][1] = (Ipp32s)core_enc->m_Pitch;
    core_enc->m_InitialOffsets[1][0] = -(Ipp32s)core_enc->m_Pitch;

    Ipp32s bitDepth = IPP_MAX(core_enc->m_params.bit_depth_aux, IPP_MAX(core_enc->m_params.bit_depth_chroma, core_enc->m_params.bit_depth_luma));
    Ipp32s chromaSampling = core_enc->m_params.chroma_format_idc;
    Ipp32s alpha = core_enc->m_params.aux_format_idc ? 1 : 0;
    switch (core_enc->m_params.rate_controls.method)
    {
        case H264_RCM_CBR:
            H264_AVBR_Init(&core_enc->avbr, 8, 4, 8, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate, core_enc->m_params.m_info.videoInfo.m_iWidth * core_enc->m_params.m_info.videoInfo.m_iHeight, bitDepth, chromaSampling, alpha);
            break;
        case H264_RCM_VBR:
            H264_AVBR_Init(&core_enc->avbr, 0, 0, 0, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate, core_enc->m_params.m_info.videoInfo.m_iWidth * core_enc->m_params.m_info.videoInfo.m_iHeight, bitDepth, chromaSampling, alpha);
            break;
        case H264_RCM_CBR_SLICE:
            H264_AVBR_Init(&core_enc->avbr, 8, 4, 8, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate * core_enc->m_params.num_slices, core_enc->m_params.m_info.videoInfo.m_iWidth * core_enc->m_params.m_info.videoInfo.m_iHeight / core_enc->m_params.num_slices, bitDepth, chromaSampling, alpha);
            break;
        case H264_RCM_VBR_SLICE:
            H264_AVBR_Init(&core_enc->avbr, 0, 0, 0, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate * core_enc->m_params.num_slices, core_enc->m_params.m_info.videoInfo.m_iWidth * core_enc->m_params.m_info.videoInfo.m_iHeight / core_enc->m_params.num_slices, bitDepth, chromaSampling, alpha);
        default:
            break;
    }
    core_enc->m_iProfileIndex = 0;
    core_enc->cflags = core_enc->cnotes = 0;
    core_enc->m_pLastFrame = NULL; //Init pointer to last frame

    Ipp32s fs = core_enc->m_params.m_info.videoInfo.m_iWidth * core_enc->m_params.m_info.videoInfo.m_iHeight;
    core_enc->m_params.m_iSuggestedOutputSize = fs;
    if (alpha)
        core_enc->m_params.m_iSuggestedOutputSize += fs;
    if (chromaSampling == 1)
        core_enc->m_params.m_iSuggestedOutputSize += fs / 2;
    else if (chromaSampling == 2)
        core_enc->m_params.m_iSuggestedOutputSize += fs;
    else if (chromaSampling == 3)
        core_enc->m_params.m_iSuggestedOutputSize += fs * 2;
    core_enc->m_params.m_iSuggestedOutputSize = core_enc->m_params.m_iSuggestedOutputSize * bitDepth / 8;
#ifdef FRAME_QP_FROM_FILE
        FILE* f;
        int qp,fn=0;
        char ft;

        if( (f = fopen(FRAME_QP_FROM_FILE, "r")) == NULL){
            fprintf(stderr,"Can't open file %s\n", FRAME_QP_FROM_FILE);
            exit(-1);
        }
        while(fscanf(f,"%c %d\n",&ft,&qp) == 2){
            frame_type.push_back(ft);
            frame_qp.push_back(qp);
            fn++;
        }
        fclose(f);
        //fprintf(stderr,"frames read = %d\n",fn);
#endif

    // update params
    //core_enc->m_params.m_qualityMeasure  = 100 - (core_enc->qscale[0]+core_enc->qscale[1]+core_enc->qscale[2])*100/(3*112);
    core_enc->m_params.m_info.streamType = H264_VIDEO;
    core_enc->m_params.m_info.iProfile   = core_enc->m_params.profile_idc;
    core_enc->m_params.m_info.iLevel     = core_enc->m_params.level_idc;

    if(core_enc->m_params.rate_controls.method == H264_RCM_QUANT)
        core_enc->m_params.m_info.iBitrate = 0;

    if(core_enc->m_params.chroma_format_idc == 0)
        core_enc->m_params.m_info.videoInfo.m_colorFormat = GRAY;
    else if(core_enc->m_params.chroma_format_idc == 1)
        core_enc->m_params.m_info.videoInfo.m_colorFormat = YUV420;
    else if(core_enc->m_params.chroma_format_idc == 2)
        core_enc->m_params.m_info.videoInfo.m_colorFormat = YUV422;
    else if(core_enc->m_params.chroma_format_idc == 3)
        core_enc->m_params.m_info.videoInfo.m_colorFormat = YUV444;

    if(core_enc->m_params.aux_format_idc != 0)
    {
        if(core_enc->m_params.m_info.videoInfo.m_colorFormat == GRAY)
            core_enc->m_params.m_info.videoInfo.m_colorFormat = GRAYA;
        else if(core_enc->m_params.m_info.videoInfo.m_colorFormat == YUV420)
            core_enc->m_params.m_info.videoInfo.m_colorFormat = YUV420A;
        else if(core_enc->m_params.m_info.videoInfo.m_colorFormat == YUV422)
            core_enc->m_params.m_info.videoInfo.m_colorFormat = YUV422A;
        else if(core_enc->m_params.m_info.videoInfo.m_colorFormat == YUV444)
            core_enc->m_params.m_info.videoInfo.m_colorFormat = YUV444A;

        core_enc->m_params.m_info.videoInfo.SetPlaneBitDepth(core_enc->m_params.bit_depth_aux, 3);
    }

    core_enc->m_params.m_info.videoInfo.SetPlaneBitDepth(core_enc->m_params.bit_depth_luma, 0);
    core_enc->m_params.m_info.videoInfo.SetPlaneBitDepth(core_enc->m_params.bit_depth_chroma, 1);
    core_enc->m_params.m_info.videoInfo.SetPlaneBitDepth(core_enc->m_params.bit_depth_chroma, 2);

    return UMC_OK;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_GetFrame(void* state, MediaData *in, MediaData *out)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status res = UMC_OK;
    bool bPrintGOP = false;

    if (in)
    {
        VideoData *vin = DynamicCast<VideoData, MediaData> (in);
        if(!vin) // Only VideoData compatible objects are allowed.
            return UMC_ERR_INVALID_STREAM;

        if(core_enc->m_SeqParamSet.aux_format_idc != 0 && ( (core_enc->m_params.chroma_format_idc != 0 && vin->GetPlanesNumber()<4) || (core_enc->m_params.chroma_format_idc == 0 && vin->GetPlanesNumber() != 2) ))
                return(UMC_ERR_INVALID_STREAM);

        res = H264CoreEncoder_Encode<COEFFSTYPE, PIXTYPE>(state, vin, out, core_enc->cflags, core_enc->cnotes);
        if(core_enc->cnotes & H264_ECN_NO_FRAME)
            return UMC_OK;
    }
    else
    {
        core_enc->cflags |= H264_ECF_LAST_FRAME;
        res = H264CoreEncoder_Encode<COEFFSTYPE, PIXTYPE>(state, NULL, out, core_enc->cflags, core_enc->cnotes);
        if (core_enc->cnotes & H264_ECN_NO_FRAME)
            return UMC_ERR_END_OF_STREAM;
    }
    // Set FrameType
    if (NULL != out && UMC_OK == res)
    {
        FrameType frame_type = NONE_PICTURE;
        switch (core_enc->m_pCurrentFrame->m_PicCodType)
        {
        case INTRAPIC: frame_type = I_PICTURE;
            if(bPrintGOP)
            {
                if(core_enc->m_pCurrentFrame->m_bIsIDRPic)
                    printf("(I)");
                else
                    printf("I");
            }
            break;
        case PREDPIC:  frame_type = P_PICTURE; if(bPrintGOP) printf("P"); break;
        case BPREDPIC: frame_type = B_PICTURE; if(bPrintGOP) printf("B"); break;
        }
        out->m_frameType = frame_type;
    }
    return(res);
}

template<typename COEFFSTYPE, typename PIXTYPE>
VideoData* H264CoreEncoder_GetReconstructedFrame(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s makeNULL = 0;

   //Deblock for non referece B frames
   if( core_enc->m_pCurrentFrame->m_PicCodType == BPREDPIC && !core_enc->m_pCurrentFrame->m_RefPic )
   {
        if( core_enc->m_Analyse & ANALYSE_RECODE_FRAME && core_enc->m_pReconstructFrame == NULL)
        {
            core_enc->m_pReconstructFrame = core_enc->m_pCurrentFrame;
            makeNULL = 1;
        }
        Ipp32s field_index, slice;
        for (field_index=0;field_index<=(Ipp8u)(core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE);field_index++)
        {
            for (slice = (Ipp32s)core_enc->m_params.num_slices*field_index; slice < core_enc->m_params.num_slices*(field_index+1); slice++)
            {
                H264CoreEncoder_DeblockSlice(
                    state,
                    core_enc->m_Slices + slice,
                    core_enc->m_Slices[slice].m_first_mb_in_slice + core_enc->m_WidthInMBs * core_enc->m_HeightInMBs * field_index,
                    core_enc->m_Slices[slice].m_MB_Counter );
            }
        }
    }
    if( core_enc->m_Analyse & ANALYSE_RECODE_FRAME && makeNULL )
        core_enc->m_pReconstructFrame = NULL;

    return &core_enc->m_pCurrentFrame->m_data;
}

// Get codec working (initialization) parameter(s)
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_GetInfo(void* state, BaseCodecParams *info)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264EncoderParams* pInfo = DynamicCast<H264EncoderParams, BaseCodecParams>(info);

    if(pInfo)
        *pInfo = core_enc->m_params;
    else if(info)
    {
        VideoEncoderParams* pInfo = DynamicCast<VideoEncoderParams, BaseCodecParams>(info);
        if(pInfo)
            *pInfo = core_enc->m_params;
        else
            return UMC_ERR_INVALID_STREAM;
    }
    else
        return UMC_ERR_NULL_PTR;

    return UMC_OK;
}

template<typename COEFFSTYPE, typename PIXTYPE>
const H264SeqParamSet* H264CoreEncoder_GetSeqParamSet(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    return &core_enc->m_SeqParamSet;
}

template<typename COEFFSTYPE, typename PIXTYPE>
const H264PicParamSet* H264CoreEncoder_GetPicParamSet(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    return &core_enc->m_PicParamSet;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Close(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;

    if (core_enc->m_EmptyThreshold)
    {
        H264_Free(core_enc->m_EmptyThreshold);
        core_enc->m_EmptyThreshold = NULL;
    }
    if(core_enc->m_DirectBSkipMEThres)
    {
        H264_Free(core_enc->m_DirectBSkipMEThres);
        core_enc->m_DirectBSkipMEThres = NULL;
    }
    if(core_enc->m_PSkipMEThres)
    {
        H264_Free(core_enc->m_PSkipMEThres);
        core_enc->m_PSkipMEThres = NULL;
    }
    if(core_enc->m_BestOf5EarlyExitThres)
    {
        H264_Free(core_enc->m_BestOf5EarlyExitThres);
        core_enc->m_BestOf5EarlyExitThres = NULL;
    }
    if (core_enc->m_pbitstreams)
    {
        Ipp32s i;
        for (i = 0; i < core_enc->m_params.num_slices*((core_enc->m_params.coding_type == 1) + 1); i++)
        {  //TODO fix for PicAFF/AFRM
            if (core_enc->m_pbitstreams[i])
            {
                H264_Free(core_enc->m_pbitstreams[i]);
                core_enc->m_pbitstreams[i] = NULL;
            }
        }
        H264_Free(core_enc->m_pbitstreams);
        core_enc->m_pbitstreams = NULL;
    }
    if (core_enc->m_pAllocEncoderInst)
    {
        H264_Free(core_enc->m_pAllocEncoderInst);
        core_enc->m_pAllocEncoderInst = NULL;
    }
    if( core_enc->eFrameType != NULL )
    {
        H264_Free(core_enc->eFrameType);
        core_enc->eFrameType = NULL;
    }
    if( core_enc->eFrameSeq != NULL )
    {
        H264_Free(core_enc->eFrameSeq);
        core_enc->eFrameSeq = NULL;
    }
    core_enc->m_pReconstructFrame = NULL;
    if (core_enc->m_pParsedDataNew)
    {
        H264_Free(core_enc->m_pParsedDataNew);
        core_enc->m_pParsedDataNew = NULL;
    }
    if (core_enc->m_Slices != NULL)
    {
        if(core_enc->m_Slices)
        {
            size_t* _alloc_ptr = (size_t*)core_enc->m_Slices - 1;
            for (size_t i = _alloc_ptr[0]; i > 0; i--)
                H264Slice_Destroy<COEFFSTYPE, PIXTYPE>((H264Slice<COEFFSTYPE, PIXTYPE>*)core_enc->m_Slices + i - 1);
            H264_Free(_alloc_ptr);
            core_enc->m_Slices = NULL;
        }
    }
#ifdef DEBLOCK_THREADING
    if(core_enc->mbs_deblock_ready != NULL)
    {
        H264_Free((void*)core_enc->mbs_deblock_ready);
        core_enc->mbs_deblock_ready = NULL;
    }
#endif

#ifdef SLICE_THREADING_LOAD_BALANCING
    // Load balancing for slice level multithreading
    if(core_enc->m_B_ticks_per_macroblock != NULL)
    {
        H264_Free(core_enc->m_B_ticks_per_macroblock);
        core_enc->m_B_ticks_per_macroblock = NULL;
    }
    if(core_enc->m_P_ticks_per_macroblock != NULL)
    {
        H264_Free(core_enc->m_P_ticks_per_macroblock);
        core_enc->m_P_ticks_per_macroblock = NULL;
    }
#endif // SLICE_THREADING_LOAD_BALANCING
    return UMC_OK;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Reset(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s i;

    H264CoreEncoder_Close<COEFFSTYPE, PIXTYPE>(state);

    core_enc->profile_frequency = 1;
    core_enc->m_iProfileIndex = 0;
    core_enc->m_is_mb_data_initialized = false;
    if( core_enc->eFrameType == NULL )
    {
        core_enc->eFrameType = (EnumPicCodType *)H264_Malloc(sizeof(EnumPicCodType));
        core_enc->eFrameSeq  = (H264EncoderFrame<PIXTYPE>**)H264_Malloc(sizeof(H264EncoderFrame<PIXTYPE>*));
        core_enc->eFrameType[0] = PREDPIC;
    }
    core_enc->m_uIntraFrameInterval = 0;
    core_enc->m_uIDRFrameInterval = 0;
    core_enc->m_PicOrderCnt = 0;
    core_enc->m_PicOrderCnt_Accu = 0;
    core_enc->m_l1_cnt_to_start_B = 0;
    core_enc->m_total_bits_encoded = 0;
    core_enc->use_implicit_weighted_bipred = false;
    core_enc->m_is_cur_pic_afrm = false;
    core_enc->m_PaddedSize.width = core_enc->m_PaddedSize.height = 0;
    core_enc->m_DirectTypeStat[0]=core_enc->m_DirectTypeStat[1]=0;
#if 0
    core_enc->m_pBitStream = core_enc->memAlloc =
    core_enc->m_pAllocEncoderInst = core_enc->m_pbitstreams = core_enc->m_bs1 = core_enc->m_dpb = core_enc->m_cpb =
    core_enc->m_pParsedDataNew = core_enc->m_pReconstructFrame = core_enc->m_pMBOffsets =
    core_enc->m_EmptyThreshold = core_enc->m_DirectBSkipMEThres = core_enc->m_PSkipMEThres =
    core_enc->m_BestOf5EarlyExitThres = core_enc->m_Slices = NULL;
#endif
    // Initialize the BRCState local variables based on the default
    // settings in core_enc->m_params.

    // If these assertions fail, then uTargetFrmSize needs to be set to
    // something other than 0.
    // Initialize the sequence parameter set structure.

    core_enc->m_SeqParamSet.profile_idc = H264_PROFILE_MAIN;
    core_enc->m_SeqParamSet.level_idc = 0;
    core_enc->m_SeqParamSet.constraint_set0_flag = 0;
    core_enc->m_SeqParamSet.constraint_set1_flag = 0;
    core_enc->m_SeqParamSet.constraint_set2_flag = 0;
    core_enc->m_SeqParamSet.chroma_format_idc = 1;
    core_enc->m_SeqParamSet.seq_parameter_set_id = 0;
    core_enc->m_SeqParamSet.log2_max_frame_num = 0;
    core_enc->m_SeqParamSet.pic_order_cnt_type = 0;
    core_enc->m_SeqParamSet.delta_pic_order_always_zero_flag = 0;
    core_enc->m_SeqParamSet.frame_mbs_only_flag = 0;
    core_enc->m_SeqParamSet.gaps_in_frame_num_value_allowed_flag = 0;
    core_enc->m_SeqParamSet.mb_adaptive_frame_field_flag = 0;
    core_enc->m_SeqParamSet.direct_8x8_inference_flag = 0;
    core_enc->m_SeqParamSet.vui_parameters_present_flag = 0;
    core_enc->m_SeqParamSet.log2_max_pic_order_cnt_lsb = 0;
    core_enc->m_SeqParamSet.offset_for_non_ref_pic = 0;
    core_enc->m_SeqParamSet.offset_for_top_to_bottom_field = 0;
    core_enc->m_SeqParamSet.num_ref_frames_in_pic_order_cnt_cycle = 0;
    core_enc->m_SeqParamSet.poffset_for_ref_frame = NULL;
    core_enc->m_SeqParamSet.num_ref_frames = 0;
    core_enc->m_SeqParamSet.frame_width_in_mbs = 0;
    core_enc->m_SeqParamSet.frame_height_in_mbs = 0;
    core_enc->m_SeqParamSet.frame_cropping_flag = 0;
    core_enc->m_SeqParamSet.frame_crop_left_offset = 0;
    core_enc->m_SeqParamSet.frame_crop_right_offset = 0;
    core_enc->m_SeqParamSet.frame_crop_top_offset = 0;
    core_enc->m_SeqParamSet.frame_crop_bottom_offset = 0;
    core_enc->m_SeqParamSet.bit_depth_luma = 8;
    core_enc->m_SeqParamSet.bit_depth_chroma = 8;
    core_enc->m_SeqParamSet.bit_depth_aux = 8;
    core_enc->m_SeqParamSet.alpha_incr_flag = 0;
    core_enc->m_SeqParamSet.alpha_opaque_value = 8;
    core_enc->m_SeqParamSet.alpha_transparent_value = 0;
    core_enc->m_SeqParamSet.aux_format_idc = 0;
    core_enc->m_SeqParamSet.seq_scaling_matrix_present_flag = false;
    for( i=0; i<8; i++)
        core_enc->m_SeqParamSet.seq_scaling_list_present_flag[i]=false;
    core_enc->m_SeqParamSet.pack_sequence_extension = false;
    core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag = 0;
    core_enc->m_SeqParamSet.residual_colour_transform_flag = false;
    core_enc->m_SeqParamSet.additional_extension_flag = 0;

    // Initialize the picture parameter set structure.

    core_enc->m_PicParamSet.pic_parameter_set_id = 0;
    core_enc->m_PicParamSet.seq_parameter_set_id = 0;
    core_enc->m_PicParamSet.entropy_coding_mode = 0;
    core_enc->m_PicParamSet.pic_order_present_flag = 0;
    core_enc->m_PicParamSet.weighted_pred_flag = 0;
    core_enc->m_PicParamSet.weighted_bipred_idc = 0;
    core_enc->m_PicParamSet.pic_init_qp = 0;
    core_enc->m_PicParamSet.pic_init_qs = 0;
    core_enc->m_PicParamSet.chroma_qp_index_offset = 0;
    core_enc->m_PicParamSet.deblocking_filter_variables_present_flag = 0;
    core_enc->m_PicParamSet.constrained_intra_pred_flag = 0;
    core_enc->m_PicParamSet.redundant_pic_cnt_present_flag = 0;
    core_enc->m_PicParamSet.num_slice_groups = 1;
    core_enc->m_PicParamSet.SliceGroupInfo.slice_group_map_type = 0;
    core_enc->m_PicParamSet.SliceGroupInfo.t3.pic_size_in_map_units = 0;
    core_enc->m_PicParamSet.SliceGroupInfo.t3.pSliceGroupIDMap = NULL;
    core_enc->m_PicParamSet.num_ref_idx_l0_active = 0;
    core_enc->m_PicParamSet.num_ref_idx_l1_active = 0;

    core_enc->m_PicParamSet.second_chroma_qp_index_offset = core_enc->m_PicParamSet.chroma_qp_index_offset;
    core_enc->m_PicParamSet.pic_scaling_matrix_present_flag = 0;
    core_enc->m_PicParamSet.transform_8x8_mode_flag = false;

    // Initialize the slice header structure.
    core_enc->m_SliceHeader.pic_parameter_set_id = 0;
    core_enc->m_SliceHeader.field_pic_flag = 0;
    core_enc->m_SliceHeader.MbaffFrameFlag = 0;
    core_enc->m_SliceHeader.bottom_field_flag = 0;
    core_enc->m_SliceHeader.direct_spatial_mv_pred_flag = 1;
    core_enc->m_SliceHeader.long_term_reference_flag = 0;
    core_enc->m_SliceHeader.sp_for_switch_flag = 0;
    core_enc->m_SliceHeader.slice_qs_delta = 0;
    core_enc->m_SliceHeader.frame_num = 0;
    core_enc->m_SliceHeader.idr_pic_id = 0;
    core_enc->m_SliceHeader.pic_order_cnt_lsb = 0;
    core_enc->m_SliceHeader.delta_pic_order_cnt[0] = 0;
    core_enc->m_SliceHeader.delta_pic_order_cnt[1] = 0;
    core_enc->m_SliceHeader.redundant_pic_cnt = 0;
    core_enc->m_SliceHeader.slice_group_change_cycle = 0;
    core_enc->m_SliceHeader.delta_pic_order_cnt_bottom = 0;

    //Clear dpb and cpb
    H264EncoderFrameList_clearFrameList<PIXTYPE>(&core_enc->m_cpb);
    H264EncoderFrameList_clearFrameList<PIXTYPE>(&core_enc->m_dpb);

    return H264CoreEncoder_Init<COEFFSTYPE, PIXTYPE>(state, &core_enc->m_params, core_enc->memAlloc);
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_SetParams(
    void* state,
    BaseCodecParams* params)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE> *)state;
    VideoEncoderParams *gen_info = DynamicCast<VideoEncoderParams, BaseCodecParams> (params);
    //H264EncoderParams *h264_info = DynamicCast<H264EncoderParams, BaseCodecParams> (params);

    if (gen_info == NULL)
        return UMC_ERR_NULL_PTR;
    if (gen_info->m_info.iBitrate == core_enc->m_params.m_info.iBitrate && gen_info->m_info.fFramerate == core_enc->m_params.m_info.fFramerate)
        return UMC_ERR_UNSUPPORTED;
    if (gen_info->m_info.iBitrate != core_enc->m_params.m_info.iBitrate || gen_info->m_info.fFramerate != core_enc->m_params.m_info.fFramerate) {
        core_enc->m_params.m_info.iBitrate = gen_info->m_info.iBitrate;
        core_enc->m_params.m_info.fFramerate = gen_info->m_info.fFramerate;
        Ipp32s bitDepth = IPP_MAX(core_enc->m_params.bit_depth_aux, IPP_MAX(core_enc->m_params.bit_depth_chroma, core_enc->m_params.bit_depth_luma));
        Ipp32s chromaSampling = core_enc->m_params.chroma_format_idc;
        Ipp32s alpha = core_enc->m_params.aux_format_idc ? 1 : 0;
        if (core_enc->m_params.rate_controls.method == H264_RCM_CBR) {
            H264_AVBR_Init(&core_enc->avbr, 8, 4, 8, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate, core_enc->m_params.m_info.videoInfo.m_iWidth*core_enc->m_params.m_info.videoInfo.m_iHeight, bitDepth, chromaSampling, alpha);
        } else if (core_enc->m_params.rate_controls.method == H264_RCM_VBR) {
            H264_AVBR_Init(&core_enc->avbr, 0, 0, 0, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate, core_enc->m_params.m_info.videoInfo.m_iWidth*core_enc->m_params.m_info.videoInfo.m_iHeight, bitDepth, chromaSampling, alpha);
        } else if (core_enc->m_params.rate_controls.method == H264_RCM_CBR_SLICE) {
            H264_AVBR_Init(&core_enc->avbr, 8, 4, 8, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate*core_enc->m_params.num_slices, core_enc->m_params.m_info.videoInfo.m_iWidth*core_enc->m_params.m_info.videoInfo.m_iHeight/core_enc->m_params.num_slices, bitDepth, chromaSampling, alpha);
        } else if (core_enc->m_params.rate_controls.method == H264_RCM_VBR_SLICE) {
            H264_AVBR_Init(&core_enc->avbr, 0, 0, 0, core_enc->m_params.m_info.iBitrate, core_enc->m_params.m_info.fFramerate*core_enc->m_params.num_slices, core_enc->m_params.m_info.videoInfo.m_iWidth*core_enc->m_params.m_info.videoInfo.m_iHeight/core_enc->m_params.num_slices, bitDepth, chromaSampling, alpha);
        }else
            return UMC_ERR_UNSUPPORTED;
    }
    return UMC_OK;
}


template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_AdjustRefPicListForFields(void* state, H264EncoderFrame<PIXTYPE> **pRefPicList,Ipp8s *pFields)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264EncoderFrame<PIXTYPE> *TempList[MAX_NUM_REF_FRAMES+1];
    Ipp8u TempFields[MAX_NUM_REF_FRAMES+1];
    //walk through list and set correct indices
    Ipp32s i=0,j=0,numSTR=0,numLTR=0;
    Ipp32s num_same_parity=0,num_opposite_parity=0;
    Ipp8s current_parity = core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index];

    //first scan the list to determine number of shortterm and longterm reference frames
    while (pRefPicList[numSTR] && H264EncoderFrame_isShortTermRef0(pRefPicList[numSTR]))
        numSTR++;
    while (pRefPicList[numSTR+numLTR] && H264EncoderFrame_isLongTermRef0(pRefPicList[numSTR + numLTR]))
        numLTR++;
    while(num_same_parity<numSTR ||  num_opposite_parity<numSTR)
    {
        //try to fill shorttermref fields with the same parity first
        if (num_same_parity<numSTR)
        {
            Ipp32s ref_field = H264EncoderFrame_GetNumberByParity(
                pRefPicList[num_same_parity],
                current_parity);

            while (num_same_parity<numSTR &&
                !H264EncoderFrame_isShortTermRef1(pRefPicList[num_same_parity], ref_field))
                num_same_parity++;
            if (num_same_parity<numSTR)
            {
                TempList[i] = pRefPicList[num_same_parity];
                TempFields[i] = current_parity;
                i++;
                num_same_parity++;
            }
        }
        //now process opposite parity
        if (num_opposite_parity<numSTR)
        {
            Ipp32s ref_field = H264EncoderFrame_GetNumberByParity(
                pRefPicList[num_opposite_parity],
                !current_parity);

            while (num_opposite_parity < numSTR &&
                !H264EncoderFrame_isShortTermRef1(pRefPicList[num_opposite_parity], ref_field))
                num_opposite_parity++;
            if (num_opposite_parity<numSTR) //selected field is reference
            {
                TempList[i] = pRefPicList[num_opposite_parity];
                TempFields[i] = !current_parity;
                i++;
                num_opposite_parity++;
            }
        }
    }
    core_enc->m_NumShortEntriesInList = (Ipp8u) i;
    num_same_parity=num_opposite_parity=0;
    //now processing LongTermRef
    while(num_same_parity<numLTR ||  num_opposite_parity<numLTR)
    {
        //try to fill longtermref fields with the same parity first
        if (num_same_parity<numLTR)
        {
            Ipp32s ref_field = H264EncoderFrame_GetNumberByParity(
                pRefPicList[num_same_parity + numSTR],
                current_parity);

            while (num_same_parity < numLTR &&
                !H264EncoderFrame_isLongTermRef1(
                    pRefPicList[num_same_parity + numSTR],
                    ref_field))
                num_same_parity++;
            if (num_same_parity<numLTR)
            {
                TempList[i] = pRefPicList[num_same_parity+numSTR];
                TempFields[i] = current_parity;
                i++;
                num_same_parity++;
            }
        }
        //now process opposite parity
        if (num_opposite_parity<numLTR)
        {
            Ipp32s ref_field = H264EncoderFrame_GetNumberByParity(
                pRefPicList[num_opposite_parity + numSTR],
                !current_parity);

            while (num_opposite_parity < numLTR &&
                !H264EncoderFrame_isLongTermRef1(
                    pRefPicList[num_opposite_parity + numSTR],
                    ref_field))
                num_opposite_parity++;
            if (num_opposite_parity<numLTR) //selected field is reference
            {
                TempList[i] = pRefPicList[num_opposite_parity+numSTR];
                TempFields[i] = !current_parity;
                i++;
                num_opposite_parity++;
            }
        }
    }
    core_enc->m_NumLongEntriesInList = (Ipp8u) (i - core_enc->m_NumShortEntriesInList);
    j=0;
#ifdef STORE_PICLIST
    if (refpic==NULL) refpic=fopen(__PICLIST_FILE__,VM_STRING("wt"));
    if (refpic)
        fprintf(refpic,"Reordering for fields:\n");
#endif
    while(j<i)//copy data back to list
    {
        pRefPicList[j]=TempList[j];
        pFields[j] = TempFields[j];
#ifdef STORE_PICLIST
        if (refpic)
            fprintf(refpic,"Entry %d poc %d parity %d\n",
                j,
                H264EncoderFrame_PicOrderCnt(
                    pRefPicList[j],
                    H264EncoderFrame_GetNumberByParity(
                        pRefPicList[j],
                        pFields[j]),
                    1),
                pFields[j]);
#endif

        j++;
    }
#ifdef STORE_PICLIST
    if (refpic){
        fprintf(refpic,"\n");
        fflush(refpic);
    }
#endif
    while(j<MAX_NUM_REF_FRAMES)//fill remaining entries
    {
        pRefPicList[j]=NULL;
        pFields[j] = -1;
        j++;
    }
    return;
}

//////////////////////////////////////////////////////////////////////////////
// ReOrderRefPicList
//  Use reordering info from the slice header to reorder (update) L0 or L1
//  reference picture list.
//////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_ReOrderRefPicList(void* state, bool bIsFieldSlice, H264EncoderFrame<PIXTYPE> **pRefPicList, Ipp8s *pFields,
    RefPicListReorderInfo *pReorderInfo, Ipp32s MaxPicNum, Ipp32s NumRefActive)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u i;
    Ipp32s j;
    Ipp32s PicNumNoWrap;
    Ipp32s PicNum;
    Ipp32s PicNumPred;
    Ipp32s PicNumCurr;
    H264EncoderFrame<PIXTYPE> *tempFrame[2];
    Ipp8s tempFields[2];
    Ipp32u NumDuplicates;

    // Reference: Reordering process for reference picture lists, 8.2.4.3
    if (!bIsFieldSlice)
    {
        PicNumCurr = H264EncoderFrame_PicNum(core_enc->m_pCurrentFrame, 0, 3);
        PicNumPred = PicNumCurr;

        for (i=0; i<pReorderInfo->num_entries; i++)
        {
            if (pReorderInfo->reordering_of_pic_nums_idc[i] < 2)
            {
                // short term reorder
                if (pReorderInfo->reordering_of_pic_nums_idc[i] == 0)
                {
                    PicNumNoWrap = PicNumPred - pReorderInfo->reorder_value[i];
                    if (PicNumNoWrap < 0)
                        PicNumNoWrap += MaxPicNum;
                }
                else
                {
                    PicNumNoWrap = PicNumPred + pReorderInfo->reorder_value[i];
                    if (PicNumNoWrap >= MaxPicNum)
                        PicNumNoWrap -= MaxPicNum;
                }
                PicNumPred = PicNumNoWrap;

                PicNum = PicNumNoWrap;
                if (PicNum > PicNumCurr)
                    PicNum -= MaxPicNum;

                // Find the PicNum frame.
                for (j=0; pRefPicList[j] !=NULL; j++)
                    if (pRefPicList[j] != NULL &&
                        H264EncoderFrame_isShortTermRef0(pRefPicList[j]) &&
                        H264EncoderFrame_PicNum(pRefPicList[j], 0, 3) == PicNum)
                        break;

                // error if not found, should not happen
                VM_ASSERT(pRefPicList[j]);

                // Place picture with PicNum on list, shifting pictures
                // down by one while removing any duplication of picture with PicNum.
                tempFrame[0] = pRefPicList[j];    // PicNum frame just found
                NumDuplicates = 0;
                for (j=i; j<NumRefActive || pRefPicList[j] !=NULL; j++)
                {
                    if (NumDuplicates == 0)
                    {
                        // shifting pictures down
                        tempFrame[1] = pRefPicList[j];
                        pRefPicList[j] = tempFrame[0];
                        tempFrame[0] = tempFrame[1];
                    }
                    else if (NumDuplicates == 1)
                    {
                        // one duplicate of PicNum made room for new entry, just
                        // look for more duplicates to eliminate
                        tempFrame[0] = pRefPicList[j];
                    }
                    else
                    {
                        // >1 duplicate found, shifting pictures up
                        pRefPicList[j - NumDuplicates + 1] = tempFrame[0];
                        tempFrame[0] = pRefPicList[j];
                    }
                    if (tempFrame[0] == NULL)
                        break;        // end of valid reference frames
                    if (H264EncoderFrame_isShortTermRef0(tempFrame[0]) &&
                        H264EncoderFrame_PicNum(tempFrame[0], 0, 3) == PicNum)
                        NumDuplicates++;
                }
            }    // short term reorder
            else
            {
                // long term reorder
                PicNum = pReorderInfo->reorder_value[i];

                // Find the PicNum frame.
                for (j=0; pRefPicList[j] !=NULL; j++)
                    if (pRefPicList[j] != NULL &&
                        H264EncoderFrame_isLongTermRef0(pRefPicList[j]) &&
                        H264EncoderFrame_LongTermPicNum(pRefPicList[j], 0, 3) == PicNum)
                        break;

                // error if not found, should not happen
                VM_ASSERT(pRefPicList[j]);

                // Place picture with PicNum on list, shifting pictures
                // down by one while removing any duplication of picture with PicNum.
                tempFrame[0] = pRefPicList[j];    // PicNum frame just found
                NumDuplicates = 0;
                for (j=i; j<NumRefActive || pRefPicList[j] !=NULL; j++)
                {
                    if (NumDuplicates == 0)
                    {
                        // shifting pictures down
                        tempFrame[1] = pRefPicList[j];
                        pRefPicList[j] = tempFrame[0];
                        tempFrame[0] = tempFrame[1];
                    }
                    else if (NumDuplicates == 1)
                    {
                        // one duplicate of PicNum made room for new entry, just
                        // look for more duplicates to eliminate
                        tempFrame[0] = pRefPicList[j];
                    }
                    else
                    {
                        // >1 duplicate found, shifting pictures up
                        pRefPicList[j - NumDuplicates + 1] = tempFrame[0];
                        tempFrame[0] = pRefPicList[j];
                    }
                    if (tempFrame[0] == NULL)
                        break;        // end of valid reference frames
                    if (H264EncoderFrame_isLongTermRef0(tempFrame[0]) &&
                        H264EncoderFrame_LongTermPicNum(tempFrame[0], 0, 3) == PicNum)
                        NumDuplicates++;
                }
            }    // long term reorder
        }    // for i
    }
    else
    {
        //NumRefActive<<=1;
        PicNumCurr = H264EncoderFrame_PicNum(
            core_enc->m_pCurrentFrame,
            core_enc->m_field_index,
            0);
        PicNumPred = PicNumCurr;

        for (i=0; i<pReorderInfo->num_entries; i++)
        {
            if (pReorderInfo->reordering_of_pic_nums_idc[i] < 2)
            {
                // short term reorder
                if (pReorderInfo->reordering_of_pic_nums_idc[i] == 0)
                {
                    PicNumNoWrap = PicNumPred - pReorderInfo->reorder_value[i];
                    if (PicNumNoWrap < 0)
                        PicNumNoWrap += MaxPicNum;
                }
                else
                {
                    PicNumNoWrap = PicNumPred + pReorderInfo->reorder_value[i];
                    if (PicNumNoWrap >= MaxPicNum)
                        PicNumNoWrap -= MaxPicNum;
                }
                PicNumPred = PicNumNoWrap;

                PicNum = PicNumNoWrap;
                if (PicNum > PicNumCurr)
                    PicNum -= MaxPicNum;

                // Find the PicNum frame.
                for (j=0; pRefPicList[j] !=NULL; j++)
                    if (pRefPicList[j] != NULL &&
                        H264EncoderFrame_isShortTermRef1(
                            pRefPicList[j],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[j],
                                pFields[j])) &&
                        H264EncoderFrame_PicNum(
                            pRefPicList[j],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[j],
                                pFields[j]),
                            1) == PicNum)
                        break;

                // error if not found, should not happen
                VM_ASSERT(pRefPicList[j]);

                // Place picture with PicNum on list, shifting pictures
                // down by one while removing any duplication of picture with PicNum.
                tempFrame[0] = pRefPicList[j];    // PicNum frame just found
                tempFields[0] = pFields[j];

                NumDuplicates = 0;
                for (j=i; j<NumRefActive || pRefPicList[j] !=NULL; j++)
                {
                    if (NumDuplicates == 0)
                    {
                        // shifting pictures down
                        tempFrame[1] = pRefPicList[j];
                        pRefPicList[j] = tempFrame[0];
                        tempFrame[0] = tempFrame[1];

                        tempFields[1] = pFields[j];
                        pFields[j] = tempFields[0];
                        tempFields[0] = tempFields[1];
                    }
                    else if (NumDuplicates == 1)
                    {
                        // one duplicate of PicNum made room for new entry, just
                        // look for more duplicates to eliminate
                        tempFrame[0] = pRefPicList[j];
                        tempFields[0] = pFields[j];

                    }
                    else
                    {
                        // >1 duplicate found, shifting pictures up
                        pRefPicList[j - NumDuplicates + 1] = tempFrame[0];
                        tempFrame[0] = pRefPicList[j];

                        pFields[j - NumDuplicates + 1] = tempFields[0];
                        tempFields[0] = pFields[j];

                    }
                    if (tempFrame[0] == NULL)
                        break;        // end of valid reference frames
                    if (H264EncoderFrame_isShortTermRef1(
                            tempFrame[0],
                            H264EncoderFrame_GetNumberByParity(
                                tempFrame[0],
                                tempFields[0])) &&
                        H264EncoderFrame_PicNum(
                            tempFrame[0],
                            H264EncoderFrame_GetNumberByParity(
                                tempFrame[0],
                                tempFields[0]),
                            1) == PicNum)
                        NumDuplicates++;
                }
            }    // short term reorder
            else
            {
                // long term reorder
                PicNum = pReorderInfo->reorder_value[i];

                // Find the PicNum frame.
                for (j=0; pRefPicList[j] !=NULL; j++)
                    if (pRefPicList[j] != NULL &&
                        H264EncoderFrame_isLongTermRef1(
                            pRefPicList[j],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[j],
                                pFields[j])) &&
                        H264EncoderFrame_LongTermPicNum(
                            pRefPicList[j],
                            H264EncoderFrame_GetNumberByParity(
                                pRefPicList[j],
                                pFields[j]),
                            1) == PicNum)
                        break;

                // error if not found, should not happen
                VM_ASSERT(pRefPicList[j]);

                // Place picture with PicNum on list, shifting pictures
                // down by one while removing any duplication of picture with PicNum.
                tempFrame[0] = pRefPicList[j];    // PicNum frame just found
                tempFields[0] = pFields[j];

                NumDuplicates = 0;
                for (j=i; j<NumRefActive || pRefPicList[j] !=NULL; j++)
                {
                    if (NumDuplicates == 0)
                    {
                        // shifting pictures down
                        tempFrame[1] = pRefPicList[j];
                        pRefPicList[j] = tempFrame[0];
                        tempFrame[0] = tempFrame[1];

                        tempFields[1] = pFields[j];
                        pFields[j] = tempFields[0];
                        tempFields[0] = tempFields[1];
                    }
                    else if (NumDuplicates == 1)
                    {
                        // one duplicate of PicNum made room for new entry, just
                        // look for more duplicates to eliminate
                        tempFrame[0] = pRefPicList[j];
                        tempFields[0] = pFields[j];
                    }
                    else
                    {
                        // >1 duplicate found, shifting pictures up
                        pRefPicList[j - NumDuplicates + 1] = tempFrame[0];
                        tempFrame[0] = pRefPicList[j];

                        pFields[j - NumDuplicates + 1] = tempFields[0];
                        tempFields[0] = pFields[j];
                    }
                    if (tempFrame[0] == NULL)
                        break;        // end of valid reference frames
                    if (H264EncoderFrame_isLongTermRef1(
                            tempFrame[0],
                            H264EncoderFrame_GetNumberByParity(
                                tempFrame[0],
                                tempFields[0])) &&
                        H264EncoderFrame_LongTermPicNum(
                            tempFrame[0],
                            H264EncoderFrame_GetNumberByParity(
                                tempFrame[0],
                                tempFields[0]),
                            1) == PicNum)
                        NumDuplicates++;
                }
            }    // long term reorder
        }    // for i
    }
}    // ReOrderRefPicList

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_UpdateRefPicList(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, EncoderRefPicList<PIXTYPE> * ref_pic_list, H264SliceHeader &SHdr,
    RefPicListReorderInfo *pReorderInfo_L0, RefPicListReorderInfo *pReorderInfo_L1)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    EnumSliceType slice_type = curr_slice->m_slice_type;
    Status ps = UMC_OK;
    Ipp32u NumShortTermRefs, NumLongTermRefs;
    H264SeqParamSet *sps;
    Ipp32u uMaxFrameNum;
    Ipp32u uMaxPicNum;

    VM_ASSERT(core_enc->m_pCurrentFrame);

    H264EncoderFrame<PIXTYPE> **pRefPicList0 = ref_pic_list->m_RefPicListL0.m_RefPicList;
    H264EncoderFrame<PIXTYPE> **pRefPicList1 = ref_pic_list->m_RefPicListL1.m_RefPicList;
    Ipp8s *pFields0 = ref_pic_list->m_RefPicListL0.m_Prediction;
    Ipp8s *pFields1 = ref_pic_list->m_RefPicListL1.m_Prediction;
    Ipp32s * pPOC0 = ref_pic_list->m_RefPicListL0.m_POC;
    Ipp32s * pPOC1 = ref_pic_list->m_RefPicListL1.m_POC;


    curr_slice->m_NumRefsInL0List = 0;
    curr_slice->m_NumRefsInL1List = 0;

    // Spec reference: 8.2.4, "Decoding process for reference picture lists
    // construction"

    // get pointers to the picture and sequence parameter sets currently in use
    //pps = &core_enc->m_PicParamSet;
    sps = &core_enc->m_SeqParamSet;
    uMaxFrameNum = (1<<sps->log2_max_frame_num);
    uMaxPicNum = (SHdr.field_pic_flag == 0) ? uMaxFrameNum : uMaxFrameNum<<1;

    if ((slice_type != INTRASLICE) && (slice_type != S_INTRASLICE))
    {
        // Detect and report no available reference frames
        H264EncoderFrameList_countActiveRefs(&core_enc->m_dpb, NumShortTermRefs, NumLongTermRefs);
        if ((NumShortTermRefs + NumLongTermRefs) == 0)
        {
            VM_ASSERT(0);
            ps = UMC_ERR_INVALID_STREAM;
        }

        if (ps == UMC_OK)
        {
            // Initialize the reference picture lists
            if ((slice_type == PREDSLICE) || (slice_type == S_PREDSLICE))
                H264CoreEncoder_InitPSliceRefPicList(state, curr_slice, SHdr.field_pic_flag != 0, pRefPicList0);
            else
                H264CoreEncoder_InitBSliceRefPicLists(state, curr_slice, SHdr.field_pic_flag != 0, pRefPicList0, pRefPicList1);

            // Reorder the reference picture lists
            Ipp32s num_ST_Ref0 = 0, num_LT_Ref = 0;
            Ipp32s num_ST_Ref1;
            if (SHdr.field_pic_flag)
            {
                H264CoreEncoder_AdjustRefPicListForFields<COEFFSTYPE, PIXTYPE>(state, pRefPicList0, pFields0);
                num_ST_Ref0 = core_enc->m_NumShortEntriesInList;
                num_LT_Ref = core_enc->m_NumLongEntriesInList;
            }
            else
            {
                for(Ipp32s i=0;i<MAX_NUM_REF_FRAMES;i++)
                    pFields0[i]=0;
            }

            if (BPREDSLICE == slice_type)
            {
                if (SHdr.field_pic_flag)
                {
                    H264CoreEncoder_AdjustRefPicListForFields<COEFFSTYPE, PIXTYPE>(state, pRefPicList1, pFields1);
                    num_ST_Ref1 = core_enc->m_NumShortEntriesInList;
                    if ((curr_slice->m_NumRefsInL0List == 0 || curr_slice->m_NumRefsInL1List == 0) && ((num_ST_Ref0+num_ST_Ref1+num_LT_Ref) > 1))//handling special case for fields
                    {
                        pRefPicList1[0] = pRefPicList0[1];
                        pRefPicList1[1] = pRefPicList0[0];
                        pFields1[0] = pFields0[1];
                        pFields1[1] = pFields0[0];

                    }

                    curr_slice->m_NumRefsInL0List = num_ST_Ref0 + num_LT_Ref;
                    curr_slice->m_NumRefsInL1List = num_ST_Ref1 + num_LT_Ref;
                }
                else
                {
                    for (Ipp32s i=0;i<MAX_NUM_REF_FRAMES;i++)
                        pFields1[i]=0;
                    curr_slice->m_NumRefsInL0List += curr_slice->m_NumRefsInLTList;
                    curr_slice->m_NumRefsInL1List += curr_slice->m_NumRefsInLTList;
                }
            }

            if (pReorderInfo_L0->num_entries > 0)
                H264CoreEncoder_ReOrderRefPicList<COEFFSTYPE, PIXTYPE>(state, SHdr.field_pic_flag != 0,pRefPicList0, pFields0,pReorderInfo_L0, uMaxPicNum, curr_slice->num_ref_idx_l0_active);

            if (BPREDSLICE == slice_type && pReorderInfo_L1->num_entries > 0)
                H264CoreEncoder_ReOrderRefPicList<COEFFSTYPE, PIXTYPE>(state, SHdr.field_pic_flag != 0,pRefPicList1, pFields1,pReorderInfo_L1, uMaxPicNum, curr_slice->num_ref_idx_l1_active);

            if (SHdr.field_pic_flag)
            {
                Ipp32s i;
                for (i = 0; i < curr_slice->m_NumRefsInL0List; i++)
                    pPOC0[i] = H264EncoderFrame_PicOrderCnt(pRefPicList0[i], pFields0[i], 1);
                for (i = 0; i < curr_slice->m_NumRefsInL1List; i++)
                    pPOC1[i] = H264EncoderFrame_PicOrderCnt(pRefPicList1[i], pFields1[i], 1);
            }
            else
            {
                Ipp32s i;
                for (i = 0; i < curr_slice->m_NumRefsInL0List; i++)
                    pPOC0[i] = H264EncoderFrame_PicOrderCnt(pRefPicList0[i], 0, 3);
                for (i = 0; i < curr_slice->m_NumRefsInL1List; i++)
                    pPOC1[i] = H264EncoderFrame_PicOrderCnt(pRefPicList1[i], 0, 3);
            }

            curr_slice->num_ref_idx_l0_active = MAX(curr_slice->m_NumRefsInL0List, 1);
            curr_slice->num_ref_idx_l1_active = MAX(curr_slice->m_NumRefsInL1List, 1);

/*            if( BPREDSLICE == slice_type ){
                curr_slice->num_ref_idx_l0_active = curr_slice->num_ref_idx_l1_active = MAX(curr_slice->m_NumRefsInL0List+curr_slice->m_NumRefsInL1List, 1);
            }
*/
            curr_slice->num_ref_idx_active_override_flag = ((curr_slice->num_ref_idx_l0_active != core_enc->m_PicParamSet.num_ref_idx_l0_active)
                                                         || (curr_slice->num_ref_idx_l1_active != core_enc->m_PicParamSet.num_ref_idx_l1_active));

            // If B slice, init scaling factors array
            if ((BPREDSLICE == slice_type) && (pRefPicList1[0] != NULL)){
                H264CoreEncoder_InitDistScaleFactor(state, curr_slice, curr_slice->num_ref_idx_l0_active, curr_slice->num_ref_idx_l1_active, pRefPicList0, pRefPicList1, pFields0,pFields1);
                H264CoreEncoder_InitMapColMBToList0(curr_slice, curr_slice->num_ref_idx_l0_active, pRefPicList0, pRefPicList1, core_enc->m_MaxSliceSize );
//                InitMVScale(curr_slice, curr_slice->num_ref_idx_l0_active, curr_slice->num_ref_idx_l1_active, pRefPicList0, pRefPicList1, pFields0,pFields1);
            }
        }
        //update temporal refpiclists
        Ipp32s i;
        switch(core_enc->m_pCurrentFrame->m_PictureStructureForDec)
        {
            case FLD_STRUCTURE:
            case FRM_STRUCTURE:
                for (i=0;i<MAX_NUM_REF_FRAMES;i++)
                {
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL0.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL0.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_RefPicList[i]= pRefPicList0[i];
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL0.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL0.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_Prediction[i]= pFields0[i];
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL0.m_POC[i]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_POC[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL0.m_POC[i]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_POC[i]= pPOC0[i];
                }
                for (i=0;i<MAX_NUM_REF_FRAMES;i++)
                {
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL1.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL1.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_RefPicList[i]= pRefPicList1[i];
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL1.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL1.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_Prediction[i]= pFields1[i];
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL1.m_POC[i]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_POC[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL1.m_POC[i]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_POC[i]= pPOC1[i];
            }
            break;
            case AFRM_STRUCTURE:
                for (i=0;i<MAX_NUM_REF_FRAMES / 2;i++)
                {
                    //frame part
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL0.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL0.m_RefPicList[i]= pRefPicList0[i];
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL0.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL0.m_Prediction[i]= 0;
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL0.m_POC[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL0.m_POC[i]= pPOC0[i];
                    //field part
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_RefPicList[2*i+0]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_RefPicList[2*i+0]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_RefPicList[2*i+1]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_RefPicList[2*i+1]= pRefPicList0[i];
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_Prediction[2*i+0]=0;
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_Prediction[2*i+1]=1;
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_Prediction[2*i+0]=1;
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_Prediction[2*i+1]=0;

                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_POC[2*i+0]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_POC[2*i+0]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL0.m_POC[2*i+1]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL0.m_POC[2*i+1]= pPOC0[i];
                }
                for (i=0;i<MAX_NUM_REF_FRAMES / 2;i++)
                {
                //frame part
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL1.m_RefPicList[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL1.m_RefPicList[i]= pRefPicList1[i];
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL1.m_Prediction[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL1.m_Prediction[i]= 0;
                    curr_slice->m_TempRefPicList[0][0].m_RefPicListL1.m_POC[i]=
                    curr_slice->m_TempRefPicList[0][1].m_RefPicListL1.m_POC[i]= pPOC1[i];
                    //field part
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_RefPicList[2*i+0]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_RefPicList[2*i+0]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_RefPicList[2*i+1]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_RefPicList[2*i+1]= pRefPicList1[i];
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_Prediction[2*i+0]=0;
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_Prediction[2*i+1]=1;
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_Prediction[2*i+0]=1;
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_Prediction[2*i+1]=0;

                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_POC[2*i+0]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_POC[2*i+0]=
                    curr_slice->m_TempRefPicList[1][0].m_RefPicListL1.m_POC[2*i+1]=
                    curr_slice->m_TempRefPicList[1][1].m_RefPicListL1.m_POC[2*i+1]= pPOC1[i];
                }
                break;
            }
    }

    return ps;
}    // UpdateRefPicList

//////////////////////////////////////////////////////////////////////////////
// InitPSliceRefPicList
//////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_InitPSliceRefPicList(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, bool bIsFieldSlice, H264EncoderFrame<PIXTYPE> **pRefPicList)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s i, j, k;
    Ipp32s NumFramesInList;
    H264EncoderFrame<PIXTYPE> *pHead = core_enc->m_dpb.m_pHead;
    H264EncoderFrame<PIXTYPE> *pFrm;
    Ipp32s PicNum;

    VM_ASSERT(pRefPicList);

    for (i=0; i<MAX_NUM_REF_FRAMES; i++)
        pRefPicList[i] = NULL;
    NumFramesInList = 0;

    if (!bIsFieldSlice)
    {
        // Frame. Ref pic list ordering: Short term largest pic num to
        // smallest, followed by long term, largest long term pic num to
        // smallest. Note that ref pic list has one extra slot to assist
        // with re-ordering.
        for (pFrm = pHead; pFrm; pFrm = pFrm->m_pFutureFrame)
        {
            if (H264EncoderFrame_isShortTermRef0(pFrm) == 3)
            {
                PicNum = H264EncoderFrame_PicNum(pFrm, 0, 0);
                j=0; // find insertion point
                while (j < NumFramesInList && H264EncoderFrame_isShortTermRef0(pRefPicList[j]) && H264EncoderFrame_PicNum(pRefPicList[j], 0, 0) > PicNum)
                    j++;

                if (pRefPicList[j])
                {   // make room if needed
                    if( NumFramesInList == MAX_NUM_REF_FRAMES )
                    {
                        NumFramesInList--;
                        VM_ASSERT(0);
                    } // Avoid writing beyond end of list, discard last element
                    for (k=NumFramesInList; k>j; k--)
                        pRefPicList[k] = pRefPicList[k-1];
                }

                pRefPicList[j] = pFrm;  // add the short-term reference
                NumFramesInList++;
            }
            else if(H264EncoderFrame_isLongTermRef0(pFrm) == 3)
            {
                PicNum = H264EncoderFrame_LongTermPicNum(pFrm, 0, 3);
                j=0; // find insertion point: Skip past short-term refs and long term refs with smaller long term pic num
                while (j < NumFramesInList && (H264EncoderFrame_isShortTermRef0(pRefPicList[j]) || (H264EncoderFrame_isLongTermRef0(pRefPicList[j]) &&
                     H264EncoderFrame_LongTermPicNum(pRefPicList[j], 0,2) < PicNum)))
                     j++;

                if (pRefPicList[j])
                { // make room if needed
                    if( NumFramesInList == MAX_NUM_REF_FRAMES )
                    {
                        NumFramesInList--;
                        VM_ASSERT(0);
                    } // Avoid writing beyond end of list, discard last element
                    for (k=NumFramesInList; k>j; k--)
                        pRefPicList[k] = pRefPicList[k-1];
                }

                pRefPicList[j] = pFrm; // add the short-term reference
                NumFramesInList++;
            }
        }
    }
    else
    {
        // TBD: field
        for (pFrm = pHead; pFrm; pFrm = pFrm->m_pFutureFrame)
        {
            if (H264EncoderFrame_isShortTermRef0(pFrm))
            {
                PicNum = pFrm->m_FrameNumWrap;
                j=0; // find insertion point
                while (j < NumFramesInList &&
                    H264EncoderFrame_isShortTermRef0(pRefPicList[j]) &&
                    pRefPicList[j]->m_FrameNumWrap > PicNum)
                    j++;

                if (pRefPicList[j]) { // make room if needed
                    if( NumFramesInList == MAX_NUM_REF_FRAMES )
                    {
                        NumFramesInList--;
                        VM_ASSERT(0);
                    }  // Avoid writing beyond end of list, discard last element
                    for (k=NumFramesInList; k>j; k--)
                        pRefPicList[k] = pRefPicList[k-1];
                }

                pRefPicList[j] = pFrm;  // add the long-term reference
                NumFramesInList++;
            }
            else if (H264EncoderFrame_isLongTermRef0(pFrm))
            {
                PicNum = H264EncoderFrame_LongTermPicNum(pFrm, 0, 2);

                j=0; // find insertion point: Skip past short-term refs and long term refs with smaller long term pic num
                while (j < NumFramesInList &&
                    (H264EncoderFrame_isShortTermRef0(pRefPicList[j]) ||
                    (H264EncoderFrame_isLongTermRef0(pRefPicList[j]) &&
                     H264EncoderFrame_LongTermPicNum(pRefPicList[j], 0, 2) < PicNum)))
                     j++;

                // make room if needed
                if (pRefPicList[j])
                {
                    if( NumFramesInList == MAX_NUM_REF_FRAMES )
                    {
                        NumFramesInList--;
                        VM_ASSERT(0);
                    } // Avoid writing beyond end of list, discard last element
                    for (k=NumFramesInList; k>j; k--)
                        pRefPicList[k] = pRefPicList[k-1];
                }

                pRefPicList[j] = pFrm; // add the long-term reference
                NumFramesInList++;
            }
        }
    }
#ifdef STORE_PICLIST
    if (refpic == NULL) refpic = fopen(__PICLIST_FILE__,VM_STRING("wt"));
    if (refpic){
        fprintf(refpic,"P Slice cur_poc %d %d field %d parity %d\n",
            H264EncoderFrame_PicOrderCnt(core_enc->m_pCurrentFrame, 0, 1),
            H264EncoderFrame_PicOrderCnt(core_enc->m_pCurrentFrame, 1, 1),
            core_enc->m_field_index,
            core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index]);
        fprintf(refpic,"RefPicList 0:\n");

        for (Ipp32s i=0;i<NumFramesInList;i++){
             fprintf(refpic,"Entry %d poc %d %d picnum %d %d FNW %d str %d\n",i,
                H264EncoderFrame_PicOrderCnt(pRefPicList[i], 0, 1),
                H264EncoderFrame_PicOrderCnt(pRefPicList[i], 1, 1),
                H264EncoderFrame_PicNum(pRefPicList[i], 0, 0),
                H264EncoderFrame_PicNum(pRefPicList[i], 1, 0),
                pRefPicList[i]->m_FrameNumWrap,
                H264EncoderFrame_isShortTermRef0(pRefPicList[i]));
        }
        fprintf(refpic,"\n");
        fflush(refpic);
    }
#endif

    curr_slice->m_NumRefsInL0List = NumFramesInList;
    curr_slice->m_NumRefsInL1List = 0;
}    // InitPSliceRefPicList

//////////////////////////////////////////////////////////////////////////////
// InitBSliceRefPicLists
//////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_InitBSliceRefPicLists(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, bool bIsFieldSlice,
    H264EncoderFrame<PIXTYPE> **pRefPicList0, H264EncoderFrame<PIXTYPE> **pRefPicList1)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s i, j, k;
    Ipp32s NumFramesInL0List;
    Ipp32s NumFramesInL1List;
    Ipp32s NumFramesInLTList;
    H264EncoderFrame<PIXTYPE> *pHead = core_enc->m_dpb.m_pHead;
    H264EncoderFrame<PIXTYPE> *pFrm;
    H264EncoderFrame<PIXTYPE> *LTRefPicList[MAX_NUM_REF_FRAMES];    // temp storage for long-term ordered list
    Ipp32s FrmPicOrderCnt;
    Ipp32s LongTermPicNum;

    for (i=0; i<MAX_NUM_REF_FRAMES; i++)
    {
        pRefPicList0[i] = NULL;
        pRefPicList1[i] = NULL;
        LTRefPicList[i] = NULL;
    }

    NumFramesInL0List = 0;
    NumFramesInL1List = 0;
    NumFramesInLTList = 0;

    if (!bIsFieldSlice)
    {
        // Short term references:
        // Need L0 and L1 lists. Both contain 2 sets of reference frames ordered
        // by PicOrderCnt. The "previous" set contains the reference frames with
        // a PicOrderCnt < current frame. The "future" set contains the reference
        // frames with a PicOrderCnt > current frame. In both cases the ordering
        // is from closest to current frame to farthest. L0 has the previous set
        // followed by the future set; L1 has the future set followed by the previous set.
        // Accomplish this by one pass through the decoded frames list creating
        // the ordered previous list in the L0 array and the ordered future list
        // in the L1 array. Then copy from both to the other for the second set.

        // Long term references:
        // The ordered list is the same for L0 and L1, is ordered by ascending
        // LongTermPicNum. The ordered list is created using local temp then
        // appended to the L0 and L1 lists after the short term references.
        Ipp32s CurrPicOrderCnt = H264EncoderFrame_PicOrderCnt(core_enc->m_pCurrentFrame, 0, 0);

        for (pFrm = pHead; pFrm; pFrm = pFrm->m_pFutureFrame)
        {
            if (H264EncoderFrame_isShortTermRef0(pFrm) == 3)
            {
                FrmPicOrderCnt = H264EncoderFrame_PicOrderCnt(pFrm, 0, 3);

                if (FrmPicOrderCnt < CurrPicOrderCnt)
                {
                    j=0; // Previous reference to L0, order large to small
                    while (j < NumFramesInL0List && (H264EncoderFrame_PicOrderCnt(pRefPicList0[j], 0, 3) > FrmPicOrderCnt))
                        j++;

                    // make room if needed
                    if (pRefPicList0[j])
                    {
                        if( NumFramesInL0List == MAX_NUM_REF_FRAMES )
                        {
                            NumFramesInL0List--;
                            VM_ASSERT(0);
                        }  // Avoid writing beyond end of list, discard last element
                        for (k=NumFramesInL0List; k>j; k--)
                            pRefPicList0[k] = pRefPicList0[k-1];
                    }
                    pRefPicList0[j] = pFrm; // add the short-term reference
                    NumFramesInL0List++;
                }
                else
                {
                    j=0; // Future reference to L1, order small to large
                    while (j < NumFramesInL1List &&
                        H264EncoderFrame_PicOrderCnt(pRefPicList1[j], 0, 3) < FrmPicOrderCnt)
                        j++;

                    // make room if needed
                    if (pRefPicList1[j]) {
                        if( NumFramesInL1List == MAX_NUM_REF_FRAMES ){  NumFramesInL1List--;  VM_ASSERT(0); }  // Avoid writing beyond end of list, discard last element
                        for (k=NumFramesInL1List; k>j; k--) pRefPicList1[k] = pRefPicList1[k-1];
                    }

                    // add the short-term reference
                    pRefPicList1[j] = pFrm;
                    NumFramesInL1List++;
                }
            }
            else if (H264EncoderFrame_isLongTermRef0(pFrm) == 3){
                LongTermPicNum = H264EncoderFrame_LongTermPicNum(pFrm, 0, 3);
                j=0; // order smallest to largest
                while (j < NumFramesInLTList &&
                    H264EncoderFrame_LongTermPicNum(LTRefPicList[j], 0, 0) < LongTermPicNum)
                    j++;

                if (LTRefPicList[j]){ // make room if needed
                    if( NumFramesInLTList == MAX_NUM_REF_FRAMES ){  NumFramesInLTList--;  VM_ASSERT(0); }  // Avoid writing beyond end of list, discard last element
                    for (k=NumFramesInLTList; k>j; k--) LTRefPicList[k] = LTRefPicList[k-1];
                }
                LTRefPicList[j] = pFrm; // add the long-term reference
                NumFramesInLTList++;

            }    // long term reference
        }    // for pFrm

        if((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) < MAX_NUM_REF_FRAMES)
        {
            // Complete L0 and L1 lists
            for (i=0; i<NumFramesInL1List; i++) // Add future short term references to L0 list, after previous
                pRefPicList0[NumFramesInL0List+i] = pRefPicList1[i];

            for (i=0; i<NumFramesInL0List; i++) // Add previous short term references to L1 list, after future
                pRefPicList1[NumFramesInL1List+i] = pRefPicList0[i];

            for (i=0; i<NumFramesInLTList; i++){ // Add long term list to both L0 and L1
                pRefPicList0[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
                pRefPicList1[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
            }

            // Special rule: When L1 has more than one entry and L0 == L1, all entries,
            // swap the first two entries of L1.
            // They can be equal only if there are no future or no previous short term
            // references.
            if ((NumFramesInL0List == 0 || NumFramesInL1List == 0) && ((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) > 1)){
                pRefPicList1[0] = pRefPicList0[1];
                pRefPicList1[1] = pRefPicList0[0];
            }
        }else{
            // too many reference frames
            VM_ASSERT(0);
        }
    }
    else
    {
        // Short term references:
        // Need L0 and L1 lists. Both contain 2 sets of reference frames ordered
        // by PicOrderCnt. The "previous" set contains the reference frames with
        // a PicOrderCnt < current frame. The "future" set contains the reference
        // frames with a PicOrderCnt > current frame. In both cases the ordering
        // is from closest to current frame to farthest. L0 has the previous set
        // followed by the future set; L1 has the future set followed by the previous set.
        // Accomplish this by one pass through the decoded frames list creating
        // the ordered previous list in the L0 array and the ordered future list
        // in the L1 array. Then copy from both to the other for the second set.

        // Long term references:
        // The ordered list is the same for L0 and L1, is ordered by ascending
        // LongTermPicNum. The ordered list is created using local temp then
        // appended to the L0 and L1 lists after the short term references.
        Ipp32s CurrPicOrderCnt = H264EncoderFrame_PicOrderCnt(core_enc->m_pCurrentFrame, core_enc->m_field_index, 0);

        for (pFrm = pHead; pFrm; pFrm = pFrm->m_pFutureFrame){
            if (H264EncoderFrame_isShortTermRef0(pFrm)){
                FrmPicOrderCnt = H264EncoderFrame_PicOrderCnt(pFrm, 0, 2);//returns POC of reference field (or min if both are reference)

                if (FrmPicOrderCnt < CurrPicOrderCnt) {
                    j=0; // Previous reference to L0, order large to small
                    while (j < NumFramesInL0List &&
                        (H264EncoderFrame_PicOrderCnt(pRefPicList0[j], 0, 2) > FrmPicOrderCnt))
                        j++;

                    if (pRefPicList0[j]){ // make room if needed
                        if( NumFramesInL0List == MAX_NUM_REF_FRAMES ){  NumFramesInL0List--;  VM_ASSERT(0); }  // Avoid writing beyond end of list, discard last element
                        for (k=NumFramesInL0List; k>j; k--) pRefPicList0[k] = pRefPicList0[k-1];
                    }
                    pRefPicList0[j] = pFrm; // add the short-term reference
                    NumFramesInL0List++;
                }else{
                    j=0; // Future reference to L1, order small to large
                    while (j < NumFramesInL1List &&
                        H264EncoderFrame_PicOrderCnt(pRefPicList1[j], 0, 2) < FrmPicOrderCnt)
                        j++;

                    if (pRefPicList1[j]) { // make room if needed
                        if( NumFramesInL1List == MAX_NUM_REF_FRAMES ){  NumFramesInL1List--;  VM_ASSERT(0); }  // Avoid writing beyond end of list, discard last element
                        for (k=NumFramesInL1List; k>j; k--) pRefPicList1[k] = pRefPicList1[k-1];
                    }
                    pRefPicList1[j] = pFrm; // add the short-term reference
                    NumFramesInL1List++;
                }
            } else if (H264EncoderFrame_isLongTermRef0(pFrm)) {
                LongTermPicNum = H264EncoderFrame_LongTermPicNum(pFrm, 0, 2);
                j=0; // order smallest to largest
                while (j < NumFramesInLTList &&
                    H264EncoderFrame_LongTermPicNum(LTRefPicList[j], 0, 2) < LongTermPicNum)
                    j++;

                if (LTRefPicList[j]){ // make room if needed
                    if( NumFramesInLTList == MAX_NUM_REF_FRAMES ){  NumFramesInLTList--;  VM_ASSERT(0); }  // Avoid writing beyond end of list, discard last element
                    for (k=NumFramesInLTList; k>j; k--) LTRefPicList[k] = LTRefPicList[k-1];
                }
                LTRefPicList[j] = pFrm; // add the long-term reference
                NumFramesInLTList++;
            }    // long term reference

            if ((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) < MAX_NUM_REF_FRAMES){
                // Complete L0 and L1 lists
                // Add future short term references to L0 list, after previous
                for (i=0; i<NumFramesInL1List; i++) pRefPicList0[NumFramesInL0List+i] = pRefPicList1[i];

                // Add previous short term references to L1 list, after future
                for (i=0; i<NumFramesInL0List; i++) pRefPicList1[NumFramesInL1List+i] = pRefPicList0[i];

                // Add long term list to both L0 and L1
                for (i=0; i<NumFramesInLTList; i++){
                    pRefPicList0[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
                    pRefPicList1[NumFramesInL0List+NumFramesInL1List+i] = LTRefPicList[i];
                }

                // Special rule: When L1 has more than one entry and L0 == L1, all entries,
                // swap the first two entries of L1.
                // They can be equal only if there are no future or no previous short term
                // references. For fields will be done later.
                /*
                if ((NumFramesInL0List == 0 || NumFramesInL1List == 0) &&
                ((NumFramesInL0List+NumFramesInL1List+NumFramesInLTList) > 1))
                {
                pRefPicList1[0] = pRefPicList0[1];
                pRefPicList1[1] = pRefPicList0[0];
                }*/
            }
            else
            {
                // too many reference frames
                VM_ASSERT(0);
            }

        }    // for pFrm
    }

#ifdef STORE_PICLIST
    if (refpic == NULL)
        refpic = fopen(__PICLIST_FILE__, "wt");
    if (refpic){
        fprintf(refpic, "B Slice cur_poc %d %d\n",
            H264EncoderFrame_PicOrderCnt(core_enc->m_pCurrentFrame, 0, 1),
            H264EncoderFrame_PicOrderCnt(core_enc->m_pCurrentFrame, 1, 1));
        fprintf(refpic, "RefPicList 0:\n");
        Ipp32s i;
        for (i = 0; i < NumFramesInL0List; i++){
            fprintf(refpic,"Entry %d poc %d %d picnum %d %d FNW %d str %d\n", i,
                H264EncoderFrame_PicOrderCnt(pRefPicList0[i], 0, 1),
                H264EncoderFrame_PicOrderCnt(pRefPicList0[i], 1, 1),
                H264EncoderFrame_PicNum(pRefPicList0[i], 0, 0),
                H264EncoderFrame_PicNum(pRefPicList0[i], 1, 0),
                pRefPicList0[i]->m_FrameNumWrap,
                H264EncoderFrame_isShortTermRef0(pRefPicList0[i]));
        }
        fprintf(refpic, "RefPicList 1:\n");

        for (i = 0; i < NumFramesInL1List; i++){
            fprintf(refpic, "Entry %d poc %d %d picnum %d %d FNW %d str %d\n",i,
                H264EncoderFrame_PicOrderCnt(pRefPicList1[i], 0, 1),
                H264EncoderFrame_PicOrderCnt(pRefPicList1[i], 1, 1),
                H264EncoderFrame_PicNum(pRefPicList1[i], 0, 0),
                H264EncoderFrame_PicNum(pRefPicList1[i], 1, 0),
                pRefPicList1[i]->m_FrameNumWrap,
                H264EncoderFrame_isShortTermRef0(pRefPicList1[i]));
        }
        fprintf(refpic, "\n");
        fflush(refpic);
    }
#endif

    curr_slice->m_NumRefsInL0List = NumFramesInL0List;
    curr_slice->m_NumRefsInL1List = NumFramesInL1List;
    curr_slice->m_NumRefsInLTList = NumFramesInLTList;

}    // InitBSliceRefPicLists

//////////////////////////////////////////////////////////////////////////////
// InitDistScaleFactor
//  Calculates the scaling factor used for B slice temporal motion vector
//  scaling and for B slice bidir predictin weighting using the picordercnt
//  values from the current and both reference frames, saving the result
//  to the DistScaleFactor array for future use. The array is initialized
//  with out of range values whenever a bitstream unit is received that
//  might invalidate the data (for example a B slice header resulting in
//  modified reference picture lists). For scaling, the list1 [0] entry
//    is always used.
//////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_InitDistScaleFactor(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32s NumL0RefActive, Ipp32s NumL1RefActive,
    H264EncoderFrame<PIXTYPE> **pRefPicList0, H264EncoderFrame<PIXTYPE> **pRefPicList1, Ipp8s *pFields0, Ipp8s *pFields1)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s L0Index,L1Index;
    Ipp32u picCntRef0;
    Ipp32u picCntRef1;
    Ipp32u picCntCur;
    Ipp32s DistScaleFactor;

    Ipp32s tb;
    Ipp32s td;
    Ipp32s tx;

    VM_ASSERT(NumL0RefActive <= MAX_NUM_REF_FRAMES);
    VM_ASSERT(pRefPicList1[0]);

    picCntCur = H264EncoderFrame_PicOrderCnt(
        core_enc->m_pCurrentFrame,
        core_enc->m_field_index,
        0);//m_PicOrderCnt;  Current POC
    for( L1Index = 0; L1Index<NumL1RefActive; L1Index++ ){
        if(core_enc->m_pCurrentFrame->m_PictureStructureForRef>=FRM_STRUCTURE){
            picCntRef1 = H264EncoderFrame_PicOrderCnt(pRefPicList1[L1Index], 0, 0);
        }else{
            Ipp32s RefField = H264EncoderFrame_GetNumberByParity(
                pRefPicList1[L1Index],
                GetReferenceField(pFields1, L1Index));
            picCntRef1 = H264EncoderFrame_PicOrderCnt(pRefPicList1[L1Index], RefField, 0);
        }

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            if(core_enc->m_pCurrentFrame->m_PictureStructureForRef>=FRM_STRUCTURE)
            {
                VM_ASSERT(pRefPicList0[L0Index]);
                picCntRef0 = H264EncoderFrame_PicOrderCnt(pRefPicList0[L0Index], 0, 0);
            }
            else
            {
                Ipp8s RefFieldTop;
                //ref0
                RefFieldTop  = GetReferenceField(pFields0,L0Index);
                VM_ASSERT(pRefPicList0[L0Index]);
                picCntRef0 = H264EncoderFrame_PicOrderCnt(
                    pRefPicList0[L0Index],
                    H264EncoderFrame_GetNumberByParity(
                        pRefPicList1[L1Index],
                        RefFieldTop),
                    0);
            }
            CalculateDSF(L0Index);
        }
    }

#if 0  //FIXME need to fix it for AFRM pictures
    if (core_enc->m_pCurrentFrame->m_PictureStructureForRef==AFRM_STRUCTURE){
        // [curmb field],[ref1field],[ref0field]
        pDistScaleFactor = curr_slice->DistScaleFactorAFF[0][0][0];        //complementary field pairs, cf=top r1=top,r0=top
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[0][0][0];  //complementary field pairs, cf=top r1=top,r0=top

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(0,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(0,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(0,1);
            CalculateDSF(L0Index);
        }

        pDistScaleFactor =curr_slice->DistScaleFactorAFF[0][0][1];        //complementary field pairs, cf=top r1=top,r0=bottom
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[0][0][1];  //complementary field pairs, cf=top r1=top,r0=bottom

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(0,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(0,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(1,1);
            CalculateDSF(L0Index);
        }
        pDistScaleFactor = curr_slice->DistScaleFactorAFF[0][1][0];        //complementary field pairs, cf=top r1=bottom,r0=top
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[0][1][0];  //complementary field pairs, cf=top r1=bottom,r0=top

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(0,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(1,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(0,1);
            CalculateDSF(L0Index);
        }
        pDistScaleFactor = curr_slice->DistScaleFactorAFF[0][1][1];        //complementary field pairs, cf=top r1=bottom,r0=bottom
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[0][1][1];  //complementary field pairs, cf=top r1=bottom,r0=bottom

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(0,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(1,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(1,1);
            CalculateDSF(L0Index);
        }

        /*--------------------------------------------------------------------*/
        pDistScaleFactor = curr_slice->DistScaleFactorAFF[1][0][0];        //complementary field pairs, cf=bottom r1=top,r0=top
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[1][0][0];  //complementary field pairs, cf=bottom r1=top,r0=top


        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(1,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(0,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(0,1);
            CalculateDSF(L0Index);
        }

        pDistScaleFactor = curr_slice->DistScaleFactorAFF[1][0][1];        //complementary field pairs, cf=bottom r1=top,r0=bottom
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[1][0][1];  //complementary field pairs, cf=bottom r1=top,r0=bottom

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(1,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(0,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(1,1);
            CalculateDSF(L0Index);
        }
        pDistScaleFactor = curr_slice->DistScaleFactorAFF[1][1][0];        //complementary field pairs, cf=bottom r1=bottom,r0=top
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[1][1][0];  //complementary field pairs, cf=bottom r1=bottom,r0=top

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(1,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(1,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(0,1);
            CalculateDSF(L0Index);
        }
        pDistScaleFactor = curr_slice->DistScaleFactorAFF[1][1][1];        //complementary field pairs, cf=bottom r1=bottom,r0=bottom
        pDistScaleFactorMV = curr_slice->DistScaleFactorMVAFF[1][1][1];  //complementary field pairs, cf=bottom r1=bottom,r0=bottom

        picCntCur = core_enc->m_pCurrentFrame->PicOrderCnt(1,1);
        picCntRef1 = pRefPicList1[0]->PicOrderCnt(1,1);

        for (L0Index=0; L0Index<NumL0RefActive; L0Index++){
            VM_ASSERT(pRefPicList0[L0Index]);

            picCntRef0 = pRefPicList0[L0Index]->PicOrderCnt(1,1);
            CalculateDSF(L0Index);
        }
    }

#endif
}    // InitDistScaleFactor

template<typename PIXTYPE>
Ipp32s MapColToList0(
    EncoderRefPicListStruct<PIXTYPE> *ref_pic_list_struct,
    H264EncoderFrame<PIXTYPE> **pRefPicList0,
    Ipp32s RefIdxCol,
    Ipp32s numRefL0Active)
{
    bool bFound = false;
    Ipp32s ref_idx = 0;
    // Translate the reference index of the colocated to current
    // L0 index to the same reference picture, using PicNum or
    // LongTermPicNum as id criteria.
    if( ref_pic_list_struct->m_RefPicList[RefIdxCol] == NULL ) return -1;

    if (H264EncoderFrame_isShortTermRef0(ref_pic_list_struct->m_RefPicList[RefIdxCol]))
    {
        Ipp32s RefPicNum = H264EncoderFrame_PicNum(
            ref_pic_list_struct->m_RefPicList[RefIdxCol],
            0,
            3);
        Ipp32s RefPOC = ref_pic_list_struct->m_POC[RefIdxCol];

        while (!bFound){         // find matching reference frame on current slice list 0
            if( ref_idx >= numRefL0Active ) break; //Don't go beyond L0 active refs
            if (H264EncoderFrame_isShortTermRef0(pRefPicList0[ref_idx]) &&
                H264EncoderFrame_PicNum(pRefPicList0[ref_idx], 0, 3) == RefPicNum &&
                H264EncoderFrame_PicOrderCnt(pRefPicList0[ref_idx], 0, 3) == RefPOC)
                bFound = true;
            else
                ref_idx++;
        }

        if (!bFound)  return -1;
    } else if (H264EncoderFrame_isLongTermRef0(ref_pic_list_struct->m_RefPicList[RefIdxCol])){
        Ipp32s RefPicNum = H264EncoderFrame_LongTermPicNum(
            ref_pic_list_struct->m_RefPicList[RefIdxCol],
            0,
            3);
        Ipp32s RefPOC = ref_pic_list_struct->m_POC[RefIdxCol];

        while (!bFound){ // find matching reference frame on current slice list 0
            if( ref_idx >= numRefL0Active ) break; //Don't go beyond L0 active refs
            if (H264EncoderFrame_isLongTermRef0(pRefPicList0[ref_idx]) &&
                H264EncoderFrame_LongTermPicNum(pRefPicList0[ref_idx], 0, 3) == RefPicNum &&
                H264EncoderFrame_PicOrderCnt(pRefPicList0[ref_idx], 0, 3) == RefPOC)
                bFound = true;
            else
                ref_idx++;
        }
        if (!bFound) return -1;
    } else return -1;
        // colocated is in a reference that is not marked as Ipp16s-term
        // or long-term, should not happen
        // Well it can be happen since in case of (num_ref_frames == 1) and
        // this frame is already unmarked as reference
        // So we could not use temporal direct prediction.
    return ref_idx;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_InitMapColMBToList0(H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32s NumL0RefActive,
    H264EncoderFrame<PIXTYPE> **pRefPicList0, H264EncoderFrame<PIXTYPE> **pRefPicList1, Ipp32u iMaxSliceSize)
{
    Ipp32s ref;
    EncoderRefPicListStruct<PIXTYPE> *ref_pic_list_struct;
    VM_ASSERT(pRefPicList1[0]);

    for(ref=0; ref<16; ref++ )
    {
        ref_pic_list_struct = H264EncoderFrame_GetRefPicList(
            pRefPicList1[0],
            curr_slice->m_slice_number,
            LIST_0, iMaxSliceSize);
        curr_slice->MapColMBToList0[ref][LIST_0] = MapColToList0(ref_pic_list_struct, pRefPicList0, ref, NumL0RefActive );

        ref_pic_list_struct = H264EncoderFrame_GetRefPicList(
            pRefPicList1[0],
            curr_slice->m_slice_number,
            LIST_1, iMaxSliceSize);
        curr_slice->MapColMBToList0[ref][LIST_1] = MapColToList0(ref_pic_list_struct, pRefPicList0, ref, NumL0RefActive );
    }
}

/*************************************************************************************************/
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb, H264BlockLocation *Block)
{
    if (BLOCK_IS_ON_LEFT_EDGE(Block->block_num)) // luma
    {
        Block->block_num+=3;
        Block->mb_num=cur_mb.MacroblockNeighbours.mb_A;
    } else {
        Block->block_num--;
        Block->mb_num=cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBChromaNonMBAFF(
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block)
{
    if (CHROMA_BLOCK_IS_ON_LEFT_EDGE(Block->block_num)) //chroma
    {
        Block->block_num+=1;
        Block->mb_num=cur_mb.MacroblockNeighbours.mb_A;
    } else {
        Block->block_num--;
        Block->mb_num=cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(void* state, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block, Ipp32s AdditionalDecrement)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    bool curmbfff = !pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    bool curmbtf  = (cur_mb.uMB&1)==0;
    Ipp32s MB_X = cur_mb.MacroblockNeighbours.mb_A;
    Ipp32s MB_N;

    //luma
    if (BLOCK_IS_ON_LEFT_EDGE(Block->block_num))
    {
        if (MB_X>=0)
        {
            Ipp8u xfff=!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]);
            if (curmbfff)
            {
                if (curmbtf)
                {
                    if (xfff)
                    {
                        // 1 1 1
                        MB_N=MB_X;
                    }
                    else
                    {
                        // 1 1 0
                        Ipp32u yN = Block->block_num/4;
                        yN*=4;
                        yN-=AdditionalDecrement;
                        yN/=2;
                        Block->block_num=(yN/4)*4;
                        if (AdditionalDecrement)
                            MB_N=MB_X  + 1;
                        else
                            MB_N=MB_X;
                        AdditionalDecrement=0;
                    }
                }
                else
                {
                    if (xfff)
                    {
                        // 1 0 1
                        MB_N=MB_X + 1;
                    }
                    else
                    {
                        // 1 0 0
                        Ipp32u yN = Block->block_num/4;
                        yN*=4;
                        yN-=AdditionalDecrement;
                        yN+=16;
                        yN/=2;
                        Block->block_num=(yN/4)*4;
                        if (AdditionalDecrement)
                            MB_N=MB_X + 1;
                        else
                            MB_N=MB_X;
                        AdditionalDecrement=0;
                    }
                }
            }
            else
            {
                if (curmbtf)
                {
                    if (xfff)
                    {
                        //0 1 1
                        Ipp32u yN = Block->block_num/4;
                        yN*=4;
                        yN-=AdditionalDecrement;
                        yN*=2;
                        if (yN<16)
                        {
                            MB_N=MB_X;
                        }
                        else
                        {
                            yN-=16;
                            MB_N=MB_X + 1;
                        }
                        Block->block_num=(yN/4)*4;
                        AdditionalDecrement=0;
                    }
                    else
                    {
                        // 0 1 0
                        MB_N=MB_X;
                    }
                }
                else
                {
                    if (xfff)
                    {
                        // 0 0 1
                        Ipp32u yN = Block->block_num/4;
                        yN*=4;
                        yN-=AdditionalDecrement;
                        yN*=2;
                        if (yN<15)
                        {
                            yN++;
                            MB_N=MB_X;
                        }
                        else
                        {
                            yN-=15;
                            MB_N=MB_X + 1;
                        }
                        Block->block_num=(yN/4)*4;
                        AdditionalDecrement=0;
                    }
                    else
                    {
                        // 0 0 0
                        MB_N=MB_X + 1;
                    }
                }
            }
        }
        else
        {
            Block->mb_num = -1;//no left neighbours
            return;
        }
        Block->block_num+=3-4*AdditionalDecrement;
        Block->mb_num = MB_N;
    }
    else
    {
        Block->block_num--;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetLeftLocationForCurrentMBChromaMBAFF(void* state, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb, H264BlockLocation *Block)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    bool curmbfff = !pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    bool curmbtf  = (cur_mb.uMB&1)==0;
    Ipp32s MB_X = cur_mb.MacroblockNeighbours.mb_A;
    Ipp32s MB_N;

    //chroma
    if (CHROMA_BLOCK_IS_ON_LEFT_EDGE(Block->block_num))
    {
        Ipp32u dec_value=16;
        if (Block->block_num>=20) dec_value=20;
        Block->block_num-=dec_value;
        if (MB_X>=0) //left mb addr vaild?
        {
            Ipp8u xfff=!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]);
            if (curmbfff)
            {
                if (curmbtf)
                {
                    if (xfff)
                    {
                        // 1 1 1
                        MB_N=MB_X;
                    }
                    else
                    {
                        // 1 1 0
                        Ipp32u yN = Block->block_num/2, xN=Block->block_num%2;
                        yN/=2;
                        Block->block_num=yN*2+xN;
                        MB_N=MB_X;
                    }
                }
                else
                {
                    if (xfff)
                    {
                        // 1 0 1
                        MB_N=MB_X+1;
                    }
                    else
                    {
                        // 1 0 0
                        Ipp32u yN = Block->block_num/2, xN=Block->block_num%2;
                        yN+=2;
                        yN/=2;
                        Block->block_num=yN*2+xN;
                        MB_N=MB_X;
                    }
                }
            }
            else
            {
                if (curmbtf)
                {
                    if (xfff)
                    {
                        //0 1 1
                        Ipp32u yN = Block->block_num/2, xN=Block->block_num%2;
                        if (yN<1)
                        {
                            yN*=2;
                            MB_N=MB_X;
                        }
                        else
                        {
                            yN*=2;
                            yN-=2;
                            MB_N=MB_X+1;
                        }
                        Block->block_num=yN*2+xN;
                    }
                    else
                    {
                        // 0 1 0
                        MB_N=MB_X;
                    }
                }
                else
                {
                    if (xfff)
                    {
                        // 0 0 1
                        Ipp32u yN = Block->block_num/2, xN=Block->block_num%2;
                        if (yN<1)
                        {
                            yN*=8;
                            yN++;
                            MB_N=MB_X;
                        }
                        else
                        {
                            yN*=8;
                            yN-=7;
                            MB_N=MB_X + 1;
                        }
                        Block->block_num=(yN/4)*2+xN;
                    }
                    else
                    {
                        // 0 0 0
                        MB_N=MB_X + 1;
                    }
                }
            }
        }
        else
        {
            Block->mb_num = -1;//no left neighbours
            return;
        }
        Block->block_num+=dec_value;
        Block->block_num+=1;
        Block->mb_num = MB_N;
    }
    else
    {
        Block->block_num--;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBLumaNonMBAFF(
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block)
{
    if (BLOCK_IS_ON_TOP_EDGE(Block->block_num)) //luma
    {
        Block->block_num+=12;
        Block->mb_num = cur_mb.MacroblockNeighbours.mb_B;
    } else {
        Block->block_num -= 4;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBChromaNonMBAFF(
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block)
{
    if (CHROMA_BLOCK_IS_ON_TOP_EDGE(Block->block_num)) //chroma
    {
        Block->block_num+=2;
        Block->mb_num  = cur_mb.MacroblockNeighbours.mb_B;
    } else {
        Block->block_num-=2;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF(void* state, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block, bool is_deblock_calls)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    bool curmbfff = !pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    bool curmbtf  = (cur_mb.uMB&1)==0;
    Ipp32s pair_offset = curmbtf? 1:-1;
    Ipp32s MB_X;
    Ipp32s MB_N;

    //luma
    if (BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        if (curmbfff && !curmbtf)
        {
            MB_N = cur_mb.uMB + pair_offset;
            Block->block_num+=12;
        }
        else
        {
            MB_X = cur_mb.MacroblockNeighbours.mb_B;
            if (MB_X>=0)
            {
                Ipp8u xfff=!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]);
                MB_N=MB_X;
                Block->block_num+=12;
                if (curmbfff || !curmbtf || xfff)
                {
                    if (!(curmbfff && curmbtf && !xfff && is_deblock_calls))
                        MB_N+= 1;
                }
            }
            else
            {
                Block->mb_num = -1;
                return;
            }
        }
        Block->mb_num = MB_N;
        return;
    }

    Block->block_num-=4;
    Block->mb_num = cur_mb.uMB;
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLocationForCurrentMBChromaMBAFF(void* state, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb, H264BlockLocation *Block)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    bool curmbfff = !pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    bool curmbtf  = (cur_mb.uMB&1)==0;
    Ipp32s pair_offset = curmbtf? 1:-1;
    Ipp32s MB_X;
    Ipp32s MB_N;

    //chroma
    if (CHROMA_BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        if (curmbfff && !curmbtf)
        {
            MB_N = cur_mb.uMB + pair_offset;
            Block->block_num+=2;
        }
        else
        {
            MB_X = cur_mb.MacroblockNeighbours.mb_B;
            if (MB_X>=0)
            {
                Ipp8u xfff=!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]);
                if (!curmbfff && curmbtf && !xfff)
                {
                    MB_N=MB_X;
                    Block->block_num+=2;
                }
                else
                {
                    //if (!curmbff && curmbtf && xfff)
                    //    Block->block_num+=0;
                    //else
                    Block->block_num+=2;
                    MB_N=MB_X + 1;
                }
            }
            else
            {
                Block->mb_num = -1;
                return;
            }
        }
        Block->mb_num = MB_N;
        return;
    }

    Block->block_num-=2;
    Block->mb_num = cur_mb.uMB;
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaNonMBAFF(
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block)
{
    //luma
    if (BLOCK_IS_ON_LEFT_EDGE(Block->block_num) && BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        Block->block_num+=15;
        Block->mb_num = cur_mb.MacroblockNeighbours.mb_D;
    }
    else if (BLOCK_IS_ON_LEFT_EDGE(Block->block_num))
    {
        Block->block_num--;
        Block->mb_num = cur_mb.MacroblockNeighbours.mb_A;
    }
    else if (BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        Block->block_num+=11;
        Block->mb_num = cur_mb.MacroblockNeighbours.mb_B;
    }
    else
    {
        Block->block_num-=5;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaMBAFF(void* state, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb, H264BlockLocation *Block)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    bool curmbfff = !pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    bool curmbtf  = (cur_mb.uMB&1)==0;
    Ipp32s MB_X;
    Ipp32s MB_N;

    //luma
    if (BLOCK_IS_ON_LEFT_EDGE(Block->block_num) && BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        if (curmbfff && !curmbtf)
        {
            MB_X = cur_mb.MacroblockNeighbours.mb_A;
            if (MB_X<0)
            {
                Block->mb_num = -1;
                return;
            }
            if (!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]))
            {
                MB_N = MB_X;
                Block->block_num+=15;
            }
            else
            {
                MB_N = MB_X + 1;
                Block->block_num+=7;
            }
        }
        else
        {
            MB_X = cur_mb.MacroblockNeighbours.mb_D;
            if (MB_X>=0)
            {
                if (curmbfff==curmbtf)
                {
                    MB_N=MB_X + 1;
                }
                else
                {
                    if (!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]))
                    {
                        MB_N=MB_X + 1;
                    }
                    else
                    {
                        MB_N=MB_X;
                    }
                }
                Block->block_num+=15;
            }
            else
            {
                Block->mb_num = -1;
                return;
            }
        }

        Block->mb_num = MB_N;
        return;
    }
    else if (BLOCK_IS_ON_LEFT_EDGE(Block->block_num))
    {
        //Block->block_num-=4;
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(state, cur_mb, Block,1);
        return;
    }
    else if (BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        Block->block_num--;
        H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF(state, cur_mb, Block,0);
        return;
    }

    Block->block_num-=5;
    Block->mb_num = cur_mb.uMB;
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopRightLocationForCurrentMBLumaNonMBAFF(
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb,
    H264BlockLocation *Block)
{
    //luma
    if (Block->block_num==3)
    {
        Block->block_num+=9;
        Block->mb_num = cur_mb.MacroblockNeighbours.mb_C;
    }
    else if (BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        Block->block_num+=13;
        Block->mb_num = cur_mb.MacroblockNeighbours.mb_B;
    }
    else if (!above_right_avail_4x4_[block_subblock_mapping_[Block->block_num]])
    {
        Block->mb_num = -1;
    }
    else
    {
        Block->block_num-=3;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_GetTopRightLocationForCurrentMBLumaMBAFF(void* state, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb, H264BlockLocation *Block)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    bool curmbfff = !pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    bool curmbtf  = (cur_mb.uMB&1)==0;
    Ipp32s MB_X;
    Ipp32s MB_N;

    //luma
    if (Block->block_num==3)
    {
        if (curmbfff && !curmbtf)
        {
            Block->mb_num = -1;
            return;
        }
        else
        {
            MB_X = cur_mb.MacroblockNeighbours.mb_C;
            if (MB_X>=0)
            {
                if (curmbfff==curmbtf)
                {
                    MB_N=MB_X + 1;
                }
                else
                {
                    if (!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[MB_X]))
                    {
                        MB_N=MB_X + 1;
                    }
                    else
                    {
                        MB_N=MB_X;
                    }
                }
                Block->block_num+=9;
            }
            else
            {
                Block->mb_num = -1;
                return;
            }
        }
        Block->mb_num = MB_N;
        return;
    }
    else if (BLOCK_IS_ON_TOP_EDGE(Block->block_num))
    {
        Block->block_num++;
        H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF(state, cur_mb, Block,0);
        return;
    }
    else if (!above_right_avail_4x4_lin[Block->block_num])
    {
        Block->mb_num = -1;
        return;
    }
    else
    {
        Block->block_num-=3;
        Block->mb_num = cur_mb.uMB;
    }
    return;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_UpdateCurrentMBInfo(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    H264MBAddr curMBAddr = cur_mb.uMB;

    cur_mb.GlobalMacroblockInfo = &core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr];
    cur_mb.LocalMacroblockInfo = &core_enc->m_mbinfo.mbs[curMBAddr];
    cur_mb.MacroblockCoeffsInfo = &core_enc->m_mbinfo.MacroblockCoeffsInfo[curMBAddr];
    cur_mb.MVs[LIST_0] = &core_enc->m_pCurrentFrame->m_mbinfo.MV[LIST_0][curMBAddr];
    cur_mb.MVs[LIST_1] = &core_enc->m_pCurrentFrame->m_mbinfo.MV[LIST_1][curMBAddr];
    cur_mb.MVs[2] = &core_enc->m_mbinfo.MVDeltas[0][curMBAddr];
    cur_mb.MVs[3] = &core_enc->m_mbinfo.MVDeltas[1][curMBAddr];
    cur_mb.RefIdxs[LIST_0] = &core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[LIST_0][curMBAddr];
    cur_mb.RefIdxs[LIST_1] = &core_enc->m_pCurrentFrame->m_mbinfo.RefIdxs[LIST_1][curMBAddr];
    cur_mb.intra_types = core_enc->m_mbinfo.intra_types[curMBAddr].intra_types;
    if ((curMBAddr & 1) == 0) {
        if (core_enc->m_SliceHeader.MbaffFrameFlag)
            cur_mb.uMBpair = curMBAddr + 1;
        else
            cur_mb.uMBpair = curMBAddr;
    } else {
        cur_mb.uMBpair = curMBAddr - 1;
    }
    cur_mb.GlobalMacroblockPairInfo = &core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.uMBpair];
    cur_mb.LocalMacroblockPairInfo = &core_enc->m_mbinfo.mbs[cur_mb.uMBpair];

    Ipp32s curMB_X = cur_mb.uMBx;
    Ipp32s curMB_Y = cur_mb.uMBy;
    if ((curMBAddr & 1) == 0 || !core_enc->m_is_cur_pic_afrm) { //update only if top mb received
        Ipp32s mb_top_offset = core_enc->m_WidthInMBs*(core_enc->m_SliceHeader.MbaffFrameFlag + 1);
        Ipp32s mb_left_offset = core_enc->m_SliceHeader.MbaffFrameFlag + 1;
        cur_mb.MacroblockNeighbours.mb_A = curMB_X > 0 ? curMBAddr-mb_left_offset : -1;
        cur_mb.MacroblockNeighbours.mb_B = curMB_Y > 0 ? curMBAddr-mb_top_offset : -1;
        cur_mb.MacroblockNeighbours.mb_C = curMB_Y > 0 && curMB_X < core_enc->m_WidthInMBs-1 ? curMBAddr-mb_top_offset+mb_left_offset : -1;
        cur_mb.MacroblockNeighbours.mb_D = curMB_Y > 0 && curMB_X > 0 ? curMBAddr-mb_top_offset-mb_left_offset : -1;
        if (core_enc->m_NeedToCheckMBSliceEdges){
            if (cur_mb.MacroblockNeighbours.mb_A >= 0 && (cur_mb.GlobalMacroblockInfo->slice_id != core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.MacroblockNeighbours.mb_A].slice_id))
                cur_mb.MacroblockNeighbours.mb_A = -1;
            if (cur_mb.MacroblockNeighbours.mb_B >= 0 && (cur_mb.GlobalMacroblockInfo->slice_id != core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.MacroblockNeighbours.mb_B].slice_id))
                cur_mb.MacroblockNeighbours.mb_B = -1;
            if (cur_mb.MacroblockNeighbours.mb_C >= 0 && (cur_mb.GlobalMacroblockInfo->slice_id != core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.MacroblockNeighbours.mb_C].slice_id))
                cur_mb.MacroblockNeighbours.mb_C = -1;
            if (cur_mb.MacroblockNeighbours.mb_D >= 0 && (cur_mb.GlobalMacroblockInfo->slice_id != core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.MacroblockNeighbours.mb_D].slice_id))
                cur_mb.MacroblockNeighbours.mb_D = -1;
        }
    }
    cur_mb.BlockNeighbours.mb_above.block_num = 0;
    cur_mb.BlockNeighbours.mb_above_chroma[0].block_num = 16;
    cur_mb.BlockNeighbours.mb_above_left.block_num = 0;
    cur_mb.BlockNeighbours.mb_above_right.block_num = 3;
    cur_mb.BlockNeighbours.mbs_left[0].block_num = 0;
    cur_mb.BlockNeighbours.mbs_left[1].block_num = 4;
    cur_mb.BlockNeighbours.mbs_left[2].block_num = 8;
    cur_mb.BlockNeighbours.mbs_left[3].block_num = 12;
    if (core_enc->m_PicParamSet.chroma_format_idc != 0) {
        cur_mb.BlockNeighbours.mbs_left_chroma[0][0].block_num = 16;
        cur_mb.BlockNeighbours.mbs_left_chroma[0][1].block_num = 18;
    }
    if (core_enc->m_SliceHeader.MbaffFrameFlag) {
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mbs_left[0], 0);
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mbs_left[1], 0);
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mbs_left[2], 0);
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mbs_left[3], 0);
        H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mb_above, false);
        H264CoreEncoder_GetTopRightLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mb_above_right);
        H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mb_above_left);
        if(core_enc->m_PicParamSet.chroma_format_idc != 0) {
            H264CoreEncoder_GetTopLocationForCurrentMBChromaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mb_above_chroma[0]);
            H264CoreEncoder_GetLeftLocationForCurrentMBChromaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mbs_left_chroma[0][0]);
            H264CoreEncoder_GetLeftLocationForCurrentMBChromaMBAFF(state, cur_mb, &cur_mb.BlockNeighbours.mbs_left_chroma[0][1]);
        }
    } else  {
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mbs_left[0]);
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mbs_left[1]);
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mbs_left[2]);
        H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mbs_left[3]);
        H264CoreEncoder_GetTopLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mb_above);
        H264CoreEncoder_GetTopRightLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mb_above_right);
        H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mb_above_left);
        if (core_enc->m_PicParamSet.chroma_format_idc != 0) {
            H264CoreEncoder_GetTopLocationForCurrentMBChromaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mb_above_chroma[0]);
            H264CoreEncoder_GetLeftLocationForCurrentMBChromaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mbs_left_chroma[0][0]);
            H264CoreEncoder_GetLeftLocationForCurrentMBChromaNonMBAFF(cur_mb, &cur_mb.BlockNeighbours.mbs_left_chroma[0][1]);
        }
    }
    if (core_enc->m_PicParamSet.chroma_format_idc != 0) {
        cur_mb.BlockNeighbours.mbs_left_chroma[1][0]=cur_mb.BlockNeighbours.mbs_left_chroma[0][0];
        cur_mb.BlockNeighbours.mbs_left_chroma[1][1]=cur_mb.BlockNeighbours.mbs_left_chroma[0][1];
        cur_mb.BlockNeighbours.mb_above_chroma[1] = cur_mb.BlockNeighbours.mb_above_chroma[0];
        cur_mb.BlockNeighbours.mbs_left_chroma[1][0].block_num += 4;
        cur_mb.BlockNeighbours.mbs_left_chroma[1][1].block_num += 4;
        cur_mb.BlockNeighbours.mb_above_chroma[1].block_num += 4;
    }
    cur_mb.cabac_data = curr_slice->Block_CABAC;
}

// Table to obtain edge info for a 4x4 block of a MB. The table entry when
// OR'd with the edge info for the MB, results in edge info for the block.
//
//  H264 4x4 Block ordering in a 16x16 Macroblock and edge assignments
//
//  ULC = Upper Left Corner, U = Upper Edge
//  L = Left Edge, R = Right Edge
//
//               luma (Y)                chroma (U)          chroma (V)
//
//        +-U--+-U--+-U--+-U--+         +-U--+-U--+         +-U--+-U--+
//        |    |    |    |    |         |    |    |         |    |    |
// ULC--> L 0  | 1  | 4  | 5  R  ULC--> L 16 | 17 R  ULC--> L 20 | 21 R
//        |    |    |    |    |         |    |    |         |    |    |
//        +----+----+----+----+         +----+----+         +----+----+
//        |    |    |    |    |         |    |    |         |    |    |
//        L 2  | 3  | 6  | 7  R         L 18 | 19 R         L 22 | 23 R
//        |    |    |    |    |         |    |    |         |    |    |
//        +----+----+----+----+         +----+----+         +----+----+
//        |    |    |    |    |
//        L 8  | 9  | 12 | 13 R
//        |    |    |    |    |
//        +----+----+----+----+
//        |    |    |    |    |
//        L 10 | 11 | 14 | 15 R
//        |    |    |    |    |
//        +----+----+----+----+
//
//  This table provides easy look-up by block number to determine
//  which edges is does NOT border on.


const H264MotionVector null_mv = {0};

inline Ipp32s convertLumaQP(Ipp32s QP, Ipp32s bit_depth_src, Ipp32s bit_depth_dst)
{
    Ipp32s qpy = QP + 6*(bit_depth_src - 8);
    Ipp32s qpDstOffset = 6*(bit_depth_dst - 8);
    Ipp32s minQpDst = -qpDstOffset;
    Ipp32s maxQpDst = 51;
    qpy = qpy - qpDstOffset;
    qpy = (qpy < minQpDst)? minQpDst : (qpy > maxQpDst)? maxQpDst : qpy;
    return(qpy);
}



//////////////////////////////////////





//--------------------------------------------------------------------------;
//
//  The algorithm fills in (1) the bottom (not including corners),
//  then (2) the sides (including the bottom corners, but not the
//  top corners), then (3) the top (including the top
//  corners) as shown below, replicating the outermost bytes
//  of the original frame outward:
//
//               ----------------------------
//              |                            |
//              |            (3)             |
//              |                            |
//              |----------------------------|
//              |     |                |     |
//              |     |                |     |
//              |     |                |     |
//              |     |    original    |     |
//              |     |     frame      |     |
//              |     |                |     |
//              | (2) |                | (2) |
//              |     |                |     |
//              |     |                |     |
//              |     |----------------|     |
//              |     |                |     |
//              |     |      (1)       |     |
//              |     |                |     |
//               ----------------------------
//
//--------------------------------------------------------------------------;
template<typename PIXTYPE>
void ExpandPlane_NV12(
    PIXTYPE* StartPtr,
    Ipp32s   frameWidth,
    Ipp32s   frameHeight,
    Ipp32s   pitchPixels,
    Ipp32s   pels)
{
    Ipp32s   row, col;
    PIXTYPE  uLeftFillVal[2];
    PIXTYPE  uRightFillVal[2];
    PIXTYPE* pByteSrc;

    PIXTYPE *pSrc = StartPtr + (frameHeight - 1)*pitchPixels;
    PIXTYPE *pDst = pSrc + pitchPixels;
    // section 1 at bottom
    // obtain pointer to start of bottom row of original frame
    for (row=0; row < pels; row++, pDst += pitchPixels) {
        memcpy(pDst, pSrc, frameWidth*sizeof(PIXTYPE));
    }

    // section 2 on left and right
    // obtain pointer to start of first row of original frame
    pByteSrc = StartPtr;
    for (row=0; row<(frameHeight + pels); row++, pByteSrc += pitchPixels)
    {
        // get fill values from left and right columns of original frame
        uLeftFillVal[0] = *pByteSrc;
        uLeftFillVal[1] = *(pByteSrc+1);
        uRightFillVal[0] = *(pByteSrc + frameWidth - 2);
        uRightFillVal[1] = *(pByteSrc + frameWidth - 1);

        // fill all bytes on both edges
        for (col=0; col<pels; col++)
        {
            *(pByteSrc - 2*pels + 2*col) = uLeftFillVal[0];
            *(pByteSrc - 2*pels + 2*col+1) = uLeftFillVal[1];
            *(pByteSrc + frameWidth + 2*col) = uRightFillVal[0];
            *(pByteSrc + frameWidth + 2*col+1) = uRightFillVal[1];
        }
    }

    // section 3 at top
    // obtain pointer to top row of original frame, less expand pels
    pSrc = StartPtr - 2*pels;
    pDst = pSrc - pitchPixels;
    for (row=0; row<pels; row++, pDst -= pitchPixels) {
       memcpy(pDst, pSrc, sizeof(PIXTYPE)*(frameWidth + 2*(pels + pels)));
    }
}   // end ExpandPlane

template<typename PIXTYPE>
void ExpandPlane(
    PIXTYPE* StartPtr,
    Ipp32s   frameWidth,
    Ipp32s   frameHeight,
    Ipp32s   pitchPixels,
    Ipp32s   pels)
{
    Ipp32s   row, col;
    PIXTYPE  uLeftFillVal;
    PIXTYPE  uRightFillVal;
    PIXTYPE* pByteSrc;

    PIXTYPE *pSrc = StartPtr + (frameHeight - 1)*pitchPixels;
    PIXTYPE *pDst = pSrc + pitchPixels;
    // section 1 at bottom
    // obtain pointer to start of bottom row of original frame
    for (row=0; row < pels; row++, pDst += pitchPixels) {
        memcpy(pDst, pSrc, frameWidth*sizeof(PIXTYPE));
    }

    // section 2 on left and right
    // obtain pointer to start of first row of original frame
    pByteSrc = StartPtr;
    for (row=0; row<(frameHeight + pels); row++, pByteSrc += pitchPixels)
    {
        // get fill values from left and right columns of original frame
        uLeftFillVal = *pByteSrc;
        uRightFillVal = *(pByteSrc + frameWidth - 1);

        // fill all bytes on both edges
        for (col=0; col<pels; col++)
        {
            *(pByteSrc - pels + col) = uLeftFillVal;
            *(pByteSrc + frameWidth + col) = uRightFillVal;
        }
    }

    // section 3 at top
    // obtain pointer to top row of original frame, less expand pels
    pSrc = StartPtr - pels;
    pDst = pSrc - pitchPixels;
    for (row=0; row<pels; row++, pDst -= pitchPixels)
       memcpy(pDst, pSrc, sizeof(PIXTYPE)*(frameWidth + pels + pels));

}   // end ExpandPlane

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Intra16x16SelectAndPredict(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32u *puAIMBSAD, PIXTYPE *pPredBuf)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    T_AIMode BestMode;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>& cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMB = cur_mb.uMB;

    cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA_16x16;
    if( core_enc->m_Analyse & ANALYSE_RD_OPT ){
        *puAIMBSAD = H264CoreEncoder_Intra16x16SelectRD(
            state,
            curr_slice,
            cur_mb.mbPtr,
            core_enc->m_pReconstructFrame->m_pYPlane +
                core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field],
            cur_mb.mbPitchPixels,
            &BestMode,
            pPredBuf);
    }else{
        *puAIMBSAD = H264CoreEncoder_AIModeSelectOneMB_16x16(
            state,
            curr_slice,
            cur_mb.mbPtr,
            core_enc->m_pReconstructFrame->m_pYPlane +
                core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field],
            cur_mb.mbPitchPixels,
            &BestMode,
            pPredBuf);
    }
    curr_slice->m_cur_mb.LocalMacroblockInfo->intra_16x16_mode = BestMode;   // Selected mode is stored in block 0
//    if ((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE)) {
    if (!(core_enc->m_Analyse & ANALYSE_RD_OPT) && (core_enc->m_Analyse & ANALYSE_RD_MODE)) {
        H264BsBase *pBitstream = curr_slice->m_pbitstream;
        H264BsFake_Reset(curr_slice->fakeBitstream);
        //H264BsFake_CopyContext_CABAC(curr_slice->fakeBitstream, pBitstream, !curr_slice->m_is_cur_mb_field, 0);
        H264BsBase_CopyContextCABAC_I16x16(&curr_slice->fakeBitstream->m_base, pBitstream, !curr_slice->m_is_cur_mb_field);
        curr_slice->m_pbitstream = (H264BsBase *)curr_slice->fakeBitstream;
        //H264MacroblockLocalInfo sLocalMBinfo = *cur_mb.LocalMacroblockInfo;
        //H264MacroblockGlobalInfo sGlobalMBinfo = *cur_mb.GlobalMacroblockInfo;
        pSetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo, false);
        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp = cur_mb.LocalMacroblockInfo->cbp_bits = 0;
        H264CoreEncoder_TransQuantIntra16x16_RD(state, curr_slice);
        H264CoreEncoder_Put_MBHeader_Fake(state, curr_slice);
        H264CoreEncoder_Put_MBLuma_Fake(state, curr_slice);
        Ipp32s bs = H264BsFake_GetBsOffset(curr_slice->fakeBitstream);
        //Ipp32s d = SSD16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field], cur_mb.mbPitchPixels)<<5;
        Ipp32s d = SSD16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, cur_mb.mb16x16.reconstruct, 16)<<5;
        *puAIMBSAD = d + cur_mb.lambda * bs;
        curr_slice->m_pbitstream = pBitstream;
        //*cur_mb.LocalMacroblockInfo = sLocalMBinfo;
        //*cur_mb.GlobalMacroblockInfo = sGlobalMBinfo;
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_MB_Decision(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u best_sad, uAIMBSAD_16x16, uAIMBSAD_4x4, uAIMBSAD_8x8, uBestIntraSAD;
    MB_Type uBestIntraMBType;
    bool bIntra8x8 = false;
    T_AIMode intra_types_save[16] = {0,};
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s MBHasEdges;
    Ipp8u  uMBQP = getLumaQP51(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.bit_depth_luma);
    Ipp32u uBestInterSAD;
    Ipp32s uMB = cur_mb.uMB;

    best_sad = uAIMBSAD_4x4 = uAIMBSAD_8x8 = uBestIntraSAD = MAX_SAD>>1;
    cur_mb.LocalMacroblockInfo->cbp_bits =
    cur_mb.LocalMacroblockInfo->cbp_bits_chroma =
    cur_mb.LocalMacroblockInfo->cbp =
    cur_mb.LocalMacroblockInfo->intra_chroma_mode = 0;
    cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
    cur_mb.LocalMacroblockInfo->cbp_chroma = 0xffffffff;
    cur_mb.LocalMacroblockInfo->cost = MAX_SAD;
    cur_mb.m_uIntraCBP4x4 = 0xffff;
    cur_mb.m_uIntraCBP8x8 = 0xffff;

    if (curr_slice->m_slice_type != INTRASLICE)
    {
        if(core_enc->m_Analyse & ANALYSE_INTRA_IN_ME)
        {
            best_sad = (BPREDSLICE == curr_slice->m_slice_type) ? H264CoreEncoder_ME_B(state, curr_slice) : H264CoreEncoder_ME_P(state, curr_slice);
            if (core_enc->m_PicParamSet.chroma_format_idc == 0)
                cur_mb.LocalMacroblockInfo->cbp_chroma = 0;
            cur_mb.LocalMacroblockInfo->cost = best_sad;
            return best_sad;
        }
        else
            uBestInterSAD = ((BPREDSLICE == curr_slice->m_slice_type) ? H264CoreEncoder_ME_B(state, curr_slice) : H264CoreEncoder_ME_P(state, curr_slice));

        if (!(core_enc->m_Analyse & ANALYSE_INTRA_IN_ME))
        {
            // MB type classification. Only need to change anything if INTRA is chosen, as the ME function filled in MB info for best INTER mode and cbp's.
            // save INTER results for possible later switch back to INTER
            MBTypeValue MBTypeInter = cur_mb.GlobalMacroblockInfo->mbtype;
            Ipp32u uInterCBP4x4 = cur_mb.LocalMacroblockInfo->cbp_luma;
            Ipp32s InterMB8x8PackFlag = pGetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo);

            Ipp32s mb_type = cur_mb.GlobalMacroblockInfo->mbtype;
            if(mb_type != MBTYPE_SKIPPED)
            {
                if((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE) || !(uBestInterSAD < (Ipp32u)EmptyThreshold[uMBQP]))
                {
                    Ipp32s cost_chroma=0;
                    Ipp32u chroma_cbp = cur_mb.LocalMacroblockInfo->cbp_chroma;
                    if ((core_enc->m_Analyse & ANALYSE_ME_CHROMA) && core_enc->m_PicParamSet.chroma_format_idc != 0)
                    {
                        Ipp32s uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field];
                        if((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE))
                        {
                            pSetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo, false);
                            cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA; //make INTRA prediction - the same for I16x16, I8x8, I4x4
                            cost_chroma = H264CoreEncoder_IntraSelectChromaRD(state, curr_slice,
                                core_enc->m_pCurrentFrame->m_pUPlane + uOffset,
                                core_enc->m_pReconstructFrame->m_pUPlane + uOffset,
                                core_enc->m_pCurrentFrame->m_pVPlane + uOffset,
                                core_enc->m_pReconstructFrame->m_pVPlane + uOffset,
                                cur_mb.mbPitchPixels,
                                &cur_mb.LocalMacroblockInfo->intra_chroma_mode,
                                cur_mb.mbChromaIntra.prediction,
                                cur_mb.mbChromaIntra.prediction+8);
                        }
                        else
                        {
                            PIXTYPE* pPredBuf = cur_mb.mbChromaIntra.prediction;
                            //Get intra chroma prediction
                            Ipp8u mode;
                            cost_chroma = H264CoreEncoder_AIModeSelectChromaMBs_8x8(state, curr_slice,
                                core_enc->m_pCurrentFrame->m_pUPlane + uOffset,
                                core_enc->m_pReconstructFrame->m_pUPlane + uOffset,
                                core_enc->m_pCurrentFrame->m_pVPlane + uOffset,
                                core_enc->m_pReconstructFrame->m_pVPlane + uOffset,
                                cur_mb.mbPitchPixels,
                                &mode,
                                pPredBuf,
                                pPredBuf + 8);
                        }
                    }
                    H264CoreEncoder_Intra16x16SelectAndPredict(state, curr_slice, &uAIMBSAD_16x16, cur_mb.mb16x16.prediction);
                    uBestIntraSAD = uAIMBSAD_16x16;
                    uBestIntraMBType = MBTYPE_INTRA_16x16;
                    // FAST intra decision
                    //f if (uAIMBSAD_16x16+cost_chroma <= 2 * uBestInterSAD && ((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE) || (curr_slice->m_uMBInterSAD >= rd_quant_intra_min[uMBQP]))) {
                    if (uAIMBSAD_16x16+cost_chroma <= (uBestInterSAD * 5 >> 2))
                    {
                        if ((core_enc->m_Analyse & ANALYSE_I_4x4) || (core_enc->m_Analyse & ANALYSE_I_8x8))
                        {
                            // Use edge detection to determine if the MB is a flat region
                            ownEdgesDetect16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, EdgePelDiffTable[uMBQP], EdgePelCountTable[uMBQP], &MBHasEdges);
                            if (MBHasEdges)
                            {
                                if (core_enc->m_Analyse & ANALYSE_I_8x8)
                                {
                                    pSetMB8x8TSFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, true);
                                    H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock8x8(state, curr_slice, uBestIntraSAD, &uAIMBSAD_8x8);
                                    //Save intra_types
                                    memcpy(intra_types_save, curr_slice->m_cur_mb.intra_types, 16 * sizeof(T_AIMode));
                                    if (uAIMBSAD_8x8 < uBestIntraSAD)
                                    {
                                        uBestIntraSAD = uAIMBSAD_8x8;
                                        uBestIntraMBType = MBTYPE_INTRA;
                                        bIntra8x8 = true;
                                    }
                                    pSetMB8x8TSFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, false);
                                    if ((core_enc->m_Analyse & ANALYSE_I_4x4) && (uAIMBSAD_8x8 <= (uAIMBSAD_16x16 * 5 >> 2)))
                                    {
                                        H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(state, curr_slice, uBestIntraSAD, &uAIMBSAD_4x4);
                                        if (uAIMBSAD_4x4 < uBestIntraSAD)
                                        {
                                            uBestIntraSAD = uAIMBSAD_4x4;
                                            uBestIntraMBType = MBTYPE_INTRA;
                                            bIntra8x8 = false;
                                        }
                                    }
                                }
                                else if (core_enc->m_Analyse & ANALYSE_I_4x4)
                                {
                                    H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(state, curr_slice, uBestIntraSAD, &uAIMBSAD_4x4);
                                    if(uAIMBSAD_4x4 < uBestIntraSAD)
                                    {
                                        uBestIntraSAD = uAIMBSAD_4x4;
                                        uBestIntraMBType = MBTYPE_INTRA;
                                        bIntra8x8 = false;
                                    }
                                }
                            }
                        }
                    }
                    cur_mb.LocalMacroblockInfo->cbp_chroma = chroma_cbp;
                    uBestIntraSAD += cost_chroma;
                    if (!((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE)) && (curr_slice->m_slice_type == BPREDSLICE))
                        uBestIntraSAD += BITS_COST(9, glob_RDQM[uMBQP]);
                    if (uBestIntraSAD < uBestInterSAD)
                    {
                        cur_mb.GlobalMacroblockInfo->mbtype = (MBTypeValue)(uBestIntraMBType);
                        pSetMB8x8TSPackFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, bIntra8x8);
                        if (uBestIntraMBType == MBTYPE_INTRA && !bIntra8x8)
                        {
                            cur_mb.LocalMacroblockInfo->cbp_luma = cur_mb.m_uIntraCBP4x4;
                            //Restore intra_types
    //                        memcpy(curr_slice->m_cur_mb.intra_types, intra_types_save, 16*sizeof(T_AIMode));
                        } else if (uBestIntraMBType == MBTYPE_INTRA && bIntra8x8)
                        {
                            cur_mb.LocalMacroblockInfo->cbp_luma = cur_mb.m_uIntraCBP8x8;
                            //Restore intra_types
                            memcpy(curr_slice->m_cur_mb.intra_types, intra_types_save, 16*sizeof(T_AIMode));
                        }
                        else
                            cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
                        cur_mb.LocalMacroblockInfo->cbp_chroma = 0xffffffff;
                        curr_slice->m_Intra_MB_Counter++;
                        best_sad = uBestIntraSAD;
                    }
                    else
                    {
                        cur_mb.LocalMacroblockInfo->intra_chroma_mode = 0; //Needed for packing
                        cur_mb.GlobalMacroblockInfo->mbtype = MBTypeInter;
                        cur_mb.LocalMacroblockInfo->cbp_luma = uInterCBP4x4;
                        best_sad = uBestInterSAD;
                        pSetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo, InterMB8x8PackFlag);
                    }
                }
                else
                    best_sad = uBestInterSAD;
            }
        }
    }
    else // intra slice
    {
        //Disable chroma coding in RD, no side effect in non RD mode
        Ipp32s tmp_chroma_format_idc = cur_mb.chroma_format_idc;
        if (((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE)) && core_enc->m_PicParamSet.chroma_format_idc != 0)
        {
            cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_INTRA; //make INTRA prediction - the same for I16x16, I8x8, I4x4
            Ipp32s uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field];
            H264CoreEncoder_IntraSelectChromaRD(
                state,
                curr_slice,
                core_enc->m_pCurrentFrame->m_pUPlane + uOffset,
                core_enc->m_pReconstructFrame->m_pUPlane + uOffset,
                core_enc->m_pCurrentFrame->m_pVPlane + uOffset,
                core_enc->m_pReconstructFrame->m_pVPlane + uOffset,
                cur_mb.mbPitchPixels,
                &cur_mb.LocalMacroblockInfo->intra_chroma_mode,
                cur_mb.mbChromaIntra.prediction,
                cur_mb.mbChromaIntra.prediction + 8);
            cur_mb.chroma_format_idc = 0;
        }
        H264CoreEncoder_Intra16x16SelectAndPredict(state, curr_slice, &uAIMBSAD_16x16, cur_mb.mb16x16.prediction);
        uBestIntraMBType = MBTYPE_INTRA_16x16;
        best_sad = uAIMBSAD_16x16;
        if ((core_enc->m_Analyse & ANALYSE_I_4x4) || (core_enc->m_Analyse & ANALYSE_I_8x8))
        {
            // Use edge detection to determine if the MB is a flat region
            ownEdgesDetect16x16(cur_mb.mbPtr, cur_mb.mbPitchPixels, EdgePelDiffTable[uMBQP], EdgePelCountTable[uMBQP], &MBHasEdges);
            if (MBHasEdges)
            {
                if (core_enc->m_Analyse & ANALYSE_I_8x8)
                {
                    pSetMB8x8TSFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, true);
                    H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock8x8(state, curr_slice, best_sad, &uAIMBSAD_8x8);
                    //Save intra_types
                    memcpy( intra_types_save, curr_slice->m_cur_mb.intra_types, 16*sizeof(T_AIMode));
                    if( uAIMBSAD_8x8 < best_sad )
                    {
                        best_sad = uAIMBSAD_8x8;
                        uBestIntraMBType = MBTYPE_INTRA;
                        bIntra8x8 = true;
                    }
                    pSetMB8x8TSFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, false);
                }
                if (core_enc->m_Analyse & ANALYSE_I_4x4)
                {
                    H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(state, curr_slice, best_sad, &uAIMBSAD_4x4);
                    if (uAIMBSAD_4x4 < best_sad)
                    {
                        uBestIntraMBType = MBTYPE_INTRA;
                        best_sad = uAIMBSAD_4x4;
                        bIntra8x8 = false;
                    }
                }
            }
        }

        cur_mb.chroma_format_idc = tmp_chroma_format_idc;
        cur_mb.GlobalMacroblockInfo->mbtype = (MBTypeValue)(uBestIntraMBType);
        pSetMB8x8TSPackFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, bIntra8x8);
        if (uBestIntraMBType == MBTYPE_INTRA && !bIntra8x8)
            cur_mb.LocalMacroblockInfo->cbp_luma = cur_mb.m_uIntraCBP4x4;
        else if(uBestIntraMBType == MBTYPE_INTRA && bIntra8x8)
        {
            cur_mb.LocalMacroblockInfo->cbp_luma = cur_mb.m_uIntraCBP8x8;
            //Restore intra_types
            memcpy(curr_slice->m_cur_mb.intra_types, intra_types_save, 16*sizeof(T_AIMode));
        }
        else
            cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
        cur_mb.LocalMacroblockInfo->cbp_chroma = 0xffffffff;
    }
    if(core_enc->m_PicParamSet.chroma_format_idc == 0)
    {
        // Monochrome
        cur_mb.LocalMacroblockInfo->cbp_chroma = 0; // Clear chroma blocks.
    }

    cur_mb.LocalMacroblockInfo->cost =  best_sad;

    return best_sad;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_ReconstuctCBP(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> *cur_mb)
{
    const Ipp8u ICBPTAB[6] = {0,16,32,15,31,47};
    if (cur_mb->GlobalMacroblockInfo->mbtype == MBTYPE_INTRA_16x16)
    {
        Ipp32s N = CALC_16x16_INTRA_MB_TYPE(INTRASLICE,
                                cur_mb->LocalMacroblockInfo->intra_16x16_mode,
                                cur_mb->MacroblockCoeffsInfo->chromaNC,
                                cur_mb->MacroblockCoeffsInfo->lumaAC)-1;
        cur_mb->LocalMacroblockInfo->cbp = ICBPTAB[N>>2];
    }
}

//////////////////////////////////
// Compress one slice
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Compress_Slice(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, bool is_first_mb)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    H264BsReal* pBitstream = (H264BsReal *)curr_slice->m_pbitstream;
    Ipp32s slice_num = curr_slice->m_slice_number;
    EnumSliceType slice_type = curr_slice->m_slice_type;
    Ipp8u uUsePCM = 0;

    Ipp8u *pStartBits;
    Ipp32u uStartBitOffset;

    Ipp32u uRecompressMB;
    Ipp8u  iLastQP;
    Ipp32u uSaved_Skip_Run;

    Ipp8u bSeenFirstMB = false;

    Status status = UMC_OK;

    Ipp32u iLimit = 0;
    bool bExpandLimit = true;

    Ipp32u uNumMBs = core_enc->m_HeightInMBs * core_enc->m_WidthInMBs;
    Ipp32u uFirstMB = core_enc->m_field_index * uNumMBs;

    Ipp32s MBYAdjust = 0;
    if (core_enc->m_field_index)
    {
        MBYAdjust  = core_enc->m_HeightInMBs;
    }

    curr_slice->m_InitialOffset = core_enc->m_InitialOffsets[core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index]];
    curr_slice->m_is_cur_mb_field = core_enc->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE;
    curr_slice->m_is_cur_mb_bottom_field = core_enc->m_pCurrentFrame->m_bottom_field_flag[core_enc->m_field_index] == 1;

    //    curr_slice->m_use_transform_for_intra_decision = core_enc->m_params.use_transform_for_intra_decision ?
    //            (curr_slice->m_slice_type == INTRASLICE) : false;
    curr_slice->m_use_transform_for_intra_decision = 1;

#ifdef SLICE_THREADING_LOAD_BALANCING
    Ipp64s *ticks;
    if (core_enc->m_pCurrentFrame->m_PicCodType == PREDPIC)
        ticks = core_enc->m_P_ticks_per_macroblock;
    else // BPREDPIC
        ticks = core_enc->m_B_ticks_per_macroblock;
    curr_slice->m_ticks_per_slice = 0;
#endif // SLICE_THREADING_LOAD_BALANCING

    // loop over all MBs in the picture
    for (Ipp32u uMB = uFirstMB; uMB < uFirstMB + uNumMBs; uMB++)
    {
        // Is this MB in the current slice?  If not, move on...
        if (core_enc->m_pCurrentFrame->m_mbinfo.mbs[uMB].slice_id != slice_num)
            continue;
        else if(!bSeenFirstMB)
        {
            // Reset xpos and ypos in framedata struct
            // This is necessary because the same slice may be recoded multiple times.

            // reset intra MB counter per slice
            curr_slice->m_Intra_MB_Counter = 0;
            curr_slice->m_MB_Counter = 0;

            // Fill in the first mb in slice field in the slice header.
            curr_slice->m_first_mb_in_slice = is_first_mb ? 0 : uMB - uFirstMB;

            // Fill in the current deblocking filter parameters.
            curr_slice->m_slice_alpha_c0_offset = (Ipp8s)core_enc->m_params.deblocking_filter_alpha;
            curr_slice->m_slice_beta_offset = (Ipp8s)core_enc->m_params.deblocking_filter_beta;
            curr_slice->m_disable_deblocking_filter_idc =  core_enc->m_params.deblocking_filter_idc;
            curr_slice->m_cabac_init_idc = core_enc->m_params.cabac_init_idc;

            // Write a slice header
            H264BsReal_PutSliceHeader(pBitstream,
                core_enc->m_SliceHeader,
                core_enc->m_PicParamSet,
                core_enc->m_SeqParamSet,
                core_enc->m_PicClass,
                curr_slice);
            bSeenFirstMB = true;

            // Fill in the correct value for m_iLastXmittedQP, used to correctly code
            // the per MB QP Delta
            curr_slice->m_iLastXmittedQP = core_enc->m_PicParamSet.pic_init_qp + curr_slice->m_slice_qp_delta;
            Ipp32s SliceQPy = curr_slice->m_iLastXmittedQP;

            if (core_enc->m_params.entropy_coding_mode_flag)
            {
                if (slice_type == INTRASLICE)
                    H264BsReal_InitializeContextVariablesIntra_CABAC(pBitstream, SliceQPy);
                else
                    H264BsReal_InitializeContextVariablesInter_CABAC(pBitstream, SliceQPy, curr_slice->m_cabac_init_idc);
            }

            // Initialize the MB skip run counter
            curr_slice->m_uSkipRun = 0;
        }
#ifdef SLICE_THREADING_LOAD_BALANCING
        ticks[uMB] = vm_time_get_tick();
#endif // SLICE_THREADING_LOAD_BALANCING

        //        cur_mb.lambda = int( (pow(2.0,MAX(0,(curr_slice->m_iLastXmittedQP-12))/3.0)*0.85) + 0.5);
        cur_mb.lambda = lambda_sq[curr_slice->m_iLastXmittedQP];
        cur_mb.uMB = uMB;
        cur_mb.chroma_format_idc = core_enc->m_PicParamSet.chroma_format_idc;
        cur_mb.mbPtr = core_enc->m_pCurrentFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field];
        cur_mb.mbPitchPixels =  core_enc->m_pCurrentFrame->m_pitchPixels << curr_slice->m_is_cur_mb_field;
        cur_mb.uMBx = uMB % core_enc->m_WidthInMBs;
        cur_mb.uMBy = uMB / core_enc->m_WidthInMBs - MBYAdjust;
        H264CoreEncoder_UpdateCurrentMBInfo(state, curr_slice);
        cur_mb.lumaQP = getLumaQP(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.bit_depth_luma);
        cur_mb.lumaQP51 = getLumaQP51(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.bit_depth_luma);
        cur_mb.chromaQP = getChromaQP(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.chroma_qp_index_offset, core_enc->m_SeqParamSet.bit_depth_chroma);
        pSetMB8x8TSFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo, 0);
        curr_slice->m_MB_Counter++;
        H264BsBase_GetState(&pBitstream->m_base, &pStartBits, &uStartBitOffset);
        iLastQP = curr_slice->m_iLastXmittedQP;
        uSaved_Skip_Run = curr_slice->m_uSkipRun;   // To restore it if we recompress
        uUsePCM = 0;    // Don't use the PCM mode initially.
        //cur_mb.LocalMacroblockInfo->QP = curr_slice->m_iLastXmittedQP;
        do
        {    // this is to recompress MBs that are too big.
            H264CoreEncoder_MB_Decision(state, curr_slice);
            Ipp32s mb_bits;
            Ipp32s bit_offset;
            if (core_enc->m_PicParamSet.entropy_coding_mode)
            {
                bit_offset = pBitstream->m_base.m_nReadyBits;
                if(pBitstream->m_base.m_nReadyBits == 9)
                    bit_offset = 8;
            }
            // Code the macroblock, all planes
            cur_mb.LocalMacroblockInfo->cbp_bits = 0;
            cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
            uSaved_Skip_Run = curr_slice->m_uSkipRun;
            H264CoreEncoder_CEncAndRecMB(state, curr_slice);

            mb_bits = 0;
            status = H264CoreEncoder_Put_MB_Real(state, curr_slice);
            if (status != UMC_OK)
                goto done;

//            if (core_enc->m_PicParamSet.entropy_coding_mode){
//                pBitstream->EncodeFinalSingleBin_CABAC( (uMB==uFirstMB + uNumMBs - 1) || (core_enc->m_pCurrentFrame->m_mbinfo.mbs[uMB + 1].slice_id != slice_num) );
//                mb_bits = bit_offset - pBitstream->m_nReadyBits;
//            }

            Ipp8u *pEndBits;
            Ipp32u uEndBitOffset;
            H264BsBase_GetState(&pBitstream->m_base, &pEndBits, &uEndBitOffset);

            mb_bits += (Ipp32u) (pEndBits - pStartBits)*8;
            if (uEndBitOffset >= uStartBitOffset)
                mb_bits += uEndBitOffset - uStartBitOffset;
            else
                mb_bits -= uStartBitOffset - uEndBitOffset;

#ifdef ALT_BITSTREAM_ALLOC
            //Expand buffer if it is nearly full
            Ipp32u bytesInBuffer = H264BsBase_GetBsSize(&(pBitstream->m_base));

            if (bytesInBuffer >= 3 * (pBitstream->m_base.m_maxBsSize >> 2))
            {
                pBitstream->m_base.m_maxBsSize <<= 1;
                Ipp8u* tmpBitstreamBuf = (Ipp8u*)H264_Malloc(pBitstream->m_base.m_maxBsSize);
                ippsCopy_8u(pBitstream->m_base.m_pbsBase,tmpBitstreamBuf,bytesInBuffer);
                H264_Free(pBitstream->m_base.m_pbsBase);
                pBitstream->m_base.m_pbsBase = tmpBitstreamBuf;
                pBitstream->m_pbsRBSPBase = tmpBitstreamBuf;
                pBitstream->m_base.m_pbs = pBitstream->m_base.m_pbsBase + bytesInBuffer;
            }
#endif

            // Should not recompress for CABAC
            if (!core_enc->m_PicParamSet.entropy_coding_mode && (mb_bits > MB_RECODE_THRESH) && core_enc->m_params.rate_controls.method == H264_RCM_QUANT)
            {
                // OK, this is bad, it's not compressing very much!!!
                // TBD: Tune this decision to QP...  Higher QPs will progressively trash PSNR,
                // so if they are still using a lot of bits, then PCM coding is extra attractive.

                // We're going to be recoding this MB, so reset some stuff.
                H264BsBase_SetState(&pBitstream->m_base, pStartBits, uStartBitOffset);
                // Zero out unused bits in buffer before OR in next op
                // This removes dependency on buffer being zeroed out.
                *pStartBits = (Ipp8u)((*pStartBits >> (8-uStartBitOffset)) << (8-uStartBitOffset));

                curr_slice->m_iLastXmittedQP = iLastQP; // Restore the last xmitted QP
                curr_slice->m_uSkipRun = uSaved_Skip_Run;   // Restore the skip run

                // If the QP has only been adjusted up 0 or 1 times, and QP != 51
                if (((cur_mb.LocalMacroblockInfo->QP -
                    core_enc->m_PicParamSet.pic_init_qp + curr_slice->m_slice_qp_delta) < 2) &&
                    (cur_mb.LocalMacroblockInfo->QP != 51))
                {
                    // Quantize more and try again!
                    cur_mb.LocalMacroblockInfo->QP++;
                    uRecompressMB = 1;
                } else {
                    // Code this block as a PCM MB next time around.
                    uUsePCM = 1;
                    uRecompressMB = 0;
                    // Reset the MB QP value to the "last transmitted QP"
                    // Since no DeltaQP will be transmitted for a PCM block
                    // This is important, since the Loop Filter will use the
                    // this value in filtering this MB
                    cur_mb.LocalMacroblockInfo->QP = curr_slice->m_iLastXmittedQP;
                }

            } else
                uRecompressMB = 0;
        } while (uRecompressMB);        // End of the MB recompression loop.

        // If the above MB encoding failed to efficiently predict the MB, then
        // code it as raw pixels using the mb_type = PCM
        if (uUsePCM)
        {
            cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_PCM;
            cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;

            memset(cur_mb.MacroblockCoeffsInfo->numCoeff, 16, 24);

            Ipp32s  k;     // block number, 0 to 15
            for (k = 0; k < 16; k++) {
                cur_mb.intra_types[k] = 2;
                cur_mb.MVs[LIST_0]->MotionVectors[k] = null_mv;
                cur_mb.MVs[LIST_1]->MotionVectors[k] = null_mv;
                cur_mb.RefIdxs[LIST_0]->RefIdxs[k] = -1;
                cur_mb.RefIdxs[LIST_1]->RefIdxs[k] = -1;
            }

            H264CoreEncoder_Put_MBHeader_Real(state, curr_slice);   // PCM values are written in the MB Header.
        }

        if (core_enc->m_MaxSliceSize)
        {
            Ipp32u size = (H264BsBase_GetBsOffset(&pBitstream->m_base) >> 3) + 3/*Start code*/ + 5;
            iLimit = core_enc->m_MaxSliceSize;
            if(bExpandLimit)
            {
                if(core_enc->m_MaxSliceSize < size)
                    iLimit = size; // expand slice limit to fit at least one macroblock
            }
            if(iLimit < size)
            {
                //Increase slice_id
                for (Ipp32u mb = uMB; mb < uFirstMB + uNumMBs; mb++)
                    core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb].slice_id++;
                curr_slice->m_MB_Counter--;
                uMB--;
                curr_slice->m_uSkipRun = uSaved_Skip_Run;    // Restore the last skipRun
                //Restore bitstream
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                    H264BsReal_RestoreCABACState(pBitstream);
                }else{
                    H264BsBase_SetState(&pBitstream->m_base, pStartBits, uStartBitOffset);
                    //Reset unused bits
                    *pStartBits = *pStartBits & (0xff<<(8- uStartBitOffset));
                }
                bExpandLimit = true;
            }
            else
                bExpandLimit = false;
        }

        if (core_enc->m_PicParamSet.entropy_coding_mode){
            Ipp32s lastFlag = ((uMB == uFirstMB + uNumMBs - 1) || (core_enc->m_pCurrentFrame->m_mbinfo.mbs[uMB + 1].slice_id != slice_num));

            if (core_enc->m_MaxSliceSize && !lastFlag)
                H264BsReal_SaveCABACState(pBitstream);

            H264BsReal_EncodeFinalSingleBin_CABAC(pBitstream, lastFlag);
            H264CoreEncoder_ReconstuctCBP(&cur_mb);
        }

#ifdef SLICE_THREADING_LOAD_BALANCING
        ticks[uMB] = vm_time_get_tick() - ticks[uMB];
        curr_slice->m_ticks_per_slice += ticks[uMB];
#endif // SLICE_THREADING_LOAD_BALANCING
    }   // loop over MBs

#ifndef NO_FINAL_SKIP_RUN
    // Check if the last N MBs were skip blocks.  If so, write a final skip run
    // NOTE!  This is _optional_.  The encoder is not required to do this, and
    // decoders need to be able to handle it either way.

    // Even though skip runs are not written for I Slices, m_uSkipRun can only be
    // non-zero for non-I slices, so the following test is OK.
    if (curr_slice->m_uSkipRun !=0 && core_enc->m_params.entropy_coding_mode_flag==0) {
        H264BsReal_PutVLCCode(pBitstream, curr_slice->m_uSkipRun);
    }

#endif // NO_FINAL_SKIP_RUN

    // save the frame class

done:
    if (core_enc->m_PicParamSet.entropy_coding_mode) {
        H264BsReal_TerminateEncode_CABAC(pBitstream);
    }
    else {
        H264BsBase_WriteTrailingBits(&pBitstream->m_base);
    }

    return status;

}


template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32s H264CoreEncoder_ComputeMBFrameFieldCost(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice,
    bool is_frame)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s curMBAddr = curr_slice->m_cur_mb.uMB;
    curr_slice->m_cur_mb.uMBx = ((curMBAddr >> 1) % core_enc->m_WidthInMBs);
    if ((curMBAddr & 1) == 0) {
        curr_slice->m_cur_mb.uMBy = ((curMBAddr >> 1) / core_enc->m_WidthInMBs)*2;
    } else {
        curr_slice->m_cur_mb.uMBy = ((curMBAddr >> 1) / core_enc->m_WidthInMBs)*2;
        if (!is_frame)
            curr_slice->m_cur_mb.uMBy ++;
    }
    H264CoreEncoder_UpdateCurrentMBInfo(state, curr_slice);
    SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr], is_frame ? 0 : 1);
    SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr+1], is_frame ? 0 : 1);
    curr_slice->m_is_cur_mb_field = pGetMBFieldDecodingFlag(curr_slice->m_cur_mb.GlobalMacroblockInfo);
    return H264CoreEncoder_MB_Decision(state, curr_slice);
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_MBFrameFieldSelect(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u &curMBAddr = curr_slice->m_cur_mb.uMB;
#if 1
    Ipp32u uOffset = core_enc->m_pMBOffsets[curMBAddr].uLumaOffset[true][false];
    PIXTYPE* curr = core_enc->m_pCurrentFrame->m_pYPlane + uOffset;
    Ipp32s pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels;
    //Ipp32s pitchBytes = core_enc->m_pCurrentFrame->pitchBytes();
    Ipp32s frame_sad = SAD16x16(curr, pitchPixels, curr + pitchPixels, pitchPixels) + SAD16x16(curr + 16*pitchPixels, pitchPixels, curr + 17*pitchPixels, pitchPixels);
    Ipp32s field_sad = SAD16x16(curr, pitchPixels << 2, curr + (pitchPixels << 2), pitchPixels << 2) + SAD16x16(curr + pitchPixels, pitchPixels << 2, curr + (pitchPixels << 2) + pitchPixels, pitchPixels << 2);

    if ((frame_sad - field_sad) > 0)
    {
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr],   1);
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr+1], 1);
    } else {
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr],   0);
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr+1], 0);
    }

#elif 1
    Ipp32u field_sad = ComputeMBFrameFieldCost(false);
    curMBAddr++;
    field_sad += ComputeMBFrameFieldCost(false);
    curMBAddr--;

    Ipp32u frame_sad = ComputeMBFrameFieldCost(true);
    curMBAddr++;
    frame_sad += ComputeMBFrameFieldCost(true);
    curMBAddr--;

    if (field_sad < frame_sad)
    {
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr],1);
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr+1],1);
        curr_slice->m_is_cur_mb_field = pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
    }

#else
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr],0);
        SetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[curMBAddr+1],0);
#endif
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Compress_Slice_MBAFF(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s slice_num = curr_slice->m_slice_number;
    Ipp32u uAIMBSAD;            // MB prediction SAD for INTRA 4x4 mode
    Ipp32u uAIMBSAD_16x16;      // MB prediction SAD for INTRA 16x16 mode
    Ipp8u  uMBQP;
    MB_Type uBestIntraMBType;
    Ipp8u   uUsePCM = 0;

    Ipp8u *pStartBits;
    Ipp32u uStartBitOffset;

    Ipp32u uRecompressMB;
    Ipp8s  iLastQP;
    Ipp32u uSaved_Skip_Run;

    Ipp8u bSeenFirstMB = false;

    Status status = UMC_OK;

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u &curMBAddr = cur_mb.uMB;
    H264BsReal* pBitstream = (H264BsReal *)curr_slice->m_pbitstream;
    Ipp32u uNumMBs = core_enc->m_HeightInMBs * core_enc->m_WidthInMBs;
    Ipp32u uFirstMB = core_enc->m_field_index*uNumMBs;

    curr_slice->m_use_transform_for_intra_decision = core_enc->m_params.use_transform_for_intra_decision ?
                    (curr_slice->m_slice_type == INTRASLICE) : false;

    // loop over all MBs in the picture
    for (Ipp32u uMB = uFirstMB; uMB < uFirstMB + uNumMBs; uMB++)
    {
        // Is this MB in the current slice?  If not, move on...
        if (core_enc->m_pCurrentFrame->m_mbinfo.mbs[uMB].slice_id != slice_num) {
            continue;
        } else if (!bSeenFirstMB)
        {
            // Reset xpos and ypos in framedata struct
            // This is necessary because the same slice may be recoded multiple times.

            // reset intra MB counter per slice
            curr_slice->m_Intra_MB_Counter = 0;
            curr_slice->m_MB_Counter = 0;
            // Fill in the first mb in slice field in the slice header.
            curr_slice->m_first_mb_in_slice = uMB >> 1;

            // Fill in the current deblocking filter parameters.
            curr_slice->m_slice_alpha_c0_offset = (Ipp8s)core_enc->m_params.deblocking_filter_alpha;
            curr_slice->m_slice_beta_offset = (Ipp8s)core_enc->m_params.deblocking_filter_beta;
            curr_slice->m_disable_deblocking_filter_idc = core_enc->m_params.deblocking_filter_idc;
            curr_slice->m_cabac_init_idc = core_enc->m_params.cabac_init_idc;

            // Write a slice header
            H264BsReal_PutSliceHeader(
                pBitstream,
                core_enc->m_SliceHeader,
                core_enc->m_PicParamSet,
                core_enc->m_SeqParamSet,
                core_enc->m_PicClass,
                curr_slice);
            bSeenFirstMB = true;

            // Fill in the correct value for m_iLastXmittedQP, used to correctly code
            // the per MB QP Delta
            curr_slice->m_iLastXmittedQP = core_enc->m_PicParamSet.pic_init_qp + curr_slice->m_slice_qp_delta;
            Ipp32s SliceQPy = curr_slice->m_iLastXmittedQP;

            if (core_enc->m_params.entropy_coding_mode_flag)
            {
                if (curr_slice->m_slice_type==INTRASLICE)
                    H264BsReal_InitializeContextVariablesIntra_CABAC(
                        pBitstream,
                        SliceQPy);
                else
                    H264BsReal_InitializeContextVariablesInter_CABAC(
                        pBitstream,
                        SliceQPy,
                        curr_slice->m_cabac_init_idc);
            }

            // Initialize the MB skip run counter
            curr_slice->m_uSkipRun = 0;
        }

        curMBAddr = uMB;
        if ((uMB&1)==0) H264CoreEncoder_MBFrameFieldSelect(state, curr_slice);
        cur_mb.uMBx = ((uMB >> 1) % core_enc->m_WidthInMBs);
        if ((uMB & 1)==0) {
            cur_mb.uMBy = ((uMB >> 1) / core_enc->m_WidthInMBs) * 2;
        } else {
            if (!GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[uMB]))
                cur_mb.uMBy ++;
        }
        H264CoreEncoder_UpdateCurrentMBInfo(state, curr_slice);

        pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 0);

        curr_slice->m_MB_Counter++;
        H264BsBase_GetState(curr_slice->m_pbitstream, &pStartBits, &uStartBitOffset);
        iLastQP = curr_slice->m_iLastXmittedQP;
        uSaved_Skip_Run = curr_slice->m_uSkipRun;   // To restore it if we recompress
        uUsePCM = 0;    // Don't use the PCM mode initially.
        curr_slice->m_is_cur_mb_field = pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo);
        curr_slice->m_InitialOffset = core_enc->m_InitialOffsets[curr_slice->m_is_cur_mb_field & curMBAddr];
        curr_slice->m_is_cur_mb_bottom_field = (curr_slice->m_is_cur_mb_field & curMBAddr) == 1;

        do {    // this is to recompress MBs that are too big.
            uMBQP = getLumaQP51(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.bit_depth_luma);
            cur_mb.LocalMacroblockInfo->cbp_chroma =
            cur_mb.LocalMacroblockInfo->cbp_luma =
            cur_mb.LocalMacroblockInfo->cbp_bits =
            cur_mb.LocalMacroblockInfo->cbp_bits_chroma =
            cur_mb.LocalMacroblockInfo->cbp =
            cur_mb.LocalMacroblockInfo->intra_chroma_mode = 0;
            Ipp32s MBHasEdges;
            // Use edge detection to determine if the MB is a flat region
            ownEdgesDetect16x16(core_enc->m_pCurrentFrame->m_pYPlane + core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][curr_slice->m_is_cur_mb_field],
                core_enc->m_pCurrentFrame->m_pitchPixels<<curr_slice->m_is_cur_mb_field,uMBQP/2,EdgePelCountTable[uMBQP],&MBHasEdges);

            if (curr_slice->m_slice_type != INTRASLICE) {
                if (core_enc->m_Analyse & ANALYSE_INTRA_IN_ME) {
                    cur_mb.LocalMacroblockInfo->cost = (BPREDSLICE == curr_slice->m_slice_type) ? H264CoreEncoder_ME_B(state, curr_slice) : H264CoreEncoder_ME_P(state, curr_slice);
                    if (core_enc->m_PicParamSet.chroma_format_idc == 0)
                        cur_mb.LocalMacroblockInfo->cbp_chroma = 0;
                } else {
                    Ipp32u uMinIntraSAD, uBestIntraSAD, uBestInterSAD;
                    uBestInterSAD = (BPREDSLICE == curr_slice->m_slice_type) ? H264CoreEncoder_ME_B(state, curr_slice) : H264CoreEncoder_ME_P(state, curr_slice);
                    // save INTER results for possible later switch back to INTER
                    Ipp32u uInterCBP4x4 = cur_mb.LocalMacroblockInfo->cbp_luma;
                    MBTypeValue MBTypeInter = cur_mb.GlobalMacroblockInfo->mbtype;
                    Ipp32s InterMB8x8PackFlag = pGetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo);
                    uMinIntraSAD = rd_quant_intra_min[uMBQP];

                    if (MBHasEdges && (uBestInterSAD >= uMinIntraSAD)) {
                        H264CoreEncoder_Intra16x16SelectAndPredict(state, curr_slice, &uAIMBSAD_16x16, curr_slice->m_pMBEncodeBuffer);
                        uBestIntraSAD = uAIMBSAD_16x16;
                        uBestIntraMBType = MBTYPE_INTRA_16x16;
                        H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(state, curr_slice, uBestIntraSAD, &uAIMBSAD);
                        //FPV TODO ADD 8x8?
                        if (uAIMBSAD < uBestIntraSAD) {
                            uBestIntraSAD = uAIMBSAD;
                            uBestIntraMBType = MBTYPE_INTRA;
                        }
                    } else {
                        H264CoreEncoder_Intra16x16SelectAndPredict(state, curr_slice, &uAIMBSAD_16x16, curr_slice->m_pMBEncodeBuffer);
                        uBestIntraSAD = uAIMBSAD_16x16;
                        uBestIntraMBType = MBTYPE_INTRA_16x16;
                    }
                    if (uBestIntraSAD < uBestInterSAD) {
                        cur_mb.GlobalMacroblockInfo->mbtype = (MBTypeValue)(uBestIntraMBType);
                        cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
                        cur_mb.LocalMacroblockInfo->cbp_chroma = 0xffffffff;
                        curr_slice->m_Intra_MB_Counter++;
                    }
                    else {
                        cur_mb.GlobalMacroblockInfo->mbtype = MBTypeInter;
                        cur_mb.LocalMacroblockInfo->cbp_luma = uInterCBP4x4;
                        pSetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo, InterMB8x8PackFlag);
                    }
                }
            } else { // intra slice
                if (MBHasEdges) {
                    H264CoreEncoder_Intra16x16SelectAndPredict(state, curr_slice, &uAIMBSAD_16x16, curr_slice->m_pMBEncodeBuffer);
                    uBestIntraMBType = MBTYPE_INTRA_16x16;
                    H264CoreEncoder_AdvancedIntraModeSelectOneMacroblock(state, curr_slice, uAIMBSAD, &uAIMBSAD);
                    if (uAIMBSAD_16x16 < uAIMBSAD)
                        uBestIntraMBType = MBTYPE_INTRA;
                } else {
                    H264CoreEncoder_Intra16x16SelectAndPredict(state, curr_slice, &uAIMBSAD_16x16, curr_slice->m_pMBEncodeBuffer);
                    uBestIntraMBType = MBTYPE_INTRA_16x16;
                }
                cur_mb.GlobalMacroblockInfo->mbtype = (MBTypeValue)(uBestIntraMBType);
                cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;
                cur_mb.LocalMacroblockInfo->cbp_chroma = 0xffffffff;
                curr_slice->m_Intra_MB_Counter++;
            }
            cur_mb.LocalMacroblockInfo->cbp_bits = 0;
            cur_mb.LocalMacroblockInfo->cbp_bits_chroma = 0;
            cur_mb.uMB = uMB;
            H264CoreEncoder_CEncAndRecMB(state, curr_slice);
            status = H264CoreEncoder_Put_MB_Real(state, curr_slice);
            if (status != UMC_OK)
                goto done;

            Ipp8u *pEndBits;
            Ipp32u uEndBitOffset;

            H264BsBase_GetState(curr_slice->m_pbitstream, &pEndBits, &uEndBitOffset);

            Ipp32u mb_bits = (Ipp32u) (pEndBits - pStartBits)*8;
            if (uEndBitOffset >= uStartBitOffset)
                mb_bits += uEndBitOffset - uStartBitOffset;
            else
                mb_bits -= uStartBitOffset - uEndBitOffset;

            if (!core_enc->m_PicParamSet.entropy_coding_mode && (mb_bits > MB_RECODE_THRESH) && core_enc->m_params.rate_controls.method == H264_RCM_QUANT)
            {
                // OK, this is bad, it's not compressing very much!!!
                // TBD: Tune this decision to QP...  Higher QPs will progressively trash PSNR,
                // so if they are still using a lot of bits, then PCM coding is extra attractive.

                // We're going to be recoding this MB, so reset some stuff.
                H264BsBase_SetState(curr_slice->m_pbitstream, pStartBits, uStartBitOffset);    // Reset the BS
                // Zero out unused bits in buffer before OR in next op
                // This removes dependency on buffer being zeroed out.
                *pStartBits = (Ipp8u)((*pStartBits >> (8-uStartBitOffset)) << (8-uStartBitOffset));

                curr_slice->m_iLastXmittedQP = iLastQP; // Restore the last xmitted QP
                curr_slice->m_uSkipRun = uSaved_Skip_Run;   // Restore the skip run

                // If the QP has only been adjusted up 0 or 1 times, and QP != 51
                if (((cur_mb.LocalMacroblockInfo->QP -
                    core_enc->m_PicParamSet.pic_init_qp + curr_slice->m_slice_qp_delta) < 2) &&
                    (cur_mb.LocalMacroblockInfo->QP != 51))
                {
                    // Quantize more and try again!
                    cur_mb.LocalMacroblockInfo->QP++;
                    uRecompressMB = 1;
                } else {
                    // Code this block as a PCM MB next time around.
                    uUsePCM = 1;
                    uRecompressMB = 0;
                    // Reset the MB QP value to the "last transmitted QP"
                    // Since no DeltaQP will be transmitted for a PCM block
                    // This is important, since the Loop Filter will use the
                    // this value in filtering this MB
                    cur_mb.LocalMacroblockInfo->QP = curr_slice->m_iLastXmittedQP;
                }

            } else
                uRecompressMB = 0;

        } while (uRecompressMB);        // End of the MB recompression loop.

        // If the above MB encoding failed to efficiently predict the MB, then
        // code it as raw pixels using the mb_type = PCM
        if (uUsePCM)
        {
            cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_PCM;
            cur_mb.LocalMacroblockInfo->cbp_luma = 0xffff;

            memset(cur_mb.MacroblockCoeffsInfo->numCoeff, 16, 24);

            Ipp32s k;     // block number, 0 to 15
            for (k = 0; k < 16; k++) {
                cur_mb.intra_types[k] = 2;
                cur_mb.MVs[LIST_0]->MotionVectors[k] = null_mv;
                cur_mb.MVs[LIST_1]->MotionVectors[k] = null_mv;
                cur_mb.RefIdxs[LIST_0]->RefIdxs[k] = -1;
                cur_mb.RefIdxs[LIST_1]->RefIdxs[k] = -1;
            }

            H264CoreEncoder_Put_MBHeader_Real(state, curr_slice);   // PCM values are written in the MB Header.
        }

        /*
        if (!m_pbitstream->CheckBsLimit())
        {
        // If the buffer is filled, break out of encoding loop
        // Encoder::CompressFrame will write a blank frame to the bitstream.
        goto done;
        }*/

        if (core_enc->m_PicParamSet.entropy_coding_mode)
        {
            if ((uMB & 1) != 0)
            {
                H264BsReal_EncodeFinalSingleBin_CABAC(
                    pBitstream,
                    (uMB==(uFirstMB + uNumMBs - 1)) ||
                        (core_enc->m_pCurrentFrame->m_mbinfo.mbs[uMB + 1].slice_id != slice_num));
            }
            H264CoreEncoder_ReconstuctCBP(&cur_mb);
        }
    }   // loop over MBs

#ifndef NO_FINAL_SKIP_RUN
    // Check if the last N MBs were skip blocks.  If so, write a final skip run
    // NOTE!  This is _optional_.  The encoder is not required to do this, and
    // decoders need to be able to handle it either way.

    // Even though skip runs are not written for I Slices, m_uSkipRun can only be
    // non-zero for non-I slices, so the following test is OK.
    if (curr_slice->m_uSkipRun !=0 && core_enc->m_params.entropy_coding_mode_flag==0) {
        H264BsReal_PutVLCCode(pBitstream, curr_slice->m_uSkipRun);
    }

#endif // NO_FINAL_SKIP_RUN

done:
    if (core_enc->m_PicParamSet.entropy_coding_mode)
    {
        H264BsReal_TerminateEncode_CABAC(pBitstream);
    }
    // use core timing infrastructure to measure/report deblock filter time
    // by measuring the time to get from here to the start of EndPicture.
    return status;

}

////////////////////////////////////////////////////////////////////////////////
// InitializeMBData
//
// One-time (after allocation) initialization of the per MB data,
// specifically edge flags which are set to match picture edges,
// the MB offsets, and block index.
//
////////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_InitializeMBData(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u uMB = 0;
    Ipp32u uLumaOffsetFRM = 0;
    Ipp32u uChromaOffsetFRM = 0;
    Ipp32u uLumaOffsetFLD = core_enc->m_pCurrentFrame->m_bottom_field_flag[0]*core_enc->m_Pitch;
    Ipp32u uChromaOffsetFLD = core_enc->m_pCurrentFrame->m_bottom_field_flag[0]*core_enc->m_Pitch;
    Ipp32u uLumaOffsetAFRMFRM = 0;
    Ipp32u uChromaOffsetAFRMFRM = 0;
    Ipp32u uLumaOffsetAFRMFLD = 0;
    Ipp32u uChromaOffsetAFRMFLD = 0;
    Ipp32s chroma_width = 0, chroma_height = 0;

    switch( core_enc->m_PicParamSet.chroma_format_idc )
    {
        case 0:
        case 1:
            chroma_width  = 8;
            chroma_height = 8;
            break;
        case 2:
            chroma_width=8;
            chroma_height=16;
            break;
        case 3:
            chroma_width=16;
            chroma_height=16;
            break;
    }

    // Initialize the MB slices first, so that the edge calculations
    // below come out correctly.
    for (Ipp32s uMBRow = 0; uMBRow < core_enc->m_HeightInMBs; uMBRow++)
    {
        for (Ipp32s uMBCol = 0; uMBCol < core_enc->m_WidthInMBs; uMBCol++, uMB++)
        {
            core_enc->m_pMBOffsets[uMB].uLumaOffset[0][0] = uLumaOffsetFRM;
            core_enc->m_pMBOffsets[uMB].uChromaOffset[0][0] = uChromaOffsetFRM;
            core_enc->m_pMBOffsets[uMB].uLumaOffset[0][1] = uLumaOffsetFLD;
            core_enc->m_pMBOffsets[uMB].uChromaOffset[0][1] = uChromaOffsetFLD;

            core_enc->m_pMBOffsets[uMB].uLumaOffset[1][0] = uLumaOffsetAFRMFRM;
            core_enc->m_pMBOffsets[uMB].uChromaOffset[1][0] = uChromaOffsetAFRMFRM;
            core_enc->m_pMBOffsets[uMB].uLumaOffset[1][1] = uLumaOffsetAFRMFLD;
            core_enc->m_pMBOffsets[uMB].uChromaOffset[1][1] = uChromaOffsetAFRMFLD;

            uLumaOffsetFRM   += 16;
            uChromaOffsetFRM += chroma_width;
            uLumaOffsetFLD   += 16;
            uChromaOffsetFLD += chroma_width;
            if ((uMB&1)==0)
            {
                uLumaOffsetAFRMFRM   += core_enc->m_Pitch*16;
                uChromaOffsetAFRMFRM += core_enc->m_Pitch*chroma_height; //TODO???
                uLumaOffsetAFRMFLD   += core_enc->m_Pitch;
                uChromaOffsetAFRMFLD += core_enc->m_Pitch;
            }
            else
            {
                uLumaOffsetAFRMFRM   += 16 - core_enc->m_Pitch*16;
                uChromaOffsetAFRMFRM += chroma_width - core_enc->m_Pitch*chroma_height;  //TODO???
                uLumaOffsetAFRMFLD   += 16 - core_enc->m_Pitch;
                uChromaOffsetAFRMFLD += chroma_width - core_enc->m_Pitch;   //TODO ???

            }
        }

        uLumaOffsetFRM += (core_enc->m_Pitch*16) - core_enc->m_WidthInMBs*16;
        uChromaOffsetFRM += (core_enc->m_Pitch*chroma_height) - core_enc->m_WidthInMBs*chroma_width;
        uLumaOffsetFLD += (core_enc->m_Pitch*32) - core_enc->m_WidthInMBs*16;
        uChromaOffsetFLD += (core_enc->m_Pitch*(chroma_height<<1)) - core_enc->m_WidthInMBs*chroma_width;
        if (uMBRow == core_enc->m_HeightInMBs/2-1)
        {
            uLumaOffsetFLD = core_enc->m_pCurrentFrame->m_bottom_field_flag[1]*core_enc->m_Pitch;
            uChromaOffsetFLD = core_enc->m_pCurrentFrame->m_bottom_field_flag[1]*core_enc->m_Pitch;
        }
        if (uMBRow&1)
        {
            uLumaOffsetAFRMFRM   += core_enc->m_Pitch*32 - core_enc->m_WidthInMBs*16;
            uChromaOffsetAFRMFRM += core_enc->m_Pitch*16 - core_enc->m_WidthInMBs*8;  //TODO???
            uLumaOffsetAFRMFLD   += core_enc->m_Pitch*32 - core_enc->m_WidthInMBs*16;
            uChromaOffsetAFRMFLD += core_enc->m_Pitch*16 - core_enc->m_WidthInMBs*8;  //TODO???
        }
    }
    T_EncodeMBOffsets *pMBOffsets = core_enc->m_pMBOffsets;
#define BORDER_SIZE 15
    if (core_enc->m_pCurrentFrame->m_PictureStructureForDec == FRM_STRUCTURE) {
        Ipp32s row, col; //, height_in_mbs = core_enc->m_HeightInMBs >> (core_enc->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE ? 1 : 0);
    //    for (row = core_enc->m_field_index * height_in_mbs; row < ((core_enc->m_field_index + 1) * height_in_mbs) << 4; row += 16) {
        for (row = 0; row < core_enc->m_HeightInMBs << 4; row += 16) {
            for (col = 0; col < core_enc->m_WidthInMBs << 4; col += 16) {
                pMBOffsets->uMVLimits_L = -MIN(MAX_MV_INT, col + BORDER_SIZE);
                pMBOffsets->uMVLimits_R =  MIN(MAX_MV_INT, ((core_enc->m_WidthInMBs - 1) << 4) - col + BORDER_SIZE);
                pMBOffsets->uMVLimits_U = -MIN(MAX_MV_INT,  row + BORDER_SIZE);
               // pMBOffsets->uMVLimits_D =  MIN(MAX_MV_INT, ((height_in_mbs - 1) << 4) - row + BORDER_SIZE);
                pMBOffsets->uMVLimits_D =  MIN(MAX_MV_INT, ((core_enc->m_HeightInMBs - 1) << 4) - row + BORDER_SIZE);
                pMBOffsets ++;
            }
        }
    } else {
        Ipp32s row, col;
        for (row = 0; row < core_enc->m_HeightInMBs << 3; row += 16) {
            for (col = 0; col < core_enc->m_WidthInMBs << 4; col += 16) {
                pMBOffsets->uMVLimits_L = -MIN(MAX_MV_INT, col + BORDER_SIZE);
                pMBOffsets->uMVLimits_R =  MIN(MAX_MV_INT, ((core_enc->m_WidthInMBs - 1) << 4) - col + BORDER_SIZE);
                pMBOffsets->uMVLimits_U = -MIN(MAX_MV_INT,  row + BORDER_SIZE);
                pMBOffsets->uMVLimits_D =  MIN(MAX_MV_INT, (((core_enc->m_HeightInMBs >> 1) - 1) << 4) - row + BORDER_SIZE);
                pMBOffsets ++;
            }
        }
        for (row = 0; row < core_enc->m_HeightInMBs << 3; row += 16) {
            for (col = 0; col < core_enc->m_WidthInMBs << 4; col += 16) {
                pMBOffsets->uMVLimits_L = -MIN(MAX_MV_INT, col + BORDER_SIZE);
                pMBOffsets->uMVLimits_R =  MIN(MAX_MV_INT, ((core_enc->m_WidthInMBs - 1) << 4) - col + BORDER_SIZE);
                pMBOffsets->uMVLimits_U = -MIN(MAX_MV_INT,  row + BORDER_SIZE);
                pMBOffsets->uMVLimits_D =  MIN(MAX_MV_INT, (((core_enc->m_HeightInMBs >> 1) - 1) << 4) - row + BORDER_SIZE);
                pMBOffsets ++;
            }
        }
    }
}


//#include "vm_time.h"
//extern char frame_types[10000];
//extern int frame_count;

///////////////////////////////////////////////////////////////////////
//
//  Make_MBSlices - "Slices" the frame by writing a Slice
//  number into the uSlice member of the per MB data structure.
//  Right now, this method implements a simple scheme where a number
//  of fixed length Slices are present in a single JVT Slice Group,
//  and the last slice contains any "remainder" MBs (and thus may be larger).
//
//  Other slicing schemes can be implemented by parameterizing this
//  method to produce different values of uSlice for each MB.
//  If doing something other than simple raster ordered slices, then
//  don't forget to change the Picture Header to properly encode the
//  chosen slicing method as appropriate "Slice Groups".
//  Right now, only a single Slice Group is used, so there is no
//  mb_allocation_map_type coded.
//
///////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Make_MBSlices(void* state)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s mb = 0;
    Ipp16u uSlice = 0;
    Ipp32u uSliceMBCnt = 0;

    Ipp32s mb_nums = (core_enc->m_HeightInMBs*core_enc->m_WidthInMBs) >> (Ipp8u)(core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE);

#ifdef SLICE_THREADING_LOAD_BALANCING
    if (core_enc->m_pCurrentFrame->m_PicCodType == INTRAPIC ||
        (!core_enc->m_B_ticks_data_available && core_enc->m_pCurrentFrame->m_PicCodType == BPREDPIC) ||
        (!core_enc->m_P_ticks_data_available && core_enc->m_pCurrentFrame->m_PicCodType == PREDPIC))
    {
#endif // SLICE_THREADING_LOAD_BALANCING
    for (Ipp32s i = 0; i < (core_enc->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE) + 1; i++)
    {
        Ipp32s num_slices = core_enc->uNumSlices << i*(core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE);

        for (mb = i*mb_nums; mb < mb_nums*(i+1); mb++)
        {
            core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb].slice_id = uSlice;

            uSliceMBCnt++;
            if ((uSliceMBCnt == core_enc->m_slice_length) &&
                (uSlice < num_slices - 1)) {
                uSlice++;
                uSliceMBCnt = 0;
            }
        }

        uSlice++;
        uSliceMBCnt = 0;
    }
#ifdef SLICE_THREADING_LOAD_BALANCING
    }
    else
    {   // Almost the same loop as the above except for the condition to switch to the next slice
        Ipp64s total;

        Ipp64s *ticks;
        if (core_enc->m_pCurrentFrame->m_PicCodType == PREDPIC)
        {
            ticks = core_enc->m_P_ticks_per_macroblock;
            total = core_enc->m_P_ticks_per_frame;
        }
        else // BPREDPIC
        {
            ticks = core_enc->m_B_ticks_per_macroblock;
            total = core_enc->m_B_ticks_per_frame;
        }

        for (Ipp32s i = 0; i < (core_enc->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE) + 1; i++)
        {
            Ipp32s num_slices = core_enc->uNumSlices << i*(core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE);
            Ipp64s average_frame_timing = total / num_slices;
            Ipp64s slice_timing = 0;

            for (mb = i*mb_nums; mb < mb_nums*(i+1); mb++)
            {
                core_enc->m_pCurrentFrame->m_mbinfo.mbs[mb].slice_id = uSlice;

                slice_timing += ticks[mb];
                uSliceMBCnt++;
                if ((slice_timing > average_frame_timing) &&
                    (uSlice < num_slices - 1)) {
                        uSlice++;
                        slice_timing = 0;
                        uSliceMBCnt = 0;
                }
            }

            uSlice++;
            uSliceMBCnt = 0;
        }
    }
#endif // SLICE_THREADING_LOAD_BALANCING
}

template<typename COEFFSTYPE>
void H264CoreEncoder_MakeSignificantLists_CABAC(
    COEFFSTYPE* coeff,
    const Ipp32s* dec_single_scan,
    T_Block_CABAC_Data<COEFFSTYPE>* c_data)
{
    /* This should be set outside
        c_data->uLastSignificant - last coeffs in decode scan
        c_data->uNumSigCoeffs - number of non-zero coeffs
        c_data->CtxBlockCat - ctxBlockCat - block context
        c_data->uLastCoeff - last possible coeff
        c_data->uFirstCoeff - first possible coeff
    */

    Ipp8u j=0;
    Ipp32s i;

    for (i=c_data->uFirstCoeff;i<=c_data->uLastSignificant;i++){
        Ipp32s coef=coeff[dec_single_scan[i]];
        if (coef)
        {
            Ipp32s sign = coef < 0;
            coef = (sign)? -coef : coef;
            c_data->uSignificantMap[j] = (Ipp8u)i;
            c_data->uSignificantLevels[j] = coef - 1;
            c_data->uSignificantSigns[j] = (Ipp8u)sign;
            j++;
        }
    }
    c_data->uFirstSignificant = c_data->uSignificantMap[0];
}

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264CoreEncoder_Start_Picture(void* state, const EnumPicClass* pic_class, EnumPicCodType pic_type)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Status ps = UMC_OK;

    core_enc->m_PicClass = *pic_class;
    core_enc->use_implicit_weighted_bipred = (core_enc->m_PicParamSet.weighted_bipred_idc == 2);
    if (!core_enc->m_is_mb_data_initialized)
    {
        Ipp32s uNumMBs = core_enc->m_HeightInMBs*core_enc->m_WidthInMBs>>(Ipp8u)(core_enc->m_pCurrentFrame->m_PictureStructureForDec<FRM_STRUCTURE);
        core_enc->uNumSlices = core_enc->m_params.num_slices;
        core_enc->m_slice_length = uNumMBs / core_enc->uNumSlices;
        core_enc->m_uSliceRemainder = uNumMBs % core_enc->uNumSlices;

        if (core_enc->m_SliceHeader.MbaffFrameFlag && (core_enc->m_slice_length&1))
        {
            core_enc->m_slice_length--;
            core_enc->m_uSliceRemainder += core_enc->uNumSlices;
        }

        H264CoreEncoder_InitializeMBData<COEFFSTYPE, PIXTYPE>(state);
        core_enc->m_is_mb_data_initialized = 1;
    }

    if (core_enc->m_field_index == 0)
        H264CoreEncoder_Make_MBSlices<COEFFSTYPE, PIXTYPE>(state);

    if (core_enc->m_pCurrentFrame->m_PictureStructureForDec < FRM_STRUCTURE)
        core_enc->m_HeightInMBs/=2;

    if (core_enc->m_params.rate_controls.method == H264_RCM_VBR || core_enc->m_params.rate_controls.method == H264_RCM_CBR)
    {
#ifdef FRAME_QP_FROM_FILE
        Ipp32s slice_qp = core_enc->m_pCurrentFrame->frame_qp;
#else
        Ipp32s slice_qp = H264_AVBR_GetQP(&core_enc->avbr, pic_type);
#endif
        core_enc->m_Slices[0].m_slice_qp_delta = (Ipp8s)(slice_qp - core_enc->m_PicParamSet.pic_init_qp);
    }
    else if(core_enc->m_params.rate_controls.method == H264_RCM_QUANT)
    {
        switch (pic_type)
        {
            case PREDPIC:
                core_enc->m_Slices[0].m_slice_qp_delta = core_enc->m_params.rate_controls.quantP - core_enc->m_PicParamSet.pic_init_qp;
                break;
            case BPREDPIC:
                core_enc->m_Slices[0].m_slice_qp_delta = core_enc->m_params.rate_controls.quantB - core_enc->m_PicParamSet.pic_init_qp;
                break;
            case INTRAPIC:
                core_enc->m_Slices[0].m_slice_qp_delta = core_enc->m_params.rate_controls.quantI - core_enc->m_PicParamSet.pic_init_qp;
                break;
        }
    }

    if(core_enc->m_pCurrentFrame->m_isAuxiliary)
    {
        Ipp32s slice_qp = core_enc->m_Slices[0].m_slice_qp_delta + core_enc->m_PicParamSet.pic_init_qp;
        slice_qp = convertLumaQP(slice_qp, core_enc->m_SeqParamSet.bit_depth_luma, core_enc->m_PicParamSet.bit_depth_luma);
        core_enc->m_Slices[0].m_slice_qp_delta = slice_qp - core_enc->m_PicParamSet.pic_init_qp;
    }
    for(Ipp32s sliceId = 0; sliceId < core_enc->m_params.num_slices*((core_enc->m_params.coding_type == 1) + 1); sliceId++)
    {
        core_enc->m_Slices[sliceId].m_slice_qp_delta = core_enc->m_Slices[0].m_slice_qp_delta;
        core_enc->m_Slices[sliceId].m_iLastXmittedQP = core_enc->m_PicParamSet.pic_init_qp + core_enc->m_Slices[sliceId].m_slice_qp_delta;

        // reset intra MB counter
        core_enc->m_Slices[sliceId].m_Intra_MB_Counter = 0;
        core_enc->m_Slices[sliceId].m_MB_Counter = 0;
        core_enc->m_Slices[sliceId].m_prev_dquant = 0;
    }

    Ipp32s slice_qp = core_enc->m_PicParamSet.pic_init_qp + core_enc->m_Slices[0].m_slice_qp_delta;
    for (Ipp32s uMB = core_enc->m_field_index*core_enc->m_WidthInMBs * core_enc->m_HeightInMBs; uMB < (core_enc->m_field_index+1)* core_enc->m_WidthInMBs * core_enc->m_HeightInMBs; uMB++)
    {
        // initialize QP settings and MB type
        core_enc->m_mbinfo.mbs[uMB].QP = slice_qp;
    }

    return ps;
}

template<typename COEFFSTYPE>
void H264CoreEncoder_ScanSignificant_CABAC(
    COEFFSTYPE coeff[],
    Ipp32s ctxBlockCat,
    Ipp32s numcoeff,
    const Ipp32s* dec_single_scan,
    T_Block_CABAC_Data<COEFFSTYPE>* c_data)
{
    Ipp8u start_scan, end_scan, j;
    Ipp32s i;

    switch(numcoeff)
    {
        case 64:start_scan = 0;end_scan=63;break;
        case 16:start_scan = 0;end_scan=15;break;
        case 15:start_scan = 1;end_scan=15;break;
        case  8:start_scan = 0;end_scan= 7;break;
        case  4:start_scan = 0;end_scan= 3;break;
        default:
            VM_ASSERT(false);
            start_scan = 0;
            end_scan = 0;
            break;
    }

    c_data->uLastCoeff = end_scan; //dec_single_scan[end_scan]; By Burakov seems to be an error.
    c_data->uFirstCoeff = start_scan;//dec_single_scan[start_scan]; By Burakov seems to be an error.
    for (i=end_scan;i>=start_scan;i--)
    {
        Ipp32s coef=coeff[dec_single_scan[i]];
        if (coef)
        {
            end_scan = (Ipp8u)i;
            break;
        }
    }
    c_data->uLastSignificant = end_scan;
    for (i=start_scan;i<=end_scan;i++)
    {
        Ipp32s coef=coeff[dec_single_scan[i]];
        if (coef)
        {
            start_scan = (Ipp8u)i;
            break;
        }
    }
    c_data->uFirstSignificant = start_scan;

    for (i=c_data->uFirstSignificant,j=0;i<=c_data->uLastSignificant;i++)
    {
        Ipp32s coef=coeff[dec_single_scan[i]];
        if (coef)
        {
            Ipp32s sign = coef < 0;
            coef = (sign)? -coef : coef;
            c_data->uSignificantMap[j] = (Ipp8u)i;
            c_data->uSignificantLevels[j] = coef - 1;
            c_data->uSignificantSigns[j] = (Ipp8u)sign;
            j++;
        }
    }

    c_data->uNumSigCoeffs = j;
    c_data->CtxBlockCat = ctxBlockCat;
}

#if 0
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_InferFDFForSkippedMBs(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder* core_enc = (H264CoreEncoder *)state;
    H264CurrentMacroblockDescriptor &cur_mb = curr_slice->m_cur_mb;

    //inherit field decoding flag
    if (cur_mb.MacroblockNeighbours.mb_A>=0)
    {
        pSetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo,GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.MacroblockNeighbours.mb_A]));
    }
    else if (cur_mb.MacroblockNeighbours.mb_B>=0)
    {
        pSetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo,GetMBFieldDecodingFlag(core_enc->m_pCurrentFrame->m_mbinfo.mbs[cur_mb.MacroblockNeighbours.mb_B]));
    }
    else
    {
        pSetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo,0);
    }
    pSetMBFieldDecodingFlag(cur_mb.GlobalMacroblockPairInfo,pGetMBFieldDecodingFlag(cur_mb.GlobalMacroblockInfo));
}
#endif

// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template        void                H264CoreEncoder_Destroy<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_Init<COEFFSTYPE, PIXTYPE>(void*, BaseCodecParams*, MemoryAllocator*);
template        Status              H264CoreEncoder_Close<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_Create<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_GetFrame<COEFFSTYPE, PIXTYPE>(void*, MediaData*, MediaData*);
template        Status              H264CoreEncoder_GetInfo<COEFFSTYPE, PIXTYPE>(void*, BaseCodecParams*);
template const  H264PicParamSet*    H264CoreEncoder_GetPicParamSet<COEFFSTYPE, PIXTYPE>(void*);
template const  H264SeqParamSet*    H264CoreEncoder_GetSeqParamSet<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_SetParams<COEFFSTYPE, PIXTYPE>(void*, BaseCodecParams*);
template        Status              H264CoreEncoder_Reset<COEFFSTYPE, PIXTYPE>(void*);
template        VideoData*          H264CoreEncoder_GetReconstructedFrame<COEFFSTYPE, PIXTYPE>(void*);
template        void                H264CoreEncoder_ScanSignificant_CABAC<COEFFSTYPE>(COEFFSTYPE[], Ipp32s, Ipp32s, const Ipp32s*, T_Block_CABAC_Data<COEFFSTYPE>*);
template        void                H264CoreEncoder_MakeSignificantLists_CABAC<COEFFSTYPE>(COEFFSTYPE*, const Ipp32s*, T_Block_CABAC_Data<COEFFSTYPE>*);
template        void                H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaMBAFF<COEFFSTYPE, PIXTYPE>(void*, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_SetSliceHeaderCommon<COEFFSTYPE, PIXTYPE>(void*, H264EncoderFrame<PIXTYPE>*);
template        void                H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopRightLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopRightLocationForCurrentMBLumaMBAFF<COEFFSTYPE, PIXTYPE>(void*, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF<COEFFSTYPE, PIXTYPE>(void*, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*, bool);
template        Status              H264CoreEncoder_Start_Picture<COEFFSTYPE, PIXTYPE>(void*, const EnumPicClass*, EnumPicCodType);
template        void                H264CoreEncoder_Intra16x16SelectAndPredict<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u*, PIXTYPE*);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template        void                H264CoreEncoder_Destroy<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_Init<COEFFSTYPE, PIXTYPE>(void*, BaseCodecParams*, MemoryAllocator*);
template        Status              H264CoreEncoder_Close<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_Create<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_GetFrame<COEFFSTYPE, PIXTYPE>(void*, MediaData*, MediaData*);
template        Status              H264CoreEncoder_GetInfo<COEFFSTYPE, PIXTYPE>(void*, BaseCodecParams*);
template const  H264PicParamSet*    H264CoreEncoder_GetPicParamSet<COEFFSTYPE, PIXTYPE>(void*);
template const  H264SeqParamSet*    H264CoreEncoder_GetSeqParamSet<COEFFSTYPE, PIXTYPE>(void*);
template        Status              H264CoreEncoder_SetParams<COEFFSTYPE, PIXTYPE>(void*, BaseCodecParams*);
template        Status              H264CoreEncoder_Reset<COEFFSTYPE, PIXTYPE>(void*);
template        VideoData*          H264CoreEncoder_GetReconstructedFrame<COEFFSTYPE, PIXTYPE>(void*);
template        void                H264CoreEncoder_ScanSignificant_CABAC<COEFFSTYPE>(COEFFSTYPE[], Ipp32s, Ipp32s, const Ipp32s*, T_Block_CABAC_Data<COEFFSTYPE>*);
template        void                H264CoreEncoder_MakeSignificantLists_CABAC<COEFFSTYPE>(COEFFSTYPE*, const Ipp32s*, T_Block_CABAC_Data<COEFFSTYPE>*);
template        void                H264CoreEncoder_GetLeftLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaMBAFF<COEFFSTYPE, PIXTYPE>(void*, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_SetSliceHeaderCommon<COEFFSTYPE, PIXTYPE>(void*, H264EncoderFrame<PIXTYPE>*);
template        void                H264CoreEncoder_GetTopLeftLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopRightLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopLocationForCurrentMBLumaNonMBAFF<COEFFSTYPE, PIXTYPE>(H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopRightLocationForCurrentMBLumaMBAFF<COEFFSTYPE, PIXTYPE>(void*, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*);
template        void                H264CoreEncoder_GetTopLocationForCurrentMBLumaMBAFF<COEFFSTYPE, PIXTYPE>(void*, H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE>&, H264BlockLocation*, bool);
template        Status              H264CoreEncoder_Start_Picture<COEFFSTYPE, PIXTYPE>(void*, const EnumPicClass*, EnumPicCodType);
template        void                H264CoreEncoder_Intra16x16SelectAndPredict<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32u*, PIXTYPE*);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER
