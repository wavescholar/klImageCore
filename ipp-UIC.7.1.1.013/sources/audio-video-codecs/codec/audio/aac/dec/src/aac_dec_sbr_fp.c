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
#include <stdio.h>
#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_dec_struct.h"
#include "aac_dec_sbr_fp.h"
#include "sbr_dec_tabs_fp.h"

/********************************************************************/

#ifndef ID_SCE
#define ID_SCE    0x0
#endif

#ifndef ID_CPE
#define ID_CPE    0x1
#endif

/********************************************************************/

static Ipp32s sbrPassiveSync(Ipp32f **XBuf,
                                   Ipp32f **ZBuf,
                                   Ipp32s mode);

/********************************************************************/

static Ipp32s sbrActiveSync(Ipp32f **XBuf, Ipp32f **YBuf,
                                  Ipp32s transitionBand, Ipp32s kx_prev, Ipp32s kx,
                                  Ipp32f **ZBuf, Ipp32s mode);

/********************************************************************/

/* Set HF subbands to zero */
static Ipp32s sbrCleanHFBand(Ipp32f** pYBuf, Ipp32s startBand, Ipp32s stopBand, Ipp32s mode);

/********************************************************************/

static Ipp32s sbrUpDateBands(Ipp32f** ppSrc );

/********************************************************************/

static Ipp32s ownSynchronization_PS_32fc( Ipp32fc** ppSrc, Ipp32fc** ppDst );

/********************************************************************/

static Ipp32s ownPassiveUpdate_PS_32fc( Ipp32fc** ppSrc, Ipp32fc** ppDst );

/********************************************************************/

static Ipp32s sbrUpdateAmpRes(Ipp32s bs_frame_class, Ipp32s L_E, Ipp32s bs_amp_res)
{
  if ((bs_frame_class == FIXFIX) && (L_E == 1))
    bs_amp_res = 0;

  return bs_amp_res;
}

/********************************************************************/

Ipp32s sbrdecReset(sSBRBlock* pSbr)
{
  sbrdecResetCommon( &(pSbr->comState) );
  ippsZero_32f(pSbr->wsState.BufGain[0][0], 2*MAX_NUM_ENV*MAX_NUM_ENV_VAL);
  ippsZero_32f(pSbr->wsState.BufNoise[0][0], 2*MAX_NUM_ENV*MAX_NUM_ENV_VAL);
  ippsZero_32f(pSbr->wsState.bwArray[0], 2 * MAX_NUM_NOISE_VAL);

  return 0;//OK
}

/********************************************************************/

void sbrDecoderGetSize(Ipp32s *pSize)
{

  Ipp32s Size32 = (32) * (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN);
  Ipp32s Size64 = (64) * (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN);

  pSize[0] = (((Size32 + 2 * Size64) * 2) * sizeof(Ipp32f) * 2) * CH_MAX;

}

/********************************************************************/

void sbrInitDecoder(sSBRBlock* pState[CH_MAX], void* pMem)
{
  Ipp32s  ch, i, j;
  sSBRBlock *pBlock;
  Ipp32f *pData = (Ipp32f *)pMem;
  Ipp32s bufLen;

  Ipp32s Size32 = (32) * (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN);
  Ipp32s Size64 = (64) * (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN);

  bufLen = (Size32 + 2 * Size64) * 2; /* in floats */

  /* --------------------- set memory for matrix --------------------- */
  for (i = 0; i < CH_MAX; i++) {
    pBlock = pState[i];
    ippsZero_8u((Ipp8u*)pBlock, sizeof(sSBRBlock));

    for (ch = 0; ch < 2; ch++) {
      pBlock->wsState.XBuf[ch][0] = pData;
      ippsZero_32f(pBlock->wsState.XBuf[ch][0], bufLen);

      /* process need because mixing memory will be done */
      pBlock->wsState._dcMemoryMatrix[ch] = 0;
      pBlock->wsState._dcMemoryMatrix[ch] = pBlock->wsState.XBuf[ch][0];

      for (j = 0; j < (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN); j++) {
        pBlock->wsState.XBuf[ch][j] = pBlock->wsState.XBuf[ch][0] + j * (2*32);
        pBlock->wsState.ZBuf[ch][j] = pBlock->wsState.XBuf[ch][0] + (2*Size32 + 0 * Size64)+ j * (2*64);
        pBlock->wsState.YBuf[ch][j] = pBlock->wsState.XBuf[ch][0] + (2*Size32 + 2 * Size64)+ j * (2*64);
      }
      pData += bufLen;
    }
    /* --  set default values -- */
    sbrdecReset(pBlock);
    pBlock->comState.sbrHeaderFlagPresent = 0;
  }
}

/* **************************************************************** */

Ipp32s sbrGetFrame(Ipp32f *pSrc,
                   Ipp32f *pDst,
                   Ipp32f *pDstR,
                   sSBRBlock * pSbr,
                   sSbrDecFilter* sbr_filter,
                   Ipp32s ch,
                   Ipp32s decode_mode,
                   Ipp32s dwnsmpl_mode,
                   Ipp32s flagPS,
                   Ipp8u* pWorkBuffer )
{

  sSBRDecComState*  com = &(pSbr->comState);
  sSBRDecWorkState* ws  = &(pSbr->wsState);
  Ipp32s startBand = 0;//RATE * com->tE[ch][0];
  Ipp32s stopBand  = 0;//RATE * com->tE[ch][ com->L_E[ch] ];
  Ipp32s transitionBand = com->transitionBand[ch];
  Ipp32s bs_amp_res;
  Ipp32s l;
  Ipp32s criterion_valid = (com->sbrHeaderFlagPresent != 0) && (com->sbrFlagError == 0);

  /* AYA : temporary */
  Ipp32s decode_modePS = flagPS; //full support

  /* preprocessing: protected for PS data */
  if( 0xA & decode_modePS ){
    decode_mode = HEAAC_HQ_MODE;
    dwnsmpl_mode= HEAAC_DWNSMPL_OFF;
  }

/* -------------------------------- Analysis ---------------------------- */
  if (decode_mode == HEAAC_HQ_MODE)
    ippsAnalysisFilter_SBR_RToC_32f32fc_D2L(pSrc,
                                        (Ipp32fc**)(ws->XBuf[ch]),
                                        SBR_TABLE_QMF_WINDOW_320,
                                        NUM_TIME_SLOTS * RATE, SBR_TIME_HFGEN,
                                        pSbr->comState.kx,
                                        (IppsFilterSpec_SBR_C_32fc *)sbr_filter->pAnalysisFilterSpec[ch],
                                        pWorkBuffer );
  else {

    for(l=0; l<NUM_TIME_SLOTS * RATE; l++) {

      /*********************************************************************
       * NOTE:
       * (1) matrix idea is better than 1D
       *     but if you can use this function for non-matrix,
       *     you must use such consrtuctions
       * (2) you can use non-standard function of window and get better
       *     result (speech codec or other area)
       *********************************************************************/
      {
        IppStatus status;
          status = ippsAnalysisFilter_SBR_RToR_32f_D2L(pSrc + l*32,
                                          ws->XBuf[ch] + l + SBR_TIME_HFGEN,
                                          SBR_TABLE_QMF_WINDOW_320,
                                          1, 0,
                                          com->kx,
                                          (IppsFilterSpec_SBR_R_32f *)sbr_filter->pAnalysisFilterSpec[ch],
                                          pWorkBuffer );
      }


    }
  }

/* -------------------------------- SBR process ---------------------------- */
  if (criterion_valid && ((com->id_aac == ID_SCE) || (com->id_aac == ID_CPE))) { // IF ACTIVE SBR

        startBand = RATE * com->sbrFIState[ch].bordersEnv[0];
        stopBand  = RATE * com->sbrFIState[ch].bordersEnv[ com->sbrFIState[ch].nEnv ];

    if ( !ch || !com->bs_coupling ) {
      bs_amp_res = sbrUpdateAmpRes( com->bs_frame_class[ch], com->sbrFIState[ch].nEnv,
                                    com->sbrHeader.bs_amp_res);

      sbrDequantization(com, ws, ch,  bs_amp_res);
    }

    /* Set HF subbands to zero */
    sbrCleanHFBand(ws->YBuf[ch], startBand, stopBand, decode_mode);

    sbrGenerationHF(ws->XBuf[ch], ws->YBuf[ch],
                    com, ws->bwArray[ch], ws->degPatched[ch],
                    ch, decode_mode);

    if (decode_mode == HEAAC_LP_MODE){
      for (l = startBand; l < stopBand; l++)
        ippsZero_32f(ws->YBuf[ch][SBR_TIME_HFADJ + l], com->kx);
    }

    sbrAdjustmentHF(ws->YBuf[ch],
                    ws->bufEnvOrig[ch], ws->bufNoiseOrig[ch],
                    ws->BufGain[ch], ws->BufNoise[ch],
                    com, ws->degPatched[ch], pWorkBuffer,
                    com->sbrHeader.Reset, ch, decode_mode );

    sbrActiveSync( // in data
                   ws->XBuf[ch],
                   ws->YBuf[ch],
                   transitionBand, com->kx_prev, com->kx,
                   // out data
                   ws->ZBuf[ch],
                   decode_mode);


    /* UpDate High Band */
    sbrUpDateBands(ws->YBuf[ch]);

   /* store ch independ */
    com->transitionBand[ch] = stopBand - NUM_TIME_SLOTS * RATE;

  } else {
    sbrPassiveSync(ws->XBuf[ch],
                   ws->ZBuf[ch],
                   decode_mode);
  }


 /* ---------------------------------- Parametric Stereo Part ---------------------------- */
  /*********************************************************************************
  * complex input of SBR_SynthesisQMF == input of PS tool.                         *
  * but SBR delay has to be removed                                                *
  *********************************************************************************/
  if( 0xA & decode_modePS ){
    ownSynchronization_PS_32fc( (Ipp32fc**)(ws->XBuf[ch]), (Ipp32fc**)(ws->ZBuf[ch]) );
    //aya_printf_qmf((Ipp32fc**)(ws->ZBuf[ch]));

    if( criterion_valid ){ //SBR ACTIVE
      //ownPassiveUpdate_PS_32fc( (Ipp32fc**)(ws->ZBuf[ch]), (Ipp32fc**)(ws->ZBuf[ch+1]) );
      psdecDecode_32fc((Ipp32fc**)(ws->ZBuf[ch]),
                       (Ipp32fc**)(ws->ZBuf[ch+1]),
                        pSbr->pPSDecState,
                        com->sbrFreqTabsState.fHiBandTab[ com->sbrFreqTabsState.nHiBand ]);
    } else { // SBR PASSIVE
      ownPassiveUpdate_PS_32fc( (Ipp32fc**)(ws->ZBuf[ch]), (Ipp32fc**)(ws->ZBuf[ch+1]) );
    }
  }



/* -------------------------------- Synthesis OR SynthesisDown ---------------------------- */
  /*********************************************************************************
   * see comment in sbrPassiveSync OR sbrActiveSync functions
   * or see fixed-point version
   *********************************************************************************/
  {
    Ipp32s curCh, indxOutCh, nCh = (0xA & decode_modePS) ? 2 : ch + 1;
    Ipp32f* pOutBuf[2];

    pOutBuf[0] = pDst;
    pOutBuf[1] = pDstR;

    for( indxOutCh = 0, curCh = ch; curCh < nCh; curCh++, indxOutCh++ ){

      switch (decode_mode | dwnsmpl_mode) {

      case (HEAAC_HQ_MODE | HEAAC_DWNSMPL_OFF):
          ippsSynthesisFilter_SBR_CToR_32fc32f_D2L( (const Ipp32fc**)(ws->ZBuf[curCh]), pOutBuf[indxOutCh],
                                                SBR_TABLE_QMF_WINDOW_640,
                                                NUM_TIME_SLOTS * RATE,
                                                (IppsFilterSpec_SBR_C_32fc *)sbr_filter->pSynthesisFilterSpec[curCh],
                                                pWorkBuffer );
          break;

      case (HEAAC_HQ_MODE | HEAAC_DWNSMPL_ON):
          ippsSynthesisDownFilter_SBR_CToR_32fc32f_D2L((const Ipp32fc**)(ws->ZBuf[curCh]), pOutBuf[indxOutCh],
                                                  SBR_TABLE_QMF_WINDOW_320,
                                                  NUM_TIME_SLOTS * RATE,
                                                  (IppsFilterSpec_SBR_C_32fc *)sbr_filter->pSynthesisDownFilterSpec[curCh],
                                                  pWorkBuffer );
          break;

      case (HEAAC_LP_MODE | HEAAC_DWNSMPL_OFF):
          ippsSynthesisFilter_SBR_RToR_32f_D2L((const Ipp32f**)(ws->ZBuf[curCh]), pOutBuf[indxOutCh],
                                              SBR_TABLE_QMF_WINDOW_640,
                                              NUM_TIME_SLOTS * RATE,
                                              (IppsFilterSpec_SBR_R_32f *)sbr_filter->pSynthesisFilterSpec[curCh],
                                              pWorkBuffer );
          break;

      default:
          ippsSynthesisDownFilter_SBR_RToR_32f_D2L( (const Ipp32f**)(ws->ZBuf[curCh]), pOutBuf[indxOutCh],
                                                  SBR_TABLE_QMF_WINDOW_320,
                                                  NUM_TIME_SLOTS * RATE,
                                                  (IppsFilterSpec_SBR_R_32f *)sbr_filter->pSynthesisDownFilterSpec[curCh],
                                                  pWorkBuffer );
          break;
      } // END OF switch (decode_mode | dwnsmpl_mode) {

    } // END OF for( curCh = 0; curCh < nCh; curCh++ ){
  } // END FOR PS BLOCK

  /* UpDate Low Bands */
  sbrUpDateBands(ws->XBuf[ch] );

/* ---------------------------- <store ch depend> ---------------------------- */

  if ((com->id_aac == ID_SCE) || ((com->id_aac == ID_CPE) && (ch == 1))) {
    com->kx_prev = com->kx;
    com->M_prev = com->M;
    com->sbrHeader.Reset = 0;
  }

  return 0;     // OK
}

/********************************************************************/

Ipp32s sbrPassiveSync(Ipp32f **XBuf, Ipp32f **ZBuf, Ipp32s mode)
{
  Ipp32s  l;

  Ipp32s xScale = (HEAAC_LP_MODE == mode) ? 1 : 2;

  for (l = 0; l < NUM_TIME_SLOTS * RATE; l++) {
    ippsCopy_32f(XBuf[l + SBR_TIME_HFADJ], ZBuf[l], NUM_TIME_SLOTS * RATE * xScale);
    ippsZero_32f(ZBuf[l] + 32 * xScale, NUM_TIME_SLOTS * RATE * xScale);
  }

  return 0;     // OK
}

/********************************************************************/

Ipp32s sbrActiveSync(Ipp32f **XBuf,
                           Ipp32f **YBuf,
                           Ipp32s transitionBand, Ipp32s kx_prev, Ipp32s kx,
                           Ipp32f **ZBuf, Ipp32s mode)
{
  Ipp32s k, l;
  Ipp32s xoverBand;
  Ipp32s xScale = (HEAAC_LP_MODE == mode) ? 1 : 2;

  /********************************************************************
   * NOTE: code may be optimized (memory), if synthesis implement here
   ********************************************************************/

  for (l = 0; l < 32; l++) {
    if (l < transitionBand) {
      xoverBand = kx_prev;
    } else {
      xoverBand = kx;
    }

    for (k = 0; k < xoverBand * xScale; k++) {
      ZBuf[l][k] = XBuf[SBR_TIME_HFADJ + l][k];
    }

    if (mode == HEAAC_LP_MODE) {
      ZBuf[l][xoverBand - 1] += YBuf[SBR_TIME_HFADJ + l][xoverBand - 1];
    }

    for (k = xoverBand * xScale; k < 64 * xScale; k++) {
      ZBuf[l][k] = YBuf[SBR_TIME_HFADJ + l][k];
    }
  }

  return 0;     // OK
}

/********************************************************************/

/* Set HF subbands to zero */
Ipp32s sbrCleanHFBand(Ipp32f** pYBuf, Ipp32s startBand, Ipp32s stopBand, Ipp32s mode)
{
  Ipp32s i;
  Ipp32s xScale = (HEAAC_LP_MODE == mode) ? 1 : 2;

  for (i = startBand; i < stopBand; i++) {
    ippsZero_32f(pYBuf[i + SBR_TIME_HFADJ], 64 * xScale);
  }

  return 0; //OK
}

/********************************************************************/

Ipp32s sbrUpDateBands(Ipp32f** ppSrc)
{
  Ipp32s  l;
  Ipp32f* pBufTmp;

  for (l = 0; l < SBR_TIME_HFGEN; l++) {
    // Re part
    pBufTmp    = ppSrc[l];
    ppSrc[l] = ppSrc[NUM_TIME_SLOTS * RATE + l];
    ppSrc[NUM_TIME_SLOTS * RATE + l] = pBufTmp;
  }

  return 0;
}

/********************************************************************/

Ipp32s ownSynchronization_PS_32fc( Ipp32fc** ppSrc, Ipp32fc** ppDst )
{
  Ipp32s l = 0, k = 0;

  for( l = NUM_TIME_SLOTS * RATE; l < NUM_TIME_SLOTS * RATE + 6; l++ ){
    for( k = 0; k < 5; k++){
      ppDst[l][k] = ppSrc[l + SBR_TIME_HFADJ][k];
    }
  }

  return 0;//OK
}

/********************************************************************/

Ipp32s ownPassiveUpdate_PS_32fc( Ipp32fc** ppSrc, Ipp32fc** ppDst )
{
  Ipp32s l = 0;

  for(l = 0; l < 32; l++){
    ippsCopy_32fc(ppSrc[l], ppDst[l], 64);
  }

  return 0;
}

/********************************************************************/
/* EOF */

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

