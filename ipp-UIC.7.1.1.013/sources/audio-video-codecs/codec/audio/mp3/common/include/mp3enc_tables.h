/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2002-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __MP3ENC_TABLES_H__
#define __MP3ENC_TABLES_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _scalefac_struct
{
  VM_ALIGN32_DECL(Ipp32u) l[23];
  VM_ALIGN32_DECL(Ipp32u) s[14];
  VM_ALIGN32_DECL(Ipp32u) si[40];
} scalefac_struct;

extern const scalefac_struct mp3enc_sfBandIndex[2][3];

typedef Ipp16s IXS[192][3];

extern const Ipp32s mp3enc_slen1_tab[16];
extern const Ipp32s mp3enc_slen2_tab[16];
extern const Ipp32s mp3enc_pretab[21];

extern const Ipp32s mp3enc_region01_table[23][2];

#ifdef __cplusplus
}
#endif

#endif  //  __MP3ENC_TABLES_H__
