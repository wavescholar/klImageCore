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

#include <math.h>
#include "aac_dec_fp.h"
#include "aac_dec_own.h"
#include "aac_dec_own_fp.h"

#include "ipps.h"

/********************************************************************/

static Ipp32s is_intensity(s_SE_Individual_channel_stream *pData,
                           Ipp32s group,
                           Ipp32s sfb);

static Ipp32s is_noise(s_SE_Individual_channel_stream *pData,
                       Ipp32s group,
                       Ipp32s sfb);

/********************************************************************/

Ipp32s ics_apply_scale_factors(s_SE_Individual_channel_stream *pData,
                               Ipp32f *p_spectrum)
{
  IPP_ALIGNED_ARRAY(32, Ipp32f, real_sf, 60);
  Ipp32s  g;
  Ipp32s  w_num;

  if (pData->window_sequence != EIGHT_SHORT_SEQUENCE) {
    if (pData->sfb_offset_long_window[pData->max_sfb] > 0) {
      ippsPow43_16s32f(pData->spectrum_data, p_spectrum,
        pData->sfb_offset_long_window[pData->max_sfb]);

      ippsCalcSF_16s32f(pData->sf[0], SF_OFFSET, real_sf, pData->max_sfb);
      ippsScale_32f_I(p_spectrum, real_sf, pData->sfb_offset_long_window,
                      pData->max_sfb);
    }

    ippsZero_32f(p_spectrum + pData->sfb_offset_long_window[pData->max_sfb],
                 1024 - pData->sfb_offset_long_window[pData->max_sfb]);

  } else {
    Ipp32s sfb_offset_buf[20], i;

    if (pData->sfb_offset_long_window[pData->max_sfb] > 0) {

      ippsPow43_16s32f(pData->spectrum_data, p_spectrum,
        pData->sfb_offset_short_window[pData->max_sfb] * 8);

      for (g = 0; g < pData->num_window_groups; g++) {
        w_num = pData->len_window_group[g];

        for (i = 0; i <= pData->max_sfb; i++) {
          sfb_offset_buf[i] = pData->sfb_offset_short_window[i] * w_num;
        }

        ippsCalcSF_16s32f(pData->sf[g], SF_OFFSET, real_sf, pData->max_sfb);
        ippsScale_32f_I(p_spectrum, real_sf, sfb_offset_buf, pData->max_sfb);

        p_spectrum +=
          w_num * pData->sfb_offset_short_window[pData->max_sfb];
      }
    }
  }

  return 0;
}

/********************************************************************/

Ipp32s cpe_apply_ms(sCpe_channel_element *pElement,
                    Ipp32f *l_spec,
                    Ipp32f *r_spec)
{
  Ipp32s i, g, sfb;
  Ipp32f tmp;
  Ipp32s *sfb_offset;
  Ipp32s num_window_groups = pElement->streams[0].num_window_groups;
  Ipp32s max_sfb = pElement->streams[0].max_sfb;
  s_SE_Individual_channel_stream *p_ics_right;

  p_ics_right = &pElement->streams[1];

  sfb_offset =
    pElement->streams[0].window_sequence == EIGHT_SHORT_SEQUENCE ?
    pElement->streams[0].sfb_offset_short_window :
    pElement->streams[0].sfb_offset_long_window;

  if (pElement->ms_mask_present >= 1) {
    for (g = 0; g < num_window_groups; g++) {
      Ipp32s len_window_group = pElement->streams[0].len_window_group[g];
      for (sfb = 0; sfb < max_sfb; sfb++) {
        if ((pElement->ms_used[g][sfb] || pElement->ms_mask_present == 2) &&
            !is_intensity(p_ics_right, g, sfb) &&
            !is_noise(p_ics_right, g, sfb)) {
          for (i = sfb_offset[sfb] * len_window_group;
               i < sfb_offset[sfb + 1]  * len_window_group; i++) {
            tmp = l_spec[i] - r_spec[i];
            l_spec[i] = l_spec[i] + r_spec[i];
            r_spec[i] = tmp;
          }
        }
      }
      l_spec += sfb_offset[max_sfb] * len_window_group;
      r_spec += sfb_offset[max_sfb] * len_window_group;
    }
  }
  return 0;
}

/********************************************************************/

static Ipp32s invert_intensity(sCpe_channel_element *pElement,
                               Ipp32s group,
                               Ipp32s sfb)
{
  if (pElement->ms_mask_present == 1)
    return (1 - 2 * pElement->ms_used[group][sfb]);
  return 1;
}

/********************************************************************/

Ipp32s cpe_apply_intensity(sCpe_channel_element *pElement,
                           Ipp32f *l_spec,
                           Ipp32f *r_spec)
{
  Ipp32s i, g, sfb;
  Ipp32f scale;
  Ipp32s *sfb_offset;
  Ipp32s num_window_groups;
  Ipp32s max_sfb;
  s_SE_Individual_channel_stream *p_ics_right;

  p_ics_right = &pElement->streams[1];

  sfb_offset =
    pElement->streams[0].window_sequence == EIGHT_SHORT_SEQUENCE ?
    pElement->streams[0].sfb_offset_short_window :
    pElement->streams[0].sfb_offset_long_window;

  num_window_groups = p_ics_right->num_window_groups;
  max_sfb = p_ics_right->max_sfb;

  for (g = 0; g < num_window_groups; g++) {
    Ipp32s len_window_group = p_ics_right->len_window_group[g];
    for (sfb = 0; sfb < max_sfb; sfb++) {
        if (is_intensity(p_ics_right, g, sfb)) {
          scale =
            is_intensity(p_ics_right, g, sfb) * invert_intensity(pElement, g,
                                                                 sfb) *
            (Ipp32f)pow(0.5, (0.25 * p_ics_right->sf[g][sfb]));

          for (i = sfb_offset[sfb] * len_window_group;
             i < sfb_offset[sfb + 1]  * len_window_group; i++) {
          r_spec[i] = l_spec[i] * scale;
        }
      }
    }
    l_spec += sfb_offset[max_sfb] * len_window_group;
    r_spec += sfb_offset[max_sfb] * len_window_group;
  }

  return 0;
}

/********************************************************************/

Ipp32s is_intensity(s_SE_Individual_channel_stream *pData,
                    Ipp32s group,
                    Ipp32s sfb)
{
  switch (pData->sfb_cb[group][sfb]) {
  case INTENSITY_HCB:
    return 1;
  case INTENSITY_HCB2:
    return -1;
  default:
    return 0;
  }
}

/********************************************************************/

Ipp32s is_noise(s_SE_Individual_channel_stream *pData,
                Ipp32s group,
                Ipp32s sfb)
{
  if (pData->sfb_cb[group][sfb] == NOISE_HCB)
    return 1;
  return 0;
}

/********************************************************************/

Ipp32f aacdec_noise_generator(Ipp32f *dst,
                              Ipp32s len,
                              Ipp32s *noiseState)
{
  Ipp32f norm;
  Ipp32s seed, i;

  norm = 0;
  seed = *noiseState;
  for (i = 0; i < len; i++) {
    seed = (1664525L * seed) + 1013904223L;
    dst[i] = (Ipp32f)seed;
    norm += dst[i] * dst[i];
  }
  *noiseState = seed;

  norm = (Ipp32f)(1 / sqrt(norm));
  return(norm);
}

/********************************************************************/

Ipp32s apply_pns(s_SE_Individual_channel_stream *pDataL,
                 s_SE_Individual_channel_stream *pDataR,
                 Ipp32f *p_spectrumL,
                 Ipp32f *p_spectrumR,
                 Ipp32s numCh,
                 Ipp32s ms_mask_present,
                 Ipp32s ms_used[8][49],
                 Ipp32s *noiseState)
{
  Ipp32s  i, g, sfb, ich;
  Ipp32f  norm;
  Ipp32s  *sfb_offset;
  Ipp32s  num_window_groups, max_sfb;
  s_SE_Individual_channel_stream *pData = pDataL;
  Ipp32f  *p_spectrum = p_spectrumL;
  Ipp32f  *p_spectrum_l = p_spectrumL;

  for (ich = 0; ich < numCh; ich++) {
    sfb_offset = pData->window_sequence == EIGHT_SHORT_SEQUENCE ?
      pData->sfb_offset_short_window : pData->sfb_offset_long_window;

    num_window_groups = pData->num_window_groups;
    max_sfb = pData->max_sfb;

    for (g = 0; g < num_window_groups; g++) {
      Ipp32s len_window_group = pData->len_window_group[g];
      for (sfb = 0; sfb < max_sfb; sfb++) {
        if (pData->sfb_cb[g][sfb] == NOISE_HCB) {
          Ipp32s tmp = 0;
          if ((ich == 1) && (ms_mask_present >= 1)) {
            if ((ms_used[g][sfb] != 0) || (ms_mask_present == 2)) {
              tmp = 1;
            }
          }

          if (tmp) {
            norm = (Ipp32f)pow(2.0, 0.25 * (pData->sf[g][sfb] - pDataL->sf[g][sfb]));

            for (i = sfb_offset[sfb] * len_window_group;
                  i < sfb_offset[sfb + 1]  * len_window_group; i++) {
              p_spectrum[i] = norm * p_spectrum_l[i];
            }
          } else {
            norm = aacdec_noise_generator(p_spectrum + sfb_offset[sfb] * len_window_group,
                                         (sfb_offset[sfb + 1] - sfb_offset[sfb]) * len_window_group,
                                          noiseState);

            norm *= (Ipp32f)pow(2.0, 0.25 * (pData->sf[g][sfb]));

            for (i = sfb_offset[sfb] * len_window_group;
                  i < sfb_offset[sfb + 1]  * len_window_group; i++) {
              p_spectrum[i] *= norm;
            }
          }
        }
      }
      p_spectrum += sfb_offset[max_sfb] * len_window_group;
      p_spectrum_l += sfb_offset[max_sfb] * len_window_group;
    }

    pData = pDataR;
    p_spectrum = p_spectrumR;
    p_spectrum_l = p_spectrumL;
  }

  return 0;
}

Ipp32s apply_pns_ref(s_SE_Individual_channel_stream *pDataL,
                 s_SE_Individual_channel_stream *pDataR,
                 Ipp32f *p_spectrumL,
                 Ipp32f *p_spectrumR,
                 Ipp32s numCh,
                 Ipp32s ms_mask_present,
                 Ipp32s ms_used[8][49],
                 Ipp32s *noiseState)
{
  Ipp32s  i, g, sfb, ich, l;
  Ipp32f  norm;
  Ipp32s  *sfb_offset;
  Ipp32s  num_window_groups, max_sfb;
  s_SE_Individual_channel_stream *pData = pDataL;
  Ipp32f  *p_spectrum = p_spectrumL;
  Ipp32f  *p_spectrum_l = p_spectrumL;
  Ipp32s  isNoise = 0;

  for (ich = 0; ich < numCh; ich++) {
    sfb_offset = pData->window_sequence == EIGHT_SHORT_SEQUENCE ?
      pData->sfb_offset_short_window : pData->sfb_offset_long_window;

    num_window_groups = pData->num_window_groups;
    max_sfb = pData->max_sfb;

    for (g = 0; g < num_window_groups; g++) {
      Ipp32s len_window_group = pData->len_window_group[g];
      for (l = 0; l < len_window_group; l++) {
        for (sfb = 0; sfb < max_sfb; sfb++) {
          if (pData->sfb_cb[g][sfb] == NOISE_HCB/* || pData->sfb_cb[g][sfb] == NOISE_HCB+100*/) {
            Ipp32s tmp = 0;
            isNoise = 1;
            if ((ich == 1) && (ms_mask_present >= 1)) {
                if ((ms_used[g][sfb] != 0) || (ms_mask_present == 2)) {
                tmp = 1;
                }
            }

            if (tmp) {
                norm = (Ipp32f)pow(2.0, 0.25 * (pData->sf[g][sfb] - pDataL->sf[g][sfb]));

                for (i = sfb_offset[sfb]; i < sfb_offset[sfb + 1]; i++) {
                    p_spectrum[i] = norm * p_spectrum_l[i];
                }
            } else {
//                norm = aacdec_noise_generator_ref(p_spectrum + sfb_offset[sfb],
//                                  (sfb_offset[sfb + 1] - sfb_offset[sfb]), noiseState);
                norm = aacdec_noise_generator(p_spectrum + sfb_offset[sfb],
                                  (sfb_offset[sfb + 1] - sfb_offset[sfb]), noiseState);

//                norm = (Ipp32f)pow(2.0, 0.25 * (pData->sf[g][sfb]));
                norm *= (Ipp32f)pow(2.0, 0.25 * (pData->sf[g][sfb]));

                for (i = sfb_offset[sfb]; i < sfb_offset[sfb + 1]; i++) {
                    p_spectrum[i] *= norm;
                }
            }
          }
        }
        p_spectrum += sfb_offset[max_sfb];
        p_spectrum_l += sfb_offset[max_sfb];
      }
    }

    pData = pDataR;
    p_spectrum = p_spectrumR;
    p_spectrum_l = p_spectrumL;
  }

  return isNoise;
}

/********************************************************************/

void FDP(Ipp32f *p_spectrum,
         s_SE_Individual_channel_stream *pData,
         IppsFDPState_32f *pFDPState)
{
  if (pData->window_sequence != EIGHT_SHORT_SEQUENCE) {

    ippsFDPInv_32f_I(p_spectrum, pData->sfb_offset_long_window,
                     pData->pred_max_sfb, pData->prediction_used,
                     pFDPState);

    if ((pData->predictor_reset) &&
         pData->predictor_reset_group_number > 0 &&
         pData->predictor_reset_group_number < 31) {

      ippsFDPResetGroup_32f(30, pData->predictor_reset_group_number - 1,
                            pFDPState);
    }
  } else {
    ippsFDPReset_32f(pFDPState);
  }
}

/********************************************************************/

Ipp32s deinterlieve(s_SE_Individual_channel_stream *pData,
                    Ipp32f *p_spectrum)
{
  if ((pData->window_sequence == EIGHT_SHORT_SEQUENCE)) {
    IPP_ALIGNED_ARRAY(32, Ipp32f, buf, 1024);
    Ipp32s   *sfb_offset = pData->sfb_offset_short_window;
    Ipp32s   num_window_groups = pData->num_window_groups;
    Ipp32s   max_sfb = pData->max_sfb;
    Ipp32f   *pBuf = buf;
    Ipp32s   g, sfb, b;

    ippsCopy_32f(p_spectrum, buf, 1024);

    for (g = 0; g < num_window_groups; g++) {
      Ipp32s len_window_group = pData->len_window_group[g];
      Ipp32s offset = 0;
      for (sfb = 0; sfb < max_sfb; sfb++) {
        Ipp32s len = sfb_offset[sfb + 1] - sfb_offset[sfb];
        for (b = 0; b < len_window_group; b++) {
          ippsCopy_32f(pBuf, p_spectrum + b * 128 + offset, (Ipp16s)len);
          pBuf += len;
        }
        offset += len;

        for (b = 0; b < len_window_group; b++) {
          ippsZero_32f(p_spectrum + b * 128 + offset, 128 - offset);
        }
      }
      p_spectrum += 128*len_window_group;
    }
  }
  return 0;
}

/********************************************************************/

static Ipp32f gain_table[4][8] = {
{ 1.0000000000000000f, 1.0905077326652577f,
  1.1892071150027210f, 1.2968395546510096f,
  1.4142135623730951f, 1.5422108254079410f,
  1.6817928305074292f, 1.8340080864093427f },
{ 1.0000000000000000f, 1.1892071150027210f,
  1.4142135623730949f, 1.6817928305074288f,
  0.f, 0.f, 0.f, 0.f},
{ 1.f, 1.4142135623730951f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f},
{ 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f}
};

/********************************************************************/

void coupling_gain_calculation(sCoupling_channel_element *pElement,
                               sCoupling_channel_data *pData,
                               Ipp32f cc_gain[18][MAX_GROUP_NUMBER][MAX_SFB])
{
  Ipp32f *pgain_table = gain_table[pData->gain_element_scale];
  Ipp32s shift = (3 - pData->gain_element_scale);
  Ipp32s ch = 0;
  Ipp32s c, g, sfb;

  for (c = 0; c < pData->num_gain_element_lists; c++) {
    if (pData->cge[c]) {
      Ipp32s   fac = -pElement->cc_fact[c][0][0];
      Ipp32s   ifac = fac >> shift;
      Ipp32s   x = ((127 + ifac) << 23);
      Ipp32f scale = (*(Ipp32f*)&x) * pgain_table[fac - (ifac << shift)];

      for (g = 0; g < pElement->stream.num_window_groups; g++) {
        for (sfb = 0; sfb < pElement->stream.max_sfb; sfb++) {
          if (pElement->stream.sfb_cb[g][sfb] != ZERO_HCB) {
            cc_gain[ch][g][sfb] = scale;
          } else {
            cc_gain[ch][g][sfb] = 0;
          }
        }
      }
    } else {
      if (pData->gain_element_sign) {
        for (g = 0; g < pElement->stream.num_window_groups; g++) {
          for (sfb = 0; sfb < pElement->stream.max_sfb; sfb++) {
            if (pElement->stream.sfb_cb[g][sfb] != ZERO_HCB) {
              Ipp32s   sign = pElement->cc_fact[c][g][sfb] & 1;
              Ipp32s   fac = -(pElement->cc_fact[c][g][sfb] >> 1);
              Ipp32s   ifac = fac >> shift;
              Ipp32s   x = ((127 + ifac) << 23);
              Ipp32f scale = (*(Ipp32f*)&x) * pgain_table[fac - (ifac << shift)];

              if (sign) scale = -scale;
              cc_gain[ch][g][sfb] = scale;
            } else {
              cc_gain[ch][g][sfb] = 0;
            }
          }
        }
      } else {
        for (g = 0; g < pElement->stream.num_window_groups; g++) {
          for (sfb = 0; sfb < pElement->stream.max_sfb; sfb++) {
            if (pElement->stream.sfb_cb[g][sfb] != ZERO_HCB) {
              Ipp32s   fac = -(pElement->cc_fact[c][g][sfb]);
              Ipp32s   ifac = fac >> shift;
              Ipp32s   x = ((127 + ifac) << 23);
              Ipp32f scale = (*(Ipp32f*)&x) * pgain_table[fac - (ifac << shift)];
              cc_gain[ch][g][sfb] = scale;
            } else {
              cc_gain[ch][g][sfb] = 0;
            }
          }
        }
      }
    }

    if (pData->cc_lr[c] == 3) {
      ch++;
    } else {
      ch += 2;
    }
  }
}

/********************************************************************/

void coupling_spectrum(AACDec *state,
                       sCoupling_channel_data *pData,
                       Ipp32f *c_spectrum,
                       Ipp32f cc_gain[18][MAX_GROUP_NUMBER][MAX_SFB])
{
  Ipp32f *m_spectrum, *pc_spectrum;
  Ipp32s c, ch, g, m_ch, sfb, w;
  Ipp32s *sfb_offset = pData->sfb_offset;
  Ipp32s max_sfb = pData->max_sfb;
  Ipp32s num_window_groups = pData->num_window_groups;
  Ipp32s num_el = 0;

  ch = 0;

  for (c = 0; c < pData->num_coupled_elements + 1; c++) {
    m_ch = pData->cc_target_ch[c];
    if (m_ch < 0) continue;

    /* if cpe and only right channel */
    if ((pData->cc_target_id[c]) && (pData->cc_lr[num_el] == 1))
      m_ch++;

    m_spectrum = state->m_spectrum_data[m_ch];
    pc_spectrum = c_spectrum;
    for (g = 0; g < num_window_groups; g++) {
      Ipp32s len_window_group = pData->len_window_group[g];
      for (w = 0; w < len_window_group; w++) {
        for (sfb = 0; sfb < max_sfb; sfb++) {
          Ipp32s len = sfb_offset[sfb + 1] - sfb_offset[sfb];
          ippsAddProductC_32f(pc_spectrum + sfb_offset[sfb], cc_gain[ch][g][sfb],
                              m_spectrum + sfb_offset[sfb], len);
        }
        m_spectrum += 128;
        pc_spectrum += 128;
      }
    }

    if (pData->cc_target_id[c]) { /* if cpe */
      if (pData->cc_lr[num_el] == 3) {

        m_spectrum = state->m_spectrum_data[m_ch+1];
        pc_spectrum = c_spectrum;
        for (g = 0; g < num_window_groups; g++) {
          Ipp32s len_window_group = pData->len_window_group[g];
          for (w = 0; w < len_window_group; w++) {
            for (sfb = 0; sfb < max_sfb; sfb++) {
              Ipp32s len = sfb_offset[sfb + 1] - sfb_offset[sfb];
              ippsAddProductC_32f(pc_spectrum + sfb_offset[sfb], cc_gain[ch+1][g][sfb],
                                  m_spectrum + sfb_offset[sfb], len);
            }
            m_spectrum += 128;
            pc_spectrum += 128;
          }
        }
        num_el++;
      } else if (pData->cc_lr[num_el] == 0) {
        m_spectrum = state->m_spectrum_data[m_ch+1];
        pc_spectrum = c_spectrum;
        for (g = 0; g < num_window_groups; g++) {
          Ipp32s len_window_group = pData->len_window_group[g];
          for (w = 0; w < len_window_group; w++) {
            for (sfb = 0; sfb < max_sfb; sfb++) {
              Ipp32s len = sfb_offset[sfb + 1] - sfb_offset[sfb];
              ippsAddProductC_32f(pc_spectrum + sfb_offset[sfb], cc_gain[ch][g][sfb],
                                  m_spectrum + sfb_offset[sfb], len);
            }
            m_spectrum += 128;
            pc_spectrum += 128;
          }
        }
      }
    }
    ch += 2;
    num_el++;
  }
}

/********************************************************************/

void coupling_samples(AACDec *state,
                      sCoupling_channel_data *pData,
                      Ipp32f *c_samlpes,
                      Ipp32f cc_gain[18][MAX_GROUP_NUMBER][MAX_SFB])
{
  Ipp32s c, ch, m_ch;
  Ipp32s num_el = 0;

  ch = 0;

  for (c = 0; c < pData->num_coupled_elements + 1; c++) {
    m_ch = pData->cc_target_ch[c];
    if (m_ch < 0) continue;

    /* if cpe and only right channel */
    if ((pData->cc_target_id[c]) && (pData->cc_lr[num_el] == 1))
      m_ch++;

    ippsAddProductC_32f(c_samlpes, cc_gain[ch][0][0],
                        state->m_curr_samples[m_ch], 1024);

    if (pData->cc_target_id[c]) { /* if cpe */
      if (pData->cc_lr[num_el] == 3) {
        ippsAddProductC_32f(c_samlpes, cc_gain[ch+1][0][0],
                            state->m_curr_samples[m_ch+1], 1024);
        num_el++;
      } else if (pData->cc_lr[num_el] == 0) {
        ippsAddProductC_32f(c_samlpes, cc_gain[ch][0][0],
                            state->m_curr_samples[m_ch+1], 1024);
      }
    }
    ch += 2;
    num_el++;
  }
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER


