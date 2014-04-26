/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef  __SEGMENTATION_H__
#define  __SEGMENTATION_H__

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif


typedef struct
{
  int   nthreads;
  int   cap;
  int   morph;
  int   normOptions;
  int   bounind;
  int   rep;
  bool  measure;
  bool  erode;
  bool  distance;
  bool  gradient;

} PARAMS_SGMFLT;


int segmentation_filter(CIppImage& src, PARAMS_SGMFLT& params, CIppImage& dst);

#endif // __SEGMENTATION_H__
