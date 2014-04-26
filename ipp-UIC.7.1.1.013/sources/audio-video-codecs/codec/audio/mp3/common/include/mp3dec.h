/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3DEC_H__
#define __MP3DEC_H__

#include "audio_codec_params.h"
#include "mp3_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  MP3_CHANNEL_STEREO             = 0x2,
  MP3_CHANNEL_CENTER             = 0x4,
  MP3_CHANNEL_SURROUND_MONO      = 0x8,
  MP3_CHANNEL_SURROUND_STEREO    = 0x10,
  MP3_CHANNEL_SURROUND_STEREO_P2 = 0x20,
  MP3_CHANNEL_LOW_FREQUENCY      = 0x40
} MP3ChannelMask;

struct _MP3Dec;
typedef struct _MP3Dec MP3Dec;

MP3Status mp3decUpdateMemMap(MP3Dec *state, Ipp32s shift);
MP3Status mp3decInit(MP3Dec *state_ptr, Ipp32s mc_lfe_filter_off, Ipp32s synchro_mode, Ipp32s *size_all);
MP3Status mp3decClose(MP3Dec *state);
MP3Status mp3decGetInfo(cAudioCodecParams *a_info, MP3Dec *state);
MP3Status mp3decGetDuration(Ipp32f *p_duration, MP3Dec *state);
MP3Status mp3decReset(MP3Dec *state);
MP3Status mp3decGetFrame(Ipp8u *inPointer, Ipp32s inDataSize, Ipp32s *decodedBytes,
                         Ipp16s *outPointer, Ipp32s outBufferSize, MP3Dec *state);
MP3Status mp3decGetSampleFrequency(Ipp32s *freq, MP3Dec *state);
MP3Status mp3decGetFrameSize(Ipp32s *frameSize, MP3Dec *state);
MP3Status mp3decGetChannels(Ipp32s *ch, Ipp32s *ch_mask, MP3Dec *state);

MP3Status mp3idecUpdateMemMap(MP3Dec *state, Ipp32s shift);
MP3Status mp3idecInit(MP3Dec *state_ptr, Ipp32s synchro_mode, Ipp32s *size_all);
MP3Status mp3idecClose(MP3Dec *state);
MP3Status mp3idecGetInfo(cAudioCodecParams *a_info, MP3Dec *state);
MP3Status mp3idecGetDuration(Ipp32f *p_duration, MP3Dec *state);
MP3Status mp3idecReset(MP3Dec *state);
MP3Status mp3idecGetFrame(Ipp8u *inPointer, Ipp32s inDataSize, Ipp32s *decodedBytes,
                          Ipp16s *outPointer, Ipp32s outBufferSize, MP3Dec *state);
MP3Status mp3idecGetSampleFrequency(Ipp32s *freq, MP3Dec *state);
MP3Status mp3idecGetFrameSize(Ipp32s *frameSize, MP3Dec *state);
MP3Status mp3idecGetChannels(Ipp32s *ch, Ipp32s *ch_mask, MP3Dec *state);

#ifdef __cplusplus
}
#endif

#endif
