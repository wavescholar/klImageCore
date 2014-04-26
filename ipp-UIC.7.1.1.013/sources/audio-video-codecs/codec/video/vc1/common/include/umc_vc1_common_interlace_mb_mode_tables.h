/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Interlace pictures MB Mode tables
//
*/

#ifndef __UMC_VC1_COMMON_INTERLACE_MB_MODE_TABLES_H__
#define __UMC_VC1_COMMON_INTERLACE_MB_MODE_TABLES_H__

#include "umc_vc1_common_defs.h"

//VC-1 Table 145: mixed MV MB Mode Table 0
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable0[];

//VC-1 Table 146: mixed MV MB Mode Table 1
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable1[];

//VC-1 Table 147: mixed MV MB Mode Table 2
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable2[];

//VC-1 Table 148: mixed MV MB Mode Table 3
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable3[];

//VC-1 Table 149: mixed MV MB Mode Table 4
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable4[];

//VC-1 Table 150: mixed MV MB Mode Table 5
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable5[];

//VC-1 Table 151: mixed MV MB Mode Table 6
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable6[];

//VC-1 Table 152: mixed MV MB Mode Table 7
extern const Ipp32s VC1_Mixed_MV_MB_ModeTable7[];

//VC-1 Table 153: 1-MV MB Mode Table 0
extern const Ipp32s VC1_1MV_MB_ModeTable0[];

//VC-1 Table 154: 1-MV MB Mode Table 1
extern const Ipp32s VC1_1MV_MB_ModeTable1[];

//VC-1 Table 155: 1-MV MB Mode Table 2
extern const Ipp32s VC1_1MV_MB_ModeTable2[];

//VC-1 Table 156: 1-MV MB Mode Table 3
extern const Ipp32s VC1_1MV_MB_ModeTable3[];

//VC-1 Table 157: 1-MV MB Mode Table 4
extern const Ipp32s VC1_1MV_MB_ModeTable4[];

//VC-1 Table 158: 1-MV MB Mode Table 5
extern const Ipp32s VC1_1MV_MB_ModeTable5[];

//VC-1 Table 159: 1-MV MB Mode Table 6
extern const Ipp32s VC1_1MV_MB_ModeTable6[];

//VC-1 Table 160: 1-MV MB Mode Table 7
extern const Ipp32s VC1_1MV_MB_ModeTable7[];

//VC-1 Table 161: P/B picture 1-MV MB Mode Table 0
extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table0[];

//VC-1 Table 162: P/B picture 1-MV MB Mode Table 1
extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table1[];

//VC-1 Table 163: P/B picture 1-MV MB Mode Table 2
extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table2[];

//VC-1 Table 164: P/B picture 1-MV MB Mode Table 3
extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table3[];

//VC-1 Table 165 Interlace Frame P/B Pictures Non 4-MV MBMODE Table 0
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table0[];

//VC-1 Table 166 Interlace Frame P/B Pictures Non 4-MV MBMODE Table 1
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table1[];

//VC-1 Table 167 Interlace Frame P/B Pictures Non 4-MV MBMODE Table 2
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table2[];

//VC-1 Table 168 Interlace Frame P/B Pictures Non 4-MV MBMODE Table 3
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table3[];

//VC1 Tables 161-168
extern const Ipp8u VC1_MB_Mode_PBPic_Transform_Table[];
extern const Ipp8u VC1_MB_Mode_PBPic_MBtype_Table[];
extern const Ipp8s VC1_MB_Mode_PBPic_MVPresent_Table[];
extern const Ipp8u VC1_MB_Mode_PBPic_FIELDTX_Table[];

//VC1 Table 111, 112
extern const Ipp8u VC1_MB_Mode_PBFieldPic_MBtype_Table[];
extern const Ipp8u VC1_MB_Mode_PBFieldPic_CBPPresent_Table[];
extern const Ipp8s VC1_MB_Mode_PBFieldPic_MVData_Table[];

#endif //__umc_vc1_common_interlace_mb_mode_tables_H__
