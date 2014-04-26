/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AACCMN_ADIF_H
#define __AACCMN_ADIF_H

#include "mp4cmn_pce.h"

enum eAACCMN_ADIF_H
{
    ADIF_SIGNATURE   = 0x41444946,
    LEN_COPYRIGHT_ID = ((72/8)),
    MAX_PCE_NUM      = 16
};

typedef struct
{
  Ipp32u adif_id;
  Ipp32s copyright_id_present;
  Ipp8s  copyright_id[LEN_COPYRIGHT_ID+1];
  Ipp32s original_copy;
  Ipp32s home;
  Ipp32s bitstream_type;
  Ipp32u bitrate;
  Ipp32s num_program_config_elements;
  Ipp32u adif_buffer_fullness;

  sProgram_config_element pce[MAX_PCE_NUM];
} sAdif_header;

#ifdef  __cplusplus
extern "C" {
#endif

Ipp32s dec_adif_header(sAdif_header* p_adif_header,sBitsreamBuffer* p_bs);

Ipp32s unpack_adif_header(sAdif_header* p_data,Ipp8u **pp_bitstream, Ipp32s* p_offset);

#ifdef  __cplusplus
}
#endif

#endif//__AACCMN_ADIF_H
