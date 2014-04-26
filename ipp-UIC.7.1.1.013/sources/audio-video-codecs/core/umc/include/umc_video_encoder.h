/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_VIDEO_ENCODER_H__
#define __UMC_VIDEO_ENCODER_H__

#include "umc_base_codec.h"

namespace UMC
{

enum // encoding flags
{
    // The encoder should reorder the incoming frames in the encoding order itself.
    FLAG_VENC_REORDER       = 0x00000004
};

class VideoEncoderParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoEncoderParams, BaseCodecParams)

    VideoEncoderParams(void)
    {
        m_iQuality = 51;
    }

    VideoStreamInfo m_info;     // video info
    Ipp32s          m_iQuality; // per cent, represent quantization precision
};

class VideoEncoder : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoEncoder, BaseCodec)

    VideoEncoder() {};
};

}

#endif
