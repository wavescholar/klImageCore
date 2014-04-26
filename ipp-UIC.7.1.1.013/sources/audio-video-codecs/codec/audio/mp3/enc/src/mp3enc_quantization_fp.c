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

#include "mp3enc_own_fp.h"

#define SF_OFFSET 210
#define MAX_QUANT     8206
#define MAGIC_NUMBER  (-0.0946 + 0.5)

/****************************************************************************/

void mp3enc_quantCalcAvailableBitsFrame(MP3Enc *state)
{
  Ipp32s grnum;
  Ipp32s len;
  Ipp32s bitsPerFrame;
  Ipp32s mean_bits;
  Ipp32s max_bits_in_buf;
  IppMP3FrameHeader *header = &state->com.header;
  Ipp32s stereo = state->com.stereo;
  Ipp32s max_br;

  grnum = state->com.grnum;

  len = 32;   // header

  if (state->com.header.id) {
    if (stereo == 1)
      len += 136;
    else
      len += 256;
    max_bits_in_buf = 8*511;
    max_br = 320;
  } else {
    if (stereo == 1)
      len += 72;
    else
      len += 136;
    max_bits_in_buf = 8*255;
    max_br = 160;
  }

  if (header->protectionBit)
    len += 16;

  state->com.frameBits = len;
  bitsPerFrame = state->com.slot_size << 3;

  mean_bits = (bitsPerFrame - len);
  state->mean_bits = mean_bits >> (grnum - 1);
  state->max_bits_in_buf = max_bits_in_buf;

  state->maxPossibleBits = (state->com.slot_sizes[14] * 8 - len) >> (grnum - 1);

  if (state->com.br_mode == MPAENC_CBR) {
    state->bits_in_buf = state->com.si_main_data_begin * 8;
  }
}

/****************************************************************************/
#if 0
static void mp3enc_LimitSF(Ipp32f *mdct_scaled,
                           Ipp32s  size,
                           Ipp32s *startSF,
                           Ipp32s *finishSF)
{
  Ipp32f start_scalefac, finish_scalefac, max_mdct_line;

  startSF[0] = -SF_OFFSET;
  finishSF[0] = -SF_OFFSET;
  max_mdct_line = 0;

  ippsMax_32f(mdct_scaled, size, &max_mdct_line);

  if (max_mdct_line > 0) {
    start_scalefac = (Ipp32f)log(max_mdct_line/MAX_QUANT)/(Ipp32f)log(2);
    start_scalefac *= (Ipp32f)16/(Ipp32f)3;
    startSF[0] = (Ipp32s)floor((Ipp64f)start_scalefac);

    if (startSF[0] < start_scalefac) startSF[0]++;

    finish_scalefac = (Ipp32f)log(max_mdct_line/(1 - MAGIC_NUMBER))/(Ipp32f)log(2);
    finish_scalefac *= (Ipp32f)16/(Ipp32f)3;
    finishSF[0] = (Ipp32s)floor((Ipp64f)finish_scalefac);

    if (finishSF[0] < finish_scalefac) finishSF[0]++;

    finishSF[0]++;

    if (startSF[0] > 255 - SF_OFFSET) startSF[0] = 255 - SF_OFFSET;
    else if (startSF[0] < -SF_OFFSET) startSF[0] = -SF_OFFSET;

    if (finishSF[0] > 255 - SF_OFFSET) finishSF[0] = 255 - SF_OFFSET;
    else if (finishSF[0] < -SF_OFFSET) finishSF[0] = -SF_OFFSET;
  }
}
#else
static void mp3enc_LimitSF(Ipp32f *mdct_scaled,
                           Ipp32s  size,
                           Ipp32s *pStartSF,
                           Ipp32s *pFinishSF)
{
  Ipp32f start_scalefac, finish_scalefac, max_mdct_line;

  Ipp32s startSF = -SF_OFFSET;
  Ipp32s finishSF = -SF_OFFSET;
  max_mdct_line = 0;

  ippsMax_32f(mdct_scaled, size, &max_mdct_line);

  if (max_mdct_line > 0) {
     start_scalefac = max_mdct_line/(Ipp32f)MAX_QUANT;
     finish_scalefac = max_mdct_line/(1.f - (Ipp32f)MAGIC_NUMBER);
    start_scalefac = (Ipp32f)logf(start_scalefac);
    finish_scalefac = (Ipp32f)logf(finish_scalefac);

    start_scalefac *= (Ipp32f)16.f/(Ipp32f)3.f/(Ipp32f)logf(2.f);
    finish_scalefac *= (Ipp32f)16.f/(Ipp32f)3.f/(Ipp32f)logf(2.f);

    startSF = (Ipp32s)floorf(start_scalefac);
    finishSF = (Ipp32s)floorf(finish_scalefac);

    startSF = (startSF < start_scalefac) ? startSF+1 : startSF;
    finishSF = (finishSF < finish_scalefac) ? finishSF+1 : finishSF;

    finishSF++;

    startSF = (startSF > (255 - SF_OFFSET)) ? (255 - SF_OFFSET) : startSF;
    finishSF = (finishSF > (255 - SF_OFFSET)) ? (255 - SF_OFFSET) : finishSF;

    startSF = (startSF < -SF_OFFSET) ? -SF_OFFSET : startSF;
    finishSF = (finishSF < -SF_OFFSET) ? -SF_OFFSET : finishSF;
  }
  *pStartSF = startSF;
  *pFinishSF = finishSF;
}
#endif

/****************************************************************************/
static void mp3enc_main_loop(MP3Enc             *state,
                             sQuantizationBlock *pBlock,
                             mpaICS             *pStream,
                             Ipp32f             *mdct_scaled,
                             Ipp32s              gr,
                             Ipp32s              ch)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmp_x_quant, LEN_MDCT_LINE);
  Ipp32f sf, temp;
  Ipp32s start_common_scalefac = pBlock->start_common_scalefac;
  Ipp32s finish_common_scalefac = pBlock->finish_common_scalefac;
  Ipp32s common_scalefactor;
  Ipp32s common_scalefactor_update;
  Ipp32s needed_bits;
  Ipp32s num_scale_factor;
  Ipp32s i, down;

  num_scale_factor = pStream->max_sfb;

  common_scalefactor = pBlock->last_frame_common_scalefactor[0];
  common_scalefactor_update = pBlock->common_scalefactor_update[0];

  if (common_scalefactor < start_common_scalefac)
    common_scalefactor = start_common_scalefac;

  if (common_scalefactor > finish_common_scalefac)
    common_scalefactor = finish_common_scalefac;

  down = 0;
  for(;;) {
    sf = (Ipp32f)pow(2.0, -common_scalefactor * (3./16.));
    temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;

    ippsAddC_32f(mdct_scaled, temp, tmp_x_quant, pStream->max_line);
    ippsMulC_Low_32f16s(tmp_x_quant, sf, state->com.quant_ix[gr][ch], pStream->max_line);

    needed_bits = mp3enc_quantCalcBits(&state->com, gr, ch);

    if (needed_bits == pBlock->available_bits)
      break;

    if (needed_bits > pBlock->available_bits) {
      if (common_scalefactor == finish_common_scalefac)
        break;

      if (common_scalefactor_update < 0) {
        common_scalefactor_update = -common_scalefactor_update;
      }
      common_scalefactor_update = (common_scalefactor_update + 1) >> 1;
      down = 1;
    } else {
      if (common_scalefactor == start_common_scalefac)
        break;

      //if (pBlock->ns_mode) {
        if (needed_bits < pBlock->min_bits_per_frame) {
          if (down == 1) {
            break;
          }
          common_scalefactor_update = -1;
        } else {
          common_scalefactor_update = 1;
        }
      //}

      if (common_scalefactor_update == 1)
        break;

      if (common_scalefactor_update > 0) {
        common_scalefactor_update = -common_scalefactor_update;
      }
      common_scalefactor_update >>= 1;
    }

    common_scalefactor += common_scalefactor_update;

    if (common_scalefactor < start_common_scalefac)
      common_scalefactor = start_common_scalefac;

    if (common_scalefactor > finish_common_scalefac)
      common_scalefactor = finish_common_scalefac;

  }

  pBlock->common_scalefactor_update[0] =
    common_scalefactor - pBlock->last_frame_common_scalefactor[0];
  pBlock->last_frame_common_scalefactor[0] = common_scalefactor;

  if (pBlock->common_scalefactor_update[0] >= 0) {
    if (pBlock->common_scalefactor_update[0] <= 2)
      pBlock->common_scalefactor_update[0] = 2;
  } else {
    if (pBlock->common_scalefactor_update[0] >= -2)
      pBlock->common_scalefactor_update[0] = -2;
  }

  pBlock->used_bits = needed_bits;

  state->com.si_globGain[gr][ch] = (Ipp16s)(common_scalefactor + SF_OFFSET);
  state->com.si_part23Len[gr][ch] = needed_bits;

  state->com.frameBits += state->com.si_part23Len[gr][ch];

  if (pBlock->ns_mode) {
    for (i = 0; i < num_scale_factor; i++) {
      pStream->scale_factors[i] = (Ipp16s)(state->com.si_globGain[gr][ch] -
        (pBlock->ns_scale_factors[i] << (state->com.si_sfScale[gr][ch] + 1)) );
    }
  }
}

/****************************************************************************/

#define SCALEFAC_POW(x) (pow(2.0, -((x) - SF_OFFSET) * (3./16.)))

static Ipp32f mp3_NoiseCalculation(Ipp32f *mdct_scaled,
                                   Ipp32f *mdct_line_abs,
                                   Ipp16s *scalefactor,
                                   Ipp16s *maxXQuant,
                                   Ipp32s *numZero,
                                   Ipp32s  width)

{
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, LEN_MDCT_LINE);
  IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, LEN_MDCT_LINE);
  Ipp32f sf, temp, real_sf, noise;
  Ipp32s i;

  sf = (Ipp32f)SCALEFAC_POW(scalefactor[0] + SF_OFFSET);
  temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;

  ippsAddC_32f(mdct_scaled, temp, mdct_rqnt, width);
  ippsMulC_Low_32f16s(mdct_rqnt, sf, (Ipp16s*)x_quant_unsigned, width);
  ippsPow43_16s32f(x_quant_unsigned, mdct_rqnt, width);
  ippsCalcSF_16s32f(scalefactor, 0, &real_sf, 1);
  ippsMulC_32f_I(real_sf, mdct_rqnt, width);
  ippsSub_32f_I(mdct_line_abs, mdct_rqnt, width);
  ippsDotProd_32f(mdct_rqnt, mdct_rqnt, width, &noise);

  ippsMax_16s(x_quant_unsigned, width, maxXQuant);

  if (numZero != 0) {
    numZero[0] = 0;
    for (i = 0; i < width; i++) {
      if (x_quant_unsigned[i] == 0) {
        numZero[0] += 1;
      }
    }
  }

  return noise;
}

/****************************************************************************/

static void mp3enc_MinMaxSF(MP3Enc *state,
                            mpaICS *pStream,
                            Ipp32s *startSF,
                            Ipp16s *scalefac,
                            Ipp16s *maxXQuant,
                            Ipp32s *minSf,
                            Ipp32s *maxSf,
                            Ipp32s  numSfb,
                            Ipp32s  gr,
                            Ipp32s  ch)
{
  Ipp32f *mdct_scaled = state->mdct_scaled;
  const Ipp32u *sfb_offset = pStream->sfb_offset;
  Ipp32s *sfb_width = pStream->sfb_width;
  Ipp32s  maxSfDelta;
  int     sfb, update;

  minSf[0] = 100000;
  maxSf[0] = -100000;

  for (sfb = 0; sfb < numSfb; sfb++) {
    if (maxXQuant[sfb] != 0) {
      if (scalefac[sfb] > maxSf[0]) maxSf[0] = scalefac[sfb];
      if (scalefac[sfb] < minSf[0]) minSf[0] = scalefac[sfb];
    } else {
      ippsZero_32f(mdct_scaled + sfb_offset[sfb], sfb_width[sfb]);
    }
  }

  if (minSf[0] > maxSf[0]) {
    return; /* all zero */
  }

  maxSfDelta = 14;

  if ((maxSf[0] - minSf[0]) > 14)
    maxSfDelta = 28;

  update = 0;

  for (sfb = 0; sfb < numSfb; sfb++) {
    if (maxXQuant[sfb] != 0) {
      /* very big difference */
      if ((minSf[0] + maxSfDelta) < startSF[sfb]) {
        minSf[0] = startSF[sfb] - maxSfDelta;
        update = 1;
      }
    }
  }

  if (update == 1) {
    for (sfb = 0; sfb < numSfb; sfb++) {
      /* re quantization */
      if (scalefac[sfb] < minSf[0]) {
        IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, LEN_MDCT_LINE);
        IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, LEN_MDCT_LINE);
        //Ipp32f mdct_rqnt[LEN_MDCT_LINE];
        //Ipp16s x_quant_unsigned[LEN_MDCT_LINE];
        Ipp32f sf = (Ipp32f)SCALEFAC_POW(minSf[0] + SF_OFFSET);
        Ipp32f temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;

        ippsAddC_32f(mdct_scaled + sfb_offset[sfb], temp, mdct_rqnt, sfb_width[sfb]);
        ippsMulC_Low_32f16s(mdct_rqnt, sf, x_quant_unsigned, sfb_width[sfb]);

        ippsMax_16s(x_quant_unsigned, sfb_width[sfb], maxXQuant + sfb);

        scalefac[sfb] = (Ipp16s)minSf[0];
      }
    }
  }

  /* Try to use preemphasis table for MPEG1 long block only */
  if ((state->com.header.id == 1) && (pStream->windows_sequence == 0)) {
    if ((maxSf[0] - minSf[0]) > 14) {
      Ipp32s tmp_ns_scale_factors[21];
      Ipp32s tmpMinSf = 100000;
      Ipp32s tmpMaxSf = -100000;

      for (sfb = 0; sfb < numSfb; sfb++) {
        tmp_ns_scale_factors[sfb] = scalefac[sfb] + (mp3enc_pretab[sfb] * 2);
        if (maxXQuant[sfb] != 0) {
          if (tmp_ns_scale_factors[sfb] > tmpMaxSf) tmpMaxSf = tmp_ns_scale_factors[sfb];
          if (tmp_ns_scale_factors[sfb] < tmpMinSf) tmpMinSf = tmp_ns_scale_factors[sfb];
        }
      }

      if (((tmpMaxSf - tmpMinSf) <= 14) && (tmpMaxSf <= maxSf[0])) {
        state->com.si_preFlag[gr][ch] = 1;
        state->com.si_sfScale[gr][ch] = 0;
      } else {
        state->com.si_sfScale[gr][ch] = 1;

        if ((maxSf[0] - minSf[0]) > 28) {

          tmpMinSf = 100000;
          tmpMaxSf = -100000;

          for (sfb = 0; sfb < numSfb; sfb++) {
            tmp_ns_scale_factors[sfb] = scalefac[sfb] + (mp3enc_pretab[sfb] * 4);
            if (maxXQuant[sfb] != 0) {
              if (tmp_ns_scale_factors[sfb] > tmpMaxSf) tmpMaxSf = tmp_ns_scale_factors[sfb];
              if (tmp_ns_scale_factors[sfb] < tmpMinSf) tmpMinSf = tmp_ns_scale_factors[sfb];
            }
          }

          if (((tmpMaxSf - tmpMinSf) <= 28) && (tmpMaxSf <= maxSf[0])) {
            state->com.si_preFlag[gr][ch] = 1;
          } else {
            for (sfb = 0; sfb < numSfb; sfb++) {
              if (maxXQuant[sfb] != 0) {
                if (scalefac[sfb] > minSf[0] + 28)
                  scalefac[sfb] = (Ipp16s)(minSf[0] + 28);
              }
            }
            maxSf[0] = minSf[0] + 28;
            state->com.si_preFlag[gr][ch] = 0;
          }
        }
      }
    } else {
      state->com.si_preFlag[gr][ch] = 0;
      state->com.si_sfScale[gr][ch] = 0;
    }
  } else {

    if ((maxSf[0] - minSf[0]) > maxSfDelta) {
      for (sfb = 0; sfb < numSfb; sfb++) {
        if (maxXQuant[sfb] != 0) {
          if (scalefac[sfb] > minSf[0] + maxSfDelta)
            scalefac[sfb] = (Ipp16s)(minSf[0] + maxSfDelta);
        }
      }
      maxSf[0] = minSf[0] + maxSfDelta;
    }

    state->com.si_preFlag[gr][ch] = 0;
    state->com.si_sfScale[gr][ch] = 0;

    if ((maxSf[0] - minSf[0]) > 14)
      state->com.si_sfScale[gr][ch] = 1;
  }
}

/****************************************************************************/

static void mp3enc_FindSF(MP3Enc            *state,
                          mpaICS            *pStream,
                          sQuantizationData *qData,
                          Ipp32s            *startSF,
                          Ipp32s            *finishSF,
                          Ipp16s            *scalefac,
                          Ipp16s            *maxXQuant,
                          Ipp32s             numSfb,
                          Ipp32f             *pSqrtBuf)
{
  //IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_abs_al, LEN_MDCT_LINE);
  Ipp32f *mdct_line_abs = state->mdct_line_abs;
  Ipp32f *mdct_scaled = state->mdct_scaled;
  Ipp32f *noiseThr = qData->noiseThr;
  Ipp32f *energy = qData->energy;
  Ipp32s *isBounded = qData->isBounded;
  const Ipp32u *sfb_offset = pStream->sfb_offset;
  Ipp32s *sfb_width = pStream->sfb_width;
  Ipp16s scalefactor = 0;
  Ipp32f noise = .0f;
  Ipp32s stopSF = 0;
  Ipp32s allowHoles = 0;
  Ipp32s sfb = 0;

  if (/*(pStream->windows_sequence == 1) || */(qData->allowHoles == 1)) {
    allowHoles = 1;
  }

  for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
    Ipp32s sfb_start = sfb_offset[sfb];
    Ipp32s width = sfb_width[sfb];
    Ipp32s start_scalefac = startSF[sfb];
    Ipp32s finish_scalefac = finishSF[sfb];
    Ipp32s i, ind, counter, numZero;
    Ipp32f sqrtMdctLine = 0;
    Ipp16s maxXQuantSaved;
    Ipp32f minNoise;
#if 0
    for (i = 0; i < width; i++) {
      sqrtMdctLine += (Ipp32f)sqrt(mdct_line_abs[sfb_start+i]);
    }
#else
    for (i = 0; i < width; i++) {
      pSqrtBuf[i] = sqrtf(mdct_line_abs[sfb_start+i]);
    }
    sqrtMdctLine = 0;
    for (i = 0; i < width; i++) {
      sqrtMdctLine += pSqrtBuf[i];
    }
#endif

    if (((noiseThr[sfb] > 0) && (energy[sfb] >= noiseThr[sfb]))) {
      if (sfb < numSfb) {
        Ipp32f tmp;

        tmp = (Ipp32f)log((27.0/4.0) * noiseThr[sfb]/sqrtMdctLine)/(Ipp32f)log(2);
        tmp *= (Ipp32f)8/(Ipp32f)3;
        scalefactor = (Ipp16s)(tmp+0.5);
        if (scalefactor < start_scalefac)  scalefactor = (Ipp16s)start_scalefac;
        if (scalefactor > finish_scalefac) scalefactor = (Ipp16s)finish_scalefac;

        if (allowHoles == 1)
          isBounded[sfb] = 2;

        if (isBounded[sfb] != 2) { /* Holes are not allowed */
          Ipp32s j;

          scalefactor += 1;
          numZero = width;

          counter = -1;

          for (j = 0; j < 3; j++) {
            if ((numZero < 0.75f * width) || (scalefactor <= start_scalefac)) {
              break;
            }
            scalefactor -= 1;
            noise = mp3_NoiseCalculation(mdct_scaled + sfb_start, mdct_line_abs + sfb_start,
                    &scalefactor, maxXQuant + sfb, &numZero, width);
            counter++;
          }
        } else {
          counter = 0;
          noise = mp3_NoiseCalculation(mdct_scaled + sfb_start, mdct_line_abs + sfb_start,
                                      &scalefactor, maxXQuant + sfb, NULL, width);
        }

        scalefac[sfb] = scalefactor;

        if (counter == 0) {
          minNoise = noise;
          ind = 0;
          maxXQuantSaved = maxXQuant[sfb];

          /* Let's check other scalefactors. */
          /* Perhaps quantization with bigger scalefactors will give us less noise. */

          stopSF = finish_scalefac - scalefactor + 1;
          if (stopSF > 5) stopSF = 5; /* magic value :) */

          for (i = 1; i < stopSF; i++) {
            Ipp16s scalefactor1 = scalefactor + (Ipp16s)i;
            if (scalefactor1 > finish_scalefac) scalefactor1 = (Ipp16s)finish_scalefac;
            noise = mp3_NoiseCalculation(mdct_scaled + sfb_start, mdct_line_abs + sfb_start,
                                        &scalefactor1, maxXQuant + sfb, NULL, width);

            if (noise < minNoise) {
              minNoise = noise;
              ind = i;
              maxXQuantSaved = maxXQuant[sfb];
            }
          }
          scalefac[sfb] = scalefactor + (Ipp16s)ind;
          maxXQuant[sfb] = maxXQuantSaved;
        }
      } else { /* sfbs */
        noise = mp3_NoiseCalculation(mdct_scaled + sfb_start, mdct_line_abs + sfb_start,
                                     &scalefactor, maxXQuant + sfb, NULL, width);
      }
    } else {
      scalefac[sfb] = -SF_OFFSET;
      maxXQuant[sfb] = 0;
    }
  }
}

/****************************************************************************/

void mp3encQuantization(MP3Enc             *state,
                        sQuantizationBlock *pBlock,
                        mpaICS             *pStream,
                        sQuantizationData  *qData,
                        Ipp32s              gr,
                        Ipp32s              ch)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_line_abs_sqrt, LEN_MDCT_LINE);
  Ipp32f *mdct_line_abs    = state->mdct_line_abs;
  Ipp32f *mdct_scaled      = state->mdct_scaled;
  Ipp32f *mdct_line        = qData->mdct_line;
  const Ipp32u *sfb_offset = pStream->sfb_offset;
  Ipp32s *sfb_width        = pStream->sfb_width;
  Ipp16s *ns_scale_factors = pBlock->ns_scale_factors;
  Ipp32s  sfb;
  Ipp32s  startSF[MAX_SECTION_NUMBER];
  Ipp32s  finishSF[MAX_SECTION_NUMBER];
  Ipp16s  maxXQuant[MAX_SECTION_NUMBER];
  Ipp32s  numSfb = pStream->max_sfb;
  Ipp32s  minSf = 100000;
  Ipp32s  maxSf = -100000;
  Ipp32s  wnd;
  Ipp32f  ifqstep;

  mp3enc_quantIterReset(&state->com, gr, ch);

  ippsZero_16s(state->com.quant_ix[gr][ch] + pStream->max_line,
               LEN_MDCT_LINE - pStream->max_line);
  ippsZero_32f(mdct_line + pStream->max_line,
               LEN_MDCT_LINE - pStream->max_line);

  ippsAbs_32f(mdct_line, mdct_line_abs, pStream->max_line);
  ippsPow34_32f(mdct_line_abs, mdct_scaled, pStream->max_line);

  pBlock->start_common_scalefac = -SF_OFFSET;
  pBlock->finish_common_scalefac = -SF_OFFSET;

  if (pBlock->ns_mode) {
    for (sfb = 0; sfb < pStream->max_sfb; sfb++) {
      mp3enc_LimitSF(mdct_scaled + sfb_offset[sfb], sfb_width[sfb],
                     &startSF[sfb], &finishSF[sfb]);
    }

    //numSfb = pStream->max_sfb;
    if (pStream->windows_sequence == 0) {
      numSfb = pStream->max_sfb - 1;
      if (numSfb < 1)
        numSfb = pStream->max_sfb;
      //if (numSfb > 21) {
      //  numSfb = 21;
      //}
    } else {
      numSfb = pStream->max_sfb - 3;
      if (numSfb < 3)
        numSfb = pStream->max_sfb;
      //if (numSfb > 36) {
      //  numSfb = 36;
      //}
    }

    mp3enc_FindSF(state, pStream, qData, startSF, finishSF,
       ns_scale_factors, maxXQuant, numSfb, mdct_line_abs_sqrt);

    mp3enc_MinMaxSF(state, pStream, startSF, ns_scale_factors,
                    maxXQuant, &minSf, &maxSf, numSfb, gr, ch);



    if (state->com.si_sfScale[gr][ch] == 0) {
      ifqstep = 1.2968395f;
    } else {
      ifqstep = 1.6817928f;
    }

    /* Last sfb processing */
    if ((numSfb != pStream->max_sfb) && (minSf > maxSf)) {
      if (pStream->windows_sequence == 0) {
        ippsZero_32f(mdct_scaled + sfb_offset[numSfb], sfb_width[numSfb]);
      } else {
        ippsZero_32f(mdct_scaled + sfb_offset[numSfb], sfb_width[numSfb]);
        ippsZero_32f(mdct_scaled + sfb_offset[numSfb + 1], sfb_width[numSfb + 1]);
        ippsZero_32f(mdct_scaled + sfb_offset[numSfb + 2], sfb_width[numSfb + 2]);
      }
    }

    if (pStream->windows_sequence) {
      Ipp32s shift = state->com.si_sfScale[gr][ch] + 1;
      Ipp32s sfb1 = 0;

      for (sfb = 0; sfb < numSfb;) {
        for (wnd = 0; wnd < 3; wnd++) {
          if (maxXQuant[sfb] != 0) {
            if (maxXQuant[sfb] > 1) {
              ns_scale_factors[sfb] = (Ipp16s)((maxSf - ns_scale_factors[sfb]) >> shift);
            } else {
              ns_scale_factors[sfb] = ((Ipp16s)((maxSf - ns_scale_factors[sfb] + (1 << shift)-1) >> shift));
            }
            if (ns_scale_factors[sfb] > 0) {
              Ipp32f mul = (Ipp32f)pow(ifqstep, ns_scale_factors[sfb]);
              ippsMulC_32f_I(mul, mdct_scaled + sfb_offset[sfb], sfb_width[sfb]);
              state->com.scalefac_s[gr][ch][sfb1][wnd] = ns_scale_factors[sfb];
            }
          }
          sfb++;
        }
        sfb1++;
      }
      if (numSfb != pStream->max_sfb) {
        ns_scale_factors[numSfb] = 0;
        ns_scale_factors[numSfb + 1] = 0;
        ns_scale_factors[numSfb + 2] = 0;
        if (numSfb < 36) {
          state->com.scalefac_s[gr][ch][numSfb/3][0] = 0;
          state->com.scalefac_s[gr][ch][numSfb/3][1] = 0;
          state->com.scalefac_s[gr][ch][numSfb/3][2] = 0;
        }
      }
    } else {
      for (sfb = 0; sfb < numSfb; sfb++) {
        Ipp32s shift = state->com.si_sfScale[gr][ch] + 1;
        if (maxXQuant[sfb] != 0) {
          if (maxXQuant[sfb] > 1) {
            ns_scale_factors[sfb] = (Ipp16s)((maxSf - ns_scale_factors[sfb]) >> shift);
          } else {
            ns_scale_factors[sfb] = ((Ipp16s)((maxSf - ns_scale_factors[sfb] + (1 << shift)-1) >> shift));
          }
          if (ns_scale_factors[sfb] > 0) {
            Ipp32f mul = (Ipp32f)pow(ifqstep, ns_scale_factors[sfb]);
            ippsMulC_32f_I(mul, mdct_scaled + sfb_offset[sfb], sfb_width[sfb]);
            state->com.scalefac_l[gr][ch][sfb] = ns_scale_factors[sfb];

            if (state->com.si_preFlag[gr][ch]) {
              state->com.scalefac_l[gr][ch][sfb] = 
                state->com.scalefac_l[gr][ch][sfb] - (Ipp16u)mp3enc_pretab[sfb];
            }
          }
        }
      }
      if (numSfb != pStream->max_sfb) {
        ns_scale_factors[numSfb] = 0;
        if (numSfb < 21) {
          state->com.scalefac_l[gr][ch][numSfb] = 0;
          if (state->com.si_preFlag[gr][ch]) {
            ns_scale_factors[numSfb] = (Ipp16s)mp3enc_pretab[sfb];
          }
        }

        if (ns_scale_factors[numSfb] > 0) {
          Ipp32f mul = (Ipp32f)pow(ifqstep, ns_scale_factors[numSfb]);
          ippsMulC_32f_I(mul, mdct_scaled + sfb_offset[numSfb], sfb_width[numSfb]);
        }
      }
    }

    pBlock->last_frame_common_scalefactor[0] = maxSf;
    pBlock->common_scalefactor_update[0] = 1;
  } else {
    Ipp32s i;
    Ipp32f sqrtMdctLine, tmp, totEnergy, totNoise;

    sqrtMdctLine = 0;
#if 0
    for (i = 0; i < LEN_MDCT_LINE; i++) {
      sqrtMdctLine += (Ipp32f)sqrt(mdct_line_abs[i]);
    }
#else
    for (i = 0; i < LEN_MDCT_LINE; i++) {
       mdct_line_abs_sqrt[i] = sqrtf(mdct_line_abs[i]);
    }
    for (i = 0; i < LEN_MDCT_LINE; i++) {
      sqrtMdctLine += mdct_line_abs_sqrt[i];
    }
#endif

    ippsSum_32f(qData->energy, pStream->max_sfb, &totEnergy, ippAlgHintNone);
    ippsSum_32f(qData->noiseThr, pStream->max_sfb, &totNoise, ippAlgHintNone);
    if (totEnergy > 0) {
      tmp = (Ipp32f)log((27.0/4.0) * totNoise/sqrtMdctLine)/(Ipp32f)log(2.f);
      tmp *= (Ipp32f)8/(Ipp32f)3;
      pBlock->last_frame_common_scalefactor[0] = (Ipp32s)(tmp+0.5);

    } else {
      pBlock->last_frame_common_scalefactor[0] = -SF_OFFSET;
    }

    pBlock->common_scalefactor_update[0] = 1;
  }

  mp3enc_LimitSF(mdct_scaled, pStream->max_line,
                 &pBlock->start_common_scalefac,
                 &pBlock->finish_common_scalefac);

  mp3enc_main_loop(state, pBlock, pStream,
                   mdct_scaled, gr, ch);


  if (pBlock->ns_mode) {
    IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_rqnt, LEN_MDCT_LINE);
    IPP_ALIGNED_ARRAY(32, Ipp16s, x_quant_unsigned, LEN_MDCT_LINE);
    Ipp32f noise[MAX_SECTION_NUMBER];
    Ipp32s numSfb = pStream->max_sfb;
    Ipp32f real_sf = .0f;
    Ipp32f pe = .0f;

    for (sfb = 0; sfb < numSfb; sfb++) {
      Ipp32f sf = (Ipp32f)pow(2.0, -(state->com.si_globGain[gr][ch] - SF_OFFSET) * (3./16.));
      Ipp32f temp = (Ipp32f)(MAGIC_NUMBER - 0.5f)/sf;
      Ipp32s sfb_start = sfb_offset[sfb];
      Ipp32f energy = qData->energy[sfb];

      ippsAddC_32f(mdct_scaled + sfb_start, temp, mdct_rqnt, sfb_width[sfb]);
      ippsMulC_Low_32f16s(mdct_rqnt, sf, (Ipp16s*)x_quant_unsigned, sfb_width[sfb]);
      ippsPow43_16s32f(x_quant_unsigned, mdct_rqnt, sfb_width[sfb]);
      ippsCalcSF_16s32f(&pStream->scale_factors[sfb], SF_OFFSET, &real_sf, 1);
      ippsMulC_32f_I(real_sf, mdct_rqnt, sfb_width[sfb]);
      ippsSub_32f_I(mdct_line_abs + sfb_start, mdct_rqnt, sfb_width[sfb]);
      ippsDotProd_32f(mdct_rqnt, mdct_rqnt, sfb_width[sfb], &noise[sfb]);

      if (energy > noise[sfb]) {
        pe += (Ipp32f)(sfb_width[sfb] * log10(energy / noise[sfb]));
      }
    }
    qData->outPe = pe;
  }
}

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER
