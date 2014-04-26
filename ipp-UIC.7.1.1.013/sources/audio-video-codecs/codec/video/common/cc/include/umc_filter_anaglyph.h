/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_FILTER_ANAGLYPH_H__
#define __UMC_FILTER_ANAGLYPH_H__

#include "umc_base_codec.h"

namespace UMC
{

class VideoAnaglyphParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoAnaglyphParams, BaseCodecParams)

    VideoAnaglyphParams()
    {
        m_bEnabled = false;
    }

    bool m_bEnabled;
};

class VideoAnaglyph : public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoAnaglyph, BaseCodec)

    VideoAnaglyph(void);
    virtual ~VideoAnaglyph(void);

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

private:
    VideoAnaglyphParams m_params;

    MediaData *m_pInDataPair;
    bool m_bPairReady;
    bool m_bUseIntermediate;
    bool m_bStateInitialized;

    MediaData *m_pInterData;
};

} // namespace UMC

#endif /* __UMC_FILTER_ANAGLYPH_H__ */
