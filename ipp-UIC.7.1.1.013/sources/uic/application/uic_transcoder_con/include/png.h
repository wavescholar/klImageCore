/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2009-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __PNG_H__
#define __PNG_H__

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

typedef struct _PARAMS_PNG
{
  bool filters[5];

} PARAMS_PNG;

IM_ERROR ReadImagePNG(BaseStreamInput& in, PARAMS_PNG& param, CIppImage& image);
IM_ERROR SaveImagePNG(CIppImage& image, PARAMS_PNG& param, BaseStreamOutput& out);

#endif // __PNG_H__
