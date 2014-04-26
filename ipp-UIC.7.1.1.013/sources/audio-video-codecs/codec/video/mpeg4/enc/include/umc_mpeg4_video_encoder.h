/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MPEG4_VIDEO_ENCODER_H__
#define __UMC_MPEG4_VIDEO_ENCODER_H__

#include "umc_defs.h"
//#pragma message (UMC_DEPRECATED_MESSAGE("MPEG-4 encoder"))

#include "mp4_enc.h"
#include "umc_video_encoder.h"
#include "umc_dynamic_cast.h"
#include "vm_strings.h"

namespace UMC
{

#define STR_LEN 511

class MPEG4EncoderParams : public VideoEncoderParams
{
public:
    DYNAMIC_CAST_DECL(MPEG4EncoderParams, VideoEncoderParams)

    MPEG4_ENC::mp4_Param  m_Param;

    MPEG4EncoderParams();

    virtual Status ReadParams(ParserCfg *par);
};

class MPEG4VideoEncoder: public VideoEncoder
{
public:
    DYNAMIC_CAST_DECL(MPEG4VideoEncoder, VideoEncoder)

    MPEG4VideoEncoder();
    ~MPEG4VideoEncoder();

    virtual Status Init(BaseCodecParams *init);
    virtual Status GetFrame(MediaData *in, MediaData *out);
    virtual Status GetInfo(BaseCodecParams *info);
    virtual Status Close();
    virtual Status Reset();
    virtual Status SetParams(BaseCodecParams* params);
    Ipp32s GetFrameMacroBlockPerRow() { return mp4enc->GetFrameMacroBlockPerRow(); }
    Ipp32s GetFrameMacroBlockPerCol() { return mp4enc->GetFrameMacroBlockPerCol(); }
    Ipp8u* GetFrameQuant() { return mp4enc->GetFrameQuant(); };
    IppMotionVector* GetFrameMVpred() { return mp4enc->GetFrameMVpred(); };
    Ipp32u* GetFrameMBpos() { return mp4enc->GetFrameMBpos(); };

    MPEG4_ENC::VideoEncoderMPEG4 *mp4enc;
    bool        m_IsInit;
    Ipp32s      m_FrameCount;

protected:
    MPEG4EncoderParams  m_Param;
    Ipp64f             *bTime, gTime, iTime;
    Ipp32s              bTimePos;
    // allocate memory for internal buffers
    Status AllocateBuffers();
    // free memory for internal buffers
    Status FreeBuffers();
    // lock memory for internal buffers
    void LockBuffers();
    // unlock memory for internal buffers
    Status UnlockBuffers();
};

} //namespace UMC

#endif /* __UMC_MPEG4_VIDEO_ENCODER_H__ */
