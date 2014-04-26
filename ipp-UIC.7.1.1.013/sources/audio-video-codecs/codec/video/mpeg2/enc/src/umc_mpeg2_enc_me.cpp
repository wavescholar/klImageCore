/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_ENCODER

#include "vm_time.h"
#include "umc_mpeg2_enc_defs.h"

using namespace UMC;


#define SAD_FUNC_16x16 \
  ippiSAD16x16_8u32s(pBlock,     \
                     BlockStep,  \
                     ref_data,   \
                     RefStep,    \
                     &MAD,       \
                     flag)

#define SAD_FUNC_16x8 \
  ippiSAD16x8_8u32s_C1R(pBlock,      \
                        BlockStep,   \
                        ref_data,    \
                        RefStep,     \
                        &MAD,        \
                        flag)

#define SAD_FUNC_16x16_fullpix \
  SAD_FUNC_16x16

#define SAD_FUNC_16x8_fullpix \
  SAD_FUNC_16x8

#define VARMEAN_FUNC_16x16                  \
  ippiVarMeanDiff16x16_8u32s_C1R(           \
                          pBlock,           \
                          BlockStep,        \
                          ref_data,         \
                          RefStep,          \
                          pSrcMean,         \
                          Var[!min_index],  \
                          Mean[!min_index], \
                          flag);            \
  MAD = Var[!min_index][0] + Var[!min_index][1] + \
        Var[!min_index][2] + Var[!min_index][3]

#define VARMEAN_FUNC_16x8                   \
  ippiVarMeanDiff16x8_8u32s_C1R(            \
                          pBlock,           \
                          BlockStep,        \
                          ref_data,         \
                          RefStep,          \
                          pSrcMean,         \
                          Var[!min_index],  \
                          Mean[!min_index], \
                          flag);            \
  MAD = Var[!min_index][0] + Var[!min_index][1]

#define CMP_MAD                                            \
/*MAD += mp2_WeightMV(XN - InitialMV0.x,                     \
                    YN - (InitialMV0.y >> FIELD_FLAG),     \
                    mp_f_code[0][0],                       \
                    quantiser_scale_code); */                \
if(MAD < BestMAD)                                          \
{                                                          \
  min_point = 1;                                           \
  BestMAD = MAD;                                           \
  min_index = !min_index;                                  \
  XMIN = XN;                                               \
  YMIN = YN;                                               \
}
/***********************************************************/

#define ME_FUNC     MotionEstimation_Frame
#define FIELD_FLAG  0
#define ME_STEP_X   2
#define ME_STEP_Y   2
#define ME_MIN_STEP 2
#define NUM_BLOCKS  4

#define TRY_POSITION_REC(FUNC)                               \
if (me_matrix[YN*me_matrix_w + XN] != me_matrix_id)          \
{                                                            \
  me_matrix[YN*me_matrix_w + XN] = (Ipp8u)me_matrix_id;      \
  ref_data = (pRecFld + (XN >> 1) + (YN >> 1)*RefStep);      \
  flag = ((XN & 1) << 3) | ((YN & 1) << 2);                  \
                                                             \
  FUNC##_16x16;                                              \
                                                             \
  CMP_MAD                                                    \
}

#define TRY_POSITION_REF(FUNC)                               \
if (me_matrix[YN*me_matrix_w + XN] != me_matrix_id)          \
{                                                            \
  me_matrix[YN*me_matrix_w + XN] = (Ipp8u)me_matrix_id;      \
  ref_data = (pRefFld + (XN >> 1) + (YN >> 1)*RefStep);      \
  flag = 0;                                                  \
                                                             \
  FUNC##_16x16_fullpix;                                      \
                                                             \
  CMP_MAD                                                    \
}

#include "umc_mpeg2_enc_me.h"

/***********************************************************/

#undef  ME_FUNC
#define ME_FUNC  MotionEstimation_FieldPict

#undef  NUM_BLOCKS
#define NUM_BLOCKS  2

#undef  TRY_POSITION_REC
#define TRY_POSITION_REC(FUNC) \
if (me_matrix[YN*me_matrix_w + XN] != me_matrix_id)     \
{                                                       \
  me_matrix[YN*me_matrix_w + XN] = (Ipp8u)me_matrix_id; \
  ref_data = (pRecFld + (XN >> 1) + (YN >> 1)*RefStep); \
  flag = ((XN & 1) << 3) | ((YN & 1) << 2);             \
                                                        \
  FUNC##_16x8;                                          \
                                                        \
  CMP_MAD                                               \
}

#undef  TRY_POSITION_REF
#define TRY_POSITION_REF(FUNC) \
if (me_matrix[YN*me_matrix_w + XN] != me_matrix_id)     \
{                                                       \
  me_matrix[YN*me_matrix_w + XN] = (Ipp8u)me_matrix_id; \
  ref_data = (pRefFld + (XN >> 1) + (YN >> 1)*RefStep); \
  flag = 0;                                             \
                                                        \
  FUNC##_16x8_fullpix;                                  \
                                                        \
  CMP_MAD                                               \
}

#include "umc_mpeg2_enc_me.h"

/***********************************************************/

#undef  ME_FUNC
#define ME_FUNC  MotionEstimation_Field

#undef  FIELD_FLAG
#define FIELD_FLAG  1

#undef  NUM_BLOCKS
#define NUM_BLOCKS  2

#undef  TRY_POSITION_REC
#define TRY_POSITION_REC(FUNC) \
if (me_matrix[YN*me_matrix_w + XN] != me_matrix_id)      \
{                                                        \
  me_matrix[YN*me_matrix_w + XN] = (Ipp8u)me_matrix_id;  \
  ref_data = (pRecFld + (XN >> 1) + (YN >> 1)*RefStep);  \
  flag = ((XN & 1) << 3) | ((YN & 1) << 2);              \
                                                         \
  FUNC##_16x8;                                           \
                                                         \
  CMP_MAD                                                \
}

#undef  TRY_POSITION_REF
#define TRY_POSITION_REF(FUNC) \
if (me_matrix[YN*me_matrix_w + XN] != me_matrix_id)      \
{                                                        \
  me_matrix[YN*me_matrix_w + XN] = (Ipp8u)me_matrix_id;  \
  ref_data = (pRefFld + (XN >> 1) + (YN >> 1)*RefStep);  \
  flag = 0;                                              \
                                                         \
  FUNC##_16x8_fullpix;                                   \
                                                         \
  CMP_MAD                                                \
}

#include "umc_mpeg2_enc_me.h"

/***********************************************************/


void MPEG2VideoEncoderBase::AdjustSearchRange(Ipp32s B_count, Ipp32s direction)
{
  Ipp32s fcRangeMin, fcRangeMax, flRangeMin, flRangeMax, co, ci, cnz, r, i;

  fcRangeMax = IPP_MAX(
    pMotionData[B_count].searchRange[direction][0],
    pMotionData[B_count].searchRange[direction][1]);
  fcRangeMin = -fcRangeMax;
  flRangeMax = fcRangeMax >> 1;
  flRangeMin = -flRangeMax;
  co = ci = cnz = 0;
  r = 4;
  //r = flRangeMax / 2;
  if (r < 2) r = 2;
  for (i = 0; i < (Ipp32s)(encodeInfo.m_info.videoInfo.m_iHeight*encodeInfo.m_info.videoInfo.m_iWidth/256); i ++) {
    if (!(pMBInfo[i].mb_type & MB_INTRA)) {
      Ipp32s dx = pMBInfo[i].MV[0][0].x;
      Ipp32s dy = pMBInfo[i].MV[0][0].y;
      if (dx >= (fcRangeMax - r) || dx < (fcRangeMin + r) || dy >= (fcRangeMax - r) || dy < (fcRangeMin + r))
        co ++;
      if (dx >= (flRangeMax - r) || dx < (flRangeMin + r) || dy >= (flRangeMax - r) || dy < (flRangeMin + r))
        ci ++;
      if (dx != 0 || dy != 0)
        cnz ++;
    }
  }
  if (cnz == 0) {
    return;
  }
  //printf("%.2f, %.2f\n", co*100.0/cnz, cnz/(ci*400.0));
  if (co * 100 >= cnz) {
    if (fcRangeMax <= 32) {
      fcRangeMax *= 2;
      //printf("%d: mul: %d\n", encodeInfo.numEncodedFrames, fcRangeMax);
    }
  } else if (ci * 100 < cnz) {
    if (fcRangeMax > 4) {
      fcRangeMax /= 2;
      //printf("%d: div: %d\n", encodeInfo.numEncodedFrames, fcRangeMax);
    }
  }
  pMotionData[B_count].searchRange[direction][0] = fcRangeMax;
  pMotionData[B_count].searchRange[direction][1] = fcRangeMax;

  Ipp32s req_f_code = 1;
  while ((4<<req_f_code) < fcRangeMax) {
    req_f_code++;
  }
  pMotionData[B_count].f_code[direction][0] = req_f_code;
  pMotionData[B_count].f_code[direction][1] = req_f_code;
}


#endif // UMC_ENABLE_MPEG2_VIDEO_ENCODER
