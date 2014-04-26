/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2008-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"

#ifndef __JPEGXR_H__
#include "jpegxr.h"
#endif
#ifndef __UIC_JPEGXR_DEC_H__
#include "uic_jpegxr_dec.h"
#endif
#ifndef __UIC_JPEGXR_ENC_H__
#include "uic_jpegxr_enc.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif

using namespace UIC;


static void CheckParam(Ipp32s &iValue, Ipp32s iMin, Ipp32s iMax)
{
  if(iMax == iMin)
    iValue = iMax;

  if(iValue > iMax)
    iValue = iMax;
  else if(iValue < iMin)
    iValue = iMin;
} // CheckParam()


static IM_ERROR ReadImagePlane(
  BaseStreamInput&,
  JPEGXRDecoder&   decoder,
  PARAMS_JPEGXR&   param,
  Ipp32u           iImageOffset,
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

  if(ExcStatusOk != decoder.SetPlaneOffset(iImageOffset))
    return IE_PARAMS;

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
  else if(dataType == T16f || dataType == T32f)
    bFloat = 1;

  if(ExcStatusOk != decoder.SetParams(JXR_OCF_AUTO, param.bands, param.thread_mode))
    return IE_PARAMS;

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

  image.Color((IM_COLOR)image.UicToIppColor(colorSpec.EnumColorSpace()));

  if(bSigned)
    image.Format(IF_FIXED);
  else if(bFloat)
    image.Format(IF_FLOAT);
  else
    image.Format(IF_UNSIGNED);

  if(0 != image.Alloc(roi, iChannels, iPrecision, 1))
    return IE_ALLOC;

  dataPtr.p8u = image;
  imageCn.Buffer().Attach(&dataPtr, dataOrder, geometry);

  if(ExcStatusOk != decoder.ReadData(imageCn.Buffer().DataPtr(), dataOrder))
    return IE_RDATA;

  if(ExcStatusOk != decoder.FreeData())
    return IE_RESET;

  return IE_OK;
} // ReadImagePlane()


IM_ERROR ReadImageJPEGXR(
  BaseStreamInput& in,
  PARAMS_JPEGXR&   param,
  CIppImage&       image)
{
  Ipp32u   iPAMode = JXR_PM_BOTH;
  Ipp32u   iImageOffset = 0;
  Ipp32u   iAlphaOffset = 0;
  Ipp8u    bPreAlpha    = 0;
  IM_ERROR imStatus;

  JPEGXRDecoder decoder;

  CheckParam(param.threads,     0, 65536);
  CheckParam(param.thread_mode, 0, 2);
  CheckParam(param.bands,       0, 3);

  if(ExcStatusOk != decoder.Init())
    return IE_INIT;

  if(ExcStatusOk != decoder.SetParams(JXR_OCF_AUTO, param.bands, param.thread_mode))
    return IE_PARAMS;

  decoder.SetNOfThreads(param.threads);

  if(ExcStatusOk != decoder.AttachStream(in))
    return IE_ASTREAM;

  if(ExcStatusOk != decoder.ReadFileHeader(iImageOffset, iAlphaOffset, bPreAlpha))
    return IE_RHEADER;

  if(iAlphaOffset)
  {
    if(iPAMode == JXR_PM_BOTH)
    {
      CIppImage   imagePrimary;
      CIppImage   imageAlpha;
      Ipp32u iCh, iChPr, iChAl;
      Ipp32u iWidth, iHeight, iStep, iStepPr, iStepAl, iPrecision;
      IppiSize iROI;
      Ipp8u *pData, *pDataPr, *pDataAl;
      Ipp32u i, j, k;

      if(IE_OK != (imStatus = ReadImagePlane(in, decoder, param, iImageOffset, imagePrimary)))
        return imStatus;
      if(IE_OK != (imStatus = ReadImagePlane(in, decoder, param, iAlphaOffset, imageAlpha)))
        return imStatus;

      iChPr = imagePrimary.NChannels();
      iChAl = imageAlpha.NChannels();
      iROI.width = imagePrimary.Width();
      iROI.height = imagePrimary.Height();
      iWidth = imagePrimary.Width();
      iHeight = imagePrimary.Height();
      iStepPr = imagePrimary.Step();
      iStepAl = imageAlpha.Step();
      pDataPr = imagePrimary;
      pDataAl = imageAlpha;
      iPrecision = imagePrimary.Precision();
      iCh = iChPr + iChAl;

      switch(imagePrimary.Color())
      {
      case IC_RGB:     image.Color((bPreAlpha)?IC_RGBAP:IC_RGBA); break;
      case IC_BGR:     image.Color((bPreAlpha)?IC_BGRAP:IC_BGRA); break;
      case IC_CMYK:    image.Color(IC_CMYKA);                     break;
      case IC_YCBCR:   image.Color(IC_YCCK);                      break;
      case IC_UNKNOWN: image.Color(IC_UNKNOWN);                   break;
      default:
        return IE_RDATA;
      }

      image.Format(imagePrimary.Format());
      if(0 != image.Alloc(iWidth, iHeight, iCh, iPrecision, 1))
        return IE_ALLOC;

      iStep = image.Step();
      pData = image;

      if(iPrecision == 8)
      {
        if(iChPr == 3)
        {
          ippiCopy_8u_C3AC4R(pDataPr, iStepPr, pData, iStep, iROI);
          ippiCopy_8u_C1C4R(pDataAl, iStepAl, pData + 3, iStep, iROI);
        }
        else
        {
          for(i = 0; i < iHeight; i++)
          {
            Ipp8u* pSrcPr = pDataPr + iStepPr * i;
            Ipp8u* pSrcAl = pDataAl + iStepAl * i;
            Ipp8u* pDst = pData + iStep * i;
            for(j = 0; j < iWidth; j++, pDst += iCh, pSrcPr += iChPr, pSrcAl++)
            {
              for(k = 0; k < iChPr; k++)
                pDst[k] = pSrcPr[k];
              pDst[iChPr] = pSrcAl[0];
            }
          }
        }
      }
      else if(iPrecision == 16)
      {
        if(iChPr == 3)
        {
          ippiCopy_16s_C3AC4R((Ipp16s*)pDataPr, iStepPr, (Ipp16s*)pData, iStep, iROI);
          ippiCopy_16s_C1C4R((Ipp16s*)pDataAl, iStepAl, (Ipp16s*)(pData) + 3, iStep, iROI);
        }
        else
        {
          for(i = 0; i < iHeight; i++)
          {
            Ipp16u* pSrcPr = (Ipp16u*)(pDataPr + iStepPr * i);
            Ipp16u* pSrcAl = (Ipp16u*)(pDataAl + iStepAl * i);
            Ipp16u* pDst = (Ipp16u*)(pData + iStep * i);
            for(j = 0; j < iWidth; j++, pDst += iCh, pSrcPr += iChPr, pSrcAl++)
            {
              for(k = 0; k < iChPr; k++)
                pDst[k] = pSrcPr[k];
              pDst[iChPr] = pSrcAl[0];
            }
          }
        }
      }
      else
      {
        if(iChPr == 3)
        {
          ippiCopy_32s_C3AC4R((Ipp32s*)pDataPr, iStepPr, (Ipp32s*)pData, iStep, iROI);
          ippiCopy_32s_C1C4R((Ipp32s*)pDataAl, iStepAl, (Ipp32s*)(pData) + 3, iStep, iROI);
        }
      }
    }
    else if(iPAMode == JXR_PM_IMAGE)
    {
      if(IE_OK != (imStatus = ReadImagePlane(in, decoder, param, iImageOffset, image)))
        return imStatus;
    }
    else if(iPAMode == JXR_PM_ALPHA)
    {
      if(IE_OK != (imStatus = ReadImagePlane(in, decoder, param, iAlphaOffset, image)))
        return imStatus;
    }
  }
  else
  {
    if(IE_OK != (imStatus = ReadImagePlane(in, decoder, param, iImageOffset, image)))
      return imStatus;
  }

  return IE_OK;
} // ReadImageJPEGXR()


IM_ERROR SaveImageJPEGXR(
  CIppImage&        image,
  PARAMS_JPEGXR&    param,
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
  Ipp32u bAlphaPresent = 0;
  Ipp16u iTilesUniform[4] = {0};
  Ipp32u iWidth = image.Width();
  Ipp32u iHeight = image.Height();
  InputParams inParams;
  Ipp32u i;

  JPEGXREncoder encoder;

  CheckParam(param.tiles_uniform[0], 1, (iWidth < 16)?1:iWidth/16);
  CheckParam(param.tiles_uniform[1], 1, (iHeight < 16)?1:iHeight/16);
  CheckParam(param.tiles_uniform[2], 0, (iWidth < 16)?0:iWidth/16);
  CheckParam(param.tiles_uniform[3], 0, (iHeight < 16)?0:iHeight/16);
  CheckParam(param.overlap,          0, 2);
  CheckParam(param.quality,          1, 255);
  CheckParam(param.aquality,         1, 255);
  CheckParam(param.bands,            0, 3);
  CheckParam(param.bitstream,        0, 1);
  CheckParam(param.cmyk_direct,      0, 1);
  CheckParam(param.sampling,         0, 2);
  CheckParam(param.trim,             0, 15);
  CheckParam(param.bits_shift,      -1, 31);
  CheckParam(param.alpha_mode,       0, 1);

  // Params override
  param.bands = 0;
  param.bitstream = 0;
  param.sampling = 0;

  if(ExcStatusOk != encoder.Init())
    return IE_INIT;

  iTilesUniform[0] = param.tiles_uniform[0];
  iTilesUniform[1] = param.tiles_uniform[1];
  iTilesUniform[2] = param.tiles_uniform[2];
  iTilesUniform[3] = param.tiles_uniform[3];

  inParams.iQuality      = param.quality;
  inParams.iOverlap      = param.overlap;
  inParams.iBands        = param.bands;
  inParams.iSampling     = param.sampling;
  inParams.iTrim         = param.trim;
  inParams.bFrequency    = param.bitstream;
  inParams.bCMYKD        = param.cmyk_direct;
  inParams.iShift        = param.bits_shift;
  inParams.bAlphaPlane   = 0;
  inParams.pTilesUniform = iTilesUniform;

  if(ExcStatusOk != encoder.SetParams(inParams))
    return IE_INIT;

  if(ExcStatusOk != encoder.AttachStream(out))
    return IE_ASTREAM;

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
    else
    {
      if(iFormat == IF_UNSIGNED)
        imageCn.ColorSpec().DataRange()[i].SetAsRange32u(IPP_MAX_32U);
      else if(iFormat == IF_FIXED)
        imageCn.ColorSpec().DataRange()[i].SetAsRange32s(IPP_MIN_32S, IPP_MAX_32S);
      else
        imageCn.ColorSpec().DataRange()[i].SetAsRange32f((Ipp32f)IPP_MIN_32S, (Ipp32f)IPP_MAX_32S);
    }
  }

  dataPtr.p8u = image;
  imageCn.Buffer().Attach(&dataPtr, dataOrder, geometry);

  if(ExcStatusOk != encoder.AttachImage(imageCn))
    return IE_AIMAGE;

  if(ExcStatusOk != encoder.WriteFileHeader(param.alpha_mode))
    return IE_WHEADER;

  // Planar alpha support
  switch(image.Color())
  {
  case RGBA:
  case BGRA:
  case RGBAP:
  case BGRAP:
  case CMYKA:
    bAlphaPresent = 1;
    break;
  }

  if(bAlphaPresent && param.alpha_mode == 1)
  {
    CIppImage imagePrimary;
    CIppImage imageAlpha;
    IppiSize  iROI = {image.Width(), image.Height()};
    Ipp8u*    pData = image;
    Ipp8u*    pDataPr;
    Ipp8u*    pDataAl;
    Ipp32u    iStep = image.Step();
    Ipp32u    iPStep, iAStep;
    Ipp32u    i, j;

    if(0 != imagePrimary.Alloc(image.Width(), image.Height(), iChannels - 1, iPrecision, 1))
      return IE_ALLOC;

    if(0 != imageAlpha.Alloc(image.Width(), image.Height(), 1, iPrecision, 1))
      return IE_ALLOC;

    pDataPr = imagePrimary;
    iPStep  = imagePrimary.Step();
    pDataAl = imageAlpha;
    iAStep  = imageAlpha.Step();

    if(iPrecision == 8)
    {
      if(image.Color() == IC_CMYKA)
      {
        for(i = 0; i < iHeight; i++)
        {
          Ipp8u* pSrc = pData + iStep * i;
          Ipp8u* pDstPr = pDataPr + iPStep * i;
          Ipp8u* pDstAl = pDataAl + iAStep * i;
          for(j = 0; j < iWidth; j++, pSrc += 5, pDstPr += 4, pDstAl++)
          {
            pDstPr[0] = pSrc[0];
            pDstPr[1] = pSrc[1];
            pDstPr[2] = pSrc[2];
            pDstPr[3] = pSrc[3];
            pDstAl[0] = pSrc[4];
          }
        }
      }
      else
      {
        ippiCopy_8u_AC4C3R(pData, iStep, pDataPr, iPStep, iROI);
        ippiCopy_8u_C4C1R(pData + 3, iStep, pDataAl, iAStep, iROI);
      }
    }
    else if(iPrecision == 16)
    {
      if(image.Color() == IC_CMYKA)
      {
        for(i = 0; i < iHeight; i++)
        {
          Ipp16u* pSrc = (Ipp16u*)(pData + iStep * i);
          Ipp16u* pDstPr = (Ipp16u*)(pDataPr + iPStep * i);
          Ipp16u* pDstAl = (Ipp16u*)(pDataAl + iAStep * i);
          for(j = 0; j < iWidth; j++, pSrc += 5, pDstPr += 4, pDstAl++)
          {
            pDstPr[0] = pSrc[0];
            pDstPr[1] = pSrc[1];
            pDstPr[2] = pSrc[2];
            pDstPr[3] = pSrc[3];
            pDstAl[0] = pSrc[4];
          }
        }
      }
      else
      {
        ippiCopy_16s_AC4C3R((Ipp16s*)pData, iStep, (Ipp16s*)pDataPr, iPStep, iROI);
        ippiCopy_16s_C4C1R((Ipp16s*)pData + 3, iStep, (Ipp16s*)pDataAl, iAStep, iROI);
      }
    }
    else
    {
      ippiCopy_32s_AC4C3R((Ipp32s*)pData, iStep, (Ipp32s*)pDataPr, iPStep, iROI);
      ippiCopy_32s_C4C1R((Ipp32s*)pData + 3, iStep, (Ipp32s*)pDataAl, iAStep, iROI);
    }

    geometry.SetRefGridRect(refgrid);
    geometry.ReAlloc(iChannels - 1);
    geometry.SetEnumSampling(UIC::S444);

    dataOrder.ReAlloc(Interleaved, iChannels - 1);
    dataOrder.PixelStep()[0] = (iChannels - 1) * iDU;
    dataOrder.LineStep() [0] = iPStep;

    switch(imageCn.ColorSpec().EnumColorSpace())
    {
    case BGRA:  imageCn.ColorSpec().SetEnumColorSpace(BGR);  break;
    case RGBA:  imageCn.ColorSpec().SetEnumColorSpace(RGB);  break;
    case BGRAP: imageCn.ColorSpec().SetEnumColorSpace(BGR);  break;
    case RGBAP: imageCn.ColorSpec().SetEnumColorSpace(RGB);  break;
    case CMYKA: imageCn.ColorSpec().SetEnumColorSpace(CMYK); break;
    default:
      return IE_INIT;
    }

    dataPtr.p8u = pDataPr;
    imageCn.Buffer().Attach(&dataPtr, dataOrder, geometry);

    if(ExcStatusOk != encoder.AttachImage(imageCn))
      return IE_AIMAGE;

    if(ExcStatusOk != encoder.WriteHeader())
      return IE_WHEADER;

    if(ExcStatusOk != encoder.WriteData())
      return IE_WDATA;

    if(ExcStatusOk != encoder.FreeData())
      return IE_RESET;

    geometry.SetRefGridRect(refgrid);
    geometry.ReAlloc(1);
    geometry.SetEnumSampling(UIC::S444);

    dataOrder.ReAlloc(Interleaved, 1);
    dataOrder.PixelStep()[0] = iDU;
    dataOrder.LineStep() [0] = iAStep;

    imageCn.ColorSpec().SetEnumColorSpace(Grayscale);

    dataPtr.p8u = pDataAl;
    imageCn.Buffer().Attach(&dataPtr, dataOrder, geometry);

    inParams.iQuality    = param.aquality;
    inParams.bAlphaPlane = 1;

    if(ExcStatusOk != encoder.SetParams(inParams))
      return IE_INIT;

    if(ExcStatusOk != encoder.AttachImage(imageCn))
      return IE_AIMAGE;

    if(ExcStatusOk != encoder.WriteHeader())
      return IE_WHEADER;

    if(ExcStatusOk != encoder.WriteData())
      return IE_WDATA;

    if(ExcStatusOk != encoder.FreeData())
      return IE_RESET;
  }
  else
  {
    if(ExcStatusOk != encoder.WriteHeader())
      return IE_WHEADER;

    if(ExcStatusOk != encoder.WriteData())
      return IE_WDATA;

    if(ExcStatusOk != encoder.FreeData())
      return IE_RESET;
  }

  return IE_OK;
} // SaveImageJPEGXR()

