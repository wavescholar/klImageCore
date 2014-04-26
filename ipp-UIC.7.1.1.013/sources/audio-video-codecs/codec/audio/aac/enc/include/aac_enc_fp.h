/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AAC_ENC_FP_H__
#define __AAC_ENC_FP_H__

#include "aac_status.h"
#include "aaccmn_const.h"
#include "mp4cmn_pce.h"
#include "aac_enc_own.h"
#include "vm_debug.h"

#include "ipps.h"

#ifdef __cplusplus
extern "C" {
#endif

  struct _AACEnc;
  typedef struct _AACEnc AACEnc;

#define AAC_MONO 0
#define AAC_LR_STEREO 1
#define AAC_MS_STEREO 2
#define AAC_JOINT_STEREO 3

  void aacencUpdateMemMap(AACEnc *state,
                          Ipp32s shift);

  AACStatus aacencInit(AACEnc *real_state,
                       Ipp32s sampling_frequency,
                       Ipp32s chNum,
                       Ipp32s bit_rate,
                       enum AudioObjectType audioObjectType,
                       enum AudioObjectType auxAudioObjectType,
                       Ipp32s stereo_mode,
                       Ipp32s ns_mode,
                       Ipp32s *size_all);

  AACStatus aacencSetNumChannel(Ipp32s chNum,
                                AACEnc *state,
                                Ipp8u  *mem,
                                enum AudioObjectType audioObjectType,
                                enum AudioObjectType auxAudioObjectType,
                                Ipp32s *size_all);

  AACStatus aacencSetBitrate(Ipp32s bit_rate,
                             AACEnc *state);

  AACStatus aacencGetFrame(Ipp16s *inPointer,
                           Ipp32s *encodedBytes,
                           Ipp8u *outPointer,
                           AACEnc *state);

  AACStatus aacencFillProgramConfigElement(sProgram_config_element* p_data,
                                           Ipp32s element_instance_tag,
                                           AACEnc *state);

  AACStatus aacencClose(AACEnc *state);

  AACStatus aacencGetSampleFrequencyIndex(Ipp32s *freq_index,
                                          AACEnc *state);

  AACStatus aacencGetCrcSaveTable(sCrcSaveTable **crcSaveTable,
                                  AACEnc *state);

  AACStatus aacencGetNumEncodedFrames(Ipp32s *m_frame_number,
                                      AACEnc *state);

  AACStatus aacencGetDuration(Ipp32f *p_duration,
                              AACEnc *state);

#ifdef __cplusplus
}
#endif

#endif
