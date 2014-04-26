/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AACCMN_ADTS_H
#define __AACCMN_ADTS_H

#include "bstream.h"

typedef struct
{
  Ipp32s ID;
  Ipp32s Layer;
  Ipp32s protection_absent;
  Ipp32s Profile;
  Ipp32s sampling_frequency_index;
  Ipp32s private_bit;
  Ipp32s channel_configuration;
  Ipp32s original_copy;
  Ipp32s Home;
  Ipp32s Emphasis;

} sAdts_fixed_header;

typedef struct
{
  Ipp32s copyright_identification_bit;
  Ipp32s copyright_identification_start;
  Ipp32s aac_frame_length;
  Ipp32s adts_buffer_fullness;
  Ipp32s no_raw_data_blocks_in_frame;

} sAdts_variable_header;

#ifdef  __cplusplus
extern "C" {
#endif

Ipp32s  dec_adts_fixed_header(sAdts_fixed_header* pHeader,sBitsreamBuffer* BS);
Ipp32s  dec_adts_variable_header(sAdts_variable_header* pHeader,sBitsreamBuffer* BS);

Ipp32s unpack_adts_fixed_header(sAdts_fixed_header* p_header,Ipp8u **pp_bitstream, Ipp32s *p_offset);
Ipp32s unpack_adts_variable_header(sAdts_variable_header* p_header,Ipp8u **pp_bitstream, Ipp32s *p_offset);

Ipp32s get_audio_object_type_by_adts_header(sAdts_fixed_header* p_header);

#ifdef  __cplusplus
}
#endif

#endif//__AACCMN_ADTS_H
