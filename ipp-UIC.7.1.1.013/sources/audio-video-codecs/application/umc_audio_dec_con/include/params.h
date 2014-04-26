/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "umc_config.h"

#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
#include "umc_aac_decoder.h"
#endif
#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
#include "umc_mp3_decoder.h"
#endif
#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
#include "umc_ac3_decoder.h"
#endif

#include "umc_defs.h"
#include "umc_parser_cmd.h"

using namespace UMC;

struct ProgramParameters
{
  DString input_file;
  DString output_file;
  DString codec_name;
  Ipp32s  timing;
  Ipp32s  channel_conf;

#ifdef UMC_ENABLE_AC3_AUDIO_DECODER
  AC3DecoderParams ac3dec_params;
#endif
#ifdef UMC_ENABLE_AAC_AUDIO_DECODER
  AACDecoderParams aacdec_params;
#endif
#ifdef UMC_ENABLE_MP3_AUDIO_DECODER
  MP3DecoderParams mp3dec_params;
#endif

  Ipp32s  puttocsv;

};

Ipp32s  params_fill_by_default(ProgramParameters * p_param);
Status  GetParams(ParserCmd *pCmd, ProgramParameters *pParams);

#endif // __PARAMS_H__
