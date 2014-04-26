/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __JPEG2K_H__
#define __JPEG2K_H__

#ifndef __UIC_BASE_STREAM_INPUT_H__
#include "uic_base_stream_input.h"
#endif
#ifndef __UIC_BASE_STREAM_OUTPUT_H__
#include "uic_base_stream_output.h"
#endif
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif

using namespace UIC;

typedef enum
{
  WT53 = 0,
  WT97 = 1

} J2KWT;


typedef enum
{
  Lossless = 0,
  Lossy    = 1

} J2KMODE;


typedef enum
{
  J2K_32 = 0,
  J2K_16 = 1

} J2KPRECISION;


typedef struct _PARAMS_JPEG2K
{
  int nthreads;
  int quality;
  int useMCT;
  int wt;      // 0 - WT53, 1 - WT97
  int mode;    // 0 - lossless, 1 - lossy
  int color;
  int arithmetic;

} PARAMS_JPEG2K;


IM_ERROR ReadImageJPEG2000(BaseStreamInput& in, PARAMS_JPEG2K& param, CIppImage& image);
IM_ERROR SaveImageJPEG2000(CIppImage& image, PARAMS_JPEG2K& param, BaseStreamOutput& out);

#endif //__JPEG2K_H__
