/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP4CMN_PCE_H__
#define __MP4CMN_PCE_H__

#include "bstream.h"

enum eMP4CMN_PCE_H
{
    LEN_COMMENT             = (1<<8),
    MAX_CHANNELS_ELEMENTS   = 16,
    MAX_ASSOC_DATA_ELEMENTS =  8,
    MAX_VALID_CC_ELEMENTS   = 16
};

typedef struct
{
  Ipp32s element_instance_tag;
  Ipp32s object_type;
  Ipp32s sampling_frequency_index;

  Ipp32s num_front_channel_elements;
  Ipp32s num_side_channel_elements;
  Ipp32s num_back_channel_elements;
  Ipp32s num_lfe_channel_elements;

  Ipp32s num_assoc_data_elements;
  Ipp32s num_valid_cc_elements;

  Ipp32s mono_mixdown_present;
  Ipp32s mono_miwdown_element_number;

  Ipp32s stereo_mixdown_present;
  Ipp32s stereo_miwdown_element_number;

  Ipp32s matrix_mixdown_idx_present;
  Ipp32s matrix_mixdown_idx;
  Ipp32s pseudo_surround_enable;

  Ipp32s front_element_is_cpe[MAX_CHANNELS_ELEMENTS];
  Ipp32s front_element_tag_select[MAX_CHANNELS_ELEMENTS];
  Ipp32s side_element_is_cpe[MAX_CHANNELS_ELEMENTS];
  Ipp32s side_element_tag_select[MAX_CHANNELS_ELEMENTS];
  Ipp32s back_element_is_cpe[MAX_CHANNELS_ELEMENTS];
  Ipp32s back_element_tag_select[MAX_CHANNELS_ELEMENTS];
  Ipp32s lfe_element_tag_select[MAX_CHANNELS_ELEMENTS];

  Ipp32s assoc_data_element_tag_select[MAX_ASSOC_DATA_ELEMENTS];
  Ipp32s cc_element_is_ind_sw[MAX_VALID_CC_ELEMENTS];
  Ipp32s valid_cc_element_tag_select[MAX_VALID_CC_ELEMENTS];

  Ipp32s comment_field_bytes;
  Ipp8s  comment_field_data[LEN_COMMENT];

  Ipp32s num_front_channels;
  Ipp32s num_side_channels;
  Ipp32s num_back_channels;
  Ipp32s num_lfe_channels;

} sProgram_config_element;

#ifdef  __cplusplus
extern "C" {
#endif

Ipp32s dec_program_config_element(sProgram_config_element * p_data,sBitsreamBuffer * p_bs);

/***********************************************************************

    Unpack function(s) (support(s) alternative bitstream representation)

***********************************************************************/

Ipp32s unpack_program_config_element(sProgram_config_element * p_data,Ipp8u **pp_bitstream, Ipp32s *p_offset);

#ifdef  __cplusplus
}
#endif

#endif//__MP4CMN_PCE_H__
