/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_VIDEO_DECODER

#include "umc_mpeg2_dec_base.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4244)
#endif

using namespace UMC;

#define DECL_MV_DERIVED(k) \
    Ipp16s *c_mv     = &video->vector[2*(k)]; \
    Ipp16s *c_pmv    = &video->PMV[2*(k)];

Status MPEG2VideoDecoderBase::update_mv(Ipp16s *pval, Ipp32s s, Mpeg2VideoContext *video)
{
  Ipp32s val = *pval;
  Ipp32s residual, mcode;
  DECODE_VLC(mcode, video->bs, vlcMotionVector);
  UPDATE_MV(val, mcode, s);
  *pval = val;
  return UMC_OK;
}

Status MPEG2VideoDecoderBase::mv_decode(Ipp32s r, Ipp32s s, Mpeg2VideoContext *video)
{
    DECL_MV_DERIVED(2*r+s)
    Ipp32s residual, mcode;
    Ipp32s framefieldflag = (video->prediction_type == IPPVC_MC_FIELD) &&
                            (PictureHeader.picture_structure == FRAME_PICTURE);

    // Decode x vector
    c_mv[0] = c_pmv[0];
    if (IS_NEXTBIT1(video->bs)) {
      SKIP_BITS(video->bs, 1)
    } else {
      DECODE_VLC(mcode, video->bs, vlcMotionVector);
      UPDATE_MV(c_mv[0], mcode, 2*s);
      c_pmv[0] = c_mv[0];
    }

    // Decode y vector
    c_mv[1] = c_pmv[1] >> framefieldflag; // frame y to field
    if (IS_NEXTBIT1(video->bs)) {
      SKIP_BITS(video->bs, 1)
    } else {
      DECODE_VLC(mcode, video->bs, vlcMotionVector);
      UPDATE_MV(c_mv[1], mcode, 2*s + 1);
    }
    c_pmv[1] = c_mv[1] << framefieldflag; // field y back to frame

    return UMC_OK;
}

Status MPEG2VideoDecoderBase::mv_decode_dp(Mpeg2VideoContext *video)
{
  DECL_MV_DERIVED(0)
  Ipp32s residual, mcode;
  Ipp32s dmv0 = 0, dmv1 = 0;
  Ipp32s ispos0, ispos1;

  // Decode x vector
  DECODE_VLC(mcode, video->bs, vlcMotionVector);
  c_mv[0] = c_pmv[0];
  if(mcode) {
    UPDATE_MV(c_mv[0], mcode, 0);
    c_pmv[0] = c_mv[0];
  }
  //get dmv0
  GET_1BIT(video->bs, dmv0);
  if(dmv0 == 1)
  {
    GET_1BIT(video->bs, dmv0);
    dmv0 = (dmv0 == 1)?-1:1;
  }

  // Decode y vector
  if (PictureHeader.picture_structure == FRAME_PICTURE)
    c_pmv[1] >>= 1;
  DECODE_VLC(mcode, video->bs, vlcMotionVector);
  c_mv[1] = c_pmv[1];
  if(mcode) {
    UPDATE_MV(c_mv[1], mcode, 1);
    c_pmv[1] = c_mv[1];
  }
  //get dmv1
  GET_1BIT(video->bs, dmv1);
  if(dmv1 == 1)
  {
    GET_1BIT(video->bs, dmv1);
    dmv1 = (dmv1 == 1)?-1:1;
  }

  //Dual Prime Arithmetic
  ispos0 = c_pmv[0]>=0;
  ispos1 = c_pmv[1]>=0;
  if (PictureHeader.picture_structure == FRAME_PICTURE)
  {
    if (PictureHeader.top_field_first    )
    {
      /* vector for prediction of top field from bottom field */
      c_mv[4] = ((c_pmv[0] + ispos0)>>1) + dmv0;
      c_mv[5] = ((c_pmv[1] + ispos1)>>1) + dmv1 - 1;

      /* vector for prediction of bottom field from top field */
      c_mv[6] = ((3*c_pmv[0] + ispos0)>>1) + dmv0;
      c_mv[7] = ((3*c_pmv[1] + ispos1)>>1) + dmv1 + 1;
    }
    else
    {
      /* vector for prediction of top field from bottom field */
      c_mv[4] = ((3*c_pmv[0] + ispos0)>>1) + dmv0;
      c_mv[5] = ((3*c_pmv[1] + ispos1)>>1) + dmv1 - 1;

      /* vector for prediction of bottom field from top field */
      c_mv[6] = ((c_pmv[0] + ispos0)>>1) + dmv0;
      c_mv[7] = ((c_pmv[1] + ispos1)>>1) + dmv1 + 1;
    }

    c_pmv[1] <<= 1;
    c_pmv[4] = c_pmv[0];
    c_pmv[5] = c_pmv[1];
  }
  else
  {
    //Dual Prime Arithmetic
    // vector for prediction from field of opposite parity
    c_mv[4] = ((c_pmv[0]+ispos0)>>1) + dmv0;
    c_mv[5] = ((c_pmv[1]+ispos1)>>1) + dmv1;

    /* correct for vertical field shift */
    if (PictureHeader.picture_structure == TOP_FIELD)
      c_mv[5]--;
    else
      c_mv[5]++;
  }

  return UMC_OK;
}

#endif // UMC_ENABLE_MPEG2_VIDEO_DECODER
