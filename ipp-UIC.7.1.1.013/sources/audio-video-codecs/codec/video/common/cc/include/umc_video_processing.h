/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_VIDEO_PROCESSING_H__
#define __UMC_VIDEO_PROCESSING_H__

#include "umc_video_data.h"
#include "umc_base_codec.h"
#include "umc_list.h"

#include "umc_filter_color_conversion.h"
#include "umc_filter_depth_conversion.h"
#include "umc_filter_deinterlace.h"
#include "umc_filter_resize.h"
#include "umc_filter_anaglyph.h"

namespace UMC
{

class VideoProcessingParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoProcessingParams, BaseCodecParams)

    VideoProcessingParams() {}
};


class VideoProcessing : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoProcessing, BaseCodec)

    VideoProcessing(void);
    virtual ~VideoProcessing(void);

    // Initialize codec with specified parameter(s)
    virtual Status Init(BaseCodecParams *);

    // Convert frame
    virtual Status GetFrame(MediaData *in, MediaData *out);

    // Get codec working (initialization) parameter(s)
    virtual Status GetInfo(BaseCodecParams *) { return UMC_OK; };

    // Close all codec resources
    virtual Status Close(void);

    // Set codec to initial state
    virtual Status Reset(void) { return UMC_OK; };

    virtual Status AddFilter(VideoFilter iFilter, BaseCodecParams *filterParams);

protected:
    VideoProcessingParams m_params;
    List<BaseCodec*>      m_filtersChain;
    Ipp32u                m_iNumFilters;
};

} // namespace UMC

#endif /* __UMC_VIDEO_PROCESSING_H__ */
