/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "vm_strings.h"
#include "umc_structures.h"

#define UMC_SAME_NAME(X) { X, VM_STRING(#X) }
#define UMC_CODE_TO_STRING(table, code) umcCodeToString(table, sizeof(table)/sizeof(CodeStringTable), code)
#define UMC_STRING_TO_CODE(table, string, type) (type)umcStringToCode(table, sizeof(table)/sizeof(CodeStringTable), string);

namespace UMC
{

struct CodeStringTable
{
    int code;
    const vm_char *string;
};

static const CodeStringTable StringsOfError[] = {
    { UMC_OK,                    VM_STRING("No any errors") },
    { UMC_ERR_FAILED,            VM_STRING("General operation fault") },
    { UMC_ERR_NOT_INITIALIZED,   VM_STRING("Object was not initialized before usage") },
    { UMC_ERR_TIMEOUT,           VM_STRING("Timeout") },
    { UMC_ERR_NOT_ENOUGH_DATA,   VM_STRING("Not enough data") },
    { UMC_ERR_NULL_PTR,          VM_STRING("Zero pointer was passed as param") },
    { UMC_ERR_INIT,              VM_STRING("Failed to initialize codec") },
    { UMC_ERR_SYNC,              VM_STRING("Required suncronization code was not found") },
    { UMC_ERR_NOT_ENOUGH_BUFFER, VM_STRING("Buffer size is not enough") },
    { UMC_ERR_END_OF_STREAM,     VM_STRING("End of stream") },
    { UMC_ERR_OPEN_FAILED,       VM_STRING("Device/file open error") },
    { UMC_ERR_ALLOC,             VM_STRING("Failed to allocate memory") },
    { UMC_ERR_INVALID_STREAM,    VM_STRING("Invalid stream") },
    { UMC_ERR_UNSUPPORTED,       VM_STRING("Unsupported") },
    { UMC_ERR_NOT_IMPLEMENTED,   VM_STRING("Not implemented yet") },
    { UMC_ERR_INVALID_PARAMS,    VM_STRING("Incorrect parameters") },
};

static const CodeStringTable StringsOfVideoRenderType[] = {
    { DEF_VIDEO_RENDER,    VM_STRING("DEFAULT") },
    { GDI_VIDEO_RENDER,    VM_STRING("GDI") },
    { FW_VIDEO_RENDER,     VM_STRING("FILE WRITER") },
    { NULL_VIDEO_RENDER,   VM_STRING("NULL") },
    { OPENGL_VIDEO_RENDER, VM_STRING("OPENGL") },
};

static const CodeStringTable StringsOfAudioRenderType[] = {
    { DEF_AUDIO_RENDER,    VM_STRING("DEFAULT") },
    { DSOUND_AUDIO_RENDER, VM_STRING("DSOUND") },
    { WINMM_AUDIO_RENDER,  VM_STRING("WINMM") },
    { OSS_AUDIO_RENDER,    VM_STRING("OSS") },
    { NULL_AUDIO_RENDER,   VM_STRING("NULL") },
    { FW_AUDIO_RENDER,     VM_STRING("FILE WRITER") },
};

static const CodeStringTable StringsOfFormatType[] = {
    {NONE, VM_STRING("DEFAULT")},
    UMC_SAME_NAME(YV12),
    UMC_SAME_NAME(NV12),
    UMC_SAME_NAME(YUY2),
    UMC_SAME_NAME(UYVY),
    UMC_SAME_NAME(YUV411),
    UMC_SAME_NAME(YUV420),
    UMC_SAME_NAME(YUV422),
    UMC_SAME_NAME(YUV444),
    UMC_SAME_NAME(Y411),
    UMC_SAME_NAME(Y41P),
    UMC_SAME_NAME(YUV420A),
    UMC_SAME_NAME(YUV422A),
    UMC_SAME_NAME(YUV444A),
    UMC_SAME_NAME(YVU9),
    UMC_SAME_NAME(GRAY),
    UMC_SAME_NAME(GRAYA),
    UMC_SAME_NAME(RGBA),
    UMC_SAME_NAME(RGB),
    UMC_SAME_NAME(RGB565),
    UMC_SAME_NAME(RGB555),
    UMC_SAME_NAME(RGB444),
    UMC_SAME_NAME(BGRA),
    UMC_SAME_NAME(BGR),
    UMC_SAME_NAME(BGR565),
    UMC_SAME_NAME(BGR555),
    UMC_SAME_NAME(BGR444),
    UMC_SAME_NAME(CMYK),
    UMC_SAME_NAME(CMYKA),
    UMC_SAME_NAME(D3D_SURFACE),
    UMC_SAME_NAME(LVA_SURFACE)
};

static const CodeStringTable StringsOfAudioType[] = {
    { UNDEF_AUDIO,         VM_STRING("UNDEF") },
    { PCM_AUDIO,           VM_STRING("PCM") },
    { LPCM_AUDIO,          VM_STRING("LPCM") },
    { AC3_AUDIO,           VM_STRING("AC3") },
    { ALAW_AUDIO,          VM_STRING("A-LAW") },
    { MULAW_AUDIO,         VM_STRING("MU-LAW") },
    { TWINVQ_AUDIO,        VM_STRING("TWINVQ") },
    { DTS_AUDIO,           VM_STRING("DTS") },
    { MPEG1_AUDIO,         VM_STRING("MPEG1") },
    { MPEG2_AUDIO,         VM_STRING("MPEG2") },
    { MPEG_AUDIO_LAYER1,   VM_STRING("MPEGxL1") },
    { MPEG_AUDIO_LAYER2,   VM_STRING("MPEGxL2") },
    { MPEG_AUDIO_LAYER3,   VM_STRING("MPEGxL3") },
    { MP1L1_AUDIO,         VM_STRING("MP1L1") },
    { MP1L2_AUDIO,         VM_STRING("MP1L2") },
    { MP1L3_AUDIO,         VM_STRING("MP1L3") },
    { MP2L1_AUDIO,         VM_STRING("MP2L1") },
    { MP2L2_AUDIO,         VM_STRING("MP2L2") },
    { MP2L3_AUDIO,         VM_STRING("MP2L3") },
    { VORBIS_AUDIO,        VM_STRING("VORBIS") },
    { AAC_AUDIO,           VM_STRING("AAC") },
    { AAC_MPEG4_STREAM,    VM_STRING("AAC_MP4") },
    { AMR_NB_AUDIO,        VM_STRING("ARM-NB") },
    { AMR_WB_AUDIO,        VM_STRING("ARM-WB") },
};

static const CodeStringTable StringsOfAudioSubType[] = {
    { UNDEF_AUDIO_SUBTYPE,  VM_STRING("UNDEF") },
    { AAC_LC_PROFILE,       VM_STRING("AAC_LC") },
    { AAC_LTP_PROFILE,      VM_STRING("AAC_LTP") },
    { AAC_MAIN_PROFILE,     VM_STRING("AAC_MAIN") },
    { AAC_SSR_PROFILE,      VM_STRING("AAC_SSR") },
    { AAC_HE_PROFILE,       VM_STRING("AAC_HE") },
    { AAC_ALS_PROFILE,      VM_STRING("AAC_ALS") },
    { AAC_BSAC_PROFILE,     VM_STRING("AAC_BSAC") },
};

static const CodeStringTable StringsOfVideoType[] = {
    { UNDEF_VIDEO,         VM_STRING("UNDEF") },
    { UNCOMPRESSED_VIDEO,  VM_STRING("UNCOMP.") },
    { MPEG1_VIDEO,         VM_STRING("MPEG1") },
    { MPEG2_VIDEO,         VM_STRING("MPEG2") },
    { MPEG4_VIDEO,         VM_STRING("MPEG4") },
    { H261_VIDEO,          VM_STRING("H261") },
    { H263_VIDEO,          VM_STRING("H263") },
    { H264_VIDEO,          VM_STRING("H264") },
    { DIGITAL_VIDEO_SD,    VM_STRING("DV_SD") },
    { DIGITAL_VIDEO_50,    VM_STRING("DV_50") },
    { DIGITAL_VIDEO_HD,    VM_STRING("DV_HD") },
    { DIGITAL_VIDEO_SL,    VM_STRING("DV_SL") },
    { WMV_VIDEO,           VM_STRING("WMV") },
    { MJPEG_VIDEO,         VM_STRING("MJPEG") },
    { VC1_VIDEO,           VM_STRING("VC1") },
    { AVS_VIDEO,           VM_STRING("AVS") },
    { VP8_VIDEO,           VM_STRING("VP8") },
};

static const CodeStringTable StringsOfVideoSubType[] = {
    { UNDEF_VIDEO_SUBTYPE,  VM_STRING("UNDEF") },
    { MPEG4_VIDEO_DIVX5,    VM_STRING("MPEG4_DIVX5") },
    { MPEG4_VIDEO_QTIME,    VM_STRING("MPEG4_QTIME") },
    { DIGITAL_VIDEO_TYPE_1, VM_STRING("DV_T1") },
    { DIGITAL_VIDEO_TYPE_2, VM_STRING("DV_T2") },
    { MPEG4_VIDEO_DIVX3,    VM_STRING("MPEG4_DIVX3") },
    { MPEG4_VIDEO_DIVX4,    VM_STRING("MPEG4_DIVX4") },
    { MPEG4_VIDEO_XVID,     VM_STRING("MPEG4_XVID") },
    { AVC1_VIDEO,           VM_STRING("AVC1") },
    { H263_VIDEO_SORENSON,  VM_STRING("H263_SORENSON") },
    { VC1_VIDEO_RCV,        VM_STRING("VC1_RCV") },
    { VC1_VIDEO_VC1,        VM_STRING("VC1") },
    { WVC1_VIDEO,           VM_STRING("WVC1") },
    { WMV3_VIDEO,           VM_STRING("WMV3") },
    { MULTIVIEW_VIDEO,      VM_STRING("MULTIVIEW") },
    { SCALABLE_VIDEO,       VM_STRING("SCALABLE") },
};

static const CodeStringTable StringsOfStreamType[] = {
    { UNDEF_STREAM,               VM_STRING("UNDEF") },
    { AVI_STREAM,                 VM_STRING("AVI") },
    { MP4_ATOM_STREAM,            VM_STRING("MP4ATOM") },
    { ASF_STREAM,                 VM_STRING("ASF") },
    { H26x_PURE_VIDEO_STREAM,     VM_STRING("H26x") },
    { H261_PURE_VIDEO_STREAM,     VM_STRING("H261PV") },
    { H263_PURE_VIDEO_STREAM,     VM_STRING("H263PV") },
    { H264_PURE_VIDEO_STREAM,     VM_STRING("H264PV") },
    { MPEGx_SYSTEM_STREAM,        VM_STRING("MPEGx") },
    { MPEG1_SYSTEM_STREAM,        VM_STRING("MPEG1") },
    { MPEG2_SYSTEM_STREAM,        VM_STRING("MPEG2") },
    { MPEG4_SYSTEM_STREAM,        VM_STRING("MPEG4") },
    { MPEGx_PURE_VIDEO_STREAM,    VM_STRING("MPEGxPV") },
    { MPEGx_PURE_AUDIO_STREAM,    VM_STRING("MPEGxPA") },
    { MPEGx_PES_PACKETS_STREAM,   VM_STRING("MPEGxPES") },
    { MPEGx_PROGRAMM_STREAM,      VM_STRING("MPEGxP") },
    { MPEGx_TRANSPORT_STREAM,     VM_STRING("MPEGxT") },
    { MPEG1_PURE_VIDEO_STREAM,    VM_STRING("MPEG1PV") },
    { MPEG1_PURE_AUDIO_STREAM,    VM_STRING("MPEG1PA") },
    { MPEG1_PES_PACKETS_STREAM,   VM_STRING("MPEG1PES") },
    { MPEG1_PROGRAMM_STREAM,      VM_STRING("MPEG1P") },
    { MPEG2_PURE_VIDEO_STREAM,    VM_STRING("MPEG2PV") },
    { MPEG2_PURE_AUDIO_STREAM,    VM_STRING("MPEG2PA") },
    { MPEG2_PES_PACKETS_STREAM,   VM_STRING("MPEG2PES") },
    { MPEG2_PROGRAMM_STREAM,      VM_STRING("MPEG2P") },
    { MPEG2_TRANSPORT_STREAM,     VM_STRING("MPEG2T") },
    { MPEG2_TRANSPORT_STREAM_TTS, VM_STRING("MPEG2TTS") },
    { MPEG4_PURE_VIDEO_STREAM,    VM_STRING("MPEG4PV") },
    { VC1_PURE_VIDEO_STREAM,      VM_STRING("VC1PV") },
    { WAVE_STREAM,                VM_STRING("WAVE") },
    { AVS_PURE_VIDEO_STREAM,      VM_STRING("AVSPV") },
    { FLV_STREAM,                 VM_STRING("FLV") },
    { IVF_STREAM,                 VM_STRING("IVF") },
    { MJPEG_STREAM,               VM_STRING("JPEG") },
    { ADTS_STREAM,                VM_STRING("ADTS") },
    { ADIF_STREAM,                VM_STRING("ADIF") }
//    { WEB_CAM_STREAM,                VM_STRING("WEB_CAM") },
};

static const sColorFormatInfo ColorFormatInfo[] =
{
    {YV12,        3,  8, 1, {{1, 1, 1, 2}, {2, 2, 1, 1}, {2, 2, 1, 1}}},
    {NV12,        2,  8, 2, {{1, 1, 1, 1}, {1, 2, 1, 1}, }},
    {YUY2,        1,  8, 2, {{2, 1, 4, 1}, }},
    {UYVY,        1,  8, 2, {{2, 1, 4, 1}, }},
    {YUV411,      3,  8, 1, {{1, 1, 1, 1}, {4, 1, 1, 1}, {4, 1, 1, 1}}},
    {YUV420,      3,  8, 1, {{1, 1, 1, 1}, {2, 2, 1, 1}, {2, 2, 1, 1}}},
    {YUV422,      3,  8, 1, {{1, 1, 1, 1}, {2, 1, 1, 1}, {2, 1, 1, 1}}},
    {YUV444,      3,  8, 1, {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}}},
    {Y411,        1,  8, 2, {{4, 1, 6, 1}}},
    {Y41P,        1,  8, 2, {{8, 1, 12, 1}}},
    {YUV420A,     4,  8, 1, {{1, 1, 1, 1}, {2, 2, 1, 1}, {2, 2, 1, 1}, {1, 1, 1, 1}}},
    {YUV422A,     4,  8, 1, {{1, 1, 1, 1}, {2, 1, 1, 1}, {2, 1, 1, 1}, {1, 1, 1, 1}}},
    {YUV444A,     4,  8, 1, {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}}},
    {YVU9,        3,  8, 1, {{1, 1, 1, 1}, {4, 4, 1, 1}, {4, 4, 1, 1}}},
    {GRAY,        1,  8, 1, {{1, 1, 1, 1}}},
    {GRAYA,       2,  8, 1, {{1, 1, 1, 1}, {1, 1, 1, 1}}},
    {RGB,         1,  8, 4, {{1, 1, 3, 1}}},
    {RGBA,        1,  8, 1, {{1, 1, 4, 1}}},
    {RGB565,      1, 16, 2, {{1, 1, 1, 1}}},
    {RGB555,      1, 16, 2, {{1, 1, 1, 1}}},
    {RGB444,      1, 16, 2, {{1, 1, 1, 1}}},
    {BGR,         1,  8, 4, {{1, 1, 3, 1}}},
    {BGRA,        1,  8, 1, {{1, 1, 4, 1}}},
    {BGR565,      1, 16, 2, {{1, 1, 1, 1}}},
    {BGR555,      1, 16, 2, {{1, 1, 1, 1}}},
    {BGR444,      1, 16, 2, {{1, 1, 1, 1}}},
    {CMYK,        1,  8, 1, {{1, 1, 4, 1}}},
    {CMYKA,       1,  8, 1, {{1, 1, 5, 1}}},
    {D3D_SURFACE, 0,  0, 0, {{0, 0, 0, 0}}},
    {LVA_SURFACE, 0,  0, 0, {{0, 0, 0, 0}}},
};

//////////////////////////////////////////////////////////////////////////////

static const vm_char* umcCodeToString(const CodeStringTable *table, int table_size, int code)
{
    for (int i = 0; i < table_size; i++)
    {
        if (table[i].code == code)
            return table[i].string;
    }
    return VM_STRING("UNDEF");
}

static int umcStringToCode(const CodeStringTable *table, int table_size, const vm_char* string)
{
    for (int i = 0; i < table_size; i++)
    {
        if (!vm_string_stricmp(table[i].string, string))
            return table[i].code;
    }
    return -1;
}

const sColorFormatInfo* GetColorFormatInfo(ColorFormat colorFormat)
{
    int iSize = sizeof(ColorFormatInfo)/sizeof(sColorFormatInfo);
    for (int i = 0; i < iSize; i++)
    {
        if (ColorFormatInfo[i].cFormat == colorFormat)
            return &ColorFormatInfo[i];
    }
    return NULL;
}

const vm_char* GetErrString(Status code)
{
    return UMC_CODE_TO_STRING(StringsOfError, code);
}
const vm_char* GetStreamTypeString(SystemStreamType code)
{
    return UMC_CODE_TO_STRING(StringsOfStreamType, code);
}
const vm_char* GetFormatTypeString(ColorFormat code)
{
    return UMC_CODE_TO_STRING(StringsOfFormatType, code);
}
const vm_char* GetAudioTypeString(AudioStreamType code)
{
    return UMC_CODE_TO_STRING(StringsOfAudioType, code);
}
const vm_char* GetAudioSubTypeString(AudioStreamSubType code)
{
    return UMC_CODE_TO_STRING(StringsOfAudioSubType, code);
}
const vm_char* GetVideoTypeString(VideoStreamType code)
{
    return UMC_CODE_TO_STRING(StringsOfVideoType, code);
}
const vm_char* GetVideoSubTypeString(VideoStreamSubType code)
{
    return UMC_CODE_TO_STRING(StringsOfVideoSubType, code);
}
const vm_char* GetVideoRenderTypeString(VideoRenderType code)
{
    return UMC_CODE_TO_STRING(StringsOfVideoRenderType, code);
}
const vm_char* GetAudioRenderTypeString(AudioRenderType code)
{
    return UMC_CODE_TO_STRING(StringsOfAudioRenderType, code);
}

ColorFormat GetFormatType(const vm_char *string)
{
    return UMC_STRING_TO_CODE(StringsOfFormatType, string, ColorFormat);
}
SystemStreamType GetStreamType(const vm_char *string)
{
    return UMC_STRING_TO_CODE(StringsOfStreamType, string, SystemStreamType);
}
AudioStreamType GetAudioType(const vm_char *string)
{
    return UMC_STRING_TO_CODE(StringsOfAudioType, string, AudioStreamType);
}
VideoStreamType GetVideoType(const vm_char *string)
{
    return UMC_STRING_TO_CODE(StringsOfVideoType, string, VideoStreamType);
}
AudioRenderType GetAudioRenderType(const vm_char *string)
{
    return UMC_STRING_TO_CODE(StringsOfAudioRenderType, string, AudioRenderType);
}
VideoRenderType GetVideoRenderType(const vm_char *string)
{
    return UMC_STRING_TO_CODE(StringsOfVideoRenderType, string, VideoRenderType);
}

};
