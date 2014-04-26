/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3_OWN_H__
#define __MP3_OWN_H__

#include "mp3_alloc_tab.h"
#include "vm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEN_MDCT_LINE 576

#define MP3_UPDATE_PTR(type, ptr, inc)    \
  if (ptr) {                              \
    ptr = (type *)((Ipp8u *)(ptr) + inc); \
  }

typedef struct {
    Ipp32s ext_bit_stream_present;                                // 4 bytes
    Ipp32s n_ad_bytes;                                            // 4 bytes
    Ipp32s center;                                                // 4 bytes
    Ipp32s surround;                                              // 4 bytes
    Ipp32s lfe;                                                   // 4 bytes
    Ipp32s audio_mix;                                             // 4 bytes
    Ipp32s dematrix_procedure;                                    // 4 bytes
    Ipp32s no_of_multi_lingual_ch;                                // 4 bytes
    Ipp32s multi_lingual_fs;                                      // 4 bytes
    Ipp32s multi_lingual_layer;                                   // 4 bytes
    Ipp32s copyright_identification_bit;                          // 4 bytes
    Ipp32s copyright_identification_start;                        // 4 bytes
} mp3_mc_header;                                                  // total 48 bytes

extern const Ipp32s mp3_bitrate[2][3][16];
extern const Ipp32s mp3_frequency[3][4];
extern const Ipp32s mp3_mc_pred_coef_table[6][16];
extern const Ipp8u mp3_mc_sb_group[32];

extern const Ipp32f mp3_lfe_filter[480];


Ipp32s mp3_SetAllocTable(Ipp32s header_id, Ipp32s mpg25, Ipp32s header_layer,
                         Ipp32s header_bitRate, Ipp32s header_samplingFreq,
                         Ipp32s stereo,
                         const Ipp32s **nbal_alloc_table,
                         const Ipp8u **alloc_table,
                         Ipp32s *sblimit);

#ifdef __cplusplus
}
#endif

#endif
