/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_VIDEO_DECODER_H__
#define __UMC_VIDEO_DECODER_H__

#include "umc_structures.h"
#include "umc_video_data.h"
#include "umc_base_codec.h"

namespace UMC
{

enum // decoding flags
{
    // Init() must only check whether stream is supported
    // (looking to specified video header)
    // and not allocate any resources
    FLAG_VDEC_CHECK_ONLY      = 0x00000002,

    //receiving this flag decoder must output decompressed data
    //in proper display order, otherwise it will output decompressed data
    //in decoding order, application is responsible to reorder frames to
    //before displaying
    FLAG_VDEC_REORDER         = 0x00000004,

    //next flag describes endian related properties of input data
    //when set, means that coded data should be accessed by 4-reading operations
    //for little-endian systems it means that each 4 bytes are swapped
    //i.e [0]<->[3], [1]<->[2]
    //for big-endian systems swapping is not required
    FLAG_VDEC_4BYTE_ACCESS    = 0x00000100
};

class VideoDecoderParams : public BaseCodecParams
{
public:
    DYNAMIC_CAST_DECL(VideoDecoderParams, BaseCodecParams)

    VideoDecoderParams(void)
    {
        m_iTrickModes       = UMC_TRICK_MODES_NO;
        m_fPlaybackRate     = 1;
        m_pPostProcessor    = NULL;
    }

    BaseCodec      *m_pPostProcessor;   // pointer to post processor
    VideoStreamInfo m_info;             // compressed video info
    Ipp32u          m_iTrickModes;      // trick modes
    Ipp64f          m_fPlaybackRate;
};

class VideoDecoder: public BaseCodec
{
public:
    DYNAMIC_CAST_DECL(VideoDecoder, BaseCodec)

    VideoDecoder(void)
    {
        m_pPostProcessor    = NULL;
    }
    virtual ~VideoDecoder(void) {}

    // Additional methods
    // Reset skip frame counter
    virtual Status ResetSkipCount(void) = 0;

    // Increment skip frame counter
    virtual Status SkipVideoFrame(Ipp32s) = 0;

    // Get skip frame counter statistic
    virtual Ipp32u GetNumOfSkippedFrames(void) = 0;

    // Preview last decoded frame
    virtual Status PreviewLastFrame(VideoData* out)
    {
        if(!m_pPostProcessor)
            return UMC_ERR_NULL_PTR;

        return m_pPostProcessor->GetFrame(&m_lastDecodedFrame, out);
    }

    // returns closed capture data
    virtual Status GetUserData(MediaData* /*pCC*/)
    {
        return UMC_ERR_NOT_IMPLEMENTED;
    }

protected:
    VideoData        m_lastDecodedFrame;  // last decoded frame
    BaseCodec       *m_pPostProcessor;    // pointer to post processing

private:
    const VideoDecoder& operator=(const VideoDecoder&) { return *this; }
    VideoDecoder(const VideoDecoder&) {}
};

}

#endif
