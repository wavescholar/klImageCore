/*//////////////////////////////////////////////////////////////////////////////
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_STRUCT_H__
#define __SBR_STRUCT_H__

#include "ippdefs.h"
#include "sbr_settings.h"

typedef struct
{
  // time depend: only encoder
  Ipp32s nSendSBRHeader;
  Ipp32s cntSentSBRHeader;
  Ipp32s flag_SBRHeaderActive;

  // tuning
  Ipp32s bs_start_freq; /* enc: from tuning tabs */
  Ipp32s bs_stop_freq;  /* enc: from tuning tabs */

  Ipp32s bs_freq_scale; /* enc: from tuning tabs */
  Ipp32s bs_alter_scale;

  Ipp32s bs_xover_band;
  Ipp32s bs_noise_bands;/* enc: from tuning tabs */

  Ipp32s bs_amp_res;

  //extra
  Ipp32s bs_extra_1;
  Ipp32s bs_extra_2;

  Ipp32s bs_limiter_bands;
  Ipp32s bs_limiter_gains;
  Ipp32s bs_interpol_freq;
  Ipp32s bs_smoothing_mode;

  Ipp32s  Reset;

} sSBRHeader;

/********************************************************************/

typedef struct
{
  /* freq tables */
  Ipp32s fMasterBandTab[MAX_SIZE_FREQ_TABS];
  Ipp32s fHiBandTab[MAX_SIZE_FREQ_TABS];
  Ipp32s fLoBandTab[MAX_SIZE_FREQ_TABS];
  Ipp32s fNoiseBandTab[MAX_SIZE_FREQ_TABS];
  /* decoder only */
  Ipp32s fLimBandTab[MAX_SIZE_FREQ_TABS];

  /* actual size of freq tables */
  Ipp32s nMasterBand;
  Ipp32s nHiBand; /* N_High */
  Ipp32s nLoBand; /* N_Low */
  Ipp32s nNoiseBand; /* NQ */
  Ipp32s nNoiseBandPrev; /* NQ_prev */
  /* decoder only */
  Ipp32s nLimBand;

  /* decoder only */
  Ipp32s numPatches;
  Ipp32s patchNumSubbandsTab[MAX_NUM_PATCHES];
  Ipp32s patchStartSubbandTab[MAX_NUM_PATCHES];

} sSBRFeqTabsState;

/********************************************************************/
/* sequence of this structure is important         */
/* for SBR encoder, see file <sbr_enc_frame_gen.c> */
/* *********************************************** */
typedef struct
{
  Ipp32s nEnv; /* bs_num_env = LE  */

  Ipp32s bordersEnv[MAX_NUM_ENV+1]; /* tE[] */

  Ipp32s freqRes[MAX_NUM_ENV];

  Ipp32s shortEnv; /* lA */

  Ipp32s nNoiseEnv; /* bs_num_noise = LQ */

  Ipp32s bordersNoise[MAX_NUM_NOISE_ENV+1]; /* tQ[] */

  /* prev data */
  Ipp32s nEnvPrev;

  Ipp32s freqResPrev[MAX_NUM_ENV];

  Ipp32s nNoiseEnvPrev;

} sSBRFrameInfoState;

/********************************************************************/

typedef struct
{
  Ipp32s  bs_add_harmonic_flag;
  Ipp32s  bs_add_harmonic_flag_prev;
  Ipp32s  bs_add_harmonic[MAX_NUM_ENV_VAL];
  Ipp32s  bs_add_harmonic_prev[MAX_NUM_ENV_VAL];

  Ipp32s  bs_df_env[MAX_NUM_ENV];
  Ipp32s  bs_df_noise[MAX_NUM_ENV];

  Ipp32s vSizeEnv[MAX_NUM_ENV+1];
  Ipp16s bufEnvQuant[MAX_NUM_ENV*64];
  Ipp16s bufEnvQuantPrev[MAX_NUM_ENV_VAL];

  Ipp32s vSizeNoise[2+1];
  Ipp16s bufNoiseQuant[2*5];
  Ipp16s bufNoiseQuantPrev[MAX_NUM_ENV_VAL];

  /* encoder only */
  Ipp32s flagEnvUpDate;
  Ipp32s flagNoiseUpDate;

  Ipp32s env_dF_edge_incr_fac;

}sSBREnvDataState;

#endif             /* __SBR_STRUCT_H__ */
