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

#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_struct.h"
#include "sbr_dec_settings.h"
#include "sbr_dec_struct.h"
#include "sbr_dec_parser.h"

#include "ipps.h"

/********************************************************************/

// it is usefull function is used by sbrdec_parser (not implemented yet)
static Ipp32s sbr_fill_default_header(sSBRHeader* pSbrHeader)
{
  /* default values for header extra 1 params */
  pSbrHeader->bs_freq_scale     = BS_FREQ_SCALE_DEFAULT;
  pSbrHeader->bs_alter_scale    = BS_ALTER_SCALE_DEFAULT;
  pSbrHeader->bs_noise_bands    = BS_NOISE_BANDS_DEFAULT;

  /* default values for header extra 2 params */
  pSbrHeader->bs_limiter_bands  = BS_LIMITER_BANDS_DEFAULT;
  pSbrHeader->bs_limiter_gains  = BS_LIMITER_GAINS_DEFAULT;
  pSbrHeader->bs_interpol_freq  = BS_INTERPOL_FREQ_DEFAULT;
  pSbrHeader->bs_smoothing_mode = BS_SMOOTHING_MODE_DEFAULT;
  pSbrHeader->bs_start_freq     = 5;
  pSbrHeader->bs_amp_res        = 1;

  pSbrHeader->Reset = 1;

  return 0; //OK
}

/********************************************************************/

Ipp32s sbrdecResetCommon(sSBRDecComState* pSbr)
{
  Ipp32s size = 2 * sizeof(Ipp32s);

  /* init FreqIndx */
  pSbr->sbr_freq_indx = -1;

  /* reset header to default param  */
  sbr_fill_default_header( &(pSbr->sbrHeader) );

  /* reset param */
  pSbr->transitionBand[0] = 0;
  pSbr->transitionBand[1] = 0;
  pSbr->kx_prev = 32;
  pSbr->kx = 32;

  pSbr->indexNoise[0] = 0;
  pSbr->indexNoise[1] = 0;
  pSbr->indexSine[0] = 0;
  pSbr->indexSine[1] = 0;

  // it is right
  //pSbr->sbrHeaderFlagPresent = 0;

  pSbr->FlagUpdate[0] = pSbr->FlagUpdate[1] = 1;

  pSbr->sbrFreqTabsState.nNoiseBandPrev = 0;
  pSbr->sbrFIState[0].nEnvPrev = pSbr->sbrFIState[1].nEnvPrev = 1;
  pSbr->sbrFIState[0].nNoiseEnvPrev = pSbr->sbrFIState[1].nNoiseEnvPrev = 1;
  pSbr->lA_prev[0] = pSbr->lA_prev[1] = -1;

  /* reset buffer */
  ippsZero_8u((Ipp8u *)(pSbr->sbrFIState[0].freqResPrev),  sizeof(Ipp32s) * MAX_NUM_ENV);
  ippsZero_8u((Ipp8u *)&(pSbr->S_index_mapped_prev[0][0]), size * MAX_NUM_ENV_VAL);
  ippsZero_8u((Ipp8u *)&(pSbr->bs_invf_mode_prev[0][0]),   size* MAX_NUM_NOISE_VAL);

  /* diagnostic */
  pSbr->sbrFlagError = SBR_NO_ERR;
  return 0;//OK
}

#endif //UMC_ENABLE_XXX

