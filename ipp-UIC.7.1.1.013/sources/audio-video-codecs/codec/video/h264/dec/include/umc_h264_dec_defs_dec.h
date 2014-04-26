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

#ifndef __UMC_H264_DEC_DEFS_DEC_H__
#define __UMC_H264_DEC_DEFS_DEC_H__

#if defined (__ICL)
    // remark #981: operands are evaluated in unspecified order
#pragma warning(disable: 981)
#endif

#include <string.h>

#include "vm_types.h"
#include "umc_memory_allocator.h"
#include "umc_structures.h"
#include "umc_mutex.h"
#include "umc_h264_dec_tables.h"

#include "ippcore.h"
#include "ipps.h"

namespace UMC
{
//
// Define some useful macros
//

#if (defined( _MSC_VER ) || defined( __ICL )) && (!defined(_WIN32_WCE))
    #define DECLALIGN(x) __declspec(align(x))
    // turn off the "structure was padded due to __declspec(align())" warning
    #pragma warning(disable: 4324)
    #pragma warning(disable: 4328)
#else
    #define DECLALIGN(x)
#endif

#define ABSOWN(x) ((x) > 0 ? (x) : (-(x)))

#define _IS_ALIGNED(p, n)    (!(((Ipp8u*)(p) - (Ipp8u*)(0)) & ((n)-1)))

//#define CABAC_OPTIMIZATION
//#define STORE_CABAC_BITS
//#define CABAC_DECORER_COMP
//#define CABAC_CONTEXTS_COMP
//#define STORE_VLC

#ifdef CABAC_OPTIMIZATION
#ifdef WINDOWS
#define BSR(INDEX, MASK) { \
::_BitScanReverse((DWORD*)&MASK, MASK); \
INDEX = 8 - MASK; }
#else
#define BSR(INDEX, MASK) { \
__asm("bsr %1,%0" :"=r" (MASK) :"rm" (MASK)); \
INDEX = 8 - MASK; }
#endif
#endif

#define __VLC_FILE__ "d:\\vlc.ipp"
#define __CABAC_FILE__ "d:\\cabac.tst"
#define __PICLIST_FILE__ "d:\\piclist.ipp"

enum
{
    ALIGN_VALUE                 = 16
};

// Although the standard allows for a minimum width or height of 4, this
// implementation restricts the minimum value to 32.

enum {
    FLD_STRUCTURE       = 0,
    TOP_FLD_STRUCTURE   = 0,
    BOTTOM_FLD_STRUCTURE    = 1,
    FRM_STRUCTURE   = 2,
    AFRM_STRUCTURE  = 3
};

enum DisplayPictureStruct {
    DPS_FRAME     = 0,
    DPS_TOP,         // one field
    DPS_BOTTOM,      // one field
    DPS_TOP_BOTTOM,
    DPS_BOTTOM_TOP,
    DPS_TOP_BOTTOM_TOP,
    DPS_BOTTOM_TOP_BOTTOM,
    DPS_FRAME_DOUBLING,
    DPS_FRAME_TRIPLING
};

#ifndef SHARED_ENCDEC_STRUCTURES_DEFS
#define SHARED_ENCDEC_STRUCTURES_DEFS

enum    // Valid QP range
{
    QP_MAX = 51,
    QP_MIN = 0
};

enum
{
    LIST_0 = 0,     // Ref/mvs list 0 (L0)
    LIST_1 = 1      // Ref/mvs list 1 (L1)
};

//  NAL Unit Types
#ifndef SHARED_ENCDECBS_STRUCTURES_DEFS
#define SHARED_ENCDECBS_STRUCTURES_DEFS

// default plane & coeffs types:
typedef Ipp8u PlaneYCommon;
typedef Ipp8u PlaneUVCommon;
typedef Ipp16s CoeffsCommon;

typedef CoeffsCommon * CoeffsPtrCommon;
typedef PlaneYCommon * PlanePtrYCommon;
typedef PlaneUVCommon * PlanePtrUVCommon;

typedef enum {
        NAL_UT_UNSPECIFIED  = 0, // Unspecified
        NAL_UT_SLICE     = 1, // Coded Slice - slice_layer_no_partioning_rbsp
        NAL_UT_DPA       = 2, // Coded Data partition A - dpa_layer_rbsp
        NAL_UT_DPB       = 3, // Coded Data partition A - dpa_layer_rbsp
        NAL_UT_DPC       = 4, // Coded Data partition A - dpa_layer_rbsp
        NAL_UT_IDR_SLICE = 5, // Coded Slice of a IDR Picture - slice_layer_no_partioning_rbsp
        NAL_UT_SEI       = 6, // Supplemental Enhancement Information - sei_rbsp
        NAL_UT_SPS       = 7, // Sequence Parameter Set - seq_parameter_set_rbsp
        NAL_UT_PPS       = 8, // Picture Parameter Set - pic_parameter_set_rbsp
        NAL_UT_AUD        = 9, // Access Unit Delimiter - access_unit_delimiter_rbsp
        NAL_END_OF_SEQ   = 10, // End of sequence end_of_seq_rbsp()
        NAL_END_OF_STREAM = 11, // End of stream end_of_stream_rbsp
        NAL_UT_FD        = 12, // Filler Data - filler_data_rbsp
        NAL_UT_SPS_EX    = 13, // Sequence Parameter Set Extension - seq_parameter_set_extension_rbsp
        NAL_UNIT_PREFIX  = 14, // Prefix NAL unit in scalable extension - prefix_nal_unit_rbsp
        NAL_UNIT_SUBSET_SPS = 15, // Subset Sequence Parameter Set - subset_seq_parameter_set_rbsp
        NAL_UT_AUXILIARY = 19, // Auxiliary coded picture
        NAL_UNIT_SLICE_SCALABLE = 20 // Coded slice in scalable extension - slice_layer_in_scalable_extension_rbsp
} NAL_Unit_Type;
#endif/* SHARED_ENCDECBS_STRUCTURES_DEFS */

// Note!  The Picture Code Type values below are no longer used in the
// core encoder.   It only knows about slice types, and whether or not
// the frame is IDR, Reference or Disposable.  See enum above.

enum EnumSliceCodType        // Permitted MB Prediction Types
{                        // ------------------------------------
    PREDSLICE      = 0,    // I (Intra), P (Pred)
    BPREDSLICE     = 1, // I, P, B (BiPred)
    INTRASLICE     = 2,    // I
    S_PREDSLICE    = 3,    // SP (SPred), I
    S_INTRASLICE   = 4    // SI (SIntra), I
};

inline
FrameType SliceTypeToFrameType(EnumSliceCodType slice_type)
{
    switch(slice_type)
    {
    case PREDSLICE:
    case S_PREDSLICE:
        return P_PICTURE;
    case BPREDSLICE:
        return B_PICTURE;
    case INTRASLICE:
    case S_INTRASLICE:
        return I_PICTURE;
    }

    return NONE_PICTURE;
}

// Macroblock type definitions
// Keep these ordered such that intra types are first, followed by
// inter types.  Otherwise you'll need to change the definitions
// of IS_INTRA_MBTYPE and IS_INTER_MBTYPE.
//
// WARNING:  Because the decoder exposes macroblock types to the application,
// these values cannot be changed without affecting users of the decoder.
// If new macroblock types need to be inserted in the middle of the list,
// then perhaps existing types should retain their numeric value, the new
// type should be given a new value, and for coding efficiency we should
// perhaps decouple these values from the ones that are encoded in the
// bitstream.
//

typedef enum {
    MBTYPE_INTRA,            // 4x4
    MBTYPE_INTRA_16x16,
    MBTYPE_INTRA_BL,
    MBTYPE_PCM,              // Raw Pixel Coding, qualifies as a INTRA type...
    MBTYPE_INTER,            // 16x16
    MBTYPE_INTER_16x8,
    MBTYPE_INTER_8x16,
    MBTYPE_INTER_8x8,
    MBTYPE_INTER_8x8_REF0,
    MBTYPE_FORWARD,
    MBTYPE_BACKWARD,
    MBTYPE_SKIPPED,
    MBTYPE_DIRECT,
    MBTYPE_BIDIR,
    MBTYPE_FWD_FWD_16x8,
    MBTYPE_FWD_FWD_8x16,
    MBTYPE_BWD_BWD_16x8,
    MBTYPE_BWD_BWD_8x16,
    MBTYPE_FWD_BWD_16x8,
    MBTYPE_FWD_BWD_8x16,
    MBTYPE_BWD_FWD_16x8,
    MBTYPE_BWD_FWD_8x16,
    MBTYPE_BIDIR_FWD_16x8,
    MBTYPE_BIDIR_FWD_8x16,
    MBTYPE_BIDIR_BWD_16x8,
    MBTYPE_BIDIR_BWD_8x16,
    MBTYPE_FWD_BIDIR_16x8,
    MBTYPE_FWD_BIDIR_8x16,
    MBTYPE_BWD_BIDIR_16x8,
    MBTYPE_BWD_BIDIR_8x16,
    MBTYPE_BIDIR_BIDIR_16x8,
    MBTYPE_BIDIR_BIDIR_8x16,
    MBTYPE_B_8x8,
    NUMBER_OF_MBTYPES
} MB_Type;

typedef enum
{
    SEI_BUFFERING_PERIOD_TYPE   = 0,
    SEI_PIC_TIMING_TYPE         = 1,
    SEI_PAN_SCAN_RECT_TYPE      = 2,
    SEI_FILLER_TYPE             = 3,
    SEI_USER_DATA_REGISTERED_TYPE   = 4,
    SEI_USER_DATA_UNREGISTERED_TYPE = 5,
    SEI_RECOVERY_POINT_TYPE         = 6,
    SEI_DEC_REF_PIC_MARKING_TYPE    = 7,
    SEI_SPARE_PIC_TYPE              = 8,
    SEI_SCENE_INFO_TYPE             = 9,
    SEI_SUB_SEQ_INFO_TYPE           = 10,
    SEI_SUB_SEQ_LAYER_TYPE          = 11,
    SEI_SUB_SEQ_TYPE                = 12,
    SEI_FULL_FRAME_FREEZE_TYPE      = 13,
    SEI_FULL_FRAME_FREEZE_RELEASE_TYPE  = 14,
    SEI_FULL_FRAME_SNAPSHOT_TYPE        = 15,
    SEI_PROGRESSIVE_REF_SEGMENT_START_TYPE  = 16,
    SEI_PROGRESSIVE_REF_SEGMENT_END_TYPE    = 17,
    SEI_MOTION_CONSTRAINED_SG_SET_TYPE      = 18,
    SEI_RESERVED                            = 19,

    SEI_NUM_MESSAGES

} SEI_TYPE;

// 8x8 Macroblock subblock type definitions
typedef enum {
    SBTYPE_DIRECT = 0,            // B Slice modes
    SBTYPE_8x8 = 1,               // P slice modes
    SBTYPE_8x4 = 2,
    SBTYPE_4x8 = 3,
    SBTYPE_4x4 = 4,
    SBTYPE_FORWARD_8x8 = 5,       // Subtract 4 for mode #
    SBTYPE_BACKWARD_8x8 = 6,
    SBTYPE_BIDIR_8x8 = 7,
    SBTYPE_FORWARD_8x4 = 8,
    SBTYPE_FORWARD_4x8 = 9,
    SBTYPE_BACKWARD_8x4 = 10,
    SBTYPE_BACKWARD_4x8 = 11,
    SBTYPE_BIDIR_8x4 = 12,
    SBTYPE_BIDIR_4x8 = 13,
    SBTYPE_FORWARD_4x4 = 14,
    SBTYPE_BACKWARD_4x4 = 15,
    SBTYPE_BIDIR_4x4 = 16
} SB_Type;
#endif /*SHARED_ENCDEC_STRUCTURES_DEFS*/

// macro - yields TRUE if a given MB type is INTRA
#define IS_INTRA_MBTYPE_NOT_BL(mbtype) (((mbtype) < MBTYPE_INTER) && ((mbtype) != MBTYPE_INTRA_BL))
#define IS_INTRA_MBTYPE(mbtype) ((mbtype) < MBTYPE_INTER)

// macro - yields TRUE if a given MB type is INTER
#define IS_INTER_MBTYPE(mbtype) ((mbtype) >= MBTYPE_INTER)

#define IS_I_SLICE(SliceType) ((SliceType) == INTRASLICE)
#define IS_P_SLICE(SliceType) ((SliceType) == PREDSLICE || (SliceType) == S_PREDSLICE)
#define IS_B_SLICE(SliceType) ((SliceType) == BPREDSLICE)

#define IS_SKIP_DEBLOCKING_MODE_NON_REF (m_PermanentTurnOffDeblocking == 1)
#define IS_SKIP_DEBLOCKING_MODE_PERMANENT (m_PermanentTurnOffDeblocking == 2)
#define IS_SKIP_DEBLOCKING_MODE_PREVENTIVE (m_PermanentTurnOffDeblocking == 3)

enum
{
    MAX_NUM_SEQ_PARAM_SETS = 32,
    MAX_NUM_PIC_PARAM_SETS = 256,

    MAX_SLICE_NUM       = 128, //INCREASE IF NEEDED OR SET to -1 for adaptive counting (increases memory usage)
    MAX_NUM_REF_FRAMES  = 32,

    DEC_NUM_ALLOC_REF_FRAMES = 2,

    MAX_NUM_SLICE_GROUPS        = 8,
    MAX_SLICE_GROUP_MAP_TYPE    = 6,

    NUM_INTRA_TYPE_ELEMENTS     = 16,

    COEFFICIENTS_BUFFER_SIZE    = 16 * 51,

    MINIMAL_DATA_SIZE           = 4
};

// Possible values for disable_deblocking_filter_idc:
enum DeblockingModes_t
{
    DEBLOCK_FILTER_ON                   = 0,
    DEBLOCK_FILTER_OFF                  = 1,
    DEBLOCK_FILTER_ON_NO_SLICE_EDGES    = 2
};

#define SCLFLAT16     0
#define SCLDEFAULT    1
#define SCLREDEFINED  2

#pragma pack(1)

struct H264ScalingList4x4
{
    Ipp8u ScalingListCoeffs[16];
};

struct H264ScalingList8x8
{
    Ipp8u ScalingListCoeffs[64];
};

struct H264WholeQPLevelScale4x4
{
    Ipp16s LevelScaleCoeffs[88]/*since we do not support 422 and 444*/[16];
};
struct H264WholeQPLevelScale8x8
{
    Ipp16s LevelScaleCoeffs[88]/*since we do not support 422 and 444*/[64];
};

#pragma pack()

#pragma pack(16)

typedef Ipp32u IntraType;

// Sequence parameter set structure, corresponding to the H.264 bitstream definition.
struct H264SeqParamSet
{
    H264SeqParamSet()
    {
        seq_parameter_set_id = MAX_NUM_SEQ_PARAM_SETS;
        poffset_for_ref_frame = 0;
    }

    ~H264SeqParamSet()
    {
        if (poffset_for_ref_frame)
            delete [] poffset_for_ref_frame;
    }

    Ipp32s GetID() const
    {
        return seq_parameter_set_id;
    }

    bool operator == (const H264SeqParamSet & sps) const
    {
        if(memcmp(&(this->profile_idc), &sps.profile_idc, ((Ipp8s*)&poffset_for_ref_frame - (Ipp8s*)&profile_idc)))
            return false;

        // DEBUG : need to compare poffset_for_ref_frame
        if (poffset_for_ref_frame != sps.poffset_for_ref_frame)
            return false;

        return true;
    }

    bool operator != (const H264SeqParamSet & sps) const
    {
        return !(*this == sps);
    }

    Ipp8u        profile_idc;                        // baseline, main, etc.
    Ipp8u        level_idc;
    Ipp8u        constrained_set0_flag;
    Ipp8u        constrained_set1_flag;
    Ipp8u        constrained_set2_flag;
    Ipp8u        constrained_set3_flag;
    Ipp8u        constrained_set4_flag;
    Ipp8u        constrained_set5_flag;
    Ipp8u        chroma_format_idc;
    Ipp8u        residual_colour_transform_flag;
    Ipp8u        bit_depth_luma;
    Ipp8u        bit_depth_chroma;
    Ipp8u        qpprime_y_zero_transform_bypass_flag;
    Ipp8u        type_of_scaling_list_used[8];
    Ipp8u        seq_scaling_matrix_present_flag;
    //Ipp8u        seq_scaling_list_present_flag[8];
    H264ScalingList4x4 ScalingLists4x4[6];
    H264ScalingList8x8 ScalingLists8x8[2];
    Ipp8u        gaps_in_frame_num_value_allowed_flag;
    Ipp8u        frame_cropping_flag;
    Ipp32u       frame_cropping_rect_left_offset;
    Ipp32u       frame_cropping_rect_right_offset;
    Ipp32u       frame_cropping_rect_top_offset;
    Ipp32u       frame_cropping_rect_bottom_offset;
    Ipp8u        more_than_one_slice_group_allowed_flag;
    Ipp8u        arbitrary_slice_order_allowed_flag;  // If zero, slice order in pictures must
                                                      // be in increasing MB address order.
    Ipp8u        redundant_pictures_allowed_flag;
    Ipp8u        seq_parameter_set_id;                // id of this sequence parameter set
    Ipp8u        log2_max_frame_num;                  // Number of bits to hold the frame_num
    Ipp8u        pic_order_cnt_type;                  // Picture order counting method

    Ipp8u        delta_pic_order_always_zero_flag;    // If zero, delta_pic_order_cnt fields are
                                                      // present in slice header.
    Ipp8u        frame_mbs_only_flag;                 // Nonzero indicates all pictures in sequence
                                                      // are coded as frames (not fields).
    Ipp8u        required_frame_num_update_behavior_flag;

    Ipp8u        mb_adaptive_frame_field_flag;        // Nonzero indicates frame/field switch
                                                      // at macroblock level
    Ipp8u        direct_8x8_inference_flag;           // Direct motion vector derivation method
    Ipp8u        vui_parameters_present_flag;         // Zero indicates default VUI parameters
    Ipp32u       log2_max_pic_order_cnt_lsb;          // Value of MaxPicOrderCntLsb.
    Ipp32s       offset_for_non_ref_pic;

    Ipp32s       offset_for_top_to_bottom_field;      // Expected pic order count difference from
                                                      // top field to bottom field.

    Ipp32u       num_ref_frames_in_pic_order_cnt_cycle;
    Ipp32u       num_ref_frames;                      // total number of pics in decoded pic buffer
    Ipp32u       frame_width_in_mbs;
    Ipp32u       frame_height_in_mbs;

    // These fields are calculated from values above.  They are not written to the bitstream
    Ipp32u       MaxMbAddress;
    Ipp32u       MaxPicOrderCntLsb;
    // vui part
    Ipp8u        aspect_ratio_info_present_flag;
    Ipp8u        aspect_ratio_idc;
    Ipp16u       sar_width;
    Ipp16u       sar_height;
    Ipp8u        overscan_info_present_flag;
    Ipp8u        overscan_appropriate_flag;
    Ipp8u        video_signal_type_present_flag;
    Ipp8u        video_format;
    Ipp8u        video_full_range_flag;
    Ipp8u        colour_description_present_flag;
    Ipp8u        colour_primaries;
    Ipp8u        transfer_characteristics;
    Ipp8u        matrix_coefficients;
    Ipp8u        chroma_loc_info_present_flag;
    Ipp8u        chroma_sample_loc_type_top_field;
    Ipp8u        chroma_sample_loc_type_bottom_field;
    Ipp8u        timing_info_present_flag;
    Ipp32u       num_units_in_tick;
    Ipp32u       time_scale;
    Ipp8u        fixed_frame_rate_flag;
    Ipp8u        nal_hrd_parameters_present_flag;
    Ipp8u        vcl_hrd_parameters_present_flag;
    Ipp8u        low_delay_hrd_flag;
    Ipp8u        pic_struct_present_flag;
    Ipp8u        bitstream_restriction_flag;
    Ipp8u        motion_vectors_over_pic_boundaries_flag;
    Ipp8u        max_bytes_per_pic_denom;
    Ipp8u        max_bits_per_mb_denom;
    Ipp8u        log2_max_mv_length_horizontal;
    Ipp8u        log2_max_mv_length_vertical;
    Ipp8u        num_reorder_frames;
    Ipp8u        max_dec_frame_buffering;
    //hrd_parameters
    Ipp8u        cpb_cnt;
    Ipp8u        bit_rate_scale;
    Ipp8u        cpb_size_scale;
    Ipp32u       bit_rate_value[32];
    Ipp32u       cpb_size_value[32];
    Ipp8u        cbr_flag[32];
    Ipp8u        initial_cpb_removal_delay_length;
    Ipp8u        cpb_removal_delay_length;
    Ipp8u        dpb_output_delay_length;
    Ipp8u        time_offset_length;

    Ipp32s       *poffset_for_ref_frame;              // pointer to array of stored frame offsets,
                                                      // length num_stored_frames_in_pic_order_cnt_cycle,
                                                      // for pic order cnt type 1
private:
    H264SeqParamSet(const H264SeqParamSet&) {}        // Empty copy constructor
};    // H264SeqParamSet

// Sequence parameter set extension structure, corresponding to the H.264 bitstream definition.
struct H264SeqParamSetExtension
{
    Ipp8u       seq_parameter_set_id;
    Ipp8u       aux_format_idc;
    Ipp8u       bit_depth_aux;
    Ipp8u       alpha_incr_flag;
    Ipp8u       alpha_opaque_value;
    Ipp8u       alpha_transparent_value;
    Ipp8u       additional_extension_flag;

    bool operator == (const H264SeqParamSetExtension& sps) const
    {
        if (memcmp(&(this->seq_parameter_set_id), &sps.seq_parameter_set_id, sizeof(H264SeqParamSetExtension)))
            return false;

        return true;
    }

    bool operator != (const H264SeqParamSetExtension & sps) const
    {
        return !(*this == sps);
    }

    H264SeqParamSetExtension()
    {
        aux_format_idc = 0;
        seq_parameter_set_id = MAX_NUM_SEQ_PARAM_SETS;    // illegal id
    }

    Ipp32s GetID() const
    {
        return seq_parameter_set_id;
    }
};    // H264SeqParamSetExtension

// Picture parameter set structure, corresponding to the H.264 bitstream definition.
struct H264PicParamSet
{
// Flexible macroblock order structure, defining the FMO map for a picture
// paramter set.

    struct SliceGroupInfoStruct
    {
        Ipp8u        slice_group_map_type;                // 0..6

        // The additional slice group data depends upon map type
        union
        {
            // type 0
            Ipp32u    run_length[MAX_NUM_SLICE_GROUPS];

            // type 2
            struct
            {
                Ipp32u top_left[MAX_NUM_SLICE_GROUPS-1];
                Ipp32u bottom_right[MAX_NUM_SLICE_GROUPS-1];
            }t1;

            // types 3-5
            struct
            {
                Ipp8u  slice_group_change_direction_flag;
                Ipp32u slice_group_change_rate;
            }t2;

            // type 6
            struct
            {
                Ipp32u pic_size_in_map_units;     // number of macroblocks if no field coding
                Ipp8u *pSliceGroupIDMap;          // Id for each slice group map unit
            }t3;
        };
    };    // SliceGroupInfoStruct

    Ipp16u       pic_parameter_set_id;            // of this picture parameter set
    Ipp8u        seq_parameter_set_id;            // of seq param set used for this pic param set
    Ipp8u        entropy_coding_mode;             // zero: CAVLC, else CABAC

    Ipp8u        pic_order_present_flag;          // Zero indicates only delta_pic_order_cnt[0] is
                                                  // present in slice header; nonzero indicates
                                                  // delta_pic_order_cnt[1] is also present.

    Ipp8u        weighted_pred_flag;              // Nonzero indicates weighted prediction applied to
                                                  // P and SP slices
    Ipp8u        weighted_bipred_idc;             // 0: no weighted prediction in B slices
                                                  // 1: explicit weighted prediction
                                                  // 2: implicit weighted prediction
    Ipp8s        pic_init_qp;                     // default QP for I,P,B slices
    Ipp8s        pic_init_qs;                     // default QP for SP, SI slices

    Ipp8s        chroma_qp_index_offset[2];       // offset to add to QP for chroma

    Ipp8u        deblocking_filter_variables_present_flag;    // If nonzero, deblock filter params are
                                                  // present in the slice header.
    Ipp8u        constrained_intra_pred_flag;     // Nonzero indicates constrained intra mode

    Ipp8u        redundant_pic_cnt_present_flag;  // Nonzero indicates presence of redundant_pic_cnt
                                                  // in slice header
    Ipp32u       num_slice_groups;                // One: no FMO
    Ipp32u       num_ref_idx_l0_active;           // num of ref pics in list 0 used to decode the picture
    Ipp32u       num_ref_idx_l1_active;           // num of ref pics in list 1 used to decode the picture
    Ipp8u        transform_8x8_mode_flag;
    Ipp8u        type_of_scaling_list_used[8];

    H264ScalingList4x4 ScalingLists4x4[6];
    H264ScalingList8x8 ScalingLists8x8[2];

    // Level Scale addition
    H264WholeQPLevelScale4x4        m_LevelScale4x4[6];
    H264WholeQPLevelScale8x8        m_LevelScale8x8[2];

    SliceGroupInfoStruct SliceGroupInfo;    // Used only when num_slice_groups > 1

    H264PicParamSet()
    {
        Reset();
    }

    void Reset()
    {
        pic_parameter_set_id = MAX_NUM_PIC_PARAM_SETS;
        seq_parameter_set_id = MAX_NUM_SEQ_PARAM_SETS;
        num_slice_groups = 0;
    }


    ~H264PicParamSet()
    {
        if (1 < num_slice_groups &&
            (6 == SliceGroupInfo.slice_group_map_type) &&
            SliceGroupInfo.t3.pSliceGroupIDMap)
        {
            delete [] SliceGroupInfo.t3.pSliceGroupIDMap;
        }
    }

    Ipp32s GetID() const
    {
        return pic_parameter_set_id;
    }

    bool operator == (const H264PicParamSet & pps) const
    {
        if (memcmp(&pic_parameter_set_id, &pps.pic_parameter_set_id,
            ((Ipp8s*)&SliceGroupInfo - (Ipp8s*)&pic_parameter_set_id)))
            return false;

        if (memcmp(&SliceGroupInfo, &pps.SliceGroupInfo, sizeof(SliceGroupInfoStruct)))
            return false;

        return true;
    }

    bool operator != (const H264PicParamSet & pps) const
    {
        return !(*this == pps);
    }

};    // H264PicParamSet

struct RefPicListReorderInfo
{
    Ipp32u       num_entries;                 // number of currently valid idc,value pairs
    Ipp8u        reordering_of_pic_nums_idc[MAX_NUM_REF_FRAMES];
    Ipp32u       reorder_value[MAX_NUM_REF_FRAMES];    // abs_diff_pic_num or long_term_pic_num
};

struct AdaptiveMarkingInfo
{
    Ipp32u       num_entries;                 // number of currently valid mmco,value pairs
    Ipp8u        mmco[MAX_NUM_REF_FRAMES];    // memory management control operation id
    Ipp32u       value[MAX_NUM_REF_FRAMES*2]; // operation-dependent data, max 2 per operation
};

struct PredWeightTable
{
    Ipp8u        luma_weight_flag;            // nonzero: luma weight and offset in bitstream
    Ipp8u        chroma_weight_flag;          // nonzero: chroma weight and offset in bitstream
    Ipp8s        luma_weight;                 // luma weighting factor
    Ipp8s        luma_offset;                 // luma weighting offset
    Ipp8s        chroma_weight[2];            // chroma weighting factor (Cb,Cr)
    Ipp8s        chroma_offset[2];            // chroma weighting offset (Cb,Cr)
};    // PredWeightTable

typedef Ipp32s H264DecoderMBAddr;

// Slice header structure, corresponding to the H.264 bitstream definition.
struct H264SliceHeader
{
    NAL_Unit_Type nal_unit_type;                        // (NAL_Unit_Type) specifies the type of RBSP data structure contained in the NAL unit as specified in Table 7-1 of h264 standart
    Ipp16u        pic_parameter_set_id;                 // of pic param set used for this slice
    Ipp8u         field_pic_flag;                       // zero: frame picture, else field picture
    Ipp8u         MbaffFrameFlag;
    Ipp8u         bottom_field_flag;                    // zero: top field, else bottom field
    Ipp8u         direct_spatial_mv_pred_flag;          // zero: temporal direct, else spatial direct
    Ipp8u         num_ref_idx_active_override_flag;     // nonzero: use ref_idx_active from slice header
                                                        // instead of those from pic param set
    Ipp8u         no_output_of_prior_pics_flag;         // nonzero: remove previously decoded pictures
                                                        // from decoded picture buffer
    Ipp8u         long_term_reference_flag;             // How to set MaxLongTermFrameIdx
    Ipp32u        cabac_init_idc;                      // CABAC initialization table index (0..2)
    Ipp8u         adaptive_ref_pic_marking_mode_flag;   // Ref pic marking mode of current picture
    Ipp32s        slice_qp_delta;                       // to calculate default slice QP
    Ipp8u         sp_for_switch_flag;                   // SP slice decoding control
    Ipp32s        slice_qs_delta;                       // to calculate default SP,SI slice QS
    Ipp32u        disable_deblocking_filter_idc;       // deblock filter control, 0=filter all edges
    Ipp32s        slice_alpha_c0_offset;               // deblock filter c0, alpha table offset
    Ipp32s        slice_beta_offset;                   // deblock filter beta table offset
    H264DecoderMBAddr first_mb_in_slice;
    Ipp32s        frame_num;
    EnumSliceCodType slice_type;
    Ipp8u         idr_flag;
    Ipp8u         nal_ref_idc;
    Ipp32u        idr_pic_id;                           // ID of an IDR picture
    Ipp32s        pic_order_cnt_lsb;                    // picture order count (mod MaxPicOrderCntLsb)
    Ipp32s        delta_pic_order_cnt_bottom;           // Pic order count difference, top & bottom fields
    Ipp32u        difference_of_pic_nums;               // Ref pic memory mgmt
    Ipp32u        long_term_pic_num;                    // Ref pic memory mgmt
    Ipp32u        long_term_frame_idx;                  // Ref pic memory mgmt
    Ipp32u        max_long_term_frame_idx;              // Ref pic memory mgmt
    Ipp32s        delta_pic_order_cnt[2];               // picture order count differences
    Ipp32u        redundant_pic_cnt;                    // for redundant slices
    Ipp32s        num_ref_idx_l0_active;                // num of ref pics in list 0 used to decode the slice,
                                                        // see num_ref_idx_active_override_flag
    Ipp32s        num_ref_idx_l1_active;                // num of ref pics in list 1 used to decode the slice
                                                        // see num_ref_idx_active_override_flag
    Ipp32u        slice_group_change_cycle;             // for FMO
    Ipp8u         luma_log2_weight_denom;               // luma weighting denominator
    Ipp8u         chroma_log2_weight_denom;             // chroma weighting denominator

    bool          is_auxiliary;
}; // H264SliceHeader

struct H264LimitedSliceHeader
{
    EnumSliceCodType slice_type;                        // (EnumSliceCodType) slice type
    Ipp8u disable_deblocking_filter_idc;                // (Ipp8u) deblock filter control, 0 = filter all edges
    Ipp8s slice_alpha_c0_offset;                        // (Ipp8s) deblock filter c0, alpha table offset
    Ipp8s slice_beta_offset;                            // (Ipp8s) deblock filter beta table offset

}; // H264LimitedSliceHeader

struct H264SEIPayLoad
{
    SEI_TYPE payLoadType;
    Ipp32u   payLoadSize;

    Ipp8u * user_data; // for UserDataRegistered or UserDataUnRegistered

    H264SEIPayLoad()
        : payLoadType(SEI_RESERVED)
        , payLoadSize(0)
        , user_data(0)
    {
    }

    void Reset()
    {
        if (user_data)
            delete [] user_data;
        user_data = 0;
        payLoadType = SEI_RESERVED;
        payLoadSize = 0;
    }

    ~H264SEIPayLoad()
    {
        if (user_data)
            delete [] user_data;
    }

    Ipp32s GetID() const
    {
        return payLoadType;
    }

    bool operator == (const H264SEIPayLoad & sei) const
    {
        if (memcmp(&payLoadType, &sei.payLoadType, sizeof(H264SEIPayLoad)))
            return false;

        return true;
    }

    bool operator != (const H264SEIPayLoad & sei) const
    {
        return !(*this == sei);
    }

    union SEIMessages
    {
        struct BufferingPeriod
        {
            Ipp32u initial_cbp_removal_delay[2][16];
            Ipp32u initial_cbp_removal_delay_offset[2][16];
        }buffering_period;

        struct PicTiming
        {
            Ipp32u cbp_removal_delay;
            Ipp32u dpb_ouput_delay;
            DisplayPictureStruct pic_struct;
            Ipp8u  clock_timestamp_flag[16];
            struct ClockTimestamps
            {
                Ipp8u ct_type;
                Ipp8u nunit_field_based_flag;
                Ipp8u counting_type;
                Ipp8u full_timestamp_flag;
                Ipp8u discontinuity_flag;
                Ipp8u cnt_dropped_flag;
                Ipp8u n_frames;
                Ipp8u seconds_value;
                Ipp8u minutes_value;
                Ipp8u hours_value;
                Ipp8u time_offset;
            }clock_timestamps[16];
        }pic_timing;

        struct PanScanRect
        {
            Ipp8u  pan_scan_rect_id;
            Ipp8u  pan_scan_rect_cancel_flag;
            Ipp8u  pan_scan_cnt;
            Ipp32u pan_scan_rect_left_offset[32];
            Ipp32u pan_scan_rect_right_offset[32];
            Ipp32u pan_scan_rect_top_offset[32];
            Ipp32u pan_scan_rect_bottom_offset[32];
            Ipp8u  pan_scan_rect_repetition_period;
        }pan_scan_rect;

        struct UserDataRegistered
        {
            Ipp8u itu_t_t35_country_code;
            Ipp8u itu_t_t35_country_code_extension_byte;
        } user_data_registered;

        struct RecoveryPoint
        {
            Ipp8u recovery_frame_cnt;
            Ipp8u exact_match_flag;
            Ipp8u broken_link_flag;
            Ipp8u changing_slice_group_idc;
        }recovery_point;

        struct DecRefPicMarkingRepetition
        {
            Ipp8u original_idr_flag;
            Ipp8u original_frame_num;
            Ipp8u original_field_pic_flag;
            Ipp8u original_bottom_field_flag;
        }dec_ref_pic_marking_repetition;

        struct SparePic
        {
            Ipp32u target_frame_num;
            Ipp8u  spare_field_flag;
            Ipp8u  target_bottom_field_flag;
            Ipp8u  num_spare_pics;
            Ipp8u  delta_spare_frame_num[16];
            Ipp8u  spare_bottom_field_flag[16];
            Ipp8u  spare_area_idc[16];
            Ipp8u  *spare_unit_flag[16];
            Ipp8u  *zero_run_length[16];
        }spare_pic;

        struct SceneInfo
        {
            Ipp8u scene_info_present_flag;
            Ipp8u scene_id;
            Ipp8u scene_transition_type;
            Ipp8u second_scene_id;
        }scene_info;

        struct SubSeqInfo
        {
            Ipp8u sub_seq_layer_num;
            Ipp8u sub_seq_id;
            Ipp8u first_ref_pic_flag;
            Ipp8u leading_non_ref_pic_flag;
            Ipp8u last_pic_flag;
            Ipp8u sub_seq_frame_num_flag;
            Ipp8u sub_seq_frame_num;
        }sub_seq_info;

        struct SubSeqLayerCharacteristics
        {
            Ipp8u  num_sub_seq_layers;
            Ipp8u  accurate_statistics_flag[16];
            Ipp16u average_bit_rate[16];
            Ipp16u average_frame_rate[16];
        }sub_seq_layer_characteristics;

        struct SubSeqCharacteristics
        {
            Ipp8u  sub_seq_layer_num;
            Ipp8u  sub_seq_id;
            Ipp8u  duration_flag;
            Ipp8u  sub_seq_duration;
            Ipp8u  average_rate_flag;
            Ipp8u  accurate_statistics_flag;
            Ipp16u average_bit_rate;
            Ipp16u average_frame_rate;
            Ipp8u  num_referenced_subseqs;
            Ipp8u  ref_sub_seq_layer_num[16];
            Ipp8u  ref_sub_seq_id[16];
            Ipp8u  ref_sub_seq_direction[16];
        }sub_seq_characteristics;

        struct FullFrameFreeze
        {
            Ipp32u full_frame_freeze_repetition_period;
        }full_frame_freeze;

        struct FullFrameSnapshot
        {
            Ipp8u snapshot_id;
        }full_frame_snapshot;

        struct ProgressiveRefinementSegmentStart
        {
            Ipp8u progressive_refinement_id;
            Ipp8u num_refinement_steps;
        }progressive_refinement_segment_start;

        struct MotionConstrainedSliceGroupSet
        {
            Ipp8u num_slice_groups_in_set;
            Ipp8u slice_group_id[8];
            Ipp8u exact_sample_value_match_flag;
            Ipp8u pan_scan_rect_flag;
            Ipp8u pan_scan_rect_id;
        }motion_constrained_slice_group_set;

        struct FilmGrainCharacteristics
        {
            Ipp8u film_grain_characteristics_cancel_flag;
            Ipp8u model_id;
            Ipp8u separate_colour_description_present_flag;
            Ipp8u film_grain_bit_depth_luma;
            Ipp8u film_grain_bit_depth_chroma;
            Ipp8u film_grain_full_range_flag;
            Ipp8u film_grain_colour_primaries;
            Ipp8u film_grain_transfer_characteristics;
            Ipp8u film_grain_matrix_coefficients;
            Ipp8u blending_mode_id;
            Ipp8u log2_scale_factor;
            Ipp8u comp_model_present_flag[3];
            Ipp8u num_intensity_intervals[3];
            Ipp8u num_model_values[3];
            Ipp8u intensity_interval_lower_bound[3][256];
            Ipp8u intensity_interval_upper_bound[3][256];
            Ipp8u comp_model_value[3][3][256];
            Ipp8u film_grain_characteristics_repetition_period;
        }film_grain_characteristics;

        struct DeblockingFilterDisplayPreference
        {
            Ipp8u deblocking_display_preference_cancel_flag;
            Ipp8u display_prior_to_deblocking_preferred_flag;
            Ipp8u dec_frame_buffering_constraint_flag;
            Ipp8u deblocking_display_preference_repetition_period;
        }deblocking_filter_display_preference;

        struct StereoVideoInfo
        {
            Ipp8u field_views_flag;
            Ipp8u top_field_is_left_view_flag;
            Ipp8u current_frame_is_left_view_flag;
            Ipp8u next_frame_is_second_view_flag;
            Ipp8u left_view_self_contained_flag;
            Ipp8u right_view_self_contained_flag;
        }stereo_video_info;

    }SEI_messages;
    const H264SEIPayLoad& operator=(const H264SEIPayLoad& other) 
    {
        Reset();
        ippsCopy_8u((Ipp8u*)&other, (Ipp8u*)this, sizeof(H264SEIPayLoad));

        if(other.user_data)
        {
            user_data = new Ipp8u[other.payLoadSize];
            ippsCopy_8u(other.user_data, user_data, other.payLoadSize);
        }
        return *this;
    }
private:
    H264SEIPayLoad(const H264SEIPayLoad&) {}    // Empty copy constructor
};

#pragma pack()

// This file defines some data structures and constants used by the decoder,
// that are also needed by other classes, such as post filters and
// error concealment.

#define INTERP_FACTOR 4
#define INTERP_SHIFT 2

#define CHROMA_INTERP_FACTOR 8
#define CHROMA_INTERP_SHIFT 3

// at picture edge, clip motion vectors to only this far beyond the edge,
// in pixel units.
#define D_MV_CLIP_LIMIT 19

enum Direction_t{
    D_DIR_FWD = 0,
    D_DIR_BWD = 1,
    D_DIR_BIDIR = 2,
    D_DIR_DIRECT = 3,
    D_DIR_DIRECT_SPATIAL_FWD = 4,
    D_DIR_DIRECT_SPATIAL_BWD = 5,
    D_DIR_DIRECT_SPATIAL_BIDIR = 6
};

inline bool IsForwardOnly(Ipp32s direction)
{
    return (direction == D_DIR_FWD) || (direction == D_DIR_DIRECT_SPATIAL_FWD);
}

inline bool IsHaveForward(Ipp32s direction)
{
    return (direction == D_DIR_FWD) || (direction == D_DIR_BIDIR) ||
        (direction == D_DIR_DIRECT_SPATIAL_FWD) || (direction == D_DIR_DIRECT_SPATIAL_BIDIR) ||
         (direction == D_DIR_DIRECT);
}

inline bool IsBackwardOnly(Ipp32s direction)
{
    return (direction == D_DIR_BWD) || (direction == D_DIR_DIRECT_SPATIAL_BWD);
}

inline bool IsHaveBackward(Ipp32s direction)
{
    return (direction == D_DIR_BWD) || (direction == D_DIR_BIDIR) ||
        (direction == D_DIR_DIRECT_SPATIAL_BWD) || (direction == D_DIR_DIRECT_SPATIAL_BIDIR) ||
        (direction == D_DIR_DIRECT);
}

inline bool IsBidirOnly(Ipp32s direction)
{
    return (direction == D_DIR_BIDIR) || (direction == D_DIR_DIRECT_SPATIAL_BIDIR) ||
        (direction == D_DIR_DIRECT);
}

// Warning: If these bit defines change, also need to change same
// defines  and related code in sresidual.s.
enum CBP
{
    D_CBP_LUMA_DC = 0x00001,
    D_CBP_LUMA_AC = 0x1fffe,

    D_CBP_CHROMA_DC = 0x00001,
    D_CBP_CHROMA_AC = 0x1fffe,
    D_CBP_CHROMA_AC_420 = 0x0001e,
    D_CBP_CHROMA_AC_422 = 0x001fe,
    D_CBP_CHROMA_AC_444 = 0x1fffe,

    D_CBP_1ST_LUMA_AC_BITPOS = 1,
    D_CBP_1ST_CHROMA_DC_BITPOS = 17,
    D_CBP_1ST_CHROMA_AC_BITPOS = 19
};

enum
{
    FIRST_DC_LUMA = 0,
    FIRST_AC_LUMA = 1,
    FIRST_DC_CHROMA = 17,
    FIRST_AC_CHROMA = 19
};

enum
{
    CHROMA_FORMAT_400       = 0,
    CHROMA_FORMAT_420       = 1,
    CHROMA_FORMAT_422       = 2,
    CHROMA_FORMAT_444       = 3
};

inline
Ipp32u CreateIPPCBPMask420(Ipp32u cbpU, Ipp32u cbpV)
{
    Ipp32u cbp4x4 = (((cbpU & D_CBP_CHROMA_DC) | ((cbpV & D_CBP_CHROMA_DC) << 1)) << D_CBP_1ST_CHROMA_DC_BITPOS) |
                     ((cbpU & D_CBP_CHROMA_AC_420) << (D_CBP_1ST_CHROMA_AC_BITPOS - 1)) |
                     ((cbpV & D_CBP_CHROMA_AC_420) << (D_CBP_1ST_CHROMA_AC_BITPOS + 4 - 1));
    return cbp4x4;

} // Ipp32u CreateIPPCBPMask420(Ipp32u nUCBP, Ipp32u nVCBP)

inline
Ipp64u CreateIPPCBPMask422(Ipp32u cbpU, Ipp32u cbpV)
{
    Ipp64u cbp4x4 = (((cbpU & D_CBP_CHROMA_DC) | ((cbpV & D_CBP_CHROMA_DC) << 1)) << D_CBP_1ST_CHROMA_DC_BITPOS) |
                    (((Ipp64u)cbpU & D_CBP_CHROMA_AC_422) << (D_CBP_1ST_CHROMA_AC_BITPOS - 1)) |
                    (((Ipp64u)cbpV & D_CBP_CHROMA_AC_422) << (D_CBP_1ST_CHROMA_AC_BITPOS + 8 - 1));

    return cbp4x4;

} // Ipp32u CreateIPPCBPMask422(Ipp32u nUCBP, Ipp32u nVCBP)

inline
Ipp64u CreateIPPCBPMask444(Ipp32u cbpU, Ipp32u cbpV)
{
    Ipp64u cbp4x4 = (((cbpU & D_CBP_CHROMA_DC) | ((cbpV & D_CBP_CHROMA_DC) << 1)) << D_CBP_1ST_CHROMA_DC_BITPOS) |
                    (((Ipp64u)cbpU & D_CBP_CHROMA_AC_444) << (D_CBP_1ST_CHROMA_AC_BITPOS - 1)) |
                    (((Ipp64u)cbpV & D_CBP_CHROMA_AC_444) << (D_CBP_1ST_CHROMA_AC_BITPOS + 16 - 1));
    return cbp4x4;

} // Ipp32u CreateIPPCBPMask444(Ipp32u nUCBP, Ipp32u nVCBP)


#define BLOCK_IS_ON_LEFT_EDGE(x) (!((x)&3))
#define BLOCK_IS_ON_TOP_EDGE(x) ((x)<4)

#define CHROMA_BLOCK_IS_ON_LEFT_EDGE(x,c) (x_pos_value[c][x]==0)
#define CHROMA_BLOCK_IS_ON_TOP_EDGE(y,c) (y_pos_value[c][y]==0)

#define GetMBFieldDecodingFlag(x) ((x).mbflags.fdf)
#define GetMBDirectSkipFlag(x) ((x).mbflags.isDirect || x.mbflags.isSkipped)
#define GetMB8x8TSFlag(x) ((x).mbflags.transform8x8)

#define pGetMBFieldDecodingFlag(x) ((x)->mbflags.fdf)
#define pGetMBDirectSkipFlag(x) ((x)->mbflags.isDirect || x->mbflags.isSkipped)
#define pGetMB8x8TSFlag(x) ((x)->mbflags.transform8x8)

#define GetMBSkippedFlag(x) ((x).mbflags.isSkipped)
#define pGetMBSkippedFlag(x) ((x)->mbflags.isSkipped)

#define pSetMBDirectFlag(x)  ((x)->mbflags.isDirect = 1);
#define SetMBDirectFlag(x)  ((x).mbflags.isDirect = 1);

#define pSetMBSkippedFlag(x)  ((x)->mbflags.isSkipped = 1);
#define SetMBSkippedFlag(x)  ((x).mbflags.isSkipped = 1);

#define GetMBBaseModeFlag(x) (x.mbflags.isBaseMode)
#define pGetMBBaseModeFlag(x) (x->mbflags.isBaseMode)

#define pSetMBBaseModeFlag(x,y)  ((x)->mbflags.isBaseMode = (Ipp8u)y)
#define SetMBBaseModeFlag(x,y)  ((x).mbflags.isBaseMode = (Ipp8u)y)

#define GetMBCropFlag(x) ((x).mbflags.isCropped)
#define pGetGetMBCropFlag(x) ((x)->mbflags.isCropped)

#define pSetMBCropFlag(x,y)  ((x)->mbflags.isCropped = (Ipp8u)y)
#define SetMBCropFlag(x,y)  ((x).mbflags.isCropped = (Ipp8u)y)

#define GetMBResidualPredictionFlag(x) ((x).mbflags.residualPrediction)
#define pGetMBResidualPredictionFlag(x) ((x)->mbflags.residualPrediction)

#define pSetMBResidualPredictionFlag(x,y)  ((x)->mbflags.residualPrediction = (Ipp8u)y)
#define SetMBResidualPredictionFlag(x,y)  ((x).mbflags.residualPrediction = (Ipp8u)y)

#define pSetMBFieldDecodingFlag(x,y)     \
    (x->mbflags.fdf = (Ipp8u)y)

#define SetMBFieldDecodingFlag(x,y)     \
    (x.mbflags.fdf = (Ipp8u)y)

#define pSetMB8x8TSFlag(x,y)            \
    (x->mbflags.transform8x8 = (Ipp8u)y)

#define SetMB8x8TSFlag(x,y)             \
    (x.mbflags.transform8x8 = (Ipp8u)y)

#define pSetPairMBFieldDecodingFlag(x1,x2,y)    \
    (x1->mbflags.fdf = (Ipp8u)y);    \
    (x2->mbflags.fdf = (Ipp8u)y)

#define SetPairMBFieldDecodingFlag(x1,x2,y)     \
    (x1.mbflags.fdf = (Ipp8u)y);    \
    (x2.mbflags.fdf = (Ipp8u)y)

///////////////// New structures

#pragma pack(1)

struct H264DecoderMotionVector
{
    Ipp16s mvx;
    Ipp16s mvy;

}; // 4 bytes

typedef Ipp8s RefIndexType;

struct H264DecoderMacroblockRefIdxs
{
    RefIndexType refIndexs[4];                              // 4 bytes

};//4 bytes

struct H264DecoderMacroblockMVs
{
    H264DecoderMotionVector MotionVectors[16];                  // (H264DecoderMotionVector []) motion vectors for each block in macroblock

}; // 64 bytes

typedef Ipp8u NumCoeffsType;
struct H264DecoderMacroblockCoeffsInfo
{
    NumCoeffsType numCoeffs[48];                                         // (Ipp8u) number of coefficients in each block in macroblock

}; // 24 bytes for YUV420. For YUV422, YUV444 support need to extend it

struct H264MBFlags
{
    Ipp8u fdf : 1;
    Ipp8u transform8x8 : 1;
    Ipp8u residualPrediction : 1;
    Ipp8u isDirect : 1;
    Ipp8u isSkipped : 1;
    Ipp8u isBaseMode : 1;
    Ipp8u isCropped : 1;
};

struct H264DecoderMacroblockGlobalInfo
{
    Ipp8s sbtype[4];                                            // (Ipp8u []) types of subblocks in macroblock
    Ipp16s slice_id;                                            // (Ipp16s) number of slice
    Ipp8s mbtype;                                               // (Ipp8u) type of macroblock
    H264MBFlags mbflags;

    H264DecoderMacroblockRefIdxs refIdxs[2];
}; // 16 bytes

struct H264DecoderMacroblockLocalInfo
{
    Ipp32u cbp4x4_luma;                                         // (Ipp32u) coded block pattern of luma blocks
    Ipp32u cbp4x4_chroma[2];                                    // (Ipp32u []) coded block patterns of chroma blocks
    Ipp8u cbp;
    Ipp8s QP;

    union
    {
        Ipp8s sbdir[4];
        struct
        {
            Ipp16u edge_type;
            Ipp8u intra_chroma_mode;
        } IntraTypes;
    };
}; // 20 bytes

struct H264DecoderBlockLocation
{
    Ipp32s mb_num;                                              // (Ipp32s) number of owning macroblock
    Ipp32s block_num;                                           // (Ipp32s) number of block

}; // 8 bytes

struct H264DecoderMacroblockNeighboursInfo
{
    Ipp32s mb_A;                                                // (Ipp32s) number of left macroblock
    Ipp32s mb_B;                                                // (Ipp32s) number of top macroblock
    Ipp32s mb_C;                                                // (Ipp32s) number of right-top macroblock
    Ipp32s mb_D;                                                // (Ipp32s) number of left-top macroblock

}; // 32 bytes

struct H264DecoderBlockNeighboursInfo
{
    H264DecoderBlockLocation mbs_left[4];
    H264DecoderBlockLocation mb_above;
    H264DecoderBlockLocation mb_above_right;
    H264DecoderBlockLocation mb_above_left;
    H264DecoderBlockLocation mbs_left_chroma[2][4];
    H264DecoderBlockLocation mb_above_chroma[2];
    Ipp32s m_bInited;

}; // 128 bytes

struct H264DecoderMacroblockLayerInfo
{
    Ipp8s sbtype[4];
    Ipp8s sbdir[4];
    Ipp8s mbtype;
};

#pragma pack()

//this structure is present in each decoder frame
struct H264DecoderGlobalMacroblocksDescriptor
{
    H264DecoderMacroblockMVs *MV[2];//MotionVectors L0 L1
    H264DecoderMacroblockGlobalInfo *mbs;//macroblocks
};

struct H264DecoderBaseFrameDescriptor
{
    Ipp32s m_PictureStructureForDec;
    Ipp32s m_PictureStructureForRef;
    Ipp32s totalMBs;
    Ipp32s m_PicOrderCnt[2];
    Ipp32s m_bottom_field_flag[2];
    bool m_isShortTermRef[2];
    bool m_isLongTermRef[2];

    H264DecoderGlobalMacroblocksDescriptor m_mbinfo;
};

//this structure is one(or couple) for all decoder
class H264DecoderFrame;

class H264DecoderLocalMacroblockDescriptor
{
public:
    // Default constructor
    H264DecoderLocalMacroblockDescriptor(void);
    // Destructor
    ~H264DecoderLocalMacroblockDescriptor(void);

    // Allocate decoding data
    bool Allocate(Ipp32s iMBCount, MemoryAllocator *pMemoryAllocator);

    H264DecoderMacroblockMVs *(MVDeltas[2]);                    // (H264DecoderMacroblockMVs * ([])) motionVectors Deltas L0 and L1
    H264DecoderMacroblockCoeffsInfo *MacroblockCoeffsInfo;      // (H264DecoderMacroblockCoeffsInfo *) info about num_coeffs in each block in the current  picture
    H264DecoderMacroblockLocalInfo *mbs;                        // (H264DecoderMacroblockLocalInfo *) reconstuction info
    H264DecoderMBAddr *active_next_mb_table;                    // (H264DecoderMBAddr *) current "next addres" table

    // Assignment operator
    H264DecoderLocalMacroblockDescriptor &operator = (H264DecoderLocalMacroblockDescriptor &);

    bool m_isBusy;
    H264DecoderFrame * m_pFrame;

protected:
    // Release object
    void Release(void);

    Ipp8u *m_pAllocated;                                        // (Ipp8u *) pointer to allocated memory
    MemID m_midAllocated;                                       // (MemID) mem id of allocated memory
    size_t m_nAllocatedSize;                                    // (size_t) size of allocated memory

    MemoryAllocator *m_pMemoryAllocator;                        // (MemoryAllocator *) pointer to memory management tool
};

#define INLINE inline
// __forceinline

INLINE H264DecoderMacroblockMVs * GetMVs(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum) {return &gmbinfo->MV[list][mbNum];}

INLINE H264DecoderMotionVector * GetMVDelta(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum) {return gmbinfo->MV[2 + list][mbNum].MotionVectors;}

INLINE H264DecoderMotionVector & GetMV(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum, Ipp32s blockNum) {return gmbinfo->MV[list][mbNum].MotionVectors[blockNum];}


INLINE NumCoeffsType * GetNumCoeffs(H264DecoderLocalMacroblockDescriptor *lmbinfo, Ipp32s mbNum) {return lmbinfo->MacroblockCoeffsInfo[mbNum].numCoeffs;}

INLINE const NumCoeffsType & GetNumCoeff(H264DecoderLocalMacroblockDescriptor *lmbinfo, Ipp32s mbNum, Ipp32s block) {return lmbinfo->MacroblockCoeffsInfo[mbNum].numCoeffs[block];}


INLINE RefIndexType * GetRefIdxs(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum) {return gmbinfo->mbs[mbNum].refIdxs[list].refIndexs;}

INLINE const RefIndexType & GetRefIdx(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum, Ipp32s block) {return gmbinfo->mbs[mbNum].refIdxs[list].refIndexs[block];}

INLINE const RefIndexType * GetReferenceIndexPtr(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum, Ipp32s block)
{
    return &gmbinfo->mbs[mbNum].refIdxs[list].refIndexs[subblock_block_membership[block]];
}

INLINE const RefIndexType & GetReferenceIndex(H264DecoderGlobalMacroblocksDescriptor *gmbinfo, Ipp32s list, Ipp32s mbNum, Ipp32s block)
{
    return *GetReferenceIndexPtr(gmbinfo, list, mbNum, block);
}


INLINE RefIndexType GetReferenceIndex(RefIndexType *refIndxs, Ipp32s block)
{
    return refIndxs[subblock_block_membership[block]];
}
/*
inline RefIndexType* GetReferenceIndexPtr(RefIndexType *refIndxs, Ipp32s block)
{
    return &(refIndxs[subblock_block_membership[block]]);
}*/

class Macroblock
{
    Macroblock(Ipp32u mbNum, H264DecoderGlobalMacroblocksDescriptor *gmbinfo, H264DecoderLocalMacroblockDescriptor *lmbinfo)
    {
        GlobalMacroblockInfo = &gmbinfo->mbs[mbNum];
        LocalMacroblockInfo = &lmbinfo->mbs[mbNum];
        MacroblockCoeffsInfo = &lmbinfo->MacroblockCoeffsInfo[mbNum];
        /*MVs[0] = &gmbinfo->MV[0][mbNum];
        MVs[1] = &gmbinfo->MV[1][mbNum];
        MVDelta[0] = &lmbinfo->MVDeltas[0][mbNum];
        MVDelta[1] = &lmbinfo->MVDeltas[1][mbNum];*/
        //RefIdxs[0] = &gmbinfo->refIdxs[0][mbNum];
        //RefIdxs[1] = &gmbinfo->refIdxs[1][mbNum];
    }

    inline H264DecoderMacroblockMVs * GetMV(Ipp32s list) {return MVs[list];}

    inline H264DecoderMacroblockMVs * GetMVDelta(Ipp32s list) {return MVs[2 + list];}

    inline H264DecoderMacroblockRefIdxs * GetRefIdx(Ipp32s list) {return RefIdxs[list];}

    inline H264DecoderMacroblockGlobalInfo * GetGlobalInfo() {return GlobalMacroblockInfo;}

    inline H264DecoderMacroblockLocalInfo * GetLocalInfo() {return LocalMacroblockInfo;}

private:

    H264DecoderMacroblockMVs *MVs[4];//MV L0,L1, MVDeltas 0,1
    H264DecoderMacroblockRefIdxs *RefIdxs[2];//RefIdx L0, L1
    H264DecoderMacroblockCoeffsInfo *MacroblockCoeffsInfo;
    H264DecoderMacroblockGlobalInfo *GlobalMacroblockInfo;
    H264DecoderMacroblockLocalInfo *LocalMacroblockInfo;
};

struct H264DecoderCurrentMacroblockDescriptor
{
    H264DecoderMacroblockMVs *MVs[2];//MV L0,L1,
    H264DecoderMacroblockMVs *MVDelta[2];//MVDeltas L0,L1
    H264DecoderMacroblockNeighboursInfo CurrentMacroblockNeighbours;//mb neighboring info
    H264DecoderBlockNeighboursInfo CurrentBlockNeighbours;//block neighboring info (if mbaff turned off remained static)
    H264DecoderMacroblockGlobalInfo *GlobalMacroblockInfo;
    H264DecoderMacroblockGlobalInfo *GlobalMacroblockPairInfo;
    H264DecoderMacroblockLocalInfo *LocalMacroblockInfo;
    H264DecoderMacroblockLocalInfo *LocalMacroblockPairInfo;

    H264DecoderMotionVector & GetMV(Ipp32s list, Ipp32s blockNum) {return MVs[list]->MotionVectors[blockNum];}

    H264DecoderMotionVector * GetMVPtr(Ipp32s list, Ipp32s blockNum) {return &MVs[list]->MotionVectors[blockNum];}

    INLINE const RefIndexType & GetRefIdx(Ipp32s list, Ipp32s block) const {return RefIdxs[list]->refIndexs[block];}

    INLINE const RefIndexType & GetReferenceIndex(Ipp32s list, Ipp32s block) const {return RefIdxs[list]->refIndexs[subblock_block_membership[block]];}

    INLINE H264DecoderMacroblockRefIdxs* GetReferenceIndexStruct(Ipp32s list)
    {
        return RefIdxs[list];
    }

    INLINE H264DecoderMacroblockCoeffsInfo * GetNumCoeffs() {return MacroblockCoeffsInfo;}
    INLINE const NumCoeffsType & GetNumCoeff(Ipp32s block) {return MacroblockCoeffsInfo->numCoeffs[block];}

    H264DecoderMacroblockRefIdxs *RefIdxs[2];//RefIdx L0, L1
    H264DecoderMacroblockCoeffsInfo *MacroblockCoeffsInfo;

    bool isInited;
};

struct ReferenceFlags // flags use for reference frames of slice
{
    char field : 3;
    unsigned char isShortReference : 1;
};

inline Ipp32s GetReferenceField(ReferenceFlags *pFields, Ipp32s RefIndex)
{
    /*if (0 <= RefIndex)
    {*/
        VM_ASSERT(pFields[RefIndex].field >= 0);
        return pFields[RefIndex].field;
    /*}
    else
    {
        return -1;
    }*/
} // Ipp32s GetReferenceField(Ipp8s *pFields, Ipp32s RefIndex)


template<class T>
inline void swapValues(T & t1, T & t2)
{
    T temp = t1;
    t1 = t2;
    t2 = temp;
}

template<typename T>
inline void storeInformationInto8x8(T* info, T value)
{
    info[0] = value;
    info[1] = value;
    info[4] = value;
    info[5] = value;
}

template<typename T>
inline void storeStructInformationInto8x8(T* info, const T &value)
{
    info[0] = value;
    info[1] = value;
    info[4] = value;
    info[5] = value;
}

template<typename T>
inline void fill_n(T *first, size_t count, T val)
{   // copy _Val _Count times through [_First, ...)
    for (; 0 < count; --count, ++first)
        *first = val;
}

template<typename T>
inline void fill_struct_n(T *first, size_t count, const T& val)
{   // copy _Val _Count times through [_First, ...)
    for (; 0 < count; --count, ++first)
        *first = val;
}

class h264_exception
{
public:
    h264_exception(Ipp32s status = -1)
        : m_Status(status)
    {
    }

    virtual ~h264_exception()
    {
    }

    Ipp32s GetStatus() const
    {
        return m_Status;
    }

private:
    Ipp32s m_Status;
};


#pragma pack(1)

extern Ipp32s lock_failed;

#pragma pack()

template <typename T>
inline T * h264_new_array_throw(Ipp32s size)
{
    T * t = new T[size];
    if (!t)
        throw h264_exception(UMC_ERR_ALLOC);
    return t;
}

template <typename T>
inline T * h264_new_throw()
{
    T * t = new T();
    if (!t)
        throw h264_exception(UMC_ERR_ALLOC);
    return t;
}

template <typename T, typename T1>
inline T * h264_new_throw_1(T1 t1)
{
    T * t = new T(t1);
    if (!t)
        throw h264_exception(UMC_ERR_ALLOC);
    return t;
}

struct H264IntraTypesProp
{
    Ipp32s m_nSize;                                             // (Ipp32s) size of allocated intra type array
    MemID m_mid;                                                // (MemID) mem id of allocated buffer for intra types

    void Reset(void)
    {
        m_nSize = 0;
        m_mid = 0;
    }
};

inline ColorFormat GetUMCColorFormat(Ipp32s color_format)
{
    ColorFormat format;
    switch(color_format)
    {
    case 0:
        format = GRAY;
        break;
    case 2:
        format = YUV422;
        break;
    case 3:
        format = YUV444;
        break;
    case 1:
    default:
        format = YUV420;
        break;
    }

    return format;
}

inline Ipp32s GetH264ColorFormat(ColorFormat color_format)
{
    Ipp32s format;
    switch(color_format)
    {
    case GRAY:
    case GRAYA:
        format = 0;
        break;
    case YUV422A:
    case YUV422:
        format = 2;
        break;
    case YUV444:
    case YUV444A:
        format = 3;
        break;
    case YUV420:
    case YUV420A:
    case NV12:
    case YV12:
    default:
        format = 1;
        break;
    }

    return format;
}

inline UMC::ColorFormat ConvertColorFormatToAlpha(UMC::ColorFormat cf)
{
    ColorFormat cfAlpha = cf;
    switch(cf)
    {
    case UMC::GRAY:
        cfAlpha = UMC::GRAYA;
        break;
    case UMC::YUV420:
        cfAlpha = UMC::YUV420A;
        break;
    case UMC::YUV422:
        cfAlpha = UMC::YUV422A;
        break;
    case UMC::YUV444:
        cfAlpha = UMC::YUV444A;
        break;
    default:
        break;
    }

    return cfAlpha;
}

inline size_t CalculateSuggestedSize(const H264SeqParamSet * sps)
{
    size_t base_size = sps->frame_width_in_mbs * sps->frame_height_in_mbs * 256;
    size_t size = 0;

    switch (sps->chroma_format_idc)
    {
    case 0:  // YUV400
        size = base_size;
        break;
    case 1:  // YUV420
        size = (base_size * 3) / 2;
        break;
    case 2: // YUV422
        size = base_size + base_size;
        break;
    case 3: // YUV444
        size = base_size + base_size + base_size;
        break;
    };

    return size;
}

} // end namespace UMC

#include "umc_h264_dec_ippwrap.h"

#endif // __UMC_H264_DEC_DEFS_DEC_H__
