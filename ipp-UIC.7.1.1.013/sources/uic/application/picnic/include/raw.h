/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __RAW_H__
#define __RAW_H__

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

typedef struct _PARAMS_RAW
{
  int    width;
  int    height;
  int    nchannels;
  int    precision;
  int    offset;
  int    color;
  int    sampling;

} PARAMS_RAW;


IM_ERROR ReadImageRAW(BaseStreamInput& in, PARAMS_RAW& param, CIppImage& image);
IM_ERROR SaveImageRAW(CIppImage& image, PARAMS_RAW& param, BaseStreamOutput& out);

#endif // __RAW_H__
