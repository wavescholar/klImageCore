/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel Integrated Performance Primitives AAC Encode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include "aac_enc_ltp_fp.h"
#include "align.h"

static Ipp32f g_ltp_coef[8] = {
  0.570829f,
  0.696616f,
  0.813004f,
  0.911304f,
  0.984900f,
  1.067894f,
  1.194601f,
  1.369533f
};

/****************************************************************************/

void ltpEncode(Ipp32f *inBuf,
               Ipp32f *ltpBuf,
               Ipp32f *predictedBuf,
               Ipp32s *ltpDelay,
               Ipp32s *ltpInd,
               IppsFFTSpec_R_32f* corrFft,
               Ipp8u  *corrBuff)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp1, 4096);
  IPP_ALIGNED_ARRAY(32, Ipp32f, corr, 4096);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, tmp1, 4096);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, corr, 4096);
#endif
  Ipp32f saveSqCorr, en, saveEn, tmpCorr;
  Ipp32f gain, real_coef;
  Ipp32s lag_size, start, delay, ind;
  Ipp32s i;

  /* Note: inBuf[2048 : 4095] = 0, ltpBuf[3072 : 4096] = 0 */
  ippsFFTFwd_RToPack_32f(inBuf, tmp1, corrFft, corrBuff);
  ippsFFTFwd_RToPack_32f(ltpBuf+1, corr, corrFft, corrBuff);
  ippsMulPackConj_32f_I(corr, tmp1, 4096);
  ippsFFTInv_PackToR_32f(tmp1, corr, corrFft, corrBuff);

  //ippsCrossCorr_32f(inBuf, 2048, ltpBuf+1, 3 * 1024, corr, 2048, 0);
  ippsDotProd_32f(ltpBuf + 2048, ltpBuf + 2048, 1024, &en);

  saveSqCorr = corr[2047] * corr[2047];
  delay = 0;
  saveEn = en;
  if (corr[2047] < 0)
    saveSqCorr = -saveSqCorr;

  for (i = 1; i <= 1024; i++) {
    tmpCorr = corr[2047 - i];
    en += ltpBuf[2048 - i] * ltpBuf[2048 - i];

    if (tmpCorr >= 0) {
      if (saveSqCorr * en < tmpCorr * tmpCorr * saveEn) {
        saveSqCorr = tmpCorr * tmpCorr;
        delay = i;
        saveEn = en;
      }
    }
  }

  for (i = 1024 + 1; i < 2048; i++) {
    tmpCorr = corr[2047 - i];
    en += ltpBuf[2048 - i] * ltpBuf[2048 - i];
    en -= ltpBuf[4096 - i] * ltpBuf[4096 - i];

    if (tmpCorr >= 0) {
      if (saveSqCorr * en < tmpCorr * tmpCorr * saveEn) {
        saveSqCorr = tmpCorr * tmpCorr;
        delay = i;
        saveEn = en;
      }
    }
  }

  tmpCorr = corr[2047 - delay];

  if (tmpCorr < 0) {
    *ltpDelay = -1;
    return;
  } else if (saveEn <= 0) {
    gain = 0;
  } else {
    gain = tmpCorr / saveEn;
  }

  ind = 0;

  for (i = 0; i < 8; i++) {
    ind = i;
    if (gain <= g_ltp_coef[i])
      break;
  }

  if (ind > 0) {
    if (((gain - g_ltp_coef[ind - 1]) * (gain - g_ltp_coef[ind - 1])) <
        ((gain - g_ltp_coef[ind]) * (gain - g_ltp_coef[ind]))) {
          ind -= 1;
    }
  }

  real_coef = g_ltp_coef[ind];
  lag_size = 2048;
  start = 2048 - delay;

  if (delay < 1024)
    lag_size = 1024 + delay;
#if 1
  ippsMulC_32f(&ltpBuf[start], real_coef, predictedBuf, lag_size);
  ippsSet_32f(.0f, &predictedBuf[lag_size], (2048-lag_size));
#else
  for (i = 0; i < lag_size; i++)
    predictedBuf[i] = real_coef * ltpBuf[i + start];
  for (; i < 2048; i++)
    predictedBuf[i] = 0;
#endif
  *ltpDelay = delay;
  *ltpInd = ind;
}

/****************************************************************************/

void ltpBufferUpdate(Ipp32f **ltpBuffer,
                     Ipp32f **prevSamples,
                     Ipp32f **predictedSpectrum,
                     sEnc_channel_pair_element *pElement,
                     sFilterbank *filterbank,
                     Ipp32s *sfb_offset_short,
                     Ipp32s *prevWindowShape,
                     Ipp32s numCh)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, spectrum, 2*1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, spectrum_i, 1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, currSamples, 1024);
  IPP_ALIGNED_ARRAY(32, Ipp32f, real_sf, 60);
  sEnc_individual_channel_stream *pStream[2];
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, spectrum, 2*1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, spectrum_i, 1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, currSamples, 1024);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, real_sf, 60);
  static sEnc_individual_channel_stream *pStream[2];
#endif
  Ipp32f         *pSpectrumData;
  Ipp32s         i, ch;

  pStream[0] = pElement->p_individual_channel_stream_0;
  pStream[1] = pElement->p_individual_channel_stream_1;

  for (ch = 0; ch < numCh; ch++) {
    Ipp32s  max_sfb = pStream[ch]->max_sfb;
    Ipp32s  max_line = pStream[ch]->max_line;

    if (max_line > 0) {
      ippsPow43_16s32f(pStream[ch]->x_quant, &spectrum[ch*1024], max_line);

      if (pStream[ch]->windows_sequence != EIGHT_SHORT_SEQUENCE) {
        ippsCalcSF_16s32f(pStream[ch]->scale_factors, SF_OFFSET, real_sf, max_sfb);
        ippsScale_32f_I(&spectrum[ch*1024], real_sf, pStream[ch]->sfb_offset, max_sfb);
      } else {
        Ipp16s *scale_factors = pStream[ch]->scale_factors;
        Ipp32s *sfb_offset = pStream[ch]->sfb_offset;
        Ipp32s num_window_groups = pStream[ch]->num_window_groups;
        Ipp32s g;

        for (g = 0; g < num_window_groups; g++) {
          ippsCalcSF_16s32f(scale_factors, SF_OFFSET, real_sf, max_sfb);
          ippsScale_32f_I(&spectrum[ch*1024], real_sf, sfb_offset, max_sfb);
          scale_factors += max_sfb;
          sfb_offset += max_sfb;
        }
      }
    }
  }

  if (pElement->ms_mask_present >= 1) {
    Ipp32s *sfb_offset = pStream[0]->sfb_offset;
    Ipp32f *ptrL = &spectrum[0*1024];
    Ipp32f *ptrR = &spectrum[1*1024];
    Ipp32f li, ri;
    Ipp32s max_sfb = pStream[0]->max_sfb;
    Ipp32s num_window_groups = pStream[0]->num_window_groups;
    Ipp32s sfb;

    for (sfb = 0; sfb < num_window_groups * max_sfb; sfb++) {
      if ((pElement->ms_used[sfb] || pElement->ms_mask_present == 2)) {
        for (i = sfb_offset[sfb]; i < sfb_offset[sfb + 1]; i++) {
          li = (ptrL[i] + ptrR[i]);
          ri = (ptrL[i] - ptrR[i]);
          ptrL[i] = li;
          ptrR[i] = ri;
        }
      }
    }
  }

  for (ch = 0; ch < numCh; ch++) {
    Ipp32s  max_sfb = pStream[ch]->max_sfb;
    Ipp32s  max_line = pStream[ch]->max_line;

    if (pStream[ch]->windows_sequence != EIGHT_SHORT_SEQUENCE) {
      if (pStream[ch]->predictor_data_present) {
        ippsAdd_32f_I(predictedSpectrum[ch], &spectrum[ch*1024], max_line);
      }
      ippsZero_32f(&spectrum[ch*1024] + max_line, 1024 - max_line);
      pSpectrumData = &spectrum[ch*1024];
    } else {

      /* Ipp16s block deinterleave */
      Ipp32s *len_window_group = pStream[ch]->len_window_group;
      Ipp32s num_window_groups = pStream[ch]->num_window_groups;
      Ipp32f *ptrIn = &spectrum[ch*1024];
      Ipp32f *ptrOut = spectrum_i;
      Ipp32s sfb, w, j, g;

      for (g = 0; g < num_window_groups; g++) {
        for (sfb = 0; sfb < max_sfb; sfb++) {
          Ipp32s sfb_start = sfb_offset_short[sfb];
          Ipp32s sfb_end = sfb_offset_short[sfb+1];
          Ipp32s sfb_width = sfb_end - sfb_start;

          for (j = 0; j < len_window_group[g]; j++) {
            for (w = 0; w < sfb_width; w++) {
              ptrOut[w + sfb_start + 128 * j] = *ptrIn;
              ptrIn++;
            }
          }
        }

        for (j = 0; j < len_window_group[g]; j++) {
          for (w = sfb_offset_short[max_sfb]; w < 128; w++) {
            ptrOut[w + 128 * j] = 0;
          }
        }
        ptrOut += 128 * len_window_group[g];
      }
      pSpectrumData = spectrum_i;
    }

    FilterbankDec(filterbank, pSpectrumData,
                  prevSamples[ch], pStream[ch]->windows_sequence,
                  pStream[ch]->window_shape, prevWindowShape[ch],
                  currSamples, prevSamples[ch]);

//    for (i = 0; i < 1024; i++) {
//      ltpBuffer[ch][i] = ltpBuffer[ch][i + 1024];
//    }
    ippsCopy_32f(&ltpBuffer[ch][1024], &ltpBuffer[ch][0], 1024);

    for (i = 0; i < 1024; i++) {
      Ipp32f tmp = currSamples[i];

      if (tmp > 0) tmp += 0.5f;
      else         tmp -= 0.5f;

      if (tmp > 32767)  tmp = 32767;
      if (tmp < -32768) tmp = -32768;

      ltpBuffer[ch][i + 1024] = (Ipp32f)(Ipp32s)tmp;
    }

    for (i = 0; i < 1024; i++) {
      Ipp32f tmp = prevSamples[ch][i];

      if (tmp > 0) tmp += 0.5f;
      else         tmp -= 0.5f;

      if (tmp > 32767)  tmp = 32767;
      if (tmp < -32768) tmp = -32768;

      ltpBuffer[ch][i + 2048] = (Ipp32f)(Ipp32s)tmp;
    }
  }
}

/****************************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

