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
#include "sbr_dec_struct.h"
#include "sbr_dec_parser.h"

#include "ipps.h"

/********************************************************************
 * this code may be integrated to Huffman decode
 ********************************************************************/

static Ipp32s LookUpI_parity(Ipp32s *fHiBandTab, Ipp32s *fLoBandTab, Ipp32s k, Ipp32s nHiBand)
{
  Ipp32s i = 0;

  while ((fHiBandTab[i] != fLoBandTab[k]) && (i <= nHiBand))
    i++;

  return i;
}

/********************************************************************/

static Ipp32s LookUpI_disparity(Ipp32s *fHiBandTab, Ipp32s *fLowBandTab, Ipp32s k,
                                Ipp32s nLoBand)
{
  Ipp32s i, ret_val = 0;

  for (i = 0; i <= nLoBand - 1; i++) {
    if ( (fLowBandTab[i] <= fHiBandTab[k]) &&
        (fLowBandTab[i + 1] > fHiBandTab[k]) )
    {
      ret_val = i;
      break;
    }
  }

  return ret_val;
}

/********************************************************************/

Ipp32s sbrEnvNoiseDec(sSBRDecComState * pSbr, Ipp32s ch)    // optimization is needed!!!
{
  Ipp16s delta, g_E;
  Ipp32s i, l, k, n_r;
  Ipp32s g;
  Ipp32s flag_switch;
  Ipp32s resolution[2];

  sSBRFrameInfoState* pFIState = &( pSbr->sbrFIState[ch] );
  sSBREnvDataState*   pEDState = &(pSbr->sbrEDState[ch]);

  Ipp32s  nNoiseEnv    = pFIState->nNoiseEnv;
  Ipp32s  nLoBand      = pSbr->sbrFreqTabsState.nLoBand;
  Ipp32s  nHiBand      = pSbr->sbrFreqTabsState.nHiBand;
  Ipp32s  nNoiseBand   = pSbr->sbrFreqTabsState.nNoiseBand;

  Ipp32s* pos          = pEDState->vSizeEnv;
  Ipp32s* posN         = pEDState->vSizeNoise;

  Ipp16s* vEnv         = pEDState->bufEnvQuant;
  Ipp16s* vNoise       = pEDState->bufNoiseQuant;

  Ipp32s* freqRes      = pFIState->freqRes;
  Ipp32s* pTable1      = pSbr->sbrFreqTabsState.fHiBandTab;
  Ipp32s* pTable0      = pSbr->sbrFreqTabsState.fLoBandTab;

  /* check */
  if( pFIState->nEnv > MAX_NUM_ENV)
    return SBR_ERR_REQUIREMENTS;

  resolution[0] = nLoBand;
  resolution[1] = nHiBand;

  delta = ((ch == 1) && (pSbr->bs_coupling == 1)) ? 2 : 1;

/*
 * calculate for l == 0
 */
  n_r = resolution[freqRes[0]];
  g   = pFIState->freqResPrev[pFIState->nEnvPrev - 1];
  flag_switch = pEDState->bs_df_env[0] * (freqRes[0] - g + 2);

  switch (flag_switch) {

  case 0: // bs_df_env[0] = 0
    vEnv[0] = delta*vEnv[0];

    for (k = 1; k < n_r; k++) {
     vEnv[k] =vEnv[k-1] +vEnv[k]*delta;
    }
    break;

  case 2: // bs_df_env[0] = 1 and freqRes(l)=g(l)
    for (k = 0; k < n_r; k++) {
      g_E    = pEDState->bufEnvQuantPrev[k];
     vEnv[k] = g_E + delta * (vEnv[k]);
    }
    break;

  case 1: // bs_df_env[0] = 1 and freqRes(l)=0 and g(l)=1
    for (k = 0; k < n_r; k++) {
      i      = LookUpI_parity(pTable1, pTable0, k, nHiBand);
      g_E    = pEDState->bufEnvQuantPrev[i];
     vEnv[k] = g_E + delta * (vEnv[k]);
    }
    break;

  case 3: // bs_df_env[0] = 1 and freqRes(l)=1 and g(l)=0
    for (k = 0; k < n_r; k++) {
      i      = LookUpI_disparity(pTable1, pTable0, k, nLoBand);
      g_E    = pEDState->bufEnvQuantPrev[i];
     vEnv[k] = g_E + delta * (vEnv[k]);
    }
  }
/*
 * END!!! l==0 END!!!
 */

/*
 * calcilate for l=1:nEnv
 */
  for (l = 1; l < pFIState->nEnv; l++) {
    n_r = resolution[freqRes[l]];
    g   = freqRes[l - 1];
    flag_switch = pEDState->bs_df_env[l] * (freqRes[l] - g + 2);


    switch (flag_switch) {

    case 0: // bs_df_env[l] = 0

     vEnv[pos[l]] = delta*vEnv[pos[l]];
      for (k = 1; k < n_r; k++) {
       vEnv[pos[l]+k] =vEnv[pos[l]+k-1] +vEnv[pos[l]+k] * delta;
      }
      break;

    case 2: // bs_df_env[l] = 1 and r(l)=g(l)
      for (k = 0; k < n_r; k++) {
        g_E =vEnv[pos[l-1]+k];
       vEnv[pos[l]+k] = g_E + delta * (vEnv[pos[l]+k]);
      }
      break;

    case 1: // bs_df_env[l] = 1 and r(l)=0 and g(l)=1
      for (k = 0; k < n_r; k++) {
        i   = LookUpI_parity(pTable1, pTable0, k, nHiBand);
        g_E = vEnv[pos[l-1]+i];
       vEnv[pos[l]+k] = g_E + delta * (vEnv[pos[l]+k]);
      }
      break;

    case 3: // bs_df_env[l] = 1 and r(l)=1 and g(l)=0
      for (k = 0; k < n_r; k++) {
        i   = LookUpI_disparity(pTable1, pTable0, k, nLoBand);
        g_E = vEnv[pos[l-1]+i];
       vEnv[pos[l]+k] = g_E + delta * (vEnv[pos[l]+k]);
      }
    }   // end switch
  }     // end for
/* step(2): noise_dec */

  if (pEDState->bs_df_noise[0] == 1)    // and l==0
  {
    for (k = 0; k < nNoiseBand; k++) {
      vNoise[posN[0]+k] = pEDState->bufNoiseQuantPrev[k] + delta * (vNoise[pos[0]+k]);
    }
  } else { // if(pSbr->SbrBSE.bs_df_noise[ch][0] == 0) and l==0

    vNoise[posN[0]+0] = delta*vNoise[posN[0]+0];
    for (k = 1; k < nNoiseBand; k++) {
      vNoise[posN[0]+k] = vNoise[posN[0]+k-1] + delta * vNoise[posN[0]+k];
    }
  }

// noise
  for (l = 1; l < nNoiseEnv; l++) {
    if (pEDState->bs_df_noise[l] == 0) {

      vNoise[posN[l]] = delta * vNoise[posN[l]];
      for (k = 1; k < nNoiseBand; k++) {
        vNoise[posN[l]+k] = delta * vNoise[posN[l]+k] + vNoise[posN[l]+k-1];
      }
    } else {
      for (k = 0; k < nNoiseBand; k++) {
        vNoise[posN[l]+k] = vNoise[posN[l-1]+k] + delta * (vNoise[posN[l]+k]);
      }
    }
  }

/* --------------------------------  update ---------------------------- */
  {
    Ipp32s size = sizeof(Ipp16s);

    ippsZero_8u((Ipp8u *)pEDState->bufEnvQuantPrev,   size * MAX_NUM_ENV_VAL);
    ippsZero_8u((Ipp8u *)pEDState->bufNoiseQuantPrev, size * MAX_NUM_ENV_VAL);
    ippsZero_8u((Ipp8u *)pFIState->freqResPrev,       sizeof(Ipp32s) * MAX_NUM_ENV);

    ippsCopy_8u((const Ipp8u*)freqRes, (Ipp8u*)pFIState->freqResPrev, sizeof(Ipp32s) * pFIState->nEnv);
  }

  l   = pFIState->nEnv - 1;
  n_r = resolution[freqRes[l]];
  for (k = 0; k < n_r; k++) {
    pEDState->bufEnvQuantPrev[k] =vEnv[pos[l]+k];
  }

  l = nNoiseEnv - 1;
  for (k = 0; k < nNoiseBand; k++)
    pEDState->bufNoiseQuantPrev[k] = vNoise[posN[l]+k];

  return 0;     // OK
}

#endif //UMC_ENABLE_XXX

