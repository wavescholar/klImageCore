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

#include <stdio.h>
#include "aac_status.h"
/* sbr decoder */
#include "sbr_struct.h"
#include "sbr_freq_tabs.h"
#include "sbr_dec_parser.h"
#include "sbr_huff_tabs.h"
/* parametric stereo */
#include "ps_dec_settings.h"
#include "ps_dec_struct.h"
#include "ps_dec_parser.h"

#include "ipps.h"

/********************************************************************/

#define SBR_ENV_PROTECT( data ) data
//IPP_MAX( IPP_MIN(data, 128), 0 )

#define SBR_NOISE_PROTECT( data ) data
//IPP_MAX( IPP_MIN(data, 30), 0 )

/********************************************************************/

static
Ipp32s sbr_extension(sBitsreamBuffer * BS,
                     sPSDecComState* pPSItem,
                     Ipp32s bs_extension_id)
{
  Ipp32s error = 0;//no error

  switch( bs_extension_id ){
    case EXTENSION_ID_PS:

      error = ps_dec_parser( BS, pPSItem );

      if( error ) {

        return SBR_ERR_PARSER;
      }


      break;

    default:

      break;

  }

  return SBR_NO_ERR;
}

/********************************************************************/

#ifndef ID_SCE
#define ID_SCE    0x0
#endif

#ifndef ID_CPE
#define ID_CPE    0x1
#endif

/********************************************************************/

static Ipp32s sbr_header_default_fill(sSBRHeader* pSbrHeader)
{

  pSbrHeader->bs_amp_res = 1; //default

  pSbrHeader->bs_start_freq = 0; //default

  pSbrHeader->bs_stop_freq = 0; //default

  pSbrHeader->bs_xover_band = 0; //default

  pSbrHeader->bs_freq_scale = BS_FREQ_SCALE_DEFAULT;

  pSbrHeader->bs_alter_scale = BS_ALTER_SCALE_DEFAULT;

  pSbrHeader->bs_noise_bands = BS_NOISE_BANDS_DEFAULT;

  pSbrHeader->bs_limiter_bands = BS_LIMITER_BANDS_DEFAULT;

  pSbrHeader->bs_limiter_gains = BS_LIMITER_GAINS_DEFAULT;

  pSbrHeader->bs_interpol_freq = BS_INTERPOL_FREQ_DEFAULT;

  pSbrHeader->bs_smoothing_mode = BS_SMOOTHING_MODE_DEFAULT;

  return 0;//OK
}

/********************************************************************/

static Ipp32s is_sbr_header_valid(sSBRHeader* pSbrHeader)
{
  Ipp32s res = SBR_OK;

  if(pSbrHeader->bs_amp_res < 0 || pSbrHeader->bs_amp_res > 1)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_start_freq < 0)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_stop_freq < 0)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_freq_scale < 0 || pSbrHeader->bs_freq_scale > 3)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_alter_scale < 0 || pSbrHeader->bs_alter_scale > 1)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_noise_bands < 0 || pSbrHeader->bs_noise_bands > 3)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_limiter_bands < 0 || pSbrHeader->bs_limiter_bands > 3)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_limiter_gains < 0 || pSbrHeader->bs_limiter_gains > 3)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_interpol_freq < 0 || pSbrHeader->bs_interpol_freq > 1)
    return SBR_ERR_REQUIREMENTS;

  if(pSbrHeader->bs_smoothing_mode < 0 || pSbrHeader->bs_smoothing_mode > 1)
    return SBR_ERR_REQUIREMENTS;

  return res;

}

/********************************************************************/

static Ipp32s sbr_header(sBitsreamBuffer * BS, sSBRHeader* pSbrHeader)
{
  Ipp32s cnt = 0;

  Ipp32s bs_header_extra_1;
  Ipp32s bs_header_extra_2;

  Ipp32s bs_start_freq_new;
  Ipp32s bs_stop_freq_new;
  Ipp32s bs_freq_scale_new;
  Ipp32s bs_alter_scale_new;
  Ipp32s bs_xover_band_new;
  Ipp32s bs_noise_bands_new;
  Ipp32s bs_reserved;

  GET_BITS(BS, pSbrHeader->bs_amp_res, 1, Ipp32s);
  GET_BITS(BS, bs_start_freq_new, 4, Ipp32s);
  GET_BITS(BS, bs_stop_freq_new, 4, Ipp32s);
  GET_BITS(BS, bs_xover_band_new, 3, Ipp32s);

  GET_BITS(BS, bs_reserved, 2, Ipp32s);

  GET_BITS(BS, bs_header_extra_1, 1, Ipp32s);
  GET_BITS(BS, bs_header_extra_2, 1, Ipp32s);

  cnt += 16;

  if (bs_header_extra_1) {
    GET_BITS(BS, bs_freq_scale_new, 2, Ipp32s);
    GET_BITS(BS, bs_alter_scale_new, 1, Ipp32s);
    GET_BITS(BS, bs_noise_bands_new, 2, Ipp32s);

    cnt += 5;
  } else {      // default
    bs_freq_scale_new = 2;
    bs_alter_scale_new = 1;
    bs_noise_bands_new = 2;
  }

  if (bs_header_extra_2) {
    GET_BITS(BS, pSbrHeader->bs_limiter_bands, 2, Ipp32s);
    GET_BITS(BS, pSbrHeader->bs_limiter_gains, 2, Ipp32s);
    GET_BITS(BS, pSbrHeader->bs_interpol_freq, 1, Ipp32s);
    GET_BITS(BS, pSbrHeader->bs_smoothing_mode, 1, Ipp32s);

    cnt += 6;
  } else {      // default
    pSbrHeader->bs_limiter_bands = 2;
    pSbrHeader->bs_limiter_gains = 2;
    pSbrHeader->bs_interpol_freq = 1;
    pSbrHeader->bs_smoothing_mode = 1;
  }

  if ((pSbrHeader->bs_start_freq != bs_start_freq_new) ||
      (pSbrHeader->bs_stop_freq != bs_stop_freq_new) ||
      (pSbrHeader->bs_freq_scale != bs_freq_scale_new) ||
      (pSbrHeader->bs_alter_scale != bs_alter_scale_new) ||
      (pSbrHeader->bs_xover_band != bs_xover_band_new) ||
      (pSbrHeader->bs_noise_bands != bs_noise_bands_new)) {
    pSbrHeader->Reset = 1;

    pSbrHeader->bs_start_freq = bs_start_freq_new;
    pSbrHeader->bs_stop_freq = bs_stop_freq_new;
    pSbrHeader->bs_freq_scale = bs_freq_scale_new;
    pSbrHeader->bs_alter_scale = bs_alter_scale_new;
    pSbrHeader->bs_xover_band = bs_xover_band_new;
    pSbrHeader->bs_noise_bands = bs_noise_bands_new;
  } else {
    pSbrHeader->Reset = 0;
  }

  /*********************************************************************
   * NOTE:
   * check SBR header params
   *********************************************************************/
  if( is_sbr_header_valid(pSbrHeader) == SBR_ERR_REQUIREMENTS ){
    sbr_header_default_fill(pSbrHeader);
    return SBR_ERR_REQUIREMENTS;
  }
  //------------------------

  return cnt;
}

/********************************************************************/

static
Ipp32s sbr_single_channel_element(sBitsreamBuffer*  BS,
                                  sSBRDecComState*  pSBRItem)
{
  Ipp32s ret_cnt = 0;
  Ipp32s cnt = 0;
  Ipp32s num_bits_left = 0;
  Ipp32s i;
  Ipp32s bs_data_extra = 0;

  Ipp32s bs_reserved = 0;
  Ipp32s bs_esc_count = 0;
  Ipp32s bs_amp_res;

  Ipp32s bs_extended_data  = 0;
  Ipp32s bs_extension_size = 0;
  Ipp32s bs_extension_id   = 0;

  Ipp32s decodeBitsStart = 0, decodeBitsStop = 0, error;

  sSBRFrameInfoState* pFIState = &(pSBRItem->sbrFIState[0]);
  sSBREnvDataState*   pEDState = &(pSBRItem->sbrEDState[0]);

  /* CODE */

  GET_BITS(BS, bs_data_extra, 1, Ipp32s);
  ret_cnt++;

  if (bs_data_extra) {

  GET_BITS(BS, bs_reserved, 4, Ipp32s);
    ret_cnt += 4;
  }
  ret_cnt += sbr_grid(BS,
                      &(pSBRItem->bs_frame_class[0]),
                      &(pSBRItem->bs_pointer[0]),
                      pFIState->freqRes,
                      pFIState->bordersEnv,
                      pFIState->bordersNoise,
                      &(pFIState->nEnv),
                      &(pFIState->nNoiseEnv),
                      &error );

  if(error)
    return SBR_ERR_REQUIREMENTS;

    //(0, BS, pSBRItem);
  ret_cnt += sbr_dtdf(BS, pEDState->bs_df_env, pEDState->bs_df_noise,
                          pFIState->nEnv, pFIState->nNoiseEnv);
  ret_cnt += sbr_invf(0, BS, pSBRItem);
/*
 * patch
 */
  if ((pSBRItem->bs_frame_class[0] == FIXFIX) && (pFIState->nEnv == 1))
    bs_amp_res = 0;
  else
    bs_amp_res = pSBRItem->sbrHeader.bs_amp_res;

  ret_cnt += sbr_envelope(0, 0, bs_amp_res, BS, pSBRItem);

  ret_cnt += sbr_noise(BS, pEDState->bufNoiseQuant, pEDState->vSizeNoise, pEDState->bs_df_noise,
    pSBRItem->sbrHuffTables, 0, 0, pFIState->nNoiseEnv, pSBRItem->sbrFreqTabsState.nNoiseBand);

  GET_BITS(BS, pEDState->bs_add_harmonic_flag, 1, Ipp32s);
  ret_cnt++;
  if (pEDState->bs_add_harmonic_flag) {
    ret_cnt += sbr_sinusoidal_coding( BS, pEDState->bs_add_harmonic, pSBRItem->sbrFreqTabsState.nHiBand );   //(0, BS, pSBRItem);
  } else {
    for (i = 0; i < MAX_NUM_ENV_VAL; i++) {
      pEDState->bs_add_harmonic[i] = 0;
    }
  }

  GET_BITS(BS, bs_extended_data, 1, Ipp32s);
  ret_cnt++;

  if (bs_extended_data) {
    GET_BITS(BS, cnt, 4, Ipp32s);
    bs_extension_size = cnt;
    ret_cnt += 4;

    if (cnt == 15) {
      GET_BITS(BS, bs_esc_count, 8, Ipp32s);
      cnt += bs_esc_count;
      ret_cnt += 8;
    }

    num_bits_left = 8 * cnt;
    while (num_bits_left > 7) {
      GET_BITS(BS, bs_extension_id, 2, Ipp32s);
      ret_cnt += 2;
      num_bits_left -= 2;

      /* PS PARSER */
      GET_BITS_COUNT(BS, decodeBitsStart);
      /* patch. you can turn off support of PS decoder */
      if( NULL == pSBRItem->psState && EXTENSION_ID_PS == bs_extension_id){
        bs_extension_id = !EXTENSION_ID_PS;
      }
      error = sbr_extension(BS, pSBRItem->psState, bs_extension_id);

      if (error) return SBR_ERR_PARSER;

      GET_BITS_COUNT(BS, decodeBitsStop);

      /* AYA path */
      if( EXTENSION_ID_PS == bs_extension_id ){
        //printf("PS_FILL_BITS = %i\n", ( decodeBitsStop - decodeBitsStart ));
      }

      ret_cnt       += ( decodeBitsStop - decodeBitsStart );
      num_bits_left -= ( decodeBitsStop - decodeBitsStart );
    }
  }

  return ret_cnt;
}

/********************************************************************/

static Ipp32s sbr_channel_pair_element(sBitsreamBuffer * BS, sSBRDecComState * pSBRItem)
{
  Ipp32s ret_cnt = 0;
  Ipp32s cnt = 0;
  Ipp32s num_bits_left = 0;

  Ipp32s bs_data_extra = 1;

  Ipp32s bs_reserved = 0;
  Ipp32s bs_esc_count = 0;
  Ipp32s ampRes;

  Ipp32s bs_extended_data = 0;
  Ipp32s bs_extension_size = 0;
  Ipp32s bs_extension_id   = 0;

  sSBRFeqTabsState*   pFTState = &(pSBRItem->sbrFreqTabsState);

  sSBRFrameInfoState* pFIState0 = &(pSBRItem->sbrFIState[0]);
  sSBRFrameInfoState* pFIState1 = &(pSBRItem->sbrFIState[1]);

  sSBREnvDataState*   pEDState0 = &(pSBRItem->sbrEDState[0]);
  sSBREnvDataState*   pEDState1 = &(pSBRItem->sbrEDState[1]);

  Ipp32s decodeBitsStart = 0, decodeBitsStop = 0, error = 0;

  /* CODE */
  GET_BITS(BS, bs_data_extra, 1, Ipp32s);
  ret_cnt++;

  if (bs_data_extra) {
    GET_BITS(BS, bs_reserved, 8, Ipp32s);
    ret_cnt += 8;
  }

  GET_BITS(BS, pSBRItem->bs_coupling, 1, Ipp32s);
  ret_cnt++;

  if (pSBRItem->bs_coupling) {
    ret_cnt += sbr_grid(BS, &(pSBRItem->bs_frame_class[0]),
                        &(pSBRItem->bs_pointer[0]),
                        pFIState0->freqRes,
                        pFIState0->bordersEnv,
                        pFIState0->bordersNoise,
                        &(pFIState0->nEnv),
                        &(pFIState0->nNoiseEnv), &error );
    if(error)
      return error;

    sbr_grid_coupling(pSBRItem);

    ret_cnt += sbr_dtdf(BS, pEDState0->bs_df_env, pEDState0->bs_df_noise, pFIState0->nEnv,
                        pFIState0->nNoiseEnv);
    ret_cnt += sbr_dtdf(BS, pEDState1->bs_df_env, pEDState1->bs_df_noise, pFIState1->nEnv,
                        pFIState1->nNoiseEnv);
    ret_cnt += sbr_invf(0, BS, pSBRItem);

    if ((pSBRItem->bs_frame_class[0] == FIXFIX) && (pFIState0->nEnv == 1))
      ampRes = 0;
    else
      ampRes = pSBRItem->sbrHeader.bs_amp_res;

    ret_cnt += sbr_envelope(0, 1, ampRes, BS, pSBRItem);
    ret_cnt += sbr_noise(BS, pEDState0->bufNoiseQuant, pEDState0->vSizeNoise,
                         pEDState0->bs_df_noise, pSBRItem->sbrHuffTables, 0, 1,
                         pFIState0->nNoiseEnv, pFTState->nNoiseBand);

    ret_cnt += sbr_envelope(1, 1, ampRes, BS, pSBRItem);
    ret_cnt += sbr_noise(BS, pEDState1->bufNoiseQuant, pEDState1->vSizeNoise,
                         pEDState1->bs_df_noise, pSBRItem->sbrHuffTables, 1, 1,
                         pFIState1->nNoiseEnv, pFTState->nNoiseBand);
  } else {
    ret_cnt += sbr_grid(BS, &(pSBRItem->bs_frame_class[0]), &(pSBRItem->bs_pointer[0]),
                        pFIState0->freqRes, pFIState0->bordersEnv,
                        pFIState0->bordersNoise, &(pFIState0->nEnv),
                        &(pFIState0->nNoiseEnv), &error );

      if(error)
        return error;

      ret_cnt += sbr_grid(BS, &(pSBRItem->bs_frame_class[1]), &(pSBRItem->bs_pointer[1]),
                          pFIState1->freqRes, pFIState1->bordersEnv,
                          pFIState1->bordersNoise, &(pFIState1->nEnv),
                          &(pFIState1->nNoiseEnv), &error );

      if(error)
        return error;

    ret_cnt += sbr_dtdf(BS, pEDState0->bs_df_env, pEDState0->bs_df_noise, pFIState0->nEnv,
                        pFIState0->nNoiseEnv);
    ret_cnt += sbr_dtdf(BS, pEDState1->bs_df_env, pEDState1->bs_df_noise, pFIState1->nEnv,
                        pFIState1->nNoiseEnv);
    ret_cnt += sbr_invf(0, BS, pSBRItem);
    ret_cnt += sbr_invf(1, BS, pSBRItem);

    if ((pSBRItem->bs_frame_class[0] == FIXFIX) && (pFIState0->nEnv == 1))
      ampRes = 0;
    else
      ampRes = pSBRItem->sbrHeader.bs_amp_res;
    ret_cnt += sbr_envelope(0, 0, ampRes, BS, pSBRItem);
    if ((pSBRItem->bs_frame_class[1] == FIXFIX) && (pFIState1->nEnv == 1))
      ampRes = 0;
    else
      ampRes = pSBRItem->sbrHeader.bs_amp_res;
    ret_cnt += sbr_envelope(1, 0, ampRes, BS, pSBRItem);

    ret_cnt += sbr_noise(BS, pEDState0->bufNoiseQuant, pEDState0->vSizeNoise,
                         pEDState0->bs_df_noise, pSBRItem->sbrHuffTables, 0, 0,
                         pFIState0->nNoiseEnv, pFTState->nNoiseBand);

    ret_cnt += sbr_noise(BS, pEDState1->bufNoiseQuant, pEDState1->vSizeNoise,
                         pEDState1->bs_df_noise, pSBRItem->sbrHuffTables, 1, 0,
                         pFIState1->nNoiseEnv, pFTState->nNoiseBand);
  }

  GET_BITS(BS, pEDState0->bs_add_harmonic_flag, 1, Ipp32s);
  ret_cnt++;
  if (pEDState0->bs_add_harmonic_flag) {
    ret_cnt += sbr_sinusoidal_coding(BS, pEDState0->bs_add_harmonic, pFTState->nHiBand);
  } else {
    Ipp32s     i;

    for (i = 0; i < MAX_NUM_ENV_VAL; i++) {
      pEDState0->bs_add_harmonic[i] = 0;
    }
  }
  GET_BITS(BS, pEDState1->bs_add_harmonic_flag, 1, Ipp32s);
  ret_cnt++;
  if (pEDState1->bs_add_harmonic_flag) {
    ret_cnt += sbr_sinusoidal_coding(BS, pEDState1->bs_add_harmonic, pFTState->nHiBand);
  } else {
    Ipp32s     i;

    for (i = 0; i < MAX_NUM_ENV_VAL; i++) {
      pEDState1->bs_add_harmonic[i] = 0;
    }
  }

  GET_BITS(BS, bs_extended_data, 1, Ipp32s);
  ret_cnt++;
  if (bs_extended_data) {
    GET_BITS(BS, cnt, 4, Ipp32s);
    bs_extension_size = cnt;
    ret_cnt += 4;
    if (cnt == 15) {
      GET_BITS(BS, bs_esc_count, 8, Ipp32s);
      ret_cnt += 8;
      cnt += bs_esc_count;
    }

    num_bits_left = 8 * cnt;
    while (num_bits_left > 7) {
      GET_BITS(BS, bs_extension_id, 2, Ipp32s);
      ret_cnt += 2;
      num_bits_left -= 2;

      /* PS PARSER */
      GET_BITS_COUNT(BS, decodeBitsStart);
      /* patch. you can turn off support of PS decoder */
      if( NULL == pSBRItem->psState && EXTENSION_ID_PS == bs_extension_id){
        bs_extension_id = !EXTENSION_ID_PS;
      }
      error = sbr_extension(BS, pSBRItem->psState, bs_extension_id);

      if (error) return SBR_ERR_PARSER;

      GET_BITS_COUNT(BS, decodeBitsStop);

      ret_cnt       += ( decodeBitsStop - decodeBitsStart );
      num_bits_left -= ( decodeBitsStop - decodeBitsStart );
    }
  }

  return ret_cnt;
}

/********************************************************************/

static Ipp32s sbr_channel_pair_base_element(sBitsreamBuffer * BS, sSBRDecComState * pSBRItem)
{
  Ipp32s ret_cnt = 0;
  Ipp32s cnt = 0;
  Ipp32s num_bits_left = 0;

  Ipp32s bs_data_extra;
  Ipp32s bs_reserved;
  Ipp32s bs_esc_count;

  Ipp32s bs_extended_data  = 0;
  Ipp32s bs_extension_size = 0;
  Ipp32s bs_extension_id   = 0;

  sSBRFrameInfoState* pFIState0 = &(pSBRItem->sbrFIState[0]);

  sSBREnvDataState*   pEDState0 = &(pSBRItem->sbrEDState[0]);

  Ipp32s decodeBitsStart = 0, decodeBitsStop = 0, error = 0;


  GET_BITS(BS, bs_data_extra, 1, Ipp32s);
  ret_cnt++;
  if (bs_data_extra) {
    GET_BITS(BS, bs_reserved, 8, Ipp32s);
    ret_cnt += 8;
  }

  GET_BITS(BS, pSBRItem->bs_coupling, 1, Ipp32s);
  ret_cnt++;

  ret_cnt += sbr_grid(BS, &(pSBRItem->bs_frame_class[0]), &(pSBRItem->bs_pointer[0]),
                      pFIState0->freqRes, pFIState0->bordersEnv,
                      pFIState0->bordersNoise, &(pFIState0->nEnv),
                      &(pFIState0->nNoiseEnv), &error );

  if(error)
    return error;

  ret_cnt += sbr_dtdf(BS, pEDState0->bs_df_env, pEDState0->bs_df_noise,
                          pFIState0->nEnv, pFIState0->nNoiseEnv);
  ret_cnt += sbr_invf(0, BS, pSBRItem);

  ret_cnt += sbr_envelope(0, 1, pSBRItem->sbrHeader.bs_amp_res, BS, pSBRItem);

  ret_cnt += sbr_noise(BS, pEDState0->bufNoiseQuant, pEDState0->vSizeNoise,
                       pEDState0->bs_df_noise, pSBRItem->sbrHuffTables, 0, 1,
                       pFIState0->nNoiseEnv, pSBRItem->sbrFreqTabsState.nNoiseBand);

  GET_BITS(BS, pEDState0->bs_add_harmonic_flag, 1, Ipp32s);
  ret_cnt++;
  if (pEDState0->bs_add_harmonic_flag) {
    ret_cnt += sbr_sinusoidal_coding(BS, pEDState0->bs_add_harmonic, pSBRItem->sbrFreqTabsState.nHiBand);
  } else {
    Ipp32s     i;

    for (i = 0; i < MAX_NUM_ENV_VAL; i++) {
      pEDState0->bs_add_harmonic[i] = 0;
    }
  }

  GET_BITS(BS, bs_extended_data, 1, Ipp32s);
  ret_cnt++;
  if (bs_extended_data) {
    GET_BITS(BS, cnt, 4, Ipp32s);
    bs_extension_size = cnt;
    ret_cnt += 4;
    if (cnt == 15) {

   GET_BITS(BS, bs_esc_count, 8, Ipp32s);
      ret_cnt += 8;
    }

    num_bits_left = 8 * cnt;
    while (num_bits_left > 7) {
      GET_BITS(BS, bs_extension_id, 2, Ipp32s);
      ret_cnt += 2;
      num_bits_left -= 2;

      /* PS PARSER */
      GET_BITS_COUNT(BS, decodeBitsStart);
      /* patch. you can turn off support of PS decoder */
      if( NULL == pSBRItem->psState && EXTENSION_ID_PS == bs_extension_id){
        bs_extension_id = !EXTENSION_ID_PS;
      }
      error = sbr_extension(BS, pSBRItem->psState, bs_extension_id);

      if (error) return SBR_ERR_PARSER;

      GET_BITS_COUNT(BS, decodeBitsStop);

      ret_cnt       += ( decodeBitsStop - decodeBitsStart );
      num_bits_left -= ( decodeBitsStop - decodeBitsStart );
    }
  }

  return ret_cnt;
}

/********************************************************************/

static Ipp32s sbr_channel_pair_enhance_element(sBitsreamBuffer * BS,
                                               sSBRDecComState * pSBRItem)
{
  Ipp32s     ret_cnt = 0;
  sSBRFrameInfoState* pFIState1 = &(pSBRItem->sbrFIState[1]);

//  sSBREnvDataState*   pEDState0 = &(pSBRItem->sbrEDState[0]);
  sSBREnvDataState*   pEDState1 = &(pSBRItem->sbrEDState[1]);

  ret_cnt += sbr_dtdf(BS, pEDState1->bs_df_env, pEDState1->bs_df_noise,
                          pFIState1->nEnv, pFIState1->nNoiseEnv);

  ret_cnt += sbr_envelope(1, 1, pSBRItem->sbrHeader.bs_amp_res, BS, pSBRItem);
  ret_cnt += sbr_noise(BS, pEDState1->bufNoiseQuant, pEDState1->vSizeNoise,
                       pEDState1->bs_df_noise,
                       pSBRItem->sbrHuffTables, 1, 1, pFIState1->nNoiseEnv,
                       pSBRItem->sbrFreqTabsState.nNoiseBand);

  GET_BITS(BS, pEDState1->bs_add_harmonic_flag, 1, Ipp32s);
  ret_cnt += pEDState1->bs_add_harmonic_flag;
  ret_cnt++;
  if (pEDState1->bs_add_harmonic_flag) {
    ret_cnt += sbr_sinusoidal_coding(BS, pEDState1->bs_add_harmonic, pSBRItem->sbrFreqTabsState.nHiBand);
  } else {
    Ipp32s     i;

    for (i = 0; i < MAX_NUM_ENV_VAL; i++) {
      pEDState1->bs_add_harmonic[i] = 0;
    }
  }

  return ret_cnt;
}

/********************************************************************/

static
Ipp32s sbr_data(sBitsreamBuffer * BS,
                Ipp32s id_aac,
                sSBRDecComState * pSBRItem)
{
  Ipp32s cnt = 0;
  Ipp32s error = 0;

  switch (pSBRItem->sbr_layer) {
  case SBR_NOT_SCALABLE:
    switch (id_aac) {
    case ID_SCE:

/* parser */
      error = sbr_single_channel_element(BS, pSBRItem);

      if(SBR_ERR_REQUIREMENTS == error)
        return error;
      else
        cnt = error;

/* algorithm decode */
      error = sbrEnvNoiseDec(pSBRItem, 0);
      if (error)
        return error;

      break;

    case ID_CPE:

/* parser */
      error = sbr_channel_pair_element(BS, pSBRItem);

      if(SBR_ERR_REQUIREMENTS == error)
        return error;
      else
        cnt = error;

/* algorithm decode */
      error = sbrEnvNoiseDec(pSBRItem, 0);
      if (error)
        return error;

      error = sbrEnvNoiseDec(pSBRItem, 1);
      if (error)
        return error;

      break;
    }
    break;

  case SBR_MONO_BASE:
    error = sbr_channel_pair_base_element(BS, pSBRItem);

    if(SBR_ERR_REQUIREMENTS == error)
        return error;
      else
        cnt = error;

    break;

  case SBR_STEREO_ENHANCE:
    error = sbr_channel_pair_enhance_element(BS, pSBRItem);

    if(SBR_ERR_REQUIREMENTS == error)
        return error;
      else
        cnt = error;

    break;

  case SBR_STEREO_BASE:
    error = sbr_channel_pair_element(BS, pSBRItem);

    if(SBR_ERR_REQUIREMENTS == error)
        return error;
      else
        cnt = error;

    break;
  }
/*
  if (error) {
    printf("\nthere is error %i\n", error);
  }
 */
  return cnt;
}

/********************************************************************/

static Ipp32s GetValSbrLayer(void)
{
  return SBR_NOT_SCALABLE;
}

/********************************************************************/

Ipp32s sbr_extension_data(Ipp32s id_aac,
                          Ipp32s crc_flag,
                          sSBRDecComState * pSBRItem,
                          sBitsreamBuffer * BS,
                          Ipp32s cnt)
{
  Ipp32s num_sbr_bits = 0;
  Ipp32s num_align_bits;
  Ipp32s sbr_layer = 0;
  Ipp32s ret_val;
  Ipp32s i;
  Ipp32s bs_header_flag;
  Ipp32s error = 0;
  Ipp32s k2 = 0;
  Ipp32s tmp = 0;
  sSBRHeader* pSBRHeader = &(pSBRItem->sbrHeader);
  sSBRFeqTabsState* pFTState = &(pSBRItem->sbrFreqTabsState);

  if (crc_flag) {
    GET_BITS(BS, pSBRItem->bs_sbr_crc_bits, 10, Ipp32s);
    num_sbr_bits += 10;
  }

  sbr_layer = GetValSbrLayer();
  pSBRItem->sbr_layer = sbr_layer;

  if (sbr_layer != SBR_STEREO_ENHANCE) {
    num_sbr_bits += 1;
    GET_BITS(BS, bs_header_flag, 1, Ipp32s);

    if (bs_header_flag) {
      pSBRItem->sbrHeaderFlagPresent++;

      //num_sbr_bits += sbr_header(BS, pSBRHeader );

      tmp = error = sbr_header(BS, pSBRHeader );

      if( error == SBR_ERR_REQUIREMENTS ){
        pSBRItem->sbrFlagError = SBR_ERR_REQUIREMENTS;
        return SBR_ERR_REQUIREMENTS;

      }else{
        num_sbr_bits += tmp;
        error = 0;

        if(pSBRItem->sbrFlagError == SBR_ERR_REQUIREMENTS) {
          pSBRItem->sbrHeader.Reset = 1;
          pSBRItem->sbrFlagError = SBR_NO_ERR;
        }

      }
    }

    if ((pSBRItem->sbrHeaderFlagPresent != 0) && (pSBRItem->sbrHeader.Reset == 1) && (pSBRItem->sbrFlagError == SBR_NO_ERR)) {

      error = sbrCalcMasterFreqBoundary(pSBRHeader->bs_start_freq,
                                        pSBRHeader->bs_stop_freq,
                                        pSBRItem->sbr_freq_indx,
                                        &(pSBRItem->k0), &k2 );

      if (error)
        return error;

      error = sbrCalcMasterFreqBandTab( // in data
                                       pSBRItem->k0, k2,
                                       pSBRHeader->bs_freq_scale,
                                       pSBRHeader->bs_alter_scale,
                                       // out data
                                       pFTState->fMasterBandTab,
                                       &(pFTState->nMasterBand));

      if (error)
        return error;

      error = sbrCalcDerivedFreqTabs(pFTState,
                                     pSBRHeader->bs_xover_band,
                                     pSBRHeader->bs_noise_bands,
                                     k2,
                                     &(pSBRItem->kx),
                                     &(pSBRItem->M) );

      if (error)
        return error;

      error =  sbrCalcPatchConstructTab(pFTState->fMasterBandTab,
                                        pFTState->nMasterBand,
                                        pSBRItem->M,
                                        pSBRItem->kx,
                                        pSBRItem->k0,
                                        pSBRItem->sbr_freq_indx,

                                        pFTState->patchNumSubbandsTab,
                                        pFTState->patchStartSubbandTab,
                                        &(pFTState->numPatches) );


      if (error)
        return error;

      error = sbrCalcLimBandTab(pSBRHeader->bs_limiter_bands,
                                pFTState->fLoBandTab,
                                pFTState->nLoBand,
                                pFTState->numPatches,
                                pFTState->patchNumSubbandsTab,

                                pFTState->fLimBandTab,
                                &(pFTState->nLimBand));
      if (error)
        return error;

    } else {
      /* NOTHING */
    }
  }

  if (pSBRItem->sbrHeaderFlagPresent != 0 && pSBRItem->sbrFlagError == SBR_NO_ERR) {
    //num_sbr_bits += sbr_data(BS, id_aac, pSBRItem);
      error = sbr_data(BS, id_aac, pSBRItem);
      if(SBR_ERR_REQUIREMENTS == error)
        return error;
      else
        num_sbr_bits += error;
  }

  pSBRItem->cnt_bit -= num_sbr_bits;
  num_align_bits = 8 * cnt - 4 - num_sbr_bits;

  for (i = 0; i < (num_align_bits >> 5); i++) {
    GET_BITS(BS, tmp, 32, Ipp32s);
  }

  if ((num_align_bits - (num_align_bits & (~0x1F))) != 0) {
    GET_BITS(BS, tmp, (num_align_bits - (num_align_bits & (~0x1F))), Ipp32s);
  }

  ret_val = (num_sbr_bits + num_align_bits + 4) / 8;

  return ret_val;

}

/********************************************************************/

static Ipp32s middleBorder(Ipp32s bs_frame_class, Ipp32s bs_pointer, Ipp32s nEnv)
{
  Ipp32s indx1, indx2, ret_val;
  Ipp32s TableMidBorder[3][3];

  TableMidBorder[2][0] = TableMidBorder[1][0] = TableMidBorder[0][0] = nEnv / 2;
  TableMidBorder[0][1] = 1;
  TableMidBorder[1][1] = TableMidBorder[1][2] = TableMidBorder[0][2] = nEnv - 1;
  TableMidBorder[2][1] = bs_pointer - 1;
  TableMidBorder[2][2] = nEnv + 1 - bs_pointer;

  if ((bs_pointer > 1) || (bs_pointer < 0))
    indx1 = 2;
  else
    indx1 = bs_pointer;

  if (bs_frame_class == FIXFIX)
    indx2 = 0;
  else if (bs_frame_class == VARFIX)
    indx2 = 1;
  else
    indx2 = 2;

  ret_val = TableMidBorder[indx1][indx2];

  return ret_val;
}

/********************************************************************/

Ipp32s sbr_grid(sBitsreamBuffer * BS, Ipp32s* bs_frame_class, Ipp32s* bs_pointer, Ipp32s* freqRes, Ipp32s* bordersEnv,
                Ipp32s* bordersNoise, Ipp32s* nEnv, Ipp32s* nNoiseEnv, Ipp32s* status)
{
  Ipp32s ret_cnt = 0;
  Ipp32s env, rel;
  Ipp32s ptr_bits = 0;
  Ipp32s tmp;
  Ipp32s absBordLead = 0;
  Ipp32s absBordTrail = 0;
  Ipp32s bs_var_bord_0 = 0;
  Ipp32s bs_var_bord_1 = 0;
  Ipp32s bs_num_rel_1 = 0;
  Ipp32s bs_num_rel_0 = 0;
  Ipp32s n_RelLead = 0;
  Ipp32s n_RelTrail = 0;
  Ipp32s bs_rel_bord_0[32];
  Ipp32s bs_rel_bord_1[32];
  Ipp32s relBordLead[32];
  Ipp32s relBordTrail[32];
  Ipp32s l, i;
  Ipp32s tmp_32s;
  const Ipp32s TABLE_PTR_BITS[6] = { 0, 1, 2, 2, 3, 3 };

  /* patch */
  *status = 0;

  /* AYA: MAY BE POTENCIAL BUG? */
  GET_BITS(BS, *bs_frame_class, 2, Ipp32s);
  ret_cnt += 2;

  switch ( *bs_frame_class ) {
  case FIXFIX:
    GET_BITS(BS, tmp, 2, Ipp32s);
    ret_cnt += 2;
    *nEnv = 1 << tmp;

    /* patch */
    if( *nEnv > 5 ){
      *status = SBR_ERR_REQUIREMENTS;
      return SBR_ERR_REQUIREMENTS;
    }

    GET_BITS(BS, freqRes[0], 1, Ipp32s);
    ret_cnt++;

    for (env = 1; env < (*nEnv); env++) {
      freqRes[env] = freqRes[0];
    }

// TimeFrequency Grid
    absBordLead = 0;
    absBordTrail = NUM_TIME_SLOTS;
    n_RelLead = *nEnv - 1;
    n_RelTrail = 0;

     tmp_32s = NUM_TIME_SLOTS / (*nEnv);
    for (l = 0; l < n_RelLead; l++) {
      relBordLead[l] = tmp_32s;
    }
// relBordTrail - N/A

    break;

  case FIXVAR:
    GET_BITS(BS, bs_var_bord_1, 2, Ipp32s);
    ret_cnt += 2;
    GET_BITS(BS, bs_num_rel_1, 2, Ipp32s);
    ret_cnt += 2;
    *nEnv = bs_num_rel_1 + 1;

    /* patch */
    if( *nEnv > 5 ){
      *status = SBR_ERR_REQUIREMENTS;
      return SBR_ERR_REQUIREMENTS;
    }

    for (rel = 0; rel < (*nEnv) - 1; rel++) {
      GET_BITS(BS, tmp, 2, Ipp32s);
      ret_cnt += 2;
      bs_rel_bord_1[rel] = 2 * tmp + 2;
    }

    ptr_bits = TABLE_PTR_BITS[ *nEnv ];
    GET_BITS(BS, *bs_pointer, ptr_bits, Ipp32s);
    ret_cnt += ptr_bits;
    for (env = 0; env < (*nEnv); env++) {
      GET_BITS(BS, freqRes[ *nEnv - 1 - env], 1, Ipp32s);
      ret_cnt++;
    }

// TimeFrequency Grid
    absBordLead = 0;
    absBordTrail = bs_var_bord_1 + NUM_TIME_SLOTS;
    n_RelLead = 0;
    n_RelTrail = bs_num_rel_1;
// relBordLead[l] - N/A
    for (l = 0; l < n_RelTrail; l++) {
      relBordTrail[l] = bs_rel_bord_1[l];
    }

    break;

  case VARFIX:
    GET_BITS(BS, bs_var_bord_0, 2, Ipp32s);
    ret_cnt += 2;
    GET_BITS(BS, bs_num_rel_0, 2, Ipp32s);
    ret_cnt += 2;
    *nEnv = bs_num_rel_0 + 1;

    /* patch */
    if( *nEnv > 5 ){
      *status = SBR_ERR_REQUIREMENTS;
      return SBR_ERR_REQUIREMENTS;
    }

    for (rel = 0; rel < (*nEnv) - 1; rel++) {
      GET_BITS(BS, tmp, 2, Ipp32s);
      ret_cnt += 2;
      bs_rel_bord_0[rel] = 2 * tmp + 2;
    }
    ptr_bits = TABLE_PTR_BITS[ *nEnv ];
    GET_BITS(BS, *bs_pointer, ptr_bits, Ipp32s);
    ret_cnt += ptr_bits;
    for (env = 0; env < (*nEnv); env++) {
      GET_BITS(BS, freqRes[env], 1, Ipp32s);
      ret_cnt++;
    }

// TimeFrequency Grid
    absBordLead = bs_var_bord_0;
    absBordTrail = NUM_TIME_SLOTS;
    n_RelLead = bs_num_rel_0;
    n_RelTrail = 0;
    for (l = 0; l < n_RelLead; l++) {
      relBordLead[l] = bs_rel_bord_0[l];
    }
// relBordTrail - N/A

    break;

  case VARVAR:
    GET_BITS(BS, bs_var_bord_0, 2, Ipp32s);
    ret_cnt += 2;
    GET_BITS(BS, bs_var_bord_1, 2, Ipp32s);
    ret_cnt += 2;
    GET_BITS(BS, bs_num_rel_0, 2, Ipp32s);
    ret_cnt += 2;
    GET_BITS(BS, bs_num_rel_1, 2, Ipp32s);
    ret_cnt += 2;

    *nEnv = bs_num_rel_0 + bs_num_rel_1 + 1;

    /* patch */
    if( *nEnv > 5 ){
      *status = SBR_ERR_REQUIREMENTS;
      return SBR_ERR_REQUIREMENTS;
    }

    for (rel = 0; rel < bs_num_rel_0; rel++) {
      GET_BITS(BS, tmp, 2, Ipp32s);
      ret_cnt += 2;
      bs_rel_bord_0[rel] = 2 * tmp + 2;
    }
    for (rel = 0; rel < bs_num_rel_1; rel++) {
      GET_BITS(BS, tmp, 2, Ipp32s);
      ret_cnt += 2;
      bs_rel_bord_1[rel] = 2 * tmp + 2;
    }
// ptr_bits = (Ipp32s)ceil( log(pSBRItem->nEnv[ch]+1.f)/log(2.) );
    ptr_bits = TABLE_PTR_BITS[ *nEnv ];
    GET_BITS(BS, *bs_pointer, ptr_bits, Ipp32s);
    ret_cnt += ptr_bits;
    for (env = 0; env < (*nEnv); env++) {
      GET_BITS(BS, freqRes[env], 1, Ipp32s);
      ret_cnt++;
    }

// TimeFrequency Grid
    absBordLead = bs_var_bord_0;
    absBordTrail = bs_var_bord_1 + NUM_TIME_SLOTS;
    n_RelLead = bs_num_rel_0;
    n_RelTrail = bs_num_rel_1;
    for (l = 0; l < n_RelLead; l++) {
      relBordLead[l] = bs_rel_bord_0[l];
    }
    for (l = 0; l < n_RelTrail; l++) {
      relBordTrail[l] = bs_rel_bord_1[l];
    }

    break;
  }     // end of switch

  *nNoiseEnv = ((*nEnv) > 1) ? 2 : 1;

  /* diagnostic */
  {
    Ipp32s tmp_max = 0;
    ippsZero_32s(bordersEnv, MAX_NUM_ENV+1);
    ippsMax_32s(bordersEnv, MAX_NUM_ENV+1, &tmp_max);
  }
// TimeFrequency Grid
  bordersEnv[0] = absBordLead;
  bordersEnv[ *nEnv ] = absBordTrail;

  for (l = 1; l <= n_RelLead; l++) {
    tmp_32s = 0;
    for (i = 0; i < l; i++) {
      tmp_32s += relBordLead[i];
    }
    bordersEnv[l] = absBordLead + tmp_32s;
  }
  for (l = n_RelLead + 1; l < (*nEnv); l++) {
    tmp_32s = 0;
    for (i = 0; i < (*nEnv) - l; i++) {
      tmp_32s += relBordTrail[i];
    }
    bordersEnv[l] = absBordTrail - tmp_32s;
  }

  /* diagnostic */
  {
    Ipp32s tmp_max = 0;
    ippsMax_32s(bordersEnv, MAX_NUM_ENV+1, &tmp_max);

    if ( tmp_max * RATE >= (NUM_TIME_SLOTS * RATE + SBR_TIME_HFGEN) ) {
      *status = SBR_ERR_REQUIREMENTS;
      return SBR_ERR_REQUIREMENTS;
    }
  }

  if ( (*nEnv) == 1) {
    bordersNoise[0] = bordersEnv[0];
    bordersNoise[1] = bordersEnv[1];
  } else {
    bordersNoise[0] = bordersEnv[0];
    tmp_32s =
      middleBorder( *bs_frame_class, *bs_pointer, *nEnv);
    bordersNoise[1] = bordersEnv[tmp_32s];
    bordersNoise[2] = bordersEnv[ *nEnv ];
  }

  return ret_cnt;
}

/********************************************************************/

void sbr_grid_coupling(sSBRDecComState * pSBRItem)
{
  Ipp32s env, l;

  sSBRFrameInfoState* pFIState0 = &(pSBRItem->sbrFIState[0]);
  sSBRFrameInfoState* pFIState1 = &(pSBRItem->sbrFIState[1]);

  pSBRItem->bs_frame_class[1] = pSBRItem->bs_frame_class[0];

  switch (pSBRItem->bs_frame_class[1]) {
  case FIXFIX:
    pFIState1->nEnv = pFIState0->nEnv;
    pFIState1->freqRes[0] = pFIState0->freqRes[0];
    for (env = 0; env < pFIState1->nEnv; env++) {
      pFIState1->freqRes[env] = pFIState1->freqRes[0];
    }

    break;

  case FIXVAR:
    pFIState1->nEnv = pFIState0->nEnv;

    pSBRItem->bs_pointer[1] = pSBRItem->bs_pointer[0];
    for (env = 0; env < pFIState1->nEnv; env++)
      pFIState1->freqRes[pFIState1->nEnv - 1 - env] = pFIState0->freqRes[pFIState1->nEnv - 1 - env];

    break;

  case VARFIX:
    pFIState1->nEnv = pFIState0->nEnv;

    pSBRItem->bs_pointer[1] = pSBRItem->bs_pointer[0];
    for (env = 0; env < pFIState1->nEnv; env++)
      pFIState1->freqRes[env] = pFIState0->freqRes[env];

    break;

  case VARVAR:
    pFIState1->nEnv = pFIState0->nEnv;
    pSBRItem->bs_pointer[1] = pSBRItem->bs_pointer[0];
    for (env = 0; env < pFIState1->nEnv; env++)
      pFIState1->freqRes[env] = pFIState0->freqRes[env];

    break;
  }

  pFIState1->nNoiseEnv = pFIState0->nNoiseEnv;

  for (l = 0; l <= pFIState1->nEnv; l++) {
    pFIState1->bordersEnv[l] = pFIState0->bordersEnv[l];
  }

  for (l = 0; l <= pFIState1->nNoiseEnv; l++) {
    pFIState1->bordersNoise[l] = pFIState0->bordersNoise[l];
  }
}

/********************************************************************/

Ipp32s sbr_dtdf(sBitsreamBuffer * BS, Ipp32s* bs_df_env, Ipp32s* bs_df_noise, Ipp32s nEnv, Ipp32s nNoiseEnv)
{
  Ipp32s ret_cnt = 0;
  Ipp32s env;
  Ipp32s noise;

  for (env = 0; env < nEnv; env++) {
    GET_BITS(BS, bs_df_env[env], 1, Ipp32s);
    ret_cnt++;
  }
  for (noise = 0; noise < nNoiseEnv; noise++) {
    GET_BITS(BS, bs_df_noise[noise], 1, Ipp32s);
    ret_cnt++;
  }

  return ret_cnt;
}

/********************************************************************/

Ipp32s sbr_invf(Ipp32s ch, sBitsreamBuffer * BS, sSBRDecComState * pSBRItem)
{
  Ipp32s ret_cnt = 0;
  Ipp32s n;
  Ipp32s num_noise_bands = pSBRItem->sbrFreqTabsState.nNoiseBand;

  if (pSBRItem->bs_coupling == 1) {
    for (n = 0; n < num_noise_bands; n++) {
      GET_BITS(BS, pSBRItem->bs_invf_mode[ch][n], 2, Ipp32s);
      pSBRItem->bs_invf_mode[1][n] = pSBRItem->bs_invf_mode[ch][n];
      ret_cnt += 2;
    }
  } else {
    for (n = 0; n < num_noise_bands; n++) {
      GET_BITS(BS, pSBRItem->bs_invf_mode[ch][n], 2, Ipp32s);
      ret_cnt += 2;
    }
  }

  return ret_cnt;
}

/********************************************************************/

static Ipp16s sbr_huff_dec(void *t_huff, sBitsreamBuffer * pBS, Ipp32s *cnt, Ipp16s LAV)
{
  Ipp16s index = 0;
  Ipp32s tmp = pBS->nBit_offset;
  Ipp32s num_bit_read = 0;
  Ipp8u  *pSrc;
  Ipp32s bitoffset;

  pSrc = (Ipp8u *)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
  bitoffset = (32 - pBS->nBit_offset) & 0x7;

  ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, &index,
                         (IppsVLCDecodeSpec_32s *) t_huff);

  pBS->pCurrent_dword = (Ipp32u *)(pSrc - ((size_t)(pSrc) & 3));
  pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
  pBS->nBit_offset =
    (Ipp32s)(32 - ((pSrc - (Ipp8u *)pBS->pCurrent_dword) << 3) - bitoffset);
/*
 * patch
 */
  if (pBS->nBit_offset < tmp)
    num_bit_read = tmp - pBS->nBit_offset;
  else
    num_bit_read = tmp + (32 - pBS->nBit_offset);

  *cnt = (*cnt) + num_bit_read;

  return index - LAV;
}

/********************************************************************/

Ipp32s sbr_envelope(Ipp32s ch, Ipp32s bs_coupling, Ipp32s bs_amp_res, sBitsreamBuffer * BS,
                    sSBRDecComState * pSBRItem)
{
  Ipp32s env;
  Ipp32s ret_cnt = 0;
  Ipp32s band;
  Ipp32s num_env_bands[2];     // = {pSBRItem->n[0], pSBRItem->n[1]};
  Ipp16s bs_env_start_value_balance;
  Ipp16s bs_env_start_value_level;
  void   *t_huff;
  void   *f_huff;
  Ipp16s LAV;

  sSBREnvDataState* pEDState = &(pSBRItem->sbrEDState[ch]);

  sSBRFrameInfoState* pFIState = &(pSBRItem->sbrFIState[ch]);

  Ipp32s*   sizeEnv = pEDState->vSizeEnv;

  num_env_bands[0] = pSBRItem->sbrFreqTabsState.nLoBand;// n[0];
  num_env_bands[1] = pSBRItem->sbrFreqTabsState.nHiBand;//  n[1];

  /* patch */
  sizeEnv[0] = 0;
  for(env = 1; env <= pFIState->nEnv; env++)
    sizeEnv[env] = sizeEnv[env-1] + num_env_bands[pFIState->freqRes[env-1]];
  // LE - total size

  if (bs_coupling) {
    if (ch) {
      if (bs_amp_res) {
        t_huff = pSBRItem->sbrHuffTables[6];        // t_huffman_env_bal_3_0dB;//
        f_huff = pSBRItem->sbrHuffTables[7];        // f_huffman_env_bal_3_0dB;//
        LAV = 12;
      } else {
        t_huff = pSBRItem->sbrHuffTables[2];        // t_huffman_env_bal_1_5dB;//
        f_huff = pSBRItem->sbrHuffTables[3];        // f_huffman_env_bal_1_5dB;//
        LAV = 24;
      }
    } else {
      if (bs_amp_res) {
        t_huff = pSBRItem->sbrHuffTables[4];        // t_huffman_env_3_0dB;//
        f_huff = pSBRItem->sbrHuffTables[5];        // f_huffman_env_3_0dB;//
        LAV = 31;
      } else {
        t_huff = pSBRItem->sbrHuffTables[0];        // t_huffman_env_1_5dB;//
        f_huff = pSBRItem->sbrHuffTables[1];        // f_huffman_env_1_5dB;//
        LAV = 60;
      }
    }
  } else {
    if (bs_amp_res) {
      t_huff = pSBRItem->sbrHuffTables[4];  // t_huffman_env_3_0dB;//
      f_huff = pSBRItem->sbrHuffTables[5];  // f_huffman_env_3_0dB;//
      LAV = 31;
    } else {
      t_huff = pSBRItem->sbrHuffTables[0];  // t_huffman_env_1_5dB;//
      f_huff = pSBRItem->sbrHuffTables[1];  // f_huffman_env_1_5dB;//
      LAV = 60;
    }

  }

  for (env = 0; env < pFIState->nEnv; env++) {
    if (pEDState->bs_df_env[env] == 0) {
      if (bs_coupling && ch) {
        if (bs_amp_res) {
          GET_BITS(BS, bs_env_start_value_balance, 5, Ipp16s);
          pEDState->bufEnvQuant[sizeEnv[env]+0] = bs_env_start_value_balance;
          ret_cnt += 5;
        } else {
          GET_BITS(BS, bs_env_start_value_balance, 6, Ipp16s);
          pEDState->bufEnvQuant[sizeEnv[env]+0] = bs_env_start_value_balance;
          ret_cnt += 6;
        }
      } else {
        if (bs_amp_res) {
          GET_BITS(BS, bs_env_start_value_level, 6, Ipp16s);
          pEDState->bufEnvQuant[sizeEnv[env]+0] = bs_env_start_value_level;
          ret_cnt += 6;
        } else {
          GET_BITS(BS, bs_env_start_value_level, 7, Ipp16s);
          pEDState->bufEnvQuant[sizeEnv[env]+0] = bs_env_start_value_level;
          ret_cnt += 7;
        }
      }
      for (band = 1; band < num_env_bands[pFIState->freqRes[env]]; band++) {
        pEDState->bufEnvQuant[sizeEnv[env]+band] =
          sbr_huff_dec(f_huff, BS, &ret_cnt, LAV);

        pEDState->bufEnvQuant[sizeEnv[env]+band] = SBR_ENV_PROTECT(pEDState->bufEnvQuant[sizeEnv[env]+band]);
      }
    } else {
      for (band = 0; band < num_env_bands[pFIState->freqRes[env]]; band++) {
        pEDState->bufEnvQuant[sizeEnv[env]+band] =
          sbr_huff_dec(t_huff, BS, &ret_cnt, LAV);

        pEDState->bufEnvQuant[sizeEnv[env]+band] = SBR_ENV_PROTECT(pEDState->bufEnvQuant[sizeEnv[env]+band]);
      }
    }
  }

  return ret_cnt;
}

/********************************************************************/

Ipp32s sbr_noise(sBitsreamBuffer* BS, Ipp16s* vNoise, Ipp32s* vSize, Ipp32s* bs_df_noise,
                 void* sbrHuffTables[10], Ipp32s ch, Ipp32s bs_coupling, Ipp32s nNoiseEnv, Ipp32s NQ)
{
  Ipp32s ret_cnt = 0;
  Ipp32s noise;
  Ipp32s band;
  Ipp16s bs_noise_start_value_balance;
  Ipp16s bs_noise_start_value_level;
  void   *t_huff;
  void   *f_huff;
  Ipp16s LAV;

  vSize[0] = 0;
  vSize[1] = NQ;
  // total size
  vSize[2] = 2*NQ;

  if (bs_coupling) {
    if (ch) {
      t_huff = sbrHuffTables[9];  // t_huffman_noise_bal_3_0dB;//
      f_huff = sbrHuffTables[7];  // f_huffman_env_bal_3_0dB;//
      LAV = 12;
    } else {
      t_huff = sbrHuffTables[8];  // t_huffman_noise_3_0dB;//
      f_huff = sbrHuffTables[5];  // f_huffman_env_3_0dB;//
      LAV = 31;
    }
  } else {
    t_huff = sbrHuffTables[8];    // t_huffman_noise_3_0dB;//
    f_huff = sbrHuffTables[5];    // f_huffman_env_3_0dB;//
    LAV = 31;
  }

  for (noise = 0; noise < nNoiseEnv; noise++) {
    if (bs_df_noise[noise] == 0) {
      if (bs_coupling && ch) {
        GET_BITS(BS, bs_noise_start_value_balance, 5, Ipp16s);
        vNoise[vSize[noise]] = bs_noise_start_value_balance;

      } else {
        GET_BITS(BS, bs_noise_start_value_level, 5, Ipp16s);
        vNoise[vSize[noise]] = bs_noise_start_value_level;
      }
      ret_cnt += 5;

      for (band = 1; band < NQ; band++) {
        vNoise[vSize[noise] + band] =
          sbr_huff_dec(f_huff, BS, &ret_cnt, LAV);

        vNoise[vSize[noise] + band] = SBR_NOISE_PROTECT(vNoise[vSize[noise] + band]);
      }
    } else {
      for (band = 0; band < NQ; band++) {
        vNoise[vSize[noise]+band] =
          sbr_huff_dec(t_huff, BS, &ret_cnt, LAV);

        vNoise[vSize[noise] + band] = SBR_NOISE_PROTECT(vNoise[vSize[noise] + band]);
      }
    }
  }

  return ret_cnt;
}

/********************************************************************/

Ipp32s sbr_sinusoidal_coding(sBitsreamBuffer * BS, Ipp32s* pDst, Ipp32s len)
{
  Ipp32s n;
  Ipp32s ret_cnt = 0;

  for (n = 0; n < len; n++) {
    GET_BITS(BS, pDst[n], 1, Ipp32s);
  }

  ret_cnt = len;

  return ret_cnt;
}

/* EOF */

#endif //UMC_ENABLE_XXX

