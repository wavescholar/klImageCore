/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "aaccmn_adif.h"
#include "bstream.h"
#include "mp4cmn_pce.h"

Ipp32s
dec_adif_header(sAdif_header* p_adif_header,sBitsreamBuffer* p_bs)
{

  Ipp32s i;

  p_adif_header->adif_id = Getbits(p_bs,16);
  p_adif_header->adif_id <<= 16;
  p_adif_header->adif_id += Getbits(p_bs,16);

  if (p_adif_header->adif_id != ADIF_SIGNATURE)
  {
    return 1;
  }
  p_adif_header->copyright_id_present = Getbits(p_bs,1);
  if (p_adif_header->copyright_id_present)
  {

    for (i = 0; i < LEN_COPYRIGHT_ID ; i++)
    {
      p_adif_header->copyright_id[i] = (Ipp8s)Getbits(p_bs,8);
    }
      p_adif_header->copyright_id[i] = 0;

  }
  p_adif_header->original_copy = Getbits(p_bs,1);
  p_adif_header->home = Getbits(p_bs,1);
  p_adif_header->bitstream_type = Getbits(p_bs,1);
  p_adif_header->bitrate = Getbits(p_bs,23);
  p_adif_header->num_program_config_elements = Getbits(p_bs,4);

  if (p_adif_header->bitstream_type == 0)
  {
    p_adif_header->adif_buffer_fullness = Getbits(p_bs,20);
  }

  for (i = 0; i < p_adif_header->num_program_config_elements + 1; i++)
  {
    dec_program_config_element(&p_adif_header->pce[i],p_bs);
  }

  return 0;
}

/***********************************************************************

    Unpack function(s) (support(s) alternative bitstream representation)

***********************************************************************/

Ipp32s
unpack_adif_header(sAdif_header* p_adif_header,Ipp8u **pp_bitstream, Ipp32s *p_offset)
{
  Ipp32s i;

  p_adif_header->adif_id = get_bits(pp_bitstream,p_offset,16);
  p_adif_header->adif_id <<= 16;
  p_adif_header->adif_id += get_bits(pp_bitstream,p_offset,16);

  if (p_adif_header->adif_id != ADIF_SIGNATURE)
  {
    return 1;
  }
  p_adif_header->copyright_id_present = get_bits(pp_bitstream,p_offset,1);
  if (p_adif_header->copyright_id_present)
  {
    for (i = 0; i < LEN_COPYRIGHT_ID ; i++)
    {
      p_adif_header->copyright_id[i] = (Ipp8s)get_bits(pp_bitstream,p_offset,8);
    }
      p_adif_header->copyright_id[i] = 0;

  }
  p_adif_header->original_copy = get_bits(pp_bitstream,p_offset,1);
  p_adif_header->home = get_bits(pp_bitstream,p_offset,1);
  p_adif_header->bitstream_type = get_bits(pp_bitstream,p_offset,1);
  p_adif_header->bitrate = get_bits(pp_bitstream,p_offset,23);
  p_adif_header->num_program_config_elements = get_bits(pp_bitstream,p_offset,4);

  if (p_adif_header->bitstream_type == 0)
  {
    p_adif_header->adif_buffer_fullness = get_bits(pp_bitstream,p_offset,20);
  }

  for (i = 0; i < p_adif_header->num_program_config_elements + 1; i++)
  {
    unpack_program_config_element(&p_adif_header->pce[i],pp_bitstream,p_offset);
  }

  return 0;
}

#endif //UMC_ENABLE_XXX

