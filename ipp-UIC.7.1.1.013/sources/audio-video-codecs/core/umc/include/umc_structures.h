/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_STRUCTURES_H__
#define __UMC_STRUCTURES_H__

#include "vm_types.h"
#include "umc_defs.h"
#include "umc_video_data.h"
#include "umc_audio_data.h"

namespace UMC
{

class StreamInfo
{
public:
    DYNAMIC_CAST_DECL_BASE(StreamInfo)

    StreamInfo()
    {
        iProfile   = 0;
        iLevel     = 0;
        iStreamPID = 0;
        iBitrate   = 0;
        fDuration  = 0;
    }

    virtual ~StreamInfo() {};

    Ipp32s iProfile;   // profile
    Ipp32s iLevel;     // level
    Ipp32u iStreamPID; // unique ID
    Ipp32u iBitrate;   // bitstream in bps
    Ipp64f fDuration;  // duration of the stream
};

class AudioStreamInfo : public StreamInfo
{
public:
    DYNAMIC_CAST_DECL(AudioStreamInfo, StreamInfo)

    AudioStreamInfo()
    {
        streamType       = UNDEF_AUDIO;
        streamSubtype    = UNDEF_AUDIO_SUBTYPE;
        iHeader          = 0;
        iLanguage        = 0;
        bProtected       = false;
    }

    AudioData          audioInfo;        // audio info template
    AudioStreamType    streamType;       // general type of stream
    AudioStreamSubType streamSubtype;    // minor type of stream
    Ipp32u             iHeader;          // can carry audio header (4-bytes)
    Ipp32u             iLanguage;        // language code
    bool               bProtected;       // audio is encrypted
};

class VideoStreamInfo : public StreamInfo
{
public:
    DYNAMIC_CAST_DECL(VideoStreamInfo, StreamInfo)

    VideoStreamInfo()
    {
        streamType    = UNDEF_VIDEO;
        streamSubtype = UNDEF_VIDEO_SUBTYPE;
        iViews        = 1;
        fFramerate    = 30;
        videoInfo.m_colorFormat = YUV420;
    }

    VideoData           videoInfo;      // video info template
    VideoStreamType     streamType;     // video stream type
    VideoStreamSubType  streamSubtype;  // video stream type
    Ipp32u              iViews;         // number of mixed views for multiview streams (e.g. stereo)
    Ipp64f              fFramerate;     // frame rate of video
};

// forward declaration of template
template<class T> inline T align_pointer(void *pv, size_t lAlignValue = DEFAULT_ALIGN_VALUE)
{
    // some compilers complain to conversion to/from
    // pointer types from/to integral types.
    return (T)((((size_t) (pv)) + (lAlignValue - 1)) & ~(lAlignValue - 1));
}

// forward declaration of template
template<class T> inline T align_value(size_t nValue, size_t lAlignValue = DEFAULT_ALIGN_VALUE)
{
    return (T)((nValue + (lAlignValue - 1)) & ~(lAlignValue - 1));
}

const vm_char* GetErrString(Status ErrCode);
const vm_char* GetStreamTypeString(SystemStreamType Code);
const vm_char* GetFormatTypeString(ColorFormat Code);
const vm_char* GetAudioTypeString(AudioStreamType Code);
const vm_char* GetAudioSubTypeString(AudioStreamSubType Code);
const vm_char* GetVideoTypeString(VideoStreamType Code);
const vm_char* GetVideoSubTypeString(VideoStreamSubType Code);
const vm_char* GetVideoRenderTypeString(VideoRenderType Code);
const vm_char* GetAudioRenderTypeString(AudioRenderType Code);

ColorFormat      GetFormatType(const vm_char *string);
SystemStreamType GetStreamType(const vm_char *string);
AudioStreamType  GetAudioType(const vm_char *string);
VideoStreamType  GetVideoType(const vm_char *string);
AudioRenderType  GetAudioRenderType(const vm_char *string);
VideoRenderType  GetVideoRenderType(const vm_char *string);

} // namespace UMC

#endif /* __UMC_STRUCTURES_H__ */
