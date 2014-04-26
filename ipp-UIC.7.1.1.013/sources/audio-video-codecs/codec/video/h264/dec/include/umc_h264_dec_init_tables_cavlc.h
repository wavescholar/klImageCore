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

#ifndef __UMC_H264_DEC_INIT_TABLES_CAVLC_H__
#define __UMC_H264_DEC_INIT_TABLES_CAVLC_H__

#include "umc_h264_dec_defs_dec.h"

namespace UMC
{

extern const IppVCHuffmanSpec_32s *vlcTblCoeffToken[5];
extern const IppVCHuffmanSpec_32s *vlcTblRunBefore[16];
extern const IppVCHuffmanSpec_32s *vlcTblTotalZeros[16];
extern const IppVCHuffmanSpec_32s *vlcTblTotalZerosCR[4];
extern const IppVCHuffmanSpec_32s *vlcTblTotalZerosCR422[8];

}

#endif
