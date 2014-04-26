/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <math.h>
#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_struct.h"
#include "sbr_freq_tabs.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/********************************************************************/

static Ipp32s
isDetectMissingHarmonic(Ipp32s add_harmonic_flag, Ipp32s freq_res,
                        Ipp32s* add_harmonic, Ipp32s* pFreqTabs[2], Ipp32s p)
{
  Ipp32s missingHarmonic = 0;
  Ipp32s i;
  Ipp32s startHiBand = 0;
  Ipp32s stopHiBand = 0;

  if(add_harmonic_flag){

    if(freq_res == HI){

      if(add_harmonic[p]){

        missingHarmonic = 1;
      }
    }else{

      while(pFreqTabs[HI][startHiBand] < pFreqTabs[LO][p]) startHiBand++;

      while(pFreqTabs[HI][stopHiBand] < pFreqTabs[LO][p + 1]) stopHiBand++;

      for(i = startHiBand; i<stopHiBand; i++){

        if(add_harmonic[i]){
          missingHarmonic = 1;
          break;
        }
      }
    }
  }

  return missingHarmonic;
}
/********************************************************************/

Ipp32s
sbrencEnvEstimation (Ipp32f  bufNrg[][64],
                     sSBRFrameInfoState* pState,
                     Ipp16s* bufEnvQuant,
                     Ipp32s  nBand[2],
                     Ipp32s* pFreqTabs[2],
                     Ipp32s  amp_res,
                     Ipp32s* add_harmonic,
                     Ipp32s  add_harmonic_flag)
{
  Ipp32s env, p, k, i, pos = 0;
  Ipp32s iStart, iEnd, kStart, kEnd;
  Ipp32s freq_res;

  Ipp32s ca = 2 - amp_res;
  Ipp32s nEnv = pState->nEnv;
  Ipp32s missingHarmonic = 0;
  Ipp32s distBound = 0;

  Ipp32s criterion1, criterion2;

//  Ipp32f boostcomp = 1.0f;
  Ipp32f nrgLeft = 0.f, nrgLeftAlt = 0.f;

  /* AYA log */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "\nenv estimation\n");
#endif

  for (env = 0; env < nEnv; env++) {

    iStart = RATE * pState->bordersEnv[env];
    iEnd   = RATE * pState->bordersEnv[env + 1];

    freq_res = pState->freqRes[env];

    if (env == pState->shortEnv - 1) {
      iEnd = iEnd - RATE;
    }

    for (p = 0; p < nBand[freq_res]; p++) {

      nrgLeft = nrgLeftAlt = 0.f;

      kStart = pFreqTabs[freq_res][p];
      kEnd = pFreqTabs[freq_res][p + 1];

      criterion1 = (freq_res == HI) && (p == 0) && (kEnd-kStart > 1);
      criterion2 = (freq_res == LO ) && (p == 0) && (kEnd-kStart > 2);

      if ( criterion1 || criterion2 ) kStart++;

      missingHarmonic = isDetectMissingHarmonic(add_harmonic_flag, freq_res,
                                                add_harmonic, pFreqTabs, p);
//-----------------------------------------------------
      if(missingHarmonic){/* missingHarmonic */

        distBound = (iEnd - iStart);

        for (i = iStart; i < iEnd; i++) {
          nrgLeft += bufNrg[i][kStart];
        }

        for (k = kStart+1; k < kEnd; k++){
          nrgLeftAlt = 0;

          for (i = iStart; i < iEnd; i++) {
            nrgLeftAlt += bufNrg[i][k];
          }

          if(nrgLeftAlt > nrgLeft){
            nrgLeft = nrgLeftAlt;
          }
        }

        /* undocumented patch */
        if(kEnd-kStart > 2){
          nrgLeft = nrgLeft*0.398107267f;
        } else if (kEnd-kStart > 1){
          nrgLeft = nrgLeft*0.5f;
        }

//-----------------------------------------------------
      } else {/* NONE missingHarmonic */

        distBound = (iEnd - iStart) * (kEnd - kStart);
        for (k = kStart; k < kEnd; k++) {

          for (i = iStart; i < iEnd; i++) {
            nrgLeft += bufNrg[i][k];
          }
        }
      }

      /********************************************************************/
      /*                     QUANTIZATION                                 */
      /********************************************************************/

      nrgLeft = (Ipp32f) (log (nrgLeft / (distBound * 64) + EPS) * ILOG2);
      /* AYA log */
#ifdef SBR_NEED_LOG
      fprintf(logFile, "\nEnv Quant %15.10f\n", nrgLeft);
#endif

      nrgLeft = IPP_MAX(nrgLeft,0);

      bufEnvQuant[pos] = (Ipp16s) (ca * nrgLeft + 0.5);

      /* AYA log */
      //fprintf(logFile, "\nEnv Quant %i\n", bufEnvQuant[pos]);

      pos++;

    } /* p */
  } /* env */

  return 0;//OK
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

