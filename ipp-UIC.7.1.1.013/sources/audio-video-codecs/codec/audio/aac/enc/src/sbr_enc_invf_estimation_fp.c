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
#include <memory.h>
#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_freq_tabs.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_struct.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/********************************************************************/

static const Ipp32f hysteresis = 1.0f;

/********************************************************************/
/*       struct from GPP SBR standard (3GPP TS 26.404 V 1.0.0       */
/*       params struct may be changed                               */
/********************************************************************/


/********************************************************************/

static const Ipp32f  regionBordSBR[4]  = { 1.0f, 10.0f, 14.0f, 19.0f };

static const Ipp32f  regionBordOrig[4] = { 0.0f,  3.0f,  7.0f, 10.0f };

static const Ipp32f  regionBordNrg[4]  = {25.0f, 30.0f, 35.0f, 40.0f };

static const Ipp32s numBordSBR  = 4;

static const Ipp32s numBordOrig = 4;

static const Ipp32s numBordNrg  = 4;

static const Ipp32s regionSpace[5][5] = {
  {INVF_INTERMEDIA_LEVEL, INVF_LOW_LEVEL,        INVF_OFF_LEVEL,        INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_INTERMEDIA_LEVEL, INVF_LOW_LEVEL,        INVF_OFF_LEVEL,        INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_STRONG_LEVEL,     INVF_INTERMEDIA_LEVEL, INVF_LOW_LEVEL,        INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_STRONG_LEVEL,     INVF_STRONG_LEVEL,     INVF_INTERMEDIA_LEVEL, INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_STRONG_LEVEL,     INVF_STRONG_LEVEL,     INVF_INTERMEDIA_LEVEL, INVF_OFF_LEVEL, INVF_OFF_LEVEL}
};

static const Ipp32s regionSpaceTransient[5][5] =   {
  {INVF_LOW_LEVEL,        INVF_LOW_LEVEL,        INVF_LOW_LEVEL,        INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_LOW_LEVEL,        INVF_LOW_LEVEL,        INVF_LOW_LEVEL,        INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_STRONG_LEVEL,     INVF_INTERMEDIA_LEVEL, INVF_INTERMEDIA_LEVEL, INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_STRONG_LEVEL,     INVF_STRONG_LEVEL,     INVF_INTERMEDIA_LEVEL, INVF_OFF_LEVEL, INVF_OFF_LEVEL},
  {INVF_STRONG_LEVEL,     INVF_STRONG_LEVEL,     INVF_INTERMEDIA_LEVEL, INVF_OFF_LEVEL, INVF_OFF_LEVEL}
};

static const Ipp32s reductionFactors[5] = {-4, -3, -2, -1, 0};

/********************************************************************/

static Ipp32f tapsInvfFIR[] = { 0.5f, 0.375f, 0.125f};

/********************************************************************/

static Ipp32s
sbrencCalcTonalMeanParams(Ipp32f bufT[][64], Ipp32f* bufNrg,
                          sSBRTonalMeanInfo* pMeanState,
                          Ipp32s* tabPatchMap,  Ipp32s iStart, Ipp32s iEnd )
{
  Ipp32f* bufOrigTonalMean = pMeanState->bufOrigTonalMean;
  Ipp32f* bufSbrTonalMean = pMeanState->bufSbrTonalMean;

  Ipp32f origTonalMean = 0.0f;
  Ipp32f sbrTonalMean = 0.0f;

  Ipp32f denum = 0.5f / ( iEnd - iStart );

  Ipp32s i;
  Ipp32s idx;

  for(i=iStart; i<iEnd; i++){
    origTonalMean += bufT[0][i] + bufT[1][i];

    idx = tabPatchMap[i];

    if( idx != EMPTY_MAPPING ){
      sbrTonalMean += bufT[0][ idx ] + bufT[1][ idx ];
    }
  }

  /* normalization */
  origTonalMean = origTonalMean * denum;
  sbrTonalMean = sbrTonalMean * denum;

  /* up-date prev Tonality Mean values */
  bufOrigTonalMean[2] = bufOrigTonalMean[1];
  bufOrigTonalMean[1] = bufOrigTonalMean[0];
  bufOrigTonalMean[0] = origTonalMean;

  bufSbrTonalMean[2] = bufSbrTonalMean[1];
  bufSbrTonalMean[1] = bufSbrTonalMean[0];
  bufSbrTonalMean[0] = sbrTonalMean;

  /* filtering */
  pMeanState->origTonalMeanSmooth = 0.0f;
  pMeanState->sbrTonalMeanSmooth  = 0.0f;

  for(i=0; i<3; i++){
    pMeanState->origTonalMeanSmooth += bufOrigTonalMean[i] * tapsInvfFIR[i];
    pMeanState->sbrTonalMeanSmooth  += bufSbrTonalMean[i]  * tapsInvfFIR[i];
  }

  //pMeanState->meanNrg = 0.5f * (bufNrg[0] + bufNrg[1]);

  pMeanState->meanNrg = bufNrg[0];

  return 0;//OK
}

/********************************************************************/

static Ipp32s
sbrencFindValidRegion(Ipp32f currVal, Ipp32f* borders, Ipp32s numBorders)
{
  Ipp32s region;

  if(currVal < borders[0])  {
    return 0;
  }

  for(region = 1; region < numBorders; region++){

    if( currVal >= borders[region-1] && currVal < borders[region]) {
      return region;
    }
  }

  if(currVal > borders[numBorders-1]) {
    return numBorders;
  }

  return 0;
}

/**************************************************************************/

static Ipp32s
sbrencRegionBordCorrect(sSBRTonalMeanInfo state,
                        Ipp32s transientFlag,
                        Ipp32s* prevRegionSBR,
                        Ipp32s* prevRegionOrig)
{
  Ipp32s invfLevel;
  Ipp32s regionSBR, regionOrig, regionNrg;

#if !defined(ANDROID)
  Ipp32f quantStepsSbrTmp[MAX_NUM_NOISE_BANDS];
  Ipp32f quantStepsOrigTmp[MAX_NUM_NOISE_BANDS];
#else
  static Ipp32f quantStepsSbrTmp[MAX_NUM_NOISE_BANDS];
  static Ipp32f quantStepsOrigTmp[MAX_NUM_NOISE_BANDS];
#endif

  Ipp32f origTonalMeanFilt;
  Ipp32f sbrTonalMeanFilt;
  Ipp32f nrgMeanFilt;

  /* init */
  origTonalMeanFilt = (Ipp32f) (ILOG2*3.0f*log(state.origTonalMeanSmooth+EPS));
  sbrTonalMeanFilt  = (Ipp32f) (ILOG2*3.0f*log(state.sbrTonalMeanSmooth+EPS));
  nrgMeanFilt       = (Ipp32f) (ILOG2*1.5f*log(state.meanNrg+EPS));

  memcpy( quantStepsSbrTmp,  regionBordSBR,  numBordSBR  * sizeof(Ipp32f) );
  memcpy( quantStepsOrigTmp, regionBordOrig, numBordOrig * sizeof(Ipp32f) );

  /* modification region */
  // SBR region
  if(*prevRegionSBR < numBordSBR){
    quantStepsSbrTmp[*prevRegionSBR] = regionBordSBR[*prevRegionSBR] + hysteresis;
  }

  if(*prevRegionSBR > 0){
    quantStepsSbrTmp[*prevRegionSBR - 1] = regionBordSBR[*prevRegionSBR - 1] - hysteresis;
  }

  regionSBR  = sbrencFindValidRegion(sbrTonalMeanFilt, quantStepsSbrTmp, numBordSBR);

  *prevRegionSBR = regionSBR;

  //Orig region
  if(*prevRegionOrig < numBordOrig){
    quantStepsOrigTmp[*prevRegionOrig] = regionBordOrig[*prevRegionOrig] + hysteresis;
  }

  if(*prevRegionOrig > 0){
    quantStepsOrigTmp[*prevRegionOrig - 1] = regionBordOrig[*prevRegionOrig - 1] - hysteresis;
  }

  regionOrig = sbrencFindValidRegion(origTonalMeanFilt, quantStepsOrigTmp, numBordOrig);

  *prevRegionOrig = regionOrig;

  regionNrg  = sbrencFindValidRegion(nrgMeanFilt, (Ipp32f*)regionBordNrg, (Ipp32s)numBordNrg);

  if(transientFlag == 1){
    invfLevel = regionSpaceTransient[regionSBR][regionOrig];
  }else{
    invfLevel = regionSpace[regionSBR][regionOrig];
  }

  invfLevel = IPP_MAX(invfLevel + reductionFactors[regionNrg], 0);

  return invfLevel;
}

/**************************************************************************/

Ipp32s sbrencInvfEstimation (sSBRInvfEst* pState,
                             Ipp32f  bufT[][64],
                             Ipp32f* bufNrg,
                             Ipp32s* tabPatchMap,
                             Ipp32s  transientFlag,
                             Ipp32s* pNoiseFreqTab,
                             Ipp32s  nNoiseBand,
                             Ipp32s* bs_invf_mode)
{
  sSBRTonalMeanInfo* pMeanState = pState->sbrTonalMeanInfo;
  Ipp32s band, iStart, iEnd;

  /* AYA log */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "TonalAvgParams\n");
#endif

  for(band = 0; band < nNoiseBand; band++){
    iStart = pNoiseFreqTab[ band ];
    iEnd   = pNoiseFreqTab[ band + 1 ];

    sbrencCalcTonalMeanParams(bufT, bufNrg, &pMeanState[band],
                              tabPatchMap,  iStart, iEnd );
#ifdef SBR_NEED_LOG
    {
      //pState->sbrTonalMeanInfo[band].sbrTonalMeanSmooth
      fprintf(logFile, "avgNrg = %15.10f\n", pMeanState[band].meanNrg);
      fprintf(logFile, "origTonalAvgSmooth = %15.10f\n", pMeanState[band].origTonalMeanSmooth);
      fprintf(logFile, "sbrTonalAvgSmooth = %15.10f\n", pMeanState[band].sbrTonalMeanSmooth);

    }
#endif

    bs_invf_mode[band]= sbrencRegionBordCorrect(pMeanState[band],
                                                transientFlag,
                                                &pState->prevRegionSBR[band],
                                                &pState->prevRegionOrig[band]);

#ifdef SBR_NEED_LOG
    fprintf(logFile, "infVec[%i] = %i\n", band, bs_invf_mode[band]);
#endif

  }

  return 0; //OK
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

