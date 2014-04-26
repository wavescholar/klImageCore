/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __DETECTOR_H__
#define __DETECTOR_H__

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif
#ifndef __UIC_BASE_STREAM_INPUT_H__
#include "uic_base_stream_input.h"
#endif

using namespace UIC;


class CFormatDetector
{
public:
  CFormatDetector(void) {}
  virtual ~CFormatDetector(void) {}

  IM_TYPE ImageFormat(UIC::BaseStreamInput& in);

protected:
  int IsBMP(UIC::BaseStreamInput& in);      // non zero if BMP, zero otherwise
  int IsPNG(UIC::BaseStreamInput& in);
  int IsJPEG(UIC::BaseStreamInput& in);
  int IsJPEG2000(UIC::BaseStreamInput& in);
  int IsDICOM(UIC::BaseStreamInput& in);
  int IsJPEGXR(UIC::BaseStreamInput& in);

};

#endif // __DETECTOR_H__

