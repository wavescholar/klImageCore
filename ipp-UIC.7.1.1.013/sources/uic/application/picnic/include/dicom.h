/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __DICOM_H__
#define __DICOM_H__

#ifndef __UIC_BASE_STREAM_INPUT_H__
#include "uic_base_stream_input.h"
#endif
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif

using namespace UIC;

typedef enum
{
  DTS_UNSUPPORTED     = -1,
  DTS_JPEG_BASELINE   = 0,
  DTS_JPEG_EXTENDED   = 1,
  DTS_JPEG_LOSSLESS   = 2,
  DTS_JPEG_LOSSLESS1  = 3,
  DTS_JPEGLS_LOSSLESS = 4,
  DTS_JPEGLS_NEARLOSS = 5,
  DTS_JPEG2K_LOSSLESS = 6,
  DTS_JPEG2K          = 7,
  DTS_RLE             = 8,
  DTS_RAW_LE          = 9,
  DTS_RAW_BE          = 10

} DICOM_TSYNTAX;


typedef enum
{
  DPM_MONOCHROME1    = 0,
  DPM_MONOCHROME2    = 1,
  DPM_PALETTE_COLOR  = 2,
  DPM_RGB            = 3,
  DPM_YBR_FULL       = 4,
  DPM_YBR_FULL_422   = 5,
  DPM_YBR_PART_422   = 6,
  DPM_ICT            = 7,
  DPM_RCT            = 8,
  DPM_UNKNOWN        = 9

} DICOM_PHOTOMETRIC;


typedef struct
{
  unsigned short  redDescriptor[3];
  unsigned short  greenDescriptor[3];
  unsigned short  blueDescriptor[3];
  unsigned short* RedLUT;
  unsigned short* GreenLUT;
  unsigned short* BlueLUT;

} DICOM_LUT;


typedef struct
{
  IM_TYPE           fmt;
  PARAMS_JPEG       param_jpeg;
  PARAMS_JPEG2K     param_jpeg2k;
  int               nFrames;
  int               isSigned;
  Ipp64u            firstFramePos;
  Ipp32s*           frameSeek;
  DICOM_TSYNTAX     syntax;
  DICOM_PHOTOMETRIC photometric;
  DICOM_LUT         lut;

} PARAMS_DICOM;


IM_ERROR ReadImageDICOM(BaseStreamInput& in, PARAMS_DICOM& param, CIppImage& image);
IM_ERROR GetFrameDICOM(BaseStreamInput& in, PARAMS_DICOM& param, int numFrame, CIppImage& image);

#endif // __DICOM_H__
