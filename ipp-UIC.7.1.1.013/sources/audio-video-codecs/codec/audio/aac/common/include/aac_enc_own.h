/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AAC_ENC_OWN_H
#define __AAC_ENC_OWN_H

#include "bstream.h"
#include "aaccmn_const.h"
#include "mp4cmn_pce.h"
#include "ippac.h"

#define AAC_UPDATE_PTR(type, ptr, inc)    \
  if (ptr) {                              \
    ptr = (type*)((Ipp8u*)(ptr) + inc); \
  }

typedef struct {
  enum AudioObjectType audioObjectType;
  Ipp32s global_gain;
  Ipp32s pulse_data_present;
  Ipp32s tns_data_present;
  Ipp32s gain_control_data_present;

  /* ics_info */
  Ipp32s ics_reserved_bit;
  Ipp32s windows_sequence;
  Ipp32s window_shape;
  Ipp32s ath_max_sfb;
  Ipp32s max_sfb;
  Ipp32s max_line;
  Ipp32s scale_factor_grouping[7];
  Ipp32s num_window_groups;
  Ipp32s len_window_group[8];
  Ipp32s predictor_data_present;

  /* section_data */
  Ipp32s sect_cb[MAX_SECTION_NUMBER];
  Ipp32s sect_len[MAX_SECTION_NUMBER];
  Ipp32s sect_num[8];
  Ipp16s sfb_cb[MAX_SECTION_NUMBER];

  /* scale_factor_data */
  Ipp16s scale_factors[MAX_SECTION_NUMBER];

  /* spectral_data */
  Ipp16s  x_quant[1024];
  Ipp32s* sfb_offset;
  Ipp32s* sfb_width;
  void**  pHuffTables;

  /* LTP data */
  Ipp32s  ltp_data_present;
  Ipp32s  ltp_lag_update;
  Ipp32s  ltp_lag;
  Ipp32s  ltp_coef;
  Ipp32s  ltp_long_used[MAX_SFB];
  Ipp32s  ltp_short_used[MAX_NUM_WINDOWS];
  Ipp32s  ltp_short_lag_present[MAX_NUM_WINDOWS];
  Ipp32s  ltp_short_lag[MAX_NUM_WINDOWS];
} sEnc_individual_channel_stream;

typedef struct
{
  Ipp32s* common_scalefactor_update;
  Ipp32s* last_frame_common_scalefactor;

    /// Special variables...
  Ipp32s  start_common_scalefac;
  Ipp32s  finish_common_scalefac;
  Ipp32s  available_bits;
  Ipp32s  bits_per_frame;
  Ipp32s  used_bits;
  Ipp32s  ns_mode;
  Ipp32s  sfb_width_scale;
  Ipp16s  ns_scale_factors[MAX_SECTION_NUMBER];
} sQuantizationBlock;

typedef struct {
  sEnc_individual_channel_stream*  p_individual_channel_stream;
} sEnc_single_channel_element;

typedef struct {
  Ipp32s common_window;
  Ipp32s ms_mask_present;
  Ipp32s ms_used[MAX_SECTION_NUMBER];

  sEnc_individual_channel_stream*  p_individual_channel_stream_0;
  sEnc_individual_channel_stream*  p_individual_channel_stream_1;
} sEnc_channel_pair_element;

typedef struct {
  Ipp32s   element_id;
  Ipp32s   element_instance_tag;
  Ipp32s   prev_window_shape;
  Ipp32s   bits_in_buf;
  Ipp32s   max_bits_in_buf;
  Ipp32s   mean_bits;
  Ipp32s   common_scalefactor_update;
  Ipp32s   last_frame_common_scalefactor;
} sOneChannelInfo;

typedef struct {
  Ipp32u *crc_ptr;
  Ipp32s  crc_offset;
  Ipp32s  crc_len;
  Ipp32s  crc_zero_len;
} sCrcSaveTable;

typedef struct {
  Ipp32s   m_channel_number;
  Ipp32s   m_sampling_frequency;
  Ipp32s   m_bitrate;
  Ipp32s   m_frame_number;

  Ipp32s   m_buff_prev_index;
  Ipp32s   m_buff_curr_index;
  Ipp32s   m_buff_next_index;

  Ipp32s   sampling_frequency_index;
  enum  AudioObjectType audioObjectType;
  /* HEAAC profile */
  enum  AudioObjectType auxAudioObjectType;
  Ipp32s*  sfb_offset[4];
  Ipp32s   real_max_sfb[4];
  Ipp32s   real_max_line[4];
  Ipp32s   non_zero_line[4];
  Ipp32s   real_max_sfb_lfe[4];
  Ipp32s   real_max_line_lfe[4];
  Ipp32s   non_zero_line_lfe[4];
  Ipp32s   ath_max_sfb[4];
  Ipp32s   ath_max_sfb_lfe[4];
  Ipp32s   allowHolesSfb[4];
  Ipp32s   sfb_offset_for_short_window[MAX_SECTION_NUMBER + 1];
  Ipp32s   stereo_mode_param;
  Ipp32s   lfe_channel_present;

  IppsVLCEncodeSpec_32s*  huffman_tables[12];
  IppsVLCEncodeSpec_32s*  sbrHuffTabs[10];
  Ipp16s** buff;

  sOneChannelInfo* chInfo;
  sCrcSaveTable*   crcSaveTable;
} AACEnc_com;

#ifdef  __cplusplus
extern "C" {
#endif

Ipp32s enc_single_channel_element(sEnc_single_channel_element* pElement,
                                  Ipp32s element_instance_tag,
                                  sBitsreamBuffer* pBS,
                                  Ipp32s writing,
                                  sCrcSaveTable *CrcSaveTable);

Ipp32s enc_channel_pair_element(sEnc_channel_pair_element* pElement,
                                Ipp32s element_instance_tag,
                                sBitsreamBuffer* pBS,
                                Ipp32s writing,
                                sCrcSaveTable *CrcSaveTable);

Ipp32s enc_scale_factor_data(sEnc_individual_channel_stream* pStream,
                             sBitsreamBuffer* pBS,
                             Ipp32s writing);

void enc_section_data(sEnc_individual_channel_stream* pStream,
                      sBitsreamBuffer* pBS);

Ipp32s fill_program_config_element(sProgram_config_element* p_data,
                                   Ipp32s element_instance_tag,
                                   AACEnc_com *state_com);

#ifdef  __cplusplus
}
#endif
#endif
