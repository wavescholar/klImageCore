/* ///////////////////////////////////////////////////////////////////////
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
//  Description:    MPEG-4 header.
//
*/

#ifndef __MP4_H__
#define __MP4_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vm_debug.h"
#include "vm_thread.h"

#include "ippcore.h"
#include "ipps.h"
#include "ippi.h"
#include "ippvc.h"

#if defined(WINDOWS)
#pragma warning(disable : 4710) // function not inlined
#pragma warning(disable : 4514) // unreferenced inline function has been removed CL
#pragma warning(disable : 4100) // unreferenced formal parameter CL
#endif

#if defined(_OPENMP)
#include <omp.h>
#define ENABLE_OPENMP
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#define USE_INLINE_BITS_FUNC
#define USE_NOTCODED_STATE

#if defined(__INTEL_COMPILER) || defined(_MSC_VER)
    #define __INLINE static __inline
#elif defined( __GNUC__ )
    #define __INLINE static __inline__
#else
    #define __INLINE static
#endif

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

#define mp4_CLIP(x, min, max) if ((x) < (min)) (x) = (min); else if ((x) > (max)) (x) = (max)
#define mp4_CLIPR(x, max) if ((x) > (max)) (x) = (max)
#define mp4_SWAP(type, x, y) {type t = (x); (x) = (y); (y) = t;}
#define mp4_ABS(a) ((a) >= 0 ? (a) : -(a))

/* Timer Info */
#if defined WINDOWS
#include <windows.h>

struct mp4_Timer {
    LARGE_INTEGER  count;
    LARGE_INTEGER  start;
    LARGE_INTEGER  stop;
    Ipp32s         calls;
};

__INLINE void mp4_TimerStart(mp4_Timer *t)
{
    QueryPerformanceCounter(&t->start);
}

__INLINE void mp4_TimerStop(mp4_Timer *t)
{
    QueryPerformanceCounter(&t->stop);
    t->count.QuadPart += t->stop.QuadPart - t->start.QuadPart;
    t->calls ++;
}

#define TIMER_FREQ_TYPE LARGE_INTEGER

__INLINE void mp4_GetTimerFreq(TIMER_FREQ_TYPE *f)
{
    QueryPerformanceFrequency(f);
}

__INLINE Ipp64f mp4_GetTimerSec(mp4_Timer *t, TIMER_FREQ_TYPE f)
{
    return (Ipp64f)t->count.QuadPart / (Ipp64f)f.QuadPart;
}

#else
#include <time.h>

struct mp4_Timer {
    clock_t  count;
    clock_t  start;
    clock_t  stop;
    Ipp32s   calls;
};

__INLINE void mp4_TimerStart(mp4_Timer *t)
{
    t->start = clock();
}

__INLINE void mp4_TimerStop(mp4_Timer *t)
{
    t->stop = clock();
    t->count += t->stop - t->start;
    t->calls ++;
}

#define TIMER_FREQ_TYPE Ipp32s

__INLINE void mp4_GetTimerFreq(TIMER_FREQ_TYPE *f)
{
    *f = CLOCKS_PER_SEC;
}

__INLINE Ipp64f mp4_GetTimerSec(mp4_Timer *t, TIMER_FREQ_TYPE f)
{
    return (Ipp64f)t->count / (Ipp64f)f;
}

#endif

/* number of exterior MB */
#define MP4_NUM_EXT_MB 1

/* Statistic Info */
struct mp4_Statistic
{
    // VideoObjectLayer Info
    Ipp32s      nVOP;
    Ipp32s      nVOP_I;
    Ipp32s      nVOP_P;
    Ipp32s      nVOP_B;
    Ipp32s      nVOP_S;
    Ipp32s      nMB;
    Ipp32s      nMB_INTER;
    Ipp32s      nMB_INTER_Q;
    Ipp32s      nMB_INTRA;
    Ipp32s      nMB_INTRA_Q;
    Ipp32s      nMB_INTER4V;
    Ipp32s      nMB_DIRECT;
    Ipp32s      nMB_INTERPOLATE;
    Ipp32s      nMB_BACKWARD;
    Ipp32s      nMB_FORWARD;
    Ipp32s      nMB_NOTCODED;
    Ipp32s      nB_INTRA_DC;
    Ipp32s      nB_INTRA_AC;
    Ipp32s      nB_INTER_C;
    Ipp32s      nB_INTER_NC;
    // app Timing Info
    mp4_Timer   time_DecodeShow;    // decode + draw + file reading
    mp4_Timer   time_Decode;        // decode + file reading
    mp4_Timer   time_DecodeOnly;    // decode only
};

__INLINE void mp4_StatisticInc(Ipp32s *s)
{
    *s = (*s) + 1;
}

// when using Full Statistic, FPS is less
#ifdef MP4_FULL_STAT
#define mp4_StatisticInc_(s) mp4_StatisticInc(s)
#define mp4_TimerStart_(t) mp4_TimerStart(t)
#define mp4_TimerStop_(t) mp4_TimerStop(t)
#else
#define mp4_StatisticInc_(s)
#define mp4_TimerStart_(t)
#define mp4_TimerStop_(t)
#endif

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

// ISO/IEC 14496-2:2004 table 7-3
enum {
    MP4_BAB_TYPE_MVDSZ_NOUPDATE  = 0,
    MP4_BAB_TYPE_MVDSNZ_NOUPDATE = 1,
    MP4_BAB_TYPE_TRANSPARENT     = 2,
    MP4_BAB_TYPE_OPAQUE          = 3,
    MP4_BAB_TYPE_INTRACAE        = 4,
    MP4_BAB_TYPE_MVDSZ_INTERCAE  = 5,
    MP4_BAB_TYPE_MVDSNZ_INTERCAE = 6
};

#define MP4_DC_MARKER  0x6B001 // 110 1011 0000 0000 0001
#define MP4_MV_MARKER  0x1F001 //   1 1111 0000 0000 0001

// ISO/IEC 14496-2:2004 table G.1
enum {
    MP4_SIMPLE_PROFILE_LEVEL_1                     = 0x01,
    MP4_SIMPLE_PROFILE_LEVEL_2                     = 0x02,
    MP4_SIMPLE_PROFILE_LEVEL_3                     = 0x03,
    MP4_SIMPLE_PROFILE_LEVEL_0                     = 0x08,
    MP4_SIMPLE_SCALABLE_PROFILE_LEVEL_0            = 0x10,
    MP4_SIMPLE_SCALABLE_PROFILE_LEVEL_1            = 0x11,
    MP4_SIMPLE_SCALABLE_PROFILE_LEVEL_2            = 0x12,
    MP4_CORE_PROFILE_LEVEL_1                       = 0x21,
    MP4_CORE_PROFILE_LEVEL_2                       = 0x22,
    MP4_MAIN_PROFILE_LEVEL_2                       = 0x32,
    MP4_MAIN_PROFILE_LEVEL_3                       = 0x33,
    MP4_MAIN_PROFILE_LEVEL_4                       = 0x34,
    MP4_NBIT_PROFILE_LEVEL_2                       = 0x42,
    MP4_SCALABLE_TEXTURE_PROFILE_LEVEL_1           = 0x51,
    MP4_SIMPLE_FACE_ANIMATION_PROFILE_LEVEL_1      = 0x61,
    MP4_SIMPLE_FACE_ANIMATION_PROFILE_LEVEL_2      = 0x62,
    MP4_SIMPLE_FBA_PROFILE_LEVEL_1                 = 0x63,
    MP4_SIMPLE_FBA_PROFILE_LEVEL_2                 = 0x64,
    MP4_BASIC_ANIMATED_TEXTURE_PROFILE_LEVEL_1     = 0x71,
    MP4_BASIC_ANIMATED_TEXTURE_PROFILE_LEVEL_2     = 0x72,
    MP4_HYBRID_PROFILE_LEVEL_1                     = 0x81,
    MP4_HYBRID_PROFILE_LEVEL_2                     = 0x82,
    MP4_ADVANCED_REAL_TIME_SIMPLE_PROFILE_LEVEL_1  = 0x91,
    MP4_ADVANCED_REAL_TIME_SIMPLE_PROFILE_LEVEL_2  = 0x92,
    MP4_ADVANCED_REAL_TIME_SIMPLE_PROFILE_LEVEL_3  = 0x93,
    MP4_ADVANCED_REAL_TIME_SIMPLE_PROFILE_LEVEL_4  = 0x94,
    MP4_CORE_SCALABLE_PROFILE_LEVEL_1              = 0xA1,
    MP4_CORE_SCALABLE_PROFILE_LEVEL_2              = 0xA2,
    MP4_CORE_SCALABLE_PROFILE_LEVEL_3              = 0xA3,
    MP4_ADVANCED_CODING_EFFICIENCY_PROFILE_LEVEL_1 = 0xB1,
    MP4_ADVANCED_CODING_EFFICIENCY_PROFILE_LEVEL_2 = 0xB2,
    MP4_ADVANCED_CODING_EFFICIENCY_PROFILE_LEVEL_3 = 0xB3,
    MP4_ADVANCED_CODING_EFFICIENCY_PROFILE_LEVEL_4 = 0xB4,
    MP4_ADVANCED_CORE_PROFILE_LEVEL_1              = 0xC1,
    MP4_ADVANCED_CORE_PROFILE_LEVEL_2              = 0xC2,
    MP4_ADVANCED_SCALABLE_TEXTURE_PROFILE_LEVEL_1  = 0xD1,
    MP4_ADVANCED_SCALABLE_TEXTURE_PROFILE_LEVEL_2  = 0xD2,
    MP4_ADVANCED_SCALABLE_TEXTURE_PROFILE_LEVEL_3  = 0xD3,
    MP4_SIMPLE_STUDIO_PROFILE_LEVEL_1              = 0xE1,
    MP4_SIMPLE_STUDIO_PROFILE_LEVEL_2              = 0xE2,
    MP4_SIMPLE_STUDIO_PROFILE_LEVEL_3              = 0xE3,
    MP4_SIMPLE_STUDIO_PROFILE_LEVEL_4              = 0xE4,
    MP4_CORE_STUDIO_PROFILE_LEVEL_1                = 0xE5,
    MP4_CORE_STUDIO_PROFILE_LEVEL_2                = 0xE6,
    MP4_CORE_STUDIO_PROFILE_LEVEL_3                = 0xE7,
    MP4_CORE_STUDIO_PROFILE_LEVEL_4                = 0xE8,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_0            = 0xF0,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_1            = 0xF1,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_2            = 0xF2,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_3            = 0xF3,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_4            = 0xF4,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_5            = 0xF5,
    MP4_ADVANCED_SIMPLE_PROFILE_LEVEL_3B           = 0xF7,
    MP4_FGS_PROFILE_LEVEL_0                        = 0xF8,
    MP4_FGS_PROFILE_LEVEL_1                        = 0xF9,
    MP4_FGS_PROFILE_LEVEL_2                        = 0xFA,
    MP4_FGS_PROFILE_LEVEL_3                        = 0xFB,
    MP4_FGS_PROFILE_LEVEL_4                        = 0xFC,
    MP4_FGS_PROFILE_LEVEL_5                        = 0xFD
};

/* Frame Info */
struct mp4_Frame {
    Ipp8u*      apY;        // allocated with border
    Ipp8u*      apCb;       // allocated with border
    Ipp8u*      apCr;       // allocated with border
    Ipp32s      stepY;
    Ipp32s      stepCr;
    Ipp32s      stepCb;
    Ipp8u*      pY;         // real pointer
    Ipp8u*      pCb;        // real pointer
    Ipp8u*      pCr;        // real pointer
    Ipp32s      type;
    Ipp64s      time;
    Ipp32s      mbPerRow;   // info for realloc VOP with Shape
    Ipp32s      mbPerCol;
    Ipp8u*      apB;        // for binary mask
    Ipp8u*      pB;
    Ipp8u*      apA[3];     // for aux components
    Ipp8u*      pA[3];
    Ipp32u      mid;
};

/* Block Info for Intra Prediction */
struct mp4_IntraPredBlock
{
    mp4_IntraPredBlock  *predA;
    mp4_IntraPredBlock  *predB;
    mp4_IntraPredBlock  *predC;
    Ipp16s      dct_acA[8];
    Ipp16s      dct_acC[8];
    Ipp16s      dct_dc;
};

/* Buffer for Intra Prediction */
struct mp4_IntraPredBuff {
    Ipp8u               *quant;     // quant buffer;
    mp4_IntraPredBlock  dcB[6];     // blocks for Left-Top DC only
    mp4_IntraPredBlock  *block;
};

/* MacroBlock Info Data Partitioned mode */
struct mp4_DataPartMacroBlock {
    Ipp16s          dct_dc[6];
    Ipp8u           type;
    Ipp8u           not_coded;
    Ipp8u           mcsel;
    Ipp8u           ac_pred_flag;
    Ipp8u           pat;
    Ipp8u           quant;
};

/* MacroBlock Info for Motion */
struct mp4_MacroBlock {
    IppMotionVector mv[4];
    Ipp8u        validPred;     // for MV pred, OBMC
    Ipp8u        type;          // for OBMC, BVOP
    Ipp8u        not_coded;     // for OBMC, BVOP
    Ipp8u        field_info;    // for Interlaced BVOP Direct mode
};

/* Group Of Video Object Plane Info */
struct mp4_GroupOfVideoObjectPlane {
    Ipp64s      time_code;
    Ipp32s      closed_gov;
    Ipp32s      broken_link;
};

/* Video Object Plane Info */
struct mp4_VideoObjectPlane {
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
    Ipp32s      vop_id;
    Ipp32s      vop_id_for_prediction_indication;
    Ipp32s      vop_id_for_prediction;
};

/* mp4_ComplexityEstimation Info */
struct mp4_ComplexityEstimation {
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
struct mp4_ScalabilityParameters {
    Ipp32s      dummy;
};

/* VOLControlParameters Info */
struct mp4_VOLControlParameters {
    Ipp32s      chroma_format;
    Ipp32s      low_delay;
    Ipp32s      vbv_parameters;
    Ipp32s      bit_rate;
    Ipp32s      vbv_buffer_size;
    Ipp32s      vbv_occupancy;
};

/* Video Object Plane with Ipp16s header Info */
struct mp4_VideoObjectPlaneH263 {
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
#ifdef ENABLE_OPENMP
    Ipp32s      nmb;
    Ipp32s      frGOB;
#endif
};

/* Video Object Info */
struct mp4_VideoObject {
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
    Ipp8u                       intra_quant_mat[64];
    Ipp32s                      load_nonintra_quant_mat;
    Ipp8u                       nonintra_quant_mat[64];
    Ipp32s                      load_intra_quant_mat_grayscale[3];
    Ipp8u                       intra_quant_mat_grayscale[3][64];
    Ipp32s                      load_nonintra_quant_mat_grayscale[3];
    Ipp8u                       nonintra_quant_mat_grayscale[3][64];
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
// app part
    Ipp32s                      VOPindex;
    Ipp32s                      MacroBlockPerRow;
    Ipp32s                      MacroBlockPerCol;
    Ipp32s                      MacroBlockPerVOP;
    Ipp32s                      mbns; // num bits for MacroBlockPerVOP
    mp4_MacroBlock*             MBinfo;
    mp4_IntraPredBuff           IntraPredBuff;
    mp4_DataPartMacroBlock*     DataPartBuff;
    IppiQuantInvIntraSpec_MPEG4*  QuantInvIntraSpec;
    IppiQuantInvInterSpec_MPEG4*  QuantInvInterSpec;
    IppiWarpSpec_MPEG4*         WarpSpec;
    // for B-VOP
    Ipp32s                      prevPlaneIsB;
    // for interlaced B-VOP direct mode
    Ipp32s                      Tframe;
    IppMotionVector*            FieldMV;
    // for B-VOP direct mode
    Ipp32s                      TRB, TRD;
    // time increment of past and future VOP for B-VOP
    Ipp64s                      rTime, nTime;
    // VOP global time
    Ipp64s                      vop_sync_time, vop_sync_time_b;
#ifdef USE_NOTCODED_STATE
    // not_coded MB state
    Ipp8u*                      ncState;
    Ipp32s                      ncStateCleared;
#endif
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

/* video_signal_type Info */
struct mp4_VideoSignalType {
    Ipp32s  is_video_signal_type;
    Ipp32s  video_format;
    Ipp32s  video_range;
    Ipp32s  is_colour_description;
    Ipp32s  colour_primaries;
    Ipp32s  transfer_characteristics;
    Ipp32s  matrix_coefficients;
};

/* Visual Object Info */
struct mp4_VisualObject {
    Ipp32s                  is_identifier;
    Ipp32s                  verid;
    Ipp32s                  priority;
    Ipp32s                  type;
    mp4_VideoSignalType     VideoSignalType;
    mp4_VideoObject         VideoObject;
    mp4_StillTextureObject  StillTextureObject;
    mp4_MeshObject          MeshObject;
    mp4_FaceObject          FaceObject;
    mp4_Frame               sFrame;  // static sprite
    mp4_Frame               cFrame;  // current
    mp4_Frame               rFrame;  // reference in past
    mp4_Frame               nFrame;  // reference in future
    mp4_Frame              *vFrame;  // frame for display
    Ipp32s                  frameCount;
    Ipp32s                  frameInterval;
    Ipp32s                  frameScale;
    mp4_Statistic           Statistic;
};

#ifdef ENABLE_OPENMP
/* MacroBlock Info for MT */
struct mp4_MacroBlockMT {
    Ipp16s    dctCoeffs[64*6];
    IppMotionVector mvF[4];     // B-VOP
    IppMotionVector mvB[4];     // B-VOP
    Ipp32s    lnz[6];
    Ipp8u     pat;
    Ipp8u     mb_type;          // B-VOP
    Ipp8u     dct_type;         // interlaced
    Ipp8u     field_info;       // interlaced
    Ipp8u     mcsel;            // S(GMC)-VOP
    Ipp8u     dummy[3];         // align 16
};
#endif

/* Full Info */
struct mp4_Info {
    Ipp32s      ftype;          // 0 - raw, 1 - mp4, 2 - avi
    Ipp32s      ftype_f;        // ftype == 1 (0 - QuickTime(tm)), ftype == 2 (0 - DivX(tm) v. < 5, XVID, 1 - DivX(tm) v. >= 5)
    Ipp8u*      buffer;         /* buffer header for saving MPEG-4 stream */
    size_t    buflen;         /* total buffer length */
    size_t    len;            /* valid data in buffer */
    Ipp8u*      bufptr;         /* current frame, point to header or data */
    Ipp32s      bitoff;         /* mostly point to next frame header or PSC */
    Ipp32s      profile_and_level_indication;
    mp4_VisualObject    VisualObject;
#ifdef ENABLE_OPENMP
    vm_thread_priority mTreadPriority;
    mp4_MacroBlockMT*  pMBinfoMT;  /* OpenMP buffer */
    Ipp32s      mbInVideoPacket;   /* needed for dcVLC */
#endif
    Ipp32s      num_threads;       /* OpenMP number of threads */
    Ipp32s      stopOnErr;
    int         strictSyntaxCheck;
    int         noPVOPs;
    int         noBVOPs;
};

/* bitstream functions */
extern Ipp8u*     mp4_FindStartCodePtr(mp4_Info* pInfo);
extern Ipp8u*     mp4_FindStartCodeOrShortPtr(mp4_Info* pInfo);
extern Ipp32s     mp4_SeekStartCodePtr(mp4_Info* pInfo);
extern Ipp32s     mp4_SeekStartCodeOrShortPtr(mp4_Info* pInfo);
extern Ipp32s     mp4_SeekStartCodeValue(mp4_Info* pInfo, Ipp8u code);
extern Ipp8u*     mp4_FindShortVideoStartMarkerPtr(mp4_Info* pInfo);
extern Ipp32s     mp4_SeekShortVideoStartMarker(mp4_Info* pInfo);
extern Ipp32s     mp4_SeekGOBMarker(mp4_Info* pInfo);
extern Ipp32s     mp4_SeekResyncMarker(mp4_Info* pInfo);
extern Ipp32s     mp4_FindResyncMarker(mp4_Info* pInfo);
extern int        mp4_IsStartCodeOrShort(mp4_Info* pInfo);
extern int        mp4_IsStartCodeValue(mp4_Info* pInfo, int min, int max);
extern int        mp4_IsShortCode(mp4_Info* pInfo);

/* tables */
struct mp4_VLC1 {
    Ipp8u  code;
    Ipp8u  len;
};

extern const Ipp8u mp4_DefaultIntraQuantMatrix[];
extern const Ipp8u mp4_DefaultNonIntraQuantMatrix[];
extern const Ipp8u mp4_ClassicalZigzag[];
extern const Ipp8u mp4_DCScalerLuma[];
extern const Ipp8u mp4_DCScalerChroma[];
extern const Ipp8u mp4_cCbCrMvRound16[];
extern const Ipp8u mp4_cCbCrMvRound12[];
extern const Ipp8u mp4_cCbCrMvRound8[];
extern const Ipp8u mp4_cCbCrMvRound4[];
extern const Ipp8s mp4_dquant[];
extern const mp4_VLC1 mp4_cbpy1[];
extern const mp4_VLC1 mp4_cbpy2[];
extern const mp4_VLC1 mp4_cbpy3[];
extern const mp4_VLC1 mp4_cbpy4[];
extern const mp4_VLC1* mp4_cbpy_t[];
extern const Ipp8u mp4_cbpy_b[];
extern const Ipp32s mp4_DC_vlc_Threshold[];
extern const Ipp8u mp4_PVOPmb_type[];
extern const Ipp8u mp4_PVOPmb_cbpc[];
extern const Ipp8u mp4_PVOPmb_bits[];
extern const mp4_VLC1 mp4_BVOPmb_type[];
extern const mp4_VLC1 mp4_MVD_B12_1[];
extern const mp4_VLC1 mp4_MVD_B12_2[];
extern const Ipp32s mp4_H263_width[];
extern const Ipp32s mp4_H263_height[];
extern const Ipp32s mp4_H263_mbgob[];
extern const Ipp32s mp4_H263_gobvop[];
extern const Ipp32s mp4_H263_rowgob[];
extern const Ipp8u mp4_aux_comp_count[];
extern const Ipp8u mp4_aux_comp_is_alpha[];
extern const Ipp8u mp4_BABtypeIntra[][3];
extern const Ipp32s mp4_DivIntraDivisor[];

// project functions
//extern void       mp4_Error(Ipp8s *str);
extern mp4_Status mp4_InitDecoder(mp4_Info *pInfo, Ipp8s *mp4FileName, Ipp32s StopOnErr);
extern mp4_Status mp4_CloseDecoder(mp4_Info *pInfo);
extern mp4_Status mp4_InitVOL(mp4_Info *pInfo);
extern mp4_Status mp4_FreeVOL(mp4_Info *pInfo);
extern void       mp4_ResetVOL(mp4_Info *pInfo);
//extern void       mp4_ShowFrame(mp4_Frame *frame);
#define mp4_ShowFrame(frame)
extern mp4_Status mp4_Parse_VisualObjectSequence(mp4_Info* pInfo);
extern mp4_Status mp4_Parse_VisualObject(mp4_Info* pInfo);
extern mp4_Status mp4_Parse_VideoObject(mp4_Info* pInfo);
extern mp4_Status mp4_Parse_GroupOfVideoObjectPlane(mp4_Info* pInfo);
extern mp4_Status mp4_Parse_VideoObjectPlane(mp4_Info* pInfo);
extern mp4_Status mp4_DecodeVideoObjectPlane(mp4_Info* pInfo);

#ifdef ENABLE_OPENMP
extern Ipp32s mp4_GetNumOfThreads(void);
#endif

#ifndef USE_INLINE_BITS_FUNC
extern Ipp32u mp4_ShowBits(mp4_Info* pInfo, Ipp32s n);
extern Ipp32u mp4_ShowBit(mp4_Info* pInfo);
extern Ipp32u mp4_ShowBits9(mp4_Info* pInfo, Ipp32s n);
extern void   mp4_FlushBits(mp4_Info* pInfo, Ipp32s n);
extern Ipp32u mp4_GetBits(mp4_Info* pInfo, Ipp32s n);
//extern Ipp32u mp4_GetBit(mp4_Info* pInfo);
extern Ipp32u mp4_GetBits9(mp4_Info* pInfo, Ipp32s n);
extern void   mp4_AlignBits(mp4_Info* pInfo);
extern void   mp4_AlignBits7F(mp4_Info* pInfo);
extern Ipp32u mp4_ShowBitsAlign(mp4_Info* pInfo, Ipp32s n);
extern Ipp32u mp4_ShowBitsAlign7F(mp4_Info* pInfo, Ipp32s n);
#else
__INLINE Ipp32u mp4_ShowBits(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] <<  8) | (ptr[3]);
    tmp <<= pInfo->bitoff;
    tmp >>= 32 - n;
    return tmp;
}

__INLINE Ipp32u mp4_ShowBit(mp4_Info* pInfo)
{
    Ipp32u tmp = pInfo->bufptr[0];
    tmp >>= 7 - pInfo->bitoff;
    return (tmp & 1);
}

__INLINE Ipp32u mp4_ShowBits9(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] <<  8) | ptr[1];
    tmp <<= (pInfo->bitoff + 16);
    tmp >>= 32 - n;
    return tmp;
}

__INLINE void mp4_FlushBits(mp4_Info* pInfo, Ipp32s n)
{
    n = n + pInfo->bitoff;
    pInfo->bufptr += n >> 3;
    pInfo->bitoff = n & 7;
}

__INLINE Ipp32u mp4_GetBits(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] <<  8) | (ptr[3]);
    tmp <<= pInfo->bitoff;
    tmp >>= 32 - n;
    n = n + pInfo->bitoff;
    pInfo->bufptr += n >> 3;
    pInfo->bitoff = n & 7;
    return tmp;
}

__INLINE Ipp32u mp4_GetBits9(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bufptr;
    Ipp32u tmp = (ptr[0] <<  8) | ptr[1];
    tmp <<= (pInfo->bitoff + 16);
    tmp >>= 32 - n;
    n = n + pInfo->bitoff;
    pInfo->bufptr += n >> 3;
    pInfo->bitoff = n & 7;
    return tmp;
}

__INLINE void mp4_AlignBits(mp4_Info* pInfo)
{
    if (pInfo->bitoff > 0) {
        pInfo->bitoff = 0;
        (pInfo->bufptr)++;
    }
}

__INLINE void mp4_AlignBits7F(mp4_Info* pInfo)
{
    if (pInfo->bitoff > 0) {
        pInfo->bitoff = 0;
        (pInfo->bufptr)++;
    } else {
        if (*pInfo->bufptr == 0x7F)
            (pInfo->bufptr)++;
    }
}

__INLINE Ipp32u mp4_ShowBitsAlign(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bitoff ? (pInfo->bufptr + 1) : pInfo->bufptr;
    Ipp32u tmp = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] <<  8) | (ptr[3]);
    tmp >>= 32 - n;
    return tmp;
}

__INLINE Ipp32u mp4_ShowBitsAlign7F(mp4_Info* pInfo, Ipp32s n)
{
    Ipp8u* ptr = pInfo->bitoff ? (pInfo->bufptr + 1) : pInfo->bufptr;
    Ipp32u tmp;
    if (!pInfo->bitoff) {
        if (*ptr == 0x7F)
            ptr ++;
    }
    tmp = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] <<  8) | (ptr[3]);
    tmp >>= 32 - n;
    return tmp;
}

#endif // USE_INLINE_BITS_FUNC

__INLINE Ipp32u mp4_GetBit(mp4_Info* pInfo)
{
    Ipp32u tmp = pInfo->bufptr[0];
    if (pInfo->bitoff != 7) {
        tmp >>= 7 - pInfo->bitoff;
        pInfo->bitoff ++;
    } else {
        pInfo->bitoff = 0;
        pInfo->bufptr ++;
    }
    return (tmp & 1);
}

__INLINE Ipp32s mp4_GetMarkerBit(mp4_Info* pInfo) {
    if (!mp4_GetBit(pInfo))
        return 0;
    return 1;
}


#ifdef __cplusplus
}
#endif

#endif // __MP4_H__


