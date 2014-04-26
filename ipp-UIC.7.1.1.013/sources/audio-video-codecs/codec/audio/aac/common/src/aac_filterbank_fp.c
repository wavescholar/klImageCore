/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER) || defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include "align.h"
#include <stdlib.h>
#include <math.h>
#include "aac_filterbank_fp.h"
#include "aac_wnd_tables_fp.h"

#include "ipps.h"

/********************************************************************/

AACStatus InitFilterbank(sFilterbank* pBlock,
                         Ipp8u* mem,
                         Ipp32s mode,
                         enum AudioObjectType audioObjectType,
                         Ipp32s *sizeAll)
{
  Ipp32s i;
  Ipp32s j;
  Ipp32s sizeInit = 0;
  Ipp32s sizeWork = 0;
  Ipp32s sizeSpecInvLong = 0;
  Ipp32s sizeSpecInvShort = 0;
  Ipp32s sizeSpecFwdLong = 0;
  Ipp32s sizeSpecFwdShort = 0;
  Ipp32s sizeInitTmp = 0;
  Ipp32s sizeWorkTmp = 0;
  Ipp32s len_long = N_LONG;
  Ipp32s len_short = N_SHORT;
  Ipp32f *KBD_win_long = KBD_long;
  Ipp32f *KBD_win_short = KBD_short;
  Ipp8u  *ptrInvShort, *ptrFwdLong, *ptrFwdShort, *ptrWork, *ptrInit;

  if (pBlock) {
    if (AOT_AAC_SSR == audioObjectType) {
      len_long = N_LONG/4;
      len_short = N_SHORT/4;
      KBD_win_long = KBD_long_ssr;
      KBD_win_short = KBD_short_ssr;
    }

    ippsZero_8u((Ipp8u*)pBlock, sizeof(sFilterbank));
  }

  if (mode & FB_DECODER) {
    if (ippsMDCTInvGetSize_32f(len_long, &sizeSpecInvLong,
      &sizeInitTmp, &sizeWorkTmp) != ippStsOk) {
      return AAC_ALLOC;
    }

    if (sizeInit < sizeInitTmp) sizeInit = sizeInitTmp;
    if (sizeWork < sizeWorkTmp) sizeWork = sizeWorkTmp;

    if (ippsMDCTInvGetSize_32f(len_short, &sizeSpecInvShort,
        &sizeInitTmp, &sizeWorkTmp) != ippStsOk) {
        return AAC_ALLOC;
    }

    if (sizeInit < sizeInitTmp) sizeInit = sizeInitTmp;
    if (sizeWork < sizeWorkTmp) sizeWork = sizeWorkTmp;
  }

  if (mode & FB_ENCODER) {
    if (ippsMDCTFwdGetSize_32f(len_long, &sizeSpecFwdLong,
      &sizeInitTmp, &sizeWorkTmp) != ippStsOk) {
      return AAC_ALLOC;
    }

    if (sizeInit < sizeInitTmp) sizeInit = sizeInitTmp;
    if (sizeWork < sizeWorkTmp) sizeWork = sizeWorkTmp;

    if (ippsMDCTFwdGetSize_32f(len_short, &sizeSpecFwdShort,
        &sizeInitTmp, &sizeWorkTmp) != ippStsOk) {
      return AAC_ALLOC;
    }

    if (sizeInit < sizeInitTmp) sizeInit = sizeInitTmp;
    if (sizeWork < sizeWorkTmp) sizeWork = sizeWorkTmp;
  }

  *sizeAll = sizeSpecInvLong + sizeSpecInvShort + sizeSpecFwdLong +
             sizeSpecFwdShort + sizeWork + sizeInit;

  if (pBlock) {
    ptrInvShort = mem + sizeSpecInvLong;
    ptrFwdLong = ptrInvShort + sizeSpecInvShort;
    ptrFwdShort = ptrFwdLong + sizeSpecFwdLong;
    ptrWork = ptrFwdShort + sizeSpecFwdShort;
    ptrInit = ptrWork + sizeWork;

    if (mode & FB_DECODER) {
      pBlock->p_mdct_inv_long = (IppsMDCTInvSpec_32f*)mem;
      if (ippsMDCTInvInit_32f(&pBlock->p_mdct_inv_long, len_long,
        mem, ptrInit) != ippStsOk)
        return AAC_ALLOC;

      pBlock->p_mdct_inv_short = (IppsMDCTInvSpec_32f*)ptrInvShort;
      if (ippsMDCTInvInit_32f(&pBlock->p_mdct_inv_short, len_short,
        ptrInvShort, ptrInit) != ippStsOk)
        return AAC_ALLOC;
    }

    if (mode & FB_ENCODER) {
      pBlock->p_mdct_fwd_long = (IppsMDCTFwdSpec_32f*)ptrFwdLong;
      if (ippsMDCTFwdInit_32f(&pBlock->p_mdct_fwd_long, len_long,
        ptrFwdLong, ptrInit) != ippStsOk)
        return AAC_ALLOC;

      pBlock->p_mdct_fwd_short = (IppsMDCTFwdSpec_32f*)ptrFwdShort;
      if (ippsMDCTFwdInit_32f(&pBlock->p_mdct_fwd_short, len_short,
        ptrFwdShort, ptrInit) != ippStsOk)
        return AAC_ALLOC;
    }

    pBlock->p_buffer_inv = ptrWork;
    pBlock->p_buffer_fwd = ptrWork;

    for (i = 0; i < len_short; i++) {
      pBlock->sin_short_wnd_table[i] =
        (Ipp32f)sin(PI/(2.0f * len_short/2) * (i + 0.5));
    }

    for (i = 0; i < len_long; i++) {
      pBlock->sin_long_wnd_table[i] =
        (Ipp32f)sin(PI/(2.0f * len_long/2) * (i + 0.5));
    }

    for (i = 0, j = (len_short)-1; i < len_short /2; i++, j--) {
      pBlock->KBD_short_wnd_table[j] =
        pBlock->KBD_short_wnd_table[i] = KBD_win_short[i];
    }

    for (i = 0, j = (len_long)-1; i < len_long/2; i++, j--) {
      pBlock->KBD_long_wnd_table[j] =
        pBlock->KBD_long_wnd_table[i] = KBD_win_long[i];
    }
  }

  return AAC_OK;
}

/********************************************************************/

void FilterbankDec(sFilterbank* p_data,
                   Ipp32f* p_in_spectrum,
                   Ipp32f* p_in_prev_samples,
                   Ipp32s  window_sequence,
                   Ipp32s  window_shape,
                   Ipp32s  prev_window_shape,
                   Ipp32f* p_out_samples_1st_part,
                   Ipp32f* p_out_samples_2nd_part)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, samples, N_LONG);
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_out_short, N_SHORT);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, samples, N_LONG);
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_out_short, N_SHORT);
#endif
  Ipp32f* p_samples;
  Ipp32f* p_spectrum;
  Ipp32f *window_table_long, *prev_window_table_long;
  Ipp32f *window_table_short, *prev_window_table_short;
  Ipp32s j;

  if (0 == window_shape) {
    window_table_long  = p_data->sin_long_wnd_table;
    window_table_short = p_data->sin_short_wnd_table;
  } else {
    window_table_long  = p_data->KBD_long_wnd_table;
    window_table_short = p_data->KBD_short_wnd_table;
  }

  if (0 == prev_window_shape) {
    prev_window_table_long  = p_data->sin_long_wnd_table;
    prev_window_table_short = p_data->sin_short_wnd_table;
  } else {
    prev_window_table_long  = p_data->KBD_long_wnd_table;
    prev_window_table_short = p_data->KBD_short_wnd_table;
  }

  switch(window_sequence)
  {
  case ONLY_LONG_SEQUENCE:
    ippsMDCTInv_32f(p_in_spectrum, samples, p_data->p_mdct_inv_long,
                    p_data->p_buffer_inv);

    ippsMul_32f(prev_window_table_long, samples,
                p_out_samples_1st_part, N_LONG/2);

    ippsAdd_32f_I(p_in_prev_samples,p_out_samples_1st_part,N_LONG/2);

    ippsMul_32f(&window_table_long[N_LONG/2], &samples[N_LONG/2],
                p_out_samples_2nd_part, N_LONG/2);
    break;
  case LONG_START_SEQUENCE:
    ippsMDCTInv_32f(p_in_spectrum, samples, p_data->p_mdct_inv_long,
                    p_data->p_buffer_inv);

    ippsMul_32f(prev_window_table_long, samples,
                p_out_samples_1st_part, N_LONG/2);

    ippsAdd_32f_I(p_in_prev_samples, p_out_samples_1st_part, N_LONG/2);

    ippsCopy_32f(&samples[N_LONG / 2], p_out_samples_2nd_part,
                ((3*N_LONG-N_SHORT)/4-N_LONG/2));

    ippsMul_32f(&window_table_short[N_SHORT / 2],
                &samples[(3*N_LONG-N_SHORT)/4],
                &p_out_samples_2nd_part[((3*N_LONG-N_SHORT)/4-N_LONG/2)],
                N_SHORT/2);

    ippsZero_32f(&p_out_samples_2nd_part[(3*N_LONG+N_SHORT)/4-N_LONG/2],
                  N_LONG-(3*N_LONG+N_SHORT)/4);
    break;
  case LONG_STOP_SEQUENCE:
    ippsMDCTInv_32f(p_in_spectrum, samples, p_data->p_mdct_inv_long,
                    p_data->p_buffer_inv);

    ippsZero_32f(p_out_samples_1st_part, (N_LONG-N_SHORT)/4);

    ippsMul_32f(prev_window_table_short, &samples[(N_LONG-N_SHORT)/4],
                &p_out_samples_1st_part[(N_LONG-N_SHORT)/4], N_SHORT/2);

    ippsCopy_32f(&samples[(N_LONG+N_SHORT)/4],
                  &p_out_samples_1st_part[(N_LONG + N_SHORT)/4],
                  ((N_LONG/2)-((N_LONG+N_SHORT)/4)));

    ippsAdd_32f_I(p_in_prev_samples, p_out_samples_1st_part, N_LONG/2);

    ippsMul_32f(&window_table_long[N_LONG/2],
                &samples[N_LONG/2], p_out_samples_2nd_part, N_LONG/2);
    break;
  case EIGHT_SHORT_SEQUENCE:
    p_samples = samples;
    p_spectrum = p_in_spectrum;

    ippsZero_32f(p_samples, N_LONG);
    p_samples += (N_LONG - N_SHORT) / 4;
    ippsMDCTInv_32f(p_spectrum, (Ipp32f*)mdct_out_short,
                    p_data->p_mdct_inv_short, p_data->p_buffer_inv);

    ippsMul_32f(prev_window_table_short, mdct_out_short, p_samples, N_SHORT/2);
    ippsMul_32f(&window_table_short[N_SHORT/2], &mdct_out_short[N_SHORT/2],
                &p_samples[N_SHORT/2],N_SHORT/2);

    p_samples  += N_SHORT/2;
    p_spectrum += N_SHORT/2;

    for (j = 1; j < 8; j++) {
      ippsMDCTInv_32f(p_spectrum, (Ipp32f*)mdct_out_short,
                      p_data->p_mdct_inv_short, p_data->p_buffer_inv);
      ippsMul_32f_I(window_table_short,(Ipp32f*)mdct_out_short, N_SHORT);
      ippsAdd_32f_I((Ipp32f*)mdct_out_short, p_samples, N_SHORT);
      p_samples  += N_SHORT/2;
      p_spectrum += N_SHORT/2;
    }

    ippsAdd_32f(p_in_prev_samples, samples,
                p_out_samples_1st_part, N_LONG/2);
    ippsCopy_32f(&samples[N_LONG/2], p_out_samples_2nd_part, N_LONG/2);
    break;
  }
}

/********************************************************************/

void FilterbankDecSSR(sFilterbank* p_data,
                      Ipp32f* p_in_spectrum,
                      Ipp32s  window_sequence,
                      Ipp32s  window_shape,
                      Ipp32s  prev_window_shape,
                      Ipp32f* p_out_samples)
{
  Ipp32f* p_in;
  Ipp32f* p_out;
  Ipp32f *window_table_long, *prev_window_table_long;
  Ipp32f *window_table_short, *prev_window_table_short;
  Ipp32s j;

  if (0 == window_shape) {
    window_table_long  = p_data->sin_long_wnd_table;
    window_table_short = p_data->sin_short_wnd_table;
  } else {
    window_table_long  = p_data->KBD_long_wnd_table;
    window_table_short = p_data->KBD_short_wnd_table;
  }

  if (0 == prev_window_shape) {
    prev_window_table_long  = p_data->sin_long_wnd_table;
    prev_window_table_short = p_data->sin_short_wnd_table;
  } else {
    prev_window_table_long  = p_data->KBD_long_wnd_table;
    prev_window_table_short = p_data->KBD_short_wnd_table;
  }

  switch(window_sequence)
  {
  case ONLY_LONG_SEQUENCE:
    ippsMDCTInv_32f(p_in_spectrum, p_out_samples,
                    p_data->p_mdct_inv_long, p_data->p_buffer_inv);

    ippsMul_32f_I(prev_window_table_long, p_out_samples, N_LONG/8);

    ippsMul_32f_I(&window_table_long[N_LONG/8],
                  &p_out_samples[N_LONG/8], N_LONG/8);
    break;
  case LONG_START_SEQUENCE:
    ippsMDCTInv_32f(p_in_spectrum, p_out_samples,
                    p_data->p_mdct_inv_long, p_data->p_buffer_inv);

    ippsMul_32f_I(prev_window_table_long, p_out_samples, N_LONG/8);

    ippsMul_32f_I(&window_table_short[N_SHORT/8],
                  &p_out_samples[(3*N_LONG-N_SHORT)/16], N_SHORT/8);

    ippsZero_32f(&p_out_samples[(3*N_LONG+N_SHORT)/16],
                 (N_LONG-N_SHORT)/16);
    break;
  case LONG_STOP_SEQUENCE:
    ippsMDCTInv_32f(p_in_spectrum, p_out_samples,
                    p_data->p_mdct_inv_long, p_data->p_buffer_inv);

    ippsZero_32f(p_out_samples, (N_LONG-N_SHORT)/16);

    ippsMul_32f_I(prev_window_table_short,
                  &p_out_samples[(N_LONG-N_SHORT)/16], N_SHORT/8);

    ippsMul_32f_I(&window_table_long[N_LONG/8],
                  &p_out_samples[N_LONG/8], N_LONG/8);
    break;
  case EIGHT_SHORT_SEQUENCE:
    p_in = p_in_spectrum;
    p_out = p_out_samples;

    ippsMDCTInv_32f(p_in, p_out, p_data->p_mdct_inv_short,
                    p_data->p_buffer_inv);

    ippsMul_32f_I(prev_window_table_short, p_out, N_SHORT/8);

    ippsMul_32f_I(&window_table_short[N_SHORT/8],
                  &p_out[N_SHORT/8], N_SHORT/8);

    p_in += N_SHORT/8;
    p_out += N_SHORT/4;

    for (j = 1; j < 8; j++) {
      ippsMDCTInv_32f(p_in, p_out, p_data->p_mdct_inv_short,
                      p_data->p_buffer_inv);

      ippsMul_32f_I(window_table_short, p_out, N_SHORT/4);

      p_in += N_SHORT/8;
      p_out += N_SHORT/4;
    }
    break;
  }
}

/********************************************************************/

void FilterbankEnc(sFilterbank* p_data,
                   Ipp32f* p_in_samples_1st_part,
                   Ipp32f* p_in_samples_2nd_part,
                   Ipp32s  window_sequence,
                   Ipp32s  window_shape,
                   Ipp32s  prev_window_shape,
                   Ipp32f* p_out_spectrum,
                   Ipp32s ltp)
{
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_in, N_LONG);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp32f, mdct_in, N_LONG);
#endif
  Ipp32f *window_table_long, *prev_window_table_long;
  Ipp32f *window_table_short, *prev_window_table_short;
  Ipp32s i;

  if (0 == window_shape) {
    window_table_long  = p_data->sin_long_wnd_table;
    window_table_short = p_data->sin_short_wnd_table;
  } else {
      window_table_long  = p_data->KBD_long_wnd_table;
      window_table_short = p_data->KBD_short_wnd_table;
  }

  if (0 == prev_window_shape) {
      prev_window_table_long  = p_data->sin_long_wnd_table;
      prev_window_table_short = p_data->sin_short_wnd_table;
  } else {
      prev_window_table_long  = p_data->KBD_long_wnd_table;
      prev_window_table_short = p_data->KBD_short_wnd_table;
  }

  switch(window_sequence)
  {
  case ONLY_LONG_SEQUENCE:
    ippsMul_32f(p_in_samples_1st_part, prev_window_table_long,
                mdct_in, N_LONG/2);

    ippsMul_32f(p_in_samples_2nd_part,
                &window_table_long[N_LONG/2],
                &mdct_in[N_LONG/2], N_LONG/2);

    ippsMDCTFwd_32f(mdct_in, p_out_spectrum, p_data->p_mdct_fwd_long,
                    p_data->p_buffer_fwd);
    break;
  case LONG_START_SEQUENCE:
    ippsMul_32f(p_in_samples_1st_part, prev_window_table_long,
                mdct_in, N_LONG/2);

    ippsCopy_32f(p_in_samples_2nd_part, &mdct_in[1024], 1472-1024);

    ippsMul_32f(&p_in_samples_2nd_part[1472-1024],
                &window_table_short[N_SHORT/2],
                &mdct_in[1472], 1600-1472);

    ippsZero_32f(&mdct_in[1600], (2048-1600));

    ippsMDCTFwd_32f(mdct_in, p_out_spectrum,p_data->p_mdct_fwd_long,
                    p_data->p_buffer_fwd);
    break;
  case LONG_STOP_SEQUENCE:
    ippsZero_32f(mdct_in, 448);

    ippsMul_32f(&p_in_samples_1st_part[448], prev_window_table_short,
                &mdct_in[448], 576-448);

    ippsCopy_32f(&p_in_samples_1st_part[576], &mdct_in[576], 1024-576);

    ippsMul_32f(p_in_samples_2nd_part,
                &window_table_long[N_LONG/2],
                &mdct_in[N_LONG/2], N_LONG/2);

    ippsMDCTFwd_32f(mdct_in, p_out_spectrum, p_data->p_mdct_fwd_long,
                    p_data->p_buffer_fwd);
    break;
  case EIGHT_SHORT_SEQUENCE:
    if (ltp) {
      ippsMul_32f(&p_in_samples_1st_part[0],
                    prev_window_table_short, &mdct_in[0], N_SHORT/2);
      ippsMul_32f(&p_in_samples_1st_part[128],
                  &window_table_short[N_SHORT/2], &mdct_in[N_SHORT/2], N_SHORT/2);

      ippsMDCTFwd_32f(mdct_in,
                      p_out_spectrum,
                      p_data->p_mdct_fwd_short,
                      p_data->p_buffer_fwd);
    } else {
      /// W0
      ippsMul_32f(&p_in_samples_1st_part[448],
                  prev_window_table_short, &mdct_in[0], N_SHORT/2);
      ippsMul_32f(&p_in_samples_1st_part[448+128],
                  &window_table_short[N_SHORT/2], &mdct_in[N_SHORT/2], N_SHORT/2);
      /// W1
      ippsMul_32f(&p_in_samples_1st_part[576],
                  window_table_short, &mdct_in[N_SHORT], N_SHORT);
      /// W2
      ippsMul_32f(&p_in_samples_1st_part[704],
                  window_table_short, &mdct_in[2*N_SHORT], N_SHORT);
      /// W3
      ippsMul_32f(&p_in_samples_1st_part[832],
                  window_table_short, &mdct_in[3*N_SHORT], (3*N_SHORT)/4);
      ippsMul_32f(p_in_samples_2nd_part,
                  &window_table_short[192], &mdct_in[3*N_SHORT+192], N_SHORT/4);

      /// W4
      ippsMul_32f(&p_in_samples_1st_part[960],
                  window_table_short, &mdct_in[4*N_SHORT], N_SHORT/4);
      ippsMul_32f(p_in_samples_2nd_part,
                  &window_table_short[64], &mdct_in[4*N_SHORT+64], (3*N_SHORT)/4);

      /// W5
      ippsMul_32f(&p_in_samples_2nd_part[64],
                  window_table_short, &mdct_in[5*N_SHORT], N_SHORT);
      /// W6
      ippsMul_32f(&p_in_samples_2nd_part[64+1*128],
                  window_table_short, &mdct_in[6*N_SHORT], N_SHORT);
      /// W7
      ippsMul_32f(&p_in_samples_2nd_part[64+2*128],
                  window_table_short, &mdct_in[7*N_SHORT], N_SHORT);

      for (i = 0; i < 8; i ++) {
        ippsMDCTFwd_32f(&mdct_in[N_SHORT*i],
                        p_out_spectrum+i*(N_SHORT/2),
                        p_data->p_mdct_fwd_short,
                        p_data->p_buffer_fwd);
      }
    }

    break;
  default:
    break;
  }
}

#endif //UMC_ENABLE_XXX

