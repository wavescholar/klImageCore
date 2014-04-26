/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    class VideoEncoderMPEG4
//
*/

#ifndef __UMC_MP4_ENC_H__
#define __UMC_MP4_ENC_H__

//#define USE_CV_GME          // use IPP CV for GME (nwp = 2, 3)
//#define USE_ME_SADBUFF      // store calculated SAD to buffer

#include "vm_debug.h"
#include "mp4_enc_bitstream.h"

#include "ippcore.h"
#include "ippvc.h"
#include "ipps.h"
#include "ippi.h"
#ifdef USE_CV_GME
#include "ippcv.h"
#endif

#if defined(WINDOWS)
#pragma warning(disable : 4514)     // unreferenced inline function has been removed
#endif

#if defined(_OPENMP)
#define _OMP_KARABAS
#endif

#if defined(_OMP_KARABAS) && defined(_OPENMP)
#include <omp.h>
#endif // _OMP_KARABAS

namespace MPEG4_ENC
{
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

// ISO/IEC 14496-2:2004 table 6-7
enum {
    MP4_VIDEO_FORMAT_COMPONENT      = 0,
    MP4_VIDEO_FORMAT_PAL            = 1,
    MP4_VIDEO_FORMAT_NTSC           = 2,
    MP4_VIDEO_FORMAT_SECAM          = 3,
    MP4_VIDEO_FORMAT_MAC            = 4,
    MP4_VIDEO_FORMAT_UNSPECIFIED    = 5
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

// ISO/IEC 14496-2:2004 table 6-11
enum {
    MP4_VIDEO_OBJECT_TYPE_SIMPLE                     = 1,
    MP4_VIDEO_OBJECT_TYPE_SIMPLE_SCALABLE            = 2,
    MP4_VIDEO_OBJECT_TYPE_CORE                       = 3,
    MP4_VIDEO_OBJECT_TYPE_MAIN                       = 4,
    MP4_VIDEO_OBJECT_TYPE_NBIT                       = 5,
    MP4_VIDEO_OBJECT_TYPE_2DTEXTURE                  = 6,
    MP4_VIDEO_OBJECT_TYPE_2DMESH                     = 7,
    MP4_VIDEO_OBJECT_TYPE_SIMPLE_FACE                = 8,
    MP4_VIDEO_OBJECT_TYPE_STILL_SCALABLE_TEXTURE     = 9,
    MP4_VIDEO_OBJECT_TYPE_ADVANCED_REAL_TIME_SIMPLE  = 10,
    MP4_VIDEO_OBJECT_TYPE_CORE_SCALABLE              = 11,
    MP4_VIDEO_OBJECT_TYPE_ADVANCED_CODING_EFFICIENCY = 12,
    MP4_VIDEO_OBJECT_TYPE_ADVANCED_SCALABLE_TEXTURE  = 13,
    MP4_VIDEO_OBJECT_TYPE_SIMPLE_FBA                 = 14,
    MP4_VIDEO_OBJECT_TYPE_SIMPLE_STUDIO              = 15,
    MP4_VIDEO_OBJECT_TYPE_CORE_STUDIO                = 16,
    MP4_VIDEO_OBJECT_TYPE_ADVANCED_SIMPLE            = 17,
    MP4_VIDEO_OBJECT_TYPE_FINE_GRANULARITY_SCALABLE  = 18
};

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

/* Block Info */
struct mp4_Block {
    mp4_Block  *predA;
    mp4_Block  *predB;
    mp4_Block  *predC;
    Ipp16s     dct_acA[8];
    Ipp16s     dct_acC[8];
    Ipp16s     dct_dc;
    Ipp16s     dct_dcq;
    Ipp8u      quant;
    Ipp8u      validPredIntra;
};

/* MacroBlock Info */
struct mp4_MacroBlock {
    IppMotionVector mv[4], mvDiff[4], mvT, mvB;
    Ipp32s          lumaErr;
    mp4_Block       block[6];
    Ipp8u           type;
    Ipp8u           not_coded;
    Ipp8u           validPredInter;
    Ipp8u           mcsel;
    Ipp8u           fieldmc;
};

/* MacroBlock Info for MT */
#ifdef _OMP_KARABAS
struct mp4_MacroBlockMT {
    Ipp16s    dctCoeffs[64*6];
    Ipp32s    nzCount[6];
    Ipp32s    pat;
    Ipp32s    ac_pred_flag;
    Ipp32s    dct_type;
    Ipp32s    predDir[6];
    Ipp32s    mb_type;
    IppMotionVector mvForw, mvBack;
    Ipp8u     dummy[8];        // align 16
};
#endif // _OMP_KARABAS

/* Video Object Plane Info */
struct mp4_VideoObjectPlane {
    Ipp32s      vop_coding_type;
    Ipp32s      modulo_time_base;
    Ipp32s      vop_time_increment;
    Ipp32s      vop_coded;
    Ipp32s      vop_id;                             // verid != 1 (newpred)
    Ipp32s      vop_id_for_prediction_indication;   // verid != 1 (newpred)
    Ipp32s      vop_id_for_prediction;              // verid != 1 (newpred)
    Ipp32s      vop_rounding_type;
    Ipp32s      vop_reduced_resolution;             // verid != 1
    Ipp32s      vop_width;
    Ipp32s      vop_height;
    Ipp32s      vop_horizontal_mc_spatial_ref;
    Ipp32s      vop_vertical_mc_spatial_ref;
    Ipp32s      background_composition;
    Ipp32s      change_conv_ratio_disable;
    Ipp32s      vop_constant_alpha;
    Ipp32s      vop_constant_alpha_value;
    // complexity Estimation
    Ipp32s          dcecs_opaque;
    Ipp32s          dcecs_transparent;
    Ipp32s          dcecs_intra_cae;
    Ipp32s          dcecs_inter_cae;
    Ipp32s          dcecs_no_update;
    Ipp32s          dcecs_upsampling;
    Ipp32s          dcecs_intra_blocks;
    Ipp32s          dcecs_inter_blocks;
    Ipp32s          dcecs_inter4v_blocks;
    Ipp32s          dcecs_not_coded_blocks;
    Ipp32s          dcecs_dct_coefs;
    Ipp32s          dcecs_dct_lines;
    Ipp32s          dcecs_vlc_symbols;
    Ipp32s          dcecs_vlc_bits;
    Ipp32s          dcecs_apm;
    Ipp32s          dcecs_npm;
    Ipp32s          dcecs_interpolate_mc_q;
    Ipp32s          dcecs_forw_back_mc_q;
    Ipp32s          dcecs_halfpel2;
    Ipp32s          dcecs_halfpel4;
    Ipp32s          dcecs_sadct;                                // verid != 1
    Ipp32s          dcecs_quarterpel;                           // verid != 1
    Ipp32s      intra_dc_vlc_thr;
    Ipp32s      top_field_first;
    Ipp32s      alternate_vertical_scan_flag;
    Ipp32s      sprite_transmit_mode;
    Ipp32s      warping_mv_code_du[4];
    Ipp32s      warping_mv_code_dv[4];
    Ipp32s      brightness_change_factor;
    Ipp32s      vop_quant;
    Ipp32s      vop_alpha_quant;
    Ipp32s      vop_fcode_forward;
    Ipp32s      vop_fcode_backward;
    Ipp32s      vop_shape_coding_type;
    //          scalability info is absent
    // short header
    Ipp32s      temporal_reference;
    Ipp32s      temporal_reference_increment;
    Ipp32s      split_screen_indicator;
    Ipp32s      document_camera_indicator;
    Ipp32s      full_picture_freeze_release;
    Ipp32s      source_format;
    Ipp32s      picture_coding_type;
    Ipp32s      gob_number;
    Ipp32s      num_gobs_in_vop;
    Ipp32s      num_macroblocks_in_gob;
    Ipp32s      gob_header_empty;
    Ipp32s      gob_frame_id;
    Ipp32s      quant_scale;
};

/* Video Object Info */
struct mp4_VideoObjectLayer {
// iso part
    Ipp32s      video_object_layer_id;
    Ipp32s      short_video_header;
    Ipp32s      random_accessible_vol;
    Ipp32s      video_object_type_indication;
    Ipp32s      is_object_layer_identifier;
    Ipp32s          video_object_layer_verid;
    Ipp32s          video_object_layer_priority;
    Ipp32s      aspect_ratio_info;
    Ipp32s          par_width;
    Ipp32s          par_height;
    Ipp32s      vol_control_parameters;
    Ipp32s          chroma_format;
    Ipp32s          low_delay;
    Ipp32s          vbv_parameters;
    Ipp32s              first_half_bit_rate;
    Ipp32s              latter_half_bit_rate;
    Ipp32s              first_half_vbv_buffer_size;
    Ipp32s              latter_half_vbv_buffer_size;
    Ipp32s              first_half_vbv_occupancy;
    Ipp32s              latter_half_vbv_occupancy;
    Ipp32s      video_object_layer_shape;
    Ipp32s      video_object_layer_shape_extension; // verid != 1
    Ipp32s      vop_time_increment_resolution;
    Ipp32s      vop_time_increment_resolution_bits;
    Ipp32s      fixed_vop_rate;
    Ipp32s          fixed_vop_time_increment;
    Ipp32s      video_object_layer_width;
    Ipp32s      video_object_layer_height;
    Ipp32s      interlaced;
    Ipp32s      obmc_disable;
    Ipp32s      sprite_enable;                  // if verid != 1 (2 bit GMC is added)
    Ipp32s      sprite_width;
    Ipp32s      sprite_height;
    Ipp32s      sprite_left_coordinate;
    Ipp32s      sprite_top_coordinate;
    Ipp32s      no_of_sprite_warping_points;
    Ipp32s      sprite_warping_accuracy;
    Ipp32s      sprite_brightness_change;
    Ipp32s      low_latency_sprite_enable;
    Ipp32s      sadct_disable;                  // verid != 1
    Ipp32s      not_8_bit;
    Ipp32s          quant_precision;
    Ipp32s          bits_per_pixel;
    Ipp32s      no_gray_quant_update;
    Ipp32s      composition_method;
    Ipp32s      linear_composition;
    Ipp32s      quant_type;
    Ipp32s      load_intra_quant_mat;
    Ipp32s          load_intra_quant_mat_len;
    Ipp8u           intra_quant_mat[64];
    Ipp32s      load_nonintra_quant_mat;
    Ipp32s          load_nonintra_quant_mat_len;
    Ipp8u           nonintra_quant_mat[64];
    Ipp32s      quarter_sample;                 // verid != 1
    Ipp32s      complexity_estimation_disable;
    Ipp32s          estimation_method;
    Ipp32s          shape_complexity_estimation_disable;
    Ipp32s              opaque;
    Ipp32s              transparent;
    Ipp32s              intra_cae;
    Ipp32s              inter_cae;
    Ipp32s              no_update;
    Ipp32s              upsampling;
    Ipp32s          texture_complexity_estimation_set_1_disable;
    Ipp32s              intra_blocks;
    Ipp32s              inter_blocks;
    Ipp32s              inter4v_blocks;
    Ipp32s              not_coded_blocks;
    Ipp32s          texture_complexity_estimation_set_2_disable;
    Ipp32s              dct_coefs;
    Ipp32s              dct_lines;
    Ipp32s              vlc_symbols;
    Ipp32s              vlc_bits;
    Ipp32s          motion_compensation_complexity_disable;
    Ipp32s              apm;
    Ipp32s              npm;
    Ipp32s              interpolate_mc_q;
    Ipp32s              forw_back_mc_q;
    Ipp32s              halfpel2;
    Ipp32s              halfpel4;
    Ipp32s          version2_complexity_estimation_disable;     // verid != 1
    Ipp32s              sadct;                                  // verid != 1
    Ipp32s              quarterpel;                             // verid != 1
    Ipp32s      resync_marker_disable;
    Ipp32s      data_partitioned;
    Ipp32s          reversible_vlc;
    Ipp32s      newpred_enable;                 // verid != 1
    Ipp32s          requested_upstream_message_type;// verid != 1
    Ipp32s          newpred_segment_type;           // verid != 1
    Ipp32s      reduced_resolution_vop_enable;  // verid != 1
    Ipp32s      scalability;
    Ipp32s          hierarchy_type;
    Ipp32s          ref_layer_id;
    Ipp32s          ref_layer_sampling_direct;
    Ipp32s          hor_sampling_factor_n;
    Ipp32s          hor_sampling_factor_m;
    Ipp32s          vert_sampling_factor_n;
    Ipp32s          vert_sampling_factor_m;
    Ipp32s          enhancement_type;
    Ipp32s          use_ref_shape;
    Ipp32s          use_ref_texture;
    Ipp32s          shape_hor_sampling_factor_n;
    Ipp32s          shape_hor_sampling_factor_m;
    Ipp32s          shape_vert_sampling_factor_n;
    Ipp32s          shape_vert_sampling_factor_m;
};

/* Group Of Video Object Plane Info */
struct mp4_GroupOfVideoObjectPlane {
    Ipp32s      time_code;
    Ipp32s      closed_gov;
    Ipp32s      broken_link;
};

/* StillTexture Object Info */
struct mp4_StillTextureObject {
    Ipp32s  dummy;
};

/* Mesh Object Info */
struct mp4_MeshObject {
    Ipp32s  dummy;
};

/* Face Object Info */
struct mp4_FaceObject {
    Ipp32s  dummy;
};

/* Visual Object Info */
struct mp4_VisualObject {
    Ipp32s      is_visual_object_identifier;
    Ipp32s      visual_object_verid;
    Ipp32s      visual_object_priority;
    Ipp32s      visual_object_type;
    Ipp32s      video_signal_type;
    Ipp32s      video_format;
    Ipp32s      video_range;
    Ipp32s      colour_description;
    Ipp32s      colour_primaries;
    Ipp32s      transfer_characteristics;
    Ipp32s      matrix_coefficients;
};

/* Visual Object Sequence Info */
struct mp4_VisualObjectSequence {
    Ipp32s                  profile_and_level_indication;
};

struct mp4_Param
{
    Ipp32s      Width;
    Ipp32s      Height;
    Ipp32s      NumOfFrames;
    Ipp32s      TimeResolution;
    Ipp32s      TimeIncrement;
    Ipp32s      short_video_header;
    Ipp32s      load_intra_quant_mat;
    Ipp32s      load_intra_quant_mat_len;
    Ipp8u       intra_quant_mat[64];
    Ipp32s      load_nonintra_quant_mat;
    Ipp32s      load_nonintra_quant_mat_len;
    Ipp8u       nonintra_quant_mat[64];
    Ipp32s      quant_type;
    Ipp32s      quantIVOP, quantPVOP, quantBVOP;
    Ipp32s      IVOPdist, BVOPdist;
    Ipp32s      PVOPsearchWidth, PVOPsearchHeight;
    Ipp32s      BVOPsearchWidthForw, BVOPsearchHeightForw;
    Ipp32s      BVOPsearchWidthBack, BVOPsearchHeightBack;
    Ipp32s      MEalgorithm;
    Ipp32s      MEaccuracy;
    Ipp32s      ME4mv;
    Ipp32s      obmc_disable;
    Ipp32s      RoundingControl;
    Ipp32s      calcPSNR;
    Ipp32s      insertGOV;
    Ipp32s      repeatHeaders;
    Ipp32s      resync;
    Ipp32s      VideoPacketLenght;
    Ipp32s      data_partitioned;
    Ipp32s      reversible_vlc;
    Ipp32s      interlaced;
    Ipp32s      top_field_first;
    Ipp32s      alternate_vertical_scan_flag;
    Ipp32s      interlacedME;
    Ipp32s      sprite_enable;
    Ipp32s      sprite_width;
    Ipp32s      sprite_height;
    Ipp32s      sprite_left_coordinate;
    Ipp32s      sprite_top_coordinate;
    Ipp32s      no_of_sprite_warping_points;
    Ipp32s      sprite_warping_accuracy;
    Ipp32s      sprite_brightness_change;
    Ipp32s      *warping_mv_code_du;
    Ipp32s      *warping_mv_code_dv;
    Ipp32s      *brightness_change_factor;
    Ipp8u       *bsBuffer;
    Ipp32s       bsBuffSize;
    Ipp32s       padType;
    Ipp32s       RateControl;
    Ipp32s       BitRate;
    Ipp32s       SceneChangeThreshold;
    Ipp32s       numThreads;
    Ipp8u        profile_and_level;
    Ipp8u        aspect_ratio_width;
    Ipp8u        aspect_ratio_height;
    mp4_Param();
};

struct mp4_VLC {
    Ipp32s  code;
    Ipp32s  len;
};

struct mp4_VLC_TCOEF {
    int           runMax[2];
    const int     *runOff[2];
    const int     *levMax[2];
    const mp4_VLC *vlc;
};

struct mp4_Frame {
    Ipp8u*      ap;
    Ipp8u*      pY;
    Ipp8u*      pU;
    Ipp8u*      pV;
    Ipp32s      type;
    Ipp32u      mid;
    Ipp64s      time;
};

#if defined (_WIN32_WCE) && defined (_M_IX86) && defined (__stdcall)
  #define _IPP_STDCALL_CDECL
  #undef __stdcall
#endif

typedef IppStatus (__STDCALL *ippSAD_func) (const Ipp8u* pSrcCur, Ipp32s srcCurStep, const Ipp8u* pSrcRef, Ipp32s srcRefStep, Ipp32s* pDst, Ipp32s mcType);
typedef IppStatus (__STDCALL *ippCopy_func) (const Ipp8u* pSrc, Ipp32s srcStep, Ipp8u *pDst, Ipp32s dstStep, Ipp32s acc, Ipp32s rc);

#if defined (_IPP_STDCALL_CDECL)
  #undef  _IPP_STDCALL_CDECL
  #define __stdcall __cdecl
#endif

struct mp4_MB_RC {
    Ipp32s  bpuAvg, bpsAvg, bpsEnc, sQuant;
    Ipp8u  *sBitPtr, *sBitPtr_1, *sBitPtr_2;
    Ipp32s  sBitOff, sBitOff_1, sBitOff_2;
};

struct mp4_Data_ME {
    mp4_MacroBlock *MBcurr;
    Ipp32s          xMB, yMB;
#ifdef USE_ME_SADBUFF
    Ipp32s         *meBuff;
#endif
    IppMotionVector mvPred[10];
    Ipp32s          numPred;
    Ipp32s          quant, fcode, flags, rt, method;
    Ipp8u           *pYc, *pYr, *pUc, *pUr, *pVc, *pVr;
    Ipp32s          stepL, stepC;
    Ipp32s          xL, xR, yT, yB;
    Ipp32s          thrDiff16x16, thrDiff8x8, thrDiff16x8;
    //
    Ipp32s          bestDiff;
    Ipp32s          thrDiff;
    Ipp32s          xPos, yPos;
    ippSAD_func     sadFunc;
    ippCopy_func    copyQPFunc;
    ippCopy_func    copyHPFunc;
};

struct mp4_Slice {
    Ipp32s          startRow;
    Ipp32s          numRow;
    Ipp32s          gob_number; // for short_video_header
    BitStream    cBS, cBS_1, cBS_2;
    Ipp32s          quantSum;
    Ipp32s          numNotCodedMB;
    Ipp32s          numIntraMB;
    mp4_MB_RC       rc;
#ifdef USE_ME_SADBUFF
    Ipp32s         *meBuff;
#endif
};

#define YUV_CHROMA_420    0
#define YUV_CHROMA_422    1
#define YUV_CHROMA_444    2

enum {
    MP4_STS_NODATA      = -2,
    MP4_STS_BUFFERED    = -1,
    MP4_STS_NOERR       =  0,
    MP4_STS_ERR_NOTINIT =  1,
    MP4_STS_ERR_PARAM   =  2,
    MP4_STS_ERR_NOMEM   =  3,
    MP4_STS_ERR_BUFOVER =  4
};


class MPEG4_RC
{
protected :
    Ipp32s  mBitRate, mBitsDesiredFrame;
    Ipp64s  mBitsEncodedTotal, mBitsDesiredTotal;
    bool    mIsInit;
    Ipp32s  mQuantI, mQuantP, mQuantB, mQuantMax, mQuantMin, mQuantPrev;
    Ipp32s  mMethod, mRCfap, mRCqap, mRCbap, mRCq;
    Ipp64f  mRCqa, mRCfa;
public :
    MPEG4_RC();
    ~MPEG4_RC();
    void    Init(Ipp32s qas, Ipp32s fas, Ipp32s bas, Ipp32s bitRate, Ipp64f fRate, Ipp32s fWidth, Ipp32s fHeight, Ipp32s qMin, Ipp32s qMax);
    void    Close();
    void    PostFrame(Ipp32s frameType, Ipp64s bEncoded);
    Ipp32s  GetQP(Ipp32s frameType);
    void    SetQP(Ipp32s frameType, Ipp32s qp);
    Ipp32s  GetInitQP(Ipp32s fWidth, Ipp32s fHeight);
};


class MPEG4_RC_MB
{
protected :
    Ipp32s  mBitRate, mBitsDesiredFrame;
    Ipp64s  mBitsEncodedTotal, mBitsDesiredTotal;
    bool    mIsInit;
    Ipp32s  mQuant, mQuantMax, mQuantMin, mNumMacroBlockPerRow, mNumMacroBlockPerCol, mIVOPdist, mDP;
    Ipp32s  GetInitQP(Ipp32s bitRate, Ipp64f frameRate, Ipp32s fWidth, Ipp32s fHeight);
public :
    MPEG4_RC_MB();
    ~MPEG4_RC_MB();
    void    Init(Ipp32s bitRate, Ipp64f fRate, Ipp32s fWidth, Ipp32s fHeight, Ipp32s mbPerRow, Ipp32s mbPerCol, Ipp32s keyInt, Ipp32s dp, Ipp32s qMin, Ipp32s qMax);
    void    Close();
    Ipp32s  GetQP();
    void    Start(mp4_Slice *slice, Ipp32s frameType, Ipp32s quant);
    void    Update(mp4_Slice *slice, int *dquant, Ipp32s frameType);
    void    PostFrame(Ipp32s bpfEncoded, Ipp32s quantSum);
};


class VideoEncoderMPEG4
{
protected :
#ifdef _OMP_KARABAS
    mp4_MacroBlockMT *MBinfoMT;
    volatile Ipp32s* volatile mCurRowMT;
    Ipp8u      *mBuffer;
    Ipp32s      mThreadingAlg;
    mp4_Slice  *mSliceMT;
#ifdef _OPENMP
    omp_lock_t *mLockMT;
#endif
#endif // _OMP_KARABAS
    Ipp32s      mNumThreads;
    bool        mIsInit;
    BitStream cBS;
    Ipp32s      mbsAlloc;
    Ipp32s      mSourceFormat;          // 4:2:0 supported only
    Ipp32s      mSourceWidth;           // width
    Ipp32s      mSourceHeight;          // height
    Ipp32s      mNumOfFrames;
    Ipp32s      mFrameCount;
    Ipp32s      mExpandSize;
    Ipp32s      mNumMacroBlockPerRow;
    Ipp32s      mNumMacroBlockPerCol;
    Ipp32s      mNumMacroBlockPerVOP;
    Ipp32s      mQuantIVOP, mQuantPVOP, mQuantBVOP, mQuantPrev;
    Ipp32s      mIVOPdist, mBVOPdist, mLastIVOP;
    Ipp32s      mPVOPsearchHor, mPVOPsearchVer;
    Ipp32s      mBVOPsearchHorForw, mBVOPsearchVerForw;
    Ipp32s      mBVOPsearchHorBack, mBVOPsearchVerBack;
    Ipp32s      mPVOPfcodeForw;
    Ipp32s      mBVOPfcodeForw;
    Ipp32s      mBVOPfcodeBack;
    Ipp16s      mDefDC;
    Ipp32s      mMEflags, mMEmethod;
#ifdef USE_ME_SADBUFF
    Ipp32s      *mMEfastSAD;
    Ipp32s      mMEfastSADsize;
#endif
    Ipp32s      mRoundingControl;
    Ipp32s      mReconstructAlways;
    Ipp32s      mMBNlength;
    Ipp32s      mVideoPacketLength;
    Ipp64s      mVOPtime, mSyncTime;
    Ipp32s      mNumBVOP, mIndxBVOP, mTRB, mTRD, mTframe, mSyncTimeB;
    Ipp32s      mPadType;
    Ipp32s      mInsertGOV;
    Ipp32s      mInsertResync;
    Ipp32s      mRepeatHeaders;
    Ipp32s      mRateControl, mBitRate, mBitsEncodedFrame;
    Ipp64s      mBitsEncodedTotal;
    MPEG4_RC    mRC;
    MPEG4_RC_MB mRC_MB;
    Ipp32s      mSceneChangeThreshold, mNumIntraMB, mNumNotCodedMB;
    Ipp32s      mQuantSum;
    mp4_Frame  *mFrameC, *mFrameF, *mFrameB, *mFrameD;
    mp4_Slice   mSlice;
    mp4_VisualObjectSequence    VOS;
    mp4_VisualObject            VO;
    mp4_VideoObjectLayer        VOL;
    mp4_GroupOfVideoObjectPlane GOV;
    mp4_VideoObjectPlane        VOP;
    // info for each macroblock
    mp4_MacroBlock              *MBinfo;
    // additional buffers for data_partitioned
    Ipp8u                       *mBuffer_1;
    Ipp8u                       *mBuffer_2;
    // ippVC specs
    IppiWarpSpec_MPEG4          *mWarpSpec;
    IppiQuantInterSpec_MPEG4    *mQuantInterSpec;
    IppiQuantIntraSpec_MPEG4    *mQuantIntraSpec;
    IppiQuantInvInterSpec_MPEG4 *mQuantInvInterSpec;
    IppiQuantInvIntraSpec_MPEG4 *mQuantInvIntraSpec;
    // buffer for GMC predict of the row of macroblocks
    Ipp8u                       *mGMCPredY, *mGMCPredU, *mGMCPredV;
#ifdef USE_CV_GME
// GMC pyramids
    IppiPyramid                 *mPyramid[2], *mPyrC, *mPyrR;
    Ipp32s                       mPyrLevel, mOptFlowNumPoint, mOptFlowNumPointX, mOptFlowNumPointY, mOptFlowWinSize;
    IppiOptFlowPyrLK_8u_C1R     *mOptFlowState;
    IppiPoint_32f               *mOptFlowPoints, *mOptFlowPtC, *mOptFlowPtR;
    Ipp8s                       *mOptFlowPtSts, *mOptFlowMask;
    Ipp32f                      *mOptFlowPatchDiff;
#endif
    // buffers for RPT support
    Ipp8u                       *mMBquant;
    IppMotionVector             *mMBpredMV;
    Ipp32u                      *mMBpos;
public :
    Ipp32s      mLumaPlaneSize;
    Ipp32s      mChromaPlaneSize;
    Ipp32s      mPlanes;
    Ipp32s      mExpandSizeA;
    Ipp32s      mStepLuma;
    Ipp32s      mStepChroma;
    mp4_Frame  *mFrame;
    int     Init(mp4_Param *par);
    void    Close();
    VideoEncoderMPEG4() { mIsInit = false; };
    VideoEncoderMPEG4(mp4_Param *par) { mIsInit = false;  Init(par); };
    ~VideoEncoderMPEG4() { Close(); };
    Ipp32s  EncodeHeader();
    Ipp32s  EncodeFrame(Ipp32s noMoreData);
    void    InitBuffer(Ipp8u *ptr, Ipp32s size) { cBS.Init(ptr, size); };
    Ipp8u*  GetBufferPtr() { return cBS.GetPtr(); };
    Ipp32s  GetBufferFullness() { return cBS.GetFullness(); };
    void    GetBufferPos(Ipp8u **ptr, Ipp32s *bitOff) { cBS.GetPos(ptr, bitOff); };
    void    ResetBuffer() { cBS.Reset(); };
    Ipp32s  GetFrameQuantSum() { return mQuantSum; };
    Ipp32s  GetFrameMacroBlockPerRow() { return mNumMacroBlockPerRow; };
    Ipp32s  GetFrameMacroBlockPerCol() { return mNumMacroBlockPerCol; };
    Ipp32s  GetFrameType() { return VOL.short_video_header ? VOP.picture_coding_type : VOP.vop_coding_type; };
    Ipp32s  GetCurrentFrameInfo(Ipp8u **pY, Ipp8u **pU, Ipp8u **pV, Ipp32s *stepLuma, Ipp32s *stepChroma);
    Ipp32s  GetDecodedFrameInfo(Ipp8u **pY, Ipp8u **pU, Ipp8u **pV, Ipp32s *stepLuma, Ipp32s *stepChroma);
    Ipp64s  GetFrameBytesEncoded() { return GetBufferFullness(); };
    Ipp32s  SetFrameBasicSpriteWarpCoeffs(Ipp32s *du, Ipp32s *dv, Ipp32s bcf);
    // for RTP support
    Ipp8u*  GetFrameQuant() { return mMBquant; };
    IppMotionVector*  GetFrameMVpred() { return mMBpredMV; };
    Ipp32u*  GetFrameMBpos() { return mMBpos; };
    void    ResetRC(Ipp32s bitRate, Ipp64f frameRate);
protected :
    void    EncodeStartCode(Ipp8u sc);
    void    EncodeZeroBitsAlign();
    void    EncodeStuffingBitsAlign();
    void    EncodeVideoPacketHeader(BitStream &cBS, Ipp32s mbn, Ipp32s quant);
    void    EncodeGOBHeader(BitStream &cBS, Ipp32s gob_number, Ipp32s quant);
    void    EncodeVOS_Header();
    void    EncodeVO_Header();
    void    EncodeVOL_Header();
    void    EncodeGOV_Header();
    void    EncodeVOP_Header();
    void    EncodeVOPSH_Header();
    void    EncodeIVOP();
    void    EncodePVOP();
    void    EncodeBVOP();
    void    EncodeSVOP();
    void    EncodeIVOPSH();
    void    EncodePVOPSH();
    void    ExpandFrame(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV);
    void    PadFrame(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV);
    void    PredictMV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred);
    void    Predict1MV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred);
    void    Predict3MV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred, IppMotionVector *mvCurr);
    void    Predict3MV(mp4_MacroBlock *MBcurr, Ipp32s i, Ipp32s j, IppMotionVector *mvPred, Ipp32s nB);
    void    PredictIntraDCAC(mp4_MacroBlock *MBcurr, Ipp16s *dcCurr, Ipp32s quant, Ipp32s *predictDir, Ipp32s predAC, Ipp32s *pSum0, Ipp32s *pSum1, Ipp32s *nzCount, Ipp32s nRow);
    Ipp32s  TransMacroBlockIntra_H263(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s *nzCount, Ipp32s quant);
    Ipp32s  TransMacroBlockInter_H263(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s *nzCount, Ipp32s quant, Ipp8u *mcPred, Ipp32s lumaErr);
    int     TransMacroBlockIntra_MPEG4(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s *nzCount, int quant, int row, int col, int *dct_type, int use_intra_dc_vlc, mp4_MacroBlock *MBcurr, int *predDir, int startRow, int *ac_pred, int *pat, int *costRD);
    int     TransMacroBlockInter_MPEG4(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp16s *coeffMB, Ipp32s *nzCount, int quant, Ipp8u *mcPred, int row, int col, int *dct_type, int trellis, int *costRD);
    void    ReconMacroBlockNotCoded(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp8u *mcPred);
    void    ReconMacroBlockIntra_H263(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s quant, Ipp32s pattern);
    void    ReconMacroBlockInter_H263(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp8u *mcPred, Ipp16s *coeffMB, Ipp32s quant, Ipp32s pattern);
    void    ReconMacroBlockIntra_MPEG4(Ipp8u *pY, Ipp8u *pU, Ipp8u *pV, Ipp16s *coeffMB, Ipp32s quant, mp4_MacroBlock *MBcurr, Ipp32s pattern, Ipp32s dct_type);
    void    ReconMacroBlockInter_MPEG4(Ipp8u *pYc, Ipp8u *pUc, Ipp8u *pVc, Ipp8u *mcPred, Ipp16s *coeffMB, Ipp32s quant, Ipp32s pattern, Ipp32s dct_type);
    void    ME_MacroBlock_PSH(mp4_Data_ME *meData, Ipp32s startRow, Ipp32s endRow);
    void    ME_MacroBlock_P(mp4_Data_ME *meData, Ipp32s startRow, Ipp32s endRow);
    void    ME_MacroBlock_S(mp4_Data_ME *meData, Ipp32s startRow, Ipp32s endRow);
    void    ME_Slice(mp4_Slice *slice);
    void    EncodeISliceSH(mp4_Slice *slice);
    void    EncodePSliceSH(mp4_Slice *slice);
    void    EncodeISlice(mp4_Slice *slice);
    void    EncodePSlice(mp4_Slice *slice);
    void    EncodeSSlice(mp4_Slice *slice);
    void    EncodeBSlice(mp4_Slice *slice);
    bool    FindTransformGMC();
#ifdef USE_CV_GME
    void    PyramidCalc(const Ipp8u* img, IppiPyramid* pyramid);
#endif
    void    AdjustSearchRange();
    void    LimitMV(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
    void    LimitMVQ(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
    void    Limit4MV(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
    void    Limit4MVQ(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
    void    LimitFMV(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
    void    LimitFMVQ(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
    void    LimitCMV(IppMotionVector *pMV, Ipp32s x, Ipp32s y);
#ifdef _OMP_KARABAS
    // for threading method 0
    void    EncodeIRowSH(Ipp32s curRow, Ipp32s threadNum, Ipp32s *nmb);
    void    EncodePRowSH(Ipp32s curRow, Ipp32s threadNum, Ipp32s *nmb);
    void    EncodeIRow(Ipp32s curRow, Ipp32s threadNum);
    void    EncodePRow(Ipp32s curRow, Ipp32s threadNum, Ipp32s *numNotCodedMB);
    void    EncodeBRow(Ipp32s curRow, Ipp32s threadNum);
    void    EncodeSRow(Ipp32s curRow, Ipp32s threadNum);
    void    ME_VOP();
#endif // _OMP_KARABAS
};

#if defined(__INTEL_COMPILER) && !defined(_WIN32_WCE)
    #define __ALIGN16(type, name, size) \
        __declspec (align(16)) type name[size]
#else
    #if defined INTEL64
        #define __ALIGN16(type, name, size) \
            Ipp8u _a16_##name[(size)*sizeof(type)+15]; type *name = (type*)(((Ipp64s)(_a16_##name) + 15) & ~15)
    #else
        #define __ALIGN16(type, name, size) \
            Ipp8u _a16_##name[(size)*sizeof(type)+15]; type *name = (type*)(((Ipp32s)(_a16_##name) + 15) & ~15)
    #endif
#endif

#define mp4_Zero4MV(mv) (mv)[0].dx = (mv)[0].dy = (mv)[1].dx = (mv)[1].dy =(mv)[2].dx = (mv)[2].dy =(mv)[3].dx = (mv)[3].dy = 0
#define mp4_Div2(a) ((a) >= 0 ? ((a) >> 1) : (((a)+1) >> 1))
#define mp4_Div2Round(a) (((a) >> 1) | ((a) & 1))
#define mp4_DivRoundInf(a, b) ((((a) + (((a) >= 0) ? ((b) >> 1) : -((b) >> 1))) / (b)))
#define mp4_Clip(a, l, r) if (a < (l)) a = l; else if (a > (r)) a = r;
#define mp4_ClipL(a, l) if (a < (l)) a = l;
#define mp4_ClipR(a, r) if (a > (r)) a = r;
#define mp4_Abs(a) ((a) >= 0 ? (a) : -(a))
#define mp4_GetNumBits(oPtr, oOff, nPtr, nOff) (Ipp32s)(((nPtr) - (oPtr)) << 3) + (nOff) - (oOff)

#define USE_TABLE_INTRA_DIV

#ifndef USE_TABLE_INTRA_DIV
#define mp4_DivIntraDC(a, b) (((a) + ((b) >> 1)) / (b))
#define mp4_DivIntraAC(a, b) mp4_DivRoundInf(a, b)
#else
// tested on (-2047..2047) // (1..46)
#define mp4_DivIntraDC(a, b) (((a) * mp4_DivIntraDivisor[b] + (1 << 17)) >> 18)
#define mp4_DivIntraAC(a, b) mp4_DivIntraDC(a, b)
#endif

#define MP4_MV_OFF_HP(dx, dy, step) \
    (((dx) >> 1) + (step) * ((dy) >> 1))

#define MP4_MV_ACC_HP(dx, dy) \
    ((((dy) & 1) << 1) + ((dx) & 1))

#define MP4_MV_OFF_QP(dx, dy, step) \
    (((dx) >> 2) + (step) * ((dy) >> 2))

#define MP4_MV_ACC_QP(dx, dy) \
    ((((dy) & 3) << 2) + ((dx) & 3))

#define MP4_MV_ACC_HP_SAD(dx, dy) \
    ((((dx) & 1) << 3) + (((dy) & 1) << 2))

#define mp4_Copy8x4HP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy8x4HP_8u_C1R(pSrc + MP4_MV_OFF_HP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_HP((mv)->dx, (mv)->dy), rc)

#define mp4_Copy8x8HP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy8x8HP_8u_C1R(pSrc + MP4_MV_OFF_HP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_HP((mv)->dx, (mv)->dy), rc)

#define mp4_Copy16x8HP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy16x8HP_8u_C1R(pSrc + MP4_MV_OFF_HP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_HP((mv)->dx, (mv)->dy), rc)

#define mp4_Copy16x16HP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy16x16HP_8u_C1R(pSrc + MP4_MV_OFF_HP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_HP((mv)->dx, (mv)->dy), rc)

#define mp4_Copy8x8QP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy8x8QP_MPEG4_8u_C1R(pSrc + MP4_MV_OFF_QP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_QP((mv)->dx, (mv)->dy), rc)

#define mp4_Copy16x8QP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy16x8QP_MPEG4_8u_C1R(pSrc + MP4_MV_OFF_QP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_QP((mv)->dx, (mv)->dy), rc)

#define mp4_Copy16x16QP_8u(pSrc, srcStep, pDst, dstStep, mv, rc) \
    ippiCopy16x16QP_MPEG4_8u_C1R(pSrc + MP4_MV_OFF_QP((mv)->dx, (mv)->dy, srcStep), srcStep, pDst, dstStep, MP4_MV_ACC_QP((mv)->dx, (mv)->dy), rc)

#define mp4_Add8x8_16s8u(pSrcDst, pResid, srcDstStep) \
    ippiAdd8x8_16s8u_C1IRS(pResid, 16, pSrcDst, srcDstStep)

//--------------------- defines to control ME process -------------------------

#define SAD_MAX                 (16 * 16 * 256)
#define SAD_FAVOR_ZERO          0
#define SAD_FAVOR_PRED          (- SAD_FAVOR_ZERO / 2)
#define SAD_FAVOR_DIRECT        (quant * 3) //(129 / 2)
#define SAD_FAVOR_INTER         mp4_Inter_Favor[quant]
//(quant * 64) // ((quant * quant * 5) >> 1) // 500 H.263 App III
#define SAD_FAVOR_16x16         (quant * 25)   // 200 H.263 App III
#define SAD_FAVOR_FIELD         (quant * 6)
#define SAD_FAVOR_GMC           (SAD_FAVOR_ZERO + SAD_FAVOR_16x16 + 1)
#define SAD_NOTCODED_THR_LUMA   10
#define SAD_NOTCODED_THR_CHROMA 20
#define DEV_FAVOR_INTRA         (mp4_Inter_Favor[quant] >> 2)
//(quant * 16)

#define ME_HP               2       // use Half-Pel ME
#define ME_QP               4       // use Quarter-Pel ME
#define ME_4MV              8       // use 4 MV
#define ME_HP_FAST          16      // use Fast Half-Pel algorithm
#define ME_CHROMA           32      // do Chroma ME additionally to the Luma
#define ME_USE_MVWEIGHT     64      // use MV length
#define ME_SUBPEL_FINE      128     // fine subpel search
#define ME_USE_THRESHOLD    256     // use threshold in ME (faster but worse)
#define ME_AUTO_RANGE       512     // auto detect search window
#define ME_FIELD            1024    // use field ME
#define ME_FRAME            2048    // use frame ME
#define ME_USE_MANYPRED     4096    // use many predictors
#define ME_ZERO_MV          8192    // disable ME
#define RD_TRELLIS         16384    // use Trellis quantization
#define RD_MODE_DECISION   32768    // use RD for Inter/Intra decision

#define RD_MUL 8

enum {
    ME_SEARCH_FULL_RECT,
    ME_SEARCH_DIAMOND_SMALL,
    ME_SEARCH_DIAMOND_BIG,
    ME_SEARCH_SQUARE,
    ME_SEARCH_LOG,
    ME_SEARCH_FULL_INVOLUTE = 15,
};


extern void mp4_ME_SAD(mp4_Data_ME *meData);
extern Ipp32s mp4_WeightMV(Ipp32s dx, Ipp32s dy, IppMotionVector mvPred, Ipp32s fcode, Ipp32s quant, Ipp32s qpel);
extern void mp4_ComputeChroma4MV(const IppMotionVector mvLuma[4], IppMotionVector *mvChroma);
extern void mp4_ComputeChroma4MVQ(const IppMotionVector mvLuma[4], IppMotionVector *mvChroma);
extern int mp4_CalcBitsMV(IppMotionVector *mv, int fcode);
extern int mp4_CalcBitsCoeffsIntra(const Ipp16s* pCoeffs, int countNonZero, int rvlcFlag, int dc_vlc, const Ipp8u* pScanTable, int blockNum);
extern int mp4_CalcBitsCoeffsInter(const Ipp16s* pCoeffs, int countNonZero, int rvlcFlag, const Ipp8u* pScanTable);


template <class T> inline void mp4_Swap(T &a, T &b)
{
    T  t;
    t = a; a = b; b = t;
}

    // tables
extern const Ipp8u mp4_DefaultIntraQuantMatrix[];
extern const Ipp8u mp4_DefaultNonIntraQuantMatrix[];
extern const Ipp8u mp4_ZigZagScan[];
extern const Ipp8u mp4_AltVertScan[];
extern const Ipp8u mp4_HorScan[];
extern const int mp4_DC_VLC_Threshold[];
extern const Ipp8u mp4_cCbCrMvRound16[];
extern const Ipp8u mp4_DCScalerLuma[];
extern const Ipp8u mp4_DCScalerChroma[];
extern const mp4_VLC mp4_VLC_CBPY_TB8[];
extern const mp4_VLC mp4_VLC_MCBPC_TB7[];
extern const mp4_VLC mp4_VLC_MVD_TB12[];
extern const Ipp32s  mp4_DivIntraDivisor[];
extern const Ipp8u mp4_MV_Weigth[];
extern const Ipp32s mp4_Inter_Favor[];
extern const Ipp8u mp4_VLC_DCSIZE_TB13_len[];
extern const Ipp8u mp4_VLC_DCSIZE_TB14_len[];
extern const mp4_VLC_TCOEF mp4_VLC_TB16;
extern const mp4_VLC_TCOEF mp4_VLC_TB17;
extern const mp4_VLC mp4_VLC_RMAX_TB21[2][7];
extern const mp4_VLC mp4_VLC_RMAX_TB22[2][6];
extern const mp4_VLC_TCOEF mp4_VLC_TB23a;
extern const mp4_VLC_TCOEF mp4_VLC_TB23b;


#ifdef _OMP_KARABAS
#ifdef _OPENMP
#define mp4_MT_get_thread_num() omp_get_thread_num()
#define mp4_MT_set_lock(lock) omp_set_lock(lock)
#define mp4_MT_unset_lock(lock) omp_unset_lock(lock)
#else
#define mp4_MT_get_thread_num() 0
#define mp4_MT_set_lock(lock)
#define mp4_MT_unset_lock(lock)
#endif
#endif // _OMP_KARABAS

} //namespace MPEG4_ENC

#endif
