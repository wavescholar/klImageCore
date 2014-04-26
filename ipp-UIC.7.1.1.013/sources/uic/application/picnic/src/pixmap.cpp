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
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QVector>

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif
#ifndef __PIXMAP_H__
#include "pixmap.h"
#endif


static bool imageSetColorTable(QImage* qimg)
{
  int i = 0;
  QVector<QRgb> colors(256);

  qimg->setNumColors(256);

  for(i = 0; i < 256; i++)
  {
    colors[i] = qRgb(i,i,i);
  }

  qimg->setColorTable(colors);

  return true;
} // imageSetColorTable()


QPixmap CreateQPixmap(Ipp8u* pImg, IppiSize size, int nchannels)
{
  int res = 0;
  int order[4];
  QImage::Format fmt;
  QPixmap   pm;
  IppStatus status;

  switch(nchannels)
  {
  case 1: fmt = QImage::Format_Indexed8; break;
  case 3: fmt = QImage::Format_RGB32;    break;
  case 4: fmt = QImage::Format_ARGB32;   break;
  default:
    break;
  }

  QImage image(size.width,size.height,fmt);

  if(image.isNull())
    return 0;

  switch(nchannels)
  {
  case 1:
    status = ippiCopy_8u_C1R(pImg,size.width,image.bits(),image.bytesPerLine(),size);
    res = imageSetColorTable(&image);
    break;

  case 3:
    order[0] = 2; order[1] = 1; order[2] = 0;
    status = ippiSwapChannels_8u_C3IR(pImg,size.width*nchannels,size,order);
    status = ippiCopy_8u_C3AC4R(pImg,size.width*nchannels,image.bits(),image.bytesPerLine(),size);
    status = ippiSet_8u_C4CR(0xff,image.bits()+3,image.bytesPerLine(),size);
    break;

  case 4:
    order[0] = 2; order[1] = 1; order[2] = 0; order[3] = 3;
    status = ippiSwapChannels_8u_C4IR(pImg,size.width*nchannels,size,order);
    status = ippiCopy_8u_C4R(pImg,size.width*nchannels,image.bits(),image.bytesPerLine(),size);
    break;

  default:
    break;
  }

  if(ippStsNoErr != status)
    return 0;

  pm = pm.fromImage(image, Qt::AutoColor);

  return pm;
} // CreateQPixmap()


QPixmap CreateQPixmap(CIppImage& src)
{
  int r = 0;
  int order[4] = { 2, 1, 0, 3 };
  QImage::Format fmt;
  IppStatus status;

  switch(src.NChannels())
  {
  case 1: fmt = QImage::Format_Indexed8; break;
  case 3: fmt = QImage::Format_RGB32;    break; // it is BGR actually
  case 4: fmt = QImage::Format_ARGB32;   break; // it is BGRA actually
  default:
    break;
  }

  if(src.Width() <= 0 && src.Height() <= 0)
    return 0;

  QImage image(src.Width(),src.Height(),fmt);
  if(image.isNull())
    return 0;

  switch(src.NChannels())
  {
  case 1:
    status = ippiCopy_8u_C1R(src.DataPtr(),src.Step(),image.bits(),image.bytesPerLine(),src.Size());
    if(ippStsNoErr != status)
      return 0;
    r = imageSetColorTable(&image);
    break;

  case 3:
    status = ippiCopy_8u_C3AC4R(src.DataPtr(),src.Step(),image.bits(),image.bytesPerLine(),src.Size());
    status = ippiSet_8u_C4CR(0xff,image.bits()+3,image.bytesPerLine(),src.Size());
    if(IC_RGB == src.Color())
      status = ippiSwapChannels_8u_C4IR(image.bits(),image.bytesPerLine(),src.Size(),order);
    break;

  case 4:
    {
//      src.FillAlpha_8u(0xff);
      status = ippiCopy_8u_C4R(src.DataPtr(),src.Step(),image.bits(),image.bytesPerLine(),src.Size());
      if(IC_RGB == src.Color() || IC_RGBA == src.Color())
        status = ippiSwapChannels_8u_C4IR(image.bits(),image.bytesPerLine(),src.Size(),order);
      break;
    }

  default:
    break;
  }

  QPixmap pm;
  pm = QPixmap::fromImage(image);

  return pm;
} // CreateQPixmap()

