/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __BMP_H__
#define __BMP_H__

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

typedef struct _PARAMS_BMP
{
  int  reserved;

} PARAMS_BMP;

IM_ERROR ReadImageBMP(BaseStreamInput& in, PARAMS_BMP& param, CIppImage& image);
IM_ERROR SaveImageBMP(CIppImage& image, PARAMS_BMP& param, BaseStreamOutput& out);

#endif // __BMP_H__
