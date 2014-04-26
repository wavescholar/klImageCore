/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives AAC Decode Sample for Windows*
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <stdio.h>
#include "bstream.h"
/* HEAAC profile */
#include "aaccmn_const.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/***************************************************************************/

static Ipp32s enc_sbr_header(sSBRHeader* pState, sBitsreamBuffer* pBS)
{
  Ipp32s payloadBits = 0;
  Ipp32s bs_reserved = 0;

  //PUT_BITS(pBS, pState->bs_amp_res,    1);
  PUT_BITS(pBS, 1,    1);
  PUT_BITS(pBS, pState->bs_start_freq, 4);
  PUT_BITS(pBS, pState->bs_stop_freq,  4);
  PUT_BITS(pBS, pState->bs_xover_band, 3);
  PUT_BITS(pBS, bs_reserved,           2);
  PUT_BITS(pBS, pState->bs_extra_1,    1);
  PUT_BITS(pBS, pState->bs_extra_2,    1);

  payloadBits += 1 + 4 + 4 + 3 + 2 + 1 + 1;

  if (pState->bs_extra_1) {

    PUT_BITS(pBS, pState->bs_freq_scale,  2);
    PUT_BITS(pBS, pState->bs_alter_scale, 1);
    PUT_BITS(pBS, pState->bs_noise_bands, 2);

    payloadBits += 2 + 1 + 2;
  }

  if (pState->bs_extra_2){

    PUT_BITS(pBS, pState->bs_limiter_bands,  2);
    PUT_BITS(pBS, pState->bs_limiter_gains,  2);
    PUT_BITS(pBS, pState->bs_interpol_freq,  1);
    PUT_BITS(pBS, pState->bs_smoothing_mode, 1);

    payloadBits += 2 + 2 + 1 + 1;
  }


  return payloadBits;
}

/********************************************************************/

/* y = log2(x) */

static Ipp32s supLOG2(Ipp32s x)
{
  Ipp32s y = -1;

  while((1<<++y) < x);

  return y;
}

/********************************************************************/

static Ipp32s
enc_sbr_grid(sSBREnc_SCE_State* pState, sBitsreamBuffer* pBS)
{
  Ipp32s payloadBits = 0;
  Ipp32s bs_frame_class = pState->sbrGrid.frameClass;
  //Ipp32s bs_num_env = pState->sbrGrid.bs_num_env;
  Ipp32s bs_num_env = pState->sbrFIState.nEnv;
  Ipp32s temp = 0;
  Ipp32s env, rel;

  PUT_BITS(pBS, bs_frame_class,  2);
  payloadBits += 2;


  switch( bs_frame_class ) {
//---------------------------------
    case FIXFIX:
      temp = supLOG2( bs_num_env );

      PUT_BITS(pBS, temp,  2);
      payloadBits += 2;

      PUT_BITS(pBS, pState->sbrFIState.freqRes[0],  1);
      payloadBits += 1;

      break;
//---------------------------------
    case FIXVAR:
      //break;
    case VARFIX:

      temp = pState->sbrGrid.bs_abs_bord - 16;
      if ( VARFIX == bs_frame_class){
        temp = pState->sbrGrid.bs_abs_bord;
      }

      PUT_BITS(pBS, temp,  2);
      payloadBits += 2;

      PUT_BITS(pBS, pState->sbrGrid.n,  2);
      payloadBits += 2;

      for(rel = 0; rel < pState->sbrGrid.n; rel++){
        temp = (pState->sbrGrid.bs_rel_bord[rel] - 2) >> 1;

        PUT_BITS(pBS, temp,  2);
        payloadBits += 2;
      }

      temp = supLOG2( pState->sbrGrid.n + 2 );

      PUT_BITS(pBS, pState->sbrGrid.bs_pointer,  temp);
      payloadBits += temp;

      for(env = 0; env < pState->sbrGrid.n + 1; env++){
        PUT_BITS(pBS, pState->sbrGrid.bs_freq_res[env],  1);
        payloadBits += 1;
      }

      break;
//---------------------------------
    case VARVAR:
      temp = pState->sbrGrid.bs_abs_bord_0;

      PUT_BITS(pBS, temp,  2);
      payloadBits += 2;

      temp = pState->sbrGrid.bs_abs_bord_1 - 16;
      PUT_BITS(pBS, temp,  2);
      payloadBits += 2;

      PUT_BITS(pBS, pState->sbrGrid.bs_num_rel_0,  2);
      PUT_BITS(pBS, pState->sbrGrid.bs_num_rel_1,  2);
      payloadBits += 2 + 2;

      /* rel_0 */
      for(rel = 0; rel < pState->sbrGrid.bs_num_rel_0; rel++){
        temp = (pState->sbrGrid.bs_rel_bord_0[rel] - 2) >> 1;

        PUT_BITS(pBS, temp,  2);
        payloadBits += 2;
      }

      /* rel_1 */
      for(rel = 0; rel < pState->sbrGrid.bs_num_rel_1; rel++){
        temp = (pState->sbrGrid.bs_rel_bord_1[rel] - 2) >> 1;

        PUT_BITS(pBS, temp,  2);
        payloadBits += 2;
      }

      temp =  pState->sbrGrid.bs_num_rel_0 + pState->sbrGrid.bs_num_rel_1 + 2;
      temp = supLOG2( temp );

      /* bs_pointer */
      PUT_BITS(pBS, pState->sbrGrid.bs_pointer,  temp);
      payloadBits += temp;

      temp = pState->sbrGrid.bs_num_rel_0 + pState->sbrGrid.bs_num_rel_1 + 1;
      for(env = 0; env < temp; env++){
        PUT_BITS(pBS, pState->sbrGrid.bs_freq_res_LR[env],  1);
        payloadBits += 1;
      }

      break;
//---------------------------------
    default:
      break;
  }


  return payloadBits;
}

/********************************************************************/

static Ipp32s enc_sbr_dtdf(sSBREnc_SCE_State* pState, sBitsreamBuffer* pBS)
{
  Ipp32s payloadBits  = 0;
  Ipp32s nDataEnv  = pState->sbrFIState.nEnv;
  Ipp32s nNoiseEnv = (nDataEnv > 1) ? 2 : 1;
  Ipp32s env;

  /* data env */
  for(env = 0; env < nDataEnv; env++){
    PUT_BITS(pBS, pState->sbrEDState.bs_df_env[env],  1);
    payloadBits += 1;
  }

  /* noise envelope */
  for(env = 0; env < nNoiseEnv; env++){
    PUT_BITS(pBS, pState->sbrEDState.bs_df_noise[env],  1);
    payloadBits += 1;
  }

  return payloadBits;
}

/********************************************************************/

static Ipp32s enc_sbr_invf(sSBREnc_SCE_State* pState, sBitsreamBuffer* pBS, Ipp32s nNoiseEnv)
{
  Ipp32s payloadBits  = 0;
  Ipp32s env;

  for(env = 0; env < nNoiseEnv; env++){
    PUT_BITS(pBS, pState->sbrInvfEst.bs_invf_mode[env],  2);
    payloadBits += 2;
  }

  return payloadBits;
}

/********************************************************************/

static Ipp32s
 enc_sbr_envelope(sSBREnc_SCE_State* pState, IppsVLCEncodeSpec_32s*  sbrHuffTabs[NUM_SBR_HUFF_TABS],
                  sBitsreamBuffer* pBS, Ipp32s bs_amp_res, Ipp32s nLoBand, Ipp32s nHiBand)
{
  IppsVLCEncodeSpec_32s* pTimeHuffTab;
  IppsVLCEncodeSpec_32s* pFreqHuffTab;
  IppsVLCEncodeSpec_32s *pCurrHuffTab;

  Ipp32s *bs_df_env   = pState->sbrEDState.bs_df_env;
  Ipp32s *bs_freq_res = pState->sbrFIState.freqRes;
  Ipp16s *bs_data_env = pState->sbrEDState.bufEnvQuant;

  Ipp32s payloadBits  = 0;
  Ipp32s env;
  Ipp32s bs_num_env  = pState->sbrFIState.nEnv;
  Ipp32s bs_coupling = 0;
  //Ipp32s bs_amp_res  = pState->sbrHeader.bs_amp_res;
  Ipp32s band;
#if !defined(ANDROID)
  Ipp32s nSfBand[2];
#else
  static Ipp32s nSfBand[2];
#endif
  Ipp32s bs_start_env_bit = (bs_amp_res) ? 6 : 7;
  Ipp32s my_bs_start_env_bit;

  Ipp16s  LAV;
  Ipp16s  delta;
  Ipp32s  pos = 0;

  Ipp32s  bs_cw_bits;
  //Ipp32s  bs_codeword;
  Ipp8u  *pDst;
  Ipp32s bitoffset;

  /* init */
  nSfBand[0] = nLoBand; //pState->sbrFreqTabsState.nLoBand;
  nSfBand[1] = nHiBand; //pState->sbrFreqTabsState.nHiBand;

  /* alternativa */
  sbrencSetEnvHuffTabs(bs_amp_res,
                      &LAV,
                      &my_bs_start_env_bit,

                      &pTimeHuffTab,
                      &pFreqHuffTab,
                      sbrHuffTabs,

                      HUFF_ENV_COMPRESS); /* [1] - envelope, [0] - noise */

  for(env = 0; env < bs_num_env; env++){
    if ( 0 == bs_df_env[env] ){

      pCurrHuffTab  = pFreqHuffTab;

      if(bs_coupling){
        // none
      }else{
        PUT_BITS(pBS, bs_data_env[pos], bs_start_env_bit);
        pos++;
        payloadBits += bs_start_env_bit;
      }
    } else {

      pCurrHuffTab  = pTimeHuffTab;
    }

    /* LOG ------------------------------------------- */
    pDst = (Ipp8u*)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
    bitoffset = (32 - pBS->nBit_offset) & 0x7;
    SAVE_BITSTREAM(pBS)
    /* LOG ------------------------------------------- */

    for(band = 1 - bs_df_env[env]; band < nSfBand[ bs_freq_res[env] ]; band++ ){
      delta = bs_data_env[pos];
      pos++;

      /* AYA */
      //printf("\n dirty = %i \n", delta);
      bs_cw_bits = ownVLCCountBits_16s32s(delta + LAV, pCurrHuffTab);

      delta = delta + LAV;
      ippsVLCEncodeBlock_16s1u(&delta, 1, &pDst, &bitoffset, pCurrHuffTab);

      payloadBits += bs_cw_bits;
    } /* for(band=0; */
    /* LOG ------------------------------------------- */
    pBS->pCurrent_dword = (Ipp32u*)((size_t)(pDst) - ((size_t)(pDst) & 3));
    pBS->nBit_offset = (Ipp32s)(32 - ((pDst - (Ipp8u*)pBS->pCurrent_dword) << 3)- bitoffset);
    LOAD_DWORD(pBS)
    /* LOG ------------------------------------------- */
  }     /* for(env=0; */

  return payloadBits;
}

/********************************************************************/

static Ipp32s
enc_sbr_noise(sSBREnc_SCE_State* pState, IppsVLCEncodeSpec_32s*  sbrHuffTabs[NUM_SBR_HUFF_TABS],
              sBitsreamBuffer* pBS, Ipp32s nNoiseBand)
{
  IppsVLCEncodeSpec_32s* pTimeHuffTab;
  IppsVLCEncodeSpec_32s* pFreqHuffTab;
  IppsVLCEncodeSpec_32s *pCurrHuffTab;

  Ipp32s payloadBits = 0;
  Ipp32s nDataEnv  = pState->sbrFIState.nEnv;
  Ipp32s nNoiseEnv = (nDataEnv > 1) ? 2 : 1;

  Ipp32s env;
  Ipp32s band;
  Ipp32s *bs_df_noise   = pState->sbrEDState.bs_df_noise;
  Ipp16s *bs_data_noise = pState->sbrEDState.bufNoiseQuant;
//  Ipp32s ch = 0;

  Ipp16s LAV;
  Ipp16s delta;
  Ipp32s pos = 0;

  Ipp32s bs_cw_bits;
  //Ipp32s  bs_codeword;
  Ipp32s bs_start_noise_bit = 5;
  Ipp32s my_bs_amp_res = 0;
  Ipp32s my_bs_start_noise_bit;

  Ipp8u  *pDst;
  Ipp32s bitoffset;

  sbrencSetEnvHuffTabs(my_bs_amp_res,
                       &LAV,
                       &my_bs_start_noise_bit,

                       &pTimeHuffTab,
                       &pFreqHuffTab,

                       sbrHuffTabs,

                       HUFF_NOISE_COMPRESS); /* [1] - envelope, [0] - noise */

  for(env = 0; env < nNoiseEnv; env++){
    if ( 0 == bs_df_noise[env] ){

      pCurrHuffTab     = pFreqHuffTab;

      PUT_BITS(pBS, bs_data_noise[pos], bs_start_noise_bit);
      pos++;
      payloadBits += bs_start_noise_bit;

    } else {

      pCurrHuffTab  = pTimeHuffTab;
    }

    /* LOG ------------------------------------------- */
    pDst = (Ipp8u*)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
    bitoffset = (32 - pBS->nBit_offset) & 0x7;
    SAVE_BITSTREAM(pBS)
    /* LOG ------------------------------------------- */
    for(band = 1 - bs_df_noise[env]; band < nNoiseBand; band++ ){
      delta = bs_data_noise[pos];
      pos++;

      //bs_codeword = pCurrHuffTabCode[ delta + LAV ];
      bs_cw_bits = ownVLCCountBits_16s32s(delta + LAV, pCurrHuffTab);

      //PUT_BITS(pBS, bs_codeword, bs_cw_bits);
      delta = delta + LAV;
      ippsVLCEncodeBlock_16s1u(&delta, 1, &pDst, &bitoffset, pCurrHuffTab);

      payloadBits += bs_cw_bits;
    } /* for(band=0; */
    /* LOG ------------------------------------------- */
    pBS->pCurrent_dword = (Ipp32u*)((size_t)(pDst) - ((size_t)(pDst) & 3));
    pBS->nBit_offset = (Ipp32s)(32 - ((pDst - (Ipp8u*)pBS->pCurrent_dword) << 3)- bitoffset);
    LOAD_DWORD(pBS)
    /* LOG ------------------------------------------- */
  }     /* for(env=0; */

  return payloadBits;
}

/********************************************************************/

static Ipp32s
enc_sbr_sinusoidal_coding(sSBREnc_SCE_State* pState, sBitsreamBuffer* pBS, Ipp32s nBand)
{
  Ipp32s payloadBits = 0;
  Ipp32s band;
  Ipp32s *bs_add_harmonic = pState->sbrEDState.bs_add_harmonic;

  for(band = 0; band < nBand; band++){
    PUT_BITS(pBS, bs_add_harmonic[band], 1);
    payloadBits += 1;
  }

  return payloadBits;
}

/********************************************************************/

static Ipp32s
enc_sbr_single_channel_element(sSBREncState* pState, sBitsreamBuffer* pBS)
{
  Ipp32s indx = pState->indx_FreqOffset_Tab[ pState->indx_CE_Tab[ pState->sbr_offset ] ];

  sSBREnc_SCE_State* pSCE_Element = ( pState->pSCE_Element + pState->sbr_offset );
  sSBRFeqTabsState*  pFreqtabsState = ( pState->sbrFreqTabsState + indx);
//  sSBRHeader*        pHeader = ( pState->sbrHeader + indx);

  Ipp32s payloadBits      = 0;
  Ipp32s bs_data_extra    = 0; /* no extra data */
  Ipp32s bs_extended_data = 0; /* no extended data */
  Ipp32s add_harm_flag    = pSCE_Element->sbrEDState.bs_add_harmonic_flag;
  Ipp32s bs_amp_res       = 0;

  /* no bs_reserved, bs_data_extra = 0 */
  PUT_BITS(pBS, bs_data_extra,  1);
  payloadBits++;

  payloadBits += enc_sbr_grid(pSCE_Element, pBS);

  payloadBits += enc_sbr_dtdf(pSCE_Element, pBS);

  payloadBits += enc_sbr_invf(pSCE_Element, pBS, pFreqtabsState->nNoiseBand);

  bs_amp_res = pSCE_Element->bs_amp_res;
  payloadBits += enc_sbr_envelope(pSCE_Element,
                                  pState->sbrHuffTabs,
                                  pBS,
                                  bs_amp_res, //pHeader->bs_amp_res,
                                  pFreqtabsState->nLoBand,
                                  pFreqtabsState->nHiBand);

  payloadBits += enc_sbr_noise(pSCE_Element,
                               pState->sbrHuffTabs,
                               pBS,
                               pFreqtabsState->nNoiseBand);

  PUT_BITS(pBS, add_harm_flag, 1);
  payloadBits++;

  if(add_harm_flag) {
    payloadBits += enc_sbr_sinusoidal_coding(pSCE_Element,
                                             pBS,
                                             pFreqtabsState->nHiBand);
  }

  PUT_BITS(pBS, bs_extended_data,  1);
  payloadBits++;

  return payloadBits;
}

/********************************************************************/
/*
static Ipp32s
sbrencUpDateAmpRes( Ipp32s nEnv, Ipp32s frameClass )
{
  Ipp32s ampRes = 1; // 3.0dB

  if( (FIXFIX == frameClass) && (1 == nEnv) ){
    ampRes = 0;// 1.5dB
  }

  return ampRes;
}
*/
/********************************************************************/

static Ipp32s
enc_sbr_channel_pair_element(sSBREncState* pState, sBitsreamBuffer* pBS)
{
  Ipp32s payloadBits      = 0;
  Ipp32s bs_data_extra    = 0; /* no extra data */
  Ipp32s bs_extended_data = 0; /* no extended data */
  Ipp32s bs_coupling      = 0; /* no coupling channel */
  Ipp32s bs_amp_res       = 0;

  /* current info */
  Ipp32s indx = pState->indx_FreqOffset_Tab[ pState->indx_CE_Tab[ pState->sbr_offset ] ];

  sSBREnc_SCE_State* pSCE_Element = ( pState->pSCE_Element + pState->sbr_offset );
  sSBRFeqTabsState*  pFreqtabsState = ( pState->sbrFreqTabsState + indx);
//  sSBRHeader*        pHeader = ( pState->sbrHeader + indx);

  Ipp32s add_harm_flag_0  = (pSCE_Element + 0)->sbrEDState.bs_add_harmonic_flag;
  Ipp32s add_harm_flag_1  = (pSCE_Element + 1)->sbrEDState.bs_add_harmonic_flag;

  /* no bs_reserved, bs_data_extra = 0 */
  PUT_BITS(pBS, bs_data_extra,  1);
  payloadBits++;

  /* no bs_coupling, bs_coupling = 0 */
  PUT_BITS(pBS, bs_coupling, 1);
  payloadBits++;

  /* grid */
  payloadBits += enc_sbr_grid(pSCE_Element + 0, pBS); //ch = 0

  payloadBits += enc_sbr_grid(pSCE_Element + 1, pBS); //ch = 1

  /* dtdf */
  payloadBits += enc_sbr_dtdf(pSCE_Element + 0, pBS); //ch = 0

  payloadBits += enc_sbr_dtdf(pSCE_Element + 1, pBS); //ch = 1

  /* invf */
  payloadBits += enc_sbr_invf(pSCE_Element + 0, pBS, pFreqtabsState->nNoiseBand); //ch = 0

  payloadBits += enc_sbr_invf(pSCE_Element + 1, pBS, pFreqtabsState->nNoiseBand); //ch = 1

  /* envelope */
  /* patch, only for CPE */
  bs_amp_res = (pSCE_Element+0)->bs_amp_res; //sbrencUpDateAmpRes( (pSCE_Element + 0)->sbrFIState.nEnv, (pSCE_Element + 0)->sbrGrid.frameClass );

  payloadBits += enc_sbr_envelope(pSCE_Element + 0,
                                  pState->sbrHuffTabs,
                                  pBS,
                                  bs_amp_res, //pHeader->bs_amp_res,
                                  pFreqtabsState->nLoBand,
                                  pFreqtabsState->nHiBand);//ch = 0

  /* patch, only for CPE */
  bs_amp_res = (pSCE_Element+1)->bs_amp_res; //sbrencUpDateAmpRes( (pSCE_Element + 1)->sbrFIState.nEnv, (pSCE_Element + 1)->sbrGrid.frameClass );
  payloadBits += enc_sbr_envelope(pSCE_Element + 1,
                                  pState->sbrHuffTabs,
                                  pBS,
                                  bs_amp_res, //pHeader->bs_amp_res,
                                  pFreqtabsState->nLoBand,
                                  pFreqtabsState->nHiBand);//ch = 1

  /* noise */
  payloadBits += enc_sbr_noise(pSCE_Element + 0,
                               pState->sbrHuffTabs,
                               pBS,
                               pFreqtabsState->nNoiseBand); //ch = 0

  payloadBits += enc_sbr_noise(pSCE_Element + 1,
                              pState->sbrHuffTabs,
                              pBS,
                              pFreqtabsState->nNoiseBand); // ch = 1

  /* harmonic */
  PUT_BITS(pBS, add_harm_flag_0,  1);
  payloadBits++;

  if(add_harm_flag_0) {
    payloadBits += enc_sbr_sinusoidal_coding(pSCE_Element + 0,
                                             pBS,
                                             pFreqtabsState->nHiBand); /* ch = 0 */
  }

  PUT_BITS(pBS, add_harm_flag_1,  1);
  payloadBits++;

  if(add_harm_flag_1) {
    payloadBits += enc_sbr_sinusoidal_coding(pSCE_Element + 1,
                                             pBS,
                                             pFreqtabsState->nHiBand); /* ch = 1 */
  }

  PUT_BITS(pBS, bs_extended_data,  1);
  payloadBits++;

  return payloadBits;
}

/********************************************************************/

static Ipp32s enc_sbr_data(sSBREncState* pState, sBitsreamBuffer* pBS, enum eId element_ID)
{
  Ipp32s payloadBits = 0;

  switch(element_ID) {
    case ID_SCE:
      payloadBits += enc_sbr_single_channel_element(pState, pBS);
      break;

    case ID_CPE:
      payloadBits += enc_sbr_channel_pair_element(pState, pBS);
      break;

    default:
      break;
  }

  return payloadBits;
}

/********************************************************************/

static Ipp32s enc_sbr_extension_data(sSBREncState* pState, sBitsreamBuffer* pBS, enum eId element_ID)
{
  Ipp32s payloadBits = 0;
  //Ipp32s bs_header_flag = pState->sbrHeader->flag_SBRHeaderActive;
  Ipp32s indx = pState->indx_FreqOffset_Tab[ pState->indx_CE_Tab[ pState->sbr_offset ] ];
  sSBRHeader* pHeader = (pState->sbrHeader + indx);
  Ipp32s bs_header_flag = pHeader->flag_SBRHeaderActive;

  PUT_BITS(pBS, bs_header_flag, 1);
  payloadBits++;

  if(bs_header_flag){
    payloadBits += enc_sbr_header(pHeader, pBS);
  }

  payloadBits += enc_sbr_data(pState, pBS, element_ID);

  return payloadBits;
}

/********************************************************************/

static Ipp32s enc_extension_payload(sSBREncState* pState, sBitsreamBuffer* pBS, Ipp32s extension_type)
{
  Ipp32s payloadBits = 0;
  enum eId element_ID = (enum eId)pState->indx_CE_Tab[ pState->sbr_offset ]; //ID_SCE;
  Ipp32s bs_fill_bits = 0;

  /* extension_type */
  PUT_BITS(pBS, extension_type, 4);
  payloadBits += 4;

  payloadBits += enc_sbr_extension_data(pState, pBS, element_ID);

  /* align fill data */
  bs_fill_bits = 8 - (payloadBits % 8);
  bs_fill_bits %= 8;

  PUT_BITS(pBS, 0, bs_fill_bits);
  payloadBits += bs_fill_bits;

  /* check */
  if (payloadBits / 8 > 270) {
    payloadBits = 0;
  }

  return payloadBits;
}

/********************************************************************/

Ipp32s enc_fill_element( sSBREncState* pState, sBitsreamBuffer* pBS)
{
  Ipp32s payloadBits = 0;
  Ipp32s extension_type = EXT_SBR_DATA;

  /* miss COUNT and ESC_COUNT fields */
  //PUT_BITS(pBS, count, 4);
  //PUT_BITS(pBS, esc_count, 8);

  /*  */
  payloadBits = enc_extension_payload(pState, pBS, extension_type);

  return payloadBits;
}

/********************************************************************/

Ipp32s enc_fill_element_write( sBitsreamBuffer *pBS, Ipp8u* bufAuxBS, Ipp32s payloadBits )
{
  Ipp32s count = 0;
  Ipp32s esc_count = 1;
  Ipp32s i;

  count = IPP_MIN((payloadBits)/8, 15);

  PUT_BITS(pBS, ID_FIL, 3);
  PUT_BITS(pBS, count,  4);

#ifdef SBR_NEED_LOG
  fprintf(logFile, "\ncnt_byte = %i\n", count);
#endif

  if(15 == count){
    esc_count = IPP_MIN( (payloadBits/8)- 15, 255);
    esc_count++;
    PUT_BITS(pBS, esc_count,  8);
  }

  /* AYA real payload_bit_formatter */
  count += esc_count - 1;

  for(i = 0; i < count; i++ ){
    PUT_BITS(pBS, bufAuxBS[i],  8);
  }

  //------------------
  return 0; //OK
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

