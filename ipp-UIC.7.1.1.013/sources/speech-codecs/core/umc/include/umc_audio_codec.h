/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AUDIO_CODEC_H__
#define __UMC_AUDIO_CODEC_H__

#include "umc_structures.h"
#include "umc_base_codec.h"
#include "umc_media_data.h"

namespace UMC
{

class AudioCodecParams : public BaseCodecParams
{
    DYNAMIC_CAST_DECL(AudioCodecParams, BaseCodecParams)
public:
    AudioCodecParams();

    AudioStreamInfo m_info_in;                                  // (AudioStreamInfo) original audio stream info
    AudioStreamInfo m_info_out;                                 // (AudioStreamInfo) output audio stream info

    Ipp32u m_frame_num;                                         // (Ipp32u) keeps number of processed frames
};

/******************************************************************************/

class AudioCodec : public BaseCodec
{
    DYNAMIC_CAST_DECL(AudioCodec, BaseCodec)

public:

    // Default constructor
    AudioCodec(void){};
    // Destructor
    virtual ~AudioCodec(void){};

    virtual Status GetDuration(Ipp32f *p_duration)
    {
        p_duration[0] = (Ipp32f)-1.0;
        return UMC_ERR_NOT_IMPLEMENTED;
    }

protected:

    Ipp32u m_frame_num;                                         // (Ipp32u) keeps number of processed frames.
};

/******************************************************************************/

class AudioData: public MediaData
{
    DYNAMIC_CAST_DECL(AudioData, MediaData)

public:
    AudioStreamInfo m_info;
    AudioData()
    {}

};

} // namespace UMC

#endif /* __UMC_AUDIO_CODEC_H__ */
