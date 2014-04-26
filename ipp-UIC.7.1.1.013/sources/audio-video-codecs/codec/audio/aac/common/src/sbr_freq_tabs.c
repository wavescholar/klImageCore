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
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER) || defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include "align.h"
#include "aac_status.h"
#include "sbr_settings.h"
#include "sbr_struct.h"
#include "sbr_freq_tabs.h"

#include "ipps.h"

/********************************************************************/

// Q14
static const Ipp32s iTABLE_LIMITER_BANDS_PER_OCTAVE[] = { 19661, 32768, 49152 };

static Ipp32s offset_16kHz[16]    = { -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7 };
static Ipp32s offset_22kHz[16]    = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13 };
static Ipp32s offset_24kHz[16]    = { -5, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 16 };
static Ipp32s offset_32kHz[16]    = { -6, -4, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 16 };
static Ipp32s offset_44_64kHz[16] = { -4, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 16, 20 };
static Ipp32s offset_up64kHz[16]  = { -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 16, 20, 24 };
static Ipp32s* SBR_TABLE_OFFSET[]    = {offset_up64kHz, offset_up64kHz, offset_44_64kHz,
    offset_44_64kHz, offset_44_64kHz, offset_32kHz, offset_24kHz, offset_22kHz, offset_16kHz};

/********************************************************************/

static Ipp32s stopDk_96kHz[14] = { 0, 2, 4, 6, 8, 11, 14, 18, 22, 26, 31, 37, 44, 51 };
static Ipp32s stopDk_88kHz[14] = { 0, 2, 4, 6, 8, 11, 14, 18, 22, 26, 31, 36, 42, 49 };
static Ipp32s stopDk_64kHz[14] = { 0, 2, 4, 6, 8, 11, 14, 17, 21, 25, 29, 34, 39, 44 };
static Ipp32s stopDk_48kHz[14] = { 0, 2, 4, 6, 8, 11, 14, 17, 20, 24, 28, 33, 38, 43 };
static Ipp32s stopDk_44kHz[14] = { 0, 2, 4, 6, 8, 11, 14, 17, 20, 24, 28, 32, 36, 41 };
static Ipp32s stopDk_32kHz[14] = { 0, 2, 4, 6, 8, 10, 12, 14, 17, 20, 23, 26, 29, 32 };
static Ipp32s stopDk_24kHz[14] = { 0, 2, 4, 6, 8, 10, 12, 14, 17, 20, 23, 26, 29, 32 };
static Ipp32s stopDk_22kHz[14] = { 0, 1, 3, 5, 7, 9, 11, 13, 15, 17, 20, 23, 26, 29 };
static Ipp32s stopDk_16kHz[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16 };
static Ipp32s stopDk_12kHz[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static Ipp32s stopDk_11kHz[14] = { 0, -1, -2, -3, -4, -5, -6, -6, -6, -6, -6, -6, -6, -6 };
static Ipp32s stopDk_08kHz[14] = { 0, -3, -6, -9, -12, -15, -18, -20, -22, -24, -26, -28, -30, -32 };
static Ipp32s* SBR_TABLE_STOP_DK[] = {stopDk_96kHz, stopDk_88kHz, stopDk_64kHz, stopDk_48kHz, stopDk_44kHz,
                                      stopDk_32kHz, stopDk_24kHz, stopDk_22kHz, stopDk_16kHz, stopDk_12kHz,
                                      stopDk_11kHz, stopDk_08kHz};

/********************************************************************/

/* FIXED POINT: Q28 format */
/* input arg from 0 to 72 */
static Ipp32s SBR_TABLE_LOG2[] = {

0,        0,       268435456, 425460128, 536870912, 623287808, 693895616, 753593600, 805306368,
850920256,891723264,928634112,962331072, 993329216, 1022029056,1048747968,1073741824,1097219968,
1119355776,1140294400,1160158720,1179053696,1197069568,1214284416,1230766464,1246575616,1261764736,
1276380416,1290464512,1304054400,1317183360,1329881984,1342177280,1354094208,1365655424,1376881408,
1387791232,1398402048,1408729856,1418789376,1428594176,1438156928,1447489152,1456601856,1465505024,
1474208128,1482719872,1491048576,1499201920,1507187200,1515011072,1522680064,1530200192,1537576960,
1544815872,1551921920,1558899968,1565754496,1572489856,1579110016,1585618816,1592020224,1598317440,
1604513920,1610612736,1616617088,1622529664,1628353408,1634090880,1639744512,1645316864,1650810112,
1656226688
};

/********************************************************************/

/* NINT[ mult * log(num / denum) / log(2) ] */
static Ipp32s GetRatioLog( Ipp32s mult, Ipp32s num, Ipp32s denum )
{
  Ipp64s ret_val;

  ret_val = mult * (Ipp64s)(SBR_TABLE_LOG2[num] - SBR_TABLE_LOG2[denum]) + (Ipp64s)134217728;
  ret_val >>= 28;

  return (Ipp32s)ret_val;

}

/********************************************************************/

static Ipp32s sbrCalcMasterFreq1(Ipp32s k0, Ipp32s k2, Ipp32s bs_alter_scale, Ipp32s *fMasterBandTab,
                                 Ipp32s *nMasterBand)
{
  IPP_ALIGNED_ARRAY(32, Ipp32s, vDk, 100);
  Ipp32s dk;
  Ipp32s numBands;
  Ipp32s k2Achieved;
  Ipp32s k2Diff;
  Ipp32s k;
  Ipp32s incr = 0;

  for (k = 0; k < 64; k++) {
    fMasterBandTab[k] = 0;
  }

  if (bs_alter_scale == 0) {
    dk = 1;
    numBands = (k2 - k0) >> 1;
  } else {
    dk = 2;
    numBands = (k2 - k0 + 2) >> 2;
  }

  numBands <<= 1;

  if(numBands <= 0)
    return SBR_ERR_REQUIREMENTS;

  k2Achieved = k0 + numBands * dk;
  k2Diff = k2 - k2Achieved;
  for (k = 0; k < numBands; k++)
    vDk[k] = dk;

  if (k2Diff < 0) {
    incr = 1;
    k = 0;
  }
  if (k2Diff > 0) {
    incr = -1;
    k = numBands - 1;
  }

  while (k2Diff != 0) {
    vDk[k] -= incr;
    k += incr;
    k2Diff += incr;
  }

  fMasterBandTab[0] = k0;
  for (k = 1; k <= numBands; k++)
    fMasterBandTab[k] = fMasterBandTab[k - 1] + vDk[k - 1];

  *nMasterBand = numBands;
  if (numBands < 0)
    return SBR_ERR_REQUIREMENTS;

  return 0;     // OK
}

/********************************************************************/

static Ipp32s sbrCalcMasterFreq2(Ipp32s k0, Ipp32s k2, Ipp32s bs_freq_scale,
                                 Ipp32s bs_alter_scale, Ipp32s *fMasterBandTab, Ipp32s *nMasterBand)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32s, vDk0, 100);
  IPP_ALIGNED_ARRAY(32, Ipp32s, vDk1, 100);
  IPP_ALIGNED_ARRAY(32, Ipp32s, vk0, 100);
  IPP_ALIGNED_ARRAY(32, Ipp32s, vk1, 100);
  IPP_ALIGNED_ARRAY(32, Ipp32s, pow_vec, 100);
  Ipp32s temp1[3] = { 6, 5, 4 };
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32s, vDk0, 100);
  static IPP_ALIGNED_ARRAY(32, Ipp32s, vDk1, 100);
  static IPP_ALIGNED_ARRAY(32, Ipp32s, vk0, 100);
  static IPP_ALIGNED_ARRAY(32, Ipp32s, vk1, 100);
  static IPP_ALIGNED_ARRAY(32, Ipp32s, pow_vec, 100);
  static Ipp32s temp1[3] = { 6, 5, 4 };
#endif
  Ipp32s bands = temp1[bs_freq_scale - 1];
  Ipp32s twoRegions;
  Ipp32s k1, k;
  Ipp32s numBands0, numBands1, change;
  Ipp32s min_vDk1, max_vDk0;
  Ipp32s threshold;
  /* Q28 */
  const Ipp32s iTemp2[2] = { 268435456, 348966093 };
  Ipp32s iWarp = iTemp2[bs_alter_scale];
  /* Q14 */
  const Ipp32s BandDivWarp[] = { 75618, 63015, 50412 };

  for (k = 0; k < 64; k++) {
    fMasterBandTab[k] = 0;
  }

  /* Q22 */
  threshold = 9436764 * k0;
  if ( (k2 << 22) > threshold ){
    twoRegions = 1;
    k1 = 2 * k0;
  } else {
    twoRegions = 0;
    k1 = k2;
  }

  numBands0 = GetRatioLog( bands, k1, k0 );
  numBands0 <<= 1;

  if (numBands0 < 0)
    return SBR_ERR_REQUIREMENTS;

  sbrGetPowerVector( numBands0, k1, k0, pow_vec);
  ippsSub_32s_Sfs((const Ipp32s*)pow_vec, (const Ipp32s*)(pow_vec+1), vDk0, numBands0, 0);

  ippsSortAscend_32s_I(vDk0, numBands0);
  ippsMin_32s(vDk0, numBands0, &threshold);
  if (threshold < 0)
    return SBR_ERR_REQUIREMENTS;

  vk0[0] = k0;
  for (k = 1; k <= numBands0; k++)
    vk0[k] = vk0[k - 1] + vDk0[k - 1];

  if (twoRegions == 1) {

    if ( iWarp == 268435456 ){ // warp == 1.0f ?
      numBands1 = GetRatioLog( bands, k2, k1 );
    }else{

      numBands1 = BandDivWarp[ bs_freq_scale - 1 ] * ((SBR_TABLE_LOG2[k2] - SBR_TABLE_LOG2[k1]) >> 14 ) + 134217728;
      numBands1 >>= 28;
    }
    numBands1 <<= 1;

    sbrGetPowerVector( numBands1, k2, k1, pow_vec );
    ippsSub_32s_Sfs((const Ipp32s*)pow_vec, (const Ipp32s*)(pow_vec+1), vDk1, numBands1, 0);

    ippsMin_32s(vDk1, numBands1, &min_vDk1);
    if (min_vDk1 < 0)
      return SBR_ERR_REQUIREMENTS;
    ippsMax_32s(vDk0, numBands0, &max_vDk0);

    if (min_vDk1 < max_vDk0) {
      ippsSortAscend_32s_I(vDk1, numBands1);
      change = max_vDk0 - vDk1[0];
      threshold = (vDk1[numBands1 - 1] - vDk1[0]) >>  1;
      if (change > threshold)
        change = threshold;

      vDk1[0] += change;
      vDk1[numBands1 - 1] -= change;
    }

    ippsSortAscend_32s_I(vDk1, numBands1);
    vk1[0] = k1;
    for (k = 1; k <= numBands1; k++)
      vk1[k] = vk1[k - 1] + vDk1[k - 1];

    *nMasterBand = numBands0 + numBands1;
    for (k = 0; k <= numBands0; k++)
      fMasterBandTab[k] = vk0[k];

    for (k = numBands0 + 1; k <= (*nMasterBand); k++)
      fMasterBandTab[k] = vk1[k - numBands0];
  } else {
    *nMasterBand = numBands0;
    for (k = 0; k <= numBands0; k++)
      fMasterBandTab[k] = vk0[k];
  }

  return 0;     // OK
}

/********************************************************************
 *
 * this table from coreAAC ( "aac_dec_api.c" )
 * if tables has been changed then SBR may work incorrect
 *
 *  static Ipp32s sampling_frequency_table[] = {
 *    96000, 88200, 64000, 48000, 44100, 32000, 24000,
 *    22050, 16000, 12000, 11025, 8000, 7350, 0, 0, 0
 *  };
 *
 ********************************************************************/

Ipp32s sbrCalcMasterFreqBoundary(Ipp32s bs_start_freq, Ipp32s bs_stop_freq,
                                 Ipp32s sbrFreqIndx, Ipp32s *k0, Ipp32s *k2)
{
  Ipp32s  startMin, stopMin;

#if !defined(ANDROID)
  Ipp32s  SBR_TABLE_START_MIN[] = { 7, 7, 10, 11, 12, 16, 16, 17, 24, 32, 35, 48 };
  Ipp32s  SBR_TABLE_STOP_MIN[]  = { 13, 15, 20, 21, 23,32, 32, 35, 48, 64, 70, 96 };
#else
  static Ipp32s  SBR_TABLE_START_MIN[] = { 7, 7, 10, 11, 12, 16, 16, 17, 24, 32, 35, 48 };
  static Ipp32s  SBR_TABLE_STOP_MIN[]  = { 13, 15, 20, 21, 23,32, 32, 35, 48, 64, 70, 96 };
#endif

  /* may be error form core AAC */
  if( ( sbrFreqIndx < 0 ) || (sbrFreqIndx > 11) ) {
     return SBR_ERR_REQUIREMENTS;
  }

  startMin = SBR_TABLE_START_MIN[ sbrFreqIndx ];
  stopMin  = SBR_TABLE_STOP_MIN[ sbrFreqIndx ];

  *k0 = startMin + SBR_TABLE_OFFSET[ sbrFreqIndx ][bs_start_freq];

  if ((bs_stop_freq >= 0) && (bs_stop_freq < 14)) {
    *k2 = IPP_MIN(64, stopMin + SBR_TABLE_STOP_DK[sbrFreqIndx][bs_stop_freq] );

  }else if (bs_stop_freq == 14){
    *k2 = IPP_MIN(64, 2 * (*k0));

  }else if (bs_stop_freq == 15){
    *k2 = IPP_MIN(64, 3 * (*k0));

  }

// check
#if 0
  if (((*k2) - (*k0) < 0) || (sbr_freq_sample < 32000) && ((*k2) - (*k0) > 48) ||
      (sbr_freq_sample == 44000) && ((*k2) - (*k0) > 35) || (sbr_freq_sample >= 48000) &&
      ((*k2) - (*k0) > 32))
    return SBR_ERR_REQUIREMENTS;
#endif
  if (((*k2) - (*k0) < 0) ||
      ((sbrFreqIndx  > 5) && ((*k2) - (*k0) > 48)) ||
      ((sbrFreqIndx == 4) && ((*k2) - (*k0) > 35)) ||
      ((sbrFreqIndx <= 3) && ((*k2) - (*k0) > 32))){

    return SBR_ERR_REQUIREMENTS;
  }

  return 0;     // OK
}

/********************************************************************/

Ipp32s sbrCalcMasterFreqBandTab(Ipp32s k0, Ipp32s k2, Ipp32s bs_freq_scale,
                                Ipp32s bs_alter_scale, Ipp32s *fMasterBandTab, Ipp32s *nMasterBand)
{
  Ipp32s error = 0;

  if (bs_freq_scale == 0)
    error = sbrCalcMasterFreq1(k0, k2, bs_alter_scale, fMasterBandTab, nMasterBand);
  else
    error =
      sbrCalcMasterFreq2(k0, k2, bs_freq_scale, bs_alter_scale, fMasterBandTab,
                         nMasterBand);

  return error;
}

/********************************************************************/

Ipp32s sbrCalcHiFreqTab(Ipp32s* fMasterBandTab, Ipp32s nMasterBand, Ipp32s bs_xover_band,
                        Ipp32s* fHiFreqTab, Ipp32s* nHighBand)
{
  Ipp32s k;

  *nHighBand = nMasterBand - bs_xover_band;

  if (*nHighBand < 0)
    return SBR_ERR_REQUIREMENTS;

  for (k = 0; k <= *nHighBand; k++){
    fHiFreqTab[k] = fMasterBandTab[k + bs_xover_band];
  }

  return 0;//OK
}

/********************************************************************/

Ipp32s sbrCalcLoFreqTab(Ipp32s* fHiFreqTab, Ipp32s nHiBand,
                        Ipp32s* fLoFreqTab, Ipp32s* nLoBand)
{
  Ipp32s k;
  Ipp32s operand1, operand2;
  Ipp32s indx;

  operand1 = nHiBand >> 1;
  operand2 = operand1 << 1;
  *nLoBand = operand1 + nHiBand - operand2;

  indx = nHiBand & 1;

  fLoFreqTab[0] = fHiFreqTab[0];
  for (k = 1; k <= *nLoBand; k++)
    fLoFreqTab[k] = fHiFreqTab[2 * k - indx];

  return 0;//OK
}

/********************************************************************/

Ipp32s sbrCalcNoiseTab(Ipp32s* fLoFreqTab, Ipp32s nLoBand,
                       Ipp32s bs_noise_bands, Ipp32s k2, Ipp32s kx,
                       Ipp32s* fNoiseTab, Ipp32s* nNoiseBand)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32s, i, 100);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32s, i, 100);
#endif
  Ipp32s k;

  *nNoiseBand  = GetRatioLog( bs_noise_bands, k2, kx );
  *nNoiseBand = IPP_MAX(1, *nNoiseBand );
  if (*nNoiseBand > 5)
    return SBR_ERR_REQUIREMENTS;

  i[0] = 0;
  for (k = 1; k <= *nNoiseBand; k++)
    i[k] = i[k-1] + ( nLoBand - i[k-1] ) / ( *nNoiseBand + 1 - k );

  for (k = 0; k <= *nNoiseBand; k++)
    fNoiseTab[k] = fLoFreqTab[i[k]];

  return 0;//OK
}

/********************************************************************/

#if 1

Ipp32s sbrCalcDerivedFreqTabs(sSBRFeqTabsState* pFTState,
                              Ipp32s bs_xover_band,
                              Ipp32s bs_noise_bands,
                              Ipp32s k2, Ipp32s* kx, Ipp32s* M)
{
  Ipp32s k;
  Ipp32s error = 0;

  /* reset tables */
  for (k = 0; k < 64; k++) {
    pFTState->fNoiseBandTab[k] = pFTState->fLoBandTab[k] = pFTState->fHiBandTab[k] = 0;
  }

  /* update high tabs */
  error = sbrCalcHiFreqTab(pFTState->fMasterBandTab,
                           pFTState->nMasterBand,
                           bs_xover_band,
                           pFTState->fHiBandTab,
                           &(pFTState->nHiBand) );

  if ( error ){
    return SBR_ERR_REQUIREMENTS;
  }

  /* update low tabs */
  sbrCalcLoFreqTab(pFTState->fHiBandTab,
                   pFTState->nHiBand,
                   pFTState->fLoBandTab,
                   &(pFTState->nLoBand) );

  /* intermedia result */
  *M  = pFTState->fHiBandTab[ pFTState->nHiBand ] - pFTState->fHiBandTab[0];
  *kx = pFTState->fHiBandTab[0];

  if (*kx > 32)
    return SBR_ERR_REQUIREMENTS;

  /* update noise bands */
  error = sbrCalcNoiseTab(pFTState->fLoBandTab,
                          pFTState->nLoBand,
                          bs_noise_bands,
                          k2,
                          *kx,
                          pFTState->fNoiseBandTab,
                          &(pFTState->nNoiseBand));

  if ( error ){
    return SBR_ERR_REQUIREMENTS;
  }

  return 0;     // OK
}

#endif

/********************************************************************/

Ipp32s sbrCalcLimBandTab(/*
                          * in data
                          */
                         Ipp32s bs_limiter_bands, Ipp32s *fLoBandTab,
                         Ipp32s nLoBand, Ipp32s numPatches,
                         Ipp32s *patchNumSubbands,
                         /*
                          * out data
                          */
                         Ipp32s *fLimBandTab, Ipp32s *nLimBand)
{
  Ipp32s iLimBands = 0;
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32s, patchBorders, 200);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32s, patchBorders, 200);
#endif
  Ipp32s i, k, nrLim;
  Ipp32s inOctaves;
  Ipp32s isEqual;
  Ipp32s UpBound = fLoBandTab[nLoBand];

  ippsZero_8u((Ipp8u *)fLimBandTab, MAX_SIZE_FREQ_TABS * sizeof(Ipp32s));

  if (bs_limiter_bands < 0)
    return SBR_ERR_REQUIREMENTS; // problem bs_limiter_bands is < 0;

  if (bs_limiter_bands == 0) {
    fLimBandTab[0] = fLoBandTab[0];      // = kx = fHiBandTab[0]
    fLimBandTab[1] = fLoBandTab[nLoBand];
    *nLimBand = 1;

    return 0;   // OK
  }

  iLimBands = iTABLE_LIMITER_BANDS_PER_OCTAVE[bs_limiter_bands - 1];
  patchBorders[0] = fLoBandTab[0];      // patchBorders[0] = kx;

  for (k = 1; k < numPatches; k++)
    patchBorders[k] = patchBorders[k - 1] + patchNumSubbands[k - 1];

/*
 * patch
 */
  patchBorders[numPatches] = fLoBandTab[nLoBand];

  for (k = 0; k <= nLoBand; k++) {
    fLimBandTab[k] = fLoBandTab[k];
  }

  for (k = 1; k < numPatches; k++) {
    fLimBandTab[nLoBand + k] = patchBorders[k];
  }

  nrLim = nLoBand + numPatches - 1;
  ippsSortAscend_32s_I(fLimBandTab, nrLim + 1);

  for (k = 1; k <= nrLim; k++) {

      //nOctaves = log((Ipp64f)(fLimBandTab[k]) / (fLimBandTab[k - 1])) / log2;
      inOctaves = ( SBR_TABLE_LOG2[ fLimBandTab[k] ] -  SBR_TABLE_LOG2[ fLimBandTab[k-1] ]) >> 14;
      inOctaves *= iLimBands;

     //if (nOctaves * limBands < 0.49) {
      if ( inOctaves < 131533373 ){

      if (fLimBandTab[k] == fLimBandTab[k - 1]) {
        fLimBandTab[k] = UpBound;
        ippsSortAscend_32s_I(fLimBandTab, nrLim + 1);
        nrLim--;
        k--;
        continue;
      }

      isEqual = 0;
      for (i = 0; i <= numPatches; i++) {
        if (fLimBandTab[k] == patchBorders[i]) {
          isEqual = 1;
          break;
        }
      }
      if (!isEqual) {
        fLimBandTab[k] = UpBound;
        ippsSortAscend_32s_I(fLimBandTab, nrLim + 1);
        nrLim--;
        k--;
        continue;
      }

      isEqual = 0;
      for (i = 0; i <= numPatches; i++) {
        if (fLimBandTab[k - 1] == patchBorders[i]) {
          isEqual = 1;
          break;
        }
      }
      if (!isEqual) {
        fLimBandTab[k - 1] = UpBound;
        ippsSortAscend_32s_I(fLimBandTab, nrLim + 1);
        nrLim--;
        k--;
        continue;
      }
    }
  }

  *nLimBand = nrLim;
// PATCH
  for (k = nrLim + 1; k < MAX_SIZE_FREQ_TABS; k++)
    fLimBandTab[k] = 0;

  return 0;     // OK
}

/********************************************************************/

//Ipp32s sbrCalcPatchConstructTab(sSBRDecComState* pSbr, Ipp32s sbrFreqIndx)
Ipp32s sbrCalcPatchConstructTab(Ipp32s* fMasterBandTab,
                                Ipp32s  nMasterBand,
                                Ipp32s  M,
                                Ipp32s  kx,
                                Ipp32s  k0,
                                Ipp32s  sbrFreqIndx,

                                Ipp32s* patchNumSubbandsTab,
                                Ipp32s* patchStartSubbandTab,
                                Ipp32s* numPatches)
{
  Ipp32s goalSb, sb, odd;
  Ipp32s i, k, j;
#if !defined(ANDROID)
  Ipp32s SBR_TABLE_GOAL_SB[] = { 21, 23, 32, 43, 46, 64, 85, 93, 128, 171, 186, 256 };
#else
  static Ipp32s SBR_TABLE_GOAL_SB[] = { 21, 23, 32, 43, 46, 64, 85, 93, 128, 171, 186, 256 };
#endif
  Ipp32s msb = k0;
  Ipp32s usb = kx;

  *numPatches = 0;
  goalSb = SBR_TABLE_GOAL_SB[ sbrFreqIndx ];

  if (goalSb < kx + M) {
    for (i = 0, k = 0; fMasterBandTab[i] < goalSb; i++) {
      k = i + 1;
    }
  } else
    k = nMasterBand;

/*
 * start loop
 */
  do {
    j = k + 1;

    do {
      j--;
      sb = fMasterBandTab[j];
      odd = (sb - 2 + k0) & 1;
    } while (sb > (k0 - 1 + msb - odd));

    patchNumSubbandsTab[ *numPatches ] = IPP_MAX(sb - usb, 0);
    patchStartSubbandTab[ *numPatches ] = k0 - odd - patchNumSubbandsTab[ *numPatches ];

    if (patchNumSubbandsTab[ *numPatches ] > 0) {
      msb = usb = sb;

      (*numPatches)++;
    } else
      msb = kx;

    if ((fMasterBandTab[k] - sb) < 3)
      k = nMasterBand;
  } while (sb != (kx + M));
/*
 * end loop
 */

  if ((patchNumSubbandsTab[( *numPatches ) - 1] < 3) && (( *numPatches ) > 1)){
    (*numPatches)--;
  }

  if ( (*numPatches < 0) || (*numPatches > 5) ){
    return SBR_ERR_REQUIREMENTS;
  }

  return 0;     // OK
}

/********************************************************************/

Ipp32s sbrCalcPatchConstruct_indxTab(Ipp32s* fMasterTab, Ipp32s nMasterBand, Ipp32s k0,
                                     Ipp32s M, Ipp32s sbrFreqIndx, Ipp32s* idxPatchMap)
{
  Ipp32s goalSb, sb, odd;
  Ipp32s i, k = 0, j;
#if !defined(ANDROID)
  Ipp32s SBR_TABLE_GOAL_SB[] = { 21, 23, 32, 43, 46, 64, 85, 93, 128, 171, 186, 256 };
#else
  static Ipp32s SBR_TABLE_GOAL_SB[] = { 21, 23, 32, 43, 46, 64, 85, 93, 128, 171, 186, 256 };
#endif
  //Ipp32s  M  = fMasterTab[ nMasterBand ];//pSbr->M;
  Ipp32s kx = fMasterTab[ 0 ];//pSbr->kx;
  //Ipp32s  k0 = pSbr->k0;
  Ipp32s msb = k0;
  Ipp32s usb = kx;
  Ipp32s numPatches = 0;
  Ipp32s x, p, k_0, q;

#if !defined(ANDROID)
  Ipp32s patchNumSubbands[MAX_NUM_PATCHES+1];
  Ipp32s patchStartSubband[MAX_NUM_PATCHES+1];
#else
  static Ipp32s patchNumSubbands[MAX_NUM_PATCHES+1];
  static Ipp32s patchStartSubband[MAX_NUM_PATCHES+1];
#endif

  numPatches = 0;
  goalSb = SBR_TABLE_GOAL_SB[ sbrFreqIndx ];

  if (goalSb < kx + M) {
    for (i = 0, k = 0; fMasterTab[i] < goalSb; i++) {
      k = i + 1;
    }
  } else
    k = nMasterBand;

  /*
  * start loop
  */
  do {
    j = k + 1;

    do {
      j--;
      sb = fMasterTab[j];
      odd = (sb - 2 + k0) & 1;
    } while (sb > (k0 - 1 + msb - odd));

    patchNumSubbands[numPatches]  = IPP_MAX(sb - usb, 0);
    patchStartSubband[numPatches] = k0 - odd - patchNumSubbands[numPatches];

    if (patchNumSubbands[numPatches] > 0) {
      msb = usb = sb;

      (numPatches)++;
    } else
      msb = kx;

    if ((fMasterTab[k] - sb) < 3)
      k = nMasterBand;
  } while (sb != (kx + M));
  /*
  * end loop
  */

  if ((patchNumSubbands[(numPatches) - 1] < 3) && ((numPatches) > 1))
    (numPatches)--;

  if ( (numPatches < 0) || (numPatches > 5) )
    return SBR_ERR_REQUIREMENTS;

  /* here special vectors are calculated: simple code */

  // reset

  ippsSet_32s(EMPTY_MAPPING, idxPatchMap, 64);

  /* patch from gp code */
  for(k = 0; k < kx; k++){
    idxPatchMap[k] = k;
  }

  for (i = 0; i < numPatches; i++) {
    k_0 = 0;
    for (q = 0; q < i; q++) {
      k_0 += patchNumSubbands[q];
    }

    k_0 += kx;
    for (x = 0; x < patchNumSubbands[i]; x++) {
      k = k_0 + x;
      p = patchStartSubband[i] + x;
      //----------------
      idxPatchMap[ k ] = p;
      //----------------
    }
  }

  return 0;     // OK
}

#endif //UMC_ENABLE_XXX

