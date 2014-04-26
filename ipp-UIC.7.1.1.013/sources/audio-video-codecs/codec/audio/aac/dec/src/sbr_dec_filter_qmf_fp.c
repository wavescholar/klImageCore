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

#include <math.h>
#include "aaccmn_chmap.h"
#include "sbr_settings.h"
#include "aac_dec_tns_fp.h"
#include "aac_dec_own_fp.h"

/********************************************************************/

Ipp32s sbrdecGetFilterSize(Ipp32s* pFilterSize)
{
  Ipp32s SizeSpecHQ, SizeInitBufHQ, SizeWorkBufHQ;
  Ipp32s SizeSpecLP, SizeInitBufLP, SizeWorkBufLP;
  IppStatus ippRes;

  /* -------------------------------- Analysis_SBR ---------------------------- */
  ippRes = ippsAnalysisFilterGetSize_SBR_RToR_32f(&SizeSpecLP, &SizeInitBufLP,
                                                  &SizeWorkBufLP);
  if (ippRes != ippStsNoErr)
    return 1;
  ippRes = ippsAnalysisFilterGetSize_SBR_RToC_32f32fc(&SizeSpecHQ, &SizeInitBufHQ,
                                                      &SizeWorkBufHQ);
  if (ippRes != ippStsNoErr)
    return 1;

  pFilterSize[0] = MAX(SizeSpecLP, SizeSpecHQ);
  pFilterSize[1] = MAX(SizeInitBufLP, SizeInitBufHQ);
  pFilterSize[6] = MAX(SizeWorkBufLP, SizeWorkBufHQ);

  /* -------------------------------- Synthesis_SBR ---------------------------- */
  ippRes = ippsSynthesisFilterGetSize_SBR_RToR_32f(&SizeSpecLP, &SizeInitBufLP,
                                                   &SizeWorkBufLP);
  if (ippRes != ippStsNoErr)
    return 1;

  pFilterSize[6] = MAX(SizeWorkBufLP, pFilterSize[6]);

  ippRes = ippsSynthesisFilterGetSize_SBR_CToR_32fc32f(&SizeSpecHQ, &SizeInitBufHQ,
                                                       &SizeWorkBufHQ);
  if (ippRes != ippStsNoErr)
    return 1;

  pFilterSize[2] = MAX(SizeSpecLP, SizeSpecHQ);
  pFilterSize[3] = MAX(SizeInitBufLP, SizeInitBufHQ);
  pFilterSize[6] = MAX(SizeWorkBufHQ, pFilterSize[6]);

  /* -------------------------------- Synthesis Down SBR ---------------------------- */
  ippRes = ippsSynthesisDownFilterGetSize_SBR_RToR_32f(&SizeSpecLP, &SizeInitBufLP,
                                                       &SizeWorkBufLP);
  if (ippRes != ippStsNoErr)
    return 1;

  pFilterSize[6] = MAX(SizeWorkBufLP, pFilterSize[6]);

  ippRes = ippsSynthesisDownFilterGetSize_SBR_CToR_32fc32f(&SizeSpecHQ, &SizeInitBufHQ,
                                                           &SizeWorkBufHQ);
  if (ippRes != ippStsNoErr)
    return 1;

  pFilterSize[4] = MAX(SizeSpecLP, SizeSpecHQ);
  pFilterSize[5] = MAX(SizeInitBufLP, SizeInitBufHQ);
  pFilterSize[6] = MAX(SizeWorkBufHQ, pFilterSize[6]);

  return 0;
}

/********************************************************************/

void sbrdecDrawMemMap(sSbrDecFilter* pDC[CH_MAX], Ipp8u* pMem, Ipp32s* pSizes)
{
  Ipp32s i;

  for (i = 0; i < CH_MAX; i++){
    /* for Analysis */
    if (pSizes[0] != 0) {
      pDC[i]->pQmfMemSpec[0][0] = pMem;
      pDC[i]->pQmfMemSpec[0][1] = pMem + pSizes[0];
    } else {
      pDC[i]->pQmfMemSpec[0][0] = NULL;
      pDC[i]->pQmfMemSpec[0][1] = NULL;
    }
    pMem += pSizes[0] + pSizes[0];

    if (pSizes[1] != 0) {
      pDC[i]->pInitBuf[0] = pMem;
    } else {
      pDC[i]->pInitBuf[0] = NULL;
    }
    pMem += pSizes[1];
    /* for Synthesis */
    if (pSizes[2] != 0) {
      pDC[i]->pQmfMemSpec[1][0] = pMem;
      pDC[i]->pQmfMemSpec[1][1] = pMem + pSizes[2];
    } else {
      pDC[i]->pQmfMemSpec[1][0] = NULL;
      pDC[i]->pQmfMemSpec[1][1] = NULL;
    }
    pMem += pSizes[2] + pSizes[2];

    if (pSizes[3] != 0) {
      pDC[i]->pInitBuf[1] = pMem;
    } else {
      pDC[i]->pInitBuf[1] = NULL;
    }
    pMem += pSizes[3];
    /* for SynthesisDown */
    if (pSizes[4] != 0) {
      pDC[i]->pQmfMemSpec[2][0] = pMem;
      pDC[i]->pQmfMemSpec[2][1] = pMem + pSizes[4];
    } else {
      pDC[i]->pQmfMemSpec[2][0] = NULL;
      pDC[i]->pQmfMemSpec[2][1] = NULL;
    }
    pMem += pSizes[4] + pSizes[4];

    if (pSizes[5] != 0) {
      pDC[i]->pInitBuf[2] = pMem;
    } else {
      pDC[i]->pInitBuf[2] = NULL;
    }
    pMem += pSizes[5];
  }
}

/********************************************************************/

void sbrdecUpdateMemMap(AACDec *state, Ipp32s mShift)
{
  Ipp32s i, ch, j;
  sSbrDecFilter* sbr_filter[CH_MAX];
  sSBRBlock*     sbr_block[CH_MAX];

  for (i = 0; i < CH_MAX; i++) {
    sbr_filter[i] = &(state->sbr_filter[i]);
    sbr_block[i]  = &(state->sbrBlock[i]);
  }

  for (i = 0; i < CH_MAX; i++){
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pQmfMemSpec[0][0], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pQmfMemSpec[0][1], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pInitBuf[0], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pQmfMemSpec[1][0], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pQmfMemSpec[1][1], mShift)

    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pInitBuf[1], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pQmfMemSpec[2][0], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pQmfMemSpec[2][1], mShift)
    AACDEC_UPDATE_PTR(Ipp8u, sbr_filter[i]->pInitBuf[2], mShift)

    for( ch = 0; ch < 2; ch++ ){
      for (j = 0; j < (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN); j++){
        AACDEC_UPDATE_PTR(Ipp32f, sbr_block[i]->wsState.XBuf[ch][j], mShift)
        AACDEC_UPDATE_PTR(Ipp32f, sbr_block[i]->wsState.YBuf[ch][j], mShift)
        AACDEC_UPDATE_PTR(Ipp32f, sbr_block[i]->wsState.ZBuf[ch][j], mShift)
      }
    }
  }

  return;//OK
}

/********************************************************************/

Ipp32s sbrdecInitFilter(AACDec *pState)
{
  Ipp32s i, j;
  IppStatus ippRes;
  sSbrDecFilter *pFilter;

  for (i = 0; i < CH_MAX; i++){
    pFilter = &(pState->sbr_filter[i]);
    for(j = 0; j < 2; j++){
      pFilter->pAnalysisFilterSpec[j] = pFilter->pQmfMemSpec[0][j];
      if (pState->com.ModeDecodeHEAACprofile == HEAAC_HQ_MODE) {
        ippRes = ippsAnalysisFilterInit_SBR_RToC_32f32fc(
          (IppsFilterSpec_SBR_C_32fc **)&(pFilter->pAnalysisFilterSpec[j]),
                                          pFilter->pQmfMemSpec[0][j],
                                          pFilter->pInitBuf[0]);
        if (ippRes != ippStsNoErr)
          return 1;
        pFilter->pSynthesisFilterSpec[j] = pFilter->pQmfMemSpec[1][j];
        ippRes =  ippsSynthesisFilterInit_SBR_CToR_32fc32f(
          (IppsFilterSpec_SBR_C_32fc **)&(pFilter->pSynthesisFilterSpec[j]),
                                          pFilter->pQmfMemSpec[1][j],
                                          pFilter->pInitBuf[1]);
        if (ippRes != ippStsNoErr)
          return 1;
        pFilter->pSynthesisDownFilterSpec[j] = pFilter->pQmfMemSpec[2][j];
        ippRes = ippsSynthesisDownFilterInit_SBR_CToR_32fc32f(
          (IppsFilterSpec_SBR_C_32fc **)&(pFilter->pSynthesisDownFilterSpec[j]),
                                          pFilter->pQmfMemSpec[2][j],
                                          pFilter->pInitBuf[2]);
        if (ippRes != ippStsNoErr)
          return 1;
      } else {
        pFilter->pAnalysisFilterSpec[j] = pFilter->pQmfMemSpec[0][j];
        ippRes = ippsAnalysisFilterInit_SBR_RToR_32f(
          (IppsFilterSpec_SBR_R_32f **)&(pFilter->pAnalysisFilterSpec[j]),
                                         pFilter->pQmfMemSpec[0][j],
                                         pFilter->pInitBuf[0]);
        if (ippRes != ippStsNoErr)
          return 1;
        pFilter->pSynthesisFilterSpec[j] = pFilter->pQmfMemSpec[1][j];
        ippRes = ippsSynthesisFilterInit_SBR_RToR_32f(
          (IppsFilterSpec_SBR_R_32f **)&(pFilter->pSynthesisFilterSpec[j]),
                                         pFilter->pQmfMemSpec[1][j],
                                         pFilter->pInitBuf[1]);
        if (ippRes != ippStsNoErr)
          return 1;
        pFilter->pSynthesisDownFilterSpec[j] = pFilter->pQmfMemSpec[2][j];
        ippRes = ippsSynthesisDownFilterInit_SBR_RToR_32f(
          (IppsFilterSpec_SBR_R_32f **)&(pFilter->pSynthesisDownFilterSpec[j]),
                                         pFilter->pQmfMemSpec[2][j],
                                         pFilter->pInitBuf[2]);
        if (ippRes != ippStsNoErr)
          return 1;
      }
    }
  }

  return 0;
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

