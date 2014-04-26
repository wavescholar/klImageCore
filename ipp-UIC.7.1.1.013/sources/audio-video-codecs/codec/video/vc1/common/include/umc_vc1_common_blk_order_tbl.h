/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, block order table
//
*/

#ifndef __UMC_VC1_COMMON_BLK_ORDER_TBL_H__
#define __UMC_VC1_COMMON_BLK_ORDER_TBL_H__

#include "ippdefs.h"

extern const Ipp32u VC1_pixel_table[6];

extern const Ipp32u VC1_PredDCIndex[3][6];
extern const Ipp32u VC1_QuantIndex [2][6];

extern const Ipp32u VC1_BlockTable[50];

extern const Ipp32u VC1_BlkStart[];

#endif //__umc_vc1_common_blk_order_tbl_H__
