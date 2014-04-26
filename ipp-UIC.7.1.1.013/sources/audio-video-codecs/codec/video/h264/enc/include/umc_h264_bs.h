//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef UMC_H264_BS_H__
#define UMC_H264_BS_H__

#include "umc_h264_defs.h"
#include "umc_base_bitstream.h"
#include "umc_h264_core_enc.h"


template<typename COEFFSTYPE, typename PIXTYPE>
struct H264Slice;

void H264BsReal_Reset(
    void* state);

void H264BsReal_ResetRBSP(
    void* state);

Ipp32u H264BsReal_EndOfNAL(
    void* state,
    Ipp8u* const pout,
    Ipp8u const uIDC,
    NAL_Unit_Type const uUnitType,
    bool& startPicture);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264BsReal_PutSliceHeader(
    void* state,
    const H264SliceHeader& slice_hdr,
    const H264PicParamSet& pic_parms,
    const H264SeqParamSet& seq_parms,
    const EnumPicClass& ePictureClass,
    const H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

Status H264BsReal_PutSeqParms(
    void* state,
    const H264SeqParamSet& seq_parms);

Status H264BsReal_PutSeqExParms(
    void* state,
    const H264SeqParamSet& seq_parms);

Status H264BsReal_PutPicParms(
    void* state,
    const H264PicParamSet& pic_parms,
    const H264SeqParamSet& seq_parms);

Status H264BsReal_PutPicDelimiter(
    void* state,
    EnumPicCodType PicCodType);

void H264BsReal_PutDQUANT(
    void* state,
    const Ipp32u quant,
    const Ipp32u quant_prev);

Status H264BsReal_PutNumCoeffAndTrailingOnes(
    void* state,
    Ipp32u uVLCSelect,
    Ipp32s bChromaDC,
    Ipp32u uNumCoeff,
    Ipp32u uNumTrailingOnes,
    Ipp32u TrOneSigns);

template <typename COEFFSTYPE>
Status H264BsReal_PutLevels(
    void* state,
    COEFFSTYPE* iLevels,
    Ipp32s NumLevels,
    Ipp32s TrailingOnes);

Status H264BsReal_PutTotalZeros(
    void* state,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs,
    Ipp32s bChromaDC);

Status H264BsReal_PutRuns(
    void* state,
    Ipp8u* uRuns,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs);

Status H264BsReal_MBFieldModeInfo_CABAC(
    void* state,
    Ipp32s mb_field,
    Ipp32s field_available_left,
    Ipp32s field_available_above);

Status H264BsReal_MBTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s mb_type_cur,
    MB_Type type_cur,
    MB_Type type_left,
    MB_Type type_above);

Status H264BsReal_SubTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s type);

Status H264BsReal_ChromaIntraPredMode_CABAC(
    void* state,
    Ipp32s mode,
    Ipp32s left_p,
    Ipp32s top_p);

Status H264BsReal_IntraPredMode_CABAC(
    void* state,
    Ipp32s mode);

Status H264BsReal_MVD_CABAC(
    void* state,
    Ipp32s vector,
    Ipp32s left_p,
    Ipp32s top_p,
    Ipp32s contextbase);

Status H264BsReal_DQuant_CABAC(
    void* state,
    Ipp32s deltaQP,
    Ipp32s left_c);

template<typename COEFFSTYPE>
Status H264BsReal_ResidualBlock_CABAC(
    void* state,
    T_Block_CABAC_Data<COEFFSTYPE> *c_data,
    bool frame_block);

Status H264BsReal_PutScalingList(
    void* state,
    const Ipp8u* scalingList,
    Ipp32s sizeOfScalingList,
    bool& useDefaultScalingMatrixFlag);

Status H264BsReal_PutSEI_UserDataUnregistred(
    void* state,
    void* data,
    Ipp32s data_size );

Status H264BsReal_PutSEI_BufferingPeriod(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u NalHrdBpPresentFlag,
    const Ipp8u VclHrdBpPresentFlag,
    Ipp32u * nal_initial_cpb_removal_delay,
    Ipp32u * nal_initial_cpb_removal_delay_offset,
    Ipp32u * vcl_initial_cpb_removal_delay,
    Ipp32u * vcl_initial_cpb_removal_delay_offset);

Status H264BsReal_PutSEI_PictureTiming(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u CpbDpbDelaysPresentFlag,
    const Ipp8u pic_struct_present_flag,
    Ipp32u cpb_removal_delay,
    Ipp32u dpb_output_delay,
    const PicStructTimingData& timing_data);

Status H264BsReal_PutSEI_RecoveryPoint(
    void* state,
    Ipp32s recovery_frame_cnt,
    Ipp8u exact_match_flag,
    Ipp8u broken_link_flag,
    Ipp8u changing_slice_group_idc);


////////////////////////////////////

inline
void H264BsFake_Reset(void* state)
{
    H264BsFake* bs = (H264BsFake *)state;
    bs->m_base.m_bitOffset = 0;
}

void H264BsFake_ResetRBSP(
    void* state);

Ipp32u H264BsFake_EndOfNAL(
    void* state,
    Ipp8u* const pout,
    Ipp8u const uIDC,
    NAL_Unit_Type const uUnitType,
    bool& startPicture);

template<typename COEFFSTYPE, typename PIXTYPE>
Status H264BsFake_PutSliceHeader(
    void* state,
    const H264SliceHeader& slice_hdr,
    const H264PicParamSet& pic_parms,
    const H264SeqParamSet& seq_parms,
    const EnumPicClass& ePictureClass,
    const H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice);

Status H264BsFake_PutSeqParms(
    void* state,
    const H264SeqParamSet& seq_parms);

Status H264BsFake_PutSeqExParms(
    void* state,
    const H264SeqParamSet& seq_parms);

Status H264BsFake_PutPicParms(
    void* state,
    const H264PicParamSet& pic_parms,
    const H264SeqParamSet& seq_parms);

Status H264BsFake_PutPicDelimiter(
    void* state,
    EnumPicCodType PicCodType);

void H264BsFake_PutDQUANT(
    void* state,
    const Ipp32u quant,
    const Ipp32u quant_prev);

Status H264BsFake_PutNumCoeffAndTrailingOnes(
    void* state,
    Ipp32u uVLCSelect,
    Ipp32s bChromaDC,
    Ipp32u uNumCoeff,
    Ipp32u uNumTrailingOnes,
    Ipp32u TrOneSigns);

template <typename COEFFSTYPE>
Status H264BsFake_PutLevels(
    void* state,
    COEFFSTYPE* iLevels,
    Ipp32s NumLevels,
    Ipp32s TrailingOnes);

Status H264BsFake_PutTotalZeros(
    void* state,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs,
    Ipp32s bChromaDC);

Status H264BsFake_PutRuns(
    void* state,
    Ipp8u* uRuns,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs);

Status H264BsFake_MBFieldModeInfo_CABAC(
    void* state,
    Ipp32s mb_field,
    Ipp32s field_available_left,
    Ipp32s field_available_above);

Status H264BsFake_MBTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s mb_type_cur,
    MB_Type type_cur,
    MB_Type type_left,
    MB_Type type_above);

Status H264BsFake_SubTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s type);

Status H264BsFake_ChromaIntraPredMode_CABAC(
    void* state,
    Ipp32s mode,
    Ipp32s left_p,
    Ipp32s top_p);

Status H264BsFake_IntraPredMode_CABAC(
    void* state,
    Ipp32s mode);

Status H264BsFake_MVD_CABAC(
    void* state,
    Ipp32s vector,
    Ipp32s left_p,
    Ipp32s top_p,
    Ipp32s contextbase);

Status H264BsFake_DQuant_CABAC(
    void* state,
    Ipp32s deltaQP,
    Ipp32s left_c);

template<typename COEFFSTYPE>
Status H264BsFake_ResidualBlock_CABAC(
    void* state,
    T_Block_CABAC_Data<COEFFSTYPE> *c_data,
    bool frame_block);

Status H264BsFake_PutScalingList(
    void* state,
    const Ipp8u* scalingList,
    Ipp32s sizeOfScalingList,
    bool& useDefaultScalingMatrixFlag);

Status H264BsFake_PutSEI_UserDataUnregistred(
    void* state,
    void* data,
    Ipp32s data_size );

Status H264BsFake_PutSEI_BufferingPeriod(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u NalHrdBpPresentFlag,
    const Ipp8u VclHrdBpPresentFlag,
    Ipp32u * nal_initial_cpb_removal_delay,
    Ipp32u * nal_initial_cpb_removal_delay_offset,
    Ipp32u * vcl_initial_cpb_removal_delay,
    Ipp32u * vcl_initial_cpb_removal_delay_offset);

Status H264BsFake_PutSEI_PictureTiming(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u CpbDpbDelaysPresentFlag,
    const Ipp8u pic_struct_present_flag,
    Ipp32u cpb_removal_delay,
    Ipp32u dpb_output_delay,
    const PicStructTimingData& timing_data);

Status H264BsFake_PutSEI_RecoveryPoint(
    void* state,
    Ipp32s recovery_frame_cnt,
    Ipp8u exact_match_flag,
    Ipp8u broken_link_flag,
    Ipp8u changing_slice_group_idc);

#endif // UMC_H264_BS_H__
