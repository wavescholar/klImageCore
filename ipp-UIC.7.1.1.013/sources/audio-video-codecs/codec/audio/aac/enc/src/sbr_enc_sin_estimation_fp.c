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
#include "align.h"
#include "aac_status.h"
#include "sbr_freq_tabs.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/********************************************************************/

static Ipp32s
sbrencDiffEst(Ipp32f* bufT, Ipp32f* bufDiff, Ipp32s* indxMapTab, Ipp32s* pFreqTab, Ipp32s nBand)
{
  Ipp32s m, k, idx, kStart, kEnd;
  Ipp32f maxOrigT, maxSbrT;

  for(m = 0; m < nBand; m++){
    kStart = pFreqTab[m];
    kEnd   = pFreqTab[m+1];

    maxOrigT = maxSbrT = 0.0f;
    for(k = kStart; k < kEnd; k++){
      maxOrigT = ( bufT[k] > maxOrigT ) ? bufT[k] : maxOrigT;

      idx = indxMapTab[k];
      if ( idx != EMPTY_MAPPING ){
        maxSbrT = ( bufT[ idx ] > maxSbrT ) ? bufT[ idx ] : maxSbrT;
      }
    }

    maxSbrT = IPP_MAX( 1.0f, maxSbrT );

    bufDiff[m] = maxOrigT;

    if ( maxSbrT > 1.0f ){
      bufDiff[m] /= maxSbrT;
    }
  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s
sbrencSfmEst(Ipp32f* bufT, Ipp32f* bufSfm, Ipp32s* pFreqTab, Ipp32s nBand)
{
  Ipp32s m, k, kStart, kEnd;
  Ipp32f accSum, accMul;
  Ipp32f deg, denum;
  Ipp32f dist;

  for(m = 0; m < nBand; m++ ){
    kStart = pFreqTab[ m ];
    kEnd   = pFreqTab[ m + 1 ];

    accSum = 0.0f;
    accMul = 1.0f;

    bufSfm[m] = 1.0f;

    dist = (Ipp32f)( kEnd - kStart );
    if (dist < 2.0f ) continue;

    deg = 1.0f / dist;

    for(k = kStart; k < kEnd; k++){
      accSum += bufT[k];
      accMul *= bufT[k];
    }

    if ( (Ipp64f)accMul * accMul > 0.0f ) {
      denum = (Ipp32f)pow(accMul, -deg);
      bufSfm[m] = accSum * deg * denum;

      /* patch from 3GPP: p.29, instead forward val use invert val */
      bufSfm[m] = 1.f / bufSfm[m];
    }
  }

  return 0; //OK
}

/********************************************************************/

static Ipp32s
sbrencCalcInDataSinEst(Ipp32f bufT[][64],
                       Ipp32f bufDiff[][64],
                       Ipp32f bufSfmOrig[][64],
                       Ipp32f bufSfmSBR[][64],
                       Ipp32s* indxMapTab,
                       Ipp32s* pFreqTab,
                       Ipp32s nBand)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, tmpBuf, 64);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, tmpBuf, 64);
#endif
  Ipp32s est, k, idx;
//  Ipp32s p;
  Ipp32s i, iStart, iEnd;

  /* up-date buffer */
  for(est =  0 ; est < 2; est++){
    ippsCopy_32f(bufDiff[est + 2],    bufDiff[est],    64);
    ippsCopy_32f(bufSfmOrig[est + 2], bufSfmOrig[est], 64);
    ippsCopy_32f(bufSfmSBR[est + 2],  bufSfmSBR[est],  64);
  }

  /* AYA log */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "\nSinesEstimation\n");
  fprintf(logFile, "\nDiff    origSfm    sbrSfm\n");
#endif

  /* detection input data */
  for(est = 2; est < 4; est++){
    /* AYA log */
#ifdef SBR_NEED_LOG
    fprintf(logFile, "band = %i\n", est);
#endif

    sbrencDiffEst( bufT[ est ], bufDiff[ est ], indxMapTab, pFreqTab, nBand );

    sbrencSfmEst( bufT[ est ], bufSfmOrig[ est ], pFreqTab, nBand );

    ippsZero_32f(tmpBuf, 64);
    for(k = 0; k < nBand; k++){
      iStart = pFreqTab[k];
      iEnd   = pFreqTab[k+1];
      for(i=iStart; i<iEnd; i++){

        idx = indxMapTab[i];
        if ( idx != EMPTY_MAPPING){
          tmpBuf[i] = bufT[est][ idx ];
        }
      }
    }

    sbrencSfmEst( tmpBuf, bufSfmSBR[ est ], pFreqTab, nBand );

    /* AYA */
#ifdef SBR_NEED_LOG
    for(p=0; p<nBand; p++){
      fprintf(logFile, "%15.10f %15.10f %15.10f\n", bufDiff[ est ][p], bufSfmOrig[ est ][p], bufSfmSBR[ est ][p]);
    }
#endif

  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s
sbrencIsDetectNewToneAllow(sSBRFrameInfoState *pFrameInfo,

                           Ipp32s* prevTranFrame,
                           Ipp32s* prevTranPos,
                           Ipp32s* prevTranFlag,

                           Ipp32s tranPosOffset,
                           Ipp32s tranFlag,
                           Ipp32s tranPos)
{
  Ipp32s tranFrame, result;

  const Ipp32s SBR_TIME_SLOTS = 16;
  Ipp32s criterion1, criterion2, criterion3;

  /* init */
  tranFrame = 0;

  /* calc criterion */
  criterion1 = tranFlag;
  criterion2 = (pFrameInfo->bordersEnv[pFrameInfo->nEnv] - (tranPos + tranPosOffset) > 0 ) ? 1 : 0;
  criterion3 = !(*prevTranFlag) || (*prevTranFrame);

  if( (criterion1 && criterion2) || !(criterion1 || criterion3) ) {
    tranFrame = 1;
  }

  /* init */
  result = 0;

  /* calc criterion */
  criterion1 = tranFrame;
  criterion2 = *prevTranFrame;
  criterion3 = (DELTA_TIME - abs(pFrameInfo->bordersEnv[0] - (*prevTranPos + tranPosOffset - SBR_TIME_SLOTS)) > 0) ? 1 : 0;

  /* decision */
  if ( criterion1 || (criterion2 && criterion3)){
     result = 1;
  }

  /* up-date */
  *prevTranFlag  = tranFlag;
  *prevTranFrame = tranFrame;
  *prevTranPos   = tranPos;

  return result;
}

/**************************************************************************/
static Ipp32s
sbrencDetectionSinEst(Ipp32f* bufT,
                      Ipp32f* bufDiff,
                      Ipp32s* detVec,
                      Ipp32s* pFreqTab,
                      Ipp32s  nBand,
                      Ipp32f* bufSfmOrig,
                      Ipp32f* bufSfmSBR,

                      sSBRGuideData guideState,
                      sSBRGuideData guideNewState)
{
  Ipp32f tmpThres = 0.f, origThres = 0.f;
  Ipp32s criterion1 = 0, criterion2 = 0, criterion3 = 0;
  Ipp32s iStart = 0, iEnd = 0;

  Ipp32s band = 0;
  Ipp32s i = 0;

  /* ******************************
   * detection algorithm: step 1
   * ****************************** */
  for(band = 0; band < nBand; band++){
    if (guideState.bufGuideDiff[band]) {

      tmpThres = IPP_MAX(DECAY_GUIDE_DIFF*guideState.bufGuideDiff[band],THR_DIFF_GUIDE);
    } else {

      tmpThres = THR_DIFF;
    }

    tmpThres = IPP_MIN(tmpThres, THR_DIFF);

    if(bufDiff[band] > tmpThres){

      detVec[band] = 1;
      guideNewState.bufGuideDiff[band] = bufDiff[band];
    } else {

      if(guideState.bufGuideDiff[band]){

        guideState.bufGuideOrig[band] = THR_TONE_GUIDE;
      }
    }
  }

  /* ******************************
  * detection algorithm: step 2
  * ****************************** */
  for(band = 0; band < nBand; band++){

    iStart = pFreqTab[band];
    iEnd = pFreqTab[band+1];

    origThres   = IPP_MAX(guideState.bufGuideOrig[band] * DECAY_GUIDE_ORIG, THR_TONE_GUIDE );

    origThres   = IPP_MIN(origThres, THR_TONE);

    if(guideState.bufGuideOrig[band]){

      for(i = iStart; i < iEnd; i++){

        if(bufT[i] > origThres){

          detVec[band] = 1;
          guideNewState.bufGuideOrig[band] = bufT[i];
        }
      }
    }
  }

  /* ******************************
   * detection algorithm: step 3
   * ****************************** */
  origThres   = THR_TONE;

  for(band = 0; band < nBand; band++){

    iStart = pFreqTab[band];
    iEnd = pFreqTab[band+1];

    if(iEnd -iStart > 1){

      for(i = iStart; i < iEnd; i++){

        /* get criterion */
        criterion1 = (bufT[i]       > origThres)      ? 1 : 0;
        criterion2 = (bufSfmSBR[band]  > THR_SFM_SBR ) ? 1 : 0;
        criterion3 = (bufSfmOrig[band] < THR_SFM_ORIG) ? 1 : 0;

        if(criterion1 && criterion2 && criterion3){

          detVec[band] = 1;
          guideNewState.bufGuideOrig[band] = bufT[i];
        }
      }
    } else {

      if(band < nBand -1){

        //iStart = pFreqTab[band];

        criterion1 = (bufT[iStart]     > THR_TONE )     ? 1 : 0;

        if(band){

          criterion2 = (bufDiff[+1]  < INV_THR_TONE ) ? 1 : 0;
          criterion3 = (bufDiff[band-1] < INV_THR_TONE ) ? 1 : 0;

          if(criterion1 && ( criterion2 || criterion3) ){

              detVec[band] = 1;
              guideNewState.bufGuideOrig[band] = bufT[iStart];
          }
        } else {

          criterion2 = (bufDiff[band+1] < INV_THR_TONE) ? 1 : 0;

          if(criterion1 && criterion2){

              detVec[band] = 1;
              guideNewState.bufGuideOrig[band] = bufT[iStart];
          }
        }
      }
    }
  }

  return 0;//OK

}

/*************************************************************************/

static Ipp32s
sbrencTransientCorrection(Ipp32f bufT[][64],
                          Ipp32s bufDetection[][64],
                          Ipp32s* pFreqTab,
                          Ipp32s nBand,
                          sSBRGuideData state,
                          Ipp32s start,
                          Ipp32s stop)
{
  Ipp32f maxVal1, maxVal2;
  Ipp32s maxPos1, maxPos2;

  Ipp32s i, est;
  Ipp32s iStart, iEnd, iStart2, iEnd2;
  Ipp32s criterion;
#if !defined(ANDROID)
  Ipp32s bs_add_harmonic[MAX_NUM_FREQ_COEFFS];
#else
  static Ipp32s bs_add_harmonic[MAX_NUM_FREQ_COEFFS];
#endif

  ippsZero_32s(bs_add_harmonic, MAX_NUM_FREQ_COEFFS);

  for(est = start; est < stop; est++){

    for(i=0;i<nBand-1;i++){

      bs_add_harmonic[i] = bs_add_harmonic[i] || bufDetection[est][i];
    }
  }

  for(i = 0; i < nBand-1; i++){
    iStart = pFreqTab[i];
    iEnd   = pFreqTab[i+1];

    criterion = bs_add_harmonic[i] && bs_add_harmonic[i+1];
    if ( !criterion ) continue;

    iStart = pFreqTab[i];
    iEnd   = pFreqTab[i+1];

    maxPos1 = iStart;
    maxVal1 = bufT[start][iStart];

    iStart2 = pFreqTab[i+1];
    iEnd2   = pFreqTab[i+2];

    maxPos2 = iStart2;
    maxVal2 = bufT[start][iStart2];

    for(est = start; est < stop; est++){

      if ( iEnd - iStart > 1 ) {
        ippsMaxIndx_32f(bufT[est] + iStart, iEnd - iStart, &maxVal1, &maxPos1);
      }

      if ( iEnd2 - iStart2 > 1 ) {
        ippsMaxIndx_32f(bufT[est] + iStart2, iEnd2 - iStart2, &maxVal2, &maxPos2);
      }
    }

    if(maxPos2 < 2 + maxPos1){

      if(maxVal1 - maxVal2 > 0){

        state.bufGuideDetect[i+1] = 0;
        state.bufGuideOrig[i+1]     = 0;
        state.bufGuideDiff[i+1]     = 0;

        for(est = start; est < stop; est++){
          bufDetection[est][i+1] = 0;
        }
      } else {

        state.bufGuideDetect[i] = 0;
        state.bufGuideOrig[i]     = 0;
        state.bufGuideDiff[i]     = 0;

        for(est = start; est < stop; est++){
          bufDetection[est][i] = 0;
        }
      }
    }
  }

  return 0;//OK
}

/**************************************************************************/

static Ipp32s
sbrencTotalDetectionSinEst(Ipp32f bufT[][64],
                           Ipp32f bufDiff[][64],
                           Ipp32s nBand,
                           Ipp32s* pFreqTab,
                           Ipp32f bufSfmOrig[][64],
                           Ipp32f bufSfmSBR[][64],
                           Ipp32s bufDetection[][64],

                           Ipp32s* prev_bs_add_harmonic,
                           sSBRGuideData* pGuideState,

                           Ipp32s noEstPerFrame,
                           Ipp32s totNoEst,
                           Ipp32s newDetectionAllowed,
                           Ipp32s* bs_add_harmonic)
{
  Ipp32s est = 0;
  Ipp32s start = (newDetectionAllowed) ? noEstPerFrame : 0;
  Ipp32s band;

  ippsZero_32s(bs_add_harmonic, nBand);

  /* ******************************
   * up-date buffers
   * ****************************** */

  if(newDetectionAllowed){

    ippsCopy_32f(pGuideState[0].bufGuideDiff, pGuideState[noEstPerFrame].bufGuideDiff, nBand);
    ippsCopy_32f(pGuideState[0].bufGuideOrig, pGuideState[noEstPerFrame].bufGuideOrig, nBand);

    ippsZero_32s(pGuideState[noEstPerFrame-1].bufGuideDetect, nBand);
  }

  for(est = start; est < totNoEst; est++){

    if(est > 0){
      ippsCopy_32s(bufDetection[est-1], pGuideState[est].bufGuideDetect, nBand);
    }

    ippsZero_32s(bufDetection[est], nBand);

    band = (est < totNoEst-1) ? est+1 : est;

    ippsZero_32f(pGuideState[band].bufGuideDiff,   nBand);
    ippsZero_32f(pGuideState[band].bufGuideOrig,   nBand);
    ippsZero_32s(pGuideState[band].bufGuideDetect, nBand);

    /* ******************************
     * main detection algorithm
     * ****************************** */

    sbrencDetectionSinEst(bufT[est],
                          bufDiff[est],
                          bufDetection[est],
                          pFreqTab,
                          nBand,
                          bufSfmOrig[est],
                          bufSfmSBR[est],
                          pGuideState[est],
                          pGuideState[band]);
  }

  /* *******************************************
   * additional step: because there is transient
   * ******************************************* */
  if(newDetectionAllowed){

      sbrencTransientCorrection(bufT,
                                bufDetection,
                                pFreqTab,
                                nBand,
                                pGuideState[noEstPerFrame],
                                start,
                                totNoEst);
  }

  /* *****************************************************
   * finally decision: merged
   * ***************************************************** */
  for(band = 0; band< nBand; band++){

    for(est = start; est < totNoEst; est++){

      bs_add_harmonic[band] = bs_add_harmonic[band] || bufDetection[est][band];
    }
  }

  /* *****************************************************
   * detections that were not present before are removed
   * ***************************************************** */
  if(!newDetectionAllowed){

    for(band=0; band < nBand; band++){

      if(bs_add_harmonic[band] - prev_bs_add_harmonic[band] > 0) {

        bs_add_harmonic[band] = 0;
      }
    }
  }

  return 0;//OK
}

/**************************************************************************/

static Ipp32s
sbrencCalcCompensation(Ipp32f bufT[][64],
                       Ipp32f bufDiff[][64],
                       Ipp32s* pFreqTab,
                       Ipp32s nBand,
                       Ipp32s* bs_add_harmonic,
                       Ipp32s* bufComp,
                       Ipp32s totNoEst)

{
  Ipp32f maxVal;
  Ipp32f curThres;

  Ipp32s band, j, i, iStart, iEnd;
  Ipp32s maxPosF,maxPosT;
  Ipp32s compValue;


  ippsZero_32s(bufComp, nBand);

  for(band=0 ; band < nBand; band++){

    if(0 == bs_add_harmonic[band]) continue;

    /* miss sine has been detected */

    iStart = pFreqTab[band];
    iEnd = pFreqTab[band+1];

    maxPosF = 0;
    maxPosT = 0;
    maxVal  = 0;

    for(j=0;j<totNoEst;j++){

      for(i=iStart; i<iEnd; i++){

        if(bufT[j][i] > maxVal) {

          maxVal = bufT[j][i];
          maxPosF = i;
          maxPosT = j;
        }
      }
    }

    if(maxPosF == iStart && band){

      compValue = (Ipp32s) (fabs(ILOG2*log(bufDiff[maxPosT][band - 1]+EPS)) + 0.5f);

      compValue = IPP_MIN( compValue, MAX_COMP );

      if((!bs_add_harmonic[band-1]) && (maxPosF >= 1) ) {

        if(bufT[maxPosT][maxPosF -1] > TONALITY_QUOTA*bufT[maxPosT][maxPosF]){

          bufComp[band-1] = -1*compValue;
        }
      }
    }

    /* STEP2 [+1] */
    if(maxPosF == iEnd-1 && band+1 < nBand){

      compValue = (Ipp32s) (fabs(ILOG2*log(bufDiff[maxPosT][band + 1]+EPS)) + 0.5f);

      compValue = IPP_MIN( compValue, MAX_COMP );

      if(!bs_add_harmonic[band+1]) {

        if(bufT[maxPosT][maxPosF+1] > TONALITY_QUOTA*bufT[maxPosT][maxPosF]){

          bufComp[band+1] = compValue;
        }
      }
    }

    /* intermediate band: (0, nBand)  */
    if(band && band < nBand - 1){

      /* [-1] */
      compValue = (Ipp32s) (fabs(ILOG2*log(bufDiff[maxPosT][band -1]+EPS)) + 0.5f);

      compValue = IPP_MIN( compValue, MAX_COMP );

      curThres = bufDiff[maxPosT][band]*bufDiff[maxPosT][band-1];
      curThres *= DIFF_QUOTA;

      if(1.0f > curThres){
        bufComp[band-1] = -1*compValue;
      }

      /* [+1] */
      compValue = (Ipp32s) (fabs(ILOG2*log(bufDiff[maxPosT][band + 1]+EPS)) + 0.5f);

      compValue = IPP_MIN( compValue, MAX_COMP );

      curThres = bufDiff[maxPosT][band]*bufDiff[maxPosT][band+1];
      curThres *= DIFF_QUOTA;

      if(1.0f > curThres ){
        bufComp[band+1] = compValue;
      }
    }
  }

  return 0; //OK
}

/**************************************************************************/

static Ipp32s
sbrencCompensationCorrect(Ipp32s* bufComp, Ipp32s*  bufCompPrev, Ipp32s nBand)
{
  Ipp32s band;

  for(band = 0; band < nBand; band++){
    if( (0 == bufCompPrev[band]) && (0 != bufComp[band]) ) {
      bufComp[band] = 0;
    }
  }

  return 0;//OK
}

/**************************************************************************/

static Ipp32s
sbrencIsHarmonicAddition(Ipp32s* bs_add_harmonic, Ipp32s nBand)
{
  Ipp32s i;

  for(i = 0; i < nBand; i++){
    if (bs_add_harmonic[i]){
      return bs_add_harmonic[i];
    }
  }

  return 0;
}

/**************************************************************************/

Ipp32s sbrencSinEstimation(sSBREnc_SCE_State* pState, sSBRFeqTabsState* pFreqTabsState)
{
  //sSBRFeqTabsState* pFreqTabsState = (pState->sbrFreqTabsState);
  sSBRSinEst* pSinEst = &( pState->sbrSinEst );

  Ipp32s* bufComp      = pSinEst->bufCompensation;
  Ipp32s* bufCompPrev  = pSinEst->bufCompensationPrev;
  Ipp32s* harmonicFlag = &(pState->sbrEDState.bs_add_harmonic_flag);

  Ipp32s* prevTranFrame = &(pState->sbrTransientState.prevTranFrame);
  Ipp32s* prevTranPos   = &(pState->sbrTransientState.prevTranPos);
  Ipp32s* prevTranFlag  = &(pState->sbrTransientState.prevTranFlag);

  Ipp32s nBand = pFreqTabsState->nHiBand;

  Ipp32s tranFlag = pState->sbrTransientState.tranFlag;
  Ipp32s tranPos  = pState->sbrTransientState.tranPos;

  Ipp32s tranPosOffset = 4;

  Ipp32s isDetectNewTone = 0;


  Ipp32s band;
  Ipp32s criterion1, criterion2, criterion3;

  /*
  sbrencCalcInDataSinEst( pState->bufT,
                           pSinEst->bufDiff,
                           pSinEst->bufSfmOrig,
                           pSinEst->bufSfmSBR,
                           pState->tabPatchMap,
                           pState->fHiBandTab,
                           pState->nHiBand );
  */

  isDetectNewTone = sbrencIsDetectNewToneAllow(&(pState->sbrFIState),

                                               prevTranFrame,
                                               prevTranPos,
                                               prevTranFlag,

                                               tranPosOffset,
                                               tranFlag,
                                               tranPos);

  /* AYA log */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "\isDetection\nprevTranFlag = %i, prevTranFrame = %i, prevTranPos = %i\n",
                   *prevTranFlag, *prevTranFrame, *prevTranPos);

  fprintf(logFile, "newDetection = %i\n", isDetectNewTone);
#endif

  sbrencCalcInDataSinEst(pState->bufT,
                         pSinEst->bufDiff,
                         pSinEst->bufSfmOrig,
                         pSinEst->bufSfmSBR,
                         pState->tabPatchMap,
                         pFreqTabsState->fHiBandTab,
                         nBand);

  sbrencTotalDetectionSinEst(pState->bufT,
                             pSinEst->bufDiff,
                             nBand,
                             pFreqTabsState->fHiBandTab,
                             pSinEst->bufSfmOrig,
                             pSinEst->bufSfmSBR,

                             pSinEst->bufDetection,

                             pSinEst->guideScfb,
                             pSinEst->sbrGuideState,

                             2, //Ipp32s noEstPerFrame,
                             4, //Ipp32s totNoEst,
                             isDetectNewTone,
                             pState->sbrEDState.bs_add_harmonic);

  /* AYA log */
#ifdef SBR_NEED_LOG
  {
    Ipp32s i;

    fprintf(logFile, "\naddHarmonic\n");
    for(i=0; i<pFreqTabsState->nHiBand; i++){
      fprintf(logFile, "%i\n", pState->sbrEDState.bs_add_harmonic[i]);
    }
  }
#endif

  sbrencCalcCompensation(pState->bufT,
                         pSinEst->bufDiff,
                         pFreqTabsState->fHiBandTab,
                         nBand,
                         pState->sbrEDState.bs_add_harmonic,
                         bufComp,
                         4);

  /* patch */
  if( !isDetectNewTone ){
    sbrencCompensationCorrect(bufComp, bufCompPrev, nBand);
  }

  *harmonicFlag = sbrencIsHarmonicAddition( pState->sbrEDState.bs_add_harmonic, nBand );

  /* buffer up-date */
  ippsCopy_32s(bufComp, bufCompPrev, nBand);
  ippsCopy_32s(pState->sbrEDState.bs_add_harmonic, pSinEst->guideScfb, nBand);
  ippsCopy_32s(pState->sbrEDState.bs_add_harmonic, pSinEst->sbrGuideState[0].bufGuideDetect, nBand);

  ippsCopy_32f(pSinEst->sbrGuideState[2].bufGuideDiff, pSinEst->sbrGuideState[0].bufGuideDiff, nBand);
  ippsCopy_32f(pSinEst->sbrGuideState[2].bufGuideOrig, pSinEst->sbrGuideState[0].bufGuideOrig, nBand);

  for(band = 0; band < nBand; band++){

    criterion1 = (Ipp32s)pSinEst->sbrGuideState[0].bufGuideDiff[band];
    criterion2 = (Ipp32s)pSinEst->sbrGuideState[0].bufGuideOrig[band];
    criterion3 = (Ipp32s)pState->sbrEDState.bs_add_harmonic[band];

    if((criterion1 || criterion2) && !criterion3){
        pSinEst->sbrGuideState[0].bufGuideDiff[band] = 0;
        pSinEst->sbrGuideState[0].bufGuideOrig[band] = 0;
    }
  }

  /* AYA log */
#ifdef SBR_NEED_LOG
  {
    Ipp32s i;

    fprintf(logFile, "\nCompensation\n");
    for(i=0; i<nBand; i++){
      fprintf(logFile, "%i\n", pSinEst->bufCompensation[i]);
    }
  }
#endif

  return 0; //OK
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

