//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#ifndef __UMC_H264_VIDEO_ENCODER_H__
#define __UMC_H264_VIDEO_ENCODER_H__

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("H.264 encoder"))

#include "umc_video_data.h"
#include "umc_video_encoder.h"
#include "umc_h264_config.h"

#include "ippdefs.h"

enum EncoderType
{
    H264_VIDEO_ENCODER_8U_16S  = 0,
    H264_VIDEO_ENCODER_16U_32S = 1,
    H264_VIDEO_ENCODER_NONE    = 2
};

namespace UMC
{
// Slice Group definitions
#define MAX_NUM_SLICE_GROUPS 8
#define MAX_SLICE_GROUP_MAP_TYPE 6
#define MAX_CPB_CNT 32

typedef enum
{
    H264_PROFILE_CAVLC444           = 44,
    H264_PROFILE_BASELINE           = 66,
    H264_PROFILE_MAIN               = 77,
    H264_PROFILE_SCALABLE_BASELINE  = 83, // Annex G
    H264_PROFILE_SCALABLE_HIGH      = 86, // Annex G
    H264_PROFILE_EXTENDED           = 88,
    H264_PROFILE_HIGH               = 100,
    H264_PROFILE_HIGH10             = 110,
    H264_PROFILE_MULTIVIEW_HIGH     = 118, // Annex H
    H264_PROFILE_HIGH422            = 122,
    H264_PROFILE_STEREO_HIGH        = 128, // Annex H
    H264_PROFILE_HIGH444            = 244
} H264_PROFILE_IDC;

typedef enum
{
    H264_RCM_QUANT     = 0, // Fix quantizer values, no actual rate control.
    H264_RCM_CBR       = 1,
    H264_RCM_VBR       = 2,
    H264_RCM_CBR_SLICE = 3,
    H264_RCM_VBR_SLICE = 4
} H264_RATE_CONTROL_METHOD;

typedef struct
{
    Ipp8u method;
    Ipp8s quantI;
    Ipp8s quantP;
    Ipp8s quantB;
} H264_Rate_Controls;

struct SliceGroupInfoStruct
{
    Ipp8u slice_group_map_type;               // 0..6
    // The additional slice group data depends upon map type
    union {
        // type 0
        Ipp32u run_length[MAX_NUM_SLICE_GROUPS];
        // type 2
        struct {
            Ipp32u top_left[MAX_NUM_SLICE_GROUPS-1];
            Ipp32u bottom_right[MAX_NUM_SLICE_GROUPS-1];
        } t1;
        // types 3-5
        struct {
            Ipp8u  slice_group_change_direction_flag;
            Ipp32u slice_group_change_rate;
        } t2;
        // type 6
        struct {
            Ipp32u pic_size_in_map_units;       // number of macroblocks if no field coding
            Ipp8u *pSliceGroupIDMap;            // Id for each slice group map unit
        } t3;
    };
};  // SliceGroupInfoStruct

struct  PicStructTimingData{
    Ipp8u pic_struct;
    Ipp8u clock_timestamp_flag[3];
    Ipp8u ct_type[3];
    Ipp8u nuit_field_based_flag[3];
    Ipp8u counting_type[3];
    Ipp8u full_timestamp_flag[3];
    Ipp8u discontinuity_flag[3];
    Ipp8u cnt_dropped_flag[3];
    Ipp8u n_frames[3];
    Ipp8u seconds_value[3];
    Ipp8u minutes_value[3];
    Ipp8u hours_value[3];
    Ipp8u seconds_flag[3];
    Ipp8u minutes_flag[3];
    Ipp8u hours_flag[3];
    Ipp32s time_offset[3];
};

struct H264VUI_HRDParams {
    Ipp8u cpb_cnt_minus1;
    Ipp8u bit_rate_scale;
    Ipp8u cpb_size_scale;

    Ipp32u bit_rate_value_minus1[MAX_CPB_CNT];
    Ipp32u cpb_size_value_minus1[MAX_CPB_CNT];
    Ipp8u cbr_flag[MAX_CPB_CNT];

    Ipp8u initial_cpb_removal_delay_length_minus1;
    Ipp8u cpb_removal_delay_length_minus1;
    Ipp8u dpb_output_delay_length_minus1;
    Ipp8u time_offset_length;
};

struct H264VUIParams {
    Ipp8u   aspect_ratio_info_present_flag;
    Ipp8u   aspect_ratio_idc;
    Ipp16u   sar_width;
    Ipp16u   sar_height;

    Ipp8u   overscan_info_present_flag;
    Ipp8u   overscan_appropriate_flag;

    Ipp8u  video_signal_type_present_flag;
    Ipp8u  video_format;
    Ipp8u  video_full_range_flag;
    Ipp8u  colour_description_present_flag;
    Ipp8u  colour_primaries;
    Ipp8u  transfer_characteristics;
    Ipp8u  matrix_coefficients;

    Ipp8u  chroma_loc_info_present_flag;
    Ipp8u  chroma_sample_loc_type_top_field;
    Ipp8u  chroma_sample_loc_type_bottom_field;

    Ipp8u  timing_info_present_flag;
    Ipp32u num_units_in_tick;
    Ipp32u time_scale;
    Ipp8u  fixed_frame_rate_flag;

    Ipp8u  nal_hrd_parameters_present_flag;
    H264VUI_HRDParams hrd_params;
    Ipp8u  vcl_hrd_parameters_present_flag;
    H264VUI_HRDParams vcl_hrd_params;
    Ipp8u  low_delay_hrd_flag;

    Ipp8u  pic_struct_present_flag;
    Ipp8u  bitstream_restriction_flag;
    Ipp8u  motion_vectors_over_pic_boundaries_flag;
    Ipp32u  max_bytes_per_pic_denom;
    Ipp32u  max_bits_per_mb_denom;
    Ipp8u  log2_max_mv_length_horizontal;
    Ipp8u  log2_max_mv_length_vertical;
    Ipp8u  num_reorder_frames;
    Ipp16u  max_dec_frame_buffering;
};

// Sequence parameter set structure, corresponding to the H.264 bitstream definition.
struct H264SeqParamSet
{
    Ipp16u      profile_idc;                   // baseline, main, etc.
    Ipp8u       level_idc;
    bool        constraint_set0_flag;               // nonzero: bitstream obeys all set 0 constraints
    bool        constraint_set1_flag;               // nonzero: bitstream obeys all set 1 constraints
    bool        constraint_set2_flag;               // nonzero: bitstream obeys all set 2 constraints
    bool        constraint_set3_flag;               // nonzero: bitstream obeys all set 3 constraints
    bool        constraint_set4_flag;               // nonzero: bitstream obeys all set 4 constraints
    bool        constraint_set5_flag;               // nonzero: bitstream obeys all set 5 constraints
    Ipp8u       chroma_format_idc;

    Ipp8s       seq_parameter_set_id;               // id of this sequence parameter set
    Ipp8s       log2_max_frame_num;                 // Number of bits to hold the frame_num
    Ipp8s       pic_order_cnt_type;                 // Picture order counting method

    Ipp8s       delta_pic_order_always_zero_flag;   // If zero, delta_pic_order_cnt fields are
    // present in slice header.
    bool       frame_mbs_only_flag;                // Nonzero indicates all pictures in sequence
    // are coded as frames (not fields).
    Ipp8s       gaps_in_frame_num_value_allowed_flag;

    Ipp8s       mb_adaptive_frame_field_flag;       // Nonzero indicates frame/field switch
    // at macroblock level
    bool       direct_8x8_inference_flag;          // Direct motion vector derivation method
    bool       vui_parameters_present_flag;        // Zero indicates default VUI parameters
    H264VUIParams vui_parameters;                   // VUI parameters if it is going to be used
    bool       frame_cropping_flag;                // Nonzero indicates frame crop offsets are present.
    Ipp32s      frame_crop_left_offset;
    Ipp32s      frame_crop_right_offset;
    Ipp32s      frame_crop_top_offset;
    Ipp32s      frame_crop_bottom_offset;
    Ipp32s      log2_max_pic_order_cnt_lsb;         // Value of MaxPicOrderCntLsb.
    Ipp32s      offset_for_non_ref_pic;

    Ipp32s      offset_for_top_to_bottom_field;     // Expected pic order count difference from
    // top field to bottom field.

    Ipp32s      num_ref_frames_in_pic_order_cnt_cycle;
    Ipp32s      *poffset_for_ref_frame;             // pointer to array of stored frame offsets,
    // length num_stored_frames_in_pic_order_cnt_cycle,
    // for pic order cnt type 1
    Ipp32s      num_ref_frames;                     // total number of pics in decoded pic buffer
    Ipp32u      frame_width_in_mbs;
    Ipp32u      frame_height_in_mbs;

    // These fields are calculated from values above.  They are not written to the bitstream
    Ipp32s      MaxMbAddress;
    Ipp32s      MaxPicOrderCntLsb;
    Ipp32s      aux_format_idc;                     // See H.264 standard for details.
    Ipp32s      bit_depth_aux;
    Ipp32s      bit_depth_luma;
    Ipp32s      bit_depth_chroma;
    Ipp32s      alpha_incr_flag;
    Ipp32s      alpha_opaque_value;
    Ipp32s      alpha_transparent_value;

    bool        seq_scaling_matrix_present_flag;
    bool        seq_scaling_list_present_flag[8];
    Ipp8u       seq_scaling_list_4x4[6][16];
    Ipp8u       seq_scaling_list_8x8[2][64];

    Ipp16s      seq_scaling_matrix_4x4[6][6][16];
    Ipp16s      seq_scaling_matrix_8x8[2][6][64];
    Ipp16s      seq_scaling_inv_matrix_4x4[6][6][16];
    Ipp16s      seq_scaling_inv_matrix_8x8[2][6][64];

    bool        pack_sequence_extension;
    bool        qpprime_y_zero_transform_bypass_flag;
    bool        residual_colour_transform_flag;
    Ipp32s      additional_extension_flag;
};  // H264SeqParamSet

// Picture parameter set structure, corresponding to the H.264 bitstream definition.
struct H264PicParamSet
{
    Ipp8s       pic_parameter_set_id;           // of this picture parameter set
    Ipp8s       seq_parameter_set_id;           // of seq param set used for this pic param set
    Ipp8s       entropy_coding_mode;            // zero: CAVLC, else CABAC

    Ipp8s       pic_order_present_flag;         // Zero indicates only delta_pic_order_cnt[0] is
    // present in slice header; nonzero indicates
    // delta_pic_order_cnt[1] is also present.

    Ipp8s       weighted_pred_flag;             // Nonzero indicates weighted prediction applied to
    // P and SP slices
    Ipp8s       weighted_bipred_idc;            // 0: no weighted prediction in B slices
    // 1: explicit weighted prediction
    // 2: implicit weighted prediction
    Ipp8s       pic_init_qp;                    // default QP for I,P,B slices
    Ipp8s       pic_init_qs;                    // default QP for SP, SI slices

    Ipp8s       chroma_qp_index_offset;         // offset to add to QP for chroma

    Ipp8s       deblocking_filter_variables_present_flag; // If nonzero, deblock filter params are
    // present in the slice header.
    Ipp8s       constrained_intra_pred_flag;    // Nonzero indicates constrained intra mode

    Ipp8s       redundant_pic_cnt_present_flag; // Nonzero indicates presence of redundant_pic_cnt
    // in slice header
    Ipp8s       num_slice_groups;               // Usually 1

    Ipp8s       second_chroma_qp_index_offset;

    SliceGroupInfoStruct SliceGroupInfo;        // Used only when num_slice_groups > 1
    Ipp32s      num_ref_idx_l0_active;          // num of ref pics in list 0 used to decode the picture
    Ipp32s      num_ref_idx_l1_active;          // num of ref pics in list 1 used to decode the picture
    bool        transform_8x8_mode_flag;
    bool        pic_scaling_matrix_present_flag; // Only "false" is supported.
    bool        pack_sequence_extension;
    Ipp32s      chroma_format_idc;              // needed for aux/primary picture switch.
    Ipp32s      bit_depth_luma;                 // needed for aux/primary picture switch.
};  // H264PicParamSet

class H264EncoderParams: public VideoEncoderParams
{
public:
    DYNAMIC_CAST_DECL(H264EncoderParams, VideoEncoderParams)

    H264EncoderParams();
    virtual Status ReadParams(ParserCfg *par);

    H264_Rate_Controls rate_controls;

    Ipp16u  profile_idc; // profile_idc
    Ipp8u   level_idc;
    bool    constraint_set_flag[6];

    Ipp32u  key_interval;
    Ipp32u  idr_interval;
    Ipp32u  B_frame_rate;
    Ipp8u   B_reference_mode;
    Ipp32u  num_ref_frames;
    Ipp8u   num_ref_to_start_code_B_slice;
    bool    direct_8x8_inference_flag;
    Ipp8u   chroma_format_idc;
    Ipp8u   coding_type; // 0 - only FRM, 1 - only FLD , 2 - only AFRM, 3  - pure PicAFF(no MBAFF) 4 PicAFF + MBAFF
    Ipp16u  num_slices; // Number of slices
    Ipp32u  max_slice_size;
    Ipp8u   m_do_weak_forced_key_frames;
    Ipp8u   deblocking_filter_idc;
    Ipp8s   deblocking_filter_alpha;
    Ipp8s   deblocking_filter_beta;
    Ipp8u   mv_search_method;
    Ipp8u   me_split_mode; // 0 - 16x16 only; 1 - 16x16, 16x8, 8x16, 8x8; 2 - could split 8x8.
    Ipp8u   me_search_x;
    Ipp8u   me_search_y;
    Ipp8u   direct_pred_mode; //  0 - temporal, 1 - spatial, 2 - auto
    bool    entropy_coding_mode_flag; // 0 - CAVLC, 1 - CABAC
    Ipp8u   cabac_init_idc; // [0..2] used for CABAC
    Ipp8u   write_access_unit_delimiters; // 1 - write, 0 - do not
    bool    weighted_pred_flag;
    Ipp8u   weighted_bipred_idc;
    bool    use_transform_for_intra_decision;
    bool    transform_8x8_mode_flag;
    bool    qpprime_y_zero_transform_bypass_flag;
    Ipp8u   use_default_scaling_matrix;
    Ipp8u   aux_format_idc;
    bool    alpha_incr_flag;
    Ipp16s  alpha_opaque_value;
    Ipp16s  alpha_transparent_value;
    Ipp8u   bit_depth_aux;
    Ipp8u   bit_depth_luma;
    Ipp8u   bit_depth_chroma;
    Ipp32u  m_QualitySpeed;
    Ipp32u  quant_opt_level;
};

class H264VideoEncoder : public VideoEncoder
{
public:
    H264VideoEncoder();
    ~H264VideoEncoder();

    virtual Status Init(BaseCodecParams *init);

    virtual Status GetFrame(MediaData *in, MediaData *out);

    virtual Status GetInfo(BaseCodecParams *info);

    const H264PicParamSet* GetPicParamSet();
    const H264SeqParamSet* GetSeqParamSet();

    virtual Status Close();

    virtual Status Reset();

    virtual Status SetParams(BaseCodecParams* params);

    VideoData* GetReconstructedFrame();

protected:
    EncoderType  m_CurrEncoderType;
    void        *m_pEncoder;
};

}

#endif
