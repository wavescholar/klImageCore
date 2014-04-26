/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_ENC_STRUCT_H__
#define __SBR_ENC_STRUCT_H__

#include "aac_status.h"
/* <ID_SCE> <ID_CPE> */
#include "aaccmn_const.h"

#include "sbr_settings.h"
#include "sbr_enc_settings.h"

#include "ippac.h"
#include "ipps.h"

/********************************************************************/

typedef struct {

  Ipp32s bit_rate_min;
  Ipp32s bit_rate_max;

  Ipp32s sampleRate;

  int ID_XCE;

  Ipp32s bs_start_freq;
  Ipp32s bs_stop_freq;
  Ipp32s bs_noise_bands;
  Ipp32s bs_freq_scale;

  Ipp32s nf_max_level;

} sSBREncTuningTab;

/********************************************************************/

typedef struct
{
  Ipp32s frameClassOld;
  Ipp32s frameClass;

  Ipp32s bs_abs_bord;
  Ipp32s n;
  Ipp32s bs_pointer;
  Ipp32s bs_rel_bord[MAX_NUM_REL];
  Ipp32s bs_freq_res[MAX_NUM_ENV];

  Ipp32s bs_abs_bord_0;
  Ipp32s bs_abs_bord_1;
  Ipp32s bs_num_rel_0;
  Ipp32s bs_num_rel_1;
  Ipp32s bs_rel_bord_0[MAX_NUM_REL];
  Ipp32s bs_rel_bord_1[MAX_NUM_REL];

  Ipp32s bs_freq_res_LR[MAX_NUM_ENV];

} sSBRGrid;

/********************************************************************/

typedef struct
{
  Ipp32s bufBorders[MAX_NUM_ENV + 1 + 1]; //MAX_NUM_ENV + MAGIC
  Ipp32s lenBord;

  Ipp32s bufFreqRes[MAX_NUM_ENV + 1 + 1]; //MAX_NUM_ENV + MAGIC
  Ipp32s lenFreqRes;

  Ipp32s iTran;

  Ipp32s iFill;

} sSBRGridRaw;

/********************************************************************/
/*                     PLATFORM DEPEND STRUCTURES                   */
/********************************************************************/

typedef struct
{
  Ipp32f bufOrigTonalMean[INVF_SMOOTH_LEN+1];
  Ipp32f bufSbrTonalMean [INVF_SMOOTH_LEN+1];

  Ipp32f origTonalMeanSmooth;
  Ipp32f sbrTonalMeanSmooth;

  Ipp32f meanNrg;

}sSBRTonalMeanInfo;

/********************************************************************/

typedef struct
{
  sSBRTonalMeanInfo sbrTonalMeanInfo[MAX_NUM_NOISE_BANDS];

  Ipp32s prevRegionSBR[MAX_NUM_NOISE_BANDS];
  Ipp32s prevRegionOrig[MAX_NUM_NOISE_BANDS];

  Ipp32s bs_invf_mode[MAX_NUM_NOISE_BANDS];
  Ipp32s prev_bs_invf_mode[MAX_NUM_NOISE_BANDS];

}sSBRInvfEst;

/********************************************************************/

typedef struct{

  /* threshold: array [t] from 3GPP spec */
  Ipp32f threshold[64];

  /* transient: array [a] from 3GPP spec */
  Ipp32f transient[64];

  Ipp32s tranPos;
  Ipp32s tranFlag;

  Ipp32s flagTransferTran;

  /* previous params */
  Ipp32s prevTranFlag;
  Ipp32s prevTranFrame;
  Ipp32s prevTranPos;

  Ipp32f nrgPrevLow;

} sSBRTransientState;

/********************************************************************/

typedef struct
{
  Ipp32f bufGuideDiff[64];
  Ipp32f bufGuideOrig[64];
  Ipp32s bufGuideDetect[64];

} sSBRGuideData;

/********************************************************************/

typedef struct
{
  sSBRGuideData sbrGuideState[4];

  Ipp32f bufDiff[4][64];
  Ipp32f bufSfmOrig[4][64];
  Ipp32f bufSfmSBR[4][64];

  Ipp32s bufDetection[4][64];

  Ipp32s guideScfb[64];

  Ipp32s bufCompensation[ MAX_NUM_FREQ_COEFFS ];
  Ipp32s bufCompensationPrev[ MAX_NUM_FREQ_COEFFS ];

}sSBRSinEst;

/********************************************************************/

typedef struct{

  Ipp32f prevNoiseFloorLevels[NF_SMOOTHING_LENGTH][MAX_NUM_NOISE_VALUES];
  Ipp32f vecNoiseOrig[ MAX_NUM_NOISE_VALUES ]; /* Q[l][k] from spec */

  Ipp32f nf_max_level;

}sSBRNoiseEst;

/********************************************************************/

typedef struct {

  Ipp8u* pWorkBuf;
  Ipp8u* pMemSpec;
  IppsFFTSpec_C_32fc* pFFTSpec;
  /* tmp */
  Ipp32f buffer[640];

} ownFilterSpec_SBR_C_32fc;

/********************************************************************/

typedef struct {

  /* individual param for every ID_EL */
  Ipp32s bs_amp_res;

  /* instead direct calculation Tsbr */
  Ipp32s tabPatchMap[ 64 ];

//------------------------------------------------

  /* in buffer SBR (time signal) */
  Ipp32f  sbrInputBuffer[SIZE_OF_SBR_INPUT_BUF];

  /* subband-samples */
  Ipp32fc bufX[32][64];

  /* energy of subband-samples bufNrg[i] = | bufX[i] |^2 */
  Ipp32f  bufNrg[64][64];

  /* transpose matrix for speed optimization */
  Ipp32f bufNrgFlip[64][64];
  Ipp32fc bufXFlip[64][32];

  Ipp32f  bufT[4][64];
  Ipp32f  bufTNrg[4];

//------------------------------------------------

/* TRANSIENT INFO */
  sSBRTransientState sbrTransientState;

/* array for GRID-RAW */
  /* current info */
  sSBRGridRaw sbrGridRaw;

  /* next info */
  sSBRGridRaw sbrGridRawNext;

/* SBR FRAME GENERATOR */
  sSBRGrid sbrGrid;

  sSBRFrameInfoState sbrFIState;

/* estimation structures */
  sSBRInvfEst sbrInvfEst;

  sSBRSinEst  sbrSinEst;

  sSBRNoiseEst sbrNoiseEst;

  sSBREnvDataState sbrEDState;

} sSBREnc_SCE_State;

/********************************************************************/

#endif //__SBR_ENC_STRUCT_H__
