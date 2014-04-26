/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SAACDEC_STREAM_ELEMENTS_H
#define __SAACDEC_STREAM_ELEMENTS_H

#include "bstream.h"
#include "aaccmn_const.h"
#include "sbr_dec_struct.h"
#include "aac_dec_own.h"
#include "aac_dec_own_fp.h"

#ifdef  __cplusplus
extern  "C" {
#endif

  Ipp32s  ics_apply_scale_factors(s_SE_Individual_channel_stream * pData,
                                  Ipp32f *p_spectrum);
  Ipp32s  cpe_apply_ms(sCpe_channel_element * pElement, Ipp32f *l_spec,
                       Ipp32f *r_spec);
  Ipp32s  cpe_apply_intensity(sCpe_channel_element * pElement, Ipp32f *l_spec,
                              Ipp32f *r_spec);

  Ipp32f aacdec_noise_generator(Ipp32f *dst,
                                Ipp32s len,
                                Ipp32s *noiseState);

  Ipp32s  apply_pns(s_SE_Individual_channel_stream *pDataL,
                    s_SE_Individual_channel_stream *pDataR,
                    Ipp32f *p_spectrumL,
                    Ipp32f *p_spectrumR,
                    Ipp32s  numCh,
                    Ipp32s  ms_mask_present,
                    Ipp32s  ms_used[8][49],
                    Ipp32s  *noiseState);
  Ipp32s  apply_pns_ref(s_SE_Individual_channel_stream *pDataL,
                    s_SE_Individual_channel_stream *pDataR,
                    Ipp32f *p_spectrumL,
                    Ipp32f *p_spectrumR,
                    Ipp32s  numCh,
                    Ipp32s  ms_mask_present,
                    Ipp32s  ms_used[8][49],
                    Ipp32s  *noiseState);

  void FDP(Ipp32f *p_spectrum,
           s_SE_Individual_channel_stream *pData,
           IppsFDPState_32f *pFDPState);

  Ipp32s deinterlieve(s_SE_Individual_channel_stream *pData,
                      Ipp32f *p_spectrum);

  void ssr_gain_control(Ipp32f               *spectrum_data,
                        Ipp32s               curr_win_shape,
                        Ipp32s               prev_win_shape,
                        Ipp32s               curr_win_sequence,
                        SSR_GAIN             **SSRInfo,
                        SSR_GAIN             *prevSSRInfo,
                        Ipp32f               *samples,
                        Ipp32f               *m_gcOverlapBuffer,
                        ownIppsIPQFState_32f *SSR_IPQFState,
                        sFilterbank          *p_data,
                        Ipp32s               len);

  void coupling_gain_calculation(sCoupling_channel_element *pElement,
                                 sCoupling_channel_data *pData,
                                 Ipp32f cc_gain[18][MAX_GROUP_NUMBER][MAX_SFB]);

  void coupling_spectrum(AACDec *state,
                         sCoupling_channel_data *pData,
                         Ipp32f *c_spectrum,
                         Ipp32f cc_gain[18][MAX_GROUP_NUMBER][MAX_SFB]);

  void coupling_samples(AACDec *state,
                        sCoupling_channel_data *pData,
                        Ipp32f *c_samlpes,
                        Ipp32f cc_gain[18][MAX_GROUP_NUMBER][MAX_SFB]);

#ifdef  __cplusplus
}
#endif
#endif
