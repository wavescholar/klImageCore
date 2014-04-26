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

#include "ippdefs.h"
#ifdef IPP_JPEG2000_TIMING
#include "timer.h"
#include <stdio.h>
#endif
#ifndef __JPEG2K_H__
#include "jpeg2k.h"
#endif
#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif
#ifndef __STDFILEIN_H__
#include "stdfilein.h"
#endif
#ifndef __UIC_JPEG2000_DEC_H__
#include "uic_jpeg2000_dec.h"
#endif
#ifndef __UIC_JP2_DEC_H__
#include "uic_jp2_dec.h"
#endif
#ifndef __UIC_JP2_ENC_H__
#include "uic_jp2_enc.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif


using namespace UIC;

static const int MAX_N_OF_RES_LEVELS = 33;


int IsJP2(BaseStreamInput& in)
{
  unsigned char buf[4];
  UIC::BaseStream::TSize cnt;

  if(UIC::BaseStream::StatusOk != in.Seek(4, UIC::BaseStreamInput::Beginning))
    return IE_FSEEK;

  if(UIC::BaseStream::StatusOk != in.Read(buf, 4*sizeof(char),cnt))
    return IE_FREAD;

  if(UIC::BaseStream::StatusOk != in.Seek(0, UIC::BaseStreamInput::Beginning))
    return IE_FSEEK;

  if(buf[0] == 0x6a && buf[1] == 0x50 && buf[2] == 0x20 && buf[3] == 0x20)
    return 1;

  return 0;
} // IsJP2()


IM_ERROR IppConvert(Ipp32s* src, int srcStep, Ipp8u* dst, int dstStep, IppiSize roi, int nChannels)
{
  IppStatus status;

  switch(nChannels)
  {
  case 1:
    status = ippiConvert_32s8u_C1R(src, srcStep, dst, dstStep, roi);
    break;

  case 3:
    status = ippiConvert_32s8u_C3R(src, srcStep, dst, dstStep, roi);
    break;

  case 4:
    status = ippiConvert_32s8u_C4R(src, srcStep, dst, dstStep, roi);
    break;

  default:
    return IE_PROCESS;
  }

  return status == ippStsNoErr ? IE_OK : IE_PROCESS;
} // IppConvert()


IM_ERROR IppConvert(Ipp16s* src, int srcStep, Ipp8u* dst, int dstStep, IppiSize roi, int nChannels)
{
  IppStatus status;

  switch(nChannels)
  {
  case 1:
    status = ippiConvert_16s8u_C1R(src, srcStep, dst, dstStep, roi);
    break;

  case 3:
    status = ippiConvert_16s8u_C3R(src, srcStep, dst, dstStep, roi);
    break;

  case 4:
    status = ippiConvert_16s8u_C4R(src, srcStep, dst, dstStep, roi);
    break;

  default:
    return IE_PROCESS;
  }

  return status == ippStsNoErr ? IE_OK : IE_PROCESS;
} // IppConvert()


IM_ERROR IppConvert(Ipp32s* src, int srcStep, Ipp16s* dst, int dstStep, IppiSize roi, int nChannels)
{
  int i;
  IppStatus status;

  switch(nChannels)
  {
  case 1:
    for(i = 0; i < roi.height; i++)
    {
      Ipp32s* pSrc = (Ipp32s*)((Ipp8u*)src + i*srcStep);
      Ipp16s* pDst = (Ipp16s*)((Ipp8u*)dst + i*dstStep);

      status = ippsConvert_32s16s(pSrc, pDst, roi.width);
    }
    break;

  case 3:
    for(i = 0; i < roi.height; i++)
    {
      Ipp32s* pSrc = (Ipp32s*)((Ipp8u*)src + i*srcStep);
      Ipp16s* pDst = (Ipp16s*)((Ipp8u*)dst + i*dstStep);

      status = ippsConvert_32s16s(pSrc, pDst, roi.width*3);
    }
    break;

  case 4:
    for(i = 0; i < roi.height; i++)
    {
      Ipp32s* pSrc = (Ipp32s*)((Ipp8u*)src + i*srcStep);
      Ipp16s* pDst = (Ipp16s*)((Ipp8u*)dst + i*dstStep);

      status = ippsConvert_32s16s(pSrc, pDst, roi.width*4);
    }
    break;

  default:
    return IE_PROCESS;
  }

  return status == ippStsNoErr ? IE_OK : IE_PROCESS;
} // IppConvert()


IM_ERROR IppConvert(Ipp32s* src, int srcStep, Ipp16u* dst, int dstStep, IppiSize roi, int nChannels)
{
  int     i, j;
  Ipp32s* pSrc;
  Ipp16u* pDst;

  pSrc = src;
  pDst = dst;

  for(i = 0; i < roi.height; i++)
  {
    pSrc = (Ipp32s*)((Ipp8u*)src + i*srcStep);
    pDst = (Ipp16u*)((Ipp8u*)dst + i*dstStep);

    for(j = 0; j < roi.width*nChannels; j++)
      pDst[j] = (Ipp16u)pSrc[j];

  }

  return IE_OK;
} // IppConvert()


IM_ERROR IppConvert(Ipp16s* src, int srcStep, Ipp16s* dst, int dstStep, IppiSize roi, int nChannels)
{
  IppStatus status;

  switch(nChannels)
  {
  case 1:
    status = ippiCopy_16s_C1R(src, srcStep, dst, dstStep, roi);
    break;

  case 3:
    status = ippiCopy_16s_C3R(src, srcStep, dst, dstStep, roi);
    break;

  case 4:
    status = ippiCopy_16s_C4R(src, srcStep, dst, dstStep, roi);
    break;

  default:
    return IE_PROCESS;
  }

  return status == ippStsNoErr ? IE_OK : IE_PROCESS;
} // IppConvert()


IM_ERROR IppCopy(Ipp32s* src[3], int srcStep, Ipp32s* dst, int dstStep, IppiSize roi)
{
  IppStatus status;

  status = ippiCopy_32s_P3C3R(src, srcStep, dst, dstStep, roi);
  if(status != ippStsNoErr)
  {
    return IE_PROCESS;
  }

  return IE_OK;
} // IppCopy()


IM_ERROR IppCopy(Ipp16s* src[3], int srcStep, Ipp16s* dst, int dstStep, IppiSize roi)
{
  IppStatus status;

  status = ippiCopy_16s_P3C3R(src, srcStep, dst, dstStep, roi);
  if(status != ippStsNoErr)
  {
    return IE_PROCESS;
  }

  return IE_OK;
} // IppCopy()


IM_ERROR IppThreshold(Ipp32s min, Ipp32s max, Ipp32s* srcDst, unsigned int size)
{
  IppStatus status;

  status = ippsThreshold_LT_32s_I(srcDst, size, min);
  if(status != ippStsNoErr)
  {
    return IE_PROCESS;
  }

  status = ippsThreshold_GT_32s_I(srcDst, size, max);
  if(status != ippStsNoErr)
  {
    return IE_PROCESS;
  }

  return IE_OK;
} // IppThreshold()


IM_ERROR IppThreshold(Ipp32s min, Ipp32s max, Ipp16s* srcDst, unsigned int size)
{
  IppStatus status;

  status = ippsThreshold_LT_16s_I(srcDst, size, min);
  if(status != ippStsNoErr)
  {
    return IE_PROCESS;
  }

  status = ippsThreshold_GT_16s_I(srcDst, size, max);
  if(status != ippStsNoErr)
  {
    return IE_PROCESS;
  }

  return IE_OK;
}  // IppThreshold()


IM_ERROR ReadImageJPEG2000(BaseStreamInput& in, PARAMS_JPEG2K& param, CIppImage& image)
{
  int                   nOfComponents;
  int                   isJP2;
  Image                 imagePn;
  ImageColorSpec        colorSpec;
  BaseStreamDiagn       diagnOutput;
  ImageSamplingGeometry geometry;

  BaseImageDecoder     *decoder;

  JP2Decoder            JP2decoder;
  JPEG2000Decoder       JP2Kdecoder;

  J2KPRECISION  j2kArithmetic;

  if(param.arithmetic == 0) // 0 == 32s
  {
    j2kArithmetic = J2K_32;
  }
  else if(param.arithmetic == 1) // 1 == 16s
  {
    j2kArithmetic = J2K_16;
  }
  else
    return IE_PROCESS;


  isJP2 = IsJP2(in);
  if(isJP2 < 0)
    return IE_UNSUP;

  if(isJP2)
    decoder = &JP2decoder;
  else
    decoder = &JP2Kdecoder;

  if(ExcStatusOk != decoder->Init())
    return IE_INIT;

  decoder->SetNOfThreads(param.nthreads);

  if(ExcStatusOk != decoder->AttachStream(in))
    return IE_ASTREAM;

  decoder->AttachDiagnOut(diagnOutput);

  if(ExcStatusOk != decoder->ReadHeader(colorSpec,geometry))
    return IE_RHEADER;

  geometry.ReduceByGCD();

  nOfComponents = geometry.NOfComponents();

  if(colorSpec.DataRange()->BitDepth() + 1 >= 14)
    j2kArithmetic = J2K_32;

  if(J2K_32 == j2kArithmetic)
    imagePn.Buffer().ReAlloc   (T32s, Plane, geometry);
  else //J2K_16 == j2kArithmetic)
    imagePn.Buffer().ReAlloc   (T16s, Plane, geometry);

  const ImageDataOrder  &dataOrderPn = imagePn.Buffer().BufferFormat().DataOrder();


  geometry.SetEnumSampling(S444);


  imagePn.ColorSpec().ReAlloc(nOfComponents);
  imagePn.ColorSpec().SetColorSpecMethod(Enumerated);
  imagePn.ColorSpec().SetComponentToColorMap(Direct);

  ImageEnumColorSpace in_color;
  in_color = colorSpec.EnumColorSpace();
  imagePn.ColorSpec().SetEnumColorSpace(in_color);

  if(ExcStatusOk != decoder->ReadData(imagePn.Buffer().DataPtr(),dataOrderPn))
    return IE_RDATA;

  int       tmpRowSize;
  IppiSize  sz;
  IppiSize  size;
  IM_ERROR  im_err = IE_OK;

  size.width  = imagePn.Buffer().BufferFormat().SamplingGeometry().RefGridRect().Width();
  size.height = imagePn.Buffer().BufferFormat().SamplingGeometry().RefGridRect().Height();

  switch(colorSpec.EnumColorSpace())
  {
    case RGB:       image.Color(IC_RGB    ); break;
    case Grayscale: image.Color(IC_GRAY   ); break;
    default:        image.Color(IC_UNKNOWN);
  }

  if(0 != image.Alloc(size, nOfComponents, colorSpec.DataRange()->BitDepth() + 1))
    return IE_ALLOC;

  switch(nOfComponents)
  {
  case 1:
    {
      if(image.Precision() <= 8)
      {
        Ipp8u* ptr = image;

        if(J2K_32 == j2kArithmetic)
        {
          im_err = IppConvert(imagePn.Buffer().DataPtr()[0].p32s, dataOrderPn.LineStep()[0], ptr, image.Step(),size,1);
        }
        else // J2K_16 == j2kArithmetic
        {
          im_err = IppConvert(imagePn.Buffer().DataPtr()[0].p16s, dataOrderPn.LineStep()[0], ptr, image.Step(),size,1);
        }

        if(IE_OK != im_err)
          return im_err;
      }
      else
      {
        Ipp16s* ptr = image;

        if(J2K_32 == j2kArithmetic)
        {
          im_err = IppConvert(imagePn.Buffer().DataPtr()[0].p32s, dataOrderPn.LineStep()[0], (Ipp16u*)ptr, image.Step(),size,1);
        }
        else // J2K_16 == j2kArithmetic
        {
          im_err = IppConvert(imagePn.Buffer().DataPtr()[0].p16s, dataOrderPn.LineStep()[0], ptr, image.Step(),size,1);
        }

        if(IE_OK != im_err)
          return im_err;
      }
    }
    break;

  case 3:
    {
      sz.height = 1;
      sz.width  = size.width;

      if(J2K_32 == j2kArithmetic)
      {
        Ipp32s* p[3];

        p[0] = imagePn.Buffer().DataPtr()[0].p32s;
        p[1] = imagePn.Buffer().DataPtr()[1].p32s;
        p[2] = imagePn.Buffer().DataPtr()[2].p32s;

        tmpRowSize = size.width * nOfComponents * sizeof(Ipp32s);
        Ipp32s* tmpRow = (Ipp32s*)ippMalloc(tmpRowSize);

        if(NULL == tmpRow)
          return IE_ALLOC;

        if(image.Precision() <= 8)
        {
          Ipp8u* ptr = image;

          for(int i = 0; i < size.height; i++)
          {
            im_err = IppCopy(p, dataOrderPn.LineStep()[0], tmpRow, tmpRowSize, sz);
            if(IE_OK != im_err)
              return im_err;

            im_err = IppConvert(tmpRow, tmpRowSize, ptr, image.Step(), sz,3);
            if(IE_OK != im_err)
              return im_err;

            ptr  += image.Step();

            p[0] = (Ipp32s*)((Ipp8u*)p[0] + dataOrderPn.LineStep()[0]);
            p[1] = (Ipp32s*)((Ipp8u*)p[1] + dataOrderPn.LineStep()[1]);
            p[2] = (Ipp32s*)((Ipp8u*)p[2] + dataOrderPn.LineStep()[2]);
          }
        }
        else
        {
          Ipp16u* ptr = image;

          for(int i = 0; i < size.height; i++)
          {
            im_err = IppCopy(p, dataOrderPn.LineStep()[2], tmpRow, tmpRowSize, sz);
            if(IE_OK != im_err)
              return im_err;

            im_err = IppThreshold(0, (1 << (image.Precision())) - 1, tmpRow, sz.width*3);
            if(IE_OK != im_err)
              return im_err;

            im_err = IppConvert(tmpRow, tmpRowSize, ptr, image.Step(), sz, 3);
            if(IE_OK != im_err)
              return im_err;

            ptr = (Ipp16u*)((Ipp8u*)ptr + image.Step());

            p[0] = (Ipp32s*)((Ipp8u*)p[0] + dataOrderPn.LineStep()[0]);
            p[1] = (Ipp32s*)((Ipp8u*)p[1] + dataOrderPn.LineStep()[1]);
            p[2] = (Ipp32s*)((Ipp8u*)p[2] + dataOrderPn.LineStep()[2]);
          }
        }

        ippFree(tmpRow);
      }
      else // J2K_16 == j2kArithmetic
      {
        Ipp16s* p[3];

        p[0] = imagePn.Buffer().DataPtr()[0].p16s;
        p[1] = imagePn.Buffer().DataPtr()[1].p16s;
        p[2] = imagePn.Buffer().DataPtr()[2].p16s;

        tmpRowSize = size.width * nOfComponents * sizeof(Ipp16s);
        Ipp16s* tmpRow = (Ipp16s*)ippMalloc(tmpRowSize);

        if(NULL == tmpRow)
          return IE_ALLOC;

        if(image.Precision() <= 8)
        {
          Ipp8u* ptr = image;

          for(int i = 0; i < size.height; i++)
          {
            im_err = IppCopy(p, dataOrderPn.LineStep()[0], tmpRow, tmpRowSize, sz);
            if(IE_OK != im_err)
              return im_err;

            im_err = IppConvert(tmpRow, tmpRowSize, ptr, image.Step(), sz,3);
            if(IE_OK != im_err)
              return im_err;

            ptr  += image.Step();

            p[0] = (Ipp16s*)((Ipp8u*)p[0] + dataOrderPn.LineStep()[0]);
            p[1] = (Ipp16s*)((Ipp8u*)p[1] + dataOrderPn.LineStep()[1]);
            p[2] = (Ipp16s*)((Ipp8u*)p[2] + dataOrderPn.LineStep()[2]);
          }
        }
        else
        {
          Ipp16s* ptr = image;

          for(int i = 0; i < size.height; i++)
          {
            im_err = IppCopy(p, dataOrderPn.LineStep()[2], tmpRow, tmpRowSize, sz);
            if(IE_OK != im_err)
              return im_err;

            im_err = IppThreshold(0, (1 << (image.Precision())) - 1, tmpRow, sz.width*3);
            if(IE_OK != im_err)
              return im_err;

            im_err = IppConvert(tmpRow, tmpRowSize, ptr, image.Step(), sz, 3);
            if(IE_OK != im_err)
              return im_err;

            ptr = (Ipp16s*)((Ipp8u*)ptr + image.Step());

            p[0] = (Ipp16s*)((Ipp8u*)p[0] + dataOrderPn.LineStep()[0]);
            p[1] = (Ipp16s*)((Ipp8u*)p[1] + dataOrderPn.LineStep()[1]);
            p[2] = (Ipp16s*)((Ipp8u*)p[2] + dataOrderPn.LineStep()[2]);
          }
        }

        ippFree(tmpRow);
      }

    }
    break;

  default:
    return IE_UNSUP;
  }

  return IE_OK;
} // ReadImageJPEG2000()


///////////////////////////////////////////////////////////////////////////////
// SAVE JPEG2000
///////////////////////////////////////////////////////////////////////////////

IM_ERROR SaveImageJPEG2000(CIppImage& image, PARAMS_JPEG2K& param, BaseStreamOutput& out)
{
  int                   i;
  int                   du;
  int                   normDataSize;
  int                   nOfComponents;
  Rect                  refgrid;
  Image                 imagePn;
  Point                 origin;
  RectSize              size;
  JP2Encoder            jp2enc;
  ImageDataPtr          dataPtr;
  ImageColorSpec        colorSpec;
  ImageDataOrder        dataOrder;
  ImageSamplingGeometry geometry;

  int       tmpRowSize;
  Ipp32s*   tmpRow = 0;

  nOfComponents = image.NChannels();

  IppiSize roi;
  IppiSize sz;

  roi.height = image.Height();
  roi.width  = image.Width();

  if(ExcStatusOk != jp2enc.Init())
    return IE_INIT;

  jp2enc.SetNOfThreads(param.nthreads);

  if(ExcStatusOk != jp2enc.AttachStream(out))
    return IE_ASTREAM;

  size.SetWidth(image.Width());
  size.SetHeight(image.Height());

  origin.SetX(0);
  origin.SetY(0);

  refgrid.SetOrigin(origin);
  refgrid.SetSize(size);

  geometry.SetRefGridRect(refgrid);
  geometry.ReAlloc(nOfComponents);
  geometry.SetEnumSampling(S444);

  du = sizeof(Ipp32s);
  dataOrder.SetDataType(T32s);
  dataOrder.ReAlloc(Plane, nOfComponents);

  for(i = 0; i < nOfComponents; i++)
  {
    dataOrder.PixelStep()[i] = NOfBytes(dataOrder.DataType());
    dataOrder.LineStep() [i] = geometry.RefGridRect().Width() * du;
  }

  imagePn.Buffer().ReAlloc(dataOrder, geometry);

  imagePn.ColorSpec().ReAlloc(nOfComponents);
  imagePn.ColorSpec().SetColorSpecMethod(Enumerated);
  imagePn.ColorSpec().SetComponentToColorMap(Direct);

  for(i = 0; i < nOfComponents; i++)
  {
    if(image.Precision() <= 8)
    {
      imagePn.ColorSpec().DataRange()[i].SetAsRange8u(255);
    }
    else
    {
      imagePn.ColorSpec().DataRange()[i].SetAsRange16u(1 << (image.Precision()-1));
    }
  }

  imagePn.ColorSpec().SetEnumColorSpace((nOfComponents == 1) ? Grayscale : RGB);

  switch(nOfComponents)
  {
  case 1:
    {
      if(image.Precision() <= 8)
      {
        Ipp8u* ptr = image;

        if(ippStsNoErr != ippiConvert_8u32s_C1R(ptr, image.Step(), imagePn.Buffer().DataPtr()[0].p32s, dataOrder.LineStep()[0], roi))
          return IE_PROCESS;
      }
      else
      {
        Ipp16s* ptr = image;
        if(ippStsNoErr != ippiConvert_16u32s_C1R((Ipp16u*)ptr,image.Step(),imagePn.Buffer().DataPtr()[0].p32s,dataOrder.LineStep()[0],roi))
          return IE_PROCESS;
      }
    }
    break;

  case 3:
    {
      sz.height = 1;
      sz.width  = roi.width;

      Ipp32s* p[3];

      p[0] = imagePn.Buffer().DataPtr()[0].p32s;
      p[1] = imagePn.Buffer().DataPtr()[1].p32s;
      p[2] = imagePn.Buffer().DataPtr()[2].p32s;

      tmpRowSize = roi.width * nOfComponents * sizeof(Ipp32s);
      tmpRow = (Ipp32s*)ippMalloc(tmpRowSize);

      if(NULL == tmpRow)
        return IE_ALLOC;

      if(image.Precision() <= 8)
      {
        Ipp8u* ptr = image;

        for(i = 0; i < image.Height(); i++)
        {
          if(ippStsNoErr != ippiConvert_8u32s_C3R(ptr,image.Step(),tmpRow, tmpRowSize, sz))
            return IE_PROCESS;

          if(ippStsNoErr != ippiCopy_32s_C3P3R(tmpRow, tmpRowSize, p, dataOrder.LineStep()[0], sz))
            return IE_PROCESS;

          ptr += image.Step();

          p[0] = (Ipp32s*)((Ipp8u*)p[0] + dataOrder.LineStep()[0]);
          p[1] = (Ipp32s*)((Ipp8u*)p[1] + dataOrder.LineStep()[1]);
          p[2] = (Ipp32s*)((Ipp8u*)p[2] + dataOrder.LineStep()[2]);
        }
      }
      else
      {
        Ipp16s* ptr = image;

        for(i = 0; i < image.Height(); i++)
        {
          if(ippStsNoErr != ippiConvert_16u32s_C3R((Ipp16u*)ptr,image.Step(),tmpRow, tmpRowSize, sz))
            return IE_PROCESS;

          if(ippStsNoErr != ippiCopy_32s_C3P3R(tmpRow, tmpRowSize, p, dataOrder.LineStep()[0], sz))
            return IE_PROCESS;

          ptr = (Ipp16s*)((Ipp8u*)ptr + image.Step());
          p[0] = (Ipp32s*)((Ipp8u*)p[0] + dataOrder.LineStep()[0]);
          p[1] = (Ipp32s*)((Ipp8u*)p[1] + dataOrder.LineStep()[1]);
          p[2] = (Ipp32s*)((Ipp8u*)p[2] + dataOrder.LineStep()[2]);
        }
      }
      ippFree(tmpRow);
    }
    break ;

   case 4:
  default:
    return IE_UNSUP;
    break;
  }

  if(ExcStatusOk != jp2enc.AttachImage(imagePn))
    return IE_AIMAGE;

    normDataSize = image.Step() * image.Height();

    if(ExcStatusOk != jp2enc.SetParams(5,(bool)param.useMCT,!((bool)param.wt),false,0,0,
                            (int)(normDataSize * (param.quality / 100.0))))
      return IE_PARAMS;

  if(ExcStatusOk != jp2enc.WriteHeader())
    return IE_WHEADER;

  if(ExcStatusOk != jp2enc.WriteData())
    return IE_WDATA;

  return IE_OK;
} // SaveImageJPEG2000()

