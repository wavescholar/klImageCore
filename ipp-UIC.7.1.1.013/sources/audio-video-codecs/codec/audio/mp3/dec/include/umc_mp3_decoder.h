/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __UMC_MP3DEC_FP_H__
#define __UMC_MP3DEC_FP_H__

#include "vm_types.h"
#include "audio_codec_params.h"
#include "mp3_status.h"
#include "umc_audio_decoder.h"

struct _MP3Dec;

namespace UMC
{

#define MP3_PROFILE_MPEG1  1
#define MP3_PROFILE_MPEG2  2

#define MP3_LEVEL_LAYER1   1
#define MP3_LEVEL_LAYER2   2
#define MP3_LEVEL_LAYER3   3

class MP3DecoderParams: public AudioDecoderParams
{
public:
    DYNAMIC_CAST_DECL(MP3DecoderParams, AudioDecoderParams)

    MP3DecoderParams(void)
    {
        mc_lfe_filter_off = 0;
        synchro_mode      = 0;
    };

    Ipp32s mc_lfe_filter_off;
    Ipp32s synchro_mode;
};

class MP3Decoder : public AudioDecoder
{
public:
    DYNAMIC_CAST_DECL(MP3Decoder, AudioDecoder)

    MP3Decoder(void);
    virtual ~MP3Decoder(void);

    virtual Status  Init(BaseCodecParams* init);
    virtual Status  Close(void);
    virtual Status  Reset(void);

    virtual Status  GetFrame(MediaData* in, MediaData* out);
    virtual Status  GetInfo(BaseCodecParams* info);
    virtual Status  SetParams(BaseCodecParams*)
    {
      return UMC_ERR_NOT_IMPLEMENTED;
    };

    virtual Status  GetDuration(Ipp32f* p_duration);

protected:
    struct _MP3Dec*   state;
    cAudioCodecParams params;
    Ipp64f            m_pts_prev;
    MemID             stateMemId;
    Ipp32s            m_mc_lfe_filter_off;
    Ipp32s            m_syncro_mode;
    Ipp32s            m_initialized;

    Status StatusMP3_2_UMC(MP3Status st);
    Status MemLock();
    Status MemUnlock();
};

}

#endif
