/*
//
//              INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license  agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in  accordance  with the terms of that agreement.
//        Copyright (c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//
*/

#ifndef __UMC_H264_DEC_INIT_TABLES_CABAC_H__
#define __UMC_H264_DEC_INIT_TABLES_CABAC_H__

#include "umc_h264_dec_defs_dec.h"

namespace UMC
{

#ifdef CABAC_OPTIMIZATION
extern const Ipp8u rangeTabLPS[64][4];
#else
extern const Ipp8u rangeTabLPS[128][4];
#endif

extern const Ipp8u transIdxMPS[128];

extern const Ipp8u transIdxLPS[128];

extern const Ipp32u NumBitsToGetTableSmall[4];

#ifndef CABAC_OPTIMIZATION
extern const Ipp8u NumBitsToGetTbl[512];
#endif

} // namespace UMC

#endif //__UMC_H264_DEC_INIT_TABLES_CABAC_H__
