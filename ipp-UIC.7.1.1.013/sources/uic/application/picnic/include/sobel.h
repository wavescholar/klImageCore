/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef  __SOBEL_H__
#define  __SOBEL_H__

#ifndef __CONSTANTS_H__
#include "constants.h"
#endif
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif


typedef struct
{
  CTX_ID  id;
  int     step;
  int     stephor;
  int     stepver;

  union
  {
    Ipp8u*  Data8u;
    Ipp32f* Data32f;
  } p;

  Ipp8u*  pBuf;

  union
  {
    Ipp16s* p16shor;
    Ipp32f* p32fhor;
  };

  union
  {
    Ipp16s* p16sver;
    Ipp32f* p32fver;
  };

} SBLFLT_IPP_CTX;


typedef struct
{
  int    nthreads;
  FILTER_KERNEL filter_kernel;
  void*  ctx;

} PARAMS_SBLFLT;


int sobel_filter_ipp (CIppImage& src, PARAMS_SBLFLT& params, CIppImage& dst);

#endif // __SOBEL_H__
