/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AACDEC_SBR_FP_H__
#define __AACDEC_SBR_FP_H__

#include "align.h"
/* AAC */
#include "aaccmn_chmap.h"
#include "aac_dec_fp.h"
/* SBR */
#include "sbr_settings.h"
#include "sbr_dec_struct.h"
/* PS */
#include "aac_dec_ps_fp.h"

/********************************************************************/

typedef struct { // specification Filter

  Ipp8u* pQmfMemSpec[3][2];
  Ipp8u* pInitBuf[3];
  Ipp8u* pAnalysisFilterSpec[2];
  Ipp8u* pSynthesisFilterSpec[2];
  Ipp8u* pSynthesisDownFilterSpec[2];

} sSbrDecFilter;

/********************************************************************/

typedef struct { //  sbr matrix's work space

/* main-matrix */
  Ipp32f *XBuf[2][40];

  Ipp32f *YBuf[2][40];

  Ipp32f *ZBuf[2][40];

  /* these descriptors contain pointer to the allocatable memory (main matrix).
  /  It is need, because mixing wiil be done
  */
  Ipp32f* _dcMemoryMatrix[2];

/* HF adjustment: these buffers keep Noise & Gain data */
  Ipp32f  BufGain[2][MAX_NUM_ENV][MAX_NUM_ENV_VAL];
  Ipp32f  BufNoise[2][MAX_NUM_ENV][MAX_NUM_ENV_VAL];

/* LP mode */
  Ipp32f  degPatched[2][MAX_NUM_ENV_VAL];

  Ipp32f  bufEnvOrig[2][5*64];
  Ipp32f  bufNoiseOrig[2][2*5];

  /* for HF generation */
  Ipp32f  bwArray[2][MAX_NUM_NOISE_VAL];

} sSBRDecWorkState;

/********************************************************************/

typedef struct {
  sSBRDecComState    comState;
  sSBRDecWorkState   wsState;

  /* only pointer. structure is contained by AACDec */
  sPSDecState_32f*   pPSDecState;

} sSBRBlock;

/********************************************************************/

#ifdef  __cplusplus
extern  "C" {
#endif

 /* algorithm */
  Ipp32s sbrDequantization(sSBRDecComState* pSbrCom, sSBRDecWorkState* pSbrWS, Ipp32s ch, Ipp32s bs_amp_res);

  Ipp32s sbrGenerationHF(Ipp32f **XBuf, Ipp32f **Xhigh,
                         sSBRDecComState* comState,
                         Ipp32f* bwArray, Ipp32f* degPatched,
                         Ipp32s ch, Ipp32s decode_mode);

  void sbrAdjustmentHF(Ipp32f **YBuf,
                       Ipp32f* bufEnvOrig, Ipp32f* bufNoiseOrig,
                       Ipp32f BufGain[][MAX_NUM_ENV_VAL], Ipp32f BufNoise[][MAX_NUM_ENV_VAL],
                       sSBRDecComState* comState, Ipp32f *degPatched, Ipp8u *WorkBuffer,
                       Ipp32s reset, Ipp32s ch, Ipp32s decode_mode);


/* SBR GENERAL HIGH LEVEL API: FLOAT POINT VERSION */
 // init()
  Ipp32s sbrdecInitFilter(AACDec *pState);
  void sbrdecDrawMemMap(sSbrDecFilter** pDC, Ipp8u* pMem, Ipp32s* pSizes);
  void sbrdecUpdateMemMap(AACDec *state, Ipp32s mShift);

  /* internal buffer size is MAX (HQ mode, LP mode) */
  Ipp32s sbrdecGetFilterSize(Ipp32s* pFilterSize);
  void sbrInitDecoder(sSBRBlock* pState[CH_MAX], void* pMem);
  void sbrDecoderGetSize(Ipp32s *pSize);

 // reset()
  Ipp32s sbrdecReset(sSBRBlock* pSbr);

 // get_frame()
  Ipp32s sbrGetFrame(Ipp32f *pSrc,
                     Ipp32f *pDst,
                     Ipp32f* pDstR,
                     sSBRBlock * pSbr,
                     sSbrDecFilter* sbr_filter,
                     Ipp32s ch,
                     Ipp32s decode_mode,
                     Ipp32s dwnsmpl_mode,
                     Ipp32s flagPS,
                     Ipp8u* pWorkBuffer);

 // free()
  Ipp32s sbrFreeDecoder(sSBRBlock* pDst);
/* end */

#ifdef  __cplusplus
}
#endif

/********************************************************************/

#endif             /* __AACDEC_SBR_FP_H__ */
