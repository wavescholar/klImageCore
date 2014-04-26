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
#include "aaccmn_const.h"
#include "aac_status.h"
#include "sbr_freq_tabs.h"
#include "sbr_huff_tabs.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"
#include "vm_strings.h"

/* AYALog */
#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/********************************************************************/

#ifndef UNDEF_SBR_FREQ_IDX
#define UNDEF_SBR_FREQ_IDX -999
#endif

/********************************************************************/

AACStatus sbrencCheckParams(Ipp32s sampleRate,
                            Ipp32s numCh,
                            Ipp32s bitRate,
                            Ipp32s* indx)
{
  AACStatus status = AAC_OK;

  /* check of frequency (half from inFreqSample) */
  if( (2*sampleRate < MIN_SBR_ENC_FREQ) || (2*sampleRate > MAX_SBR_ENC_FREQ) ){
    return AAC_UNSUPPORTED;
  }

  /* check of channel */
  if( numCh > SBR_MAX_CH ){
    return AAC_UNSUPPORTED;
  }

  //-------------------
  switch( numCh ) {
  case 1:
    status = sbrencGetTuningTab(sampleRate, ID_SCE, bitRate, indx);
    break;

  case 2:
    status = sbrencGetTuningTab(sampleRate, ID_CPE, bitRate, indx);
    break;

  /* may be inserted more detailed analysis, but in my opinion it will be sybaritism */
  default:
    bitRate = bitRate / numCh ;
    status = sbrencGetTuningTab(sampleRate, ID_SCE, bitRate, indx);
    if ( AAC_OK != status ) return status;

/* support asymmetric cutoff freq is turned OFF  */
    indx[1] = indx[0];

      break;
  }
  //-------------------

  return status;
}

/********************************************************************
 *
 * this table from coreAAC ( "aac_dec_api.c" )
 * if tables has been changed then SBR will work incorrect
 *
 *  static Ipp32s sampling_frequency_table[] = {
 *    96000, 88200, 64000, 48000, 44100, 32000, 24000,
 *    22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0
 *  };
 *
 ********************************************************************/

static Ipp32s sampling_frequency_table[] = {
  96000, 88200, 64000, 48000, 44100, 32000, 24000,
  22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0
};

static Ipp32s sbrencGetSbrFreqIndx(Ipp32s sampleRate)
{
  Ipp32s indx = UNDEF_SBR_FREQ_IDX;
  Ipp32s i;

  for(i=0; i<16; i++){
    if( sampling_frequency_table[i] ==  sampleRate  ){
      indx = i;
      break;
    }
  }

  return indx;
}

/********************************************************************/

static Ipp32f sbrencGetSplitThr(Ipp32s sampleFreq)
{
  const Ipp32s frameSize = 2048;
  Ipp32f frameDur = 0.0f;
  Ipp32f frameDur001 = 0.0f;
  Ipp32f splitThr = 0.0f;
  Ipp32f ratioBitRates = 1.0f; // ratioBitRates = totalBitRate / codecBitRate

  frameDur = (Ipp32f)frameSize / (Ipp32f)sampleFreq;

  frameDur001 = frameDur - 0.01f;

  frameDur001 = ( frameDur001 < 0.01f ) ? 0.01f : frameDur001;

  splitThr = ratioBitRates * 7.5e-5f * 1.f / ( frameDur001 * frameDur001 );

  return splitThr;
}

/********************************************************************/

AACStatus sbrencReset(sSBREncState* pState, Ipp32s* indxTuningTab)
{
  sSBRFeqTabsState* pFreqTabsState = (pState->sbrFreqTabsState);
  sSBREnc_SCE_State* pSCE_Element = (pState->pSCE_Element);
  sSBRHeader* pHeader = (pState->sbrHeader);
  Ipp32s* indx_CE_Tab = (pState->indx_CE_Tab);

  Ipp32s indx = 0;//indxTuningTab; /* here indx MUST be correct */
  Ipp32s k0 = 0, k2 = 0, kx = 0, M = 0;
  Ipp32s error = 0;
  Ipp32s sampleRate = 0;
  Ipp32s nf_max_level = 1;

  Ipp32s bs_xover_band = 0; /* may be corrected by usr */
  Ipp32s i = 0;
  Ipp32s ch = 0;

#if !defined(ANDROID)
  Ipp32s tabPatchMap[2][ 64 ];
#else
  static Ipp32s tabPatchMap[2][ 64 ];
#endif

  /* offset for any CE is 0: default */
  pState->indx_FreqOffset_Tab[ID_SCE] = 0;
  pState->indx_FreqOffset_Tab[ID_CPE] = 0;

  //------------------------------------------------------------------------------
  //while ( EMPTY_MAPPING != indx_CE_Tab[numCh++] )

  //------------------------------------------------------------------------------
  indx = indxTuningTab[0]; // nonchalant
  /* MAIN: get correct sbr_freq_indx */
  sampleRate = SBR_TUNING_TABS[indx].sampleRate << 1; // because sample rate is core aac sample rate

  pState->sbrFreqIndx = sbrencGetSbrFreqIndx( sampleRate );

  /* calculate split threshold */
  pState->splitThr = sbrencGetSplitThr( sampleRate );

  if (UNDEF_SBR_FREQ_IDX == pState->sbrFreqIndx ){
    return HEAAC_UNSUPPORTED;
  }

  //------------------------------------------------------------------------------
  i = 0;
  while( EMPTY_MAPPING != indxTuningTab[i] ){
    /* HEADER: up-date init params of freq tabs */
    sSBRFeqTabsState* pCurFreqTabsState = pFreqTabsState + i;
    sSBRHeader* pCurHeader = pHeader + i;
    Ipp32s indx = indxTuningTab[i];

    pCurHeader->bs_start_freq  = SBR_TUNING_TABS[indx].bs_start_freq;
    pCurHeader->bs_stop_freq   = SBR_TUNING_TABS[indx].bs_stop_freq;

    /* bs_header_extra_1 ? */
    pCurHeader->bs_extra_1 = 0;

    pCurHeader->bs_noise_bands = SBR_TUNING_TABS[indx].bs_noise_bands;
    pCurHeader->bs_freq_scale  = SBR_TUNING_TABS[indx].bs_freq_scale;
    pCurHeader->bs_alter_scale = BS_ALTER_SCALE_DEFAULT;
    /* CHECK: DEFAULT/ UNDEFAULT ? */
    if (pCurHeader->bs_noise_bands != BS_NOISE_BANDS_DEFAULT ||
        pCurHeader->bs_freq_scale  != BS_FREQ_SCALE_DEFAULT  ||
        pCurHeader->bs_alter_scale != BS_ALTER_SCALE_DEFAULT) {

        pCurHeader->bs_extra_1 = 1;
    }

    /* calculation number of header send */
    pCurHeader->nSendSBRHeader = (Ipp32s)(SBR_HEADER_SEND_TIME * 0.001f * sampleRate / 2048.0f);
    pCurHeader->nSendSBRHeader = IPP_MAX(pCurHeader->nSendSBRHeader, 1);

    pCurHeader->cntSentSBRHeader = 0;
    pCurHeader->flag_SBRHeaderActive = 0;

    /* header extra params */
    /* bs_header_extra_2 ? */
    pCurHeader->bs_extra_2 = 0;

    pCurHeader->bs_limiter_bands = BS_LIMITER_BANDS_DEFAULT;
    pCurHeader->bs_limiter_gains = BS_LIMITER_GAINS_DEFAULT;
    pCurHeader->bs_interpol_freq = BS_INTERPOL_FREQ_DEFAULT;
    pCurHeader->bs_smoothing_mode= BS_SMOOTHING_MODE_DEFAULT;

    if (pCurHeader->bs_limiter_bands  != BS_LIMITER_BANDS_DEFAULT ||
        pCurHeader->bs_limiter_gains  != BS_LIMITER_GAINS_DEFAULT ||
        pCurHeader->bs_interpol_freq  != BS_INTERPOL_FREQ_DEFAULT ||
        pCurHeader->bs_smoothing_mode != BS_SMOOTHING_MODE_DEFAULT) {

        pCurHeader->bs_extra_2 = 1;
    }

    /* header reset: internal default */
    pCurHeader->bs_amp_res = 1; //3dB

  //}

    //------------------------------------------------------------------------------
    /* FREQ TABS: up-date freq tabs */
    error = sbrCalcMasterFreqBoundary(pCurHeader->bs_start_freq,
                                      pCurHeader->bs_stop_freq,
                                      pState->sbrFreqIndx,
                                      &k0,
                                      &k2);

    if ( error ){
      return HEAAC_UNSUPPORTED;
    }

    error = sbrCalcMasterFreqBandTab(k0,
                                     k2,
                                     pCurHeader->bs_freq_scale,
                                     pCurHeader->bs_alter_scale,
                                     pCurFreqTabsState->fMasterBandTab,
                                     &(pCurFreqTabsState->nMasterBand));

    if ( error ){
      return HEAAC_UNSUPPORTED;
    }

    error = sbrCalcHiFreqTab(pCurFreqTabsState->fMasterBandTab,
                             pCurFreqTabsState->nMasterBand, bs_xover_band,
                             pCurFreqTabsState->fHiBandTab,
                             &(pCurFreqTabsState->nHiBand));
    if ( error ){
      return HEAAC_UNSUPPORTED;
    }

    kx = pCurFreqTabsState->fHiBandTab[ 0 ];
    M = pCurFreqTabsState->fHiBandTab[ pCurFreqTabsState->nHiBand ] - pCurFreqTabsState->fHiBandTab[ 0 ];

    sbrCalcLoFreqTab(pCurFreqTabsState->fHiBandTab,
                     pCurFreqTabsState->nHiBand,
                     pCurFreqTabsState->fLoBandTab,
                     &(pCurFreqTabsState->nLoBand));

    error = sbrCalcNoiseTab(pCurFreqTabsState->fLoBandTab,
                            pCurFreqTabsState->nLoBand,
                            pCurHeader->bs_noise_bands,
                            k2,
                            kx,
                            pCurFreqTabsState->fNoiseBandTab,
                            &(pCurFreqTabsState->nNoiseBand));
    if ( error ){
      return HEAAC_UNSUPPORTED;
    }

    error = sbrCalcPatchConstruct_indxTab(pCurFreqTabsState->fMasterBandTab,
                                          pCurFreqTabsState->nMasterBand,
                                          k0,
                                          M,
                                          pState->sbrFreqIndx,
                                          tabPatchMap[i]);
    if ( error ){
      return HEAAC_UNSUPPORTED;
    }

    /* correct offset for freq tab */
    pState->indx_FreqOffset_Tab[i] = i;

    //increment
    i++;
  }
  //------------------------------------------------------------------------------
  ch = 0;

  /* SCE init */
  while(EMPTY_MAPPING != indx_CE_Tab[ch]){
    Ipp32s indx = pState->indx_FreqOffset_Tab[ indx_CE_Tab[ch] ];
    sSBRFeqTabsState* pCurFreqTabsState = pFreqTabsState + indx;
    sSBREnc_SCE_State* pCurSCE_Element = pSCE_Element + ch;

    pCurSCE_Element->bs_amp_res = 1; //3dB

    /* patch */
    ippsCopy_32s(tabPatchMap[indx], pCurSCE_Element->tabPatchMap, 64);

    /* invf estimation init */
    for(i = 0; i < pCurFreqTabsState->nNoiseBand; i++){

      pCurSCE_Element->sbrInvfEst.prev_bs_invf_mode[i]   = INVF_OFF_LEVEL;
      pCurSCE_Element->sbrInvfEst.prevRegionOrig[i] = 0;
      pCurSCE_Element->sbrInvfEst.prevRegionSBR[i]  = 0;
    }

    /* noisef estimation state */
    nf_max_level = SBR_TUNING_TABS[indx].nf_max_level;
    pCurSCE_Element->sbrNoiseEst.nf_max_level = (Ipp32f)pow(2, nf_max_level/3.0f);

    pCurSCE_Element->sbrEDState.flagEnvUpDate   = 0;
    pCurSCE_Element->sbrEDState.flagNoiseUpDate = 0;

    /* addition initialization */
    pCurSCE_Element->sbrTransientState.flagTransferTran = 0;

    ch++;

    /* patch */
    if ( ID_LFE == indx_CE_Tab[ch]) {
      return AAC_OK;
    }
  }
  return AAC_OK;
}

/********************************************************************/

static Ipp32s
sbrencUpDateAmpRes(Ipp32s nEnv, Ipp32s frameClass)
{
  Ipp32s ampRes = 1; // 3.0dB

  if( (FIXFIX == frameClass) && (1 == nEnv) ){
    ampRes = 0;// 1.5dB
  }

  return ampRes;
}

/********************************************************************/

AACStatus sbrencGetFrame(sSBREncState* pState, ownFilterSpec_SBR_C_32fc* pSpec)
{
  AACStatus status = AAC_OK;

  /* common part */
  Ipp32s offset = pState->sbr_offset;
  Ipp32s indx = pState->indx_FreqOffset_Tab[ pState->indx_CE_Tab[offset] ];
  Ipp32s ch;

  sSBRFeqTabsState* pFreqTabsState = (pState->sbrFreqTabsState + indx);
  sSBRHeader* pHeader = (pState->sbrHeader + indx);

  sSBREnc_SCE_State* pRoot_SCE_Element = ( pState->pSCE_Element + offset);

#if !defined(ANDROID)
  Ipp32s pHiFreqResTab[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

  Ipp32s nBand[2];
  Ipp32s nNoiseBand[2];

  Ipp32s* pFreqTabs[2];

  Ipp32s bs_invf_mode[MAX_NUM_NOISE_BANDS];
#else
  static Ipp32s pHiFreqResTab[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

  static Ipp32s nBand[2];
  static Ipp32s nNoiseBand[2];

  static Ipp32s* pFreqTabs[2];

  static Ipp32s bs_invf_mode[MAX_NUM_NOISE_BANDS];
#endif

  Ipp32s curr_amp_res = 0;

  Ipp32s i = 0;
  Ipp32s tranPos = 0, tranFlag = 0;
  Ipp32s splitFlag = 0;
  Ipp32s lastBand = 0; //kx + M

  Ipp32s flagTranForInvfEst    = 0;
  Ipp32s flagTranForNoisefEst  = 0;

  Ipp32s flagResetNeed = 0;

  if (pFreqTabsState->nHiBand < 0 || pFreqTabsState->nHiBand >= MAX_SIZE_FREQ_TABS) 
      return AAC_BAD_PARAMETER;

  /* ******************************************************************* */
  /*                   update flag_header                                */
  /* this step need for synchronization other device (for ex DRadioM)    */
  /* every 500 mls header is sent                                        */
  /* ******************************************************************* */

  pHeader->flag_SBRHeaderActive = 0;

  if ( 0 == pHeader->cntSentSBRHeader ) {
    pHeader->flag_SBRHeaderActive = 1;
  }

  pHeader->cntSentSBRHeader++;
  pHeader->cntSentSBRHeader %= pHeader->nSendSBRHeader;
  //----------------------------------------------------

  /* ********************************************************************* */
  /*                  START MAIL LOOP                                      */
  /* ********************************************************************* */

  for( ch = 0; ch < pState->indx_CE_Tab[offset] + 1; ch++ ){

    /* init structures */
    sSBREnc_SCE_State* pSCE_Element = pRoot_SCE_Element + ch;
    sSBRFrameInfoState* pFIState = &( pSCE_Element->sbrFIState );
    sSBREnvDataState* pEDState = &( pSCE_Element->sbrEDState );

    Ipp32f* pSrc = pSCE_Element->sbrInputBuffer;


    Ipp16s* bufEnvQuant   = pEDState->bufEnvQuant;
    Ipp16s* bufNoiseQuant = pEDState->bufNoiseQuant;

    Ipp16s* bufEnvQuantPrev = pEDState->bufEnvQuantPrev;
    Ipp32s* nEnv = &( pFIState->nEnv );

    Ipp32s  nNoiseEnv   = *nEnv > 1 ? 2 : 1;
    Ipp16s* bufNoiseQuantPrev = pEDState->bufNoiseQuantPrev;

    /* reset vals */
    i = 0;
    tranPos = 0;
    tranFlag = 0;
    splitFlag = 0;
    lastBand = 0; //kx + M

    flagTranForInvfEst    = 0;
    flagTranForNoisefEst  = 0;

    /* analysis */
    for(i=0; i<32; i++){
      ownAnalysisFilter_SBREnc_RToC_32f32fc(pSrc + 64*i, pSCE_Element->bufX[i], pSpec);
    }

    /* calc energy of signal: history up-date here */
    sbrencCalcNrg(pSCE_Element->bufX, pSCE_Element->bufXFlip, pSCE_Element->bufNrg, pSCE_Element->bufNrgFlip);

    /*  */
    lastBand = pFreqTabsState->fHiBandTab[ pFreqTabsState->nHiBand ];

    /* TNR estimation */
    ippsCopy_32f(pSCE_Element->bufT[2], pSCE_Element->bufT[0], 128);

    pSCE_Element->bufTNrg[0] = pSCE_Element->bufTNrg[1];
    pSCE_Element->bufTNrg[1] = 0.f;

    for(i = 0; i < lastBand; i++){
      Ipp32f meanNrg = 0.f;

      ownEstimateTNR_SBR_32f(pSCE_Element->bufXFlip[i],
                            &(pSCE_Element->bufT[2][i]),
                            &(pSCE_Element->bufT[3][i]), &meanNrg );

      pSCE_Element->bufTNrg[1] += meanNrg;
    }

    /* transient detector */
    sbrencTransientDetector(pSCE_Element->bufNrgFlip,
                            pSCE_Element->sbrTransientState.threshold,
                            pSCE_Element->sbrTransientState.transient,
                            &tranPos, &tranFlag);

    /* frame splitter */
    if ( 0 == tranFlag ){
      splitFlag = sbrencFrameSplitter(pSCE_Element->bufNrg,
                                      pFreqTabsState->fHiBandTab,
                                      pFreqTabsState->nHiBand,
                                      &(pSCE_Element->sbrTransientState.nrgPrevLow),
                                      pState->splitThr);

  #ifdef SBR_NEED_LOG
      fprintf(logFile, "splitFlag = %i\n", splitFlag);
  #endif
    } else {
  #ifdef SBR_NEED_LOG
      fprintf(logFile, "splitFlag = %i\n", tranPos);
  #endif
    }

    /* frame generator */
    pSCE_Element->sbrTransientState.tranPos  = tranPos;
    pSCE_Element->sbrTransientState.tranFlag = tranFlag;
    /* AYA patch */
    if (tranFlag == 0) {
      pSCE_Element->sbrTransientState.tranPos = splitFlag;
    }
    sbrencFrameGenerator(pSCE_Element, splitFlag);

  //-----------------------------------------------------------------------------
    /* duty code: MAY BE POTENCIAL ERROR, WHEN DIFFERENT AMP_RES IS CALCULATED */
    /* correct bs_amp_res */
    curr_amp_res = sbrencUpDateAmpRes( *nEnv, pSCE_Element->sbrGrid.frameClass );

    if( pSCE_Element->bs_amp_res != curr_amp_res ){
    //if ( pHeader->bs_amp_res != curr_amp_res ) {

     // if( ch > 0 ){
     //   vm_string_printf(VM_STRING("sbr err: CONFLICT WITH AMP_RES\n"));
     // }

      pSCE_Element->bs_amp_res = curr_amp_res;
      //pHeader->bs_amp_res = curr_amp_res;

      pEDState->flagEnvUpDate   = 0;
      pEDState->flagNoiseUpDate = 0;

      /* patch */
      flagResetNeed++;
    }
  //-----------------------------------------------------------------------------
    /* set flags */
    flagTranForNoisefEst = 0;
    //pState->flagTransferTran

    if(pSCE_Element->sbrTransientState.flagTransferTran){
      flagTranForNoisefEst = 1;
      pSCE_Element->sbrTransientState.flagTransferTran = 0;

      if(tranFlag){
        if(tranPos + 4 >= pFIState->bordersEnv[pFIState->nEnv]){
          pSCE_Element->sbrTransientState.flagTransferTran = 1;
        }
      }
    } else {
      if(tranFlag){
        if(tranPos + 4 < pFIState->bordersEnv[pFIState->nEnv]){

          flagTranForNoisefEst = 1;
          pSCE_Element->sbrTransientState.flagTransferTran = 0;
        }else{
          pSCE_Element->sbrTransientState.flagTransferTran = 1;
        }
      }
    }

    flagTranForInvfEst = pSCE_Element->sbrTransientState.flagTransferTran;

  //---------------------------------

    /* inv filtering estimation */
    sbrencInvfEstimation (&(pSCE_Element->sbrInvfEst),
                          pSCE_Element->bufT,
                          pSCE_Element->bufTNrg,
                          pSCE_Element->tabPatchMap,
                          flagTranForInvfEst,
                          pFreqTabsState->fNoiseBandTab,
                          pFreqTabsState->nNoiseBand,
                          bs_invf_mode);


    sbrencSinEstimation(pSCE_Element, pFreqTabsState);

    sbrencNoisefEstimation(pSCE_Element,
                           pFreqTabsState->fNoiseBandTab,
                           pFreqTabsState->nNoiseBand,
                           bufNoiseQuant);

    /* up-date */
    for(i = 0; i < pFreqTabsState->nNoiseBand; i++){
      pSCE_Element->sbrInvfEst.bs_invf_mode[i] = bs_invf_mode[i];
    }

    /* data */
    nBand[0] = pFreqTabsState->nLoBand;
    nBand[1] = pFreqTabsState->nHiBand;

    pFreqTabs[0] = pFreqTabsState->fLoBandTab;
    pFreqTabs[1] = pFreqTabsState->fHiBandTab;

    /* AYA log */
    //printf("\namp_res = %i\n", pState->bs_amp_res);

    sbrencEnvEstimation(pSCE_Element->bufNrg,
                        &(pSCE_Element->sbrFIState),
                        bufEnvQuant,
                        nBand,//Ipp32s    nBand[2],
                        pFreqTabs, //Ipp32s*   pFreqTabs[2],
                        curr_amp_res,//pHeader->bs_amp_res, //Ipp32s    amp_res,
                        pEDState->bs_add_harmonic, //Ipp32s*   add_harmonic,
                        pEDState->bs_add_harmonic_flag);

    /* *********** start noise coding **************** */
    /* up-date */
    nNoiseEnv   = *nEnv > 1 ? 2 : 1;

    /* data */
    nNoiseBand[0] = pFreqTabsState->nNoiseBand;
    nNoiseBand[1] = pFreqTabsState->nNoiseBand;

    /* AYAlog: Ipp32s data (env) */
#ifdef SBR_NEED_LOG
    fprintf(logFile, "in data (noise)\n");
    {
      Ipp32s i,j, Band;
      Ipp16s *ptr = bufNoiseQuant;

      for(i=0; i<nNoiseEnv; i++){
        Band = nNoiseBand[ pFIState->freqRes[i] ];
        for(j=0; j<Band; j++){
          fprintf(logFile, "%i\n", *ptr);
          ptr++;
        }
      }
    }
#endif

    sbrencDeltaCoding(bufNoiseQuant,
                      bufNoiseQuantPrev,

                      pHiFreqResTab, //Ipp32s*  freq_res, (only HiFreqRes for noise coding)
                      nNoiseBand,
                      curr_amp_res, // ignored
                      pEDState->bs_df_noise, //directionDeltaCoding,
                      //0, //coupling,
                      3, //Ipp32s   offset,
                      nNoiseEnv, //Ipp32s   nEnv,
                      //0, //channel,

                      pHeader->flag_SBRHeaderActive,//Ipp32s   headerActive,
                      &(pEDState->flagNoiseUpDate),
                      pEDState->env_dF_edge_incr_fac,
                      0,
                      pState->sbrHuffTabs);

    /* AYAlog: Ipp32s data (env) */
#ifdef SBR_NEED_LOG
    fprintf(logFile, "out data (noise)\n");
    {
      Ipp32s i,j, Band;
      Ipp16s *ptr = bufNoiseQuant;

      for(i=0; i<nNoiseEnv; i++){
        fprintf(logFile, "vec_df = %i\n", pEDState->bs_df_noise[i]);
        Band = nNoiseBand[ pFIState->freqRes[i] ];
        for(j=0; j<Band; j++){
          fprintf(logFile, "%i\n", *ptr);
          ptr++;
        }
      }
    }
#endif
    /* *********** stop noise coding **************** */
  //---------------------------------------------------------
    /* *********** start envelope coding **************** */

    /* AYA: Ipp32s data (env) */
#ifdef SBR_NEED_LOG
    fprintf(logFile, "in data (env)\n");
    {
      Ipp32s i,j, Band;
      Ipp16s *ptr = bufEnvQuant;

      for(i=0; i<*nEnv; i++){
        Band = nBand[ pFIState->freqRes[i] ];
        for(j=0; j<Band; j++){
          fprintf(logFile, "%i\n", *ptr);
          ptr++;
        }
      }
    }
#endif

    sbrencDeltaCoding(bufEnvQuant,
                      bufEnvQuantPrev,

                      pFIState->freqRes, //Ipp32s*  freq_res,
                      nBand,
                      curr_amp_res,
                      pEDState->bs_df_env, //directionDeltaCoding,
                      //0, //coupling,
                      0, //Ipp32s   offset,
                      *nEnv, //Ipp32s   nEnv,
                      //0, //channel,

                      pHeader->flag_SBRHeaderActive,//Ipp32s   headerActive,
                      &(pEDState->flagEnvUpDate),
                      pEDState->env_dF_edge_incr_fac,
                      1,
                      pState->sbrHuffTabs);

    /* start patch */
    if (pEDState->bs_df_env[0] == TIME_DOMAIN) {
      pEDState->env_dF_edge_incr_fac++;
    }else {
      pEDState->env_dF_edge_incr_fac = 0;
    }
    /* end patch */

    /* AYA: Ipp32s data (env) */
#ifdef SBR_NEED_LOG
    fprintf(logFile, "out data (env)\n");
    {
      Ipp32s i,j, Band;
      Ipp16s *ptr = bufEnvQuant;

      for(i=0; i<*nEnv; i++){
        fprintf(logFile, "vec_df = %i\n", pEDState->bs_df_env[i]);

        Band = nBand[ pFIState->freqRes[i] ];
        for(j=0; j<Band; j++){
          fprintf(logFile, "%i\n", *ptr);
          ptr++;
        }
      }
    }
#endif
    /* *********** end envelope coding **************** */
  }
  /* ********************************************************************* */
  /*                   ENF of MAIN LOOP                                    */
  /* ********************************************************************* */


  return status;
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

