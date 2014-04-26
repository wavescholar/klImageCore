/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AC3_DECODER_H__
#define __AC3_DECODER_H__

#include "ippdefs.h"
#include "umc_audio_decoder.h"
#include "umc_dynamic_cast.h"

#include "ac3_dec_status.h"
#include "audio_codec_params.h"

struct _AC3Dec;

namespace UMC
{

// profile and level definitions for aac
// values for profile and level fields in BaseCodecParams
#define AC3_PROFILE_MAIN  0
#define AC3_LEVEL_MAIN    0

class AC3DecoderParams: public AudioDecoderParams
{
public:
    DYNAMIC_CAST_DECL(AC3DecoderParams, AudioDecoderParams)

    AC3DecoderParams(void)
    {
        out_acmod      = 0;
        outlfeon       = 0;
        dualmonomode   = 0;
        drc_scaleLow   = 1;
        drc_scaleHigh  = 1;
        out_compmod    = 0;
        karaokeCapable = 3;
        crc_mute       = 0;
        gainScale      = 1;
    };

    Ipp32s  dualmonomode;
    Ipp32s  out_acmod;
    Ipp32s  outlfeon;
    Ipp32s  out_compmod;
    Ipp32s  karaokeCapable;
    Ipp32s  crc_mute;
    Ipp32f  drc_scaleLow;
    Ipp32f  drc_scaleHigh;
    Ipp32f  gainScale;
};

class AC3Decoder: public AudioDecoder
{
public:
    DYNAMIC_CAST_DECL(AC3Decoder, AudioDecoder);

    AC3Decoder(void);
    virtual ~AC3Decoder(void);

    virtual Status  Init(BaseCodecParams * init);
    virtual Status  GetFrame(MediaData * in, MediaData * out);
    virtual Status  Close();
    virtual Status  Reset();
    virtual Status  GetInfo(BaseCodecParams * info);
    virtual Status  GetDuration(Ipp32f *p_duration);
    virtual Status  SetParams(BaseCodecParams * params);

protected:
    struct _AC3Dec *state;
    cAudioCodecParams params;
    Ipp64f  m_pts_prev;
    MemID  stateMemId;

    Status StatusAC3_2_UMC(AC3Status st);
    Status MemLock();
    Status MemUnlock();
};

}       // namespace UMC

#endif
