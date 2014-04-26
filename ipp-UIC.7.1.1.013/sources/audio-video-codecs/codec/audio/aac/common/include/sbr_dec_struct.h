/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_DEC_STRUCT_H__
#define __SBR_DEC_STRUCT_H__

#include "aaccmn_chmap.h"
/* SBR */
#include "sbr_settings.h"
#include "sbr_struct.h"
#include "sbr_dec_settings.h"
/* PARAMETRIC STEREO */
#include "ps_dec_struct.h"

typedef struct {

/* SBR header */
  sSBRHeader sbrHeader;

/* SET OF FREQ TABS */
  sSBRFeqTabsState sbrFreqTabsState;

/* TF Grid */
  sSBRFrameInfoState sbrFIState[2];

/* ENV: noise, sines and env data */
  sSBREnvDataState   sbrEDState[2];

/* Huffman's tables (from core AAC dec) */
  void   *sbrHuffTables[10];

  Ipp32s  bs_sbr_crc_bits;

  Ipp32s  bs_coupling;
  Ipp32s  bs_frame_class[2];
  Ipp32s  bs_pointer[2];
  Ipp32s  bs_invf_mode[2][MAX_NUM_NOISE_VAL];
  Ipp32s  bs_invf_mode_prev[2][MAX_NUM_NOISE_VAL];

/* boundary band */
  Ipp32s  M;
  Ipp32s  M_prev;
  Ipp32s  kx;
  Ipp32s  kx_prev;
  Ipp32s  k0;

/* for HF Adjusment */
  Ipp32s  S_index_mapped_prev[2][MAX_NUM_ENV_VAL];

  Ipp32s  lA_prev[2];

  Ipp32s  indexNoise[2];
  Ipp32s  indexSine[2];

  Ipp32s  FlagUpdate[2];

/* last step band */
  Ipp32s  transitionBand[2];

/* core AAC */
  Ipp32s  sbr_layer;
  Ipp32s  id_aac;
  Ipp32s  sbr_freq_sample;
  Ipp32s  sbr_freq_indx;

/* for APPLICATION */
  Ipp32s  sbrHeaderFlagPresent;

/* for DEBUG */
  Ipp32s  sbrFlagError;
  Ipp32s  cnt_bit;

  /* PARAMETRIC STEREO ELEMENT */
  sPSDecComState* psState;
} sSBRDecComState;   /* common SBR structure */

/********************************************************************/

#ifdef  __cplusplus
extern  "C" {
#endif

  Ipp32s sbrdecResetCommon(sSBRDecComState* pSbr);

#ifdef  __cplusplus
}
#endif

#endif             /* __SBR_DEC_STRUCT_H__ */
