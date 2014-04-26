/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "align.h"
#include "aac_dec_own.h"
#include "ippdc.h"
#include "ippac.h"

/********************************************************************/

Ipp32s dec_cpe_channel_element(sCpe_channel_element *pElement,
                               sBitsreamBuffer *pBS,
                               enum AudioObjectType audioObjectType)
{
  Ipp32s  g;
  Ipp32s  sfb;
  Ipp32s  max_sfb;
  Ipp32s  num_window_groups;
  Ipp32u  *crc_ptr = 0, *crc_ptr1 = 0;
  Ipp32s  crc_offset = 0, crc_offset1 = 0;
  Ipp32s  decodedBits0 = 0, decodedBits1 = 0, decodedBits2 = 0;

  if (pElement->crc_enable) {
    crc_ptr = pBS->pCurrent_dword;
    crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits0)
  }

  GET_BITS(pBS, pElement->element_instance_tag, 4, Ipp32s)

  GET_BITS(pBS, pElement->common_window, 1, Ipp32s)

  if (pElement->common_window) {
    if (dec_ics_info(&pElement->streams[0], pBS, audioObjectType) < 0)
      return -1;

    ics_info_copy(&pElement->streams[0], &pElement->streams[1],
                  audioObjectType);

    pElement->streams[1].ltp_data_present = 0;
    if (audioObjectType == AOT_AAC_LTP &&
        EIGHT_SHORT_SEQUENCE != pElement->streams[1].window_sequence) {
      if (pElement->streams[1].predictor_data_present) {
        GET_BITS(pBS, pElement->streams[1].ltp_data_present, 1, Ipp32s)
        if (pElement->streams[1].ltp_data_present) {
          dec_ltp_data(&pElement->streams[1], pBS, audioObjectType);
        }
      }
    }

    GET_BITS(pBS, pElement->ms_mask_present, 2, Ipp32s)
    if (pElement->ms_mask_present == 1) {
      max_sfb = pElement->streams[0].max_sfb;
      num_window_groups = pElement->streams[0].num_window_groups;
      for (g = 0; g < num_window_groups; g++) {
        for (sfb = 0; sfb < max_sfb; sfb++) {
          GET_BITS(pBS, pElement->ms_used[g][sfb], 1, Ipp32s)
        }
      }
    }
  } else {
    pElement->ms_mask_present = 0;
  }
  if (dec_individual_channel_stream
      (&pElement->streams[0], pBS, pElement->common_window, 0,
       audioObjectType) < 0)
    return -1;

  if (pElement->crc_enable) {
    crc_ptr1 = pBS->pCurrent_dword;
    crc_offset1 = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits1)
  }

  if (dec_individual_channel_stream
      (&pElement->streams[1], pBS, pElement->common_window, 0,
       audioObjectType) < 0)
    return -1;

  if (pElement->crc_enable) {
    Ipp32s len;
    Ipp32u crc = (Ipp32u)pElement->crc;

    GET_BITS_COUNT(pBS, decodedBits2)

    len = decodedBits2 - decodedBits0;
    if (len > 192) len = 192;
    bs_CRC_update(crc_ptr, crc_offset, len, &crc);

    if (len < 192)
      bs_CRC_update_zero(192 - len, &crc);

    len = decodedBits2 - decodedBits1;
    if (len > 128) len = 128;
    bs_CRC_update(crc_ptr1, crc_offset1, len, &crc);

    if (len < 128)
      bs_CRC_update_zero(128 - len, &crc);

    pElement->crc = (Ipp32s)crc;
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_sce_channel_element(sSce_channel_element *pElement,
                               sBitsreamBuffer *pBS,
                               enum AudioObjectType audioObjectType)
{
  Ipp32u  *crc_ptr = 0;
  Ipp32s  crc_offset = 0;
  Ipp32s  decodedBits0 = 0, decodedBits2 = 0;

  if (pElement->crc_enable) {
    crc_ptr = pBS->pCurrent_dword;
    crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits0)
  }

  GET_BITS(pBS, pElement->element_instance_tag, 4, Ipp32s)

  if (dec_individual_channel_stream
      (&pElement->stream, pBS, 0, 0, audioObjectType) < 0)
    return -1;

  if (pElement->crc_enable) {
    Ipp32s len;
    Ipp32u crc = (Ipp32u)pElement->crc;

    GET_BITS_COUNT(pBS, decodedBits2)

    len = decodedBits2 - decodedBits0;
    if (len > 192) len = 192;
    bs_CRC_update(crc_ptr, crc_offset, len, &crc);

    if (len < 192)
      bs_CRC_update_zero(192 - len, &crc);

    pElement->crc = (Ipp32s)crc;
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_lfe_channel_element(sLfe_channel_element *pElement,
                               sBitsreamBuffer *pBS,
                               enum AudioObjectType audioObjectType)
{
  Ipp32u  *crc_ptr = 0;
  Ipp32s  crc_offset = 0;
  Ipp32s  decodedBits0 = 0, decodedBits2 = 0;

  if (pElement->crc_enable) {
    crc_ptr = pBS->pCurrent_dword;
    crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits0)
  }

  GET_BITS(pBS, pElement->element_instance_tag, 4, Ipp32s)

  if (dec_individual_channel_stream
      (&pElement->stream, pBS, 0, 0, audioObjectType) < 0)
    return -1;

  if (pElement->crc_enable) {
    Ipp32s len;
    Ipp32u crc = (Ipp32u)pElement->crc;

    GET_BITS_COUNT(pBS, decodedBits2)

      len = decodedBits2 - decodedBits0;
    if (len > 192) len = 192;
    bs_CRC_update(crc_ptr, crc_offset, len, &crc);

    if (len < 192)
      bs_CRC_update_zero(192 - len, &crc);

    pElement->crc = (Ipp32s)crc;
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_individual_channel_stream(s_SE_Individual_channel_stream *pData,
                                     sBitsreamBuffer *pBS,
                                     Ipp32s common_window,
                                     Ipp32s scal_flag,
                                     enum AudioObjectType audioObjectType)
{
  GET_BITS(pBS, pData->global_gain, 8, Ipp32s)

  if (!common_window && !scal_flag) {
    if (dec_ics_info(pData, pBS, audioObjectType) < 0)
      return -1;
  }

  if (dec_section_data(pData, pBS) < 0)
    return -1;

  dec_scale_factor_data(pData, pData->sf, pData->global_gain,
                        pData->global_gain - 90, pBS);

  if (!scal_flag) {

    GET_BITS(pBS, pData->pulse_data_present, 1, Ipp32s)
    if (pData->pulse_data_present) {
      if (dec_pulse_data(pData, pBS) < 0)
        return -1;
    }

    GET_BITS(pBS, pData->tns_data_present, 1, Ipp32s)
    if (pData->tns_data_present) {
      dec_tns_data(pData, pBS);
    }
    GET_BITS(pBS, pData->gain_control_data_present, 1, Ipp32s)
    if (pData->gain_control_data_present) {
      if (AOT_AAC_SSR == audioObjectType) {
        dec_gain_control_data(pData, pBS);
      } else {
        return -1;
      }
    }
  }

  dec_spectral_data(pData, pBS);

  return 0;
}

/********************************************************************/

Ipp32s dec_ics_info(s_SE_Individual_channel_stream *pData,
                    sBitsreamBuffer *pBS,
                    enum AudioObjectType audioObjectType)
{
  Ipp32s  i;
  Ipp32s  pred_min_sfb;

  GET_BITS(pBS, pData->ics_reserved_bit, 1, Ipp32s)
  GET_BITS(pBS, pData->window_sequence, 2, Ipp32s)
  GET_BITS(pBS, pData->window_shape, 1, Ipp32s)

  pData->num_window_groups = 1;
  pData->len_window_group[0] = 1;
  pData->predictor_reset = 0;
  pData->ltp_data_present = 0;

  if (pData->window_sequence == EIGHT_SHORT_SEQUENCE) {
    pData->num_windows = 8;
    GET_BITS(pBS, pData->max_sfb, 4, Ipp32s)
    if (pData->num_swb_short < pData->max_sfb)
      return -1;
    for (i = 0; i < 7; i++) {
      GET_BITS(pBS, pData->scale_factor_grouping[i], 1, Ipp32s)

      if (pData->scale_factor_grouping[i] == 0) {
        pData->len_window_group[pData->num_window_groups] = 1;
        pData->num_window_groups++;
      } else {
        pData->len_window_group[pData->num_window_groups - 1]++;

      }
    }
  } else {
    pData->num_windows = 1;
    GET_BITS(pBS, pData->max_sfb, 6, Ipp32s)
    if (pData->num_swb_long < pData->max_sfb)
      return -1;
    if (audioObjectType != AOT_ER_BSAC) {
      GET_BITS(pBS, pData->predictor_data_present, 1, Ipp32s)
      if (pData->predictor_data_present) {
        if (audioObjectType == AOT_AAC_MAIN) {
          GET_BITS(pBS, pData->predictor_reset, 1, Ipp32s)
          if (pData->predictor_reset) {
            GET_BITS(pBS, pData->predictor_reset_group_number, 5, Ipp32s)
          }
          pred_min_sfb =
            pData->max_sfb <
            pData->pred_max_sfb ? pData->max_sfb : pData->pred_max_sfb;
          for (i = 0; i < pred_min_sfb; i++) {
            GET_BITS(pBS, pData->prediction_used[i], 1, Ipp8u)
          }
          for (i = pred_min_sfb; i < pData->pred_max_sfb; i++) {
            pData->prediction_used[i] = 0;
          }
        } else if (audioObjectType == AOT_AAC_LTP) {
          GET_BITS(pBS, pData->ltp_data_present, 1, Ipp32s)
          if (pData->ltp_data_present) {
//                    dbg_trace("ltp_data_present\n");
            dec_ltp_data(pData, pBS, audioObjectType);
          }
        } else {
          return -1;
        }
      } else {
        if (audioObjectType == AOT_AAC_MAIN) {
          for (i = 0; i < pData->pred_max_sfb; i++) {
            pData->prediction_used[i] = 0;
          }
        }
      }
    }
  }

  return 0;
}

/********************************************************************/

Ipp32s ics_info_copy(s_SE_Individual_channel_stream *pDataSrc,
                     s_SE_Individual_channel_stream *pDataDst,
                     enum AudioObjectType audioObjectType)
{
  Ipp32s  i;

  pDataDst->ics_reserved_bit = pDataSrc->ics_reserved_bit;
  pDataDst->window_sequence = pDataSrc->window_sequence;
  pDataDst->window_shape = pDataSrc->window_shape;

  pDataDst->num_window_groups = 1;
  pDataDst->len_window_group[0] = 1;

  if (pDataSrc->window_sequence == EIGHT_SHORT_SEQUENCE) {
    pDataDst->num_windows = 8;
    pDataDst->max_sfb = pDataSrc->max_sfb;

    for (i = 0; i < 7; i++) {
      pDataDst->scale_factor_grouping[i] = pDataSrc->scale_factor_grouping[i];

      if (pDataDst->scale_factor_grouping[i] == 0) {
        pDataDst->len_window_group[pDataDst->num_window_groups] = 1;
        pDataDst->num_window_groups++;
      } else {
        pDataDst->len_window_group[pDataDst->num_window_groups - 1]++;
      }
    }

  } else {
    pDataDst->num_windows = 1;
    pDataDst->max_sfb = pDataSrc->max_sfb;
    pDataDst->predictor_data_present = pDataSrc->predictor_data_present;

    if (audioObjectType == AOT_AAC_MAIN) {
      pDataDst->predictor_reset = pDataSrc->predictor_reset;
      pDataDst->predictor_reset_group_number =
        pDataSrc->predictor_reset_group_number;
      for (i = 0; i < pDataDst->pred_max_sfb; i++) {
        pDataDst->prediction_used[i] = pDataSrc->prediction_used[i];
      }
    }
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_ltp_data(s_SE_Individual_channel_stream *pData,
                    sBitsreamBuffer *pBS,
                    enum AudioObjectType audioObjectType)
{
  Ipp32s  i;
  Ipp32s  w;
  Ipp32s  pred_max_sfb;

  pred_max_sfb =
    MAX_LTP_SFB_LONG < pData->max_sfb ? MAX_LTP_SFB_LONG : pData->max_sfb;
  if (audioObjectType == AOT_ER_AAC_LD) {
    GET_BITS(pBS, pData->ltp_lag_update, 1, Ipp32s)
    if (pData->ltp_lag_update) {
      GET_BITS(pBS, pData->ltp_lag, 10, Ipp32s)
    } else {
    }
    GET_BITS(pBS, pData->ltp_coef, 3, Ipp32s)
    for (i = 0; i < pred_max_sfb; i++) {
      GET_BITS(pBS, pData->ltp_long_used[i], 1, Ipp32s)
    }
  } else {
    GET_BITS(pBS, pData->ltp_lag, 11, Ipp32s)
    GET_BITS(pBS, pData->ltp_coef, 3, Ipp32s)

    if (pData->window_sequence == EIGHT_SHORT_SEQUENCE) {
      for (w = 0; w < pData->num_windows; w++) {
        GET_BITS(pBS, pData->ltp_short_used[w], 1, Ipp32s)
        if (pData->ltp_short_used[w]) {
          GET_BITS(pBS, pData->ltp_short_lag_present[w], 1, Ipp32s)
          if (pData->ltp_short_lag_present[w]) {
            GET_BITS(pBS, pData->ltp_short_lag[w], 4, Ipp32s)
          } else {
            pData->ltp_short_lag[w] = 0;
          }
        }
      }

    } else {
        if(pData->max_sfb > MAX_SFB)
            pData->max_sfb = MAX_SFB;

      for (i = 0; i < pred_max_sfb; i++) {
        GET_BITS(pBS, pData->ltp_long_used[i], 1, Ipp32s)
      }

      for (i = pred_max_sfb; i < pData->max_sfb; i++) {
        pData->ltp_long_used[i] = 0;
      }
    }
  }
  return 0;
}

/********************************************************************/

Ipp32s dec_section_data(s_SE_Individual_channel_stream *pData,
                        sBitsreamBuffer *pBS)
{
  Ipp32s  sfb;
  Ipp32s  k;
  Ipp32s  g;
  Ipp32s  sect_esc_val;
  Ipp32s  sect_len_incr;
  Ipp32s  esc_code_len;
  Ipp32s  sect_cb;
  Ipp32s  sect_len;

  if (pData->window_sequence == EIGHT_SHORT_SEQUENCE) {
    sect_esc_val = (1 << 3) - 1;
    esc_code_len = 3;
  } else {
    sect_esc_val = (1 << 5) - 1;
    esc_code_len = 5;
  }
  for (g = 0; g < pData->num_window_groups; g++) {
    Ipp32s num_sec = 0;
    k = 0;
    while (k < pData->max_sfb) {
      GET_BITS(pBS, sect_cb, 4, Ipp32s)
      num_sec++;
      if (num_sec > pData->max_sfb) {
        return -1;
      }
      sect_len = 0;
      GET_BITS(pBS, sect_len_incr, esc_code_len, Ipp32s)
      while (sect_len_incr == sect_esc_val) {
        sect_len += sect_esc_val;
        GET_BITS(pBS, sect_len_incr, esc_code_len, Ipp32s)
      }
      sect_len += sect_len_incr;

      if (k + sect_len > pData->max_sfb) {
        return -1;
      }

      for (sfb = k; sfb < k + sect_len; sfb++) {
        pData->sfb_cb[g][sfb] = sect_cb;
      }
      k += sect_len;
    }
    for (; k < 51; k++) {
      pData->sfb_cb[g][k] = 0;
    }
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_scale_factor_data(s_SE_Individual_channel_stream *pData,
                             Ipp16s scalef[8][51],
                             Ipp32s scale_factor,
                             Ipp32s noise_nrg,
                             sBitsreamBuffer *pBS)
{
  Ipp32s  g;
  Ipp32s  sfb;
  Ipp16s  t;
  Ipp32s  is_pos;
  Ipp32s  noise_pcm_flag;
  Ipp8u  *pSrc;
  Ipp32s  bitoffset;
  IppsVLCDecodeSpec_32s *pVLCDecSpec =
    (IppsVLCDecodeSpec_32s *) pData->p_huffman_tables[0];

  is_pos = 0;
  noise_pcm_flag = 1;

  pSrc = (Ipp8u *)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
  bitoffset = (32 - pBS->nBit_offset) & 0x7;

  for (g = 0; g < pData->num_window_groups; g++) {
    for (sfb = 0; sfb < pData->max_sfb; sfb++) {
      switch (pData->sfb_cb[g][sfb]) {
      case ZERO_HCB:
        scalef[g][sfb] = 0;
        break;
      case INTENSITY_HCB:
      case INTENSITY_HCB2:
        ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, &t, pVLCDecSpec);
        is_pos += t - SF_MID;
        scalef[g][sfb] = (Ipp16s)is_pos;
        break;
      case NOISE_HCB:
        if (noise_pcm_flag) {
          pBS->pCurrent_dword = (Ipp32u *)(pSrc - ((size_t)pSrc & 3));
          pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
          pBS->nBit_offset =
            (Ipp32s)(32 - ((pSrc - (Ipp8u *)pBS->pCurrent_dword) << 3) - bitoffset);
          noise_pcm_flag = 0;
          GET_BITS(pBS, t, 9, Ipp16s)
          pSrc = (Ipp8u *)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
          bitoffset = (32 - pBS->nBit_offset) & 0x7;
          t -= 256;
        } else {
          ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, &t, pVLCDecSpec);
          t -= SF_MID;
        }
        noise_nrg += t;
        scalef[g][sfb] = (Ipp16s)noise_nrg;
        break;
      default:
        ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, &t, pVLCDecSpec);
        scale_factor += t - SF_MID;
        scalef[g][sfb] = (Ipp16s)scale_factor;
        break;
      }
    }
  }
  pBS->pCurrent_dword = (Ipp32u *)(pSrc - ((size_t)(pSrc) & 3));
  pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
  pBS->nBit_offset =
    (Ipp32s)(32 - ((pSrc - (Ipp8u *)pBS->pCurrent_dword) << 3) - bitoffset);
  return 0;
}

/********************************************************************/

Ipp32s dec_spectral_data(s_SE_Individual_channel_stream *pData,
                         sBitsreamBuffer *pBS)
{
  IPP_ALIGNED_ARRAY(32, Ipp16s, pDst, 512);
  Ipp32s  g;
  Ipp32s  sfb;
  Ipp32s *sfb_offset;
  Ipp16s *qp;
  Ipp8u  *pSrc;
  Ipp32s  bitoffset;
  Ipp32s i, num;

  if (pData->window_sequence == EIGHT_SHORT_SEQUENCE) {
    sfb_offset = pData->sfb_offset_short_window;
  } else {
    sfb_offset = pData->sfb_offset_long_window;
  }

  pSrc = (Ipp8u *)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
  bitoffset = (32 - pBS->nBit_offset) & 0x7;

  qp = pData->spectrum_data;

  for (g = 0; g < pData->num_window_groups; g++) {
    for (sfb = 0; sfb < pData->max_sfb; sfb++) {
      Ipp32s  sfb_cb = pData->sfb_cb[g][sfb];
      Ipp32s  sfb_begin = sfb_offset[sfb];
      Ipp32s  sfb_end = sfb_offset[sfb + 1];
      Ipp32s  shift = pData->vlcShifts[sfb_cb];
      Ipp32s  offset = pData->vlcOffsets[sfb_cb];
      Ipp32s  mask = (1 << (shift)) - 1;
      IppsVLCDecodeSpec_32s *pVLCDecSpec =
        (IppsVLCDecodeSpec_32s *) pData->p_huffman_tables[sfb_cb];
      IppsVLCDecodeUTupleSpec_32s *pVLCDecSpecTuple =
        (IppsVLCDecodeUTupleSpec_32s *) pData->p_huffman_tables[sfb_cb];

      switch (pData->vlcTypes[sfb_cb]) {
      case 0:  /* 4 tuples */

        num = ((sfb_end - sfb_begin) >> 2) * pData->len_window_group[g];

        ippsVLCDecodeBlock_1u16s(&pSrc, &bitoffset, pDst,
                                 num, pVLCDecSpec);

        for (i = 0; i < num; i++) {
          Ipp32s tmp = pDst[i];

          qp[0] = (Ipp16s)(tmp >> (3 * shift));
          qp[1] = (Ipp16s)(((tmp >> (2 * shift)) & mask) - offset);
          qp[2] = (Ipp16s)(((tmp >> (shift)) & mask) - offset);
          qp[3] = (Ipp16s)((tmp & mask) - offset);

          qp += 4;
        }

        break;
      case 1:  /* 2 tuples */
        num = ((sfb_end - sfb_begin) >> 1) * pData->len_window_group[g];

        ippsVLCDecodeBlock_1u16s(&pSrc, &bitoffset, pDst,
                                 num, pVLCDecSpec);

        for (i = 0; i < num; i++) {
          Ipp32s tmp = pDst[i];

          qp[0] = (Ipp16s)(tmp >> shift);
          qp[1] = (Ipp16s)((tmp & mask) - offset);

          qp += 2;
        }
        break;
      case 2:  /* esc */
        num = ((sfb_end - sfb_begin)) * pData->len_window_group[g];

        ippsVLCDecodeEscBlock_AAC_1u16s(&pSrc, &bitoffset, qp,
                                        num, pVLCDecSpec);

        qp += num;
        break;
      case 3:  /* esc Ipp32u */
        num = ((sfb_end - sfb_begin)) * pData->len_window_group[g];

        ippsVLCDecodeUTupleEscBlock_AAC_1u16s(&pSrc, &bitoffset, qp,
          num, pVLCDecSpecTuple);

        qp += num;
        break;
      case 4:  /* 2, 4 tuples Ipp32u */
        num = ((sfb_end - sfb_begin)) * pData->len_window_group[g];

        ippsVLCDecodeUTupleBlock_1u16s(&pSrc, &bitoffset, qp,
                                       num, pVLCDecSpecTuple);

        qp += num;
        break;
      default:
        num = ((sfb_end - sfb_begin)) * pData->len_window_group[g];
        for (i = 0; i < num; i++) {
          qp[0] = 0;
          qp++;
        }
        break;
      }
    }
  }

  pBS->pCurrent_dword = (Ipp32u *)(pSrc - ((size_t)(pSrc) & 3));
  pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
  pBS->nBit_offset =
    (Ipp32s)(32 - ((pSrc - (Ipp8u *)pBS->pCurrent_dword) << 3) - bitoffset);
  return 0;
}

/********************************************************************/

Ipp32s dec_pulse_data(s_SE_Individual_channel_stream *pData,
                      sBitsreamBuffer *pBS)
{
  Ipp32s  i, k;

  GET_BITS(pBS, pData->number_pulse, 2, Ipp32s)
  GET_BITS(pBS, pData->pulse_start_sfb, 6, Ipp32s)
  if (pData->pulse_start_sfb > pData->num_swb_long)
    return -1;

  k = pData->sfb_offset_long_window[pData->pulse_start_sfb];
  for (i = 0; i < pData->number_pulse + 1; i++) {
    GET_BITS(pBS, pData->pulse_offset[i], 5, Ipp32s)
    GET_BITS(pBS, pData->pulse_amp[i], 4, Ipp32s)
    k += pData->pulse_offset[i];
  }
  if (k > 1024)
    return -1;

  return 0;
}

/********************************************************************/

Ipp32s dec_tns_data(s_SE_Individual_channel_stream *pData,
                    sBitsreamBuffer *pBS)
{
  Ipp32s  w;
  Ipp32s  filt;
  Ipp32s  i;
  Ipp32s  coef_len;

  for (w = 0; w < pData->num_windows; w++) {
    Ipp32s nbits;

    nbits = (pData->num_windows == 8) ? 1 : 2;
    GET_BITS(pBS, pData->n_filt[w], nbits, Ipp32s)
    if (pData->n_filt[w] == 0)
      continue;
    GET_BITS(pBS, pData->coef_res[w], 1, Ipp32s)

    for (filt = 0; filt < pData->n_filt[w]; filt++) {
      nbits = (pData->num_windows == 8) ? 4 : 6;
      GET_BITS(pBS, pData->length[w][filt], nbits, Ipp32s)

      nbits = (pData->num_windows == 8) ? 3 : 5;
      GET_BITS(pBS, pData->order[w][filt], nbits, Ipp32s)

      if (pData->order[w][filt] == 0)
        continue;

      GET_BITS(pBS, pData->direction[w][filt], 1, Ipp32s)
      GET_BITS(pBS, pData->coef_compress[w][filt], 1, Ipp32s)
      coef_len = 3 + pData->coef_res[w] - pData->coef_compress[w][filt];

      for (i = 0; i < pData->order[w][filt]; i++) {
        GET_BITS(pBS, pData->coef[w][filt][i], coef_len, Ipp32s)
      }
    }
  }
  return 0;
}

/********************************************************************/

Ipp32s dec_coupling_channel_element(sCoupling_channel_element *pElement,
                                    sCoupling_channel_data *pData,
                                    sBitsreamBuffer *pBS,
                                    enum AudioObjectType audioObjectType)
{
  Ipp32s  c, cc_l, cc_r;
  Ipp16s  t;
  Ipp8u  *pSrc;
  Ipp32s  bitoffset;
  IppsVLCDecodeSpec_32s *pVLCDecSpec =
    (IppsVLCDecodeSpec_32s *) pElement->stream.p_huffman_tables[0];
  Ipp32u  *crc_ptr = 0;
  Ipp32s  crc_offset = 0;
  Ipp32s  decodedBits0 = 0, decodedBits2 = 0;

  if (pElement->crc_enable) {
    crc_ptr = pBS->pCurrent_dword;
    crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits0)
  }

  GET_BITS(pBS, pElement->element_instance_tag, 4, Ipp32s)
  GET_BITS(pBS, pData->ind_sw_cce_flag, 1, Ipp32s)
  GET_BITS(pBS, pData->num_coupled_elements, 3, Ipp32s)

  pData->num_gain_element_lists = 0;
  for (c = 0; c < pData->num_coupled_elements + 1; c++) {
    pData->num_gain_element_lists++;
    GET_BITS(pBS, pData->cc_target_id[c], 1, Ipp32s)
    GET_BITS(pBS, pData->cc_target_tag[c], 4, Ipp32s)
    if (pData->cc_target_id[c]) {
      GET_BITS(pBS, cc_l, 1, Ipp32s)
      GET_BITS(pBS, cc_r, 1, Ipp32s)
      pData->cc_lr[pData->num_gain_element_lists - 1] = (cc_l << 1) + cc_r;
      if (pData->cc_lr[pData->num_gain_element_lists - 1] == 3) {
        pData->num_gain_element_lists++;
        pData->cc_lr[pData->num_gain_element_lists - 1] = 3;
      }
    }
  }

  GET_BITS(pBS, pData->cc_domain, 1, Ipp32s)
  GET_BITS(pBS, pData->gain_element_sign, 1, Ipp32s)
  GET_BITS(pBS, pData->gain_element_scale, 2, Ipp32s)

  if (dec_individual_channel_stream
      (&pElement->stream, pBS, 0, 0, audioObjectType) < 0)
    return -1;

  pData->max_sfb = pElement->stream.max_sfb;
  pData->num_window_groups = pElement->stream.num_window_groups;
  for (c = 0; c < pData->num_window_groups; c++) {
    pData->len_window_group[c] = pElement->stream.len_window_group[c];
  }

  if (pElement->stream.window_sequence != EIGHT_SHORT_SEQUENCE) {
    pData->sfb_offset = pElement->stream.sfb_offset_long_window;
  } else {
    pData->sfb_offset = pElement->stream.sfb_offset_short_window;
  }

  pData->cge[0] = 1;
  pElement->cc_fact[0][0][0] = 0;

  for (c = 1; c < pData->num_gain_element_lists; c++) {
    if (pData->ind_sw_cce_flag) {
      pData->cge[c] = 1;
    } else {
      GET_BITS(pBS, pData->cge[c], 1, Ipp32s)
    }

    if (pData->cge[c]) {
      pSrc = (Ipp8u *)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
      bitoffset = (32 - pBS->nBit_offset) & 0x7;
      ippsVLCDecodeOne_1u16s(&pSrc, &bitoffset, &t, pVLCDecSpec);
      pElement->cc_fact[c][0][0] = t - SF_MID;
      pBS->pCurrent_dword = (Ipp32u *)(pSrc - ((size_t)(pSrc) & 3));
      pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
      pBS->nBit_offset =
        (Ipp32s)(32 - ((pSrc - (Ipp8u *)pBS->pCurrent_dword) << 3) - bitoffset);
    } else {
      dec_scale_factor_data(&pElement->stream, pElement->cc_fact[c], 0,
                            pElement->stream.global_gain - 90, pBS);
    }
  }

  if (pElement->crc_enable) {
    Ipp32s len;
    Ipp32u crc = (Ipp32u)pElement->crc;

    GET_BITS_COUNT(pBS, decodedBits2)

    len = decodedBits2 - decodedBits0;
    if (len > 192) len = 192;
    bs_CRC_update(crc_ptr, crc_offset, len, &crc);

    if (len < 192)
      bs_CRC_update_zero(192 - len, &crc);

    pElement->crc = (Ipp32s)crc;
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_data_stream_element(sData_stream_element *pData,
                               sBitsreamBuffer *pBS)
{
  Ipp32s  data_byte_align_flag;
  Ipp32s  count;
  Ipp32s  cnt;
  Ipp32s  i;

  GET_BITS(pBS, pData->element_instance_tag, 4, Ipp32s)
  GET_BITS(pBS, data_byte_align_flag, 1, Ipp32s)
  GET_BITS(pBS, count, 8, Ipp32s)
  cnt = count;
  if (cnt == 255) {
    GET_BITS(pBS, count, 8, Ipp32s)
    cnt += count;
  }
  if (data_byte_align_flag) {
    Byte_alignment(pBS);
  }
  for (i = 0; i < cnt; i++) {
    GET_BITS(pBS, pData->data_stream_byte[i], 8, Ipp8s)
  }
  return 0;
}

/********************************************************************/

static
Ipp32s dec_extension_payload(sSBRDecComState * pSBRItem,
                             Ipp32s *pNumFillSbr,
                             sDynamic_range_info *pInfo,
                             sBitsreamBuffer *pBS,
                             Ipp32s cnt,
                             eSBR_SUPPORT flag_SBR_support_lev)
{
  Ipp32s  extension_type = - 123;
  Ipp32s  ret_value;
  Ipp32s  fill_nibble;
  Ipp32s  fill_byte;
  Ipp32s  i;
  Ipp32s  other_bits;
  //Ipp32s  flag_SBR_enable = 1;

  ret_value = 0;

  if( SBR_ENABLE == flag_SBR_support_lev ){
    GET_BITS(pBS, extension_type, 4, Ipp32s)
    pSBRItem->cnt_bit -= 4;
  }

  switch (extension_type) {
  case EXT_DYNAMIC_RANGE:
    ret_value = dec_dynamic_range_info(pInfo, pBS);
    break;

  case EXT_SBR_DATA:
    *pNumFillSbr += 1;
    ret_value = sbr_extension_data(pSBRItem->id_aac, 0, pSBRItem, pBS, cnt);

    /* diagnostic */
    if (SBR_ERR_REQUIREMENTS == ret_value) {
      pSBRItem->sbrFlagError = SBR_ERR_REQUIREMENTS;
    }
    break;

  case EXT_SBR_DATA_CRC:
    *pNumFillSbr += 1;
    ret_value = sbr_extension_data(pSBRItem->id_aac, 1, pSBRItem, pBS, cnt);
    /* diagnostic */
    if (SBR_ERR_REQUIREMENTS == ret_value) {
      pSBRItem->sbrFlagError = SBR_ERR_REQUIREMENTS;
    }
    break;

  case EXT_FILL_DATA:
    GET_BITS(pBS, fill_nibble, 4, Ipp32s)
    for (i = 0; i < cnt - 1; i++) {
      GET_BITS(pBS, fill_byte, 8, Ipp32s)
    }
    ret_value = cnt;
    break;
  default:

    if( SBR_ENABLE == flag_SBR_support_lev ){
      for (i = 0; i < 8 * (cnt - 1) + 4; i++) {
        GET_BITS(pBS, other_bits, 1, Ipp32s)
      }
    } else {
      for (i = 0; i < (cnt ); i++) {
        GET_BITS(pBS, other_bits, 8, Ipp32s)
      }
    }

    ret_value = cnt;
    break;
  }

  /******************************************************
   * ret_value - positive, number of bit has been read
   * but...
   * if there is error,
   * then ret_value = SBR_ERR_REQUIREMENTS
   ******************************************************/
  return ret_value;
}

/********************************************************************/

Ipp32s dec_fill_element(sSBRDecComState *pSBRItem,
                        //sPSDecComState*     pPSItem,
                        Ipp32s *cnt_fill_sbr_element,
                        sDynamic_range_info *pInfo,
                        sBitsreamBuffer * pBS,
                        eSBR_SUPPORT flag_SBR_support_lev)
{
  Ipp32s  cnt;
  Ipp32s  ret;

  GET_BITS(pBS, cnt, 4, Ipp32s)

  if (cnt == 15) {
    Ipp32s tmp;

    GET_BITS(pBS, tmp, 8, Ipp32s)
    cnt += tmp - 1;
  }
  pSBRItem->cnt_bit = cnt * 8;

  /******************************************************
   * ret - positive value, number of bit has been read
   * but...
   * if there is error,
   * then ret = SBR_ERR_REQUIREMENTS
   ******************************************************/
  while (cnt > 0) {
    ret = dec_extension_payload(pSBRItem,
                                //pPSItem,
                                cnt_fill_sbr_element,
                                pInfo,
                                pBS,
                                cnt,
                                flag_SBR_support_lev);

    if ( ret < 0 )
      return SBR_ERR_REQUIREMENTS;
    else
      cnt -= ret;
  }

  return 0;
}

/********************************************************************/

Ipp32s dec_excluded_channels(sExcluded_channels *pData,
                             sBitsreamBuffer *pBS)
{
  Ipp32s  n, i;

  n = 0;

  pData->num_excl_chan = 7;
  for (i = 0; i < 7; i++) {
    GET_BITS(pBS, pData->exclude_mask[i], 1, Ipp8u)
    n++;
  }

  GET_BITS(pBS, pData->additional_excluded_chns[n - 1], 1, Ipp8u)
  while (pData->additional_excluded_chns[n - 1] == 1) {
    for (i = pData->num_excl_chan; i < pData->num_excl_chan + 7; i++) {
      GET_BITS(pBS, pData->exclude_mask[i], 1, Ipp8u)
    }
    n++;
    /* diagnostic */
    if (n > 10) return -1;
    pData->num_excl_chan += 7;
    GET_BITS(pBS, pData->additional_excluded_chns[n - 1], 1, Ipp8u)
  }
  return n;
}

/********************************************************************/

Ipp32s dec_dynamic_range_info(sDynamic_range_info *pInfo,
                              sBitsreamBuffer *pBS)
{
  Ipp32s  n, i;

  n = 1;

  pInfo->drc_num_bands = 1;
  GET_BITS(pBS, pInfo->pce_tag_present, 1, Ipp32s)

  if (pInfo->pce_tag_present == 1) {
    GET_BITS(pBS, pInfo->pce_innstance_tag, 4, Ipp32s)
    GET_BITS(pBS, pInfo->drc_tag_reserved_bits, 4, Ipp32s)
    n++;
  }

  GET_BITS(pBS, pInfo->excluded_chns_present, 1, Ipp32s)

  if (pInfo->excluded_chns_present == 1) {
    Ipp32s tmp_res = dec_excluded_channels(&pInfo->ec_data, pBS);

    if (tmp_res < 0) {
      return -1;
    }
    n += tmp_res;
  }

  GET_BITS(pBS, pInfo->drc_bands_present, 1, Ipp32s)
  if (pInfo->drc_bands_present == 1) {
    GET_BITS(pBS, pInfo->drc_band_incr, 4, Ipp32s)
    GET_BITS(pBS, pInfo->drc_bands_reserved_bits, 4, Ipp32s)
    n++;

    pInfo->drc_num_bands += pInfo->drc_band_incr;

    for (i = 0; i < pInfo->drc_num_bands; i++) {
      GET_BITS(pBS, pInfo->drc_band_top[i], 8, Ipp32s)
      n++;
    }
  }

  GET_BITS(pBS, pInfo->prog_ref_level_present, 1, Ipp32s)

  if (pInfo->prog_ref_level_present == 1) {
    GET_BITS(pBS, pInfo->prog_ref_level, 7, Ipp32s)
    GET_BITS(pBS, pInfo->prog_ref_level_reserved_bits, 1, Ipp32s)
    n++;
  }

  for (i = 0; i < pInfo->drc_num_bands; i++) {
    GET_BITS(pBS, pInfo->dyn_rng_sgn[i], 1, Ipp32s)
    GET_BITS(pBS, pInfo->dyn_rng_ctl[i], 7, Ipp32s)
    n++;
  }

  return n;
}

/********************************************************************/

Ipp32s dec_gain_control_data(s_SE_Individual_channel_stream *pData,
                             sBitsreamBuffer *pBS)
{
  Ipp32s  bd, ad, wd;

  GET_BITS(pBS, pData->max_band, 2, Ipp32s)

  if (pData->window_sequence == ONLY_LONG_SEQUENCE) {
    for (bd = 1; bd <= pData->max_band; bd++) {
      for (wd = 0; wd < 1; wd++) {
        GET_BITS(pBS, pData->SSRInfo[bd][wd].adjust_num, 3, Ipp32s)
        for (ad = 0; ad < pData->SSRInfo[bd][wd].adjust_num; ad++) {
          GET_BITS(pBS, pData->SSRInfo[bd][wd].alevcode[ad], 4, Ipp32s)
          GET_BITS(pBS, pData->SSRInfo[bd][wd].aloccode[ad], 5, Ipp32s)
        }
      }
    }
  } else if (pData->window_sequence == LONG_START_SEQUENCE) {
    for (bd = 1; bd <= pData->max_band; bd++) {
      for (wd = 0; wd < 2; wd++) {
        GET_BITS(pBS, pData->SSRInfo[bd][wd].adjust_num, 3, Ipp32s)
        for (ad = 0; ad < pData->SSRInfo[bd][wd].adjust_num; ad++) {
          GET_BITS(pBS, pData->SSRInfo[bd][wd].alevcode[ad], 4, Ipp32s)
          if (wd == 0) {
            GET_BITS(pBS, pData->SSRInfo[bd][wd].aloccode[ad], 4, Ipp32s)
          } else {
            GET_BITS(pBS, pData->SSRInfo[bd][wd].aloccode[ad], 2, Ipp32s)
          }
        }
      }
    }
  } else if (pData->window_sequence == EIGHT_SHORT_SEQUENCE) {
    for (bd = 1; bd <= pData->max_band; bd++) {
      for (wd = 0; wd < 8; wd++) {
        GET_BITS(pBS, pData->SSRInfo[bd][wd].adjust_num, 3, Ipp32s)
        for (ad = 0; ad < pData->SSRInfo[bd][wd].adjust_num; ad++) {
          GET_BITS(pBS, pData->SSRInfo[bd][wd].alevcode[ad], 4, Ipp32s)
          GET_BITS(pBS, pData->SSRInfo[bd][wd].aloccode[ad], 2, Ipp32s)
        }
      }
    }
  } else if (pData->window_sequence == LONG_STOP_SEQUENCE) {
    for (bd = 1; bd <= pData->max_band; bd++) {
      for (wd = 0; wd < 2; wd++) {
        GET_BITS(pBS, pData->SSRInfo[bd][wd].adjust_num, 3, Ipp32s)
        for (ad = 0; ad < pData->SSRInfo[bd][wd].adjust_num; ad++) {
          GET_BITS(pBS, pData->SSRInfo[bd][wd].alevcode[ad], 4, Ipp32s)
          if (wd == 0) {
            GET_BITS(pBS, pData->SSRInfo[bd][wd].aloccode[ad], 4, Ipp32s)
          } else {
            GET_BITS(pBS, pData->SSRInfo[bd][wd].aloccode[ad], 5, Ipp32s)
          }
        }
      }
    }
  }

  return 0;
}

/********************************************************************/

void save_gain_control_data(Ipp32s ch,
                            s_SE_Individual_channel_stream *pData,
                            AACDec_com *state_com)
{
  Ipp32s  bd, ad, wd, max_wd = 0;

  if (pData->gain_control_data_present) {
    if (pData->window_sequence == ONLY_LONG_SEQUENCE) {
      max_wd = 1;
    } else if (pData->window_sequence == LONG_START_SEQUENCE) {
      max_wd = 2;
    } else if (pData->window_sequence == EIGHT_SHORT_SEQUENCE) {
      max_wd = 8;
    } else if (pData->window_sequence == LONG_STOP_SEQUENCE) {
      max_wd = 2;
    }

    for (bd = 1; bd <= pData->max_band; bd++) {
      for (wd = 0; wd < max_wd; wd++) {
        state_com->SSRInfo[ch][bd][wd].adjust_num = pData->SSRInfo[bd][wd].adjust_num;
        for (ad = 0; ad < pData->SSRInfo[bd][wd].adjust_num; ad++) {
          state_com->SSRInfo[ch][bd][wd].alevcode[ad] = pData->SSRInfo[bd][wd].alevcode[ad];
          state_com->SSRInfo[ch][bd][wd].aloccode[ad] = pData->SSRInfo[bd][wd].aloccode[ad];
        }
      }
    }
  }
}

/********************************************************************/

Ipp32s ics_apply_pulse_I(s_SE_Individual_channel_stream *p_data)
{
  Ipp32s  i, k;

  k = p_data->sfb_offset_long_window[p_data->pulse_start_sfb];

  for (i = 0; i <= p_data->number_pulse; i++) {
    k += p_data->pulse_offset[i];
    if (p_data->spectrum_data[k] > 0) {
      p_data->spectrum_data[k] = p_data->spectrum_data[k] + (Ipp16s)p_data->pulse_amp[i];
    } else {
      p_data->spectrum_data[k] = p_data->spectrum_data[k] - (Ipp16s)p_data->pulse_amp[i];
    }

  }
  return 0;
}

#endif //UMC_ENABLE_XXX
