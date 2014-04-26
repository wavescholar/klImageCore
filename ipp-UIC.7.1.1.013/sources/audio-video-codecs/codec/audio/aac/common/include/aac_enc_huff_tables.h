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

#ifndef __ENC_HUFF_TABLES_H
#define __ENC_HUFF_TABLES_H

#include "aac_status.h"

#include "ippac.h"
#include "ippdc.h"

extern Ipp32s vlcEncShifts[];
extern Ipp32s vlcEncOffsets[];
extern Ipp32s vlcEncTypes[];

#ifdef  __cplusplus
extern "C" {
#endif

  AACStatus BuildHuffmanTables(IppsVLCEncodeSpec_32s** pTables,
                               Ipp32s *sizeAll);
#ifdef  __cplusplus
}
#endif

#endif//__ENC_HUFF_TABLES_H
