/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, DC Differential tables
//
*/

#ifndef __UMC_VC1_COMMON_DC_TBL_H__
#define __UMC_VC1_COMMON_DC_TBL_H__

#include "umc_vc1_common_defs.h"

//VC-1 Table 174: Low-motion Luma DC Differential VLC Table
extern const Ipp32s VC1_LowMotionLumaDCDiff[];

//VC-1 Table 175: Low-motion Chroma DC Differential VLC Table
extern const Ipp32s VC1_LowMotionChromaDCDiff[];

//VC-1 Table 176: High-motion Luma DC Differential VLC Table
extern const Ipp32s VC1_HighMotionLumaDCDiff[];

//VC-1 Table 177: High-motion Chroma DC Differential VLC Table
extern const Ipp32s VC1_HighMotionChromaDCDiff[];

#endif //__umc_vc1_common_dc_tbl_H__
