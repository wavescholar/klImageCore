/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_MPEG2_MUXER

#include "umc_mpeg2_muxer_defs.h"

using namespace UMC;

const Ipp16u AC3FrameSize[115] = {
      64,   69,   96,   64,   70,   96,
      80,   87,  120,   80,   88,  120,
      96,  104,  144,   96,  105,  144,
     112,  121,  168,  112,  122,  168,
     128,  139,  192,  128,  140,  192,
     160,  174,  240,  160,  175,  240,
     192,  208,  288,  192,  209,  288,
     224,  243,  336,  224,  244,  336,
     256,  278,  384,  256,  279,  384,
     320,  348,  480,  320,  349,  480,
     384,  417,  576,  384,  418,  576,
     448,  487,  672,  448,  488,  672,
     512,  557,  768,  512,  558,  768,
     640,  696,  960,  640,  697,  960,
     768,  835, 1152,  768,  836, 1152,
     896,  975, 1344,  896,  976, 1344,
    1024, 1114, 1536, 1024, 1115, 1536,
    1152, 1253, 1728, 1152, 1254, 1728,
    1280, 1393, 1920, 1280, 1394, 1920, 0
};

bool UMC::IsVideo(UMC::MPEG2MuxerESType type)
{
    return (MPEG2MUX_ES_MPEG1_VIDEO == type ||
            MPEG2MUX_ES_MPEG2_VIDEO == type ||
            MPEG2MUX_ES_MPEG4_VIDEO == type ||
            MPEG2MUX_ES_H264_VIDEO  == type);
} //bool UMC::IsVideo(MPEG2MuxerESType type)

MPEG2MuxerESType UMC::ConvertAudioType(UMC::SystemStreamType systemType, UMC::AudioStreamType audioType)
{
    if (LPCM_AUDIO == audioType)
        return MPEG2MUX_ES_LPCM_AUDIO;
    else if (AC3_AUDIO == audioType)
        return MPEG2MUX_ES_AC3_AUDIO;
    else if (AAC_AUDIO == audioType && !IsPSOrPES(systemType))
        return MPEG2MUX_ES_AAC_AUDIO;
    else if (MPEG1_AUDIO & audioType)
        return MPEG2MUX_ES_MPEG1_AUDIO;
    else if (MPEG2_AUDIO & audioType)
        return MPEG2MUX_ES_MPEG2_AUDIO;
    else
        return MPEG2MUX_ES_UNKNOWN;
} //MPEG2MuxerESType UMC::ConvertAudioType(UMC::SystemStreamType systemType, UMC::AudioStreamType audioType)

MPEG2MuxerESType UMC::ConvertVideoType(UMC::SystemStreamType systemType, UMC::VideoStreamType videoType)
{
    if (MPEG1_VIDEO == videoType)
        return MPEG2MUX_ES_MPEG1_VIDEO;
    else if (MPEG2_VIDEO == videoType)
        return MPEG2MUX_ES_MPEG2_VIDEO;
    else if (MPEG4_VIDEO == videoType && !IsPSOrPES(systemType))
        return MPEG2MUX_ES_MPEG4_VIDEO;
    else if (H264_VIDEO == videoType && !IsPSOrPES(systemType))
        return MPEG2MUX_ES_H264_VIDEO;
    else
        return MPEG2MUX_ES_UNKNOWN;
} //MPEG2MuxerESType UMC::ConvertVideoType(UMC::SystemStreamType systemType, UMC::VideoStreamType videoType)

bool UMC::IsPure(UMC::SystemStreamType systemType)
{
    return (systemType != MPEG2_PROGRAMM_STREAM &&
            systemType != MPEG2_PES_PACKETS_STREAM &&
            systemType != MPEG2_TRANSPORT_STREAM &&
            systemType != MPEG2_TRANSPORT_STREAM_TTS &&
            systemType != MPEG2_TRANSPORT_STREAM_TTS0);
} //bool UMC::IsPure(UMC::SystemStreamType systemType)

bool UMC::IsPSOrPES(UMC::SystemStreamType systemType)
{
    return (systemType == MPEG2_PROGRAMM_STREAM ||
            systemType == MPEG2_PES_PACKETS_STREAM);
} //bool UMC::IsPSOrPES(UMC::SystemStreamType systemType)

bool UMC::IsTS(UMC::SystemStreamType systemType)
{
    return (systemType == MPEG2_TRANSPORT_STREAM ||
            systemType == MPEG2_TRANSPORT_STREAM_TTS ||
            systemType == MPEG2_TRANSPORT_STREAM_TTS0);
} //bool UMC::IsTS(UMC::SystemStreamType systemType)

bool UMC::IsTTS(UMC::SystemStreamType systemType)
{
    return (systemType == MPEG2_TRANSPORT_STREAM_TTS ||
            systemType == MPEG2_TRANSPORT_STREAM_TTS0);
} //bool UMC::IsTTS(UMC::SystemStreamType systemType)

bool UMC::IsTTS0(UMC::SystemStreamType systemType)
{
    return (systemType == MPEG2_TRANSPORT_STREAM_TTS0);
} //bool UMC::IsTTS0(UMC::SystemStreamType systemType)

#endif
