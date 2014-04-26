/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif
#ifndef __RAW_H__
#include "raw.h"
#endif

IM_ERROR ReadImageRAW(
  BaseStreamInput& in,
  PARAMS_RAW&      param,
  CIppImage&       image)
{
  int        r;
  int        size;

  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus status;

  int       width     = param.width;
  int       height    = param.height;
  int       nchannels = param.nchannels;
  int       precision = param.precision;
  int       offset    = param.offset;

  IppiSize  roi       = { width, height };

  r = image.Alloc(roi, nchannels, precision, 0);
  if(0 != r)
  {
    return IE_ALLOC;
  }

  if(offset > 0)
  {
    status = in.Seek(offset, UIC::BaseStreamInput::Current);
    if(UIC::BaseStream::StatusOk != status)
      return IE_FSEEK;
  }

  if(precision > 8)
  {
    Ipp16s* ptr = image;
    size = width * height * nchannels * sizeof(Ipp16s);

    status = in.Read((Ipp16s*)ptr, size, cnt);
    if(UIC::BaseStream::StatusOk != status)
      return IE_FREAD;
  }
  else
  {
    Ipp8u* ptr = image;
    size = width * height * nchannels * sizeof(Ipp8u);

    status = in.Read((Ipp8u*)ptr, size, cnt);
    if(UIC::BaseStream::StatusOk != status)
      return IE_FREAD;
  }

  return IE_OK;
} // ReadImageRAW()


IM_ERROR SaveImageRAW(
  CIppImage&        image,
  PARAMS_RAW&       param,
  BaseStreamOutput& out)
{
  int        i;
  int        step;
  int        size;
  int        width;
  int        height;
  int        nchannels;
  int        precision;
  IppiSize   roi;

  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus status;

  width      = param.width;
  height     = param.height;
  nchannels  = param.nchannels;
  precision  = param.precision;
  roi.width  = width;
  roi.height = height;

  step = image.Step();

  if(precision <= 8)
  {
    Ipp8u* ptr  = image;
    size = nchannels * width * sizeof(Ipp8u);

    for(i = 0; i < height; i++)
    {
      status = out.Write(ptr, size, cnt);
      if(UIC::BaseStream::StatusOk != status)
        return IE_FWRITE;

      ptr += step;
    }
  }
  else
  {
    Ipp16s* ptr = image;
    size = nchannels * width * sizeof(Ipp16s);

    for(i = 0; i < height; i++)
    {
      status = out.Write(ptr, size, cnt);
      if(UIC::BaseStream::StatusOk != status)
        return IE_FWRITE;

      ptr = (Ipp16s*)((Ipp8u*)ptr + step);
    }
  }

  return IE_OK;
} // SaveImageRAW
