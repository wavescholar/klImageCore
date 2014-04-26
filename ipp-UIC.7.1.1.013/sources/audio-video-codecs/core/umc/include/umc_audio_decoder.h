/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AUDIO_DECODER_H__
#define __UMC_AUDIO_DECODER_H__

#include "umc_structures.h"
#include "umc_base_codec.h"
#include "umc_media_data.h"

namespace UMC
{

class AudioDecoderParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(AudioDecoderParams, BaseCodecParams)

    AudioDecoderParams(void) {}

    AudioStreamInfo m_info;   // audio stream info
};

class AudioDecoder : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(AudioDecoder, BaseCodec)

    AudioDecoder(void)
    {
        m_frame_num = 0;
    };

    virtual Status GetDuration(Ipp32f *pDuration)
    {
        pDuration[0] = (Ipp32f)-1.0;
        return UMC_ERR_NOT_IMPLEMENTED;
    }

protected:
    Ipp32u m_frame_num;
};

}

#endif
