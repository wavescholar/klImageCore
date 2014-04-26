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


void mp3encUpdateThr(mpaICS             *pStream,
                     sQuantizationData  *qData,
                     Ipp32f             NeededPE,
                     Ipp32s             *msMask,
                     Ipp32s             ms_mask_present,
                     Ipp32s             startCh,
                     Ipp32s             numCh)
{
  Ipp32f  powNoiseThr[2][MAX_SECTION_NUMBER];
  Ipp32f  minPowNoiseThr[2][MAX_SECTION_NUMBER];
  Ipp32f *sfbPE[2];
  Ipp32f *energy[2];
  Ipp32f *logEnergy[2];
  Ipp32f *minSNR[2];
  Ipp32f *noiseThr[2];
  Ipp32s *sfb_width[2];
  Ipp32s *isBounded[2];
  Ipp32s  savedSfb[2];
  Ipp32s  numSfb[2];
  Ipp32f  PEBoundedTotal, currPE, x;
  Ipp32s  max_sfb[2];
  Ipp32s  i, sfb, numLines, iter;
  Ipp32f  sumLogEnergy;
  Ipp32s  totalBounded, found;
  Ipp32f  MinSfbPE[2][40], minPE;

  qData->allowHoles = 0;

  for (i = startCh; i < numCh+startCh; i++) {
    max_sfb[i] = pStream[i].max_sfb;
    numSfb[i] = max_sfb[i];
    sfb_width[i] = pStream[i].sfb_width;
    sfbPE[i] = qData[i].sfbPE;
    energy[i] = qData[i].energy;
    logEnergy[i] = qData[i].logEnergy;
    minSNR[i] = qData[i].minSNR;
    noiseThr[i] = qData[i].noiseThr;
    isBounded[i] = qData[i].isBounded;

    for (sfb = 0; sfb < numSfb[i]; sfb++) {
      isBounded[i][sfb] = 0;
    }

    if (qData[i].predAttackWindow >= 0) {
      for (sfb = 0; sfb < max_sfb[i] / 3; sfb++) {
        isBounded[i][sfb * 3 + qData[i].predAttackWindow] = 2;
      }
    }
  }

  if ((numCh == 2) && (ms_mask_present != 0)) {
    for (sfb = 0; sfb < numSfb[0]; sfb++) {
      if (msMask[sfb] == MPA_MS_STEREO) {
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

  if (pStream[startCh].windows_sequence) {
    minPE = 0;
    for (i = startCh; i < numCh+startCh; i++) {
      for (sfb = 0; sfb < numSfb[i]; sfb++) {
        if (minSNR[i][sfb] < 1) {
          MinSfbPE[i][sfb] = (Ipp32f)(-log10(minSNR[i][sfb])) * sfb_width[i][sfb];
        } else {
          MinSfbPE[i][sfb] = 0;
        }
        minPE += MinSfbPE[i][sfb];
      }
    }

    sfb = numSfb[startCh] - 1;

    while ((minPE > 0.5 * NeededPE) && (sfb >= 0)) {
      for (i = startCh; i < numCh+startCh; i++) {
        minPE -= MinSfbPE[i][sfb];
        isBounded[i][sfb] = 2;
      }
      sfb--;
    }
  }

  for (i = startCh; i < numCh+startCh; i++) {
    for (sfb = 0; sfb < numSfb[i]; sfb++) {
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

    for (i = startCh; i < numCh+startCh; i++) {
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

  for (i = startCh; i < numCh+startCh; i++) {
    for (sfb = 0; sfb < numSfb[i]; sfb++) {
      noiseThr[i][sfb] = (Ipp32f)pow(powNoiseThr[i][sfb], POWER);
    }
  }

  currPE += PEBoundedTotal;

  NeededPE *= 1.1f;

  /* If currPE is very big - allow some frequency holes */
  if (currPE > NeededPE) {
    Ipp32f minEnergy, avEnergy;
    Ipp32f boundEn[4];
    Ipp32s totNumSfb, j;

    qData->allowHoles = 1;

    minEnergy = avEnergy = energy[startCh][0];
    totNumSfb = 0;

    for (i = startCh; i < numCh+startCh; i++) {
      for (sfb = 0; sfb < numSfb[i]; sfb++) {
        if (minEnergy > energy[i][sfb])
          minEnergy = energy[i][sfb];

        avEnergy += energy[i][sfb];
      }
      totNumSfb += numSfb[i];
    }

    avEnergy /= totNumSfb;

    if (minEnergy < 1.0e-15f) minEnergy = 1.0e-15f;
    if (avEnergy  < 1.0e-15f) avEnergy = 1.0e-15f;

    /* log scale between minEnergy and avEnergy */
    for (i = 0; i < 4; i++) {
      boundEn[i] = (Ipp32f)(minEnergy * pow(avEnergy/minEnergy, (i+1)/4.0));
    }

    j = 0;

    for (;;) {
      for (i = startCh; i < numCh+startCh; i++) {
        savedSfb[i] = numSfb[i] - 1;
      }

      found = 1;
      while ((currPE > NeededPE) && (found == 1)) {
        found = 0;
        for (i = startCh; i < numCh+startCh; i++) {
          sfb = savedSfb[i];
          if (sfb < 0) continue;
          while ((energy[i][sfb] > boundEn[j]) || (sfbPE[i][sfb] < 0)) {
            sfb -= 1;
            if (sfb < 0) break;
          }
          savedSfb[i] = sfb;
          if (sfb < 0) continue;
          /* should be more than energy. Coeff doesn't matter */
          /* Here will be hole                                */
          noiseThr[i][sfb] = 10*energy[i][sfb];
          isBounded[i][sfb] = 2;
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

#endif //UMC_ENABLE_MP3_AUDIO_ENCODER

