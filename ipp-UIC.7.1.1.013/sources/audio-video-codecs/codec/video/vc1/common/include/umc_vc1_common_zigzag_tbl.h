/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Zig-zag scan tables
//
*/

#ifndef __UMC_VC1_COMMON_ZIGZAG_TBL_H__
#define __UMC_VC1_COMMON_ZIGZAG_TBL_H__

//VC-1 Table 234: Intra Normal Scan
//remapped as src index for continues dst index
extern const  Ipp8u VC1_Intra_Normal_Scan_chroma[64];
extern const  Ipp8u VC1_Intra_Normal_Scan_luma[64];

//VC-1 Table 235: Intra Horizontal Scan
//remapped as src index for continues dst index
extern const  Ipp8u VC1_Intra_Horizontal_Scan_chroma[64];
extern const  Ipp8u VC1_Intra_Horizontal_Scan_luma[64];

//VC-1 Table 236: Intra Vertical Scan
//remapped as src index for continues dst index
extern const Ipp8u VC1_Intra_Vertical_Scan_chroma[64];
extern const Ipp8u VC1_Intra_Vertical_Scan_luma[64];

//Table 237: Inter 8x8 Scan for Simple and Main Profiles
//and Progressive Mode in Advanced Profile
//remapped as src index for continues dst index
extern const Ipp8u VC1_Inter_8x8_Scan_chroma[64];
extern const Ipp8u VC1_Inter_8x8_Scan_luma[64];

//VC-1 Table 238: Inter 8x4 Scan for Simple and Main Profiles
//remapped as src index for continues dst index
extern const Ipp8u VC1_Inter_8x4_Scan_chroma[64];
extern const Ipp8u VC1_Inter_8x4_Scan_luma[64];

//VC-1 Table 239: Inter 4x8 Scan for Simple and Main Profiles
//remapped as src index for continues dst index
extern const Ipp8u VC1_Inter_4x8_Scan_chroma[64];
extern const Ipp8u VC1_Inter_4x8_Scan_luma[64];

//Table 240: Inter 4x4 Scan for Simple and Main Profiles and
//Progressive Mode in Advanced Profile
//remapped as src index for continues dst index
extern const Ipp8u VC1_Inter_4x4_Scan_chroma[64];
extern const Ipp8u VC1_Inter_4x4_Scan_luma[64];

//Table 240: Progressive Mode Inter 8x4 Scan for Advanced Profile
extern const Ipp8u VC1_Inter_8x4_Scan_Adv_chroma[64];
extern const Ipp8u VC1_Inter_8x4_Scan_Adv_luma[64];

//Table 241: Progressive Mode Inter 4x8 Scan for Advanced Profile
extern const Ipp8u VC1_Inter_4x8_Scan_Adv_chroma[64];
extern const Ipp8u VC1_Inter_4x8_Scan_Adv_luma[64];

extern const Ipp8u VC1_Inter_InterlaceIntra_8x8_Scan_Adv_chroma[64];
extern const Ipp8u VC1_Inter_InterlaceIntra_8x8_Scan_Adv_luma[64];

/*Table 243 (SMPTE-421M-FDS1): Interlace Mode Inter 8x4 Scan for Advanced Profile*/
extern const Ipp8u VC1_Inter_Interlace_8x4_Scan_Adv_chroma[64];
extern const Ipp8u VC1_Inter_Interlace_8x4_Scan_Adv_luma[64];

/*Table 244 (SMPTE-421M-FDS1): Interlace Mode Inter 4x8 Scan for Advanced Profile*/
extern const Ipp8u VC1_Inter_Interlace_4x8_Scan_Adv_chroma[64];
extern const Ipp8u VC1_Inter_Interlace_4x8_Scan_Adv_luma[64];

/*Table 245 (SMPTE-421M-FDS1): Interlace Mode Inter 4x4 Scan for Advanced Profile*/
extern const Ipp8u VC1_Inter_Interlace_4x4_Scan_Adv_chroma[64];
extern const Ipp8u VC1_Inter_Interlace_4x4_Scan_Adv_luma[64];

//Advanced profile
extern const Ipp8u* AdvZigZagTables_IProgressive_luma[2][7];
extern const Ipp8u* AdvZigZagTables_IProgressive_chroma[2][7];

extern const Ipp8u* AdvZigZagTables_IInterlace_luma[2][7];
extern const Ipp8u* AdvZigZagTables_IInterlace_chroma[2][7];

extern const Ipp8u* AdvZigZagTables_IField_luma[2][7];
extern const Ipp8u* AdvZigZagTables_IField_chroma[2][7];

extern const Ipp8u* AdvZigZagTables_PBProgressive_luma[2][7];
extern const Ipp8u* AdvZigZagTables_PBProgressive_chroma[2][7];

extern const Ipp8u* AdvZigZagTables_PBInterlace_luma[2][7];
extern const Ipp8u* AdvZigZagTables_PBInterlace_chroma[2][7];

extern const Ipp8u* AdvZigZagTables_PBField_luma[2][7];
extern const Ipp8u* AdvZigZagTables_PBField_chroma[2][7];

//Simple/main
extern const Ipp8u* ZigZagTables_I_luma[2][7];
extern const Ipp8u* ZigZagTables_I_chroma[2][7];
extern const Ipp8u* ZigZagTables_PB_luma[2][7];
extern const Ipp8u* ZigZagTables_PB_chroma[2][7];

#endif //__umc_vc1_common_zigzag_tbl_H__
