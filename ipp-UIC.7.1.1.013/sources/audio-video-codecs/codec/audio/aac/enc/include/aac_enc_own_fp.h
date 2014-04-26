/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AAC_ENC_OWN_FP_H__
#define __AAC_ENC_OWN_FP_H__

#include "aaccmn_const.h"
#include "aac_enc_psychoacoustic_fp.h"
#include "aac_filterbank_fp.h"
#include "aac_enc_quantization_fp.h"
#include "aac_enc_own.h"
/* HEAAC profile*/
#include "sbr_enc_api_fp.h"

#include "ipps.h"

struct _AACEnc {
  Ipp32f**                 m_buff_pointers;
  Ipp32f**                 ltp_buff;
  Ipp32f**                 ltp_overlap;
  IppsFFTSpec_R_32f*       corrFft;
  Ipp8u*                   corrBuff;

  sPsychoacousticBlock*    psychoacoustic_block;
  sPsychoacousticBlockCom  psychoacoustic_block_com;
  sFilterbank              filterbank_block;

  /*AYA: HEAAC profile*/
  sSBREncState*             sbrState;
  ownFilterSpec_SBR_C_32fc* pQMFSpec;

  Ipp32f                    minSNRLong[MAX_SFB];
  Ipp32f                    minSNRShort[MAX_SFB_SHORT];

  AACEnc_com com;
};

#endif
