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
#include "sbr_settings.h"
#include "sbr_freq_tabs.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"
#include "sbr_enc_settings.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

#include "aac_status.h"

/********************************************************************/

#ifndef NOISE_FLOOR_OFFSET
#define NOISE_FLOOR_OFFSET  6
#endif

/********************************************************************/

/* differ from GPP standard */
static const Ipp32f hSmoothFilter[] = {
  0.05857864376269f,
  0.2f,
  0.34142135623731f,
  0.4f
};

/********************************************************************/

static Ipp32s
sbrencCalcNoiseFloor(Ipp32f bufT[][64],
                     Ipp32s* mapTab,

                     Ipp32s startIndex,
                     Ipp32s stopIndex,
                     Ipp32s startQBand,
                     Ipp32s stopQBand,

                     Ipp32s missingHarmonicFlag,
                     Ipp32s inverseFilteringLevel,

                     Ipp32f *noiseLevel,
                     Ipp32f nf_max_level)
{
  Ipp32s band,k;
  Ipp32f origTonalMean, sbrTonalMean, diff;
  Ipp32f origTonal, sbrTonal;
  Ipp32s distQBand = (stopQBand - startQBand);
  Ipp32s distIndx  = (stopIndex-startIndex);

  Ipp32f invDistQBand = 1.f / distQBand;
  Ipp32f invDistIndx  = 1.f / distIndx;

  /* AYA log */
  //const Ipp32f nfMaxLevel   = 4.0f ;
  const Ipp32f nfOffset= 1.0f;
  Ipp32s diffThres   = INVF_INTERMEDIA_LEVEL;
  const Ipp32f weightFac = 0.25f;

  Ipp32f invDist = (missingHarmonicFlag) ? invDistIndx : (invDistQBand * invDistIndx);

  /* CODE */
  origTonalMean = sbrTonalMean = 0.0f;

  for(band = startQBand; band < stopQBand;band++){

    sbrTonal = origTonal = 0.0f;

    for(k = startIndex ; k < stopIndex; k++){
      origTonal += bufT[k][band];
      sbrTonal  += bufT[k][mapTab[band]];
    }

    if( missingHarmonicFlag ){
      origTonalMean = IPP_MAX(origTonal,  origTonalMean);
      sbrTonalMean  = IPP_MAX(sbrTonal, sbrTonalMean);
    } else {
      origTonalMean += origTonal;
      sbrTonalMean  += sbrTonal;
    }
  }

  origTonalMean *= invDist;
  sbrTonalMean  *= invDist;

  /* patch: corrected */
  if(origTonalMean < (Ipp32f)0.000976562f && sbrTonalMean < (Ipp32f)0.000976562f){
    origTonalMean = 101.5936673f;
    sbrTonalMean  = 101.5936673f;
  }

  if(origTonalMean < (Ipp32f)1.0f) {
    origTonalMean = 1.0f;
  }

  if(sbrTonalMean < (Ipp32f)1.0f){
    sbrTonalMean  = 1.0f;
  }

  if(missingHarmonicFlag == 1){
    diff = 1.0f;
  } else {
    diff = IPP_MAX((Ipp32f)1.0f, weightFac*sbrTonalMean/origTonalMean);
  }

  if(inverseFilteringLevel == INVF_INTERMEDIA_LEVEL ||
     inverseFilteringLevel == INVF_LOW_LEVEL        ||
     inverseFilteringLevel == INVF_OFF_LEVEL){

      diff = 1.0f;
    }

    if (inverseFilteringLevel <= diffThres) {
      diff = 1.0f;
    }

    *noiseLevel  = diff/origTonalMean;
    *noiseLevel *= nfOffset;
    *noiseLevel  = IPP_MIN(*noiseLevel, nf_max_level);

    return 0;//OK
}

/**************************************************************************/

static Ipp32s
sbrencNoiseSmoothing (Ipp32f *bufNoiseFloor,
                      Ipp32f prevNoiseLevels[NF_SMOOTHING_LENGTH][MAX_NUM_NOISE_VALUES],
                      Ipp32s nEnv,
                      Ipp32s nNoiseBands)
{
  Ipp32s i;
  Ipp32s band,env;
  Ipp32f* bufNoiseOrig = bufNoiseFloor;


  for(env = 0; env < nEnv; env++){

    for (i = 1; i < NF_SMOOTHING_LENGTH; i++){
      ippsCopy_32f(prevNoiseLevels[i], prevNoiseLevels[i - 1], nNoiseBands);
    }

    ippsCopy_32f(bufNoiseFloor+env*nNoiseBands, prevNoiseLevels[NF_SMOOTHING_LENGTH - 1], nNoiseBands);

    for (band = 0; band < nNoiseBands; band++){

      bufNoiseOrig[band+ env*nNoiseBands] = 0;

      for (i = 0; i < NF_SMOOTHING_LENGTH; i++){

        bufNoiseOrig[band+ env*nNoiseBands] += hSmoothFilter[i]*prevNoiseLevels[i][band];
      }
    }
  }

  return 0;//OK
}

/********************************************************************/

Ipp32s sbrencNoisefEstimation(sSBREnc_SCE_State* pState, Ipp32s* pFreqTab, Ipp32s  nNoiseBand, Ipp16s* bufNoiseQuant)
{
  Ipp32s  nNoiseEnv  = pState->sbrFIState.nNoiseEnv;
  Ipp32s* prev_bs_invf_mode = pState->sbrInvfEst.bs_invf_mode;

#if !defined(ANDROID)
  Ipp32s  startIndx[2];
  Ipp32s  stopIndx[2];
#else
  static Ipp32s  startIndx[2];
  static Ipp32s  stopIndx[2];
#endif

  Ipp32s  curNoiseQuant = 0;

  Ipp32s  missingHarmonicsFlag = pState->sbrEDState.bs_add_harmonic_flag;

  Ipp32s  env, band;

  Ipp32f* bufNoiseFloor = pState->sbrNoiseEst.vecNoiseOrig;

  /* init */
  ippsZero_32f(bufNoiseFloor, MAX_NUM_NOISE_VALUES);

  if(nNoiseEnv == 1){
     startIndx[0] = 0;
     stopIndx[0]  = 2;// mismatch with GPP Spec
  } else if(nNoiseEnv == 2){

    startIndx[0] = 0;
    stopIndx[0]  = 1;

    startIndx[1] = 1;
    stopIndx[1]  = 2;
  } else {
    return -1; //error
  }

  /* calculation of noise floor tonality */

  /* AYA log */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "\n NF Est\n");
#endif

  for(env = 0; env < nNoiseEnv; env++){
    for(band = 0; band < nNoiseBand; band++){

      sbrencCalcNoiseFloor(pState->bufT,
                           pState->tabPatchMap,

                           startIndx[env],
                           stopIndx[env],

                           pFreqTab[band],
                           pFreqTab[band+1],

                           missingHarmonicsFlag,
                           prev_bs_invf_mode[band],
                           bufNoiseFloor + band + env*nNoiseBand,
                           pState->sbrNoiseEst.nf_max_level);

      /* AYA log */
#ifdef SBR_NEED_LOG
      fprintf(logFile, " %15.10f\n", bufNoiseFloor[band + env*nNoiseBand]);
#endif
    }
  }

  sbrencNoiseSmoothing (bufNoiseFloor,
                        pState->sbrNoiseEst.prevNoiseFloorLevels,
                        nNoiseEnv,
                        nNoiseBand );

  /* AYA log */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "\n NF smooth Est\n");

  for(env = 0; env < nNoiseEnv; env++){
    for(band = 0; band < nNoiseBand; band++){
      /* AYA log */
      fprintf(logFile, " %15.10f\n", bufNoiseFloor[band + env*nNoiseBand]);
    }
  }
#endif

  /********************************************************************/
  /*                     QUANTIZATION                                 */
  /********************************************************************/
  for(env = 0; env < nNoiseEnv; env++){
    for(band = 0; band < nNoiseBand; band++){
      curNoiseQuant = (Ipp32s)(NOISE_FLOOR_OFFSET - (Ipp32f) (ILOG2*log(bufNoiseFloor[band + env*nNoiseBand])) + 0.5f);
      /* saturation */
      curNoiseQuant = IPP_MIN(curNoiseQuant, 30);
      bufNoiseQuant[band + env*nNoiseBand] = (Ipp16s)IPP_MAX(curNoiseQuant, 0);
    }
  }

  return 0;//OK
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

