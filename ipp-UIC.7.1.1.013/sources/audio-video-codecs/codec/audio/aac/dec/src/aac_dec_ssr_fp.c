/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives AAC Decode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "aac_dec_own.h"
#include "aac_dec_own_fp.h"

/*****************************************************************************/

static Ipp32f pp_q[2][2][4] =
{
  {
    {1.662939224605047f, -0.390180644032485f, -1.961570560806537f, -1.111140466038753f},
    {1.961570560806456f,  1.662939224605047f,  1.111140466039097f,  0.390180644032079f}
  },
  {
    {1.111140466039097f, -1.961570560806537f,  0.390180644032891f,  1.662939224604588f},
    {0.390180644032079f, -1.111140466038753f,  1.662939224604588f, -1.961570560806214f}
  }
};

/*****************************************************************************/

static Ipp32f ipqfCoef[4][24] =
{
  {
    0.000048827645504f, -0.000231089994560f,  0.001249394417161f, -0.001954565456367f,
    0.004938525749586f, -0.009415387936685f,  0.011328429370750f, -0.031222681814718f,
    0.016656829150441f, -0.087758205145203f,  0.007188685379275f, -0.355400101898807f,
    0.536895747386805f,  0.257950108992411f,  0.060488265680179f,  0.064966112708519f,
    0.032807746534238f,  0.019875496694136f,  0.015498166263877f,  0.004606437154885f,
    0.005407883270011f,  0.000558605576506f,  0.001126666947571f,  0.000043335772391f
  },
  {
    0.000069047946895f, -0.000510590704758f,  0.001069540798338f, -0.003481785171006f,
    0.003078128364566f, -0.013613249228851f,  0.003401555692948f, -0.038811163874361f,
   -0.007345781529095f, -0.098132719788353f, -0.067613653714302f, -0.440453162442224f,
    0.503416082057504f,  0.158689263496508f,  0.090366653351075f,  0.036153945237667f,
    0.039984169248066f,  0.007542700474140f,  0.016011420428794f,  0.000208969730318f,
    0.004779772107974f, -0.000477976987273f,  0.000838607467001f, -0.000049200374628f
  },
  {
    0.000049200374628f,  0.000838607467001f,  0.000477976987273f,  0.004779772107974f,
   -0.000208969730318f,  0.016011420428794f, -0.007542700474140f,  0.039984169248066f,
   -0.036153945237667f,  0.090366653351075f, -0.158689263496508f,  0.503416082057504f,
    0.440453162442224f, -0.067613653714302f,  0.098132719788353f, -0.007345781529095f,
    0.038811163874361f,  0.003401555692948f,  0.013613249228851f,  0.003078128364566f,
    0.003481785171006f,  0.001069540798338f,  0.000510590704758f,  0.000069047946895f
  },
  {
   -0.000043335772391f,  0.001126666947571f, -0.000558605576506f,  0.005407883270011f,
   -0.004606437154885f,  0.015498166263877f, -0.019875496694136f,  0.032807746534238f,
   -0.064966112708519f,  0.060488265680179f, -0.257950108992411f,  0.536895747386805f,
    0.355400101898807f,  0.007188685379275f,  0.087758205145203f,  0.016656829150441f,
    0.031222681814718f,  0.011328429370750f,  0.009415387936685f,  0.004938525749586f,
    0.001954565456367f,  0.001249394417161f,  0.000231089994560f,  0.000048827645504f
  }
};

/*****************************************************************************/

static Ipp32f ssr_interpolate_table[] = {
  1.000000000000f,  1.414213562373f,  2.000000000000f,  2.828427124746f,
  4.000000000000f,  5.656854249492f,  8.000000000000f, 11.313708498985f,
 16.000000000000f,  1.000000000000f,  1.296839554651f,  1.681792830507f,
  2.181015465331f,  2.828427124746f,  3.668016172819f,  4.756828460011f,
  6.168843301632f,  8.000000000000f,  1.000000000000f,  1.189207115003f,
  1.414213562373f,  1.681792830507f,  2.000000000000f,  2.378414230005f,
  2.828427124746f,  3.363585661015f,  4.000000000000f,  1.000000000000f,
  1.090507732665f,  1.189207115003f,  1.296839554651f,  1.414213562373f,
  1.542210825408f,  1.681792830507f,  1.834008086409f,  2.000000000000f,
  1.000000000000f,  1.000000000000f,  1.000000000000f,  1.000000000000f,
  1.000000000000f,  1.000000000000f,  1.000000000000f,  1.000000000000f,
  1.000000000000f,  1.000000000000f,  0.917004043205f,  0.840896415254f,
  0.771105412704f,  0.707106781187f,  0.648419777326f,  0.594603557501f,
  0.545253866333f,  0.500000000000f,  1.000000000000f,  0.840896415254f,
  0.707106781187f,  0.594603557501f,  0.500000000000f,  0.420448207627f,
  0.353553390593f,  0.297301778751f,  0.250000000000f,  1.000000000000f,
  0.771105412704f,  0.594603557501f,  0.458502021602f,  0.353553390593f,
  0.272626933166f,  0.210224103813f,  0.162104944331f,  0.125000000000f,
  1.000000000000f,  0.707106781187f,  0.500000000000f,  0.353553390593f,
  0.250000000000f,  0.176776695297f,  0.125000000000f,  0.088388347648f,
  0.062500000000f,  1.000000000000f,  0.648419777326f,  0.420448207627f,
  0.272626933166f,  0.176776695297f,  0.114625505401f,  0.074325444688f,
  0.048194088294f,  0.031250000000f,  1.000000000000f,  0.594603557501f,
  0.353553390593f,  0.210224103813f,  0.125000000000f,  0.074325444688f,
  0.044194173824f,  0.026278012977f,  0.015625000000f,  1.000000000000f,
  0.545253866333f,  0.297301778751f,  0.162104944331f,  0.088388347648f,
  0.048194088294f,  0.026278012977f,  0.014328188175f,  0.007812500000f,
  1.000000000000f,  0.500000000000f,  0.250000000000f,  0.125000000000f,
  0.062500000000f,  0.031250000000f,  0.015625000000f,  0.007812500000f,
  0.003906250000f,  1.000000000000f,  0.458502021602f,  0.210224103813f,
  0.096388176588f,  0.044194173824f,  0.020263118041f,  0.009290680586f,
  0.004259795831f,  0.001953125000f,  1.000000000000f,  0.420448207627f,
  0.176776695297f,  0.074325444688f,  0.031250000000f,  0.013139006488f,
  0.005524271728f,  0.002322670146f,  0.000976562500f
};

/*****************************************************************************/

static Ipp32f ssr_pow_table[] = {
 16.0000000000, 8.0000000000, 4.0000000000, 2.0000000000,
  1.0000000000, 0.5000000000, 0.2500000000, 0.1250000000,
  0.0625000000, 0.0312500000, 0.0156250000, 0.0078125000,
  0.0039062500, 0.0019531250, 0.0009765625
};

/*****************************************************************************/

static Ipp32f ssr_fmd(SSR_GAIN *gainc,
                      Ipp32s    max_loc_gainc,
                      Ipp32f   *p_fmd)
{
  Ipp32s alev[16];
  Ipp32s position[16], gainIndex[16];
  Ipp32s num_gain_data = gainc->adjust_num;
  Ipp32s ind0, ind1;
  Ipp32s min, num;
  Ipp32s i, j;

  for (i = 0; i < num_gain_data; i++) {
    alev[i + 1] = gainc->alevcode[i];
  }

  if (num_gain_data == 0) alev[0] = 4;
  else alev[0] = alev[1];

  alev[num_gain_data + 1] = 4;

  min = max_loc_gainc;
  position[0] = max_loc_gainc;
  num = 1;

  for (i = num_gain_data - 1; i >= 0; i--) {
    if (8 * gainc->aloccode[i] < min) {
      min = 8 * gainc->aloccode[i];
      position[num] = min;
      gainIndex[num - 1] = i + 1;
      num++;
    }
  }

  if (position[num - 1] != 0) {
    position[num] = gainIndex[num-1] = 0;
    num++;
  }

  for (j = num - 2; j >= 0; j--) {
    Ipp32f tmp;

    ind0 = alev[gainIndex[j]] * 9 + 8;
    ind1 = alev[gainIndex[j] + 1] * 9;
    tmp = ssr_pow_table[alev[gainIndex[j] + 1]];

    for (i = 0; i < 8; i++) {
      p_fmd[position[j + 1] + i] = ssr_interpolate_table[ind0 - i] *
                                   ssr_interpolate_table[ind1 + i];
    }

    for (i = position[j + 1] + 8; i < position[j]; i++) {
      p_fmd[i] = tmp;
    }
  }

  return(ssr_pow_table[alev[0]]);
}

/*****************************************************************************/

static void ssr_gainc_window(Ipp32s    len,
                             SSR_GAIN *gain0,
                             SSR_GAIN *gain1,
                             SSR_GAIN *gain2,
                             Ipp32f   *p_ad,
                             Ipp32s    window_sequence)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, p_fmd0, 1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, p_fmd1, 1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, p_fmd2, 1024);
  Ipp32f a_alev2nd, a_alev3rd;
  Ipp32s max_loc_gainc0, max_loc_gainc1, max_loc_gainc2;
  Ipp32s flat_length;
  Ipp32s i;

  switch(window_sequence){
    case ONLY_LONG_SEQUENCE:
    case EIGHT_SHORT_SEQUENCE:
      max_loc_gainc0 = max_loc_gainc1 = len/2;
      max_loc_gainc2 = 0;
    break;
    case LONG_START_SEQUENCE:
      max_loc_gainc0 = len/2;
      max_loc_gainc1 = len*7/32;
      max_loc_gainc2 = len/16;
    break;
    case LONG_STOP_SEQUENCE:
    default:
      max_loc_gainc0 = len/16;
      max_loc_gainc1 = len*7/32;
      max_loc_gainc2 = len/2;
    break;
  }

  ssr_fmd(gain0, max_loc_gainc0, p_fmd0);
  a_alev2nd = ssr_fmd(gain1, max_loc_gainc1, p_fmd1);

  if ((window_sequence == LONG_START_SEQUENCE)
    ||(window_sequence == LONG_STOP_SEQUENCE)) {
    a_alev3rd = ssr_fmd(gain2, max_loc_gainc2, p_fmd2);
  } else {
    a_alev3rd = 1;
  }

  flat_length = 0;
  if (window_sequence == LONG_STOP_SEQUENCE) {
    flat_length = len/2-max_loc_gainc0-max_loc_gainc1;
    for (i = 0; i < flat_length; i++) {
      p_ad[i] = 1.0;
    }
  }

  for (i = 0; i < max_loc_gainc0; i++) {
    p_ad[i+flat_length] = a_alev3rd * a_alev2nd * p_fmd0[i];
  }
  for (i = 0; i < max_loc_gainc1; i++) {
    p_ad[i+flat_length+max_loc_gainc0] = a_alev3rd * p_fmd1[i];
  }

  if(window_sequence == LONG_START_SEQUENCE){
    for (i = 0; i < max_loc_gainc2; i++) {
      p_ad[i+max_loc_gainc0+max_loc_gainc1] = p_fmd2[i];
    }
    flat_length = len/2 - max_loc_gainc1 - max_loc_gainc2;
    for (i = 0; i < flat_length; i++) {
      p_ad[i+max_loc_gainc0+max_loc_gainc1+max_loc_gainc2] = 1.0;
    }
  }
  else if(window_sequence == LONG_STOP_SEQUENCE){
    for (i = 0; i < max_loc_gainc2; i++) {
      p_ad[i+flat_length+max_loc_gainc0+max_loc_gainc1] = p_fmd2[i];
    }
  }
}

/*****************************************************************************/

static void ssr_compensate(Ipp32f   *input,
                           SSR_GAIN **SSRInfo,
                           SSR_GAIN *prevSSRInfo,
                           Ipp32s   len,
                           Ipp32s   window_sequence,
                           Ipp32f   **out,
                           Ipp32f   *m_gcOverlapBuffer)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, a_gcwind, 512);
  Ipp32f *gcOverlapBuffer[4];
  Ipp32s i, band, k;

  Ipp32s  last_cur_block = 0;

  for (i = 0; i < 4; i++) {
    gcOverlapBuffer[i] = m_gcOverlapBuffer + 512 * i;
  }

  if (window_sequence == EIGHT_SHORT_SEQUENCE){
    for (band = 0; band < 4; band++) {
      for (k = 0; k < 8; k++) {
        ssr_gainc_window(len/16,
                         &prevSSRInfo[band],
                         &SSRInfo[band][k], &SSRInfo[band][k],
                         a_gcwind, window_sequence);

        ippsMul_32f_I(a_gcwind, input+band*len/2 + k*len/16,
                      len/32);

        ippsAdd_32f_I(input+band*len/2+k*len/16,
                      &gcOverlapBuffer[band][len*7/64+len/32*k],
                      len/32);

        ippsMul_32f(input+band*len/2+k*len/16+len/32,
                    a_gcwind+len/32,
                    &gcOverlapBuffer[band][len*7/64+(k+1)*len/32],
                    len/32);
        prevSSRInfo[band] = SSRInfo[band][k];
      }

      ippsMove_32f(&gcOverlapBuffer[band][0], &out[band][0],
                   len/4);

      ippsMove_32f(&gcOverlapBuffer[band][len/4],
                   &gcOverlapBuffer[band][0],
                   len/4);
    }
  } else {
    last_cur_block = 0;
    if (window_sequence != ONLY_LONG_SEQUENCE)
      last_cur_block = 1;

    for (band = 0; band < 4; band++) {
      ssr_gainc_window(len/2,
                       &prevSSRInfo[band],
                       &SSRInfo[band][0], &SSRInfo[band][1],
                       a_gcwind, window_sequence);

      ippsMul_32f_I(a_gcwind, input+band*len/2,
                    len/4);
      ippsAdd_32f(&gcOverlapBuffer[band][0], input+band*len/2,
                  &out[band][0], len/4);

      ippsMul_32f(input+band*len/2+len/4,
                  a_gcwind + len/4,
                  &gcOverlapBuffer[band][0], len/4);

      prevSSRInfo[band] = SSRInfo[band][last_cur_block];
    }
  }
}

/*****************************************************************************/

static void ssr_imdct(Ipp32f      *coef,
                      Ipp32s       curr_win_shape,
                      Ipp32s       prev_win_shape,
                      Ipp32s       window_sequence,
                      Ipp32f      *timeSig,
                      sFilterbank *p_data)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, imdct_in, 1024);
  Ipp32s i;
  Ipp32s band, short_block;

  if (window_sequence == EIGHT_SHORT_SEQUENCE) {
    for (band = 0; band < 4; band += 2){
      for (short_block = 0; short_block < 8; short_block++) {
        for (i = 0; i < 32; i++) {
          imdct_in[256*band + 32*short_block + i] =
            coef[128*short_block + 32*band + i];
        }
        for (i = 0; i < 32; i++) {
          imdct_in[256*band + 256 + 32*short_block + i] =
            coef[128*short_block + 32*band + 63 - i];
        }
      }
    }

    for (band = 0; band < 4; band++) {
      FilterbankDecSSR(p_data,
                       imdct_in + 256 * band,
                       window_sequence,
                       curr_win_shape,
                       prev_win_shape,
                       timeSig + 512 * band);
    }

  } else {
    for (band = 0; band < 4; band += 2){
      FilterbankDecSSR(p_data,
                       coef + 256 * band,
                       window_sequence,
                       curr_win_shape,
                       prev_win_shape,
                       timeSig + 512 * band);
      for (i = 0; i < 256; i++) {
        imdct_in[256*band + i + 256] = coef[256*band + 511 - i];
      }
      FilterbankDecSSR(p_data,
                       imdct_in + 256 * (band + 1),
                       window_sequence,
                       curr_win_shape,
                       prev_win_shape,
                       timeSig + 512 * (band + 1));
    }
  }
}

/*****************************************************************************/

static void ssr_ipqf_main(Ipp32f *ipqfInBufCh[4],
                          Ipp32s len,
                          Ipp32f *output,
                          ownIppsIPQFState_32f *SSR_IPQFState)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, tbuf0, 280);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tbuf1, 280);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tbuf2, 280);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tbuf3, 280);
  Ipp32f *app_pqfbuf0[2];
  Ipp32f *app_pqfbuf1[2];
  Ipp32s i, k;
  Ipp32f acc;

  app_pqfbuf0[0] = SSR_IPQFState->app_pqfbuf0;
  app_pqfbuf0[1] = SSR_IPQFState->app_pqfbuf1;
  app_pqfbuf1[0] = SSR_IPQFState->app_pqfbuf2;
  app_pqfbuf1[1] = SSR_IPQFState->app_pqfbuf3;

  for (i = 0; i < len / 4; i+=2) {
    tbuf0[i+23] = pp_q[0][0][0]*ipqfInBufCh[0][i] +
                  pp_q[0][0][1]*ipqfInBufCh[1][i] +
                  pp_q[0][0][2]*ipqfInBufCh[2][i] +
                  pp_q[0][0][3]*ipqfInBufCh[3][i];

    tbuf0[i+24] = pp_q[1][0][0]*ipqfInBufCh[0][i+1] +
                  pp_q[1][0][1]*ipqfInBufCh[1][i+1] +
                  pp_q[1][0][2]*ipqfInBufCh[2][i+1] +
                  pp_q[1][0][3]*ipqfInBufCh[3][i+1];

    tbuf1[i+23] = pp_q[0][1][0]*ipqfInBufCh[0][i] +
                  pp_q[0][1][1]*ipqfInBufCh[1][i] +
                  pp_q[0][1][2]*ipqfInBufCh[2][i] +
                  pp_q[0][1][3]*ipqfInBufCh[3][i];

    tbuf1[i+24] = pp_q[1][1][0]*ipqfInBufCh[0][i+1] +
                  pp_q[1][1][1]*ipqfInBufCh[1][i+1] +
                  pp_q[1][1][2]*ipqfInBufCh[2][i+1] +
                  pp_q[1][1][3]*ipqfInBufCh[3][i+1];

    tbuf2[i+22] = pp_q[1][0][0]*ipqfInBufCh[0][i] +
                  pp_q[1][0][1]*ipqfInBufCh[1][i] +
                  pp_q[1][0][2]*ipqfInBufCh[2][i] +
                  pp_q[1][0][3]*ipqfInBufCh[3][i];

    tbuf2[i+23] = pp_q[0][0][0]*ipqfInBufCh[0][i+1] +
                  pp_q[0][0][1]*ipqfInBufCh[1][i+1] +
                  pp_q[0][0][2]*ipqfInBufCh[2][i+1] +
                  pp_q[0][0][3]*ipqfInBufCh[3][i+1];

    tbuf3[i+22] = pp_q[1][1][0]*ipqfInBufCh[0][i] +
                  pp_q[1][1][1]*ipqfInBufCh[1][i] +
                  pp_q[1][1][2]*ipqfInBufCh[2][i] +
                  pp_q[1][1][3]*ipqfInBufCh[3][i];

    tbuf3[i+23] = pp_q[0][1][0]*ipqfInBufCh[0][i+1] +
                  pp_q[0][1][1]*ipqfInBufCh[1][i+1] +
                  pp_q[0][1][2]*ipqfInBufCh[2][i+1] +
                  pp_q[0][1][3]*ipqfInBufCh[3][i+1];
  }

  for (i = 0; i < 22; i++) {
    tbuf0[i] = app_pqfbuf0[0][i];
    tbuf1[i] = app_pqfbuf0[1][i];
    tbuf2[i] = app_pqfbuf1[0][i];
    tbuf3[i] = app_pqfbuf1[1][i];
  }

  tbuf0[22] = app_pqfbuf0[0][22];
  tbuf1[22] = app_pqfbuf0[1][22];

  for (i = 0; i < len/8; i++) {
    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[0][k]*tbuf0[23-k+2*i];
    }
    output[i*8] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[1][k]*tbuf1[23-k+2*i];
    }
    output[i*8+1] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[2][k]*tbuf1[23-k+2*i];
    }
    output[i*8+2] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[3][k]*tbuf0[23-k+2*i];
    }
    output[i*8+3] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[0][k]*tbuf2[23-k+2*i];
    }
    output[i*8+4] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[1][k]*tbuf3[23-k+2*i];
    }
    output[i*8+5] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[2][k]*tbuf3[23-k+2*i];
    }
    output[i*8+6] = acc;

    acc = 0.0;
    for (k = 0; k < 24; k++) {
      acc += ipqfCoef[3][k]*tbuf2[23-k+2*i];
    }
    output[i*8+7] = acc;
  }

  for (i = 0; i < 22; i++) {
    app_pqfbuf0[0][i] = tbuf0[i+256];
    app_pqfbuf0[1][i] = tbuf1[i+256];
    app_pqfbuf1[0][i] = tbuf2[i+256];
    app_pqfbuf1[1][i] = tbuf3[i+256];
  }

  app_pqfbuf0[0][22] = tbuf0[22+256];
  app_pqfbuf0[1][22] = tbuf1[22+256];
}

/*****************************************************************************/

void ssr_gain_control(Ipp32f               *spectrum_data,
                      Ipp32s               curr_win_shape,
                      Ipp32s               prev_win_shape,
                      Ipp32s               curr_win_sequence,
                      SSR_GAIN             **SSRInfo,
                      SSR_GAIN             *prevSSRInfo,
                      Ipp32f               *samples,
                      Ipp32f               *m_gcOverlapBuffer,
                      ownIppsIPQFState_32f *SSR_IPQFState,
                      sFilterbank          *p_data,
                      Ipp32s               len)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, bandSigChBuf, 1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, imdctOut, 2048);
  Ipp32f *bandSigCh[4];
  Ipp32s i;

  for (i = 0; i < 4; i++) {
    bandSigCh[i] = bandSigChBuf + i * 256;
  }

  ssr_imdct(spectrum_data, curr_win_shape, prev_win_shape,
            curr_win_sequence, imdctOut, p_data);

  ssr_compensate(imdctOut, SSRInfo, prevSSRInfo, len,
                 curr_win_sequence, bandSigCh, m_gcOverlapBuffer);

  ssr_ipqf_main(bandSigCh, len, samples, SSR_IPQFState);
}

/*****************************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

