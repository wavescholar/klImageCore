/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_MP3_AUDIO_ENCODER)

#include "mp3enc_own.h"

Ipp32s mp3enc_checkBitRate(Ipp32s id, Ipp32s layer, Ipp32s stereo, Ipp32s br, Ipp32s *ind)
{
  Ipp32s i;

  if (br == 0)
    return 0;

  if (layer == 1 && br == 32)
    return 0;

  if (layer == 2 && stereo == 1 && br > 192)
    return 0;

  for (i = 0; i < 15; i++) {
    if (mp3_bitrate[id][layer-1][i] == br) {
      *ind = i;
      return 1;
    }
  }

  return 0;
}

MP3Status mp3encGetFrameSize(Ipp32s *frameSize, Ipp32s id, Ipp32s layer, Ipp32s upsample)
{
  static const VM_ALIGN32_DECL(Ipp32s) fs[2][4] = {
    { 0, 384, 1152,  576 },
    { 0, 384, 1152, 1152 }
  };

  if (!frameSize)
    return MP3_NULL_PTR;

  *frameSize = fs[id][layer];

  if (upsample)
    *frameSize >>= upsample;

  return MP3_OK;
}

Ipp32s mp3encLEBitrate(MP3Enc_com *state, Ipp32s slot_size)
{
  Ipp32s i;
  for (i = 2; i <= 14; i++) {
    if (slot_size < state->slot_sizes[i])
      return i - 1;
  }

  return 14;
}

Ipp32s mp3encGEBitrate(MP3Enc_com *state, Ipp32s slot_size)
{
  Ipp32s i;
  for (i = 13; i >= 1; i--) {
    if (slot_size > state->slot_sizes[i])
      return i + 1;
  }

  return 1;
}

#endif //UMC_ENABLE_XXX
