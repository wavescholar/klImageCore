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

#ifndef __SFB_TABLES_H
#define __SFB_TABLES_H

#include "ippdefs.h"

typedef struct {
  Ipp32s  samp_rate;
  Ipp32s  num_sfb_long_window;
  Ipp32s *sfb_offset_long_window;
  Ipp32s  num_sfb_short_window;
  Ipp32s *sfb_offset_short_window;

} sSfbTableElement;

#ifdef  __cplusplus
extern  "C" {
#endif

  extern sSfbTableElement sfb_tables[];

#ifdef  __cplusplus
}
#endif
#endif             // __SFB_TABLES_H
