/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AC3_DEC_H__
#define __AC3_DEC_H__

#include "stdlib.h"
#include "audio_codec_params.h"
#include "ac3_dec_status.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum
  {
    AC3_CHANNEL_FRONT_LEFT      = 0x1,
    AC3_CHANNEL_FRONT_RIGHT     = 0x2,
    AC3_CHANNEL_FRONT_CENTER    = 0x4,
    AC3_CHANNEL_LOW_FREQUENCY   = 0x8,
    AC3_CHANNEL_BACK_LEFT       = 0x10,
    AC3_CHANNEL_BACK_RIGHT      = 0x20,
    AC3_CHANNEL_BACK_CENTER     = 0x100,
  } AC3ChannelMask;

  struct _AC3Dec;
  typedef struct _AC3Dec AC3Dec;

  AC3Status ac3decReset(AC3Dec *state);
  void ac3decUpdateMemMap(AC3Dec *state,
                          Ipp32s shift);
  AC3Status ac3decInit(AC3Dec *state_ptr,
                       Ipp32s *sizeAll);
  AC3Status ac3decClose(/* AC3Dec *state */);
  AC3Status ac3decGetFrame(Ipp8u *inPointer,
                           Ipp32s inDataSize,
                           Ipp32s *decodedBytes,
                           Ipp16s *outPointer,
                           Ipp32s outBufferSize,
                           AC3Dec *state);
  AC3Status ac3decGetDuration(Ipp32f *p_duration,
                              AC3Dec *state);
  AC3Status ac3decGetInfo(cAudioCodecParams *a_info,
                          AC3Dec *state);

  AC3Status ac3decSetOutAcmod(Ipp32s out_acmod, AC3Dec *state);
  AC3Status ac3decSetOuLfeOn(Ipp32s outlfeon, AC3Dec *state);
  AC3Status ac3decSetDualMonoMode(Ipp32s dualmonomode, AC3Dec *state);
  AC3Status ac3decSetDrcScaleLow(Ipp32f drc_scaleLow, AC3Dec *state);
  AC3Status ac3decSetDrcScaleHigh(Ipp32f drc_scaleHigh, AC3Dec *state);
  AC3Status ac3decSetOutCompMod(Ipp32s out_compmod, AC3Dec *state);
  AC3Status ac3decSetKaraokeCapable(Ipp32s karaokeCapable, AC3Dec *state);
  AC3Status ac3decSetCrcMute(Ipp32s crc_mute, AC3Dec *state);
  AC3Status ac3decSetGainScale(Ipp32f gainScale, AC3Dec *state);

  AC3Status ac3decGetNumChannelOut(Ipp32s *nChannelOut, AC3Dec *state);
  AC3Status ac3decGetSampleFrequency(Ipp32s *freq, AC3Dec *state);

#ifdef __cplusplus
}
#endif

#endif
