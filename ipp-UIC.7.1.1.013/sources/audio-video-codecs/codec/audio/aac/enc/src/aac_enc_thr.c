/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
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

#include "math.h"
#include "aac_enc_quantization_fp.h"
#include "aac_enc_search.h"

/* Loudness is defined as E^0.23 (see [Zwicker and Feldtkeller, 1967])   */
/* so POWER = 1/0.23. Because of dependence of quality of encoded signal */
/* from POWER is not very high one can change POWER to 4 in order to use */
/* sqrt instead of pow                                                   */
#define POWER 4.35
#define INV_POWER (1.0 / POWER)

/* SfbPE = w * log10(energy(sfb)/thr(sfb))                                       */
/* PE = SUM sfbPE                                                                */
/* NeededPE = BitToPeCoeff * bits_per_frame                                      */
/* Let thr(x) = (thr^(1/n) + x)^n                                                */
/* NeededPE = SUM w*log10(energy(sfb)) - SUM n*w*log10(thr(sfb)^(1/n) + x) =     */
/*                           logEnergy - SUM n*w*log10(thr(sfb)^(1/n) + x),      */
/* where logEnergy = SUM w*log10(energy(sfb))                                    */
/* How to find x?                                                                */
/* Iterations:                                                                   */
/* Approximation - all thresholds are the same (average),                        */
/* NeededPE = SUM w*log10(energy(sfb)) - (SUM n*w) * log10(avThr^(1/n) + x)      */
/* Steps of each iterations:                                                     */
/* 1) Calculate current currPE = logEnergy - SUM (n*w*log10(currThr(sfb)^(1/n))) */
/* 2) Calculate current CurAvThr^(1/n):                                          */
/*    currPE = logEnergy - (SUM n*w) * log10(CurAvThr^(1/n))                     */
/*    CurAvThr^(1/n) = pow(10, (logEnergy - currPE)/(SUM n*w))                   */
/* 3) Calculate x = pow(10, (logEnergy - NeededPE)/(SUM n*w)) - CurAvThr^(1/n)   */
/* 4) Calculate new currThr(sfb)^(1/n) += x                                      */


void aac_UpdateThr(
  sEnc_individual_channel_stream* pStream,
  sQuantizationData*              qData,
  Ipp32f                          NeededPE,
  Ipp32s*                         msMask,
  Ipp32s                          ms_mask_present,
  Ipp32s                          numCh)
{
#if !defined(ANDROID)
  Ipp32f powNoiseThr[2][MAX_SECTION_NUMBER];
  Ipp32f minPowNoiseThr[2][MAX_SECTION_NUMBER];
  Ipp32f minNoiseThr[2][MAX_SECTION_NUMBER];
  Ipp32s isBounded[2][MAX_SECTION_NUMBER];
  Ipp32f *sfbPE[2];
  Ipp32f *energy[2];
  Ipp32f *logEnergy[2];
  Ipp32f *minSNR[2];
  Ipp32f *noiseThr[2];
  Ipp32s *sfb_width[2];
  Ipp32s savedSfb[2];
  Ipp32s numSfb[2];
  Ipp32s max_sfb[2];
#else
  static Ipp32f powNoiseThr[2][MAX_SECTION_NUMBER];
  static Ipp32f minPowNoiseThr[2][MAX_SECTION_NUMBER];
  static Ipp32f minNoiseThr[2][MAX_SECTION_NUMBER];
  static Ipp32s isBounded[2][MAX_SECTION_NUMBER];
  static Ipp32f *sfbPE[2];
  static Ipp32f *energy[2];
  static Ipp32f *logEnergy[2];
  static Ipp32f *minSNR[2];
  static Ipp32f *noiseThr[2];
  static Ipp32s *sfb_width[2];
  static Ipp32s savedSfb[2];
  static Ipp32s numSfb[2];
  static Ipp32s max_sfb[2];
#endif
  Ipp32f PEBoundedTotal, currPE, x;
  Ipp32s i, sfb, numLines, iter;
  Ipp32f sumLogEnergy;
  Ipp32s totalBounded, found;

  for (i = 0; i < numCh; i++) {
    max_sfb[i] = pStream[i].max_sfb;
    numSfb[i] = pStream[i].num_window_groups * pStream[i].max_sfb;
    sfb_width[i] = pStream[i].sfb_width;
    sfbPE[i] = qData[i].sfbPE;
    energy[i] = qData[i].energy;
    logEnergy[i] = qData[i].logEnergy;
    minSNR[i] = qData[i].minSNR;
    noiseThr[i] = qData[i].noiseThr;

    for (sfb = 0; sfb < numSfb[i]; sfb++) {
      isBounded[i][sfb] = 0;
    }

    if (qData[i].predAttackWindow >= 0) {
      for (sfb = 0; sfb < max_sfb[i]; sfb++) {
        isBounded[i][qData[i].predAttackWindow * pStream[i].max_sfb + sfb] = 2;
      }
    }
  }

  if ((numCh == 2) && (ms_mask_present != 0)) {
    for (sfb = 0; sfb < numSfb[0]; sfb++) {
      if (msMask[sfb] == 1) {
        if (energy[0][sfb] > energy[1][sfb] && energy[1][sfb] != 0) {

          minSNR[1][sfb] *= (energy[0][sfb] / energy[1][sfb]);
          if (minSNR[1][sfb] >= 1) {
            isBounded[1][sfb] = 2;
          }
        } else if (energy[1][sfb] > energy[0][sfb] && energy[0][sfb] != 0){
          minSNR[0][sfb] *= (energy[1][sfb] / energy[0][sfb]);
          if (minSNR[0][sfb] >= 1) {
            isBounded[0][sfb] = 2;
          }
        }
      }
    }
  }

  totalBounded = 0;
  PEBoundedTotal = 0;
  currPE = 0;
  sumLogEnergy = 0;
  numLines = 0;

  for (i = 0; i < numCh; i++) {
    for (sfb = 0; sfb < numSfb[i]; sfb++) {
      minNoiseThr[i][sfb] = minSNR[i][sfb] * energy[i][sfb];
      minPowNoiseThr[i][sfb] = (Ipp32f)pow(minSNR[i][sfb] * energy[i][sfb], INV_POWER);
      powNoiseThr[i][sfb] = (Ipp32f)pow(noiseThr[i][sfb], INV_POWER);

      if (isBounded[i][sfb] != 2) {
        if (powNoiseThr[i][sfb] > minPowNoiseThr[i][sfb]) {
          isBounded[i][sfb] = 1;
          totalBounded++;
        }
      }

      /* Step 1 */
      if (sfbPE[i][sfb] > 0) {
        if (isBounded[i][sfb] != 1) {
          sumLogEnergy += sfb_width[i][sfb] * logEnergy[i][sfb];
          currPE += sfbPE[i][sfb];
          numLines += sfb_width[i][sfb];
        } else {
          PEBoundedTotal += sfbPE[i][sfb];
        }
      }
    }
  }

  if (currPE < NeededPE - PEBoundedTotal)
    return;

  iter = 0;
  while ((fabs(NeededPE - currPE - PEBoundedTotal) > 0.02 * NeededPE) && (numLines > 0) && (iter < 3)) {
    /* Step 2, 3 */
    x = (Ipp32f)(pow(10, (sumLogEnergy - (NeededPE - PEBoundedTotal)) / (POWER*numLines)) -
      pow(10, (sumLogEnergy - currPE) / (POWER*numLines)));

    if (x < 0) x = 0;

    /* Step 4, 1 */
    iter++;
    currPE = 0;
    sumLogEnergy = 0;
    numLines = 0;

    for (i = 0; i < numCh; i++) {
      for (sfb = 0; sfb < numSfb[i]; sfb++) {
        Ipp32f tmp;

        if (isBounded[i][sfb] != 1) {
          tmp = powNoiseThr[i][sfb];
          powNoiseThr[i][sfb] += x;

          if (isBounded[i][sfb] != 2) {
            if (powNoiseThr[i][sfb] > minPowNoiseThr[i][sfb]) {
              powNoiseThr[i][sfb] = minPowNoiseThr[i][sfb];
              isBounded[i][sfb] = 1;
              totalBounded++;
            }
          }

          if (powNoiseThr[i][sfb] < 0)
            powNoiseThr[i][sfb] = 0;

          if (powNoiseThr[i][sfb] > 0) {
            tmp = (Ipp32f)(POWER * log10(powNoiseThr[i][sfb]));
          } else {
            tmp = -100000000;
          }

          if (logEnergy[i][sfb] > tmp) {
            sfbPE[i][sfb] = sfb_width[i][sfb] * (logEnergy[i][sfb] - tmp);
            if (isBounded[i][sfb] != 1) {
              sumLogEnergy += sfb_width[i][sfb] * logEnergy[i][sfb];
              currPE += sfbPE[i][sfb];
              numLines += sfb_width[i][sfb];
            } else {
              PEBoundedTotal += sfbPE[i][sfb];
            }
          } else {
            sfbPE[i][sfb] = -1;
          }
        }
      }
    }
  }

  for (i = 0; i < numCh; i++) {
    for (sfb = 0; sfb < numSfb[i]; sfb++) {
      noiseThr[i][sfb] = (Ipp32f)pow(powNoiseThr[i][sfb], POWER);
    }
  }

  currPE += PEBoundedTotal;

  NeededPE *= 1.4f;

  /* If currPE is very big - allow some frequency holes */
  if (currPE > NeededPE) {
    Ipp32f minEnergy, avEnergy;
#if !defined(ANDROID)
    Ipp32f boundEn[4];
#else
    static Ipp32f boundEn[4];
#endif
    Ipp32s totNumSfb, j;

    minEnergy = avEnergy = energy[0][0];
    totNumSfb = 0;

    for (i = 0; i < numCh; i++) {
      for (sfb = 0; sfb < numSfb[i]; sfb++) {
        if (minEnergy > energy[i][sfb])
          minEnergy = energy[i][sfb];

        avEnergy += energy[i][sfb];
      }
      totNumSfb += numSfb[i];
    }

    avEnergy /= totNumSfb;

    if (minEnergy < 1) minEnergy = 1;
    if (avEnergy  < 1) avEnergy = 1;

    /* log scale between minEnergy and avEnergy */
    for (i = 0; i < 4; i++) {
      boundEn[i] = (Ipp32f)(minEnergy * pow(avEnergy/minEnergy, (i+1)/4.0));
    }

    j = 0;

    for (;;) {
      for (i = 0; i < numCh; i++) {
        savedSfb[i] = numSfb[i] - 1;
      }

      found = 1;
      while ((currPE > NeededPE) && (found == 1)) {
        found = 0;
        for (i = 0; i < numCh; i++) {
          sfb = savedSfb[i];
          if (sfb == -max_sfb[i]) continue;
          while ((energy[i][sfb] > boundEn[j]) || (sfbPE[i][sfb] < 0)) {
            sfb -= max_sfb[i];
            if (sfb == -max_sfb[i]) break;
            if (sfb < 0) sfb += numSfb[i] - 1;
          }
          savedSfb[i] = sfb;
          if (sfb == -max_sfb[i]) continue;
          /* should be more than energy. Coeff doesn't matter */
          /* Here will be hole                                */
          noiseThr[i][sfb] = 10*energy[i][sfb];
          currPE -= sfbPE[i][sfb];
          sfbPE[i][sfb] = -1;
          found = 1;
        }
      }

      if (currPE <= NeededPE) break;
      j++;
      if (j >= 4) break;
    }
  }
}

/****************************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

