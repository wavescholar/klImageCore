/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2009-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __JPEGXR_H__
#define __JPEGXR_H__

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

#define JXR_OCF_AUTO 0xff

typedef enum _JXR_PA_MODE
{
  JXR_PM_BOTH  = 0,
  JXR_PM_IMAGE = 1,
  JXR_PM_ALPHA = 2

} JXR_PA_MODE;


typedef struct _PARAMS_JPEGXR
{
  int quality;
  int aquality;
  int bands;
  int bitstream;
  int overlap;
  int sampling;
  int trim;
  int alpha_mode;
  int cmyk_direct;
  int tiles_uniform[4];
  int bits_shift;
  int threads;
  int thread_mode;

} PARAMS_JPEGXR;

IM_ERROR ReadImageJPEGXR(BaseStreamInput& in, PARAMS_JPEGXR& param, CIppImage& image);
IM_ERROR SaveImageJPEGXR(CIppImage& image, PARAMS_JPEGXR& param, BaseStreamOutput& out);

#endif // __JPEGXR_H__
