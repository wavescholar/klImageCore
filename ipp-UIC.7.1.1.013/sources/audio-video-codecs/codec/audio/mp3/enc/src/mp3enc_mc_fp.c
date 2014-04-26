/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "mp3enc_own_fp.h"

void mp3enc_mc_normalizing (MP3Enc *state)
{
  Ipp32f norm_all = 0.0f;
  Ipp32f norm_c = 0.0f;
  Ipp32f norm_lsrs = 0.0f;
  int stereo = state->com.stereo;

  switch (state->com.mc_header.dematrix_procedure)
  {
  case 0:
  case 2:
    norm_all  = 0.414213562f; /* 1/(1+sqrt(2)) */
    norm_c  = 0.707106781f; /* 1/sqrt(2) */
    norm_lsrs  = 0.707106781f; /* 1/sqrt(2) */
    break;
  case 1:
    norm_all  = 0.453081839f; /* 1/(1.5+0.5*sqrt(2)) */
    norm_c  = 0.707106781f; /* 1/sqrt(2) */
    norm_lsrs  = 0.5f;
    break;
  case 3:
    return;
  }

  ippsMulC_32f_I(norm_all, &state->fbout_data[0][0][0][0], 1152 << (stereo - 1));

  if (state->com.mc_channel_conf == 320)
  {
    ippsMulC_32f_I(norm_all * norm_c, &state->fbout_data[2][0][0][0], 1152);
    ippsMulC_32f_I(norm_all * norm_lsrs, &state->fbout_data[3][0][0][0], 1152);
    ippsMulC_32f_I(norm_all * norm_lsrs, &state->fbout_data[4][0][0][0], 1152);
  }
  else if (state->com.mc_channel_conf == 310)
  {
    ippsMulC_32f_I(norm_all * norm_c, &state->fbout_data[2][0][0][0], 1152);
    ippsMulC_32f_I(norm_all * norm_lsrs, &state->fbout_data[3][0][0][0], 1152);
  }
  else if (state->com.mc_channel_conf == 220)
  {
    ippsMulC_32f_I(norm_all * norm_lsrs, &state->fbout_data[2][0][0][0], 1152);
    ippsMulC_32f_I(norm_all * norm_lsrs, &state->fbout_data[3][0][0][0], 1152);
  }
  else if (state->com.mc_channel_conf == 300 ||
    state->com.mc_channel_conf == 302){
    ippsMulC_32f_I(norm_all * norm_c, &state->fbout_data[2][0][0][0], 1152);
  }
  else if (state->com.mc_channel_conf == 210)
    ippsMulC_32f_I(norm_all * norm_lsrs, &state->fbout_data[2][0][0][0], 1152);
}

void mp3enc_mc_matricing (MP3Enc *state)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, tbuf, 1152);
  Ipp32f matr_c = 0.0;
  Ipp32f tmp;
  int j, k, l;

  switch (state->com.mc_header.dematrix_procedure)
  {
  case 0:
  case 1:
  case 2:
    matr_c  = 1;
    break;
  case 3:
    matr_c  = 0.707106781f; /* 1/sqrt(2) */
    break;
  }

  if (state->com.mc_channel_conf == 320)
  {
    if (state->com.mc_header.dematrix_procedure == 0 ||
      state->com.mc_header.dematrix_procedure == 1)
    {
/*      for (j = 0; j < 3; ++j)
        for (l = 0; l < 12; l ++)
          for (k = 0; k < 32; k ++) {
            Ipp32f val, valL, valR;
            Ipp32f (*sb_sample)[3][12][32] = state->fbout_data;
      valL = sb_sample[0][j][l][k];
      valR = sb_sample[1][j][l][k];
      sb_sample[0][j][l][k] = valL + (sb_sample[2][j][l][k] + sb_sample[3][j][l][k]);
      sb_sample[1][j][l][k] = valR + (sb_sample[2][j][l][k] + sb_sample[4][j][l][k]);
        }*/
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[3][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[1][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[4][0][0][0], &state->fbout_data[1][0][0][0], 1152);
    }
    else if (state->com.mc_header.dematrix_procedure == 2)
    {
      ippsAdd_32f(&state->fbout_data[3][0][0][0], &state->fbout_data[4][0][0][0], tbuf, 1152);
      ippsMulC_32f_I(0.5f, tbuf, 1152);
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsSub_32f_I(tbuf, &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[1][0][0][0], 1152);
      ippsAdd_32f_I(tbuf, &state->fbout_data[1][0][0][0], 1152);
    }
  }
  else if (state->com.mc_channel_conf == 310)
  {
    if (state->com.mc_header.dematrix_procedure == 0 ||
      state->com.mc_header.dematrix_procedure == 1)
    {
      ippsAdd_32f(&state->fbout_data[2][0][0][0], &state->fbout_data[3][0][0][0], tbuf, 1152);
      ippsAdd_32f_I(tbuf, &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(tbuf, &state->fbout_data[1][0][0][0], 1152);
    }
    else if (state->com.mc_header.dematrix_procedure == 2)
    {
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsSub_32f_I(&state->fbout_data[3][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[1][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[3][0][0][0], &state->fbout_data[1][0][0][0], 1152);
    }
  }
  else if (state->com.mc_channel_conf == 220)
  {
    if (state->com.mc_header.dematrix_procedure == 0 ||
      state->com.mc_header.dematrix_procedure == 1)
    {
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[3][0][0][0], &state->fbout_data[1][0][0][0], 1152);
    }
  }
  else if (state->com.mc_channel_conf == 300 ||
    state->com.mc_channel_conf == 302)
  {
    if (state->com.mc_header.dematrix_procedure == 0 ||
      state->com.mc_header.dematrix_procedure == 1)
    {
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[1][0][0][0], 1152);
    }
  }
  else if (state->com.mc_channel_conf == 210)
  {
    if (state->com.mc_header.dematrix_procedure == 0 ||
      state->com.mc_header.dematrix_procedure == 1)
    {
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[0][0][0][0], 1152);
      ippsAdd_32f_I(&state->fbout_data[2][0][0][0], &state->fbout_data[1][0][0][0], 1152);
    }
  }

  if (state->com.mc_header.center == 3 &&
    state->com.mc_header.dematrix_procedure == 3)
  {
      for (j = 0; j < 2; j++)
        for (l = 0; l < 18; l++)
          for (k = 12; k < 32; k ++) {
            tmp = matr_c * state->fbout_data[2][j][l][k];
            state->fbout_data[0][j][l][k] += tmp;
            state->fbout_data[1][j][l][k] += tmp;
            state->fbout_data[2][j][l][k] = 0;
          }
  }
}

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER
