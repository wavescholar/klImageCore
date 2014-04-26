/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __SAACDEC_FILTERBANK_H
#define __SAACDEC_FILTERBANK_H

#include "ippdefs.h"
#include "ippac.h"
#include "aaccmn_const.h"
#include "aac_status.h"

typedef struct {
  Ipp8u  *pMDCTInvLongMem;
  IppsMDCTInvSpec_32f *p_mdct_inv_long;
  IppsMDCTInvSpec_32f *p_mdct_inv_short;
  Ipp8u  *p_buffer_inv;

  IppsMDCTFwdSpec_32f *p_mdct_fwd_long;
  IppsMDCTFwdSpec_32f *p_mdct_fwd_short;
  Ipp8u  *p_buffer_fwd;

  Ipp32f  KBD_long_wnd_table[N_LONG];
  Ipp32f  KBD_short_wnd_table[N_SHORT];
  Ipp32f  sin_long_wnd_table[N_LONG];
  Ipp32f  sin_short_wnd_table[N_SHORT];
} sFilterbank;

#ifdef  __cplusplus
extern  "C" {
#endif

  AACStatus InitFilterbank(sFilterbank* pBlock, Ipp8u* mem, Ipp32s mode,
                           enum AudioObjectType audioObjectType, Ipp32s *sizeAll);
  void FreeFilterbank(sFilterbank* pBlock);
  void    FilterbankDec(sFilterbank * p_data, Ipp32f *p_in_spectrum,
                        Ipp32f *p_in_prev_samples, Ipp32s window_sequence,
                        Ipp32s window_shape, Ipp32s prev_window_shape,
                        Ipp32f *p_out_samples_1st, Ipp32f *p_out_samples_2nd);
  void    FilterbankDecSSR(sFilterbank* p_data, Ipp32f* p_in_spectrum,
                           Ipp32s window_sequence, Ipp32s window_shape,
                           Ipp32s prev_window_shape, Ipp32f* p_out_samples);
  void    FilterbankEnc(sFilterbank * p_data, Ipp32f *p_in_samples_1st_part,
                        Ipp32f *p_in_samples_2nd_part, Ipp32s window_sequence,
                        Ipp32s window_shape, Ipp32s prev_window_shape,
                        Ipp32f *p_out_spectrum, Ipp32s ltp);

#ifdef  __cplusplus
}
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

#endif             // __SAACDEC_FILTERBANK_H
