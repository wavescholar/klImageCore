/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include<math.h>
#include <stdio.h>
#include "sbr_settings.h"
#include "sbr_dec_struct.h"
#include "sbr_dec_tabs_fp.h"
#include "aac_dec_sbr_fp.h"

/********************************************************************/

static const Ipp32s POW_MINUS_UNIT[] =
  { 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1,
  1, -1, 1, -1, 1, -1, 1, -1, 1, -1,
  1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1,
  1, -1, 1, -1, 1, -1, 1, -1, 1, -1
};

/********************************************************************/

static const Ipp32f EPS0 = 1e-12f;

/********************************************************************/

static const Ipp32s SIN_FI_RE[4] = { 1, 0, -1, 0 };

/********************************************************************/

static const Ipp32s SIN_FI_IM[4] = { 0, 1, 0, -1 };

/********************************************************************/

static const Ipp32f TABLE_LIMIT_GAIN[4] = { 0.70795f, 1.f, 1.41254f, 1e10f };

/********************************************************************/

static const Ipp32f hSmoothFilter[] = { 0.03183050093751f,
  0.11516383427084f,
  0.21816949906249f,
  0.30150283239582f,
  0.33333333333333f
};

/********************************************************************/

static void sbrAliasReduction(Ipp32f *degPatched, Ipp32f *bufECurr,
                              Ipp32f *bufG, Ipp32s *s_mapped, Ipp32s kx,
                              Ipp32s M)
{
  Ipp32s  f_group[64];
  Ipp32f  denum, energ_total, new_gain, energ_total_new, alpha;
  Ipp32s  grouping, i, n_group, k, m, i_start, i_end;

/* --------------------------------  calculation gain groups ---------------------------- */
  i = 0;
  grouping = 0;
  for (k = kx; k < kx + M - 1; k++) {
    if (degPatched[k + 1] && s_mapped[k - kx] == 0) {
      if (grouping == 0) {
        f_group[i] = k;
        grouping = 1;
        i++;
      }
    } else {
      if (grouping == 1) {
        if (s_mapped[k - kx] == 0)
          f_group[i] = k + 1;
        else
          f_group[i] = k;

        grouping = 0;
        i++;
      }
    }
  }

  if (grouping == 1) {
    f_group[i] = M + kx;
    i++;
  }
  n_group = i >> 1;

/* --------------------------------  aliasing reduction ---------------------------- */
  for (k = 0; k < n_group; k++) {

    i_start = f_group[2 * k] - kx;
    i_end = f_group[2 * k + 1] - kx;

    denum = EPS0;
    energ_total = 0.0f;

    for (i = i_start; i < i_end; i++) {
      energ_total += bufG[i] * bufG[i] * bufECurr[i];
      denum += bufECurr[i];
    }

    new_gain = energ_total / (denum);
    energ_total_new = EPS0;

    for (m = i_start + kx; m < i_end + kx; m++) {
      if (m < M + kx - 1) {
        alpha = IPP_MAX(degPatched[m], degPatched[m + 1]);
      } else {
        alpha = degPatched[m];
      }
      bufG[m - kx] =
        alpha * new_gain + (1.0f - alpha) * bufG[m - kx] * bufG[m - kx];
    }

    for (i = i_start; i < i_end; i++)
      energ_total_new += bufG[i] * bufECurr[i];

    energ_total_new = energ_total / energ_total_new;
    for (i = i_start; i < i_end; i++) {
      bufG[i] = (Ipp32f)(sqrt(energ_total_new * bufG[i]));

    }
  }

  return;
}

/********************************************************************/

static Ipp32s sbrUpdate_lA(Ipp32s bs_pointer, Ipp32s bs_frame_class, Ipp32s L_E)
{

  Ipp32s  table_lA[3][3] = {
    {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}
  };

  Ipp32s  indx1, indx2;
  Ipp32s  lA;

  table_lA[1][1] = table_lA[2][1] = L_E + 1 - bs_pointer;
  table_lA[2][2] = bs_pointer - 1;

  if ((bs_pointer > 1) || (bs_pointer < 0))
    indx1 = 2;
  else
    indx1 = bs_pointer;

  if (bs_frame_class == VARVAR)
    indx2 = 1;
  else
    indx2 = bs_frame_class;

  lA = table_lA[indx1][indx2];

  return lA;
}

/********************************************************************/

static Ipp32s sbrSinAdd_LP_32f(Ipp32f **YRe, Ipp32s indxSine, Ipp32s i, Ipp32s tE0,
                               Ipp32s m, Ipp32s kx, Ipp32f *bufSM, Ipp32s M, Ipp32s *numSin)
{
  Ipp32s  fIndexSineMinus1 = 0;
  Ipp32s  fIndexSinePlus1 = 0;
  Ipp32s  num_sinusoids = *numSin;
  Ipp32f  ksi_middle = 0.f;

  fIndexSineMinus1 = (indxSine + (i - 1) - RATE * tE0) & 3;
  fIndexSinePlus1 = (indxSine + (i + 1) - RATE * tE0) & 3;

  if (m == 0) {
    YRe[i][m + kx - 1] =
      -0.00815f * POW_MINUS_UNIT[kx -
                                 1] * bufSM[0] * SIN_FI_RE[fIndexSinePlus1];

    if (m < M - 1) {
      YRe[i][m + kx] +=
        -0.00815f * POW_MINUS_UNIT[kx] * bufSM[1] * SIN_FI_RE[fIndexSinePlus1];
    }
  } else if ((0 < m) && (m < M - 1) && (num_sinusoids < 16)) {
    ksi_middle =
      (bufSM[m - 1] * SIN_FI_RE[fIndexSineMinus1] +
       bufSM[m + 1] * SIN_FI_RE[fIndexSinePlus1]);
    ksi_middle *= -0.00815f * POW_MINUS_UNIT[m + kx];

    YRe[i][m + kx] += ksi_middle;

  } else if ((m == M - 1) && (num_sinusoids < 16)) {
    if (m > 0) {
      YRe[i][m + kx] +=
        -0.00815f * POW_MINUS_UNIT[m + kx] * bufSM[m -
                                                   1] *
        SIN_FI_RE[fIndexSineMinus1];
    }
    if (M + kx < 64) {
      YRe[i][m + kx + 1] =
        -0.00815f * POW_MINUS_UNIT[m + kx +
                                   1] * bufSM[m] * SIN_FI_RE[fIndexSineMinus1];
    }
  }

  if (bufSM[m])
    num_sinusoids++;

  *numSin = num_sinusoids;

  return 0;     // OK
}

/********************************************************************/

void sbrAdjustmentHF(Ipp32f **YBuf,
                     Ipp32f *bufEnvOrig, Ipp32f *bufNoiseOrig,
                     Ipp32f BufGain[][MAX_NUM_ENV_VAL],
                     Ipp32f BufNoise[][MAX_NUM_ENV_VAL],
                     sSBRDecComState * comState, Ipp32f *degPatched,
                     Ipp8u *WorkBuffer, Ipp32s reset, Ipp32s ch,
                     Ipp32s decode_mode)
{
/*
 * VALUES
 */
  Ipp32s  k, m, n, pos, kl, kh, l, p, i, j, delta_step, delta;
  Ipp32f  energ, sumEOrig, sumECurr, g_max_temp, gainMax, denum, boost_gain,
    gainFilt, noiseFilt;
  Ipp32s  iStart, iEnd;
  Ipp32s  nResolution;

  Ipp32f  mulQ = 0.0f;
  Ipp32f  mulQQ = 0.0f;
  Ipp32f *bufSM;
  Ipp32f *bufQM;
  Ipp32f *bufECurr;
  Ipp32f *bufEOrig;
  Ipp32f *bufG;
  Ipp32f  bufQMapped[64];
  Ipp32f  sumEWoInterpol = 0.0f;

  Ipp32f  LimiterGains = TABLE_LIMIT_GAIN[comState->sbrHeader.bs_limiter_gains];
  Ipp32s  interpolation = comState->sbrHeader.bs_interpol_freq;
  Ipp32s  fIndexNoise = 0;
  Ipp32s  fIndexSine = 0;

  Ipp32s  num_sinusoids = 0;
  Ipp32s  s_mapped[64];

  Ipp32s  hSmoothLen = (comState->sbrHeader.bs_smoothing_mode == 0) ? 4 : 0;
  Ipp32s  hSmoothLenConst = hSmoothLen;

  Ipp32s  kx = comState->kx;
  Ipp32s  M = comState->M;
  Ipp32s  nEnv = comState->sbrFIState[ch].nEnv;
  Ipp32s  nNoiseEnv = comState->sbrFreqTabsState.nNoiseBand;//   N_Q;
  Ipp32s  lA = 0;

  Ipp32s  offset_32f = 64 * sizeof(Ipp32f);
  Ipp32s *sineIndxMap;
  Ipp32s *sineIndxMapPrev;
  Ipp32s *pFreqTbl;
  Ipp32s *F[2];
  Ipp32s  resolution[2];

  //Ipp32f **YRe = YBufRe + SBR_TIME_HFADJ;
  //Ipp32f **YIm = YBufIm + SBR_TIME_HFADJ;
  Ipp32fc** pYcmp = (Ipp32fc**)YBuf + SBR_TIME_HFADJ;
  Ipp32f** pYre =  YBuf + SBR_TIME_HFADJ;

  sSBRFeqTabsState*   pFTState = &( comState->sbrFreqTabsState );
  sSBRFrameInfoState* pFIState = &( comState->sbrFIState[ch] );
  sSBREnvDataState*   pEDState = &(comState->sbrEDState[ch] );

  //Ipp32f yRe, yIm;

/*
 * CODES
 */
  F[0] = pFTState->fLoBandTab;// f_TableLow;
  F[1] = pFTState->fHiBandTab;//  f_TableHigh;

  resolution[0] = pFTState->nLoBand;// N_low;
  resolution[1] = pFTState->nHiBand;// N_high;
/* set memory */
  bufSM = (Ipp32f *)(WorkBuffer + 0 * offset_32f);
  bufQM = (Ipp32f *)(WorkBuffer + 1 * offset_32f);
  bufECurr = (Ipp32f *)(WorkBuffer + 2 * offset_32f);
  bufEOrig = (Ipp32f *)(WorkBuffer + 3 * offset_32f);
  bufG = (Ipp32f *)(WorkBuffer + 4 * offset_32f);
  sineIndxMap = (Ipp32s *)(WorkBuffer + 5 * offset_32f);
  sineIndxMapPrev = comState->S_index_mapped_prev[ch];

  if (reset) {
    comState->FlagUpdate[ch] = 1;
    comState->indexNoise[ch] = 0;
  }

  ippsZero_8u((Ipp8u *)sineIndxMap, 64 * sizeof(Ipp32s));
  for (i = 0; i < pFTState->nHiBand; i++) {
    m = (pFTState->fHiBandTab[i + 1] + pFTState->fHiBandTab[i]) >> 1;
    sineIndxMap[m - kx] = pEDState->bs_add_harmonic[i];
  }

  lA =
    sbrUpdate_lA(comState->bs_pointer[ch], comState->bs_frame_class[ch],
                 pFIState->nEnv);

/* main loop */
  for (l = 0; l < nEnv; l++) {
    for (k = 0; k < pFIState->nNoiseEnv; k++) {
      if (pFIState->bordersEnv[l] >= pFIState->bordersNoise[k] &&
          pFIState->bordersEnv[l + 1] <= pFIState->bordersNoise[k + 1])
        break;
    }

    for (i = 0; i < nNoiseEnv; i++) {
      for (m = pFTState->fNoiseBandTab[i]; m < pFTState->fNoiseBandTab[i + 1]; m++) {
        bufQMapped[m - kx] = bufNoiseOrig[pEDState->vSizeNoise[k] + i];
      }
    }

    delta = (l == lA || l == comState->lA_prev[ch]) ? 1 : 0;

    if (decode_mode == HEAAC_HQ_MODE)
      hSmoothLen = (delta ? 0 : hSmoothLenConst);
    else
      hSmoothLen = 0;

/* --------------------------------  Estimation envelope ---------------------------- */
    pos = 0;
    nResolution = resolution[pFIState->freqRes[l]];
    pFreqTbl = F[pFIState->freqRes[l]];
    iStart = RATE * pFIState->bordersEnv[l];
    iEnd = RATE * pFIState->bordersEnv[1 + l];

    for (p = 0; p < nResolution; p++) {
      kl = pFreqTbl[p];
      kh = pFreqTbl[p + 1];
      delta_step = 0;
      sumEWoInterpol = 0.0f;
      for (j = kl; j < kh; j++) {
        energ = 0.0f;
        for (i = iStart; i < iEnd; i++) {

          /*
          energ += YRe[i][j] * YRe[i][j];

          if (decode_mode == HEAAC_HQ_MODE) {
            energ += YIm[i][j] * YIm[i][j];
          }
          */
          ////////
          if(decode_mode == HEAAC_LP_MODE) {
            energ += pYre[i][j] * pYre[i][j];

          } else { //if (decode_mode == HEAAC_HQ_MODE) {
            energ += pYcmp[i][j].re * pYcmp[i][j].re + pYcmp[i][j].im * pYcmp[i][j].im;
          }
          ////////

        }
        delta_step = (sineIndxMap[pos] &&
                      (l >= lA || sineIndxMapPrev[pos + kx])) ? 1 : delta_step;

        bufECurr[pos] = energ;
        if( 0 != iEnd - iStart ){
          bufECurr[pos] /= (iEnd - iStart);
        }

        if (!interpolation){
          sumEWoInterpol += bufECurr[pos] / (kh - kl);
        }

        pos++;
      }

      for (k = pos - (kh - kl); k < pos; k++) {
        bufSM[k] = 0;
        bufEOrig[k] = bufEnvOrig[pEDState->vSizeEnv[l] + p];

        if (!interpolation)
          bufECurr[k] = sumEWoInterpol;

        mulQ = 1.0f / (1.0f + bufQMapped[k]);
        mulQQ = bufQMapped[k] * mulQ;

        if (decode_mode == HEAAC_LP_MODE) {
          bufECurr[k] *= 2.0f;

          if (delta_step)
            s_mapped[k - pos + kh - kx] = 1;
          else
            s_mapped[k - pos + kh - kx] = 0;
        }

        if ((delta_step) &&
            (sineIndxMap[k] && (l >= lA || sineIndxMapPrev[k + kx]))) {
          bufSM[k] = bufEOrig[k] * mulQ;
        }

        bufG[k] = bufEOrig[k];

        if (delta_step) {
          bufG[k] = bufG[k] * mulQQ;

        } else if (!delta) {
          bufG[k] = bufG[k] * mulQ;
        }

        bufG[k] /= (bufECurr[k] + 1);

        bufQM[k] = bufEOrig[k] * mulQQ;

      } // end for(k=
    }   // end for(p=

/* --------------------------------  Calculation of gain ---------------------------- */
    for (k = 0; k < pFTState->nLimBand; k++) {
      sumEOrig = sumECurr = EPS0;
      for (i = pFTState->fLimBandTab[k] - kx; i < pFTState->fLimBandTab[k + 1] - kx;
           i++) {
        sumEOrig += bufEOrig[i];
        sumECurr += bufECurr[i];
      }

      g_max_temp = sumEOrig / sumECurr * (LimiterGains * LimiterGains);
      gainMax = IPP_MIN(1.0e5f * 1.0e5f, g_max_temp);

      denum = EPS0;
      for (i = pFTState->fLimBandTab[k] - kx; i < pFTState->fLimBandTab[k + 1] - kx;
           i++) {
        if (gainMax <= bufG[i]) {
          bufQM[i] = bufQM[i] * (gainMax / bufG[i]);
          bufG[i] = gainMax;
        }

        denum += bufG[i] * bufECurr[i] + bufSM[i];
        if (!(bufSM[i] != 0.0f || delta))
          denum += bufQM[i];
      }

      boost_gain = sumEOrig / denum;
      boost_gain = IPP_MIN(boost_gain, 1.584893192f * 1.584893192f);

      for (i = pFTState->fLimBandTab[k] - kx; i < pFTState->fLimBandTab[k + 1] - kx;
           i++) {
        bufG[i] = (Ipp32f)sqrt(bufG[i] * boost_gain);
        bufQM[i] = (Ipp32f)sqrt(bufQM[i] * boost_gain);
        bufSM[i] = (Ipp32f)sqrt(bufSM[i] * boost_gain);
      }
    }

    if (decode_mode == HEAAC_LP_MODE) {
      sbrAliasReduction(degPatched, bufECurr, bufG, s_mapped, kx, M);
    }

/* --------------------------------  Assembling ---------------------------- */
    if (comState->FlagUpdate[ch]) {
      for (n = 0; n < 4; n++) {
        ippsCopy_32f(bufG, BufGain[n], M);
        ippsCopy_32f(bufQM, BufNoise[n], M);
      }
      comState->FlagUpdate[ch] = 0;
    }

    for (i = iStart; i < iEnd; i++) {
      for (m = 0; m < M; m++) {
        BufGain[4][m] = bufG[m];
        BufNoise[4][m] = bufQM[m];

        if (decode_mode == HEAAC_LP_MODE) {
          num_sinusoids = 0;
        }

        gainFilt = noiseFilt = 0.0f;

        if (hSmoothLen) {
          for (n = 0; n <= 4; n++) {
            gainFilt += BufGain[n][m] * hSmoothFilter[n];
            noiseFilt += BufNoise[n][m] * hSmoothFilter[n];
          }
        } else {
          gainFilt = BufGain[4][m];
          noiseFilt = BufNoise[4][m];
        }

        if (bufSM[m] != 0 || delta)
          noiseFilt = 0;

        fIndexNoise =
          (comState->indexNoise[ch] + (i - RATE * pFIState->bordersEnv[0]) * M + m +
           1) & 511;
        fIndexSine =
          (comState->indexSine[ch] + i - RATE * pFIState->bordersEnv[0]) & 3;

        if(decode_mode == HEAAC_HQ_MODE) {

          pYcmp[i][m + kx].re = pYcmp[i][m + kx].re * gainFilt +
                                noiseFilt * SBR_TABLE_V[0][fIndexNoise] +
                                bufSM[m] * SIN_FI_RE[fIndexSine];

          pYcmp[i][m + kx].im = pYcmp[i][m + kx].im * gainFilt +
                                noiseFilt * SBR_TABLE_V[1][fIndexNoise] +
                                bufSM[m] * POW_MINUS_UNIT[m + kx] * SIN_FI_IM[fIndexSine];

        } else {//if (decode_mode == HEAAC_LP_MODE) {

          pYre[i][m + kx] = pYre[i][m + kx] * gainFilt +
                                noiseFilt * SBR_TABLE_V[0][fIndexNoise] +
                                bufSM[m] * SIN_FI_RE[fIndexSine];

          sbrSinAdd_LP_32f(pYre, comState->indexSine[ch], i, pFIState->bordersEnv[0],
                           m, kx, bufSM, M, &num_sinusoids);
        }

      } // end for ( m = 0; m < M; m++ )

      for (n = 0; n < 4; n++) {
        ippsCopy_32f(BufGain[n + 1], BufGain[n], 64);
        ippsCopy_32f(BufNoise[n + 1], BufNoise[n], 64);
      }
    }   // end for (i = iStart; i < iEnd; i++)
//
  }     /* end main loop */

/* --------------------------------  update ---------------------------- */
  ippsCopy_8u((const Ipp8u *)sineIndxMap, (Ipp8u *)&(sineIndxMapPrev[kx]),
              (64 - kx) * sizeof(Ipp32s));

  if (lA == nEnv)
    comState->lA_prev[ch] = 0;
  else
    comState->lA_prev[ch] = -1;

  pFIState->nEnvPrev = pFIState->nEnv;
  //comState->L_Q_prev[ch] = comState->L_Q[ch];
  pFIState->nNoiseEnvPrev = pFIState->nNoiseEnv;

  comState->indexNoise[ch] = fIndexNoise;
  comState->indexSine[ch] = (fIndexSine + 1) & 3;
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

