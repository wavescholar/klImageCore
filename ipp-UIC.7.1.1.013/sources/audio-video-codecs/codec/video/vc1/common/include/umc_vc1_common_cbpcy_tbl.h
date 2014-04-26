/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Coded block pattern tables
//
*/

#ifndef __UMC_VC1_COMMON_CBPCY_TBL_H__
#define __UMC_VC1_COMMON_CBPCY_TBL_H__

#include "umc_vc1_common_defs.h"

//VC-1 Table 169: I-Picture CBPCY VLC Table
extern const Ipp32s VC1_CBPCY_Ipic[];

//VC-1 Table 170: P and B-Picture CBPCY VLC Table 0
extern const Ipp32s VC1_CBPCY_PBpic_tbl0[];

//VC-1 Table 171: P and B-Picture CBPCY VLC Table 1
extern const Ipp32s VC1_CBPCY_PBpic_tbl1[];

//VC-1 Table 172: P and B-Picture CBPCY VLC Table 2
extern const Ipp32s VC1_CBPCY_PBpic_tbl2[];

//VC-1 Table 173: P and B-Picture CBPCY VLC Table 3
extern const Ipp32s VC1_CBPCY_PBpic_tbl3[];

#endif //__umc_vc1_common_cbpcy_tbl_H__
