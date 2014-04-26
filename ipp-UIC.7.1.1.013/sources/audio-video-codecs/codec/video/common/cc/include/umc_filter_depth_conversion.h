/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILTER_DEPTH_CONVERSION_H__
#define __UMC_FILTER_DEPTH_CONVERSION_H__

#include "umc_base_codec.h"

#include "ippi.h"

namespace UMC
{

class VideoDepthConversionParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoDepthConversionParams, BaseCodecParams)

    VideoDepthConversionParams() {}
};

class VideoDepthConversion : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoDepthConversion, BaseCodec)

    VideoDepthConversion();
    virtual ~VideoDepthConversion();

    // Initialize codec with specified parameter(s)
    virtual Status Init(BaseCodecParams *);

    // Convert next frame
    virtual Status GetFrame(MediaData *in, MediaData *out);

    // Get codec working (initialization) parameter(s)
    virtual Status GetInfo(BaseCodecParams *) { return UMC_OK; };

    // Close all codec resources
    virtual Status Close(void);

    // Set codec to initial state
    virtual Status Reset(void) { return UMC_OK; };

private:
    VideoDepthConversionParams m_params;
    ColorFormat m_srcFormat;
    ColorFormat m_dstFormat;
    IppiSize    m_size;
    bool        m_bUseIntermediate;
    bool        m_bStateInitialized;

    MediaData    *m_pInterData;
};

}

#endif
