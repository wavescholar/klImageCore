/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SBR_FREQ_TABS_H__
#define __SBR_FREQ_TABS_H__

#include "sbr_struct.h"

#ifdef  __cplusplus
extern  "C" {
#endif

  Ipp32s  sbrCalcMasterFreqBoundary(Ipp32s bs_start_freq,
                                    Ipp32s bs_stop_freq,
                                    Ipp32s sbrFreqIndx,
                                    Ipp32s *k0,
                                    Ipp32s *k2);

  Ipp32s  sbrCalcMasterFreqBandTab(Ipp32s k0,
                                   Ipp32s k2,
                                   Ipp32s bs_freq_scale,
                                   Ipp32s bs_alter_scale,
                                   Ipp32s *fMasterBandTab,
                                   Ipp32s *nMasterBand);

  /* common function: sbrCalcNoiseTab + sbrCalcHiFreqTab + sbrCalcLoFreqTab */
  Ipp32s sbrCalcDerivedFreqTabs(sSBRFeqTabsState* pFTState,
                                Ipp32s  bs_xover_band,
                                Ipp32s  bs_noise_bands,
                                Ipp32s  k2,
                                Ipp32s* kx,
                                Ipp32s* M);

  Ipp32s sbrGetPowerVector(Ipp32s numBands0,
                           Ipp32s k1,
                           Ipp32s k0,
                           Ipp32s* pow_vec);

  Ipp32s sbrCalcNoiseTab(Ipp32s* fLoFreqTab,
                         Ipp32s  nLoBand,
                         Ipp32s  bs_noise_bands,
                         Ipp32s  k2,
                         Ipp32s  kx,
                         Ipp32s* fNoiseTab,
                         Ipp32s* nNoiseBand);

  Ipp32s sbrCalcLoFreqTab(Ipp32s* fHiFreqTab,
                          Ipp32s  nHiBand,
                          Ipp32s* fLoFreqTab,
                          Ipp32s* nLoBand);

  Ipp32s sbrCalcHiFreqTab(Ipp32s* fMasterBandTab,
                          Ipp32s  nMasterBand,
                          Ipp32s  bs_xover_band,
                          Ipp32s* fHiFreqTab,
                          Ipp32s* nHighBand);


  Ipp32s  sbrCalcLimBandTab(Ipp32s bs_limiter_bands,
                            Ipp32s *fLoBandTab,
                            Ipp32s nLoBand,
                            Ipp32s numPatches,
                            Ipp32s *patchNumSubbands,

                            Ipp32s *fLimBandTab,
                            Ipp32s *nLimBand);

  Ipp32s sbrCalcPatchConstructTab(Ipp32s* fMasterBandTab,
                                  Ipp32s  nMasterBand,
                                  Ipp32s  M,
                                  Ipp32s  kx,
                                  Ipp32s  k0,
                                  Ipp32s  sbrFreqIndx,

                                  Ipp32s* patchNumSubbandsTab,
                                  Ipp32s* patchStartSubbandTab,
                                  Ipp32s* numPatches);

  Ipp32s sbrCalcPatchConstruct_indxTab(Ipp32s* fMasterTab,
                                       Ipp32s  nMasterBand,
                                       Ipp32s  k0,
                                       Ipp32s  M,
                                       Ipp32s  sbrFreqIndx,
                                       Ipp32s* idxPatchMap);


#ifdef  __cplusplus
}
#endif

#endif/*__SBR_FREQ_TABS_H__ */
