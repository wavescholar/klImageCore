/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MP3_ENCODER_H__
#define __UMC_MP3_ENCODER_H__

#include "umc_media_data.h"
#include "umc_audio_encoder.h"
#include "audio_codec_params.h"
#include "mp3_status.h"

struct _MP3Enc;

namespace UMC
{

class MP3EncoderParams: public AudioEncoderParams
{
public:
    DYNAMIC_CAST_DECL(MP3EncoderParams, AudioEncoderParams)

    MP3EncoderParams(void)
    {
        stereo_mode         = UMC_MPA_LR_STEREO;
        layer               = 3;
        mode                = UMC_MPAENC_CBR;
        ns_mode             = 0;
        force_mpeg1         = 0;
        mc_matrix_procedure = 0;
        mc_lfe_filter_off   = 0;
    };

    enum UMC_MP3StereoMode stereo_mode;
    Ipp32s layer;
    Ipp32s mode;
    Ipp32s ns_mode;
    Ipp32s force_mpeg1;
    Ipp32s mc_matrix_procedure;
    Ipp32s mc_lfe_filter_off;
};

/* /////////////////////////////////////////////////////////////////////////////
//  Class:       MP3Encoder
//
//  Notes:       Implementation of MPEG-I layer 3 encoder.
//
*/
class MP3Encoder: public AudioEncoder
{
public:
    DYNAMIC_CAST_DECL(MP3Encoder, AudioEncoder)

    MP3Encoder(void);
    virtual ~MP3Encoder(void);

    virtual Status  Init(BaseCodecParams * init);
    virtual Status  Close();

    virtual Status  GetFrame(MediaData * in, MediaData * out);

    virtual Status  GetInfo(BaseCodecParams * init);
    virtual Status  GetDuration(Ipp32f *p_duration);

    virtual Status  SetParams(BaseCodecParams * params);
    virtual Status  Reset();

    virtual bool CheckBitRate(Ipp32s sample_rate,
                         Ipp32s layer,
                         Ipp32s bitrate,
                         Ipp32s stereo);

protected:
    struct _MP3Enc  *state;
    Ipp32s  m_stereo;
    Ipp32s  m_freq;
    Ipp32s  m_layer;
    Ipp32s  m_bitrate;
    Ipp32s  m_br_mode;
    Ipp32s  m_stereo_mode;
    Ipp32s  m_ns_mode;
    Ipp32s  m_id;
    Ipp32s  m_frame_size;
    Ipp32s  m_upsample;
    Ipp32s  m_force_mpeg1;
    Ipp32s  m_mc_matrix_procedure;
    Ipp32s  m_mc_lfe_filter_off;
    Ipp64f  m_pts_prev;
    MemID  stateMemId;
    Ipp32s m_initialized;

    Status StatusMP3_2_UMC(MP3Status st);
    Status MemLock();
    Status MemUnlock();
};

};

#endif
