/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2001-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifdef WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif

#include <stdio.h>
#include <stdlib.h>


#include <fcntl.h>
#include <string.h>

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCC_H__
#include "ippcc.h"
#endif
#ifndef __STDFILEIN_H__
#include "stdfilein.h"
#endif
#ifndef __MEMBUFFIN_H__
#include "membuffin.h"
#endif
#ifndef __JPEGBASE_H__
#include "jpegbase.h"
#endif
#ifndef __JPEG_H__
#include "jpeg.h"
#endif
#ifndef __JPEG2K_H__
#include "jpeg2k.h"
#endif
#ifndef __DICOM_H__
#include "dicom.h"
#endif

using namespace UIC;

#define DICOM_PREAMBLE 128
#define MAKE_VR(a,b) (((a) << 8) | (b))

typedef enum
{
  VR_AE = MAKE_VR('A','E'),
  VR_AS = MAKE_VR('A','S'),
  VR_AT = MAKE_VR('A','T'),
  VR_CS = MAKE_VR('C','S'),
  VR_DA = MAKE_VR('D','A'),
  VR_DS = MAKE_VR('D','S'),
  VR_DT = MAKE_VR('D','T'),
  VR_FL = MAKE_VR('F','L'),
  VR_FD = MAKE_VR('F','D'),
  VR_IS = MAKE_VR('I','S'),
  VR_LO = MAKE_VR('L','O'),
  VR_LT = MAKE_VR('L','T'),
  VR_OB = MAKE_VR('O','B'),
  VR_OF = MAKE_VR('O','F'),
  VR_OW = MAKE_VR('O','W'),
  VR_PN = MAKE_VR('P','N'),
  VR_SH = MAKE_VR('S','H'),
  VR_SL = MAKE_VR('S','L'),
  VR_SQ = MAKE_VR('S','Q'),
  VR_SS = MAKE_VR('S','S'),
  VR_ST = MAKE_VR('S','T'),
  VR_TM = MAKE_VR('T','M'),
  VR_UI = MAKE_VR('U','I'),
  VR_UL = MAKE_VR('U','L'),
  VR_UN = MAKE_VR('U','N'),
  VR_US = MAKE_VR('U','S'),
  VR_UT = MAKE_VR('U','T'),
  VR_OX = MAKE_VR(0,0)

} DICOM_VR;


typedef enum
{
  DPC_PIXEL = 0,
  DPC_PLANE = 1

} DICOM_ORDER;


typedef union
{
  struct
  {
    unsigned short group;
    unsigned short element;
  };

  unsigned int u;

} DICOM_TAG;


typedef union
{
  DICOM_TAG*      VR_AT;
  double*         VR_FD;
  float*          VR_FL;
  unsigned int*   VR_UL;
  int*            VR_SL;
  unsigned char*  VR_OB;
  unsigned short* VR_OW;
  float*          VR_OF;
  unsigned short* VR_US;
  short*          VR_SS;
  char**          VR_AE;
  char**          VR_AS;
  char**          VR_CS;
  char**          VR_DA;
  char**          VR_DS;
  char**          VR_DT;
  char**          VR_IS;
  char**          VR_LO;
  char**          VR_LT;
  char**          VR_PN;
  char**          VR_SH;
  char**          VR_ST;
  char**          VR_TM;
  char**          VR_UI;
  char**          VR_UT;
  void*           VR_SQ;
  void*           VR_UN;

} DICOM_VALUE;


typedef struct
{
  DICOM_TAG    tag;
  DICOM_VR     vr;
  unsigned int length;
  DICOM_VALUE  value;

} DICOM_ELEMENT;


typedef struct
{
  DICOM_TAG    tag;
  unsigned int length;
  DICOM_VALUE  value;

} DICOM_SEQ_ITEM;


char* vr2str(DICOM_VR vr)
{
  char* str;

  switch(vr)
  {
  case VR_AE: str = "AE"; break;
  case VR_AS: str = "AS"; break;
  case VR_AT: str = "AT"; break;
  case VR_CS: str = "CS"; break;
  case VR_DA: str = "DA"; break;
  case VR_DS: str = "DS"; break;
  case VR_DT: str = "DT"; break;
  case VR_FL: str = "FL"; break;
  case VR_FD: str = "FD"; break;
  case VR_IS: str = "IS"; break;
  case VR_LO: str = "LO"; break;
  case VR_LT: str = "LT"; break;
  case VR_OB: str = "OB"; break;
  case VR_OF: str = "OF"; break;
  case VR_OW: str = "OW"; break;
  case VR_PN: str = "PN"; break;
  case VR_SH: str = "SH"; break;
  case VR_SL: str = "SL"; break;
  case VR_SQ: str = "SQ"; break;
  case VR_SS: str = "SS"; break;
  case VR_ST: str = "ST"; break;
  case VR_TM: str = "TM"; break;
  case VR_UI: str = "UI"; break;
  case VR_UL: str = "UL"; break;
  case VR_UN: str = "UN"; break;
  case VR_US: str = "US"; break;
  case VR_UT: str = "UT"; break;
  default:    str = "unknown"; break;
  }

  return str;
} // vr2str()


BaseStream::TStatus read_dicom_i32(
  BaseStreamInput& in,
  int*             val)
{
  BaseStream::TSize cnt;

  return in.Read(val,4,cnt);
} // read_dicom_i32()


BaseStream::TStatus read_dicom_tag(
  BaseStreamInput& in,
  DICOM_TAG*       tag)
{
  BaseStream::TSize   cnt;
  BaseStream::TStatus status;

  status = in.Read(&tag->group,2,cnt);
  if(UIC::BaseStream::StatusOk != status)
    return status;

  status = in.Read(&tag->element,2,cnt);
  if(UIC::BaseStream::StatusOk != status)
    return status;

#ifdef __LOG__
  printf("TAG: 0x%04X, {group:element:0x%02X:0x%02X}\n",tag->u,tag->group,tag->element);
#endif

  return BaseStream::StatusOk;
} // read_dicom_tag()


BaseStream::TStatus read_dicom_vr(
  BaseStreamInput& in,
  DICOM_VR*        vr)
{
  unsigned char       bh;
  unsigned char       bl;
  BaseStream::TSize   cnt;
  BaseStream::TStatus status;

  *vr = (DICOM_VR)0;

  status = in.Read(&bh,1,cnt);
  if(UIC::BaseStream::StatusOk != status)
    return status;

  status = in.Read(&bl,1,cnt);
  if(UIC::BaseStream::StatusOk != status)
    return status;

  *vr = (DICOM_VR)((bh << 8) | bl);

#ifdef __LOG__
  printf("  VR: 0x%02X, {%s}\n",*vr,vr2str(*vr));
#endif

  return BaseStream::StatusOk;
} // read_dicom_vr()


BaseStream::TStatus read_dicom_len(
  BaseStreamInput& in,
  DICOM_VR         vr,
  int*             length)
{
  int                 sz;
  short               tmp;
  BaseStream::TSize   cnt;
  BaseStream::TStatus status;

  *length = 0;

  switch(vr)
  {
  case VR_OB:
  case VR_OW:
  case VR_SQ:
  case VR_UN:
  case VR_UT:
    // skip 2 bytes
    status = in.Read(&tmp,2,cnt);
    if(BaseStream::StatusOk != status)
      return status;

    sz = 4;
    break;

  default:
    sz = 2;
  }

  status = in.Read(length,sz,cnt);
  if(BaseStream::StatusOk != status)
    return status;

#ifdef __LOG__
  printf("  length: 0x%04X\n",*length);
#endif

  return BaseStream::StatusOk;
} // read_dicom_len()


BaseStream::TStatus read_dicom_val(
  BaseStreamInput& in,
  DICOM_VR         vr,
  int*             len,
  DICOM_VALUE*     value)
{
#ifdef __LOG__
  int        i;
#endif
  int        sz;
  int        length;
  char*      str = 0;

  BaseStream::TSize   cnt;
  BaseStream::TStatus status;

  length = *len;
  sz = length;

  if(length == 0)
    return BaseStream::StatusOk;

  if(length != 0xffffffff)
  {
    if(length < sizeof(Ipp8u*))
      sz = sizeof(Ipp8u*);

    str = (char*)ippMalloc(sz);
    if(NULL == str)
    {
      return BaseStream::StatusFail;
    }

    status = in.Read(str,length,cnt);
    if(BaseStream::StatusOk != status)
      return status;
  }

  switch(vr)
  {
  case VR_AE:
  case VR_AS:
  case VR_AT:
  case VR_CS:
  case VR_DA:
  case VR_DS:
  case VR_DT:
  case VR_IS:
  case VR_LO:
  case VR_LT:
  case VR_PN:
  case VR_SH:
  case VR_ST:
  case VR_TM:
  case VR_UI:
  case VR_UT:  value->VR_AE = (char**)str;   break;
  case VR_FL:  value->VR_FL = (float*)str;   break;
  case VR_FD:  value->VR_FD = (double*)str;  break;
  case VR_OF:  value->VR_OF = (float*)str;   break;
  case VR_SS:  value->VR_SS = (short*)str;   break;
  case VR_UL:  value->VR_UL = (unsigned int*)str;   break;
  case VR_UN:  value->VR_UN = str;  break;
  case VR_US:  value->VR_US = (unsigned short*)str;  break;
  case VR_SL:  value->VR_SL = (int*)str;   break;

  case VR_OB:
    if(length == 0xffffffff)
    {
      DICOM_ELEMENT item;

      do
      {
        status = read_dicom_tag(in, &item.tag);
        if(BaseStream::StatusOk != status)
          return status;

        status = read_dicom_i32(in,(int*)&item.length);
        if(BaseStream::StatusOk != status)
          return status;

        if(item.tag.u == 0xe00dfffe || item.tag.u == 0xe0ddfffe)
        {
          break;
        }

        if(item.length == 0xffffffff)
        {
          DICOM_ELEMENT seq_element;

          while(1)
          {
            status = read_dicom_tag(in, &seq_element.tag);
            if(BaseStream::StatusOk != status)
              return status;

            if(seq_element.tag.u == 0xe00dfffe)
            {
              status = read_dicom_i32(in, (int*)&seq_element.length);
              if(BaseStream::StatusOk != status)
                return status;
              break;
            }

            status = read_dicom_vr(in, &seq_element.vr);
            if(BaseStream::StatusOk != status)
              return status;

            status = read_dicom_len(in, seq_element.vr, (int*)&seq_element.length);
            if(BaseStream::StatusOk != status)
              return status;

            status = read_dicom_val(in, seq_element.vr, (int*)&seq_element.length, &seq_element.value);
            if(BaseStream::StatusOk != status)
              return status;
          }

          status = read_dicom_tag(in, &seq_element.tag);
          if(BaseStream::StatusOk != status)
            return status;

          status = read_dicom_i32(in, (int*)&seq_element.length);
          if(BaseStream::StatusOk != status)
            return status;
        }
        else
        {
          if(item.length != 0)
          {
            str = (char*)ippMalloc(item.length);
            if(NULL == str)
            {
              return UIC::BaseStream::StatusFail;
            }

            value->VR_OB = (unsigned char*)str;

            status = in.Read(value->VR_OB,item.length, cnt);
            if(BaseStream::StatusOk != status)
              return status;

            *len = item.length;
          }
          else
          {
            value->VR_OB = 0;
            *len = 0;
          }
        }
      } while(1);
    }
    else
    {
      value->VR_OB = (unsigned char*)str;
    }
    break;

  case VR_OW:
    if(length == 0xffffffff)
    {
      DICOM_ELEMENT item;

      do
      {
        status = read_dicom_tag(in,&item.tag);
        if(UIC::BaseStream::StatusOk != status)
          return status;

        status = read_dicom_i32(in,(int*)&item.length);

        if(item.tag.u == 0xe00dfffe || item.tag.u == 0xe0ddfffe)
        {
          break;
        }

        if(item.length == 0xffffffff)
        {
          DICOM_ELEMENT seq_element;

          while(1)
          {
            status = read_dicom_tag(in,&seq_element.tag);
            if(BaseStream::StatusOk != status)
              return status;

            if(seq_element.tag.u == 0xe00dfffe)
            {
              status = read_dicom_i32(in,(int*)&seq_element.length);
              if(BaseStream::StatusOk != status)
                return status;
              break;
            }

            status = read_dicom_vr(in,&seq_element.vr);
            if(BaseStream::StatusOk != status)
              return status;

            status = read_dicom_len(in,seq_element.vr,(int*)&seq_element.length);
            if(BaseStream::StatusOk != status)
              return status;

            status = read_dicom_val(in,seq_element.vr,(int*)&seq_element.length,&seq_element.value);
            if(BaseStream::StatusOk != status)
              return status;
          }

          status = read_dicom_tag(in,&seq_element.tag);
          if(BaseStream::StatusOk != status)
            return status;

          status = read_dicom_i32(in,(int*)&seq_element.length);
          if(BaseStream::StatusOk != status)
            return status;
        }
        else
        {
          if(item.length != 0)
          {
            str = (char*)ippMalloc(item.length);
            if(NULL == str)
            {
              return BaseStream::StatusFail;
            }

            value->VR_OW = (unsigned short*)str;

            status = in.Read(value->VR_OW,item.length,cnt);
            if(BaseStream::StatusOk != status)
              return status;

            *len = item.length;
          }
          else
          {
            value->VR_OW = 0;
            *len = 0;
          }
        }
      } while(1);
    }
    else
    {
      value->VR_OW = (unsigned short*)str;
    }
    break;

  case VR_SQ:
    if(length == 0xffffffff)
    {
      DICOM_ELEMENT item;

      status = read_dicom_tag(in,&item.tag);
      if(BaseStream::StatusOk != status)
        return status;

      status = read_dicom_i32(in,(int*)&item.length);
      if(BaseStream::StatusOk != status)
        return status;

      do
      {
        if(item.length == 0)
          break;

        if(item.length == 0xffffffff)
        {
          DICOM_ELEMENT seq_element;

          seq_element.length      = 0;
          seq_element.tag.u       = 0;
          seq_element.value.VR_UN = 0;
          seq_element.vr          = VR_OX;

          while(1)
          {
            status = read_dicom_tag(in,&seq_element.tag);
            if(BaseStream::StatusOk != status)
              return status;

            if(seq_element.tag.u == 0xe00dfffe || seq_element.tag.u == 0xe0ddfffe)
            {
              status = read_dicom_i32(in,(int*)&seq_element.length);
              if(BaseStream::StatusOk != status)
                return status;
              break;
            }

            status = read_dicom_vr(in,&seq_element.vr);
            if(BaseStream::StatusOk != status)
              return status;

            status = read_dicom_len(in,seq_element.vr,(int*)&seq_element.length);
            if(BaseStream::StatusOk != status)
              return status;

            status = read_dicom_val(in,seq_element.vr,(int*)&seq_element.length,&seq_element.value);
            if(BaseStream::StatusOk != status)
              return status;

            ippFree(seq_element.value.VR_UN);
            seq_element.value.VR_UI = 0;
          } // while(1)

          status = read_dicom_tag(in,&seq_element.tag);
          if(BaseStream::StatusOk != status)
            return status;

          status = read_dicom_i32(in,(int*)&seq_element.length);
          if(BaseStream::StatusOk != status)
            return status;

          if(seq_element.tag.u == 0xe0ddfffe)
          {
            break;
          }
        } // if(item.length == 0xffffffff)
      }while(1);
    }
    break;
  }

  return BaseStream::StatusOk;
} // read_dicom_val()


UIC::BaseStream::TStatus skip_dicom_data(
  BaseStreamInput& in,
  DICOM_ELEMENT*   element)
{
  return in.Seek(element->length,UIC::BaseStreamInput::Current);
} // skip_dicom_data()


BaseStream::TStatus ReadImageRLE(
  BaseStreamInput& in,
  CIppImage&       image,
  int              rleSize)
{
  int        j, i;
  int        size;
  int        bsSize;
  int        index;
  int        width;
  int        height;
  int        step;
  int        nchanels;
  int        precision;
  Ipp8u*     ptr      = 0;
  Ipp8u*     bs       = 0;
  Ipp8u*     decData  = 0;
  Ipp8u*     pRLE     = 0;
  IppiSize   roi;
  IppStatus  st;

  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus status;

  int     curPos;
  int     numberOfSegments;
  long*   segmentOffset;
  int     rleSegSize;

  step      = image.Step();
  roi       = image.Size();
  nchanels  = image.NChannels();
  width     = image.Width();
  height    = image.Height();
  precision = image.Precision();

  ptr  = (Ipp8u*)ippMalloc(rleSize);
  if(NULL == ptr)
  {
    return UIC::BaseStream::StatusFail;
  }

  status = in.Read((Ipp8u*)ptr, rleSize, cnt);
  if(UIC::BaseStream::StatusOk != status)
    return status;

  numberOfSegments = (long)*ptr;
  segmentOffset    = (long*)ippMalloc(numberOfSegments * sizeof(long));
  if(0 == segmentOffset)
  {
    return UIC::BaseStream::StatusFail;
  }

  size     = width * height * numberOfSegments;
  decData  = (Ipp8u*)ippMalloc(size);
  if(0 == decData)
  {
    return UIC::BaseStream::StatusFail;
  }

  bsSize = width * height;
  bs = (Ipp8u*)ippMalloc(bsSize);
  if(0 == bs)
  {
    return UIC::BaseStream::StatusFail;
  }

  for(j = 0; j < numberOfSegments; j++)
  {
    segmentOffset[j] = *((long*)ptr + j + 1);
  }

  for(j = 0; j < numberOfSegments; j++)
  {
    pRLE = ptr + segmentOffset[j];
    if(numberOfSegments == 1)
    {
      rleSegSize = rleSize - 64;
    }
    else if(j < (numberOfSegments - 1))
    {
      rleSegSize = segmentOffset[j + 1] - segmentOffset[j];
    }
    else
    {
      rleSegSize = rleSize - segmentOffset[j];
    }

    curPos = 0;

    st = ippiUnpackBitsRow_TIFF_8u_C1((const Ipp8u*)pRLE, &curPos, rleSegSize, bs, bsSize);
    if(ippStsNoErr != st)
    {
      return UIC::BaseStream::StatusFail;
    }

    for(i = 0; i < bsSize; i++)
    {
      index = j + i * numberOfSegments;
      decData[index] = bs[i];
    }
  } // for (number of RLE Segment)

  if(precision  > 8)
  {
    ippsSwapBytes_16u_I((Ipp16u*)decData, width * height);
    image.CopyFrom((Ipp16s*)decData, step, roi);
  }
  else
    image.CopyFrom(decData, step, roi);

  ippFree(ptr);
  ippFree(bs);
  ippFree(decData);
  ippFree(segmentOffset);

  return BaseStream::StatusOk;
} // ReadImageRLE()


BaseStream::TStatus ReadImageRAW(
  BaseStreamInput& in,
  CIppImage&       image)
{
  int         r;
  int         size;
  int         width;
  int         height;
  int         step;
  int         nchannels;
  IM_COLOR    color;
  IM_SAMPLING sampling;
  Ipp8u*      ptr = 0;
  IppiSize    roi;
  IppStatus   st;

  UIC::BaseStream::TSize   cnt;
  UIC::BaseStream::TStatus status;

  cnt    = 0;
  status = UIC::BaseStream::StatusOk;

  sampling  = image.Sampling();
  color     = image.Color();
  step      = image.Step();
  roi       = image.Size();
  nchannels = image.NChannels();
  width     = image.Width();
  height    = image.Height();

  if(sampling ==IS_422)
  {
    size = width * height * 2;
  }
  else
  {
    size = width * height * nchannels;
  }

  ptr  = (Ipp8u*)ippMalloc(size);
  if(NULL == ptr)
  {
    status = UIC::BaseStream::StatusFail;
    goto Exit;
  }

  status = in.Read((Ipp8u*)ptr, size, cnt);
  if(UIC::BaseStream::StatusOk != status)
    return status;

  switch (color)
  {
    case IC_GRAY:
    case IC_RGB:
    case IC_UNKNOWN:
    {
      r = image.CopyFrom(ptr, step, roi);
      if(r != 0)
      {
        status = UIC::BaseStream::StatusFail;
        goto Exit;
      }
    }
    break;

    case IC_YCBCR:
    {
      if(nchannels != 3)
      {
        status = UIC::BaseStream::StatusFail;
        goto Exit;
      }

      if(sampling == IS_444)
      {
        st = ippiYCbCrToRGB_8u_C3R(ptr, 3*width, image, step, roi);
        if(ippStsNoErr != st)
        {
          status = UIC::BaseStream::StatusFail;
          goto Exit;
        }
      }
      else if(sampling == IS_422)
      {
        int      i;
        Ipp8u*   row422_c2;
        Ipp8u*   rowRGB;
        IppiSize sz;
        IppiSize sz2;

        sz.width  = width;
        sz.height = 1;

        sz2.width  = width >> 1;
        sz2.height = 1;

        for(i = 0; i < height; i++)
        {
          const int dorder[4] = { 0, 1, 3, 2 };

          row422_c2 = ptr + i*width*2;

          // convert to YUY2
          st = ippiSwapChannels_8u_C4IR(row422_c2,width,sz2,dorder);
          if(ippStsNoErr != st)
          {
            status = UIC::BaseStream::StatusFail;
            goto Exit;
          }

          rowRGB = (Ipp8u*)image + i*step;

          st = ippiYCbCr422ToRGB_JPEG_8u_C2C3R(row422_c2,width,rowRGB,step,sz);
          if(ippStsNoErr != st)
          {
            status = UIC::BaseStream::StatusFail;
            goto Exit;
          }
        }
      }
      else
      {
        status = UIC::BaseStream::StatusFail;
        goto Exit;
      }
    }
    break;

    default:
    {
      status = UIC::BaseStream::StatusFail;
      goto Exit;
    }
  }

Exit:

  if(0 != ptr)
    ippFree(ptr);

  return status;
} // ReadImageRAW()


void map2palette(
  CIppImage* paletteSrc,
  CIppImage* image,
  DICOM_LUT* lut)
{
  int    i, j;
  int    width;
  int    height;
  int    paletteIndex;
  Ipp8u* src = 0;
  Ipp8u* dst = 0;

  src = *paletteSrc;
  dst = *image;

  width  = paletteSrc->Width();
  height = paletteSrc->Height();

  for(i = 0; i < height; i++)
  {
    for(j = 0; j < width; j ++)
    {
      paletteIndex = src[j];

      dst[3*j]     = (Ipp8u)(lut->RedLUT  [paletteIndex] >> 8);
      dst[3*j + 1] = (Ipp8u)(lut->GreenLUT[paletteIndex] >> 8);
      dst[3*j + 2] = (Ipp8u)(lut->BlueLUT [paletteIndex] >> 8);
    }
    src += paletteSrc->Step();
    dst += image->Step();
  }

  return;
} // map2palette()


IM_ERROR ReadImageDICOM(
  BaseStreamInput& in,
  PARAMS_DICOM&    param,
  CIppImage&       image)
{
  int           end;
  int           nchannels;
  int           precision;
  int           signature;
  unsigned int  readSize = 0;
  char          transfer_syntax[128];

  UIC::BaseStream::TSize   cnt;

  DICOM_ORDER       planar;
  DICOM_PHOTOMETRIC photometric;
  DICOM_ELEMENT     element;
  DICOM_TSYNTAX     syntax;
  IppiSize      roi;
  IM_ERROR      im_err = IE_OK;

  nchannels   = 1;
  precision   = 0;
  planar      = (DICOM_ORDER)0;
  photometric = (DICOM_PHOTOMETRIC)0;
  syntax      = DTS_UNSUPPORTED;

  roi.width  = 0;
  roi.height = 0;

  element.length      = 0;
  element.tag.u       = 0;
  element.value.VR_UN = 0;
  element.vr          = VR_OX;

  param.nFrames   = 1;
  param.frameSeek = 0;

  if(UIC::BaseStream::StatusOk != in.Seek(DICOM_PREAMBLE,UIC::BaseStreamInput::Beginning))
    return IE_FSEEK;

  if(UIC::BaseStream::StatusOk != in.Read(&signature,sizeof(Ipp32s), cnt))
    return IE_FREAD;

#ifdef __LOG__
  printf("FILE: %s\n",lpszPathName);
  printf("DICOM signature: 0x%04X\n",signature);
#endif

  end = 0;

  do
  {
    if(UIC::BaseStream::StatusFail == read_dicom_tag(in, &element.tag))
    {
      im_err = IE_OK;
      goto Exit;
    }

    if(IE_OK != im_err)
      goto Exit;

    if(UIC::BaseStream::StatusOk != read_dicom_vr(in,&element.vr))
    {
      im_err = IE_FREAD;
      break;
    }

    if(UIC::BaseStream::StatusOk != read_dicom_len(in,element.vr,(int*)&element.length))
    {
      im_err = IE_FREAD;
      break;
    }

    if(element.length == 0)
      continue;

    switch(element.tag.u)
    {
      case 0xfffcfffc:
      {
        if(UIC::BaseStream::StatusOk != in.Seek(element.length,UIC::BaseStreamInput::Current))
        {
          im_err = IE_FSEEK;
          break;
        }
      }
      break;

      case 0x00107FE0:
      {
        if(param.nFrames == 1)
        {
          if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
          {
            im_err = IE_FREAD;
            break;
          }

          readSize = element.length;
        }
        else
        {
          if(precision <= 8)
            readSize = roi.width*roi.height*nchannels*sizeof(Ipp8u);
          else
            readSize = roi.width*roi.height*nchannels*sizeof(Ipp16u);

          if(0xFFFFFFFF == element.length) // enable BASIC OFFSET TABLE ???
          {
            if(UIC::BaseStream::StatusOk != in.Seek(2*sizeof(Ipp32s), UIC::BaseStreamInput::Current))
            {
              im_err = IE_FSEEK;
              break;
            }

            param.frameSeek = (Ipp32s*)ippMalloc(param.nFrames*sizeof(Ipp32s));
            if(0 == param.frameSeek)
            {
              im_err =  IE_ALLOC;
              break;
            }

            if(UIC::BaseStream::StatusOk != in.Read(param.frameSeek, param.nFrames*sizeof(Ipp32s),cnt))
            {
              im_err = IE_FREAD;
              break;
            }

            in.Position((UIC::BaseStream::TPosition&)param.firstFramePos);

            if(UIC::BaseStream::StatusOk != in.Seek(2*sizeof(Ipp32s), UIC::BaseStreamInput::Current))
            {
              im_err = IE_FSEEK;
              break;
            }

            readSize = param.frameSeek[1] - 2*sizeof(Ipp32s);
          }
          else
           in.Position((UIC::BaseStream::TPosition&)param.firstFramePos);

          if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&readSize,&element.value))
          {
            im_err = IE_FREAD;
            break;
          }
        }
        image.NChannels(nchannels);

        switch(photometric)
        {
        case DPM_MONOCHROME1:  image.Color(IC_GRAY);    image.Sampling(IS_444);  break;
        case DPM_MONOCHROME2:  image.Color(IC_GRAY);    image.Sampling(IS_444);  break;
        case DPM_RGB:          image.Color(IC_RGB);     image.Sampling(IS_444);  break;
        case DPM_YBR_FULL:     image.Color(IC_YCBCR);   image.Sampling(IS_444);  break;
        case DPM_YBR_FULL_422: image.Color(IC_YCBCR);   image.Sampling(IS_422);  break;
        case DPM_YBR_PART_422: image.Color(IC_YCBCR);   image.Sampling(IS_422);  break;
        case DPM_PALETTE_COLOR:
          {
            if(DTS_RLE != syntax && precision > 8)
              return IE_UNSUP;
          }
          break;

        default:
          image.Color(IC_UNKNOWN);
          image.Sampling(IS_444);
          break;
        }

        // got image
        switch(syntax)
        {
          case DTS_JPEG_BASELINE:
          case DTS_JPEG_EXTENDED:
          case DTS_JPEG_LOSSLESS1:
          {
            CMemBuffInput mem;

            param.fmt = IT_JPEG;

            if(UIC::BaseStream::StatusOk != mem.Open((Ipp8u*)element.value.VR_UI,readSize))
            {
              im_err = IE_FOPEN;
              break;
            }

            im_err = ReadImageJPEG(mem,param.param_jpeg,image);
            if(IE_OK != im_err)
              return im_err;

          }
          break;

          case DTS_RAW_LE:
          {
            int r;
            int step;
            CMemBuffInput mem;

            if(planar == DPC_PLANE)
              return IE_UNSUP;

            if(0 != image.Alloc(roi,nchannels,precision))
              return IE_ALLOC;

            step = image.Step();

            if(precision <= 8)
            {
              if(UIC::BaseStream::StatusOk != mem.Open((Ipp8u*)element.value.VR_UI,readSize))
                return IE_FOPEN;

              if(UIC::BaseStream::StatusOk != ReadImageRAW(mem, image))
                return IE_FREAD;
            }
            else
            {
              Ipp16s* ptr = (Ipp16s*)element.value.VR_UI;
              r = image.CopyFrom(ptr,step,roi);
            }
            goto Exit;
          }
          break;

          case DTS_RLE:
          {
            CMemBuffInput mem;

            if(0 != image.Alloc(roi,nchannels,precision))
              return IE_ALLOC;

            if(UIC::BaseStream::StatusOk != mem.Open((Ipp8u*)element.value.VR_UI,readSize))
              return IE_FOPEN;

            if(DPM_PALETTE_COLOR != photometric)
            {
              if(UIC::BaseStream::StatusOk != ReadImageRLE(mem, image, readSize))
                return IE_FREAD;
            }
            else
            {
              CIppImage paletteImage;
              if(0 != paletteImage.Alloc(roi,1,precision))
                return IE_ALLOC;

              if(UIC::BaseStream::StatusOk != ReadImageRLE(mem, paletteImage, readSize))
                return IE_FREAD;

              map2palette(&paletteImage, &image, &param.lut);

              paletteImage.Free();
            }
          }
          break;

          case DTS_JPEG2K_LOSSLESS:
          case DTS_JPEG2K:
          {
            CMemBuffInput mem;

            param.fmt = IT_JPEG2000;

            if(UIC::BaseStream::StatusOk != mem.Open((Ipp8u*)element.value.VR_UI,readSize))
              return IE_FOPEN;

            im_err = ReadImageJPEG2000(mem, param.param_jpeg2k,image);
            if(IE_OK != im_err)
              return im_err;
          }
            break;

        default:
          return IE_UNSUP;
        }
      }
      break;

      case 0x00080028:
      {
        // multi-frame images -> number of frames
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        param.nFrames = atoi((char*)element.value.VR_OB);
      }
      break;

      case 0x00060028:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        planar = (DICOM_ORDER)*element.value.VR_US;
      }
      break;

      case 0x00040028:
      {
        char buf[32];

        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        char* str = (char*)element.value.VR_CS;
        memcpy(buf,str,element.length);
        buf[element.length] = 0;

        if(strstr(buf,"MONOCHROME1"))
        {
          photometric = DPM_MONOCHROME1;
        }
        else if(strstr(buf,"MONOCHROME2"))
        {
          photometric = DPM_MONOCHROME2;
        }
        else if(strstr(buf,"PALETTE COLOR"))
        {
          photometric = DPM_PALETTE_COLOR;
          nchannels   = 3;
        }
        else if(strstr(buf,"RGB"))
        {
          photometric = DPM_RGB;
        }
        else if(strstr(buf,"YBR_PART_422"))
        {
          photometric = DPM_YBR_PART_422;
        }
        else if(strstr(buf,"YBR_FULL_422"))
        {
          photometric = DPM_YBR_FULL_422;
        }
        else if(strstr(buf,"YBR_FULL"))
        {
          photometric = DPM_YBR_FULL;
        }
        else if(strstr(buf,"YBR_ICT"))
        {
          photometric = DPM_ICT;
        }
        else if(strstr(buf,"YBR_RCT"))
        {
          photometric = DPM_RCT;
        }
        else
        {
          photometric = DPM_UNKNOWN;
        }

        param.photometric = photometric;
      }
      break;

      case 0x00020028:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        nchannels = *element.value.VR_US;
      }
      break;

      case 0x01000028:
      {
        // bits allocated
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }
      }
      break;

      case 0x00100028:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        roi.height = *element.value.VR_US;
      }
      break;

      case 0x00110028:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        roi.width = *element.value.VR_US;
      }
      break;

      case 0x00100002:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        ippsCopy_8u((Ipp8u*)element.value.VR_UI,(Ipp8u*)transfer_syntax,element.length);
        transfer_syntax[element.length] = 0;

        if(!strcmp(transfer_syntax,"1.2.840.10008.1.2.5"))
        {
          // RLE compression
          syntax = DTS_RLE;
        }
        else if(!strcmp(transfer_syntax,"1.2.840.10008.1.2"))
        {
          // RAW LE
          syntax = DTS_RAW_LE;
        }
        else if(!strcmp(transfer_syntax,"1.2.840.10008.1.2.1"))
        {
          // RAW LE
          syntax = DTS_RAW_LE;
        }
        else if(!strcmp(transfer_syntax,"1.2.840.10008.1.2.2"))
        {
          // RAW BE
          syntax = DTS_RAW_BE;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.50"))
        {
          // JPEG 8-bit Lossy
          syntax = DTS_JPEG_BASELINE;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.51"))
        {
          // JPEG 12-bit Lossy
          syntax = DTS_JPEG_EXTENDED;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.57"))
        {
          // JPEG Lossless
          syntax = DTS_JPEG_LOSSLESS;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.70"))
        {
          // JPEG Lossless 1-st predictor
          syntax = DTS_JPEG_LOSSLESS1;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.80"))
        {
          // JPEG-LS Lossless
          syntax = DTS_JPEGLS_LOSSLESS;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.81"))
        {
          // JPEG-LS Near Lossless
          syntax = DTS_JPEGLS_NEARLOSS;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.90"))
        {
          // JPEG2000 Lossless
          syntax = DTS_JPEG2K_LOSSLESS;
        }
        else if(!strcmp(transfer_syntax, "1.2.840.10008.1.2.4.91"))
        {
          // JPEG2000
          syntax = DTS_JPEG2K;
        }

        param.syntax = syntax;

        if(DTS_UNSUPPORTED == syntax)
        {
          im_err = IE_UNSUP;
          goto Exit;
        }
      }
      break;

      case 0x01010028:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        // bits stored
        precision = *element.value.VR_US;
      }
      break;

      case 0x11010028: // RED LUT Descriptor
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        memcpy(param.lut.redDescriptor,element.value.VR_US, 3*sizeof(unsigned short));
      }
      break;

      case 0x11020028: // GREEN LUT Descriptor
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

         memcpy(param.lut.greenDescriptor,element.value.VR_US, 3*sizeof(unsigned short));
      }
      break;

      case 0x11030028: // BLUE LUT Descriptor
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

         memcpy(param.lut.blueDescriptor, element.value.VR_US, 3*sizeof(unsigned short));
      }
      break;

      case 0x12010028: // RED LUT data
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        param.lut.RedLUT = (unsigned short*)ippMalloc(param.lut.redDescriptor[0]*sizeof(unsigned short));
        if(0 == param.lut.RedLUT)
        {
          im_err = IE_ALLOC;
          break;
        }

        memcpy(param.lut.RedLUT ,element.value.VR_OW, param.lut.redDescriptor[0]*sizeof(unsigned short));
      }
      break;

      case 0x12020028: // GREEN LUT data
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        param.lut.GreenLUT = (unsigned short*)ippMalloc(param.lut.greenDescriptor[0]*sizeof(unsigned short));
        if(0 == param.lut.GreenLUT)
        {
          im_err = IE_ALLOC;
          break;
        }

        memcpy(param.lut.GreenLUT ,element.value.VR_OW, param.lut.greenDescriptor[0]*sizeof(unsigned short));
      }
      break;

      case 0x12030028: // BLUE LUT data
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        param.lut.BlueLUT = (unsigned short*)ippMalloc(param.lut.blueDescriptor[0]*sizeof(unsigned short));
        if(0 == param.lut.BlueLUT)
        {
          im_err = IE_ALLOC;
          break;
        }

        memcpy(param.lut.BlueLUT ,element.value.VR_OW, param.lut.blueDescriptor[0]*sizeof(unsigned short));
      }
      break;

      case 0x01030028: // Pixel Representation: 0 - unsigned data; 1 - signed data
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }

        param.isSigned = *element.value.VR_US;

        (param.isSigned == 0) ? image.Format(IF_UNSIGNED) : image.Format(IF_SIGNED);
      }
      break;

      case 0x00000002:
      case 0x00010002:
      case 0x00020002:
      case 0x00030002:
      case 0x00120002:
      case 0x00130002:
      case 0x00160002:
      case 0x00120028:
      case 0x01020028:
      case 0x01060028:
      case 0x01070028:
      case 0x01200028:
      default:
      {
        if(UIC::BaseStream::StatusOk != read_dicom_val(in,element.vr,(int*)&element.length,&element.value))
        {
          im_err = IE_FREAD;
          break;
        }
      }
      break;
    }

    ippFree(element.value.VR_UI);
    element.value.VR_UI = 0;

  } while(!end);

Exit:

if(0 != element.value.VR_UI)
  {
    ippFree(element.value.VR_UI);
  }

  return im_err;
} // ReadImageDICOM()


IM_ERROR GetFrameDICOM(
  BaseStreamInput& in,
  PARAMS_DICOM&    param,
  int              numFrame,
  CIppImage&       image)
{
  int        width;
  int        height;
  int        dstOrder[3] = { 2, 1, 0 };
  int        precision;
  int        frameSize;
  Ipp64u     frameStep;
  IM_ERROR   im_err = IE_OK;

  UIC::BaseStream::TSize   cnt;

  width     = image.Width();
  height    = image.Height();
  precision = image.Precision();

  switch(param.syntax)
  {
  case DTS_RAW_LE:
    {
      if(precision <= 8)
      {
        frameSize = width * height * image.NChannels();
        frameStep = param.firstFramePos + numFrame * frameSize;

        if(UIC::BaseStream::StatusOk != in.Seek(frameStep,UIC::BaseStreamInput::Beginning))
          return IE_FSEEK;

        if(UIC::BaseStream::StatusOk != ReadImageRAW(in, image))
          return IE_FREAD;
      }
      else
      {
        frameSize = width * height * sizeof(Ipp16s) * image.NChannels();
        frameStep = param.firstFramePos + numFrame * frameSize;

        if(UIC::BaseStream::StatusOk != in.Seek(frameStep,UIC::BaseStreamInput::Beginning))
          return IE_FSEEK;

        Ipp16s* ptr = image;
        if(UIC::BaseStream::StatusOk != in.Read(ptr, frameSize, cnt))
          return IE_FREAD;
      }
    }
    break;

  case DTS_JPEG_BASELINE:
  case DTS_JPEG_EXTENDED:
  case DTS_JPEG_LOSSLESS1:
  case DTS_JPEG_LOSSLESS:
    {
      Ipp32s readSize = 0;

      param.fmt = IT_JPEG;

      frameStep = param.firstFramePos + param.frameSeek[numFrame];

      if(UIC::BaseStream::StatusOk != in.Seek(frameStep,UIC::BaseStreamInput::Beginning))
        return IE_FSEEK;

      if(UIC::BaseStream::StatusOk != in.Seek(sizeof(Ipp32s),UIC::BaseStreamInput::Current))
        return IE_FSEEK;

      if(UIC::BaseStream::StatusOk != in.Read(&readSize, sizeof(Ipp32s), cnt))
        return IE_FREAD;

      im_err = ReadImageJPEG(in,param.param_jpeg,image);
      if(IE_OK != im_err)
        return im_err;
    }
    break;

  case DTS_RLE:
    {
      int readSize = 0;

      frameStep = param.firstFramePos + param.frameSeek[numFrame];

      if(UIC::BaseStream::StatusOk != in.Seek(frameStep,UIC::BaseStreamInput::Beginning))
        return IE_FSEEK;

      if(UIC::BaseStream::StatusOk != in.Seek(sizeof(Ipp32s),UIC::BaseStreamInput::Current))
        return IE_FSEEK;

      if(UIC::BaseStream::StatusOk != in.Read(&readSize, sizeof(Ipp32s), cnt))
        return IE_FREAD;

      if(DPM_PALETTE_COLOR != param.photometric)
      {
        if(UIC::BaseStream::StatusOk != ReadImageRLE(in, image, readSize))
          return IE_FREAD;
      }
      else
      {
        CIppImage paletteImage;
        if(0 != paletteImage.Alloc(image.Size(),1,precision))
          return IE_ALLOC;

        if(UIC::BaseStream::StatusOk != ReadImageRLE(in, paletteImage, readSize))
          return IE_FREAD;

        map2palette(&paletteImage, &image, &param.lut);

        paletteImage.Free();
      }

      if(image.NChannels() == 3)
        image.SwapChannels(dstOrder);
    }
    break;

  default:
    return IE_UNSUP;
  }

  return im_err;
} // GetFrameDICOM()

