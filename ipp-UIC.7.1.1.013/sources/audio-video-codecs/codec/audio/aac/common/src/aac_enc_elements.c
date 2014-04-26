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
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <stdio.h>

#include "align.h"
#include "aac_enc_huff_tables.h"
#include "bstream.h"
#include "aaccmn_adts.h"
#include "aaccmn_adif.h"
#include "aac_enc_own.h"

#include "ippdc.h"

Ipp32u sf_huff_codebook[];

Ipp32s enc_ltp_data(sEnc_individual_channel_stream* pStream,
                    sBitsreamBuffer* pBS,
                    Ipp32s writing);

/********************************************************************/

//void tns_data(sEnc_individual_channel_stream* pStream,
//              sBitsreamBuffer* pBS);

/****************************************************************************/

Ipp32s fill_program_config_element(sProgram_config_element* p_data,
                                   Ipp32s element_instance_tag,
                                   AACEnc_com *state_com)
{
  Ipp32s chNum = state_com->m_channel_number;
  sOneChannelInfo* chInfo = state_com->chInfo;
#if !defined(ANDROID)
  Ipp32s num_channel_elements[3];
  Ipp32s *element_tag_select[3];
  Ipp32s *element_is_cpe[3];
#else
  static Ipp32s num_channel_elements[3];
  static Ipp32s *element_tag_select[3];
  static Ipp32s *element_is_cpe[3];
#endif
  Ipp32s i;

  switch (state_com->audioObjectType) {
  case AOT_AAC_MAIN:
    p_data->object_type = 0;
    break;
  case AOT_AAC_LC:
    p_data->object_type = 1;
    break;
  case AOT_AAC_SSR:
    p_data->object_type = 2;
    break;
  case AOT_AAC_LTP:
    p_data->object_type = 3;
    break;
  }

  p_data->element_instance_tag = element_instance_tag;
  p_data->sampling_frequency_index = state_com->sampling_frequency_index;

  p_data->num_assoc_data_elements = 0;
  p_data->num_valid_cc_elements = 0;
  p_data->mono_mixdown_present = 0;
  p_data->stereo_mixdown_present = 0;
  p_data->matrix_mixdown_idx_present = 0;
  p_data->comment_field_bytes = 0;

  p_data->num_lfe_channel_elements = 0;

  element_tag_select[0] = p_data->front_element_tag_select;
  element_tag_select[1] = p_data->side_element_tag_select;
  element_tag_select[2] = p_data->back_element_tag_select;

  element_is_cpe[0] = p_data->front_element_is_cpe;
  element_is_cpe[1] = p_data->side_element_is_cpe;
  element_is_cpe[2] = p_data->back_element_is_cpe;

  num_channel_elements[0] = 0;
  num_channel_elements[1] = 0;
  num_channel_elements[2] = 0;

  i = 0;

  while (chNum > 0) {
    chNum--;
    if (chInfo[0].element_id == ID_LFE) {
      if (p_data->num_lfe_channel_elements == MAX_CHANNELS_ELEMENTS) {
        return -1;
      }
      p_data->lfe_element_tag_select[p_data->num_lfe_channel_elements] =
        chInfo[0].element_instance_tag;
      p_data->num_lfe_channel_elements++;
      chInfo++;
    } else {
      element_tag_select[i][num_channel_elements[i]] = chInfo[0].element_instance_tag;
      element_is_cpe[i][num_channel_elements[i]] = 0;
      if (chInfo[0].element_id == ID_CPE) {
        element_is_cpe[i][num_channel_elements[i]] = 1;
        chNum--;
        chInfo++;
      }
      chInfo++;
      num_channel_elements[i]++;
      if (num_channel_elements[i] == MAX_CHANNELS_ELEMENTS) {
        i++;
        if ((i > 2) && (chNum > 0)) {
          return -1;
        }
      }
    }
  }

  p_data->num_front_channel_elements = num_channel_elements[0];
  p_data->num_side_channel_elements = num_channel_elements[1];
  p_data->num_back_channel_elements = num_channel_elements[2];

  return 0;
}

/****************************************************************************/

void enc_program_config_element(sProgram_config_element* p_data,
                                sBitsreamBuffer* pBS)
{
  Ipp32s i;

  PUT_BITS(pBS,p_data->element_instance_tag,4);
  PUT_BITS(pBS,p_data->object_type,2);
  PUT_BITS(pBS,p_data->sampling_frequency_index,4);

  PUT_BITS(pBS,p_data->num_front_channel_elements,4);
  PUT_BITS(pBS,p_data->num_side_channel_elements,4);
  PUT_BITS(pBS,p_data->num_back_channel_elements,4);
  PUT_BITS(pBS,p_data->num_lfe_channel_elements,2);

  PUT_BITS(pBS,p_data->num_assoc_data_elements,3);
  PUT_BITS(pBS,p_data->num_valid_cc_elements,4);

  PUT_BITS(pBS,p_data->mono_mixdown_present,1);
  if (p_data->mono_mixdown_present == 1) {
    PUT_BITS(pBS,p_data->mono_miwdown_element_number,4);
  }

  PUT_BITS(pBS,p_data->stereo_mixdown_present,1);
  if (p_data->stereo_mixdown_present == 1) {
    PUT_BITS(pBS,p_data->stereo_miwdown_element_number,4);
  }

  PUT_BITS(pBS,p_data->matrix_mixdown_idx_present,1);
  if (p_data->matrix_mixdown_idx_present == 1) {
    PUT_BITS(pBS,p_data->matrix_mixdown_idx,2);
    PUT_BITS(pBS,p_data->pseudo_surround_enable,1);
  }

  for (i = 0; i < p_data->num_front_channel_elements; i++) {
    PUT_BITS(pBS,p_data->front_element_is_cpe[i],1);
    PUT_BITS(pBS,p_data->front_element_tag_select[i],4);
  }

  for (i = 0; i < p_data->num_side_channel_elements; i++) {
    PUT_BITS(pBS,p_data->side_element_is_cpe[i],1);
    PUT_BITS(pBS,p_data->side_element_tag_select[i],4);
  }

  for (i = 0; i < p_data->num_back_channel_elements; i++) {
    PUT_BITS(pBS,p_data->back_element_is_cpe[i],1);
    PUT_BITS(pBS,p_data->back_element_tag_select[i],4);
  }

  for (i = 0; i < p_data->num_lfe_channel_elements; i++) {
    PUT_BITS(pBS,p_data->lfe_element_tag_select[i],4);
  }

  for (i = 0; i < p_data->num_assoc_data_elements; i++) {
    PUT_BITS(pBS,p_data->assoc_data_element_tag_select[i],4);
  }

  for (i = 0; i < p_data->num_valid_cc_elements; i++) {
    PUT_BITS(pBS,p_data->cc_element_is_ind_sw[i],1);
    PUT_BITS(pBS,p_data->valid_cc_element_tag_select[i],4);
  }

  SAVE_BITSTREAM(pBS)
  Byte_alignment(pBS);

  PUT_BITS(pBS,p_data->comment_field_bytes,8);
  for (i = 0; i < p_data->comment_field_bytes; i++) {
    PUT_BITS(pBS,p_data->comment_field_data[i],8);
  }
  SAVE_BITSTREAM(pBS)
}

/****************************************************************************/

void enc_adif_header(sAdif_header* pAdifHeader,
                     sBitsreamBuffer* pBS)
{
  int i;

  Byte_alignment(pBS);
  PUT_BITS(pBS,pAdifHeader->adif_id,32);

  PUT_BITS(pBS,pAdifHeader->copyright_id_present,1);
  if (pAdifHeader->copyright_id_present) {
    for (i = 0; i < LEN_COPYRIGHT_ID; i++) {
      PUT_BITS(pBS,pAdifHeader->copyright_id[i],8);
    }
  }

  PUT_BITS(pBS,pAdifHeader->original_copy,1);
  PUT_BITS(pBS,pAdifHeader->home,1);
  PUT_BITS(pBS,pAdifHeader->bitstream_type,1);
  PUT_BITS(pBS,pAdifHeader->bitrate,23);
  PUT_BITS(pBS,pAdifHeader->num_program_config_elements,4);

  if (pAdifHeader->bitstream_type == 0) {
    PUT_BITS(pBS,pAdifHeader->adif_buffer_fullness,20);
  }

  for (i = 0; i < pAdifHeader->num_program_config_elements + 1; i++) {
    enc_program_config_element(&pAdifHeader->pce[i],pBS);
  }
  SAVE_BITSTREAM(pBS)
}

/****************************************************************************/

void enc_adts_header(sAdts_fixed_header *pFixedHeader,
                     sAdts_variable_header *pVarHeader,
                     sBitsreamBuffer *pBS)
{
  Byte_alignment(pBS);
  PUT_BITS(pBS,0x0FFF,12);

  /// The following puts in bitsream ADTS fixed header
  PUT_BITS(pBS,pFixedHeader->ID,1);
  PUT_BITS(pBS,pFixedHeader->Layer,2);
  PUT_BITS(pBS,pFixedHeader->protection_absent,1);
  PUT_BITS(pBS,pFixedHeader->Profile,2);
  PUT_BITS(pBS,pFixedHeader->sampling_frequency_index,4);
  PUT_BITS(pBS,pFixedHeader->private_bit,1);
  PUT_BITS(pBS,pFixedHeader->channel_configuration,3);
  PUT_BITS(pBS,pFixedHeader->original_copy,1);
  PUT_BITS(pBS,pFixedHeader->Home,1);

  /// The following puts in bitsream ADTS variable header
  PUT_BITS(pBS,pVarHeader->copyright_identification_bit,1);
  PUT_BITS(pBS,pVarHeader->copyright_identification_start,1);
  PUT_BITS(pBS,pVarHeader->aac_frame_length,13);
  PUT_BITS(pBS,pVarHeader->adts_buffer_fullness,11);
  PUT_BITS(pBS,pVarHeader->no_raw_data_blocks_in_frame,2);
  SAVE_BITSTREAM(pBS)
}

/****************************************************************************/

Ipp32s enc_ics_info(sEnc_individual_channel_stream* pStream,
                    sBitsreamBuffer* pBS,
                    Ipp32s writing)
{
  Ipp32s i;
  Ipp32s count_bits;

  if (writing) {
    PUT_BITS(pBS,pStream->ics_reserved_bit,1);
    PUT_BITS(pBS,pStream->windows_sequence,2);
    PUT_BITS(pBS,pStream->window_shape,1);
  }

  count_bits = 4;

  if (pStream->windows_sequence == EIGHT_SHORT_SEQUENCE) {
    if (writing) {
      PUT_BITS(pBS,pStream->max_sfb,4);
      for (i = 0; i < 7; i ++) {
        PUT_BITS(pBS,pStream->scale_factor_grouping[i],1);
      }
    }
    count_bits += 11;
  } else {
    if (writing) {
      PUT_BITS(pBS,pStream->max_sfb,6);
      PUT_BITS(pBS, pStream->predictor_data_present, 1)
    }
    count_bits += 7;

    if (pStream->predictor_data_present) {
      if (pStream->audioObjectType == AOT_AAC_LTP) {
        if (writing) {
          PUT_BITS(pBS, pStream->ltp_data_present, 1)
        }
        count_bits++;
        if (pStream->ltp_data_present) {
          count_bits += enc_ltp_data(pStream, pBS, writing);
        }
      }
    }
  }

  return count_bits;
}

/****************************************************************************/

void enc_section_data(sEnc_individual_channel_stream* pStream,
                      sBitsreamBuffer* pBS)
{
  Ipp16s* sfb_cb;
  Ipp32s len_esc_value;
  Ipp32s sect_esc_value;
  Ipp32s sect_len;
  Ipp32s i;
  Ipp32s win;
  Ipp32s sect_counter;
  Ipp32s last_sect_counter;
  Ipp32s sect_cb;

  if (pStream->windows_sequence == EIGHT_SHORT_SEQUENCE) {
    len_esc_value = 3;
    sect_esc_value = (1<<3) - 1;
  } else {
    len_esc_value = 5;
    sect_esc_value = (1<<5) - 1;
  }

  sfb_cb = pStream->sfb_cb;

  sect_counter = -1;
  last_sect_counter = -1;

  for (win = 0; win < pStream->num_window_groups; win++) {

    sect_cb = sfb_cb[0];
    sect_counter++;
    pStream->sect_cb[sect_counter] = sect_cb;
    pStream->sect_len[sect_counter] = 0;

    for (i = 0; i < pStream->max_sfb; i++) {
      if (sfb_cb[i] == sect_cb) {
        pStream->sect_len[sect_counter]++;
      } else {
        sect_cb = sfb_cb[i];
        sect_counter++;
        pStream->sect_cb[sect_counter] = sect_cb;
        pStream->sect_len[sect_counter] = 1;
      }
    }
    sfb_cb += pStream->max_sfb;
    pStream->sect_num[win] = sect_counter - last_sect_counter;
    last_sect_counter = sect_counter;
  }

  sect_counter ++;

  for (i = 0; i < sect_counter; i++) {
    PUT_BITS(pBS,pStream->sect_cb[i],4);
    sect_len = pStream->sect_len[i];

    while (sect_len >= sect_esc_value) {
      PUT_BITS(pBS,sect_esc_value,len_esc_value);
      sect_len -= sect_esc_value;
    }
    PUT_BITS(pBS,sect_len,len_esc_value);
  }
}

/****************************************************************************/

Ipp32s enc_scale_factor_data(sEnc_individual_channel_stream* pStream,
                             sBitsreamBuffer* pBS,
                             Ipp32s writing)
{
  Ipp32s i;
  Ipp32s last_sf = 0;
  Ipp32s index;
  Ipp32s len;
  Ipp32u code;
  Ipp32s num_scale_factor;
  Ipp32s count_bits;

  num_scale_factor = pStream->num_window_groups * pStream->max_sfb;
  count_bits = 0;

  if (writing) {
    last_sf = pStream->global_gain;
  } else {
    for (i = 0; i < num_scale_factor; i++) {
      if (pStream->sfb_cb[i] != 0) {
        last_sf = pStream->scale_factors[i];
        break;
      }
    }
  }

  for (i = 0; i < num_scale_factor; i++) {
    switch(pStream->sfb_cb[i]) {
    case ZERO_HCB:
      break;
    case INTENSITY_HCB:
      break;
    case INTENSITY_HCB2:
      break;
    default:
      index = pStream->scale_factors[i] - last_sf + SF_MID;
      len  = sf_huff_codebook[2*index+0];
      if (writing) {
        code = sf_huff_codebook[2*index+1];
        PUT_BITS(pBS,code,len);
      } else {
        count_bits += len;
      }
      last_sf = pStream->scale_factors[i];
      break;
    }
  }
  return count_bits;
}

/****************************************************************************/

void enc_spectral_data(sEnc_individual_channel_stream* pStream,
                       sBitsreamBuffer* pBS)
{
  IppsVLCEncodeSpec_32s* pVLCEncSpec;
  Ipp8u  *pDst;
#if !defined(ANDROID)
  IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 512);
#else
  static IPP_ALIGNED_ARRAY(32, Ipp16s, tmp_src, 512);
#endif
  Ipp16s *q;
  Ipp32s* sfb_offset;
  Ipp32s i, j;
  Ipp32s win;
  Ipp32s sect_cb;
  Ipp32s sect_counter;
  Ipp32s sect_end;
  Ipp32s data_begin;
  Ipp32s data_end;
  Ipp32s shift;
  Ipp32s offset;
  Ipp32s bitoffset;

  sfb_offset = pStream->sfb_offset;

  pDst = (Ipp8u*)pBS->pCurrent_dword + ((32 - pBS->nBit_offset) >> 3);
  bitoffset = (32 - pBS->nBit_offset) & 0x7;

  SAVE_BITSTREAM(pBS)

  sect_counter = 0;

  for (win = 0; win < pStream->num_window_groups; win++) {

    data_begin = sfb_offset[0];
    sect_end = 0;

    for (i = 0; i < pStream->sect_num[win]; i++) {
      sect_end += pStream->sect_len[sect_counter];
      data_end  = sfb_offset[sect_end];
      sect_cb   = pStream->sect_cb[sect_counter];

      shift = vlcEncShifts[sect_cb];
      offset = vlcEncOffsets[sect_cb];

      pVLCEncSpec = (IppsVLCEncodeSpec_32s*)pStream->pHuffTables[sect_cb];

      switch (vlcEncTypes[sect_cb]) {
      case 0: /* 4 tuples */
        q = &pStream->x_quant[data_begin];
        for (j = 0; j < (data_end-data_begin) >> 2; j++) {
          tmp_src[j] = (Ipp16s)((q[0] << (3*shift)) + ((q[1] + offset) << (2*shift)) +
                        ((q[2] + offset) << shift) + (q[3] + offset));
          q += 4;
        }

        ippsVLCEncodeBlock_16s1u(tmp_src, (data_end-data_begin) >> 2, &pDst,
                                 &bitoffset, pVLCEncSpec);
        break;
      case 1: /* 2 tuples */
        q = &pStream->x_quant[data_begin];
        for (j = 0; j < (data_end-data_begin) >> 1; j++) {
          tmp_src[j] = (Ipp16s)((q[0] << shift) + (q[1] + offset));
          q += 2;
        }

        ippsVLCEncodeBlock_16s1u(tmp_src, (data_end-data_begin) >> 1, &pDst,
                                 &bitoffset, pVLCEncSpec);
        break;
      case 2: /* esc */
        ippsVLCEncodeEscBlock_AAC_16s1u(&pStream->x_quant[data_begin],
                                        (data_end-data_begin), &pDst,
                                        &bitoffset, pVLCEncSpec);
      }

      data_begin = data_end;
      sect_counter++;
    }

    sfb_offset += pStream->max_sfb;
  }

  pBS->pCurrent_dword = (Ipp32u*)(pDst - ((size_t)(pDst) & 3));
  pBS->nBit_offset = (Ipp32s)(32 - ((pDst - (Ipp8u*)pBS->pCurrent_dword) << 3)-
                     bitoffset);
  LOAD_DWORD(pBS)
}

/****************************************************************************/

Ipp32s enc_individual_channel_stream(sEnc_individual_channel_stream* pStream,
                                     Ipp32s common_window,
                                     Ipp32s scale_flag,
                                     sBitsreamBuffer* pBS,
                                     Ipp32s writing)
{
  Ipp32s sb;
  Ipp32s count_bits;

  if (writing) {
    Ipp32s num_scale_factor;

    num_scale_factor = pStream->num_window_groups * pStream->max_sfb;

    for (sb = 0; sb < num_scale_factor; sb++) {
      if (pStream->sfb_cb[sb] != 0) {
        pStream->global_gain = pStream->scale_factors[sb];
        break;
      }
    }

    PUT_BITS(pBS,pStream->global_gain,8);
  }

  count_bits = 8;

  if (!common_window && !scale_flag) {
    count_bits += enc_ics_info(pStream, pBS, writing);
  }

  if (writing) {
    enc_section_data(pStream, pBS);
    enc_scale_factor_data(pStream, pBS, 1);
  }

  if (!scale_flag) {
    if (writing) {
      PUT_BITS(pBS,0,1); /// Pulse data isn't supported.
      PUT_BITS(pBS,0,1); /// TNS data isn't supported.
      PUT_BITS(pBS,0,1); /// Gain control data isn't supported.
    }
    count_bits += 3;
  }

  if (writing) {
    enc_spectral_data(pStream, pBS);
  }

  return count_bits;
}

/****************************************************************************/

Ipp32s enc_single_channel_element(sEnc_single_channel_element* pElement,
                                  Ipp32s element_instance_tag,
                                  sBitsreamBuffer* pBS,
                                  Ipp32s writing,
                                  sCrcSaveTable *CrcSaveTable)
{
  Ipp32s count_bits;
  Ipp32s decodedBits0 = 0, decodedBits2 = 0;

  if (writing) {
    CrcSaveTable[0].crc_ptr = pBS->pCurrent_dword;
    CrcSaveTable[0].crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits0)
  }

  if (writing) {
    PUT_BITS(pBS, element_instance_tag,4);
  }

  count_bits = 4;

  count_bits += enc_individual_channel_stream(pElement->p_individual_channel_stream,
                                              0, 0, pBS, writing);

  if (writing) {
    Ipp32s len;

    GET_BITS_COUNT(pBS, decodedBits2)

    len = decodedBits2 - decodedBits0;
    if (len > 192) len = 192;

    CrcSaveTable[0].crc_len = len;
    CrcSaveTable[0].crc_zero_len = 192 - len;
  }

  return count_bits;
}

extern Ipp32s ubit[];

/****************************************************************************/

Ipp32s enc_channel_pair_element(sEnc_channel_pair_element* pElement,
                                Ipp32s element_instance_tag,
                                sBitsreamBuffer* pBS,
                                Ipp32s writing,
                                sCrcSaveTable *CrcSaveTable)
{
  Ipp32s common_window = pElement->common_window;
  Ipp32s count_bits;
  Ipp32s decodedBits0 = 0, decodedBits1 = 0, decodedBits2 = 0;

  if (writing) {
    CrcSaveTable[0].crc_ptr = pBS->pCurrent_dword;
    CrcSaveTable[0].crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits0)
  }

  if (writing) {
    PUT_BITS(pBS,element_instance_tag,4);
    PUT_BITS(pBS,common_window,1);
  }

  count_bits = 5;

  if (common_window) {
    count_bits += enc_ics_info(pElement->p_individual_channel_stream_0,
                               pBS, writing);

    if ((pElement->p_individual_channel_stream_0->audioObjectType == AOT_AAC_LTP) &&
        (EIGHT_SHORT_SEQUENCE !=
        pElement->p_individual_channel_stream_0->windows_sequence)) {

      if (pElement->p_individual_channel_stream_0->predictor_data_present) {
        if (writing) {
          PUT_BITS(pBS, pElement->p_individual_channel_stream_1->ltp_data_present, 1)
        }
        count_bits++;

        if (pElement->p_individual_channel_stream_1->ltp_data_present) {
          count_bits += enc_ltp_data(pElement->p_individual_channel_stream_1,
                                     pBS, writing);
        }
      }
    }

    if (writing) {
      PUT_BITS(pBS, pElement->ms_mask_present, 2);

      if (pElement->ms_mask_present == 1) {
        Ipp32s max_sfb = pElement->p_individual_channel_stream_0[0].max_sfb;
        Ipp32s num_window_groups = pElement->p_individual_channel_stream_0[0].num_window_groups;
        Ipp32s sfb;

        for (sfb = 0; sfb < num_window_groups*max_sfb; sfb++) {
          PUT_BITS(pBS, pElement->ms_used[sfb], 1)
        }
        count_bits += num_window_groups*max_sfb;
      }
    }

    count_bits += 2;
  }

  count_bits += enc_individual_channel_stream(pElement->p_individual_channel_stream_0,
                                              common_window, 0, pBS, writing);

  if (writing) {
    CrcSaveTable[1].crc_ptr = pBS->pCurrent_dword;
    CrcSaveTable[1].crc_offset = pBS->nBit_offset;
    GET_BITS_COUNT(pBS, decodedBits1)
  }

  count_bits += enc_individual_channel_stream(pElement->p_individual_channel_stream_1,
                                              common_window, 0, pBS, writing);

  if (writing) {
    Ipp32s len;

    GET_BITS_COUNT(pBS, decodedBits2)

    len = decodedBits2 - decodedBits0;
    if (len > 192) len = 192;
    CrcSaveTable[0].crc_len = len;
    CrcSaveTable[0].crc_zero_len = 192 - len;

    len = decodedBits2 - decodedBits1;
    if (len > 128) len = 128;
    CrcSaveTable[1].crc_len = len;
    CrcSaveTable[1].crc_zero_len = 128 - len;
  }

  return count_bits;
}

/****************************************************************************/

Ipp32s enc_ltp_data(sEnc_individual_channel_stream* pStream,
                    sBitsreamBuffer* pBS,
                    Ipp32s writing)
{
  Ipp32s  i;
  Ipp32s  pred_max_sfb;

  pred_max_sfb =
    MAX_LTP_SFB_LONG < pStream->max_sfb ? MAX_LTP_SFB_LONG : pStream->max_sfb;

  if (writing) {

    PUT_BITS(pBS, pStream->ltp_lag, 11)
    PUT_BITS(pBS, pStream->ltp_coef, 3)

    for (i = 0; i < pred_max_sfb; i++) {
      PUT_BITS(pBS, pStream->ltp_long_used[i], 1)
    }
  }

  return (14 + pred_max_sfb);
}

/****************************************************************************/

#if 0

void tns_data(sEnc_individual_channel_stream* pStream,
              sBitsreamBuffer* pBS)
{
  Ipp32s w;
  Ipp32s filt;
  Ipp32s i;
  Ipp32s coef_len;
  sTns_data* pData;

  pData = &pStream->tns_data;

  for (w = 0; w < pData->w_number; w++)
  {
    PUT_BITS(pBS,pData->n_filt[w],pData->is_window_long ? 2 : 1);
    if (pData->n_filt[w] == 0) continue;
    PUT_BITS(pBS,pData->coef_res[w],1);

    for(filt = 0; filt < pData->n_filt[w]; filt ++)
    {
      PUT_BITS(pBS,pData->length[w][filt],pData->is_window_long ? 6:4);
      PUT_BITS(pBS,pData->order[w][filt],pData->is_window_long ? 5:3);

      if (pData->order[w][filt] == 0) continue;

      PUT_BITS(pBS,pData->direction[w][filt],1);
      PUT_BITS(pBS,pData->coef_compress[w][filt],1);

      coef_len = 3 + pData->coef_res[w] - pData->coef_compress[w][filt];

      for(i = 0; i < pData->order[w][filt]; i++)
      {
        PUT_BITS(pBS,pData->coef[w][filt][i],coef_len);
      }
    }
  }
}

/****************************************************************************/

#endif

Ipp32u sf_huff_codebook[] = {

 /* len, code*/
 18, 0x3FFE8, 18, 0x3FFE6, 18, 0x3FFE7, 18, 0x3FFE5,
 19, 0x7FFF5, 19, 0x7FFF1, 19, 0x7FFED, 19, 0x7FFF6,
 19, 0x7FFEE, 19, 0x7FFEF, 19, 0x7FFF0, 19, 0x7FFFC,
 19, 0x7FFFD, 19, 0x7FFFF, 19, 0x7FFFE, 19, 0x7FFF7,
 19, 0x7FFF8, 19, 0x7FFFB, 19, 0x7FFF9, 18, 0x3FFE4,
 19, 0x7FFFA, 18, 0x3FFE3, 17, 0x1FFEF, 17, 0x1FFF0,
 16, 0x0FFF5, 17, 0x1FFEE, 16, 0x0FFF2, 16, 0x0FFF3,
 16, 0x0FFF4, 16, 0x0FFF1, 15, 0x07FF6, 15, 0x07FF7,
 14, 0x03FF9, 14, 0x03FF5, 14, 0x03FF7, 14, 0x03FF3,
 14, 0x03FF6, 14, 0x03FF2, 13, 0x01FF7, 13, 0x01FF5,
 12, 0x00FF9, 12, 0x00FF7, 12, 0x00FF6, 11, 0x007F9,
 12, 0x00FF4, 11, 0x007F8, 10, 0x003F9, 10, 0x003F7,
 10, 0x003F5,  9, 0x001F8,  9, 0x001F7,  8, 0x000FA,
  8, 0x000F8,  8, 0x000F6,  7, 0x00079,  6, 0x0003A,
  6, 0x00038,  5, 0x0001A,  4, 0x0000B,  3, 0x00004,
  1, 0x00000,  4, 0x0000A,  4, 0x0000C,  5, 0x0001B,
  6, 0x00039,  6, 0x0003B,  7, 0x00078,  7, 0x0007A,
  8, 0x000F7,  8, 0x000F9,  9, 0x001F6,  9, 0x001F9,
 10, 0x003F4, 10, 0x003F6, 10, 0x003F8, 11, 0x007F5,
 11, 0x007F4, 11, 0x007F6, 11, 0x007F7, 12, 0x00FF5,
 12, 0x00FF8, 13, 0x01FF4, 13, 0x01FF6, 13, 0x01FF8,
 14, 0x03FF8, 14, 0x03FF4, 16, 0x0FFF0, 15, 0x07FF4,
 16, 0x0FFF6, 15, 0x07FF5, 18, 0x3FFE2, 19, 0x7FFD9,
 19, 0x7FFDA, 19, 0x7FFDB, 19, 0x7FFDC, 19, 0x7FFDD,
 19, 0x7FFDE, 19, 0x7FFD8, 19, 0x7FFD2, 19, 0x7FFD3,
 19, 0x7FFD4, 19, 0x7FFD5, 19, 0x7FFD6, 19, 0x7FFF2,
 19, 0x7FFDF, 19, 0x7FFE7, 19, 0x7FFE8, 19, 0x7FFE9,
 19, 0x7FFEA, 19, 0x7FFEB, 19, 0x7FFE6, 19, 0x7FFE0,
 19, 0x7FFE1, 19, 0x7FFE2, 19, 0x7FFE3, 19, 0x7FFE4,
 19, 0x7FFE5, 19, 0x7FFD7, 19, 0x7FFEC, 19, 0x7FFF4,
 19, 0x7FFF3
};

#endif //UMC_ENABLE_XXX
