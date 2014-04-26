/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/
//  MPEG-2 is a international standard promoted by ISO/IEC and
//  other organizations. Implementations of this standard, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.

#ifndef __UMC_MPEG2_DEC_H
#define __UMC_MPEG2_DEC_H

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("MPEG-2 decoder"))

#include "umc_video_decoder.h"

namespace UMC
{

// profile and level definitions for mpeg2 video
// values for profile and level fields in BaseCodecParams
typedef enum
{
    MPEG2_PROFILE_SIMPLE  = 5,
    MPEG2_PROFILE_MAIN    = 4,
    MPEG2_PROFILE_SNR     = 3,
    MPEG2_PROFILE_SPATIAL = 2,
    MPEG2_PROFILE_HIGH    = 1
} MPEG2_PROFILE;

typedef enum
{
    MPEG2_LEVEL_LOW  = 10,
    MPEG2_LEVEL_MAIN = 8,
    MPEG2_LEVEL_H14  = 6,
    MPEG2_LEVEL_HIGH = 4
} MPEG2_LEVEL;


class MPEG2VideoDecoderBase;

class MPEG2VideoDecoder : public VideoDecoder
{
public:
    DYNAMIC_CAST_DECL(MPEG2VideoDecoder, VideoDecoder)

    ///////////////////////////////////////////////////////
    /////////////High Level public interface///////////////
    ///////////////////////////////////////////////////////
    MPEG2VideoDecoder(void);
    virtual ~MPEG2VideoDecoder(void);

    // Initialize for subsequent frame decoding.
    virtual Status Init(BaseCodecParams *init);

    // Get next frame
    virtual Status GetFrame(MediaData* in, MediaData* out);

    // Close  decoding & free all allocated resources
    virtual Status Close(void);

    // Reset decoder to initial state
    virtual Status Reset(void);

    // Get video stream information, valid after initialization
    virtual Status GetInfo(BaseCodecParams* info);

    //reset skip frame counter
    virtual Status    ResetSkipCount();

    // increment skip frame counter
    virtual Status    SkipVideoFrame(Ipp32s);

    // get skip frame counter statistic
    virtual Ipp32u    GetNumOfSkippedFrames();

    //access to the latest decoded frame
    virtual Status PreviewLastFrame(VideoData *out, BaseCodec *pPostProcessing = NULL);

    // returns closed capture data from gop user data
    virtual Status GetUserData(MediaData* pCC);

    virtual Status SetParams(BaseCodecParams* params);

#ifdef OVERLAY_SUPPORT
    virtual void   SwitchToExternalFrameBuffer(Ipp8u *ext_buffer[], Ipp32s bufstep[]);
    virtual void   SwitchToInternalFrameBuffer(void);
    virtual void*  GetCurrentFramePtr();
    virtual Ipp32s GetNumberOfBuffers();
#endif

protected:
    MPEG2VideoDecoderBase* m_pDec;
};

}

#endif
