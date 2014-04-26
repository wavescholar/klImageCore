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
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include <math.h>
#include "mp3enc_own_fp.h"

#include "ippcore.h"

/****************************************************************************/

static Ipp32f sprdngf(Ipp32f b1,
                      Ipp32f b2,
                      Ipp32s layer)
{
  Ipp32f tmpx,tmpy,tmpz;

  if (layer == 3) {
    tmpx = (b2 >= b1) ? 3*(b2-b1) : 1.5f*(b2-b1);
  } else {
    tmpx = 1.05f * (b2-b1);
  }

  tmpz = 8 * MIN((tmpx-0.5f)*(tmpx-0.5f) - 2*(tmpx-0.5f), 0);

  tmpy = 15.811389f + 7.5f*(tmpx + 0.474f)-
         17.5f*(Ipp32f)sqrt(1 + (tmpx+0.474f)*(tmpx+0.474f));

  return (tmpy < -100 ? 0 : (Ipp32f)pow(10.0,(tmpz + tmpy)/10.0));
}

/****************************************************************************/

static void mp3enc_psychoacousticLongWindow(MP3Enc *state,
                                            Ipp32f *input_data,
                                            Ipp32f* rsqr_long, Ipp32s ch)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, sw, 1024);
  Ipp32f *fft_line = state->fft_line_long[ch & 1];

  IPP_ALIGNED_ARRAY(32, Ipp32f, c_w, 512);
  IPP_ALIGNED_ARRAY(32, Ipp32f, minthres, 32);
  IPP_ALIGNED_ARRAY(32, Ipp32f, x2, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, c_b, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ct, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, d_cb, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp0, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp1, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp2, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, num, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, denum, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, r_pred, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, re_pred, NUM_UNPRED_LINES_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, im_pred, NUM_UNPRED_LINES_LONG);
  Ipp32f *r, *r_prev, *r_prev_prev;
  Ipp32f *re, *re_prev, *re_prev_prev;
  Ipp32f *im, *im_prev, *im_prev_prev;
  Ipp32f *tmp_dst[2];
  Ipp32f *nb,*nb_l;
  Ipp32s i, j, b, index;
  Ipp32s current_f_r_index, prev_f_r_index, prev_prev_f_r_index;
  Ipp32s pa_num_ptt_long = state->pa_num_ptt_long;
  Ipp32s freq_ind, layer, sblimit_real;
  Ipp32s absthr_len[3] = {130, 126, 132};
  Ipp32s upsample = state->com.upsample;

  freq_ind = state->com.header.samplingFreq;
  layer = state->com.header.layer;

  if (ch < 2 || state->com.mc_channel)
    ippsMul_32f(input_data, state->pa_hann_window_long, sw, 1024);

  if (ch < 2 || state->com.mc_channel) {
    ippsFFTFwd_RToCCS_32f(sw, fft_line, state->pa_pFFTSpecLong, state->pa_pBuffer);
    if (upsample)
      ippsZero_32f(fft_line + (1024 >> upsample), 1024 - (1024 >> upsample));
  }

/* **** */
  fft_line[0] = fft_line[1] = fft_line[2] = fft_line[3] = 0;
  fft_line[1020] = fft_line[1021] = fft_line[1022] = fft_line[1023] = 0;
/* **** */

  current_f_r_index = state->pa_current_f_r_index[ch];
  prev_f_r_index = current_f_r_index - 1;
  if (prev_f_r_index < 0) prev_f_r_index = 2;
  prev_prev_f_r_index = prev_f_r_index - 1;
  if (prev_prev_f_r_index < 0) prev_prev_f_r_index = 2;

  r            = (Ipp32f*)state->pa_r[ch][current_f_r_index];
  r_prev       = (Ipp32f*)state->pa_r[ch][prev_f_r_index];
  r_prev_prev  = (Ipp32f*)state->pa_r[ch][prev_prev_f_r_index];

  re           = (Ipp32f*)state->pa_re[ch][current_f_r_index];
  re_prev      = (Ipp32f*)state->pa_re[ch][prev_f_r_index];
  re_prev_prev = (Ipp32f*)state->pa_re[ch][prev_prev_f_r_index];

  im           = (Ipp32f*)state->pa_im[ch][current_f_r_index];
  im_prev      = (Ipp32f*)state->pa_im[ch][prev_f_r_index];
  im_prev_prev = (Ipp32f*)state->pa_im[ch][prev_prev_f_r_index];

  ippsMagnitude_32fc((Ipp32fc *)fft_line, (Ipp32f*)r, 512);

  ippsThreshold_LT_32f_I((Ipp32f*)r, 512, 1);

  tmp_dst[0] = re;
  tmp_dst[1] = im;

  /* Calculate the unpredictebility measure c(w)                */
  /* Some transformations:                                      */
  /* re((2*r_prev-r_prev_prev)*exp(-j(2*f_prev-f_prev_prev))) = */
  /* ((2*r_prev-r_prev_prev)/(r_prev*r_prev*r_prev_prev))*      */
  /* (2*im_prev_prev*re_prev*im_prev +                          */
  /* re_prev_prev*(re_prev*re_prev-im_prev*im_prev))            */
  /*                                                            */
  /* im((2*r_prev-r_prev_prev)*exp(-j(2*f_prev-f_prev_prev))) = */
  /* ((2*r_prev-r_prev_prev)/(r_prev*r_prev*r_prev_prev))*      */
  /* (2*re_prev_prev*re_prev*im_prev -                          */
  /* im_prev_prev*(re_prev*re_prev-im_prev*im_prev))            */
  /*                                                            */
  /* where re_prev_prev = prev_prev_r*cos(prev_prev_f),         */
  /*       im_prev_prev = prev_prev_r*sin(prev_prev_f),         */
  /*       re_prev = prev_r*cos(prev_prev_f),                   */
  /*       im_prev = prev_r*sin(prev_prev_f)                    */

  ippsDeinterleave_32f(fft_line, 2,  NUM_UNPRED_LINES_LONG, tmp_dst);

  /* tmp0 = (2*r_prev-r_prev_prev)/(r_prev*r_prev*r_prev_prev) */
  ippsMulC_32f(r_prev, (Ipp32f)2, r_pred,  NUM_UNPRED_LINES_LONG);
  ippsSub_32f_I(r_prev_prev, r_pred,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f(r_prev, r_prev, denum,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f_I(r_prev_prev, denum,  NUM_UNPRED_LINES_LONG);
  ippsDiv_32f(denum, r_pred, tmp0,  NUM_UNPRED_LINES_LONG);

  /* tmp1 = 2*re_prev*im_prev */
  ippsMulC_32f(re_prev, (Ipp32f)2, tmp1,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f_I(im_prev, tmp1,  NUM_UNPRED_LINES_LONG);

  /* tmp2 = re_prev*re_prev-im_prev*im_prev */
  ippsMul_32f(re_prev, re_prev, tmp,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f(im_prev, im_prev, tmp2,  NUM_UNPRED_LINES_LONG);
  ippsSub_32f_I(tmp, tmp2,  NUM_UNPRED_LINES_LONG);

  ippsMul_32f(im_prev_prev, tmp1, re_pred,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f(re_prev_prev, tmp2, tmp,  NUM_UNPRED_LINES_LONG);
  ippsAdd_32f_I(tmp, re_pred,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f_I(tmp0, re_pred,  NUM_UNPRED_LINES_LONG);

  ippsMul_32f(re_prev_prev, tmp1, im_pred,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f(im_prev_prev, tmp2, tmp,  NUM_UNPRED_LINES_LONG);
  ippsSub_32f_I(tmp, im_pred,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f_I(tmp0, im_pred,  NUM_UNPRED_LINES_LONG);

  ippsSub_32f(re, re_pred, tmp0,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f_I(tmp0, tmp0,  NUM_UNPRED_LINES_LONG);

  ippsSub_32f(im, im_pred, tmp1,  NUM_UNPRED_LINES_LONG);
  ippsMul_32f_I(tmp1, tmp1,  NUM_UNPRED_LINES_LONG);

  ippsAdd_32f(tmp0, tmp1, num,  NUM_UNPRED_LINES_LONG);
  ippsSqrt_32f_I(num,  NUM_UNPRED_LINES_LONG);

  ippsAbs_32f(r_pred, denum,  NUM_UNPRED_LINES_LONG);
  ippsAdd_32f_I(r, denum,  NUM_UNPRED_LINES_LONG);

  ippsDiv_32f(denum, num, c_w,  NUM_UNPRED_LINES_LONG);

  ippsSet_32f(layer == 3 ? 0.4f : 0.3f, &c_w[ NUM_UNPRED_LINES_LONG],
               512 -  NUM_UNPRED_LINES_LONG);

  ippsSqr_32f((Ipp32f*)r, rsqr_long, 512);

  index = 0;

  for (b = 0; b < pa_num_ptt_long; b++) {
    Ipp32f *tmp_rsqr = &rsqr_long[index];
    Ipp32f *tmp_c_w = (Ipp32f*)&c_w[index];
    Ipp32s len;

 //   if (layer == 3) {
 //      len = mp3enc_ptbl_l[freq_ind][b].numlines;
 //   } else {
      len = mp3enc_ptbl_numlines_l12[freq_ind][b];
 //   }

    ippsSum_32f(tmp_rsqr, len, &e_b[b], ippAlgHintNone);
    ippsDotProd_32f(tmp_rsqr, tmp_c_w, len, &c_b[b]);
    index += len;
  }

  nb   = state->pa_nb_long[ch][1];
  nb_l = state->pa_nb_long[ch][0];

  for (b = 0; b < pa_num_ptt_long; b++) {
    //Ipp32u cntNonZero = (Ipp32u)(state->pa_sprdngf_cntsNonzero[b]);
    Ipp32f *tmp_ptr = (Ipp32f*)state->pa_sprdngf_long + b * state->pa_num_ptt_long_algn4/*pa_num_ptt_long*/;// aligned step

    ippsDotProd_32f(e_b, tmp_ptr, pa_num_ptt_long/*cntNonZero*/, &ecb[b]);//aligned length (tail are zero)
    ippsDotProd_32f(c_b, tmp_ptr, pa_num_ptt_long/*cntNonZero*/, &ct[b]);
  }

  if (layer != 3) {
    Ipp32f nmt = 5.5f;

    ippsDiv_32f(ecb, ct, d_cb, pa_num_ptt_long);
    ippsSqr_32f(d_cb, x2, pa_num_ptt_long);
    ippsMulC_32f_I(/*5.8734f*/4.3097f, x2, pa_num_ptt_long);
    ippsMulC_32f_I(/*-5.6788f*/-4.274f, d_cb, pa_num_ptt_long);
    ippsAdd_32f_I(x2, d_cb, pa_num_ptt_long);
    ippsAddC_32f_I(/*1.352f*/1.1002f, d_cb, pa_num_ptt_long);

    ippsThreshold_LT_32f_I(d_cb, pa_num_ptt_long, 0.0f);
    ippsThreshold_GT_32f_I(d_cb, pa_num_ptt_long, 1.0f);

    ippsMul_32f(d_cb, mp3enc_ptbl_TMN_l2[freq_ind], x2, pa_num_ptt_long);
    ippsSubCRev_32f_I(1.0f, d_cb, pa_num_ptt_long);
    ippsMulC_32f_I(nmt, d_cb, pa_num_ptt_long);
    ippsAdd_32f_I(x2, d_cb, pa_num_ptt_long);

    ippsMaxEvery_32f_I(mp3enc_ptbl_minval_l12[freq_ind], d_cb, pa_num_ptt_long);
    ippsMulC_32f_I(-0.230258509f, d_cb, pa_num_ptt_long);
    ippsExp_32f_I(d_cb, pa_num_ptt_long);
    ippsMul_32f_I(state->pa_rnorm_long, d_cb, pa_num_ptt_long);
    ippsMul_32f_I(ecb, d_cb, pa_num_ptt_long);

    index = 0;
    for (b = 0; b < pa_num_ptt_long; b++) {
      Ipp32s len = mp3enc_ptbl_numlines_l12[freq_ind][b];

      ippsSet_32f(d_cb[b], sw + index, len);
      index += len;
    }

    index = 0;
    for(j = 0; j < absthr_len[freq_ind]; j++) {
      Ipp32f max;
      Ipp32s len = mp3enc_absthr[freq_ind][j].numlines;

#if 1
      max = mp3enc_absthr[freq_ind][j].absthr;
      for (i = 0; i < len; i++) {
        max = ( sw[index + i] > max) ? sw[index + i] : max;
      }
      sw[index + j] = max;
#else
      for (i = 0; i < len; i++) {
        max = sw[index + i];
        if(max < mp3enc_absthr[freq_ind][j].absthr)
          max = mp3enc_absthr[freq_ind][j].absthr;
        sw[index + j] = max;
      }
#endif
      index += len;
    }

    ippsSet_32f(mp3enc_absthr[freq_ind][absthr_len[freq_ind]-1].absthr,
      sw + index, 512 - index);

    sblimit_real = state->com.sblimit_real;

    for(j = 0; j < sblimit_real; j++) {
      ippsMin_32f(sw + (j << 4), 16, &(minthres[j]));
      ippsSum_32f(rsqr_long + (j << 4), 16, &(state->snr[j]), ippAlgHintNone);
    }

    ippsDiv_32f_I(minthres, state->snr, sblimit_real);
  }
}

/****************************************************************************/

MP3Status mp3enc_psychoacousticInit(MP3Enc* state, Ipp8u* mem, Ipp32s* size_all)
{
  Ipp32s i, ch;
  Ipp32s num_ptt, b, bb;
  const Ipp32s num_ptt_l12[3] = {57, 58, 49};
  Ipp32s freq_ind, layer;
  Ipp32s size = 0, ts = 0, ts1 = 0, size_buf = 0, size_init = 0, size_buf1 = 0, size_init1 = 0;
  Ipp8u* mem_init = NULL;

  ippsFFTGetSize_R_32f(10, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, &ts, &size_init, &size_buf);
  ippsFFTGetSize_R_32f(8, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, &ts1, &size_init1, &size_buf1);
  if (size_buf < size_buf1) size_buf = size_buf1;
  if (size_init < size_init1) size_init = size_init1;

  size += ts + ts1 + size_buf;

  if (mem) {
    mem_init = 0;
    if (size_init) {
      mem_init = (Ipp8u*)ippMalloc(size_init);
      if (!mem_init)
        return MP3_ALLOC;
    }

    state->pa_pFFTSpecLong = (IppsFFTSpec_R_32f*)mem;
    ippsFFTInit_R_32f(&(state->pa_pFFTSpecLong), 10, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, mem, mem_init);
    mem += ts;
    state->pa_pFFTSpecShort = (IppsFFTSpec_R_32f*)mem;
    ippsFFTInit_R_32f(&(state->pa_pFFTSpecShort), 8, IPP_FFT_NODIV_BY_ANY, ippAlgHintFast, mem, mem_init);
    mem += ts1;
    state->pa_pBuffer = mem;
    mem += size_buf;
    if(mem_init)
      ippFree(mem_init);
  }

  if(state) {
    freq_ind = state->com.header.samplingFreq;
    layer = state->com.header.layer;

    for (ch = 0; ch < NUM_CHANNELS; ch++)
      state->pa_current_f_r_index[ch] = 2;

    state->pa_num_ptt_long = num_ptt = num_ptt_l12[freq_ind];
    state->pa_num_ptt_long_algn4 = (state->pa_num_ptt_long+3)&(-4);

    //ippsSet_32f(0.f, state->pa_sprdngf_long, (MAX_PPT_LONG * MAX_PPT_LONG));// already zeroed

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = state->pa_sprdngf_long + b * (state->pa_num_ptt_long_algn4)/*num_ptt*/; // aligned step
      Ipp32f tmp = 0;
      Ipp32s len;

      for (bb = 0; bb < num_ptt; bb++) {
        tmp_ptr[bb] = sprdngf(mp3enc_ptbl_bval_l12[freq_ind][bb], mp3enc_ptbl_bval_l12[freq_ind][b], layer);
        tmp += tmp_ptr[bb];
      }

      len = mp3enc_ptbl_numlines_l12[freq_ind][b];
      state->pa_rnorm_long[b] = 1/(tmp * len);
    }

    /* filling Hann windows */
    for (i = 0; i < 1024; i++)
      state->pa_hann_window_long[i] =
      (Ipp32f)(0.5f * (1 - cos(PI * (i + 0.5f) / 512)));

    ippsSet_32f(1.0e30, (Ipp32f*)state->pa_nb_long, NUM_CHANNELS*2*MAX_PPT_LONG);

    ippsSet_32f(1, (Ipp32f*)state->pa_r, NUM_CHANNELS*3*512);
    ippsSet_32f(1, (Ipp32f*)state->pa_re, NUM_CHANNELS*3*512);
    ippsSet_32f(0, (Ipp32f*)state->pa_im, NUM_CHANNELS*3*512);
  }

  if(size_all)
    *size_all = size;

  return MP3_OK;
}

/****************************************************************************/

void mp3enc_psychoacoustic_l1(MP3Enc *state, Ipp32f **pa_buffer)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, rsqr_long, 1024);
  Ipp32s ch, stereo;

  stereo = state->com.stereo;

  for (ch = 0; ch < stereo; ch++) {
    state->snr = state->pa_snr[ch];
    mp3enc_psychoacousticLongWindow(state, pa_buffer[ch], rsqr_long, ch);

    ippsLn_32f_I(state->snr, state->com.sblimit_real);
    ippsMulC_32f_I(4.342944819f, state->snr, state->com.sblimit_real);

    state->pa_current_f_r_index[ch]++;
    if (state->pa_current_f_r_index[ch] >= 3)
      state->pa_current_f_r_index[ch] = 0;
  }
}

/****************************************************************************/

void mp3enc_psychoacoustic_l2(MP3Enc *state, Ipp32f **pa_buffer)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, rsqr_long, 1024);
  Ipp32s gr, ch, stereo, sblimit;

  stereo = state->com.stereo;
  sblimit = state->com.sblimit_real;

  for (ch = 0; ch < stereo + state->com.mc_channel; ch++) {
    for (gr = 0; gr < 2; gr++) {
      state->snr = state->pa_snr[ch + gr];
      mp3enc_psychoacousticLongWindow(state, pa_buffer[ch] + gr * LEN_MDCT_LINE, rsqr_long, ch);

      state->pa_current_f_r_index[ch]++;
      if (state->pa_current_f_r_index[ch] >= 3)
        state->pa_current_f_r_index[ch] = 0;
    }

    ippsMaxEvery_32f_I(state->pa_snr[ch+1], state->pa_snr[ch], sblimit);
    ippsLn_32f_I(state->pa_snr[ch], sblimit);
    ippsMulC_32f_I(4.342944819f, state->pa_snr[ch], sblimit);
    for(gr = sblimit; gr < 32; gr++)
      state->pa_snr[ch][gr] = -1e37f;
  }
}

/****************************************************************************/

#define START_BND 16

static void mp3encBlockSwitching(MP3Enc *state,
                                 Ipp32s gr,
                                 Ipp32s ch)
{
  mpaPsychoacousticBlock    *pBlock = &(state->psychoacoustic_block[ch]);
  mpaPsychoacousticBlockCom *pBlockCom = &(state->psychoacoustic_block_com);
  Ipp32f winEnergy[3];
  Ipp32f winEnergyLow[3];
  Ipp32f max, maxLow;
  Ipp32s w, attack, attackLow, bnd;
  Ipp32s start_bnd = (state->com.lowpass_maxline + 1) >> 1;
  Ipp32s end_bnd = state->com.lowpass_maxline;
  Ipp32s start_bnd_low = 3;

  state->attackWindow[2*gr + ch] = pBlock->attackIndex = pBlock->lastAttackIndex;

  for (w = 0; w < 3; w++) {
    Ipp32s curGr = gr+1;
    Ipp32s start = w * 6 + 9;
    Ipp32s k;

    if (start >= 18) {
      start -= 18;
      curGr++;
    }

    winEnergy[w] = 0;
    winEnergyLow[w] = 0;

    if (start <= 12) {
      for (bnd = start_bnd; bnd < end_bnd; bnd++) {
        for (k = start; k < start + 6; k++) {
          Ipp32f sample = (*(state->fbout[curGr]))[ch][k][bnd];
          winEnergy[w] += sample * sample;
        }
      }

      for (bnd = start_bnd_low; bnd < start_bnd; bnd++) {
        for (k = start; k < start + 6; k++) {
          Ipp32f sample = (*(state->fbout[curGr]))[ch][k][bnd];
          winEnergyLow[w] += sample * sample;
        }
      }
    } else {
      for (bnd = start_bnd; bnd < end_bnd; bnd++) {
        for (k = start; k < 18; k++) {
          Ipp32f sample = (*(state->fbout[curGr]))[ch][k][bnd];
          winEnergy[w] += sample * sample;
        }

        for (k = 0; k < start - 12; k++) {
          Ipp32f sample = (*(state->fbout[curGr+1]))[ch][k][bnd];
          winEnergy[w] += sample * sample;
        }
      }

      for (bnd = start_bnd_low; bnd < start_bnd; bnd++) {
        for (k = start; k < 18; k++) {
          Ipp32f sample = (*(state->fbout[curGr]))[ch][k][bnd];
          winEnergyLow[w] += sample * sample;
        }

        for (k = 0; k < start - 12; k++) {
          Ipp32f sample = (*(state->fbout[curGr+1]))[ch][k][bnd];
          winEnergyLow[w] += sample * sample;
        }
      }
    }
  }

  attack = 0;
  max = 0;
  attackLow = 0;
  maxLow = 0;

  for (w = 0; w < 3; w++) {
    if (winEnergy[w] > pBlock->avWinEnergy * pBlockCom->attackThreshold) {
      attack = 1;
    }
    if (winEnergy[w] > pBlock->avWinEnergy) {
      pBlock->avWinEnergy = 0.9f * pBlock->avWinEnergy + 0.1f * winEnergy[w];
    } else {
      pBlock->avWinEnergy = winEnergy[w];
    }
    if (max < winEnergy[w]) max = winEnergy[w];

    winEnergyLow[w] += winEnergy[w];

    if (winEnergyLow[w] > pBlock->avWinEnergyLow * 100) {
      attackLow = 1;
    }
    pBlock->avWinEnergyLow =
      (0.5f * pBlock->avWinEnergyLow) + (0.5f * winEnergyLow[w]);
    if (maxLow < winEnergyLow[w]) maxLow = winEnergyLow[w];

  }

  if (max < 1.0e-9f) {
    attack = 0;
  }

  if (maxLow < 1.0e-9f) {
    attackLow = 0;
  }

  pBlock->lastAttackIndex = -1;

  if (attack) {
    Ipp32f tmpEn = pBlock->lastWindowEnergy;
    pBlock->lastAttackIndex = -1;
    for (w = 0; w < 3; w++) {
      if (winEnergy[w] > 20 * tmpEn) {
        pBlock->lastAttackIndex = w;
      }
      tmpEn = winEnergy[w];
    }
  }

  if ((attack == 0) && (max < 1.0e-3f)) {
    attack = attackLow;
    pBlock->lastAttackIndex = -1;

    if (attack) {
      Ipp32f tmpEn = pBlock->lastWindowEnergyLow;
      pBlock->lastAttackIndex = -1;
      for (w = 0; w < 3; w++) {
        if (winEnergyLow[w] > 20 * tmpEn) {
          pBlock->lastAttackIndex = w;
        }
        tmpEn = winEnergyLow[w];
      }
    }
  }

  if (attack) {
    pBlock->next_desired_block_type = SHORT_TYPE;
  } else{
    pBlock->next_desired_block_type = NORM_TYPE;
  }

  pBlock->lastWindowEnergy = winEnergy[2];
  pBlock->lastWindowEnergyLow = winEnergyLow[2];

  /* If attack occurs in the last window */
  /* let the next block will be short block */
  if (pBlock->lastAttackIndex == 2) {
    pBlock->avWinEnergy = winEnergy[1];
    /* Save some bits for the next block */
    pBlock->lastAttackIndex = -1;

  }
}

/****************************************************************************/

#if 0
static void mp3encPsy_long_window(mpaPsychoacousticBlock    *pBlock,
                                  mpaPsychoacousticBlockCom *pBlockCom,
                                  Ipp32f                    *input_data,
                                  Ipp32s                     ch,
                                  Ipp32s                     gr)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_LONG);
  Ipp32f *nb,*nb_l;
  Ipp32f *noiseThr;
  Ipp32s b, num_ptt, sb, index;

  num_ptt = pBlockCom->longWindow->num_ptt;

  index = 0;

  for (b = 0; b < num_ptt; b++) {
    Ipp32f *tmp_rsqr = &input_data[index];
    Ipp32s len = pBlockCom->longWindow->numlines[b];

    ippsDotProd_32f(tmp_rsqr, tmp_rsqr, len, &e_b[b]);
    index += len;
  }

  for (b = 0; b < num_ptt; b++) {
    Ipp32f *tmp_ptr = (Ipp32f*)pBlockCom->sprdngf_long + b * num_ptt;

    ippsDotProd_32f(e_b, tmp_ptr, num_ptt, &ecb[b]);
  }

  nb   = pBlock->nb_long[pBlockCom->nb_curr_index];
  nb_l = pBlock->nb_long[pBlockCom->nb_prev_index];

  ippsMulC_32f_I(ATTENUATION, ecb, num_ptt);
  ippsMul_32f((Ipp32f*)pBlockCom->rnorm_long, ecb, (Ipp32f*)nb, num_ptt);

  ippsMaxEvery_32f_I(pBlockCom->longWindow->qsthr, nb, num_ptt);

  if (pBlock->block_type != STOP_TYPE) {
    ippsMulC_32f(nb_l, 0.01f, ecb, num_ptt);
    ippsMaxEvery_32f_I(nb, ecb, num_ptt);
    ippsMulC_32f_I(2, nb_l, num_ptt);
    ippsMinEvery_32f_I(ecb, nb_l, num_ptt);
  } else {
    ippsCopy_32f(nb, nb_l, num_ptt);
  }

  noiseThr = &pBlockCom->noiseThr[4 * gr + ch][0];
  {
    IPP_ALIGNED_ARRAY(32, Ipp32f, tmpBuf, LEN_MDCT_LINE);
    Ipp32f* tmpPtr;
    Ipp32s iii;

    tmpPtr = tmpBuf;
    for (b = 0; b < num_ptt; b++) {
      Ipp32s len = pBlockCom->longWindow->numlines[b];
      Ipp32f val = nb_l[b] / len;
      for (iii = 0; iii < len; iii++) {
        tmpPtr[0] = val;
        tmpPtr++;
      }
    }

    tmpPtr = tmpBuf;
    for (sb = 0; sb < pBlockCom->num_sfb_long; sb++) {
      Ipp32s start = pBlockCom->sfb_offset_long[sb];
      Ipp32s width = pBlockCom->sfb_offset_long[sb + 1] - start;
      Ipp32f min = tmpBuf[start];
      Ipp32s iii;

      for (iii = 1; iii < width; iii++) {
        if (min > tmpBuf[start + iii]) {
          min = tmpBuf[start + iii];
        }
      }
      noiseThr[sb] = min * width;
    }
  }
}
#else
static void mp3encPsy_long_window(mpaPsychoacousticBlock    *pBlock,
                                  mpaPsychoacousticBlockCom *pBlockCom,
                                  Ipp32f                    *input_data,
                                  Ipp32s                     ch,
                                  Ipp32s                     gr,
                                  Ipp32f                     * pTmpBuf)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_LONG);// len=72 is multiply 4 - it's good for DotProd main branch
  IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_LONG);
  Ipp32f *nb,*nb_l;
  Ipp32f *noiseThr;
  Ipp32s b, num_ptt, sb, index, len4;

  num_ptt = pBlockCom->longWindow->num_ptt;

  ippsSet_8u(0, (Ipp8u*)e_b, (MAX_PPT_LONG)*sizeof(Ipp32f));
  ippsSet_8u(0, (Ipp8u*)ecb, (MAX_PPT_LONG)*sizeof(Ipp32f));

  index = 0;

  for (b = 0; b < num_ptt; b++) {
    Ipp32f *tmp_rsqr = &input_data[index];
    Ipp32s len = pBlockCom->longWindow->numlines[b];
    ippsDotProd_32f(tmp_rsqr, tmp_rsqr, len, &e_b[b]);
    index += len;
  }

  len4 = (num_ptt + 3)&(-4);
  for (b = 0; b < num_ptt; b++) {
    Ipp32f *tmp_ptr = (Ipp32f*)pBlockCom->sprdngf_long + b * num_ptt;
    ippsDotProd_32f(e_b, tmp_ptr, len4/*num_ptt*/, &ecb[b]);
  }

  nb   = pBlock->nb_long[pBlockCom->nb_curr_index];
  nb_l = pBlock->nb_long[pBlockCom->nb_prev_index];

  ippsMulC_32f_I(ATTENUATION, ecb, len4/*num_ptt*/);
  ippsMul_32f(ecb, (Ipp32f*)pBlockCom->rnorm_long, (Ipp32f*)nb, len4/*num_ptt*/);
  ippsMaxEvery_32f_I(pBlockCom->longWindow->qsthr, nb, len4/*num_ptt*/);

  if (pBlock->block_type != STOP_TYPE) {
    ippsMulC_32f(nb_l, 0.01f, ecb, len4/*num_ptt*/);
    ippsMaxEvery_32f_I(nb, ecb, len4/*num_ptt*/);
    ippsMulC_32f_I(2, nb_l, len4/*num_ptt*/);
    ippsMinEvery_32f_I(ecb, nb_l, len4/*num_ptt*/);
  } else {
    ippsCopy_32f(nb, nb_l, len4/*num_ptt*/);
  }

  noiseThr = &pBlockCom->noiseThr[4 * gr + ch][0];
  {
    //IPP_ALIGNED_ARRAY(32, Ipp32f, tmpBuf, LEN_MDCT_LINE);
    Ipp32f* tmpPtr;
    Ipp32s iii;

    tmpPtr = pTmpBuf/*tmpBuf*/;
    for (b = 0; b < num_ptt; b++) {
      Ipp32s len = pBlockCom->longWindow->numlines[b];
      Ipp32f val = nb_l[b] / len;
      for (iii = 0; iii < len; iii++) {
        tmpPtr[iii] = val;
      }
      tmpPtr+=len;
    }

    for (sb = 0; sb < pBlockCom->num_sfb_long; sb++) {
      Ipp32s start = pBlockCom->sfb_offset_long[sb];
      Ipp32s width = pBlockCom->sfb_offset_long[sb + 1] - start;
      Ipp32f min = pTmpBuf[start]/*tmpBuf[start]*/;
      Ipp32s iii;
      
      tmpPtr = pTmpBuf+start/*tmpBuf+start*/;
      
      for (iii = 1; iii < width; iii++) {
        /*if (min > tmpBuf[start + iii]) { min = tmpBuf[start + iii]; }*/
         min = (min > tmpPtr[iii]) ? tmpPtr[iii] : min;
      }
      noiseThr[sb] = min * width;
    }
  }
}
#endif

/****************************************************************************/

static void mp3encPsy_short_window(mpaPsychoacousticBlock    *pBlock,
                                   mpaPsychoacousticBlockCom *pBlockCom,
                                   Ipp32f                    *input_data,
                                   Ipp32s                     ch,
                                   Ipp32s                     lastBlockType,
                                   Ipp32s                     gr)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, e_b, MAX_PPT_SHORT);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ecb, MAX_PPT_SHORT);
  IPP_ALIGNED_ARRAY(32, Ipp32f, r, 192);
  Ipp32f *nb,*nb_s;
  Ipp32f *noiseThr;
  Ipp32s b, num_ptt, sb, index;
  Ipp32s win_counter;

  for (win_counter = 0; win_counter < 3; win_counter++) {
    for (sb = 0; sb <= SBBND_S; sb++) {
      Ipp32s start = pBlockCom->sfb_offset_short[sb];
      Ipp32s width = pBlockCom->sfb_offset_short[sb + 1] - start;

      ippsCopy_32f(input_data + 3 * start + win_counter * width,
                   r + start, width);
    }

    num_ptt = pBlockCom->shortWindow->num_ptt;

    index = 0;

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_rsqr = &r[index];
      Ipp32s len = pBlockCom->shortWindow->numlines[b];

      ippsDotProd_32f(tmp_rsqr, tmp_rsqr, len, &e_b[b]);
      index += len;
    }

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = (Ipp32f*)pBlockCom->sprdngf_short + b * num_ptt;

      ippsDotProd_32f(e_b, tmp_ptr, num_ptt, &ecb[b]);
    }

    nb = pBlock->nb_short[win_counter];
    if (win_counter == 0) {
      nb_s = pBlock->nb_short[2];
    } else {
      nb_s = pBlock->nb_short[win_counter - 1];
    }

    ippsMulC_32f_I(ATTENUATION, ecb, num_ptt);
    ippsMul_32f((Ipp32f*)pBlockCom->rnorm_short, ecb, (Ipp32f*)nb, num_ptt);

    ippsMaxEvery_32f_I(pBlockCom->shortWindow->qsthr, nb, num_ptt);

    if ((lastBlockType != START_TYPE) || (win_counter != 0)) {
      ippsMulC_32f(nb_s, 0.01f, ecb, num_ptt);
      ippsMaxEvery_32f_I(nb, ecb, num_ptt);
      ippsMulC_32f_I(2, nb_s, num_ptt);
      ippsMinEvery_32f_I(ecb, nb_s, num_ptt);
    } else {
      ippsCopy_32f(nb, nb_s, num_ptt);
    }

    noiseThr = &pBlockCom->noiseThr[4 * gr + ch][0];
    {
      IPP_ALIGNED_ARRAY(32, Ipp32f, tmpBuf, 192);
      Ipp32f* tmpPtr;
      Ipp32s iii;

      tmpPtr = tmpBuf;
      for (b = 0; b < num_ptt; b++) {
        Ipp32s len = pBlockCom->shortWindow->numlines[b];
        Ipp32f val = nb_s[b] / len;
        for (iii = 0; iii < len; iii++) {
          tmpPtr[0] = val;
          tmpPtr++;
        }
      }

      tmpPtr = tmpBuf;
      for (sb = 0; sb < pBlockCom->num_sfb_short; sb++) {
        Ipp32s start = pBlockCom->sfb_offset_short[sb];
        Ipp32s width = pBlockCom->sfb_offset_short[sb + 1] - start;
        Ipp32f min = tmpBuf[start];
        Ipp32f thr;
        Ipp32s iii;

        for (iii = 1; iii < width; iii++) {
          if (min > tmpBuf[start + iii]) {
            min = tmpBuf[start + iii];
          }
        }

        thr = min * width;

        /* pre-echo control */
        if (win_counter == pBlock->attackIndex) {
          thr *= 0.01f;
          if (thr > MAX_TRH_SHORT)
            thr = MAX_TRH_SHORT;
        }

        noiseThr[3 * sb + win_counter] = thr;
      }
    }
  }
}

/****************************************************************************/

MP3Status mp3encInitPsychoacousticCom(MP3Enc *state,
                                      Ipp32s  mpeg_id,
                                      Ipp32s  freq_index,
                                      Ipp32s  ns_mode)
{
  Ipp32s num_ptt, b, bb;
  Ipp32f *bval;
  mpaPsychoacousticBlockCom *pBlock;

  if (state)
    pBlock = &(state->psychoacoustic_block_com);
  else
    pBlock = 0;

  if (pBlock) {
    ippsZero_8u((Ipp8u*)pBlock, sizeof(mpaPsychoacousticBlockCom));

    pBlock->nb_curr_index = 1;
    pBlock->nb_prev_index = 0;

    pBlock->sfb_offset_long  = mp3enc_sfBandIndex[mpeg_id][freq_index].l;
    pBlock->sfb_offset_short = mp3enc_sfBandIndex[mpeg_id][freq_index].s;
    pBlock->sfb_offset_short_interleave = mp3enc_sfBandIndex[mpeg_id][freq_index].si;

    pBlock->longWindow    = (mpaPsyPartitionTable*)&mp3_psy_partition_tables_long[mpeg_id][freq_index];
    pBlock->shortWindow   = (mpaPsyPartitionTable*)&mp3_psy_partition_tables_short[mpeg_id][freq_index];

    num_ptt = pBlock->longWindow->num_ptt;
    bval = pBlock->longWindow->bval;

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = pBlock->sprdngf_long + b * num_ptt;
      Ipp32f tmp = 0;

      for (bb = 0; bb < num_ptt; bb++) {
        tmp_ptr[bb] = sprdngf(bval[bb], bval[b], 3);
        tmp += tmp_ptr[bb];
      }
      pBlock->rnorm_long[b] = 1/tmp;
    }

    num_ptt = pBlock->shortWindow->num_ptt;
    bval = pBlock->shortWindow->bval;

    for (b = 0; b < num_ptt; b++) {
      Ipp32f *tmp_ptr = pBlock->sprdngf_short + b * num_ptt;
      Ipp32f tmp = 0;

      for (bb = 0; bb < num_ptt; bb++) {
        tmp_ptr[bb] = sprdngf(bval[bb], bval[b], 3);
        tmp += tmp_ptr[bb];
      }
      pBlock->rnorm_short[b] = 1/tmp;
    }

    pBlock->attackThreshold = (Ipp32f)(1280/state->bit_rate_per_ch);
    pBlock->ns_mode = ns_mode;
  }

  return MP3_OK;
}

/****************************************************************************/

void mp3encInitPsychoacoustic(mpaPsychoacousticBlockCom *pBlockCom,
                              mpaPsychoacousticBlock    *pBlock)
{
  pBlock->block_type = NORM_TYPE;
  pBlock->desired_block_type = NORM_TYPE;
  pBlock->bitsToPECoeff = 0.5f;
  pBlock->scalefactorDataBits = 60;
  pBlock->PEtoNeededPECoeff = 1;

  ippsCopy_32f(pBlockCom->longWindow->qsthr, pBlock->nb_long[0],
               pBlockCom->longWindow->num_ptt);

  ippsCopy_32f(pBlockCom->longWindow->qsthr, pBlock->nb_long[1],
               pBlockCom->longWindow->num_ptt);

  pBlock->avWinEnergy = 0;
  pBlock->lastWindowEnergy = 0;
  pBlock->avWinEnergyLow = 0;
  pBlock->lastWindowEnergyLow = 0;
  pBlock->attackIndex = 0;
  pBlock->lastAttackIndex = 0;
}

/****************************************************************************/

void mp3enc_psychoacoustic_l3(MP3Enc *state,
                              Ipp32s  gr, Ipp32f* pTmpBuf)
{
  mpaPsychoacousticBlock    *pBlock[2];
  mpaPsychoacousticBlockCom *pBlockCom = &(state->psychoacoustic_block_com);
  Ipp32s lastBlockType[2];
  Ipp32s ch;

  for (ch = 0; ch < 2; ch++) {
    pBlock[ch] = &state->psychoacoustic_block[ch];
  }

  for (ch = 0; ch < state->com.stereo; ch++) {
    mp3encBlockSwitching(state, gr, ch);
  }

  if (state->com.stereo == 2) {
    if (state->com.stereo_mode == MPA_MS_STEREO) {
      if (pBlock[0]->next_desired_block_type !=
        pBlock[1]->next_desired_block_type) {
          pBlock[0]->next_desired_block_type =
            pBlock[1]->next_desired_block_type = SHORT_TYPE;
        }
    }
  }

  for (ch = 0; ch < state->com.stereo; ch++) {
    Ipp32s win_seq;
    Ipp32s next_desired_block_type = pBlock[ch]->next_desired_block_type;

    lastBlockType[ch] = pBlock[ch]->block_type;

    if (next_desired_block_type == NORM_TYPE) {
      if (pBlock[ch]->desired_block_type == SHORT_TYPE)
        next_desired_block_type = STOP_TYPE;
    } else {
      if (pBlock[ch]->desired_block_type == NORM_TYPE )
        pBlock[ch]->desired_block_type = START_TYPE;
      if ( pBlock[ch]->desired_block_type == STOP_TYPE ) {
        pBlock[ch]->desired_block_type = SHORT_TYPE;
      }
    }

    state->com.si_blockType[gr][ch] = pBlock[ch]->block_type =
      pBlock[ch]->desired_block_type;

    pBlock[ch]->desired_block_type = next_desired_block_type;

    if (state->com.si_blockType[gr][ch] == NORM_TYPE) {
      state->com.si_winSwitch[gr][ch] = 0;
    } else {
      state->com.si_winSwitch[gr][ch] = 1;
    }

    state->com.si_mixedBlock[gr][ch] = 0;

    win_seq = state->ics[ch].windows_sequence =
      ((pBlock[ch]->block_type == SHORT_TYPE) ? 1 : 0);
  }

  mp3enc_mdctBlock(state, gr);

  for (ch = 0; ch < state->com.stereo; ch++) {
    if (state->com.si_blockType[gr][ch] == SHORT_TYPE) {
      mp3encPsy_short_window(pBlock[ch], pBlockCom, state->mdct_out[gr][ch],
                             ch, lastBlockType[ch], gr);
    } else {
      mp3encPsy_long_window(pBlock[ch], pBlockCom, state->mdct_out[gr][ch], ch, gr, pTmpBuf);
    }
  }

  state->psychoacoustic_block_com.nb_curr_index++;
  state->psychoacoustic_block_com.nb_curr_index &= 1;

  state->psychoacoustic_block_com.nb_prev_index++;
  state->psychoacoustic_block_com.nb_prev_index &= 1;

}

/****************************************************************************/

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER
