/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#include <stdlib.h>
#ifndef __BMP_H__
#include "bmp.h"
#endif
#ifndef __UIC_BMP_DEC_H__
#include "uic_bmp_dec.h"
#endif
#ifndef __UIC_BMP_ENC_H__
#include "uic_bmp_enc.h"
#endif


using namespace UIC;


IM_ERROR ReadImageBMP(
  BaseStreamInput& in,
  PARAMS_BMP&,
  CIppImage&       image)
{
  int                   i;
  Image                 imageCn;
  IppiSize              roi;
  ImageDataPtr          dataPtr;
  ImageDataOrder        dataOrder;
  ImageColorSpec        colorSpec;
  ImageSamplingGeometry geometry;

  BMPDecoder decoder;

  if(ExcStatusOk != decoder.Init())
    return IE_INIT;

  if(ExcStatusOk != decoder.AttachStream(in))
    return IE_ASTREAM;

  if(ExcStatusOk != decoder.ReadHeader(colorSpec,geometry))
    return IE_RHEADER;

  int nOfComponents = geometry.NOfComponents();

  dataOrder.SetDataType(T8u);

  dataOrder.ReAlloc(Interleaved, nOfComponents);
  dataOrder.PixelStep()[0] = nOfComponents;
  dataOrder.LineStep() [0] = geometry.RefGridRect().Width() * nOfComponents + BYTES_PAD(geometry.RefGridRect().Width(), nOfComponents, 1);

  imageCn.ColorSpec().ReAlloc(nOfComponents);
  imageCn.ColorSpec().SetColorSpecMethod(Enumerated);
  imageCn.ColorSpec().SetComponentToColorMap(Direct);

  for(i = 0; i < nOfComponents; i++)
   imageCn.ColorSpec().DataRange()[i].SetAsRange8u(255);

  ImageEnumColorSpace in_color;
  ImageEnumColorSpace out_color;
  in_color  = colorSpec.EnumColorSpace();
  out_color = in_color;

  imageCn.ColorSpec().SetEnumColorSpace(out_color);
  image.Color((IM_COLOR)image.UicToIppColor(out_color));

  roi.width  = geometry.RefGridRect().Width();
  roi.height = geometry.RefGridRect().Height();

  if(0 != image.Alloc(roi,nOfComponents,8,1))
    return IE_ALLOC;

  dataPtr.p8u = image;

  imageCn.Buffer().Attach(&dataPtr,dataOrder,geometry);

  if(ExcStatusOk != decoder.ReadData(imageCn.Buffer().DataPtr(),dataOrder))
    return IE_RDATA;

  return IE_OK;
} // ReadImageBMP()


IM_ERROR SaveImageBMP(
  CIppImage&        image,
  PARAMS_BMP&,
  BaseStreamOutput& out)
{
  int                   i;
  int                   nOfComponents;
  Image                 imageCn;
  Rect                  refgrid;
  Point                 origin;
  RectSize              size;
  ImageDataPtr          dataPtr;
  ImageColorSpec        colorSpec;
  ImageDataOrder        dataOrder;
  ImageSamplingGeometry geometry;

  BMPEncoder encoder;

  if(ExcStatusOk != encoder.Init())
    return IE_INIT;

  if(ExcStatusOk != encoder.AttachStream(out))
    return IE_ASTREAM;

  nOfComponents = image.NChannels();

  dataOrder.SetDataType(T8u);

  size.SetWidth(image.Width());
  size.SetHeight(image.Height());

  origin.SetX(0);
  origin.SetY(0);

  refgrid.SetOrigin(origin);
  refgrid.SetSize(size);

  geometry.SetRefGridRect(refgrid);
  geometry.ReAlloc(nOfComponents);
  geometry.SetEnumSampling(S444);

  dataOrder.ReAlloc(Interleaved, nOfComponents);
  dataOrder.PixelStep()[0] = nOfComponents;
  dataOrder.LineStep() [0] = image.Step();

  imageCn.ColorSpec().ReAlloc(nOfComponents);
  imageCn.ColorSpec().SetColorSpecMethod(Enumerated);
  imageCn.ColorSpec().SetComponentToColorMap(Direct);

  for(i = 0; i < nOfComponents; i++)
    imageCn.ColorSpec().DataRange()[i].SetAsRange8u(255);

  switch(image.NChannels())
  {
    case 1: imageCn.ColorSpec().SetEnumColorSpace(Grayscale); break;
    case 3: imageCn.ColorSpec().SetEnumColorSpace(BGR);       break;
    case 4: imageCn.ColorSpec().SetEnumColorSpace(BGRA);      break;
    default:
      break;
  }

  int dstOrder_c3[3] = {2, 1, 0};
  int dstOrder_c4[4] = {2, 1, 0, 3};

  if(image.NChannels() == 3 && image.Color() == IC_RGB)
  {
    image.SwapChannels(dstOrder_c3);
    image.Color((IM_COLOR)IC_BGR);
  }
  else if(image.NChannels() == 4 && image.Color() == IC_RGBA)
  {
    image.SwapChannels(dstOrder_c4);
    image.Color((IM_COLOR)IC_BGRA);
  }

  dataPtr.p8u = image;

  imageCn.Buffer().Attach(&dataPtr,dataOrder,geometry);

  if(ExcStatusOk != encoder.AttachImage(imageCn))
    return IE_AIMAGE;

  if(ExcStatusOk != encoder.WriteHeader())
    return IE_WHEADER;

  if(ExcStatusOk != encoder.WriteData())
    return IE_WDATA;

  return IE_OK;
} // SaveImageBMP()

