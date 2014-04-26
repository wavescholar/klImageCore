/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_ENC_DEFS_H
#define __UMC_MPEG2_ENC_DEFS_H

#include "vm_semaphore.h"
#include "vm_event.h"
#include "vm_thread.h"
#include "umc_mpeg2_enc.h"

//#define ME_REF_ORIGINAL

#define SCENE_DETECTION
#define ALIGN_VALUE  16

#if 1
#define MP2_ALLOC(TYPE, SIZE) ((TYPE*)ippMalloc(sizeof(TYPE)*(SIZE)))
#define MP2_FREE(PTR)         ippFree(PTR)
#else
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void *my_malloc(size_t size);
void my_free(void *ptr);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#define MP2_ALLOC(TYPE, SIZE) ((TYPE*)my_malloc(sizeof(TYPE)*(SIZE)))
#define MP2_FREE(PTR)  my_free(PTR)
#endif

#if defined(_DEBUG) || defined(MPEG2_ASSERT) || defined(MPEG2_DEBUG_CODE)
#define mpeg2_assert(value) \
if (!(value)) { \
  printf("######################################################\n"); \
  printf("%s: %d: Assertion failed: " #value "\n", __FILE__, __LINE__); \
  printf("######################################################\n"); \
  exit(1); \
}
#else
#define mpeg2_assert(value)
#endif

#if defined(_DEBUG) || defined(MPEG2_DEBUG_CODE)
#if 1
#define GET_TICKS      vm_time_get_tick()
#define GET_FREQUENCY  vm_time_get_frequency()
#else
#define GET_TICKS      ippGetCpuClocks()
#define GET_FREQUENCY  CPU_FREQUENCY
#endif
#else
#define GET_TICKS      0
#define GET_FREQUENCY  0
#endif

#define PICTURE_START_CODE 0x100L
#define SLICE_MIN_START    0x101L
#define SLICE_MAX_START    0x1AFL
#define USER_START_CODE    0x1B2L
#define SEQ_START_CODE     0x1B3L
#define EXT_START_CODE     0x1B5L
#define SEQ_END_CODE       0x1B7L
#define GOP_START_CODE     0x1B8L
#define ISO_END_CODE       0x1B9L
#define PACK_START_CODE    0x1BAL
#define SYSTEM_START_CODE  0x1BBL

/* macroblock type */
#define MB_INTRA    1
#define MB_PATTERN  2
#define MB_BACKWARD 4
#define MB_FORWARD  8
#define MB_QUANT    16

/* motion_type */
#define MC_FIELD 1
#define MC_FRAME 2
#define MC_16X8  2
#define MC_DMV   3

/* extension start code IDs */

#define SEQ_ID       1
#define DISP_ID      2
#define QUANT_ID     3
#define SEQSCAL_ID   5
#define PANSCAN_ID   7
#define CODING_ID    8
#define SPATSCAL_ID  9
#define TEMPSCAL_ID 10

/* input file type */
#define T_Y_U_V 0   // .y; .u; .v;
#define T_YUV   1   // .yuv
#define T_PPM   2   // .ppm

#define V_DC_Tbl Cr_DC_Tbl
#define U_DC_Tbl Cr_DC_Tbl

// scale parameters for frame/field or intra/predicted selection
// use because of counting bits for vectors encoding

#define SC_VAR_INTRA 16
#define SC_VAR_1V    16
#define SC_VAR_2V    17
#define SC_VAR_1VBI  16
#define SC_VAR_2VBI  17

#define SCALE_VAR(val, scale) ( (val)*(scale) )
#define SCALE_VAR_INTRA(val)              \
( i==0 ? (val+1)*SC_VAR_INTRA             \
       : (pMBInfo[k-1].mb_type==MB_INTRA  \
          ? val*SC_VAR_INTRA              \
          :(val+2)*SC_VAR_INTRA)          \
)

#define RANGE_TO_F_CODE(range, fcode) { \
  Ipp32s fc = 1;                        \
  while((4<<fc) < range && fc <= 15)    \
    fc++;                               \
  fcode = fc;                           \
}


#define CALC_START_STOP_ROWS                       \
  if(threads)                                      \
  {                                                \
    start_y = threadSpec[numTh].start_row;         \
    if(encodeInfo.FieldPicture) start_y >>= 1;     \
    if (numTh < (Ipp32s)encodeInfo.m_iThreads - 1) {       \
      stop_y = threadSpec[numTh + 1].start_row;    \
      if(encodeInfo.FieldPicture) stop_y >>= 1;    \
    } else {                                       \
      stop_y = MBcountV*16;                        \
    }                                              \
    if (encodeInfo.m_info.videoInfo.m_colorFormat == YUV420) {  \
      start_uv = (start_y >> 1);                   \
    } else {                                       \
      start_uv = start_y;                          \
    }                                              \
    k = (start_y/16)*MBcountH;                     \
  }                                                \
  else                                             \
  {                                                \
    start_y = start_uv = 0;                        \
    stop_y = MBcountV*16;                          \
    k = 0;                                         \
  }

#define SWAP_PTR(ptr0, ptr1) { \
  MB_prediction_info *tmp_ptr = ptr0; \
  ptr0 = ptr1; \
  ptr1 = tmp_ptr; \
}

#define SET_MOTION_VECTOR(vectorF, mv_x, mv_y) {                              \
  Ipp32s x_l = mv_x;                                                          \
  Ipp32s y_l = mv_y;                                                          \
  Ipp32s i_c = (BlkWidth_c  == 16) ? i : (i >> 1);                            \
  Ipp32s j_c = (BlkHeight_c == 16) ? j : (j >> 1);                            \
  Ipp32s x_c = (BlkWidth_c  == 16) ? x_l : (x_l/2);                           \
  Ipp32s y_c = (BlkHeight_c == 16) ? y_l : (y_l/2);                           \
  vectorF->x = x_l;                                                           \
  vectorF->y = y_l;                                                           \
  vectorF->mctype_l = ((x_l & 1) << 3) | ((y_l & 1) << 2);                    \
  vectorF->mctype_c = ((x_c & 1) << 3) | ((y_c & 1) << 2);                    \
  vectorF->offset_l = (i   + (x_l >> 1)) + (j   + (y_l >> 1)) * YFrameHSize;  \
  vectorF->offset_c = (i_c + (x_c >> 1)) + (j_c + (y_c >> 1)) * UVFrameHSize; \
}

// doubt about field pictures
#define SET_FIELD_VECTOR(vectorF, mv_x, mv_y) {                               \
  Ipp32s x_l = mv_x;                                                          \
  Ipp32s y_l = mv_y;                                                          \
  Ipp32s i_c = (BlkWidth_c  == 16) ? i : (i >> 1);                            \
  Ipp32s j_c = (BlkHeight_c == 16) ? j : (j >> 1);                            \
  Ipp32s x_c = (BlkWidth_c  == 16) ? x_l : (x_l/2);                           \
  Ipp32s y_c = (BlkHeight_c == 16) ? y_l : (y_l/2);                           \
  vectorF->x = x_l;                                                           \
  vectorF->y = y_l;                                                           \
  vectorF->mctype_l = ((x_l & 1) << 3) | ((y_l & 1) << 2);                    \
  vectorF->mctype_c = ((x_c & 1) << 3) | ((y_c & 1) << 2);                    \
  vectorF->offset_l = (i   + (x_l >> 1)) + (j   + (y_l &~ 1) ) * YFrameHSize; \
  vectorF->offset_c = (i_c + (x_c >> 1)) + (j_c + (y_c &~ 1) ) * UVFrameHSize;\
}

#define OFF_Y    0
#define OFF_U  256
#define OFF_V  512

#define DCT_FRAME  0
#define DCT_FIELD  1

#define BlkWidth_l   16
#define BlkHeight_l  16
#define BlkStride_l  16

#define func_getdiff_frame_l  ippiGetDiff16x16_8u16s_C1
#define func_getdiff_field_l  ippiGetDiff16x8_8u16s_C1
#define func_getdiffB_frame_l ippiGetDiff16x16B_8u16s_C1
#define func_getdiffB_field_l ippiGetDiff16x8B_8u16s_C1
#define func_mc_frame_l  ippiMC16x16_8u_C1
#define func_mc_field_l  ippiMC16x8_8u_C1

#define VARMEAN_FRAME(pDiff, vardiff, meandiff, _vardiff)             \
  ippiVarSum8x8_16s32s_C1R(pDiff    , 32, &vardiff[0], &meandiff[0]); \
  ippiVarSum8x8_16s32s_C1R(pDiff+8  , 32, &vardiff[1], &meandiff[1]); \
  ippiVarSum8x8_16s32s_C1R(pDiff+128, 32, &vardiff[2], &meandiff[2]); \
  ippiVarSum8x8_16s32s_C1R(pDiff+136, 32, &vardiff[3], &meandiff[3]); \
  _vardiff = vardiff[0] + vardiff[1] + vardiff[2] + vardiff[3]

#define VARMEAN_FIELD(pDiff, vardiff, meandiff, _vardiff)             \
  ippiVarSum8x8_16s32s_C1R(pDiff   ,  64, &vardiff[0], &meandiff[0]); \
  ippiVarSum8x8_16s32s_C1R(pDiff+8 ,  64, &vardiff[1], &meandiff[1]); \
  ippiVarSum8x8_16s32s_C1R(pDiff+16,  64, &vardiff[2], &meandiff[2]); \
  ippiVarSum8x8_16s32s_C1R(pDiff+24,  64, &vardiff[3], &meandiff[3]); \
  _vardiff = vardiff[0] + vardiff[1] + vardiff[2] + vardiff[3]

#define VARMEAN_FRAME_Y(vardiff, meandiff, _vardiff)                                           \
  ippiVarSum8x8_8u32s_C1R(YBlock                    , YFrameHSize, &vardiff[0], &meandiff[0]); \
  ippiVarSum8x8_8u32s_C1R(YBlock + 8                , YFrameHSize, &vardiff[1], &meandiff[1]); \
  ippiVarSum8x8_8u32s_C1R(YBlock + 8*YFrameHSize    , YFrameHSize, &vardiff[2], &meandiff[2]); \
  ippiVarSum8x8_8u32s_C1R(YBlock + 8*YFrameHSize + 8, YFrameHSize, &vardiff[3], &meandiff[3]); \
  _vardiff = vardiff[0] + vardiff[1] + vardiff[2] + vardiff[3]

#define VARMEAN_FIELD_Y(vardiff, meandiff, _vardiff)                                           \
  ippiVarSum8x8_8u32s_C1R(YBlock,                   2*YFrameHSize, &vardiff[0], &meandiff[0]); \
  ippiVarSum8x8_8u32s_C1R(YBlock + 8,               2*YFrameHSize, &vardiff[1], &meandiff[1]); \
  ippiVarSum8x8_8u32s_C1R(YBlock + YFrameHSize,     2*YFrameHSize, &vardiff[2], &meandiff[2]); \
  ippiVarSum8x8_8u32s_C1R(YBlock + YFrameHSize + 8, 2*YFrameHSize, &vardiff[3], &meandiff[3]); \
  _vardiff = vardiff[0] + vardiff[1] + vardiff[2] + vardiff[3]

#define IF_GOOD_PRED(vardiff, meandiff)             \
  if(best->mb_type != MB_INTRA &&                   \
     vardiff[0] <= varThreshold &&     \
     vardiff[1] <= varThreshold &&     \
     vardiff[2] <= varThreshold &&     \
     vardiff[3] <= varThreshold)       \
    if(meandiff[0] < meanThreshold/8 &&       \
       meandiff[0] > -meanThreshold/8)        \
      if(meandiff[1] < meanThreshold/8 &&     \
         meandiff[1] > -meanThreshold/8)      \
        if(meandiff[2] < meanThreshold/8 &&   \
           meandiff[2] > -meanThreshold/8)    \
          if(meandiff[3] < meanThreshold/8 && \
             meandiff[3] > -meanThreshold/8)

#define GETDIFF_FRAME(X, CC, C, pDiff, DIR) \
  func_getdiff_frame_##C(                   \
    X##Block,                               \
    CC##FrameHSize,                         \
    X##RecFrame[pMBInfo[k].mv_field_sel[2][DIR]][DIR] + vector[2][DIR].offset_##C, \
    CC##FrameHSize,                         \
    pDiff + OFF_##X,                        \
    2*BlkStride_##C,                        \
    0, 0, vector[2][DIR].mctype_##C, 0)

#define GETDIFF_FRAME_FB(X, CC, C, pDiff)   \
  func_getdiffB_frame_##C(X##Block,         \
    CC##FrameHSize,                         \
    X##RecFrame[pMBInfo[k].mv_field_sel[2][0]][0] + vector[2][0].offset_##C, \
    CC##FrameHSize,                         \
    vector[2][0].mctype_##C,                \
    X##RecFrame[pMBInfo[k].mv_field_sel[2][1]][1] + vector[2][1].offset_##C, \
    CC##FrameHSize,                         \
    vector[2][1].mctype_##C,                \
    pDiff + OFF_##X,                        \
    2*BlkStride_##C,                        \
    ippRndZero)

#define GETDIFF_FIELD(X, CC, C, pDiff, DIR)            \
if (picture_structure == MPS_PROGRESSIVE) {              \
  func_getdiff_field_##C(X##Block,                     \
    2*CC##FrameHSize,                                  \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][DIR]][DIR] +vector[0][DIR].offset_##C, \
    2*CC##FrameHSize,                                  \
    pDiff + OFF_##X,                                   \
    4*BlkStride_##C,                                   \
    0, 0, vector[0][DIR].mctype_##C, 0);               \
                                                       \
  func_getdiff_field_##C(X##Block + CC##FrameHSize,    \
    2*CC##FrameHSize,                                  \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][DIR]][DIR] + vector[1][DIR].offset_##C, \
    2*CC##FrameHSize,                                  \
    pDiff + OFF_##X + BlkStride_##C,                   \
    4*BlkStride_##C,                                   \
    0, 0, vector[1][DIR].mctype_##C, 0);               \
} else {                                               \
  func_getdiff_field_##C(X##Block,                     \
    CC##FrameHSize,                                    \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][DIR]][DIR] + vector[0][DIR].offset_##C, \
    CC##FrameHSize,                                    \
    pDiff + OFF_##X,                                   \
    2*BlkStride_##C,                                   \
    0, 0, vector[0][DIR].mctype_##C, 0);               \
                                                       \
  func_getdiff_field_##C(X##Block + (BlkHeight_##C/2)*CC##FrameHSize, \
    CC##FrameHSize,                                    \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][DIR]][DIR] + vector[1][DIR].offset_##C + (BlkHeight_##C/2)*CC##FrameHSize, \
    CC##FrameHSize,                                    \
    pDiff + OFF_##X + (BlkHeight_##C/2)*BlkStride_##C, \
    2*BlkStride_##C,                                   \
    0, 0, vector[1][DIR].mctype_##C, 0);               \
}

#define GETDIFF_FIELD_FB(X, CC, C, pDiff)              \
if (picture_structure == MPS_PROGRESSIVE) {              \
  func_getdiffB_field_##C(X##Block,                    \
    2*CC##FrameHSize,                                  \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][0]][0] + vector[0][0].offset_##C, \
    2*CC##FrameHSize,                                  \
    vector[0][0].mctype_##C,                           \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][1]][1] + vector[0][1].offset_##C, \
    2*CC##FrameHSize,                                  \
    vector[0][1].mctype_##C,                           \
    pDiff + OFF_##X,                                   \
    4*BlkStride_##C,                                   \
    ippRndZero);                                       \
                                                       \
  func_getdiffB_field_##C(X##Block + CC##FrameHSize,   \
    2*CC##FrameHSize,                                  \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][0]][0] + vector[1][0].offset_##C, \
    2*CC##FrameHSize,                                  \
    vector[1][0].mctype_##C,                           \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][1]][1] + vector[1][1].offset_##C, \
    2*CC##FrameHSize,                                  \
    vector[1][1].mctype_##C,                           \
    pDiff + OFF_##X + BlkStride_##C,                   \
    4*BlkStride_##C,                                   \
    ippRndZero);                                       \
} else {                                               \
  func_getdiffB_field_##C(X##Block,                    \
    CC##FrameHSize,                                    \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][0]][0] + vector[0][0].offset_##C,\
    CC##FrameHSize,                                    \
    vector[0][0].mctype_##C,                           \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][1]][1] + vector[0][1].offset_##C,\
    CC##FrameHSize,                                    \
    vector[0][1].mctype_##C,                           \
    pDiff + OFF_##X,                                   \
    2*BlkStride_##C,                                   \
    ippRndZero);                                       \
                                                       \
  func_getdiffB_field_##C(                             \
    X##Block + (BlkHeight_##C/2)*CC##FrameHSize,       \
    CC##FrameHSize,                                    \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][0]][0] + vector[1][0].offset_##C + (BlkHeight_##C/2)*CC##FrameHSize,  \
    CC##FrameHSize,                                    \
    vector[1][0].mctype_##C,                           \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][1]][1] + vector[1][1].offset_##C + (BlkHeight_##C/2)*CC##FrameHSize, \
    CC##FrameHSize,                                    \
    vector[1][1].mctype_##C,                           \
    pDiff + OFF_##X + (BlkHeight_##C/2)*BlkStride_##C, \
    2*BlkStride_##C,                                   \
    ippRndZero);                                       \
}

#define MC_FRAME_F(X, CC, C, pDiff) \
  func_mc_frame_##C(                \
    X##RecFrame[pMBInfo[k].mv_field_sel[2][0]][0] + vector[2][0].offset_##C, \
    CC##FrameHSize,                 \
    pDiff + OFF_##X,                \
    2*BlkStride_##C,                \
    X##BlockRec,                    \
    CC##FrameHSize,                 \
    vector[2][0].mctype_##C,        \
    (IppRoundMode)0 )

#define MC_FIELD_F(X, CC, C, pDiff)                    \
if (picture_structure == MPS_PROGRESSIVE) {              \
  func_mc_field_##C(                                   \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][0]][0] + vector[0][0].offset_##C, \
    2*CC##FrameHSize,                                  \
    pDiff + OFF_##X,                                   \
    4*BlkStride_##C,                                   \
    X##BlockRec,                                       \
    2*CC##FrameHSize,                                  \
    vector[0][0].mctype_##C,                           \
    (IppRoundMode)0 );                                 \
                                                       \
  func_mc_field_##C(                                   \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][0]][0] + vector[1][0].offset_##C, \
    2*CC##FrameHSize,                                  \
    pDiff + OFF_##X + BlkStride_##C,                   \
    4*BlkStride_##C,                                   \
    X##BlockRec + CC##FrameHSize,                      \
    2*CC##FrameHSize,                                  \
    vector[1][0].mctype_##C,                           \
    (IppRoundMode)0 );                                 \
} else {                                               \
  func_mc_field_##C(                                   \
    X##RecFrame[pMBInfo[k].mv_field_sel[0][0]][0] + vector[0][0].offset_##C, \
    CC##FrameHSize,                                    \
    pDiff + OFF_##X,                                   \
    2*BlkStride_##C,                                   \
    X##BlockRec,                                       \
    CC##FrameHSize,                                    \
    vector[0][0].mctype_##C,                           \
    (IppRoundMode)0 );                                 \
                                                       \
  func_mc_field_##C(                                   \
    X##RecFrame[pMBInfo[k].mv_field_sel[1][0]][0] + vector[1][0].offset_##C + (BlkHeight_##C/2)*CC##FrameHSize, \
    CC##FrameHSize,                                    \
    pDiff + OFF_##X + (BlkHeight_##C/2)*BlkStride_##C, \
    2*BlkStride_##C,                                   \
    X##BlockRec + (BlkHeight_##C/2)*CC##FrameHSize,    \
    CC##FrameHSize,                                    \
    vector[1][0].mctype_##C,                           \
    (IppRoundMode)0 );                                 \
}

// Note: right and bottom borders are excluding

#define BOUNDS_H(DIR, xoff)                                              \
  me_bound_left[DIR] = 2*xoff - 2*pMotionData[B_count].searchRange[DIR][0];  \
  if (me_bound_left[DIR] < 0)                                            \
  {                                                                      \
    me_bound_left[DIR] = 0;                                              \
  }                                                                      \
  me_bound_right[DIR] = 2*xoff + 2*pMotionData[B_count].searchRange[DIR][0] - 1; \
  if (me_bound_right[DIR] > MBcountH*32 - 32) {                          \
    me_bound_right[DIR] = IPP_MAX(2*xoff, MBcountH*32 - 32);             \
  }                                                                      \
  me_bound_left[DIR] -= 2*xoff;                                          \
  me_bound_right[DIR] -= 2*xoff;

#define BOUNDS_V(DIR, yoff)                                              \
  me_bound_top[DIR] = 2*yoff - 2*pMotionData[B_count].searchRange[DIR][1]; \
  if( me_bound_top[DIR] < 0 )                                            \
  {                                                                      \
    me_bound_top[DIR] = 0;                                               \
  }                                                                      \
  me_bound_bottom[DIR] = 2*yoff + 2*pMotionData[B_count].searchRange[DIR][1] - 1; \
  if (me_bound_bottom[DIR] > 2*16*MBcountV - 32) {                       \
    me_bound_bottom[DIR] = IPP_MAX(2*yoff, 2*16*MBcountV - 32);          \
  }                                                                      \
  me_bound_top[DIR] -= 2*yoff;                                           \
  me_bound_bottom[DIR] -= 2*yoff;

// internal border for 16x8 prediction
#define BOUNDS_V_FIELD(DIR, yoff)                                            \
  me_bound_1_bottom[DIR] = 2*yoff + 2*pMotionData[B_count].searchRange[DIR][1] - 1; \
  if (me_bound_1_bottom[DIR] > 2*16*MBcountV - 2*8) {                        \
    me_bound_1_bottom[DIR] = IPP_MAX(2*yoff, 2*16*MBcountV - 2*8);           \
  }                                                                          \
  me_bound_2_top[DIR] = 2*yoff + 2*8 - 2*pMotionData[B_count].searchRange[DIR][1]; \
  if( me_bound_2_top[DIR] < 0 )                                              \
  {                                                                          \
    me_bound_2_top[DIR] = 0;                                                 \
  }                                                                          \
  me_bound_1_bottom[DIR] -= 2*yoff;                                          \
  me_bound_2_top[DIR] -= 2*yoff + 2*8;


#define ME_FRAME(DIR, vardiff_res, pDiff, dct_type)            \
{                                                              \
  Ipp32s vardiff_tmp[4], meandiff_tmp[4], _vardiff;            \
                                                               \
  pRef[0] = YRefFrame[curr_field][DIR] + cur_offset;           \
  pRef[1] = YRefFrame[1-curr_field][DIR] + cur_offset;         \
  pRec[0] = YRecFrame[curr_field][DIR] + cur_offset;           \
  pRec[1] = YRecFrame[1-curr_field][DIR] + cur_offset;         \
  dct_type = DCT_FRAME;                                        \
                                                               \
  vardiff_res = MotionEstimation_Frame( pRef[ipflag],          \
    pRec[ipflag],                                              \
    YFrameHSize,                                               \
    YBlock,                                                    \
    YFrameHSize,                                               \
    mean_frm,                                                  \
    curr->var,                                                 \
    curr->mean,                                                \
    me_bound_left[DIR],                                        \
    me_bound_right[DIR],                                       \
    me_bound_top[DIR],                                         \
    me_bound_bottom[DIR],                                      \
    threadSpec[numTh].PMV[0][DIR],                             \
    pMBInfo[k].MV[0][DIR],                                     \
    (j != start_y) ? pMBInfo[k - MBcountH].MV[0][DIR]          \
                   : MV_ZERO,                                  \
    &vector[2][DIR],                                           \
    &threadSpec[numTh],                                        \
    i, j,                                                      \
    &pMBInfo[k].mv_field_sel[2][DIR],                          \
    NULL, ipflag);                                             \
  if(picture_structure != MPS_PROGRESSIVE && !ipflag) {          \
    MotionEstimation_Frame( pRef[1],                           \
      pRec[1],                                                 \
      YFrameHSize,                                             \
      YBlock,                                                  \
      YFrameHSize,                                             \
      mean_frm,                                                \
      curr->var,                                               \
      curr->mean,                                              \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      me_bound_top[DIR],                                       \
      me_bound_bottom[DIR],                                    \
      threadSpec[numTh].PMV[1][DIR],                           \
      pMBInfo[k].MV[1][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[1][DIR]        \
                    : MV_ZERO,                                 \
      &vector[2][DIR],                                         \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[2][DIR],                        \
      &vardiff_res, 1);                                        \
  }                                                            \
  else if (!curr_frame_dct) {                                  \
    GETDIFF_FRAME(Y, Y, l, pDiff, DIR);                        \
                                                               \
    VARMEAN_FIELD(pDiff, vardiff_tmp, meandiff_tmp, _vardiff); \
    {                                                          \
      Ipp32s var_fld = 0, var = 0;                             \
      ippiFrameFieldSAD16x16_16s32s_C1R(pDiff, BlkStride_l*2, &var, &var_fld);  \
      if (var_fld < var) {                                     \
        dct_type = DCT_FIELD;                                  \
        vardiff_res = _vardiff;                                \
        ippsCopy_8u((Ipp8u*)vardiff_tmp, (Ipp8u*)curr->var, sizeof(curr->var)); \
        ippsCopy_8u((Ipp8u*)meandiff_tmp, (Ipp8u*)curr->mean, sizeof(curr->mean)); \
      }                                                        \
    }                                                          \
  }                                                            \
}

#define ME_FIELD(DIR, vardiff_fld, pDiff, dct_type)            \
{                                                              \
  Ipp32s vardiff_tmp[4], meandiff_tmp[4], _vardiff;            \
  Ipp32s diff_f[2][2]; /* [srcfld][reffld] */                  \
                                                               \
  if (picture_structure == MPS_PROGRESSIVE) {                    \
    diff_f[0][0] = MotionEstimation_Field( pRef[0],            \
      pRec[0],                                                 \
      2*YFrameHSize,                                           \
      YBlock,                                                  \
      2*YFrameHSize,                                           \
      mean_fld,                                                \
      curr->var,                                               \
      curr->mean,                                              \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      (me_bound_top[DIR] + 0) >> 1,                            \
      me_bound_bottom[DIR] >> 1,                               \
      threadSpec[numTh].PMV[0][DIR],                           \
      pMBInfo[k].MV[0][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[0][DIR]        \
                     : MV_ZERO,                                \
      &vector[0][DIR],                                         \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[0][DIR],                        \
      NULL, 0);                                                \
    diff_f[0][1] = MotionEstimation_Field( pRef[1],            \
      pRec[1],                                                 \
      2*YFrameHSize,                                           \
      YBlock,                                                  \
      2*YFrameHSize,                                           \
      mean_fld,                                                \
      curr->var,                                               \
      curr->mean,                                              \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      (me_bound_top[DIR] + 0) >> 1,                            \
      me_bound_bottom[DIR] >> 1,                               \
      threadSpec[numTh].PMV[0][DIR],                           \
      pMBInfo[k].MV[0][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[0][DIR]        \
                     : MV_ZERO,                                \
      &vector[0][DIR],                                         \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[0][DIR],                        \
      &diff_f[0][0], 1);                                       \
                                                               \
    diff_f[1][0] = MotionEstimation_Field( pRef[0],            \
      pRec[0],                                                 \
      2*YFrameHSize,                                           \
      YBlock + YFrameHSize,                                    \
      2*YFrameHSize,                                           \
      mean_fld + 2,                                            \
      curr->var + 2,                                           \
      curr->mean + 2,                                          \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      (me_bound_top[DIR] + 0) >> 1,                            \
      me_bound_bottom[DIR] >> 1,                               \
      threadSpec[numTh].PMV[1][DIR],                           \
      pMBInfo[k].MV[1][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[1][DIR]        \
                     : MV_ZERO,                                \
      &vector[1][DIR],                                         \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[1][DIR],                        \
      NULL, 0);                                                \
    diff_f[1][1] = MotionEstimation_Field( pRef[1],            \
      pRec[1],                                                 \
      2*YFrameHSize,                                           \
      YBlock + YFrameHSize,                                    \
      2*YFrameHSize,                                           \
      mean_fld + 2,                                            \
      curr->var + 2,                                           \
      curr->mean + 2,                                          \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      (me_bound_top[DIR] + 0) >> 1,                            \
      me_bound_bottom[DIR] >> 1,                               \
      threadSpec[numTh].PMV[1][DIR],                           \
      pMBInfo[k].MV[1][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[1][DIR]        \
                     : MV_ZERO,                                \
      &vector[1][DIR],                                         \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[1][DIR],                        \
      &diff_f[1][0], 1);                                       \
    vardiff_fld = diff_f[0][0] + diff_f[1][0];                 \
    dct_type = DCT_FIELD;                                      \
                                                               \
  } else {                                                     \
    diff_f[0][0] = MotionEstimation_FieldPict(pRef[ipflag],    \
      pRec[ipflag],                                            \
      YFrameHSize,                                             \
      YBlock,                                                  \
      YFrameHSize,                                             \
      mean_frm,                                                \
      curr->var,                                               \
      curr->mean,                                              \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      me_bound_top[DIR],                                       \
      me_bound_1_bottom[DIR],                                  \
      threadSpec[numTh].PMV[0][DIR],                           \
      pMBInfo[k].MV[0][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[0][DIR]        \
                     : MV_ZERO,                                \
      &(vector[0][DIR]),                                       \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[0][DIR],                        \
      NULL, ipflag);                                           \
    if(!ipflag)                                                \
     diff_f[0][1] = MotionEstimation_FieldPict(pRef[1],        \
      pRec[1],                                                 \
      YFrameHSize,                                             \
      YBlock,                                                  \
      YFrameHSize,                                             \
      mean_frm,                                                \
      curr->var,                                               \
      curr->mean,                                              \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      me_bound_top[DIR],                                       \
      me_bound_1_bottom[DIR],                                  \
      threadSpec[numTh].PMV[0][DIR],                           \
      pMBInfo[k].MV[0][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[0][DIR]        \
                     : MV_ZERO,                                \
      &(vector[0][DIR]),                                       \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[0][DIR],                        \
      &diff_f[0][0], 1);                                       \
    diff_f[1][0] = MotionEstimation_FieldPict(pRef[ipflag] + 8*YFrameHSize, \
      pRec[ipflag] + 8*YFrameHSize,                            \
      YFrameHSize,                                             \
      YBlock + 8*YFrameHSize,                                  \
      YFrameHSize,                                             \
      mean_frm + 2,                                            \
      curr->var + 2,                                           \
      curr->mean + 2,                                          \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      me_bound_2_top[DIR],                                     \
      me_bound_bottom[DIR],                                    \
      threadSpec[numTh].PMV[1][DIR],                           \
      pMBInfo[k].MV[1][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[1][DIR]        \
                     : MV_ZERO,                                \
      &(vector[1][DIR]),                                       \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[1][DIR],                        \
      NULL, ipflag);                                           \
    if(!ipflag)                                                \
     diff_f[1][1] = MotionEstimation_FieldPict(pRef[1] + 8*YFrameHSize, \
      pRec[1] + 8*YFrameHSize,                                 \
      YFrameHSize,                                             \
      YBlock + 8*YFrameHSize,                                  \
      YFrameHSize,                                             \
      mean_frm + 2,                                            \
      curr->var + 2,                                           \
      curr->mean + 2,                                          \
      me_bound_left[DIR],                                      \
      me_bound_right[DIR],                                     \
      me_bound_2_top[DIR],                                     \
      me_bound_bottom[DIR],                                    \
      threadSpec[numTh].PMV[1][DIR],                           \
      pMBInfo[k].MV[1][DIR],                                   \
      (j != start_y) ? pMBInfo[k - MBcountH].MV[1][DIR]        \
                     : MV_ZERO,                                \
      &(vector[1][DIR]),                                       \
      &threadSpec[numTh],                                      \
      i, j,                                                    \
      &pMBInfo[k].mv_field_sel[1][DIR],                        \
      &diff_f[1][0], 1);                                       \
    vardiff_fld = diff_f[0][0] + diff_f[1][0];                 \
    dct_type = DCT_FRAME;                                      \
                                                               \
  }                                                            \
  GETDIFF_FIELD(Y, Y, l, pDiff, DIR);                          \
                                                               \
  if( picture_structure == MPS_PROGRESSIVE ) {                   \
    VARMEAN_FRAME(pDiff, vardiff_tmp, meandiff_tmp, _vardiff); \
    {                                                          \
      Ipp32s var_fld = 0, var = 0;                             \
      ippiFrameFieldSAD16x16_16s32s_C1R(pDiff, BlkStride_l*2, &var, &var_fld); \
      if (var < var_fld) {                                     \
        dct_type = DCT_FRAME;                                  \
        vardiff_fld = _vardiff;                                \
        ippsCopy_8u((Ipp8u*)vardiff_tmp, (Ipp8u*)curr->var, sizeof(curr->var)); \
        ippsCopy_8u((Ipp8u*)meandiff_tmp, (Ipp8u*)curr->mean, sizeof(curr->mean)); \
      }                                                        \
    }                                                          \
    /*if(_vardiff <= vardiff_fld) {                            \
      dct_type = DCT_FRAME;                                    \
      vardiff_fld = _vardiff;                                  \
    }*/                                                        \
  }                                                            \
}

/*********************************************************/

#ifdef _BIG_ENDIAN_
#define BSWAP(x) (x)
#else // Little endian
#define BSWAP(x) (Ipp32u)(((x) << 24) + (((x)&0xff00) << 8) + (((x) >> 8)&0xff00) + ((x) >> 24))
#endif

// to suppress "conditional expression is constant" warning
#if defined(_DEBUG) || defined(MPEG2_ASSERT) || defined(MPEG2_DEBUG_CODE)
  #define CHECK_ippiPutBits(offset,val,len) {      \
    Ipp32s _len = len;                             \
    mpeg2_assert((offset) >= 0 && (offset) <= 32); \
    mpeg2_assert((_len) > 0 && (_len) < 32);       \
    mpeg2_assert(((val) & (-1 << (_len))) == 0);   \
  }
#else
  #define CHECK_ippiPutBits(offset,val,len)
#endif

#define ippiPutBits(pBitStream,offset,val,len)        \
 {                                                    \
    Ipp32s tmpcnt;                                    \
    Ipp32u r_tmp;                                     \
    CHECK_ippiPutBits(offset,val,len)                 \
    tmpcnt = (offset) - (len);                        \
    if(tmpcnt < 0)                                    \
    {                                                 \
      r_tmp = (pBitStream)[0] | ((val) >> (-tmpcnt)); \
      (pBitStream)[0] = BSWAP(r_tmp);                 \
      (pBitStream)++;                                 \
      (pBitStream)[0] = (val) << (32 + tmpcnt);       \
      (offset) = 32 + tmpcnt;                         \
    }                                                 \
    else                                              \
    {                                                 \
      (pBitStream)[0] |= (val) << tmpcnt;             \
      (offset) = tmpcnt;                              \
    }                                                 \
}

#define SET_BUFFER(bb, ptr, len)  \
  bb.start_pointer = ptr;         \
  *(Ipp32u*)bb.start_pointer = 0; \
  bb.bit_offset = 32;             \
  bb.bytelen = len;               \
  bb.current_pointer = (Ipp32u*)bb.start_pointer;

#define FLUSH_BITSTREAM(pBitStream, offset)   \
  if (offset != 32) {                         \
    (pBitStream)[0] = BSWAP((pBitStream)[0]); \
    offset &= ~7;                             \
  }

#define BITPOS(bs) \
  ((Ipp32s)((Ipp8u*)bs.bBuf.current_pointer - (Ipp8u*)bs.bBuf.start_pointer)*8 \
  + 32 - bs.bBuf.bit_offset)

#define CHECK_BUFFER

#define PUT_BITS_THREAD(nTh, VAL, NBITS)       \
  ippiPutBits(threadSpec[nTh].bBuf.current_pointer, \
              threadSpec[nTh].bBuf.bit_offset,      \
              VAL, NBITS)

#define PUT_BITS_TH(VAL, NBITS) \
  PUT_BITS_THREAD(numTh, VAL, NBITS)

#define PUT_BITS(VAL, NBITS) \
  PUT_BITS_THREAD(0, VAL, NBITS)

#define PUT_ALIGNMENT(nTh) \
  threadSpec[nTh].bBuf.bit_offset &= ~7;

#define PUT_START_CODE_TH(th,scode) {                    \
  Ipp32u code1, code2;                                   \
  Ipp32s off = threadSpec[th].bBuf.bit_offset &~ 7;      \
  code1 = threadSpec[th].bBuf.current_pointer[0];        \
  code2 = 0;                                             \
  if(off > 0) code1 |= (scode) >> (32-off);              \
  if(off < 32) code2 = (scode) << off;                   \
  threadSpec[th].bBuf.current_pointer[0] = BSWAP(code1); \
  threadSpec[th].bBuf.current_pointer++;                 \
  threadSpec[th].bBuf.current_pointer[0] = code2;        \
  threadSpec[th].bBuf.bit_offset = off;                  \
}

#define PUT_START_CODE(scode) PUT_START_CODE_TH(0,scode)

#define PUT_MB_TYPE(PictureType, MOTION_TYPE) {         \
  Ipp32s mb_type = MOTION_TYPE;                         \
  if (CodedBlockPattern) mb_type |= MB_PATTERN;         \
  PUT_BITS_TH(mbtypetab[PictureType - MPEG2_I_PICTURE][mb_type].code, \
              mbtypetab[PictureType - MPEG2_I_PICTURE][mb_type].len); \
}

#define PUT_BLOCK_PATTERN(CodedBlockPattern) \
if (CodedBlockPattern) { \
  Ipp32s extra_bits = block_count - 6; \
  Ipp32s cbp6 = (CodedBlockPattern >> extra_bits) & 63; \
  PUT_BITS_TH(CBP_VLC_Tbl[cbp6].code, CBP_VLC_Tbl[cbp6].len); \
  if (extra_bits) { \
    PUT_BITS_TH(CodedBlockPattern & ((1 << extra_bits) - 1), extra_bits); \
  } \
}

#define PUT_MB_MODES(PictureType, MOTION_TYPE) \
{ \
  PUT_MB_TYPE(PictureType, MOTION_TYPE) \
  \
  if (picture_structure != MPS_PROGRESSIVE) { \
    /* (3 - x) is: MC_FRAME -> MC_FIELD, MC_FIELD -> MC_16X8 */ \
    PUT_BITS_TH(3 - pMBInfo[k].prediction_type, 2); \
  } else \
  if (!curr_frame_dct) { \
    PUT_BITS_TH(pMBInfo[k].prediction_type, 2); \
    if (CodedBlockPattern) { \
      PUT_BITS_TH(pMBInfo[k].dct_type, 1); \
    } \
  } \
}

#define PUT_MB_MODE_NO_MV(PictureType) \
{ \
  PUT_MB_TYPE(PictureType, 0) \
  \
  if (picture_structure == MPS_PROGRESSIVE && !curr_frame_dct) { \
    PUT_BITS_TH(pMBInfo[k].dct_type, 1); \
  } \
}

enum MPEGPictureStructure
{
    MPS_TOP_FIELD                = 1,
    MPS_BOTTOM_FIELD             = 2,
    MPS_PROGRESSIVE              = 3,
    MPS_TOP_FIELD_FIRST          = 7,
    MPS_BOTTOM_FIELD_FIRST       = 11
};

#endif // __UMC_MPEG2_ENC_DEFS_H
