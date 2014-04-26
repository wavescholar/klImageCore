/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2009-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"

#ifndef __PNG_H__
#include "png.h"
#endif
#ifndef __UIC_PNG_DEC_H__
#include "uic_png_dec.h"
#endif
#ifndef __UIC_PNG_ENC_H__
#include "uic_png_enc.h"
#endif

using namespace UIC;


IM_ERROR ReadImagePNG(
  BaseStreamInput& in,
  PARAMS_PNG&,
  CIppImage&       image)
{
  Image                 imageCn;
  ImageDataPtr          dataPtr;
  ImageDataOrder        dataOrder;
  ImageColorSpec        colorSpec;
  ImageSamplingGeometry geometry;
  ImageDataType         dataType;
  IppiSize              roi;
  Ipp32u iChannels;
  Ipp32u iPrecision;
  Ipp32u iDU;
  Ipp32u bSigned = 0;
  Ipp32u bFloat = 0;
  Ipp32u i;

  PNGDecoder decoder;

  if(ExcStatusOk != decoder.Init())
    return IE_INIT;

  if(ExcStatusOk != decoder.AttachStream(in))
    return IE_ASTREAM;

  if(ExcStatusOk != decoder.ReadHeader(colorSpec, geometry))
    return IE_RHEADER;

  iChannels  = geometry.NOfComponents();
  roi.width  = geometry.RefGridRect().Width();
  roi.height = geometry.RefGridRect().Height();

  iPrecision = colorSpec.DataRange()->BitDepth() + 1;
  dataType   = colorSpec.DataRange()->DataType();
  iDU        = UIC::NOfBytes(dataType);

  if(dataType == T16s || dataType == T32s)
    bSigned = 1, iPrecision++;
  if(dataType == T16f || dataType == T32f)
    bFloat = 1;

  dataOrder.ReAlloc(Interleaved, iChannels);
  dataOrder.SetDataType(dataType);
  dataOrder.PixelStep()[0] = iChannels * iDU;
  dataOrder.LineStep() [0] = roi.width * iChannels * iDU + BYTES_PAD(roi.width, iChannels, iDU);

  imageCn.ColorSpec().ReAlloc(iChannels);
  imageCn.ColorSpec().SetColorSpecMethod(Enumerated);
  imageCn.ColorSpec().SetComponentToColorMap(Direct);
  imageCn.ColorSpec().SetEnumColorSpace(colorSpec.EnumColorSpace());

  for(i = 0; i < iChannels; i++)
    imageCn.ColorSpec().DataRange()[i] = colorSpec.DataRange()[i];

  image.Color((IM_COLOR)colorSpec.EnumColorSpace());

  if(bSigned)
    image.Format(IF_FIXED);
  if(bFloat)
    image.Format(IF_FLOAT);

  if(0 != image.Alloc(roi, iChannels, iPrecision, 1))
    return IE_ALLOC;

  dataPtr.p8u = image;
  imageCn.Buffer().Attach(&dataPtr, dataOrder, geometry);

  if(ExcStatusOk != decoder.ReadData(imageCn.Buffer().DataPtr(), dataOrder))
    return IE_RDATA;

  return IE_OK;
} // ReadImagePNG()


IM_ERROR SaveImagePNG(
  CIppImage&        image,
  PARAMS_PNG&       param,
  BaseStreamOutput& out)
{
  Image                 imageCn;
  Rect                  refgrid;
  Point                 origin;
  RectSize              size;
  ImageDataPtr          dataPtr;
  ImageColorSpec        colorSpec;
  ImageDataOrder        dataOrder;
  ImageSamplingGeometry geometry;
  Ipp32u iChannels;
  Ipp32u iPrecision;
  Ipp32u iFormat;
  Ipp32u iDU;
  Ipp32u i;

  PNGEncoder encoder;

  if(ExcStatusOk != encoder.Init())
    return IE_INIT;

  if(ExcStatusOk != encoder.AttachStream(out))
    return IE_ASTREAM;

  if(ExcStatusOk != encoder.SetParams(param.filters))
    return IE_PARAMS;

  iChannels  = image.NChannels();
  iPrecision = image.Precision();
  iFormat    = image.Format();
  iDU        = iPrecision/8;

  size.SetWidth(image.Width());
  size.SetHeight(image.Height());
  origin.SetX(0);
  origin.SetY(0);
  refgrid.SetOrigin(origin);
  refgrid.SetSize(size);

  geometry.SetRefGridRect(refgrid);
  geometry.ReAlloc(iChannels);
  geometry.SetEnumSampling(UIC::S444);

  dataOrder.ReAlloc(Interleaved, iChannels);
  dataOrder.PixelStep()[0] = iChannels * iDU;
  dataOrder.LineStep() [0] = image.Step();

  imageCn.ColorSpec().ReAlloc(iChannels);
  imageCn.ColorSpec().SetEnumColorSpace((ImageEnumColorSpace)image.Color());
  imageCn.ColorSpec().SetColorSpecMethod(Enumerated);
  imageCn.ColorSpec().SetComponentToColorMap(Direct);

  for(i = 0; i < iChannels; i++)
  {
    if(iPrecision <= 8)
      imageCn.ColorSpec().DataRange()[i].SetAsRange8u(IPP_MAX_8U);
    else if(iPrecision <= 16)
    {
      if(iFormat == IF_UNSIGNED)
        imageCn.ColorSpec().DataRange()[i].SetAsRange16u(IPP_MAX_16U);
      else if(iFormat == IF_FIXED)
        imageCn.ColorSpec().DataRange()[i].SetAsRange16s(IPP_MIN_16S, IPP_MAX_16S);
      else
        imageCn.ColorSpec().DataRange()[i].SetAsRange16f(IPP_MAX_16U);
    }
  }

  dataPtr.p8u = image;
  imageCn.Buffer().Attach(&dataPtr, dataOrder, geometry);

  if(ExcStatusOk != encoder.AttachImage(imageCn))
    return IE_AIMAGE;

  if(ExcStatusOk != encoder.WriteHeader())
    return IE_WHEADER;

  if(ExcStatusOk != encoder.WriteData())
    return IE_WDATA;

  return IE_OK;
} // SaveImagePNG()
