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
#include "align.h"
#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_dec_struct.h"

#include "ipps.h"
#include "ippac.h"

/********************************************************************/

static Ipp32f SBR_TABLE_NEW_BW[4][4] = {
  {0.f, 0.6f, 0.9f, 0.98f},
  {0.6f, 0.75f, 0.9f, 0.98f},
  {0.f, 0.75f, 0.9f, 0.98f},
  {0.f, 0.75f, 0.9f, 0.98f}
};

/********************************************************************/

static  IppStatus ownPredictCoef_SBR_R_32f_D2L(Ipp32f **pSrc,
                                               Ipp32f *pAlpha0, Ipp32f *pAlpha1,
                                               Ipp32f *pReflectCoef, Ipp32s k0,
                                               Ipp32s len);

/********************************************************************/

static Ipp32s sbrCalcAliasDegree(Ipp32f *ref, Ipp32f *deg, Ipp32s k0)
{
  Ipp32s  sign = 0;
  Ipp32s  k;

  ippsZero_32f(deg, k0);
  ref[0] = 0.0f;
  deg[1] = 0.0f;

  for (k = 2; k < k0; k++) {
    if ((k % 2 == 0) && (ref[k] < 0.0f)) {
      sign = 1;
    } else if ((k % 2 == 1) && (ref[k] > 0.0f)) {
      sign = -1;
    } else {
      sign = 0;
      continue;
    }

    if (sign * ref[k - 1] < 0) {
      deg[k] = 1.0f;
      if (sign * ref[k - 2] > 0.0f) {
        deg[k - 1] = 1 - ref[k - 1] * ref[k - 1];
      }
    } else {
      if (sign * ref[k - 2] > 0.0f) {
        deg[k] = 1 - ref[k - 1] * ref[k - 1];
      }
    }
  }

  return 0;     // OK
}

/********************************************************************/

static Ipp32s sbrCalcChirpFactors(Ipp32s N_Q, Ipp32s *bs_invf_mode_prev,
                                  Ipp32s *bs_invf_mode, Ipp32f *bwArray)
{
  Ipp32s  i;
  Ipp32f  tmpBw, newBw;

  for (i = 0; i < N_Q; i++) {
    newBw = SBR_TABLE_NEW_BW[bs_invf_mode_prev[i]][bs_invf_mode[i]];

    if (newBw < bwArray[i])
      tmpBw = 0.75f * newBw + 0.25f * bwArray[i];
    else
      tmpBw = 0.90625f * newBw + 0.09375f * bwArray[i];

    if (tmpBw < 0.015625f)
      bwArray[i] = 0.f;
    else
      bwArray[i] = tmpBw;

    bs_invf_mode_prev[i] = bs_invf_mode[i];
  }

  return 0;
}

/********************************************************************/

static Ipp32s sbrHFGenerator(
                      /*
                       * in data
                       */
                              Ipp32f **XBuf,
                              Ipp32f *vbwArray,
                              Ipp32f *alpha_0,
                              Ipp32f *alpha_1,
                              sSBRDecComState* pSbr, Ipp32s ch,
                              Ipp32f *deg, Ipp32f *degPatched,
                      /*
                       * out data
                       */
                              Ipp32f **YBuf, Ipp32s mode)
{
  Ipp32s  i, x, q, k_0, k, p, g, l;

  Ipp32s  l_start = RATE * pSbr->sbrFIState[ch].bordersEnv[0];
  Ipp32s  l_end = RATE * pSbr->sbrFIState[ch].bordersEnv[pSbr->sbrFIState[ch].nEnv];

  Ipp32f  accYRe, accYIm;
  Ipp32f  bwArr, bwArr2;
  Ipp32f  cA0Re, cA0Im, cA1Re, cA1Im;

  Ipp32fc** pXcmp = (Ipp32fc**)XBuf;
  Ipp32fc** pYcmp = (Ipp32fc**)YBuf;

  Ipp32f** pXre = XBuf;
  Ipp32f** pYre = YBuf;

  sSBRFeqTabsState* pFTState = &(pSbr->sbrFreqTabsState);

  //------------------------

  Ipp32fc* pA0cmp = (Ipp32fc*)alpha_0 ;
  Ipp32fc* pA1cmp = (Ipp32fc*)alpha_1 ;

  Ipp32f* pA0re = alpha_0 ;
  Ipp32f* pA1re = alpha_1 ;

  /* CODE */
  if (mode == HEAAC_LP_MODE) {
    ippsZero_32f(degPatched, MAX_NUM_ENV_VAL);
    ippsCopy_32f(deg, degPatched, MAX_NUM_ENV_VAL);
  }

   for (i = 0; i < pFTState->numPatches; i++) {
    k_0 = 0;
    for (q = 0; q < i; q++) {
      k_0 += pFTState->patchNumSubbandsTab[q];
    }

    k_0 += pSbr->kx;
    for (x = 0; x < pFTState->patchNumSubbandsTab[i]; x++) {
      k = k_0 + x;
      p = pFTState->patchStartSubbandTab[i] + x;

      for (g = 0; g < pFTState->nNoiseBand; g++) {
        if ((k >= pFTState->fNoiseBandTab[g]) && (k < pFTState->fNoiseBandTab[g + 1]))
          break;
      }

      if (mode == HEAAC_LP_MODE) {
        if (x == 0)
          degPatched[k] = 0.0f;
        else
          degPatched[k] = deg[p];
      }

      bwArr    = vbwArray[g];
      /******************************************
       * code may be optimized because:
       * if ( 0 == bwArr )
       *   pY[ l ][ k ] = pX[ l ][ p ] ONLY!!!
       * else
       *  ippsPredictCoef_SBR_C_32f_D2L(...)
       *  and code is written below,
       *  but it is impossible because
       *  ipp function works only k = [0, k0)
       *
       ******************************************/
      bwArr2   = bwArr * bwArr;

      if (mode == HEAAC_HQ_MODE) {
        cA0Re = bwArr * pA0cmp[p].re ;
        cA0Im = bwArr * pA0cmp[p].im ;

        cA1Re = bwArr2 * pA1cmp[p].re;
        cA1Im = bwArr2 * pA1cmp[p].im;

        for (l = l_start; l < l_end; l++) {

            /*  bw * Alpha0 * XLow[l-1] */
            accYRe = pXcmp[l - 1 + 0][p].re * cA0Re  -
                     pXcmp[l - 1 + 0][p].im * cA0Im ;

            accYIm = pXcmp[l - 1 + 0][p].re * cA0Im  +
                     pXcmp[l - 1 + 0][p].im * cA0Re;

            /*  bw^2 * Alpha1 * XLow[l-2] */
            accYRe += pXcmp[l - 2 + 0][p].re * cA1Re -
                      pXcmp[l - 2 + 0][p].im * cA1Im;

            accYIm += pXcmp[l - 2 + 0][p].re * cA1Im +
                      pXcmp[l - 2 + 0][p].im * cA1Re;

            pYcmp[l + 0][k].re = pXcmp[l - 0 + 0][p].re + accYRe;
            pYcmp[l + 0][k].im = pXcmp[l - 0 + 0][p].im + accYIm;
        }// end for
      } else {// if (mode == HEAAC_HQ_MODE)

        //------------

        cA0Re = bwArr * pA0re[p];

        cA1Re = bwArr2 * pA1re[p];

        for (l = l_start; l < l_end; l++) {

            /*  bw * Alpha0 * XLow[l-1] */
            accYRe = pXre[l - 1 + 0][p] * cA0Re;

            /*  bw^2 * Alpha1 * XLow[l-2] */
            accYRe += pXre[l - 2 + 0][p] * cA1Re;

            pYre[l + 0][k] = pXre[l - 0 + 0][p] + accYRe;
        }
        //------------

      }

    }
  }

  if (mode == HEAAC_LP_MODE) {
    k_0 = 0;
    for (q = 0; q < pFTState->numPatches; q++)
      k_0 += pFTState->patchNumSubbandsTab[q];

    for (k = pSbr->kx + k_0; k < MAX_NUM_ENV_VAL; k++)
      degPatched[k] = 0;
  }

  return 0;     // OK
}

/********************************************************************/

Ipp32s sbrGenerationHF(Ipp32f **XBuf, Ipp32f **YBuf,
                       sSBRDecComState* comState, Ipp32f* bwArray, Ipp32f* degPatched,
                       Ipp32s ch, Ipp32s decode_mode)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, alpha_0, 128);
  IPP_ALIGNED_ARRAY(32, Ipp32f, alpha_1, 128);
  IPP_ALIGNED_ARRAY(32, Ipp32f, ref_coef, 64);
  IPP_ALIGNED_ARRAY(32, Ipp32f, alias_degree, 64);

  sbrCalcChirpFactors(comState->sbrFreqTabsState.nNoiseBand, &(comState->bs_invf_mode_prev[ch][0]),
    &(comState->bs_invf_mode[ch][0]), bwArray);

  if (decode_mode == HEAAC_LP_MODE) {
    ownPredictCoef_SBR_R_32f_D2L(XBuf, alpha_0, alpha_1,
                                 ref_coef, comState->k0, NUM_TIME_SLOTS * RATE + 6);

   sbrCalcAliasDegree(ref_coef, alias_degree, comState->k0);
  } else {
    ippsPredictCoef_SBR_C_32fc_D2L( (const Ipp32fc**)XBuf, (Ipp32fc*)alpha_0,
                                    (Ipp32fc*)alpha_1,
                                    comState->k0, NUM_TIME_SLOTS * RATE + 6);
  }

  sbrHFGenerator(XBuf+SBR_TIME_HFADJ,
                 bwArray,
                 alpha_0, alpha_1,
                 comState, ch,
                 alias_degree,
                 degPatched,
                 YBuf+SBR_TIME_HFADJ,
                 decode_mode);

  return 0;     // OK
}

/********************************************************************/

IppStatus ownPredictCoef_SBR_R_32f_D2L(Ipp32f **pSrc, Ipp32f *pAlpha0,
                                       Ipp32f *pAlpha1, Ipp32f *pReflectCoef,
                                       Ipp32s k0, Ipp32s len) {
  Ipp32f  fi[3][3];
  const Ipp32s TIME_HF_ADJ = 2;
  Ipp32s  i, j, k, n;
  const Ipp32f rel = 1.f / (1.f + 1e-6f);
  Ipp32f  d;
  Ipp32f** ppX = pSrc + TIME_HF_ADJ;

  for (k = 0; k < k0; k++) {
    fi[0][0] = fi[0][1] = fi[0][2] = fi[1][0] = fi[1][1] = fi[1][2] = fi[2][0] =
      fi[2][1] = fi[2][2] = 0.f;
/*
 * auto correlation
 */
    for (n = 0; n < len; n++) {
      i = 0;
      j = 1;
      fi[0][1] += ppX[n - i][k] * ppX[n - j][k];

      i = 0;
      j = 2;
      fi[0][2] += ppX[n - i][k] * ppX[n - j][k];

      i = 1;
      j = 1;
      fi[1][1] += ppX[n - i][k] * ppX[n - j][k];

      i = 1;
      j = 2;
      fi[1][2] += ppX[n - i][k] * ppX[n - j][k];

      i = 2;
      j = 2;
      fi[2][2] += ppX[n - i][k] * ppX[n - j][k];
    }

    d = fi[1][1] * fi[2][2] - rel * fi[1][2] * fi[1][2];
/*
 * pAlpha1
 */
    if ((Ipp64f)d * d > 0.f) {  /* if (d != 0) */
      pAlpha1[k] = (fi[0][1] * fi[1][2] - fi[0][2] * fi[1][1]) * (1.f / d);
    } else {
      pAlpha1[k] = 0.f;
    }
/*
 * pAlpha0
 */
    if (fi[1][1] > 0.f) {       /* if (fi[1][1] != 0) */
      pAlpha0[k] = -(fi[0][1] + pAlpha1[k] * fi[1][2]) * (1.f / fi[1][1]);
    } else {
      pAlpha0[k] = 0.f;
    }

    if (((pAlpha1[k] * pAlpha1[k]) >= 16.f) ||
        ((pAlpha0[k] * pAlpha0[k]) >= 16.f)) {
      pAlpha1[k] = pAlpha0[k] = 0.f;
    }

    if (fi[1][1] == 0.0f) {
      pReflectCoef[k] = 0.0f;
    } else {
      pReflectCoef[k] = IPP_MIN(IPP_MAX(-fi[0][1] / fi[1][1], -1), 1);
    }
  }

  return ippStsNoErr;
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER
