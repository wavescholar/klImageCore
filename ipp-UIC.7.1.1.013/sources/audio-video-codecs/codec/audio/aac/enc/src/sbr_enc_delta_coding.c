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

#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_freq_tabs.h"
#include "sbr_huff_tabs.h"
#include "sbr_enc_settings.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/***************************************************************************/

#define OWN_DF_EDGE_1ST_ENV  0.3f

#define OWN_DF_EDGE_GAIN     0.3f

/***************************************************************************/

static void
sbrencMapLoResSf(Ipp16s envQuantVal, Ipp16s* bufEnvQuantPrev, Ipp32s offset, Ipp32s index, Ipp32s res)
{

  if ( HI == res ) {
    bufEnvQuantPrev[index] = envQuantVal;
    return;
  }

  if (offset >= 0) {
    if(index < offset) {

      bufEnvQuantPrev[index] = envQuantVal;
    } else {

      bufEnvQuantPrev[2*index - offset] = envQuantVal;
      bufEnvQuantPrev[2*index+1 - offset] = envQuantVal;
    }
  } else {
    offset = -offset;

    if (index < offset) {

      bufEnvQuantPrev[3*index] = envQuantVal;
      bufEnvQuantPrev[3*index+1] = envQuantVal;
      bufEnvQuantPrev[3*index+2] = envQuantVal;
    } else {

      bufEnvQuantPrev[2*index + offset] = envQuantVal;
      bufEnvQuantPrev[2*index + 1 + offset] = envQuantVal;
    }
  }

  return;
}

/*******************************************************************************/

static Ipp32s
indexLo2Hi(Ipp32s offset, Ipp32s index, Ipp32s res)
{
  if ( HI == res ) return index;

  if (offset >= 0) {

    if (index < offset) {
      return(index);
    } else {
      return(2*index - offset);
    }
  } else {
    offset = -offset;

    if (index < offset) {
      return(2*index+index);
    } else {
      return(2*index + offset);
    }
  }

}

/*******************************************************************************/

Ipp32s
sbrencDeltaCoding (Ipp16s*  bufEnvQuant,
                   Ipp16s*  bufEnvQuantPrev,

                   Ipp32s*  freq_res,
                   Ipp32s   nSfBands[2],
                   Ipp32s   bs_amp_res,
                   Ipp32s*  bs_df_env,
                   //Ipp32s   coupling,
                   Ipp32s   offset,
                   Ipp32s   nEnv,
                   //Ipp32s   channel,

                   Ipp32s   headerActive,
                   Ipp32s*  flagUpDate,
                   Ipp32s   cur_dF_edge_gain_fac,
                   Ipp32s   typeCompress,

                   IppsVLCEncodeSpec_32s*  sbrHuffTabs[NUM_SBR_HUFF_TABS]
                   ) /* [0] - noise
                        [1] - envelope*/
{
  IppsVLCEncodeSpec_32s* pTimeHuffTab = NULL;
  IppsVLCEncodeSpec_32s* pFreqHuffTab = NULL;

#if !defined(ANDROID)
  Ipp16s deltaFreq[MAX_NUM_FREQ_COEFFS];
  Ipp16s deltaTime[MAX_NUM_FREQ_COEFFS];
#else
  static Ipp16s deltaFreq[MAX_NUM_FREQ_COEFFS];
  static Ipp16s deltaTime[MAX_NUM_FREQ_COEFFS];
#endif

  Ipp16s lastSf, currSf;
  Ipp16s *ptrSf = 0;
  Ipp16s *ptrBestDelta = 0;
  Ipp16s compressFactor;
  Ipp16s cbLAV;

  Ipp32s num_bits_freq = 0, num_bits_time = 0;
  Ipp32s env, nBands, band;
  Ipp32s start_bits = 0;
  Ipp32s upDate = *flagUpDate;

  Ipp32s flag_d_time, i;

  Ipp32f dF_edge_1stEnv = OWN_DF_EDGE_1ST_ENV + OWN_DF_EDGE_GAIN * cur_dF_edge_gain_fac;

  /* CODE */

  if( typeCompress == 0){
    dF_edge_1stEnv = 0.0f;
  }

  sbrencSetEnvHuffTabs(bs_amp_res,
                       &cbLAV,
                       &start_bits,

                       &pTimeHuffTab,
                       &pFreqHuffTab,

                       sbrHuffTabs,

                       typeCompress);

  compressFactor = 0;

  if (headerActive) upDate = 0;

  for (env = 0; env < nEnv; env++){

    if (freq_res[env] == HI)
      nBands = nSfBands[HI];
    else
      nBands = nSfBands[LO];

    ptrSf = bufEnvQuant;
    currSf = *ptrSf;

    deltaFreq[0] = currSf >> compressFactor;

    num_bits_freq = start_bits;

    if(upDate != 0) {
      deltaTime[0] = (currSf - bufEnvQuantPrev[0]) >> compressFactor;

      num_bits_time = ownVLCCountBits_16s32s (deltaTime[0]+ cbLAV, pTimeHuffTab);
    }

    sbrencMapLoResSf(currSf, bufEnvQuantPrev, offset, 0, freq_res[env]);

    for (band = 1; band < nBands; band++) {

      lastSf = (*ptrSf);

      ptrSf++;

      currSf = (*ptrSf);

      deltaFreq[band] = (currSf - lastSf) >> compressFactor;

      num_bits_freq += ownVLCCountBits_16s32s (deltaFreq[band]+ cbLAV, pFreqHuffTab);

      if(upDate != 0) {

        deltaTime[band] = currSf - bufEnvQuantPrev[indexLo2Hi(offset, band, freq_res[env])];

        deltaTime[band] = deltaTime[band] >> compressFactor;
      }

      sbrencMapLoResSf(currSf, bufEnvQuantPrev, offset, band, freq_res[env]);

      if(upDate != 0) {

        num_bits_time += ownVLCCountBits_16s32s (deltaTime[band]+ cbLAV, pTimeHuffTab);
      }
    } /* end of for(band = ...) */

    /* STEP 2: DECISION */
    if (env == 0) {
      flag_d_time = (upDate != 0 && (num_bits_freq > num_bits_time * (1 + dF_edge_1stEnv)));
    } else {
      flag_d_time = (num_bits_freq > num_bits_time);
    }

    if (flag_d_time) {

      bs_df_env[env] = TIME_DOMAIN;
      ptrBestDelta = deltaTime;
    } else {

      bs_df_env[env] = FREQ_DOMAIN;
      ptrBestDelta = deltaFreq;
    }

    //ippsCopy_16s(ptrBestDelta, bufEnvQuant, nBands);
    for (i=0; i<nBands; i++)
       bufEnvQuant[i]= ptrBestDelta[i];

    bufEnvQuant += nBands; /* up-date bufEnvQuant */

    upDate = 1;
  }

  *flagUpDate = upDate;

  return 0;//OK

}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

