/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG2_MUXER_DEFS_H__
#define __UMC_MPEG2_MUXER_DEFS_H__

#include "ippdefs.h"
#include "umc_media_data.h"
#include "umc_mpeg2_muxer_bitstream.h"

#define CHECK_TRACK_TYPE(INDEX, TYPE) (m_pParams->m_pTrackParams[INDEX].type == TYPE)

#define IS_TIME_EQUAL(REF, TEST) (0 == (Ipp32s)(90000 * ((REF) - (TEST))))

extern const Ipp16u AC3FrameSize[115];

namespace UMC
{
    static const Ipp32u MPEG2MUX_SYS_CLOCK_FREQ     = 27000000;
    static const Ipp32u MPEG2MUX_TIME_TO_SLEEP      = 5;

    static const Ipp32u MPEG2MUX_LPCM_FRAME_SIZE    = 320;
    static const Ipp32u MPEG2MUX_CHUNK_HEADER_SIZE  = 256;
    static const Ipp32u MPEG2MUX_TS_PACKET_LENGTH   = 188;
    static const Ipp32u MPEG2MUX_MAX_TS_PAYLOAD     = 184;

    static const Ipp32u MPEG2MUX_SYS_ID_AUDIO       = 0xB8;
    static const Ipp32u MPEG2MUX_SYS_ID_VIDEO       = 0xB9;
    static const Ipp32u MPEG2MUX_PES_ID_AUDIO       = 0xC0;
    static const Ipp32u MPEG2MUX_PES_ID_VIDEO       = 0xE0;
    static const Ipp32u MPEG2MUX_PES_ID_PRIVATE_1   = 0xBD;
    static const Ipp32u MPEG2MUX_PES_ID_PADDING     = 0xBE;
    static const Ipp32u MPEG2MUX_AC3_SUB_ID         = 0x80;
    static const Ipp32u MPEG2MUX_LPCM_SUB_ID        = 0xA0;

    static const Ipp32u MPEG2MUX_PROGRAM_MAP_PID    = 0x006E;
    static const Ipp32u MPEG2MUX_INITIAL_ES_PID     = 0x0070;

    static const Ipp32u MPEG2MUX_TS_ID              = 0x0000;

    typedef enum
    {
        MPEG2MUX_ES_MPEG1_VIDEO = 0x01,
        MPEG2MUX_ES_MPEG2_VIDEO = 0x02,
        MPEG2MUX_ES_MPEG4_VIDEO = 0x10,
        MPEG2MUX_ES_H264_VIDEO  = 0x1B,
        MPEG2MUX_ES_LPCM_AUDIO  = 0x83,
        MPEG2MUX_ES_AC3_AUDIO   = 0x81,
        MPEG2MUX_ES_AAC_AUDIO   = 0x0F,
        MPEG2MUX_ES_MPEG1_AUDIO = 0x03,
        MPEG2MUX_ES_MPEG2_AUDIO = 0x04,
        MPEG2MUX_ES_VBI_DATA    = 0x06,
        MPEG2MUX_ES_UNKNOWN     = 0xFF
    } MPEG2MuxerESType;

    // Determines if type is video or not.
    bool IsVideo(MPEG2MuxerESType type);

    // Converts UMC audio type to MPEG2Muxer stream type.
    // It also checks permissibility of audio type in the system stream of certain type.
    // Return MPEG2MUX_ES_UNKNOWN if audio type is impermissible.
    MPEG2MuxerESType ConvertAudioType(SystemStreamType systemType, AudioStreamType audioType);

    // Converts UMC video type to MPEG2Muxer stream type
    // It also checks permissibility of video type in the system stream of certain type.
    // Return MPEG2MUX_ES_UNKNOWN if video type is impermissible.
    MPEG2MuxerESType ConvertVideoType(SystemStreamType systemType, VideoStreamType videoType);

    // Determines if system type is pure audio or video
    bool IsPure(SystemStreamType systemType);

    // Determines of system type is MPEG2PS or MPEG2PES
    bool IsPSOrPES(SystemStreamType systemType);

    // Determines of system type is MPEG2TS, MPEG2TTS or MPEG2TTS with zero time-stamps
    bool IsTS(SystemStreamType systemType);

    // Determines of system type is MPEG2TTS or MPEG2TTS with zero time-stamps
    bool IsTTS(SystemStreamType systemType);

    // Determines of system type is MPEG2TTS with zero time-stamps
    bool IsTTS0(SystemStreamType systemType);

}; // end namespace UMC

#endif // __UMC_MPEG2_MUXER_DEFS_H__
