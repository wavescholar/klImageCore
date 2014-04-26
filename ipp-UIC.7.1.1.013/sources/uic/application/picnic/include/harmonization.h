/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef  __HARMONIZATION_H__
#define  __HARMONIZATION_H__

#ifndef __CONSTANTS_H__
#include "constants.h"
#endif
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif


typedef struct
{
  CTX_ID  id;
  int     step32;
  int     step32b;
  Ipp32f* p32f;
  Ipp32f* p32fb;

} HRMFLT_IPP_CTX;


typedef struct
{
  int           nthreads;
  int           c1;
  int           c2;
  Ipp32f        v1;
  Ipp32f        v2;
  Ipp32s        v3;
  FILTER_KERNEL filter_kernel;
  void*         ctx;

} PARAMS_HRMFLT;


int harmonization_filter_ipp (CIppImage& src, PARAMS_HRMFLT& params, CIppImage& dst);

#endif // __HARMONIZATION_H__
