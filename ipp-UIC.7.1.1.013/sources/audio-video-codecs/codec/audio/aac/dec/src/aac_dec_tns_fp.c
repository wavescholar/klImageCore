/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives AAC Decode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "aac_dec_tns_fp.h"

/********************************************************************/

static void tns_ar_filter_plus(Ipp32f *p_spectrum,
                               Ipp32s size,
                               Ipp32f *lpc_coef,
                               Ipp32s order);
static void tns_ar_filter_minus(Ipp32f *p_spectrum,
                                Ipp32s size,
                                Ipp32f *lpc_coef,
                                Ipp32s order);
static void tns_ma_filter(Ipp32f *p_spectrum,
                          Ipp32s size,
                          Ipp32s inc,
                          Ipp32f *lpc_coef,
                          Ipp32s order);

/********************************************************************/

void ics_apply_tns_enc_I(s_tns_data *p_data,
                         Ipp32f *p_spectrum)
{
  Ipp32s  w;
  Ipp32s  f;

  for (w = 0; w < p_data->m_num_windows; w++) {
    for (f = 0; f < p_data->m_n_filt[w]; f++) {
      if (0 == p_data->m_order[w][f])
        continue;
      tns_ma_filter(&(p_spectrum[w * 128 + p_data->m_start[w][f]]),
                    p_data->m_size[w][f], p_data->m_inc[w][f],
                    p_data->m_lpc[w][f], p_data->m_order[w][f]);
    }
  }
}

/********************************************************************/

void ics_apply_tns_dec_I(s_tns_data *p_data,
                         Ipp32f *p_spectrum)
{
  Ipp32s  w;
  Ipp32s  f;

  for (w = 0; w < p_data->m_num_windows; w++) {
    for (f = 0; f < p_data->m_n_filt[w]; f++) {
      if (0 == p_data->m_order[w][f])
        continue;

      if (p_data->m_inc[w][f] > 0) {
        tns_ar_filter_plus(&(p_spectrum[w * 128 + p_data->m_start[w][f]]),
                           p_data->m_size[w][f], p_data->m_lpc[w][f],
                           p_data->m_order[w][f]);
      } else {
        tns_ar_filter_minus(&(p_spectrum[w * 128 + p_data->m_start[w][f]]),
                            p_data->m_size[w][f], p_data->m_lpc[w][f],
                            p_data->m_order[w][f]);
      }
    }
  }
}

/********************************************************************/

static void tns_decode_coef(Ipp32s order,
                            Ipp32s coef_res_bits,
                            Ipp32s coef_compress,
                            Ipp32s *coef,
                            Ipp32f *a);

/********************************************************************/

void ics_calc_tns_data(s_SE_Individual_channel_stream *p_stream,
                       s_tns_data *p_data)
{
  Ipp32s  w;
  Ipp32s  f;
  Ipp32s  bottom;
  Ipp32s  top;
  Ipp32s  tns_order;
  Ipp32s  min_sfb;
  Ipp32s  tns_max_bands;
  Ipp32s  tns_max_order;
  Ipp32s  start;
  Ipp32s  end;
  Ipp32s *p_sfb_offset;
  Ipp32s  size;
  Ipp32s  num_swb;

  p_data->m_tns_data_present = p_stream->tns_data_present;

  if (0 == p_stream->tns_data_present)
    return;

  if (EIGHT_SHORT_SEQUENCE == p_stream->window_sequence) {
    tns_max_bands = p_stream->tns_max_bands_short;
    tns_max_order = p_stream->tns_max_order_short;
    p_sfb_offset = p_stream->sfb_offset_short_window;
    num_swb = p_stream->num_swb_short;
  } else {
    tns_max_bands = p_stream->tns_max_bands_long;
    tns_max_order = p_stream->tns_max_order_long;
    p_sfb_offset = p_stream->sfb_offset_long_window;
    num_swb = p_stream->num_swb_long;
  }

  for (w = 0; w < p_stream->num_windows; w++) {
    bottom = num_swb;

    for (f = 0; f < p_stream->n_filt[w]; f++) {
      p_data->m_order[w][f] = 0;
      top = bottom;
      bottom = MAX(top - p_stream->length[w][f], 0);
      tns_order = MIN(p_stream->order[w][f], tns_max_order);

      if (0 == tns_order)
        continue;

      tns_decode_coef(tns_order, p_stream->coef_res[w] + 3,
                      p_stream->coef_compress[w][f], p_stream->coef[w][f],
                      p_data->m_lpc[w][f]);

      min_sfb = MIN(tns_max_bands, p_stream->max_sfb);

      start = p_sfb_offset[MIN(bottom, min_sfb)];
      end = p_sfb_offset[MIN(top, min_sfb)];

      if ((size = end - start) <= 0)
        continue;

      p_data->m_order[w][f] = tns_order;
      p_data->m_size[w][f] = size;;

      if (p_stream->direction[w][f]) {
        p_data->m_inc[w][f] = -1;
        p_data->m_start[w][f] = end - 1;
      } else {
        p_data->m_inc[w][f] = 1;
        p_data->m_start[w][f] = start;
      }

    }
    p_data->m_n_filt[w] = p_stream->n_filt[w];
  }

  p_data->m_num_windows = p_stream->num_windows;
}

/********************************************************************/

static const Ipp32f tnsTable[2][16] =
{
  { -0.342020143325f, -0.642787609686f, -0.866025403784f, -0.984807753012f,
    -0.984807753012f, -0.866025403784f, -0.642787609687f, -0.342020143326f,
     0.000000000000f,  0.433883739118f,  0.781831482468f,  0.974927912182f,
     0.974927912182f,  0.781831482468f,  0.433883739117f, -0.000000000000 },

  { -0.995734176295f, -0.961825643173f, -0.895163291355f, -0.798017227280f,
    -0.673695643647f, -0.526432162877f, -0.361241666187f, -0.183749517817f,
     0.000000000000f,  0.207911690818f,  0.406736643076f,  0.587785252293f,
     0.743144825477f,  0.866025403784f,  0.951056516295f,  0.994521895368f }
};

/********************************************************************/

static void tns_decode_coef(Ipp32s order,
                            Ipp32s coef_res_bits,
                            Ipp32s coef_compress,
                            Ipp32s *coef,
                            Ipp32f *a)
{
  Ipp32f  tmp2[TNS_MAX_ORDER + 1];
  Ipp32s  i, m;
  Ipp32s  coef_res2;

  /* Inverse quantization */

  coef_res2 = coef_res_bits - coef_compress;
  for (i = 0; i < order; i++) {
    Ipp32s ind = ((coef[i] << (32 - coef_res2)) >> (32 - coef_res2)) + 8;
    tmp2[i] = tnsTable[coef_res_bits - 3][ind];
  }

 /* Conversion to LPC coefficients */
  a[0] = 1;
  for (m = 1; m <= order; m++) {
    Ipp32f tmp = tmp2[m - 1];
    a[m] = tmp;
    for (i = 1; i <= (m >> 1); i++) {
      Ipp32f tmp0 = a[i];
      Ipp32f tmp1 = a[m - i];

      a[i] = tmp0 + tmp * tmp1;
      a[m - i] = tmp1 + tmp * tmp0;
    }
  }
}

/********************************************************************/

static void tns_ar_filter_plus(Ipp32f *p_spectrum,
                               Ipp32s size,
                               Ipp32f *lpc_coef,
                               Ipp32s order)
{
  Ipp32s  i, j;
  Ipp32f  y;
  Ipp32f  tmp;
  Ipp32s  size0;

  size0 = order;
  if (size < order) size0 = size;

  for (i = 0; i < size0; i++) {
    y = *p_spectrum;

    for (j = 0; j < i; j++) {
      tmp = lpc_coef[j + 1] * p_spectrum[-j-1];
      y -= tmp;
    }
    *p_spectrum = y;
    p_spectrum++;
  }

  for (i = order; i < size; i++) {
    y = *p_spectrum;

    for (j = 0; j < order; j++) {
      tmp = lpc_coef[j + 1] * p_spectrum[-j-1];
      y -= tmp;
    }
    *p_spectrum = y;
    p_spectrum++;
  }
}

/********************************************************************/

static void tns_ar_filter_minus(Ipp32f *p_spectrum,
                                Ipp32s size,
                                Ipp32f *lpc_coef,
                                Ipp32s order)
{
  Ipp32s  i, j;
  Ipp32f  y;
  Ipp32f  tmp;
  Ipp32s  size0;

  size0 = order;
  if (size < order) size0 = size;

  for (i = 0; i < size0; i++) {
    y = *p_spectrum;

    for (j = 0; j < i; j++) {
      tmp = lpc_coef[j + 1] * p_spectrum[j + 1];
      y -= tmp;
    }
    *p_spectrum = y;
    p_spectrum--;
  }

  for (i = order; i < size; i++) {
    y = *p_spectrum;

    for (j = 0; j < order; j++) {
      tmp = lpc_coef[j + 1] * p_spectrum[j + 1];
      y -= tmp;
    }
    *p_spectrum = y;
    p_spectrum--;
  }
}

/********************************************************************/

static void tns_ma_filter(Ipp32f *p_spectrum,
                          Ipp32s size,
                          Ipp32s inc,
                          Ipp32f *lpc_coef,
                          Ipp32s order)
{
  Ipp32f  flt_state[TNS_MAX_ORDER+1024];
  Ipp32f  *p_state = flt_state + size;
  Ipp32s  i, j;
  Ipp32f  y;
  Ipp32f  tmp;

  for (i = 0; i < order; i++)
    p_state[i] = 0;

  for (i = 0; i < size; i++) {
    y = *p_spectrum;

    for (j = 0; j < order; j++) {
      tmp = lpc_coef[j + 1] * p_state[j];
      y += tmp;
    }

    p_state--;
    p_state[0] = *p_spectrum;
    *p_spectrum = y;
    p_spectrum += inc;
  }
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

