/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include <ipps.h>

#include "umc_audio_codec.h"
#include "umc_base_codec.h"
#include "umc_media_buffer.h"
#include "umc_data_reader.h"
#include "umc_data_writer.h"
#include "umc_media_receiver.h"
#include "umc_muxer.h"
#include "umc_module_context.h"

namespace UMC
{

AudioCodecParams::AudioCodecParams()
{
    m_frame_num = 0;
    m_pData = NULL;

} // AudioCodecParams::AudioCodecParams()

} // namespace UMC
