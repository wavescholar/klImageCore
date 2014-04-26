/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef  __INPAINTING_H__
#define  __INPAINTING_H__

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif


typedef struct
{
  int nthreads;
  int radius;
  int mode;
  int alg;

} PARAMS_INPFLT;

int inpainting_filter(const CIppImage& src, PARAMS_INPFLT& params, CIppImage& dst, CIppImage& mask);

#endif // __INPAINTING_H__
