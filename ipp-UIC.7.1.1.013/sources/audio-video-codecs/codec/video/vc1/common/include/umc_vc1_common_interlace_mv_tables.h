/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Interlace MV tables
//
*/

#ifndef __UMC_VC1_COMMON_INTERLACE_MV_TABLES_H__
#define __UMC_VC1_COMMON_INTERLACE_MV_TABLES_H__

#include "umc_vc1_common_defs.h"

//VC-1 Table 133: 2-Field reference interlace MV table 0
extern const Ipp32s VC1_InterlacedMVDifTable0[];

//VC-1 Table 134: 2-Field reference interlace MV table 1
extern const Ipp32s VC1_InterlacedMVDifTable1[];

//VC-1 Table 135: 2-Field reference interlace MV table 2
extern const Ipp32s VC1_InterlacedMVDifTable2[];

//VC-1 Table 136: 2-Field reference interlace MV table 3
extern const Ipp32s VC1_InterlacedMVDifTable3[];

//VC-1 Table 137: 2-Field reference interlace MV table 4
extern const Ipp32s VC1_InterlacedMVDifTable4[];

//VC-1 Table 138: 2-Field reference interlace MV table 5
extern const Ipp32s VC1_InterlacedMVDifTable5[];

//VC-1 Table 139: 2-Field reference interlace MV table 6
extern const Ipp32s VC1_InterlacedMVDifTable6[];

//VC-1 Table 140: 2-Field reference interlace MV table 7
extern const Ipp32s VC1_InterlacedMVDifTable7[];

//VC-1 Table 141: 1-Field reference interlace MV table 0
extern const Ipp32s VC1_InterlacedMVDifTable8[];

//VC-1 Table 142: 1-Field reference interlace MV table 1
extern const Ipp32s VC1_InterlacedMVDifTable9[];

//VC-1 Table 143: 1-Field reference interlace MV table 2
extern const Ipp32s VC1_InterlacedMVDifTable10[];

//VC-1 Table 144: 1-Field reference interlace MV table 3
extern const Ipp32s VC1_InterlacedMVDifTable11[];


//for scaling mv predictors P picture
extern const VC1PredictScaleValuesPPic VC1_PredictScaleValuesPPicTbl1[16];
extern const VC1PredictScaleValuesPPic VC1_PredictScaleValuesPPicTbl2[16];

//for scaling mv predictors B picture
extern const VC1PredictScaleValuesBPic VC1_PredictScaleValuesBPicTbl1[16];

#endif //__umc_vc1_common_interlace_mv_tables_H__
