/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    MPEG-4 video headers parsing.
//
*/

#ifndef __MP4_VIDEO_PARSER_H__
#define __MP4_VIDEO_PARSER_H__

#include "ippdefs.h"

/* status codes */
typedef enum {
    MP4_STATUS_OK           =  0,   // no error
    MP4_STATUS_NO_MEM       = -1,   // out of memory
    MP4_STATUS_FILE_ERROR   = -2,   // file error
    MP4_STATUS_NOTSUPPORT   = -3,   // not supported mode
    MP4_STATUS_PARSE_ERROR  = -4,   // fail in parse MPEG-4 stream
    MP4_STATUS_ERROR        = -5    // unknown/unspecified error
} mp4_Status;

/* MPEG-4 start code values */
// ISO/IEC 14496-2: table 6-3
enum {
    MP4_VIDEO_OBJECT_MIN_SC       = 0x00,
    MP4_VIDEO_OBJECT_MAX_SC       = 0x1F,
    MP4_VIDEO_OBJECT_LAYER_MIN_SC = 0x20,
    MP4_VIDEO_OBJECT_LAYER_MAX_SC = 0x2F,
    MP4_FGS_BP_MIN_SC             = 0x40,
    MP4_FGS_BP_MAX_SC             = 0x5F,
    MP4_VISUAL_OBJECT_SEQUENCE_SC = 0xB0,
    MP4_VISUAL_OBJECT_SEQUENCE_EC = 0xB1,
    MP4_USER_DATA_SC              = 0xB2,
    MP4_GROUP_OF_VOP_SC           = 0xB3,
    MP4_VIDEO_SESSION_ERROR_SC    = 0xB4,
    MP4_VISUAL_OBJECT_SC          = 0xB5,
    MP4_VIDEO_OBJECT_PLANE_SC     = 0xB6,
    MP4_SLICE_SC                  = 0xB7,
    MP4_EXTENSION_SC              = 0xB8,
    MP4_FGS_VOP_SC                = 0xB9,
    MP4_FBA_OBJECT_SC             = 0xBA,
    MP4_FBA_OBJECT_PLANE_SC       = 0xBB,
    MP4_MESH_OBJECT_SC            = 0xBC,
    MP4_MESH_OBJECT_PLANE_SC      = 0xBD,
    MP4_STILL_TEXTURE_OBJECT_SC   = 0xBE,
    MP4_TEXTURE_SPATIAL_LAYER_SC  = 0xBF,
    MP4_TEXTURE_SNR_LAYER_SC      = 0xC0,
    MP4_TEXTURE_TILE_SC           = 0xC1,
    MP4_TEXTURE_SHAPE_LAYER_SC    = 0xC2,
    MP4_STUFFING_SC               = 0xC3
};

/* MPEG-4 code values */
// ISO/IEC 14496-2:2004 table 6-6
enum {
    MP4_VISUAL_OBJECT_TYPE_VIDEO     = 1,
    MP4_VISUAL_OBJECT_TYPE_TEXTURE   = 2,
    MP4_VISUAL_OBJECT_TYPE_MESH      = 3,
    MP4_VISUAL_OBJECT_TYPE_FBA       = 4,
    MP4_VISUAL_OBJECT_TYPE_3DMESH    = 5
};

// ISO/IEC 14496-2:2004 table 6-8..10
enum {
    MP4_VIDEO_COLORS_FORBIDDEN         = 0,
    MP4_VIDEO_COLORS_ITU_R_BT_709      = 1,
    MP4_VIDEO_COLORS_UNSPECIFIED       = 2,
    MP4_VIDEO_COLORS_RESERVED          = 3,
    MP4_VIDEO_COLORS_ITU_R_BT_470_2_M  = 4,
    MP4_VIDEO_COLORS_ITU_R_BT_470_2_BG = 5,
    MP4_VIDEO_COLORS_SMPTE_170M        = 6,
    MP4_VIDEO_COLORS_SMPTE_240M        = 7,
    MP4_VIDEO_COLORS_GENERIC_FILM      = 8
};

// ISO/IEC 14496-2:2004 table 6.17 (maximum defined video_object_layer_shape_extension)
#define MP4_SHAPE_EXT_NUM 13
// ISO/IEC 14496-2:2004 table 6-14
enum {
    MP4_ASPECT_RATIO_FORBIDDEN  = 0,
    MP4_ASPECT_RATIO_1_1        = 1,
    MP4_ASPECT_RATIO_12_11      = 2,
    MP4_ASPECT_RATIO_10_11      = 3,
    MP4_ASPECT_RATIO_16_11      = 4,
    MP4_ASPECT_RATIO_40_33      = 5,
    MP4_ASPECT_RATIO_EXTPAR     = 15
};
// ITU-T Rec. H.264 200711 table E-1
enum {
    H264_ASPECT_RATIO_UNSPECIFIED  = 0,
    H264_ASPECT_RATIO_1_1          = 1,
    H264_ASPECT_RATIO_12_11        = 2,
    H264_ASPECT_RATIO_10_11        = 3,
    H264_ASPECT_RATIO_16_11        = 4,
    H264_ASPECT_RATIO_40_33        = 5,
    H264_ASPECT_RATIO_24_11        = 6,
    H264_ASPECT_RATIO_20_11        = 7,
    H264_ASPECT_RATIO_32_11        = 8,
    H264_ASPECT_RATIO_80_33        = 9,
    H264_ASPECT_RATIO_18_11        = 10,
    H264_ASPECT_RATIO_15_11        = 11,
    H264_ASPECT_RATIO_64_33        = 12,
    H264_ASPECT_RATIO_160_99       = 13,
    H264_ASPECT_RATIO_4_3          = 14,
    H264_ASPECT_RATIO_3_2          = 15,
    H264_ASPECT_RATIO_2_1          = 16,
    H264_EXTENDED_SAR              = 255
};

// ISO/IEC 14496-2:2004 table 6-15
#define MP4_CHROMA_FORMAT_420    1
// ISO/IEC 14496-2:2004 table 6-16
enum {
    MP4_SHAPE_TYPE_RECTANGULAR  = 0,
    MP4_SHAPE_TYPE_BINARY       = 1,
    MP4_SHAPE_TYPE_BINARYONLY   = 2,
    MP4_SHAPE_TYPE_GRAYSCALE    = 3
};

// ISO/IEC 14496-2:2004 table 6-19
#define MP4_SPRITE_STATIC   1
#define MP4_SPRITE_GMC      2
// ISO/IEC 14496-2:2004 table 6-24
enum {
    MP4_VOP_TYPE_I  = 0,
    MP4_VOP_TYPE_P  = 1,
    MP4_VOP_TYPE_B  = 2,
    MP4_VOP_TYPE_S  = 3
};

// ISO/IEC 14496-2:2004 table 6-26
enum {
    MP4_SPRITE_TRANSMIT_MODE_STOP   = 0,
    MP4_SPRITE_TRANSMIT_MODE_PIECE  = 1,
    MP4_SPRITE_TRANSMIT_MODE_UPDATE = 2,
    MP4_SPRITE_TRANSMIT_MODE_PAUSE  = 3
};

/* Group Of Video Object Plane Info */
struct mp4_GroupOfVideoObjectPlane
{
    Ipp64s      time_code;
    Ipp32s      closed_gov;
    Ipp32s      broken_link;
};

/* Video Object Plane Info */
struct mp4_VideoObjectPlane
{
    Ipp32s      coding_type;
    Ipp32s      modulo_time_base;
    Ipp32s      time_increment;
    Ipp32s      coded;
    Ipp32s      id;                             // verid != 1 (newpred)
    Ipp32s      id_for_prediction_indication;   // verid != 1 (newpred)
    Ipp32s      id_for_prediction;              // verid != 1 (newpred)
    Ipp32s      rounding_type;
    Ipp32s      reduced_resolution;             // verid != 1
    Ipp32s      vop_width;
    Ipp32s      vop_height;
    Ipp32s      vop_horizontal_mc_spatial_ref;
    Ipp32s      vop_vertical_mc_spatial_ref;
    Ipp32s      background_composition;
    Ipp32s      change_conv_ratio_disable;
    Ipp32s      vop_constant_alpha;
    Ipp32s      vop_constant_alpha_value;
    Ipp32s      intra_dc_vlc_thr;
    Ipp32s      top_field_first;
    Ipp32s      alternate_vertical_scan_flag;
    Ipp32s      sprite_transmit_mode;
    Ipp32s      warping_mv_code_du[4];
    Ipp32s      warping_mv_code_dv[4];
    Ipp32s      brightness_change_factor;
    Ipp32s      quant;
    Ipp32s      alpha_quant[3];
    Ipp32s      fcode_forward;
    Ipp32s      fcode_backward;
    Ipp32s      shape_coding_type;
    Ipp32s      load_backward_shape;
    Ipp32s      ref_select_code;
    Ipp32s      dx;
    Ipp32s      dy;
    Ipp32s      quant_scale;
    Ipp32s      macroblock_num;
};

/* mp4_ComplexityEstimation Info */
struct mp4_ComplexityEstimation
{
    Ipp32s      estimation_method;
    Ipp32s      shape_complexity_estimation_disable;
    Ipp32s      opaque;
    Ipp32s      transparent;
    Ipp32s      intra_cae;
    Ipp32s      inter_cae;
    Ipp32s      no_update;
    Ipp32s      upsampling;
    Ipp32s      texture_complexity_estimation_set_1_disable;
    Ipp32s      intra_blocks;
    Ipp32s      inter_blocks;
    Ipp32s      inter4v_blocks;
    Ipp32s      not_coded_blocks;
    Ipp32s      texture_complexity_estimation_set_2_disable;
    Ipp32s      dct_coefs;
    Ipp32s      dct_lines;
    Ipp32s      vlc_symbols;
    Ipp32s      vlc_bits;
    Ipp32s      motion_compensation_complexity_disable;
    Ipp32s      apm;
    Ipp32s      npm;
    Ipp32s      interpolate_mc_q;
    Ipp32s      forw_back_mc_q;
    Ipp32s      halfpel2;
    Ipp32s      halfpel4;
    Ipp32s      version2_complexity_estimation_disable;     // verid != 1
    Ipp32s      sadct;                                      // verid != 1
    Ipp32s      quarterpel;                                 // verid != 1
    Ipp32s      dcecs_opaque;
    Ipp32s      dcecs_transparent;
    Ipp32s      dcecs_intra_cae;
    Ipp32s      dcecs_inter_cae;
    Ipp32s      dcecs_no_update;
    Ipp32s      dcecs_upsampling;
    Ipp32s      dcecs_intra_blocks;
    Ipp32s      dcecs_inter_blocks;
    Ipp32s      dcecs_inter4v_blocks;
    Ipp32s      dcecs_not_coded_blocks;
    Ipp32s      dcecs_dct_coefs;
    Ipp32s      dcecs_dct_lines;
    Ipp32s      dcecs_vlc_symbols;
    Ipp32s      dcecs_vlc_bits;
    Ipp32s      dcecs_apm;
    Ipp32s      dcecs_npm;
    Ipp32s      dcecs_interpolate_mc_q;
    Ipp32s      dcecs_forw_back_mc_q;
    Ipp32s      dcecs_halfpel2;
    Ipp32s      dcecs_halfpel4;
    Ipp32s      dcecs_sadct;                                // verid != 1
    Ipp32s      dcecs_quarterpel;                           // verid != 1
};

/* mp4_Scalability Info */
struct mp4_ScalabilityParameters
{
    Ipp32s      dummy;
};

/* VOLControlParameters Info */
struct mp4_VOLControlParameters
{
    Ipp32s      chroma_format;
    Ipp32s      low_delay;
    Ipp32s      vbv_parameters;
    Ipp32s      bit_rate;
    Ipp32s      vbv_buffer_size;
    Ipp32s      vbv_occupancy;
};

/* Video Object Plane with Ipp16s header Info */
struct mp4_VideoObjectPlaneH263
{
    Ipp32s      temporal_reference;
    Ipp32s      split_screen_indicator;
    Ipp32s      document_camera_indicator;
    Ipp32s      full_picture_freeze_release;
    Ipp32s      source_format;
    Ipp32s      picture_coding_type;
    Ipp32s      vop_quant;
    Ipp32s      gob_number;
    Ipp32s      num_gobs_in_vop;
    Ipp32s      num_macroblocks_in_gob;
    Ipp32s      gob_header_empty;
    Ipp32s      gob_frame_id;
    Ipp32s      quant_scale;
    Ipp32s      num_rows_in_gob;
};

/* Video Object Info */
struct mp4_VideoObject
{
// iso part
    Ipp32s                      id;
    Ipp32s                      short_video_header;
    Ipp32s                      random_accessible_vol;
    Ipp32s                      type_indication;
    Ipp32s                      is_identifier;
    Ipp32s                      verid;
    Ipp32s                      priority;
    Ipp32s                      aspect_ratio_info;
    Ipp32s                      aspect_ratio_info_par_width;
    Ipp32s                      aspect_ratio_info_par_height;
    Ipp32s                      is_vol_control_parameters;
    mp4_VOLControlParameters    VOLControlParameters;
    Ipp32s                      shape;
    Ipp32s                      shape_extension;                // verid != 1
    Ipp32s                      vop_time_increment_resolution;
    Ipp32s                      vop_time_increment_resolution_bits;
    Ipp32s                      fixed_vop_rate;
    Ipp32s                      fixed_vop_time_increment;
    Ipp32s                      width;
    Ipp32s                      height;
    Ipp32s                      interlaced;
    Ipp32s                      obmc_disable;
    Ipp32s                      sprite_enable;                  // if verid != 1 (2 bit GMC is added)
    Ipp32s                      sprite_width;
    Ipp32s                      sprite_height;
    Ipp32s                      sprite_left_coordinate;
    Ipp32s                      sprite_top_coordinate;
    Ipp32s                      sprite_warping_points;
    Ipp32s                      sprite_warping_accuracy;
    Ipp32s                      sprite_brightness_change;
    Ipp32s                      low_latency_sprite_enable;
    Ipp32s                      sadct_disable;                  // verid != 1
    Ipp32s                      not_8_bit;
    Ipp32s                      quant_precision;
    Ipp32s                      bits_per_pixel;
    Ipp32s                      no_gray_quant_update;
    Ipp32s                      composition_method;
    Ipp32s                      linear_composition;
    Ipp32s                      quant_type;
    Ipp32s                      load_intra_quant_mat;
    Ipp32s                      load_nonintra_quant_mat;
    Ipp32s                      load_intra_quant_mat_grayscale[3];
    Ipp32s                      load_nonintra_quant_mat_grayscale[3];
    Ipp32s                      quarter_sample;                 // verid != 1
    Ipp32s                      complexity_estimation_disable;
    mp4_ComplexityEstimation    ComplexityEstimation;
    Ipp32s                      resync_marker_disable;
    Ipp32s                      data_partitioned;
    Ipp32s                      reversible_vlc;
    Ipp32s                      newpred_enable;                 // verid != 1
    Ipp32s                      requested_upstream_message_type;// verid != 1
    Ipp32s                      newpred_segment_type;           // verid != 1
    Ipp32s                      reduced_resolution_vop_enable;  // verid != 1
    Ipp32s                      scalability;
    mp4_ScalabilityParameters   ScalabilityParameters;
    mp4_GroupOfVideoObjectPlane GroupOfVideoObjectPlane;
    mp4_VideoObjectPlane        VideoObjectPlane;
    mp4_VideoObjectPlaneH263    VideoObjectPlaneH263;
};

/* StillTexture Object Info */
struct mp4_StillTextureObject
{
    Ipp32s  dummy;
};

/* Mesh Object Info */
struct mp4_MeshObject
{
    Ipp32s  dummy;
};

/* Face Object Info */
struct mp4_FaceObject
{
    Ipp32s  dummy;
};

/* video_signal_type Info */
struct mp4_VideoSignalType
{
    Ipp32s  is_video_signal_type;
    Ipp32s  video_format;
    Ipp32s  video_range;
    Ipp32s  is_colour_description;
    Ipp32s  colour_primaries;
    Ipp32s  transfer_characteristics;
    Ipp32s  matrix_coefficients;
};

/* Visual Object Info */
struct mp4_VisualObject
{
    Ipp32s                  is_identifier;
    Ipp32s                  verid;
    Ipp32s                  priority;
    Ipp32s                  type;
    mp4_VideoSignalType     VideoSignalType;
    mp4_VideoObject         VideoObject;
    mp4_StillTextureObject  StillTextureObject;
    mp4_MeshObject          MeshObject;
    mp4_FaceObject          FaceObject;
};

/* Full Info */
struct mp4_Info
{
    Ipp8u*      bufptr;         /* current frame, point to header or data */
    Ipp32s      bitoff;         /* mostly point to next frame header or PSC */
    Ipp32s      profile_and_level_indication;
    mp4_VisualObject    VisualObject;
};

extern mp4_Status mp4_Parse_VisualObject(mp4_Info* pInfo);
extern mp4_Status mp4_Parse_VideoObjectPlane(mp4_Info* pInfo);
extern mp4_Status mp4_Parse_VideoObject(mp4_Info* pInfo);
#define mp4_Error(str) vm_debug_trace(VM_DEBUG_ERROR, __VM_STRING(str))

#endif // __MP4_VIDEO_PARSER_H__


