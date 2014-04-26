/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AAC_DEC_OWN_H
#define __AAC_DEC_OWN_H

#include "bstream.h"
#include "aaccmn_const.h"
#include "sbr_dec_struct.h"
#include "ps_dec_struct.h"
#include "aaccmn_chmap.h"
#include "aac_status.h"

typedef struct {
  Ipp32s  adjust_num;
  Ipp32s  alevcode[8];
  Ipp32s  aloccode[8];
} SSR_GAIN;

typedef struct {

  Ipp32s  global_gain;

  /* ics_info */
  Ipp32s  ics_reserved_bit;
  Ipp32s  window_sequence;
  Ipp32s  window_shape;

  Ipp32s  max_sfb;
  Ipp32s  num_swb_long;
  Ipp32s  num_swb_short;
  Ipp32s  scale_factor_grouping[7];

  Ipp32s  num_window_groups;
  Ipp32s  len_window_group[8];
  Ipp32s  num_windows;

  Ipp32s  predictor_data_present;
  Ipp32s  predictor_reset;
  Ipp32s  predictor_reset_group_number;
  Ipp32s  pred_max_sfb;
  Ipp8u   prediction_used[41];

  Ipp32s  pulse_data_present;
  Ipp32s  tns_data_present;
  Ipp32s  gain_control_data_present;

  Ipp32s  sfb_cb[MAX_GROUP_NUMBER][MAX_SFB];
  Ipp16s  sf[MAX_GROUP_NUMBER][MAX_SFB];

  Ipp16s  spectrum_data[1024];
  Ipp16s *p_spectrum[8];

  Ipp32s *sfb_offset_long_window;
  Ipp32s *sfb_offset_short_window;

  void  **p_huffman_tables;
  Ipp32s *vlcShifts;
  Ipp32s *vlcOffsets;
  Ipp32s *vlcTypes;

  Ipp32s  number_pulse;
  Ipp32s  pulse_start_sfb;
  Ipp32s  pulse_offset[MAX_NUMBER_PULSE];
  Ipp32s  pulse_amp[MAX_NUMBER_PULSE];

  /* Tns data */
  Ipp32s  n_filt[MAX_NUM_WINDOWS];
  Ipp32s  coef_res[MAX_NUM_WINDOWS];
  Ipp32s  length[MAX_NUM_WINDOWS][MAX_FILT];
  Ipp32s  order[MAX_NUM_WINDOWS][MAX_FILT];
  Ipp32s  direction[MAX_NUM_WINDOWS][MAX_FILT];
  Ipp32s  coef_compress[MAX_NUM_WINDOWS][MAX_FILT];
  Ipp32s  coef[MAX_NUM_WINDOWS][MAX_FILT][MAX_ORDER];
  Ipp32s  tns_max_bands_short;
  Ipp32s  tns_max_bands_long;
  Ipp32s  tns_max_order_short;
  Ipp32s  tns_max_order_long;

  /* Gain control data */
  Ipp32s   max_band;
  SSR_GAIN SSRInfo[4][8];

  /* LTP data */
  Ipp32s  ltp_data_present;
  Ipp32s  ltp_lag_update;
  Ipp32s  ltp_lag;
  Ipp32s  ltp_coef;
  Ipp32s  ltp_long_used[MAX_SFB];
  Ipp32s  ltp_short_used[MAX_NUM_WINDOWS];
  Ipp32s  ltp_short_lag_present[MAX_NUM_WINDOWS];
  Ipp32s  ltp_short_lag[MAX_NUM_WINDOWS];

} s_SE_Individual_channel_stream;

typedef struct {
  Ipp32s  element_instance_tag;
  Ipp32s  crc_enable;
  Ipp32s  crc;

  s_SE_Individual_channel_stream stream;
} sLfe_channel_element;

typedef struct {
  Ipp32s  element_instance_tag;
  Ipp32s  crc_enable;
  Ipp32s  crc;

  s_SE_Individual_channel_stream stream;
} sSce_channel_element;

typedef struct {
  Ipp32s  element_instance_tag;
  Ipp32s  crc_enable;
  Ipp32s  crc;
  Ipp32s  common_window;

  Ipp32s  ms_mask_present;
  Ipp32s  ms_used[8][49];

  s_SE_Individual_channel_stream streams[2];
} sCpe_channel_element;

typedef struct {
  Ipp32s  element_instance_tag;
  Ipp8s   data_stream_byte[512];

} sData_stream_element;

typedef struct {
  Ipp32s  element_instance_tag;
  Ipp32s  crc_enable;
  Ipp32s  crc;
  Ipp16s  cc_fact[18][MAX_GROUP_NUMBER][MAX_SFB];

  s_SE_Individual_channel_stream stream;
} sCoupling_channel_element;

typedef struct {
  Ipp32s  ind_sw_cce_flag;
  Ipp32s  num_coupled_elements;
  Ipp32s  cc_target_id[9];
  Ipp32s  cc_target_tag[9];
  Ipp32s  cc_target_ch[9];
  Ipp32s  cc_lr[18];
  Ipp32s  cge[18];
  Ipp32s  max_sfb;
  Ipp32s  num_window_groups;
  Ipp32s  len_window_group[8];
  Ipp32s  cc_domain;
  Ipp32s  gain_element_sign;
  Ipp32s  gain_element_scale;
  Ipp32s  num_gain_element_lists;
  Ipp32s  *sfb_offset;
} sCoupling_channel_data;

typedef struct {
  Ipp8u   exclude_mask[128];
  Ipp8u   additional_excluded_chns[10];
  Ipp32s  num_excl_chan;

} sExcluded_channels;

typedef struct {
  Ipp32s  drc_num_bands;

  Ipp32s  pce_tag_present;
  Ipp32s  pce_innstance_tag;
  Ipp32s  drc_tag_reserved_bits;

  Ipp32s  excluded_chns_present;

  sExcluded_channels ec_data;
  Ipp32s  drc_bands_present;
  Ipp32s  drc_band_incr;
  Ipp32s  drc_bands_reserved_bits;
  Ipp32s  drc_band_top[17];
  Ipp32s  prog_ref_level_present;
  Ipp32s  prog_ref_level;
  Ipp32s  prog_ref_level_reserved_bits;

  Ipp32s  dyn_rng_sgn[17];
  Ipp32s  dyn_rng_ctl[17];

} sDynamic_range_info;

typedef struct {
  Ipp32s                    m_is_chmap_valid;
  Ipp32s                    m_is_pce_valid;
  Ipp32s                    m_sampling_frequency;
  Ipp32s                    m_up_sample;
  Ipp32s                    m_sampling_frequency_index;
  Ipp32s                    m_element_number;
  Ipp32s                    m_channel_number;
  Ipp32s                    m_channel_number_save;
  Ipp32s                    m_channel_number_all;
  Ipp32s                    m_channel_config;
  Ipp32s                    m_frame_number;
  Ipp32s                    m_frame_size;
  enum AudioObjectType      m_audio_object_type;
  sCh_map_item              m_chmap[CH_MAX];
  sEl_map_item              m_elmap[CH_MAX];

  Ipp32s                    m_index_1st;
  Ipp32s                    m_index_2nd;
  Ipp32s                    m_index_3rd;

  Ipp32s                    m_curr_win_shape[CH_MAX + COUPL_CH_MAX];
  Ipp32s                    m_prev_win_shape[CH_MAX + COUPL_CH_MAX];
  Ipp32s                    m_curr_win_sequence[CH_MAX + COUPL_CH_MAX];

  Ipp32s                    m_order[CH_MAX + COUPL_CH_MAX];
  Ipp32s                    noiseState;
  Ipp32s                    adts_channel_configuration;

  SSR_GAIN                  prevSSRInfo[CH_MAX + COUPL_CH_MAX][4];
  SSR_GAIN                  SSRInfo[CH_MAX + COUPL_CH_MAX][4][8];

  sProgram_config_element   m_pce;
  sCpe_channel_element      m_cpe;
  sSce_channel_element      m_sce;
  sLfe_channel_element      m_lfe;
  sCoupling_channel_element m_cce;
  sCoupling_channel_data    m_cdata[COUPL_CH_MAX];

  Ipp32s                    SbrFlagPresent;
  Ipp32s                    ModeDecodeHEAACprofile;
  Ipp32s                    ModeDwnsmplHEAACprofile;

  eSBR_SUPPORT              m_flag_SBR_support_lev;
  Ipp32s                    m_flag_PS_support_lev;

  IppsVLCDecodeSpec_32s     *sbrHuffTables[10];
  void                      *huffman_tables[16];

  Ipp32s                    FirstID3Search;
  Ipp32s                    id3_size;
  Ipp32s                    decodedBytes;
  Ipp32s                    crc_enable;
  Ipp32s                    crc;
  Ipp32s                    saved_crc;
  Ipp32s                    layer;
} AACDec_com;

#ifdef  __cplusplus
extern  "C" {
#endif

  Ipp32s  ics_apply_pulse_I(s_SE_Individual_channel_stream *p_data);
  Ipp32s  dec_individual_channel_stream(s_SE_Individual_channel_stream *pData,
                                        sBitsreamBuffer *pBS,
                                        Ipp32s common_window,
                                        Ipp32s scal_flag,
                                        enum AudioObjectType audioObjectType);
  Ipp32s  dec_cpe_channel_element(sCpe_channel_element *pElement,
                                  sBitsreamBuffer *pBS,
                                  enum AudioObjectType audioObjectType);
  Ipp32s  dec_sce_channel_element(sSce_channel_element *pElement,
                                  sBitsreamBuffer *pBS,
                                  enum AudioObjectType audioObjectType);
  Ipp32s  dec_lfe_channel_element(sLfe_channel_element *pElement,
                                  sBitsreamBuffer *pBS,
                                  enum AudioObjectType audioObjectType);
  Ipp32s  dec_coupling_channel_element(sCoupling_channel_element *pElement,
                                       sCoupling_channel_data *pData,
                                       sBitsreamBuffer *pBS,
                                       enum AudioObjectType audioObjectType);
  Ipp32s  dec_data_stream_element(sData_stream_element *pData,
                                  sBitsreamBuffer *pBS);

  Ipp32s  dec_fill_element(sSBRDecComState * pSBRItem,
                           //sPSDecComState*     pPSItem,
                           Ipp32s *cnt_fill_sbr_element,
                           sDynamic_range_info *pInfo,
                           sBitsreamBuffer *pBS,
                           eSBR_SUPPORT flag_SBR_support_lev);
  Ipp32s  sbr_extension_data(Ipp32s id_aac,
                             Ipp32s crc_flag,
                             sSBRDecComState * pSbr,
                             sBitsreamBuffer *BS,
                             Ipp32s cnt);

  Ipp32s  dec_dynamic_range_info(sDynamic_range_info *pInfo,
                                 sBitsreamBuffer *pBS);

  AACStatus aacdecSetSamplingFrequencyCom(Ipp32s sampling_frequency_index,
                                          AACDec_com *state_com);

  void save_gain_control_data(Ipp32s ch,
                              s_SE_Individual_channel_stream *pData,
                              AACDec_com *state_com);

  Ipp32s  dec_ics_info(s_SE_Individual_channel_stream *pData,
                     sBitsreamBuffer *pBS,
                     enum AudioObjectType audioObjectType);

Ipp32s  ics_info_copy(s_SE_Individual_channel_stream *pDataSrc,
                      s_SE_Individual_channel_stream *pDataDst,
                      enum AudioObjectType audioObjectType);

Ipp32s  dec_section_data(s_SE_Individual_channel_stream *pData,
                         sBitsreamBuffer *pBS);

Ipp32s  dec_scale_factor_data(s_SE_Individual_channel_stream *pData,
                              Ipp16s scalef[8][51],
                              Ipp32s scale_factor,
                              Ipp32s noise_nrg,
                              sBitsreamBuffer *pBS);

Ipp32s  dec_spectral_data(s_SE_Individual_channel_stream *pData,
                          sBitsreamBuffer *BS);

Ipp32s  dec_pulse_data(s_SE_Individual_channel_stream *pData,
                       sBitsreamBuffer *pBS);

Ipp32s  dec_tns_data(s_SE_Individual_channel_stream *pData,
                     sBitsreamBuffer *pBS);

Ipp32s  dec_gain_control_data(s_SE_Individual_channel_stream *pData,
                              sBitsreamBuffer *pBS);

Ipp32s  dec_ltp_data(s_SE_Individual_channel_stream *pData,
                     sBitsreamBuffer *pBS,
                     enum AudioObjectType audioObjectType);

#ifdef  __cplusplus
}
#endif

#define EXT_FILL          0x00
#define EXT_FILL_DATA     0x01
#define EXT_DYNAMIC_RANGE 0x0B
#define EXT_SBR_DATA      0x0D
#define EXT_SBR_DATA_CRC  0x0E
#endif
