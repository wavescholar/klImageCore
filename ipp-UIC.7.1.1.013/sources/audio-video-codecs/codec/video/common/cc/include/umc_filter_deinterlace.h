/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILTER_DEINTERLACE_H__
#define __UMC_FILTER_DEINTERLACE_H__

#include "umc_base_codec.h"

#include "ippi.h"

namespace UMC
{

enum DeinterlacingType
{
    DEINTERLACING_DUPLICATE        = 1,
    DEINTERLACING_BLEND            = 2,
    DEINTERLACING_MEDIAN           = 3,
    DEINTERLACING_EDGE_DETECT      = 4,
    DEINTERLACING_MEDIAN_THRESHOLD = 5,
    DEINTERLACING_CAVT             = 6
};

enum FieldsMode
{
    FIELDS_LEAVE   = 0,    // do nothing
    FIELDS_COMBINE_STRICT, // combine separate fields into interlaced frame, wait for proper field pair
    FIELDS_COMBINE_SOFT,   // always consider next field as pair
    FIELDS_EXPAND,         // expand fields to full frame size
};

class VideoDeinterlaceParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoDeinterlaceParams, BaseCodecParams)

    VideoDeinterlaceParams()
    {
        m_deinterlacingType = DEINTERLACING_DUPLICATE;
        m_fieldsMode        = FIELDS_COMBINE_SOFT;
        m_iThreshold        = 0;
        m_bEnabled          = false;
    }

    DeinterlacingType m_deinterlacingType;
    FieldsMode        m_fieldsMode;
    Ipp16u            m_iThreshold;
    bool              m_bEnabled;
};

class VideoDeinterlace : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoDeinterlace, BaseCodec)

    VideoDeinterlace();
    virtual ~VideoDeinterlace();

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

    // Join separate fields together
    Status CombineFields(MediaData *input, MediaData *output);

protected:
    VideoDeinterlaceParams m_params;

    PictureStructure m_pairStructure;
    PictureStructure m_prevStructure;
    bool        m_bUseIntermediate;
    bool        m_bStateInitialized;
    bool        m_bHaveField;

    MediaData    *m_pInterData;
    MediaData    *m_pInDataPair;
};

}

#endif
