/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#ifndef __JPEGBASE_H__
#include "jpegbase.h"
#endif
#ifndef __DETECTOR_H__
#include "detector.h"
#endif


int CFormatDetector::IsBMP(UIC::BaseStreamInput& in)
{
  char buf[2];
  BaseStream::TSize   cnt;
  BaseStream::TStatus jerr;

  jerr = in.Seek(0,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(buf,2*sizeof(char),cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if((buf[0] == 'B') && (buf[1] == 'M'))
  {
    return 1;
  }

  return 0;
} // CFormatDetector::IsBMP()

int CFormatDetector::IsPNG(UIC::BaseStreamInput& in)
{
  unsigned char buf[4];
  BaseStream::TSize   cnt;
  BaseStream::TStatus jerr;

  jerr = in.Seek(0,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(buf,4*sizeof(char),cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if(buf[0] == 0x89 && buf[1] == 0x50 && buf[2] == 0x4e && buf[3] == 0x47)
  {
    return 1;
  }

  return 0;
} // CFormatDetector::IsPNG()


int CFormatDetector::IsJPEG(UIC::BaseStreamInput& in)
{
  unsigned char byte;
  BaseStream::TSize   cnt;
  BaseStream::TStatus jerr;

  jerr = in.Seek(0,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(&byte,sizeof(char),cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if(byte == 0xff)
  {
    while(byte == 0xff)
    {
      jerr = in.Read(&byte,sizeof(char),cnt);
      if(UIC::BaseStreamInput::StatusOk != jerr)
        return 0;
    }

    // check for Start Of Image marker
    if(byte == 0xd8)
      return 1;
  }

  return 0;
} // CFormatDetector::IsJPEG()


int CFormatDetector::IsJPEG2000(UIC::BaseStreamInput& in)
{
  unsigned char buf[4];

  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus jerr;

  jerr = in.Seek(0,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(buf,2*sizeof(char),cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if(buf[0] == 0xff && buf[1] == 0x4f)
  {
    return 1;
  }

  jerr = in.Seek(4,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(buf,4*sizeof(char),cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if(buf[0] == 0x6a && buf[1] == 0x50 && buf[2] == 0x20 && buf[3] == 0x20)
  {
    return 1;
  }

  return 0;
} // CFormatDetector::IsJPEG2000()


int CFormatDetector::IsDICOM(UIC::BaseStreamInput& in)
{
  char       buf[4];
  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus jerr;

  jerr = in.Seek(128,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(buf,4*sizeof(char),cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if((buf[0] == 'D') && (buf[1] == 'I') && (buf[2] == 'C') && (buf[3] == 'M'))
  {
    return 1;
  }

  return 0;
} // CFormatDetector:IsDICOM()


int CFormatDetector::IsJPEGXR(UIC::BaseStreamInput& in)
{
  Ipp16u wByteOrder;
  Ipp8u  jxrID;

  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus jerr;

  jerr = in.Seek(0, UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  jerr = in.Read(&wByteOrder, sizeof(Ipp16u), cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if(0x4949 != wByteOrder)
  {
    return 0;
  }

  jerr = in.Read(&jxrID, sizeof(Ipp8u), cnt);
  if(UIC::BaseStreamInput::StatusOk != jerr)
    return 0;

  if(0xbc != jxrID)
  {
    return 0;
  }

  return 1;
} // CFormatDetector:IsJPEGXR()

IM_TYPE CFormatDetector::ImageFormat(UIC::BaseStreamInput& in)
{
  IM_TYPE             fmt;
  BaseStream::TStatus status;

  fmt = IT_UNKNOWN;

  if(IsBMP(in))
    fmt = IT_BMP;
  else if(IsPNG(in))
    fmt = IT_PNG;
  else if(IsJPEG(in))
    fmt = IT_JPEG;
  else if(IsJPEG2000(in))
    fmt = IT_JPEG2000;
  else if(IsDICOM(in))
    fmt = IT_DICOM;
  else if(IsJPEGXR(in))
    fmt = IT_JPEGXR;

  status = in.Seek(0,UIC::BaseStreamInput::Beginning);
  if(UIC::BaseStreamInput::StatusOk != status)
    return IT_UNKNOWN;

  return fmt;
} // CFormatDetector::ImageFormat()

