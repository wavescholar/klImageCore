/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_AAC_ENCODER_H__
#define __UMC_AAC_ENCODER_H__

#include "umc_media_data.h"
#include "umc_audio_encoder.h"

#include "aaccmn_const.h"

struct _AACEnc;

namespace UMC
{

class AACEncoderParams: public AudioEncoderParams
{
public:
    DYNAMIC_CAST_DECL(AACEncoderParams, AudioEncoderParams)

    AACEncoderParams(void)
    {
        audioObjectType    = AOT_AAC_LC;
        auxAudioObjectType = AOT_UNDEF;

        stereo_mode  = UMC_AAC_LR_STEREO;
        outputFormat = UMC_AAC_ADTS;
        ns_mode = 0;
    };

    enum AudioObjectType audioObjectType;
    enum AudioObjectType auxAudioObjectType;

    enum UMC_AACStereoMode  stereo_mode;
    enum UMC_AACOuputFormat outputFormat;
    Ipp32s ns_mode;
};

/* /////////////////////////////////////////////////////////////////////////////
//  Class:       AACEncoder
//
//  Notes:       Implementation of AAC encoder.
//
*/

class AACEncoder : public AudioEncoder
{
public:
    DYNAMIC_CAST_DECL(AACEncoder, AudioEncoder)

    AACEncoder(void);
    virtual ~AACEncoder(void);
    
    virtual Status Init(BaseCodecParams* init);
    virtual Status Close();

    virtual Status GetFrame(MediaData* in, MediaData* out);

    virtual Status GetInfo(BaseCodecParams* init);

    virtual Status SetParams(BaseCodecParams* params) { params = params; return UMC_ERR_NOT_IMPLEMENTED;};
    virtual Status Reset() { return UMC_ERR_NOT_IMPLEMENTED;};
    virtual Status GetDuration(Ipp32f* p_duration);

    bool CheckBitRate(Ipp32s br, Ipp32s& idx);

protected:
    struct _AACEnc *state;
    Ipp32s m_channel_number;
    Ipp32s m_sampling_frequency;
    Ipp32s m_bitrate;
    Ipp32s m_adtsProfile;
    Ipp32s m_adtsID;
    enum AudioObjectType    m_audioObjectType;
    enum UMC_AACOuputFormat m_outputFormat;

  /* HEAAC profile: auxiliary AOT */
    enum AudioObjectType m_auxAudioObjectType;

    MemID  stateMemId;

    Status MemLock();
    Status MemUnlock();

    Ipp64f m_pts_prev;
};

}// namespace UMC

#endif /* __UMC_AAC_ENCODER_H__ */

