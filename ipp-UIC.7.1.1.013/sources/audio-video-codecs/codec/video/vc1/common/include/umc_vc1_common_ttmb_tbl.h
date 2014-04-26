/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Transform type tables
//
*/

#ifndef __UMC_VC1_COMMON_TTMB_TBL_H__
#define __UMC_VC1_COMMON_TTMB_TBL_H__

#include "umc_vc1_common_defs.h"

//VC-1 Table 53: High Rate (PQUANT < 5) TTMB VLC Table
extern const Ipp32s VC1_HighRateTTMB[];

//VC-1 Table 54: Medium Rate (5 <= PQUANT < 13) TTMB VLC Table
extern const Ipp32s VC1_MediumRateTTMB[];

//VC-1 Table 55: Low Rate (PQUANT >= 13) TTMB VLC Table
extern const Ipp32s VC1_LowRateTTMB[];

//VC-1 Table 61: High Rate (PQUANT < 5) TTBLK VLC Table
extern const Ipp32s VC1_HighRateTTBLK[];

//VC-1 Table 62: Medium Rate (5 =< PQUANT < 13) TTBLK VLC Table
extern const Ipp32s VC1_MediumRateTTBLK[];

//VC-1 Table 63: Low Rate (PQUANT >= 13) TTBLK VLC Table
extern const Ipp32s VC1_LowRateTTBLK[];

//VC-1 Table 64: High Rate (PQUANT < 5) SUBBLKPAT VLC Table
extern const Ipp32s VC1_HighRateSBP[];

//VC-1 Table 65: Medium Rate (5 =< PQUANT < 13) SUBBLKPAT VLC Table
extern const Ipp32s VC1_MediumRateSBP[];

//VC-1 Table 66: Low Rate (PQUANT >= 13) SUBBLKPAT VLC Table
extern const Ipp32s VC1_LowRateSBP[];

#endif //__umc_vc1_common_ttmb_tbl_H__
