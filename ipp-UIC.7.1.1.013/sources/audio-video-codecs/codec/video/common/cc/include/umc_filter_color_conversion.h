/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILTER_COLOR_CONVERSION_H__
#define __UMC_FILTER_COLOR_CONVERSION_H__

#include "umc_base_codec.h"

#include "ippi.h"

namespace UMC
{

class VideoColorConversionParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoColorConversionParams, BaseCodecParams)

    VideoColorConversionParams() {}
};

class VideoColorConversion : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoColorConversion, BaseCodec)

    VideoColorConversion();
    virtual ~VideoColorConversion();

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
    VideoColorConversionParams m_params;
    ColorFormat m_srcFormat;
    ColorFormat m_dstFormat;
    IppiSize    m_size;
    Ipp32u      m_iCopyMode;
    bool        m_bSrcSwapUV;
    bool        m_bDstSwapUV;
    bool        m_bUseIntermediate;
    bool        m_bStateInitialized;

    MediaData    *m_pInterData;
    MediaData    *m_pInterConvert; // additional intermediate buffer for complex conversions
};

}

#endif
