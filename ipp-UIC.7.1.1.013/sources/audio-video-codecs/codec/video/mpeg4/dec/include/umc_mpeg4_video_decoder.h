/* ////////////////////////////////////////////////////////////////////////// */
/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_MPEG4_VIDEO_DECODER_H__
#define __UMC_MPEG4_VIDEO_DECODER_H__

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("MPEG-4 decoder"))

#include "umc_structures.h"
#include "umc_video_decoder.h"
#include "mp4.h"

namespace UMC
{

#define MPEG4_PROFILE_SIMPLE                      0
#define MPEG4_PROFILE_SIMPLE_SCALABLE             1
#define MPEG4_PROFILE_CORE                        2
#define MPEG4_PROFILE_MAIN                        3
#define MPEG4_PROFILE_NBIT                        4
#define MPEG4_PROFILE_SCALABLE_TEXTURE            5
#define MPEG4_PROFILE_SIMPLE_FACE                 6
#define MPEG4_PROFILE_BASIC_ANIMATED_TEXTURE      7
#define MPEG4_PROFILE_HYBRID                      8
#define MPEG4_PROFILE_ADVANCED_REAL_TIME_SIMPLE   9
#define MPEG4_PROFILE_CORE_SCALABLE              10
#define MPEG4_PROFILE_ADVANCED_CODE_EFFICIENCY   11
#define MPEG4_PROFILE_ADVANCED_CORE              12
#define MPEG4_PROFILE_ADVANCED_SCALABLE_TEXTURE  13
#define MPEG4_PROFILE_STUDIO                     14
#define MPEG4_PROFILE_ADVANCED_SIMPLE            15
#define MPEG4_PROFILE_FGS                        16

#define MPEG4_LEVEL_0    0
#define MPEG4_LEVEL_1    1
#define MPEG4_LEVEL_2    2
#define MPEG4_LEVEL_3    3
#define MPEG4_LEVEL_4    4
#define MPEG4_LEVEL_5    5
#define MPEG4_LEVEL_3B   13


class MPEG4VideoDecoder : public VideoDecoder
{
public:
    DYNAMIC_CAST_DECL(MPEG4VideoDecoder, VideoDecoder)

    // Default constructor
    MPEG4VideoDecoder(void);
    // Destructor
    virtual ~MPEG4VideoDecoder(void);
    // Initialize for subsequent frame decoding.
    virtual Status Init(BaseCodecParams *init);
    // Get next frame
    virtual Status GetFrame(MediaData* in, MediaData* out);
    // Close decoding & free all allocated resources
    virtual Status Close(void);
    // Reset decoder to initial state
    virtual Status  Reset(void);
    // Get decoder info
    virtual Status GetInfo(BaseCodecParams* info);
    // Get performance
    Status GetPerformance(Ipp64f *perf);
    // reset skip frame counter
    Status  ResetSkipCount();
    // increment skip frame counter
    Status  SkipVideoFrame(Ipp32s);
    // get skip frame counter statistic
    Ipp32u GetNumOfSkippedFrames();
    // get pointers to internal current frame
    //mp4_Frame* GetCurrentFramePtr(void);
    // Set post-processing deblocking parameters
    Status SetDeblockingParams(bool procPlane0, bool procPlane1, bool procPlane2, Ipp32s THR1 = 2, Ipp32s THR2 = 6);
    // Set post-processing deringing parameters
    Status SetDeringingParams(bool procPlane0, bool procPlane1, bool procPlane2);

protected:
    VideoStreamInfo         m_ClipInfo;
    bool                    m_IsInit, m_IsReset, m_IsInitBase;
    Ipp64f                  m_dec_time, m_dec_time_prev, m_dec_time_base, m_dec_time_fr, m_dec_time_frinc;
    Ipp32s                  m_buffered_frame, m_is_skipped_b, m_skipped_fr, m_b_prev;
    Ipp32s                  m_time_reset;
    VideoDecoderParams      m_Param;
    mp4_Info               *m_decInfo;
    // post-processing
    Ipp32s m_DeblockingTHR1, m_DeblockingTHR2;
    bool   m_DeblockingProcPlane[3], m_DeringingProcPlane[3];
    mp4_Frame ppFrame0, ppFrame1;
    Status PostProcess(mp4_Frame *inout);
    // internal init after VOL parsing, memory allocation, etc
    Status InsideInit();
    // allocate frame memory
    Status AllocateInitFrame(mp4_Frame* pFrame);
    // lock frame memory
    void LockFrame(mp4_Frame* pFrame);
    // allocate memory for internal buffers
    Status AllocateBuffers();
    // free memory for internal buffers
    Status FreeBuffers();
    // lock memory for internal buffers
    void LockBuffers();
    // unlock memory for internal buffers
    Status UnlockBuffers();
private:
    const MPEG4VideoDecoder& operator=(const MPEG4VideoDecoder&) { return *this; }
    MPEG4VideoDecoder(const MPEG4VideoDecoder&) {}
};

} // end namespace UMC

#endif // __UMC_MPEG4_VIDEO_DECODER_H__
