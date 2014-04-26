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
#include <stdio.h>
#include <stdlib.h>
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __IPPI_H__
#include "ippi.h"
#endif
#ifndef __IPPCC_H__
#include "ippcc.h"
#endif
#ifndef __IPPCV_H__
#include "ippcv.h"
#endif
#ifndef __EDGEHISTOGRAMFEA_H__
#include "edgehistogramfea.h"
#endif


namespace ISF
{

/////////////////////////////////////////////////////////////////////
// EdgeHistogramFeatureImpl

class EdgeHistogramFeatureImpl
{
public:
  EdgeHistogramFeatureImpl(void) {}
  ~EdgeHistogramFeatureImpl(void) { Clear(); }

  bool IsEmpty(void) { return false; }
  Status Clear(void) { return Ok; }
  Status Extract(const CIppImage& src);

  int Set(float ehd[80])       { ippsCopy_8u((Ipp8u*)ehd,(Ipp8u*)m_ehd,sizeof(m_ehd)); return 0; }
  int Get(float ehd[80]) const { ippsCopy_8u((Ipp8u*)m_ehd,(Ipp8u*)ehd,sizeof(m_ehd)); return 0; }

private:
  float m_ehd[80];
};


Status EdgeHistogramFeatureImpl::Extract(const CIppImage& image)
{
  Ipp8u* pBuf = 0;
  Ipp8u* pSrc = 0;
  IppiSize roi;
  int    step;
  int    nchannels;
  Ipp8u* pRgb[4] = { 0, 0, 0, 0 };
  int    rgbStep;
  IppStatus status;

  CIppImage& src = const_cast<CIppImage&>(image);

  pSrc      = src;
  nchannels = src.NChannels();
  step      = src.Step();
  roi       = src.Size();

  ///////////////////////////////////////////////////////////////
  // prepare planar image in memory

  if(nchannels >= 3)
  {
    pBuf = ippiMalloc_8u_C1(roi.width,roi.height * nchannels,&rgbStep);
    if(0 == pBuf)
      return AllocFailed;

    pRgb[0] = &pBuf[0 * roi.height * rgbStep];
    pRgb[1] = &pBuf[1 * roi.height * rgbStep];
    pRgb[2] = &pBuf[2 * roi.height * rgbStep];

    if(nchannels == 4)
    {
      pRgb[3] = &pBuf[3 * roi.height * rgbStep];

      status = ippiCopy_8u_C4P4R(pSrc,step,pRgb,rgbStep,roi);
      if(ippStsNoErr != status)
        return Fail;
    }
    else
    {
      status = ippiCopy_8u_C3P3R(pSrc,step,pRgb,rgbStep,roi);
      if(ippStsNoErr != status)
        return Fail;
    }
  }
  else
  {
    rgbStep = src.Step();
    pRgb[0] = pRgb[1] = pRgb[2] = /*pRgb[3] = */(Ipp8u*)src;
    nchannels = 3;
  }

  ipp_edge_histogram_descriptor((const Ipp8u**)pRgb,rgbStep,roi,nchannels,m_ehd);

  if(pBuf != 0)
    ippiFree(pBuf);

  return Ok;
} // EdgeHistogramFeatureImpl::Extract()


/////////////////////////////////////////////////////////////////////
// EdgeHistogramFeature

EdgeHistogramFeature::EdgeHistogramFeature(void)
{
  m_pEHImpl = 0;
  return;
} // ctor


EdgeHistogramFeature::~EdgeHistogramFeature(void)
{
  if(0 != m_pEHImpl)
    delete m_pEHImpl;

  return;
} // dtor


bool EdgeHistogramFeature::IsEmpty(void) const
{
  return m_pEHImpl->IsEmpty();
} // EdgeHistogramFeature::IsEmpty()


Status EdgeHistogramFeature::Init(void)
{
  if(0 == m_pEHImpl)
  {
    m_pEHImpl = new EdgeHistogramFeatureImpl;
    if(0 == m_pEHImpl)
      return AllocFailed;
  }

  return Ok;
} // EdgeHistogramFeature::Init()


Status EdgeHistogramFeature::Clear(void)
{
  return m_pEHImpl->Clear();
} // EdgeHistogramFeature::Clear()


Status EdgeHistogramFeature::Extract(const CIppImage& src)
{
  return m_pEHImpl->Extract(src);
} // EdgeHistogramFeature::Extract()


int EdgeHistogramFeature::Set(float ehd[80])
{
  m_pEHImpl->Set(ehd);
  return 0;
} // EdgeHistogramFeature::Set()


int EdgeHistogramFeature::Get(float ehd[80]) const
{
  return m_pEHImpl->Get(ehd);
} // EdgeHistogramFeature::Get()


Status EdgeHistogramFeature::Compare(const ImageFeatureBase& another, double& distance) const
{
  int i;
  float ehd1[80];
  float ehd2[80];
  double sum = 0;

  if( Id() != another.Id() )
    return BadParams;

  Get(ehd1);
  ((EdgeHistogramFeature&)another).Get(ehd2);

  for( i = 0; i < 80; i++ )
    sum += fabs(ehd1[i] - ehd2[i]);

  distance = sum;

  return Ok;
} // Compare()



#define Te_Define             11
#define Desired_Num_of_Blocks 1100

#define NoEdge                    0
#define vertical_edge             1
#define horizontal_edge           2
#define non_directional_edge      3
#define diagonal_45_degree_edge   4
#define diagonal_135_degree_edge  5


typedef struct Edge_Histogram_Descriptor
{
  Ipp32f Local_Edge[80];
} EHD;


//----------------------------------------------------------------------------
static
int GetEdgeFeature(
  Ipp8u*  pImage_Y,
  int     image_width,
  int     block_size,
  int     Te_Value)
{
  int i;
  int j;
  int e_index;
  Ipp32f d1;
  Ipp32f d2;
  Ipp32f d3;
  Ipp32f d4;
  Ipp32f dc_th = Te_Value;
  Ipp32f e_h;
  Ipp32f e_v;
  Ipp32f e_45;
  Ipp32f e_135;
  Ipp32f e_m;
  Ipp32f e_max;

  d1 = 0.0;
  d2 = 0.0;
  d3 = 0.0;
  d4 = 0.0;

  for(j = 0; j < block_size; j++)
  {
    for(i = 0; i < block_size; i++)
    {
      if(j < block_size / 2)
      {
        if(i < block_size / 2)
          d1 += (pImage_Y[i + image_width * j]);
        else
          d2 += (pImage_Y[i + image_width * j]);
      }
      else
      {
        if(i < block_size / 2)
          d3 += (pImage_Y[i + image_width * j]);
        else
          d4 += (pImage_Y[i + image_width * j]);
      }
    }
  }

  d1 = d1 / (block_size * block_size / 4.0);
  d2 = d2 / (block_size * block_size / 4.0);
  d3 = d3 / (block_size * block_size / 4.0);
  d4 = d4 / (block_size * block_size / 4.0);

  e_h   = fabs(d1 + d2 - (d3 + d4));
  e_v   = fabs(d1 + d3 - (d2 + d4));
  e_45  = sqrt(2.0) * fabs(d1 - d4);
  e_135 = sqrt(2.0) * fabs(d2 - d3);

  e_m = 2 * fabs(d1 - d2 - d3 + d4);

  e_max   = e_v;
  e_index = vertical_edge;

  if(e_h > e_max)
  {
    e_max   = e_h;
    e_index = horizontal_edge;
  }

  if(e_45 > e_max)
  {
    e_max   = e_45;
    e_index = diagonal_45_degree_edge;
  }

  if(e_135 > e_max)
  {
    e_max   = e_135;
    e_index = diagonal_135_degree_edge;
  }

  if(e_m > e_max)
  {
    e_max   = e_m;
    e_index = non_directional_edge;
  }

  if(e_max < dc_th)
    e_index = NoEdge;

  return e_index;
} // GetEdgeFeature()


//----------------------------------------------------------------------------
static
unsigned long GetBlockSize(
  unsigned long image_width,
  unsigned long image_height,
  unsigned long desired_num_of_blocks)
{
  unsigned long block_size;
  Ipp32f temp_size;

  temp_size  = (Ipp32f) sqrt((Ipp32f)(image_width * image_height / desired_num_of_blocks));
  block_size = (unsigned long)(temp_size / 2) * 2;

  return block_size;
} // GetBlockSize()


//----------------------------------------------------------------------------
static
void EdgeHistogramGeneration(
  Ipp8u*         pImage_Y,
  unsigned long  image_width,
  unsigned long  image_height,
  unsigned long  block_size,
  EHD*           pLocal_Edge,
  int            Te_Value)
{
  unsigned int i, j;
  int Offset;
  int sub_local_index;
  int EdgeTypeOfBlock;
  int Count_Local[16];
  long LongTyp_Local_Edge[80];

  // Clear
  memset(Count_Local, 0, 16*sizeof(int));
  memset(LongTyp_Local_Edge, 0, 80*sizeof(long));

  for(j = 0; j <= image_height - block_size; j += block_size)
  {
    for(i = 0; i <= image_width - block_size; i += block_size)
    {
      sub_local_index = (int)(i * 4 / image_width) + (int)(j * 4 / image_height) * 4;
      Count_Local[sub_local_index]++;

      Offset = image_width * j + i;

      EdgeTypeOfBlock = GetEdgeFeature(pImage_Y + Offset, image_width, block_size, Te_Value);

      switch(EdgeTypeOfBlock)
      {
      case NoEdge:
        break;

      case vertical_edge:
        LongTyp_Local_Edge[sub_local_index*5]++;
        break;

      case horizontal_edge:
        LongTyp_Local_Edge[sub_local_index*5+1]++;
        break;

      case diagonal_45_degree_edge:
        LongTyp_Local_Edge[sub_local_index*5+2]++;
        break;

      case diagonal_135_degree_edge:
        LongTyp_Local_Edge[sub_local_index*5+3]++;
        break;

      case non_directional_edge:
        LongTyp_Local_Edge[sub_local_index*5+4]++;
        break;
      } // switch(EdgeTypeOfBlock)
    } // for( i )
  }

  for(i = 0; i < 80; i++)
  {
    // Range 0.0 ~ 1.0
    sub_local_index = (int)(i / 5);
    pLocal_Edge->Local_Edge[i] = (Ipp32f)LongTyp_Local_Edge[i] / Count_Local[sub_local_index];
  }

  return;
} // EdgeHistogramGeneration()


int ipp_edge_histogram_descriptor(
  const Ipp8u*    pSrc[4],
        int       step,
        IppiSize  roi,
        int       nchannels,
        Ipp32f    ehd[80])
{
  unsigned long desired_num_of_blocks;
  unsigned long block_size;
  int   Te_Value;
  EHD   pLocal_Edge;

  Te_Value              = Te_Define;
  desired_num_of_blocks = Desired_Num_of_Blocks;

  int i, j;
  int xsize;
  int ysize;
  int min_size;
  int re_xsize;
  int re_ysize;
  int max_x = 0;
  int max_y = 0;
  int min_x = roi.width-1;
  int min_y = roi.height-1;
  Ipp8u* pGrayImage;
  Ipp8u* pResampleImage = NULL;
  Ipp32f scale;
  Ipp32f EWweight;
  Ipp32f NSweight;
  Ipp32f EWtop;
  Ipp32f EWbottom;
  Ipp8u NW, NE, SW, SE;

  const Ipp32f coefs[3] = { 1/3.f, 1/3.f, 1/3.f };
  Ipp8u* pRow;
  IppiSize sz = { roi.width, 1 };

  IppStatus status;

  if(nchannels == 4)
  {
    for(j = 0; j < roi.height; j++)
    {
      for(i = 0; i < roi.width; i++)
      {
        if(pSrc[3][j * step + i])
        {
          if(max_x < i) max_x = i;
          if(max_y < j) max_y = j;
          if(min_x > i) min_x = i;
          if(min_y > j) min_y = j;
        }
      }
    }
    xsize = max_x - min_x + 1;
    ysize = max_y - min_y + 1;
  }
  else
  {
    xsize = roi.width;
    ysize = roi.height;
    min_x = min_y = 0;
  }

  pGrayImage = (Ipp8u*)ippMalloc(xsize * ysize);
  if(0 == pGrayImage)
    return -1;

  pRow = (Ipp8u*)ippMalloc(xsize*3);
  if(0 == pRow)
    return -1;

  if(nchannels == 4)
  {
    for(j = 0; j < ysize; j++)
    {
      for(i = 0; i < xsize; i++)
      {
        if(pSrc[3][(j + min_y) * step + (i + min_x)])
        {
          pGrayImage[j * xsize + i] = (Ipp8u)
            ((Ipp32f)(pSrc[0][(j + min_y) * step + (i + min_x)] +
                      pSrc[1][(j + min_y) * step + (i + min_x)] +
                      pSrc[2][(j + min_y) * step + (i + min_x)]) / 3.0f);
        }
        else
        {
          pGrayImage[j * xsize + i] = 0;
        }
      }
    }
  }
  else
  {
    for(j = 0; j < ysize; j++)
    {
      status = ippiCopy_8u_P3C3R(pSrc,step,pRow,xsize,sz);
      if(ippStsNoErr != status)
        return -1;

      pSrc[0] += step;
      pSrc[1] += step;
      pSrc[2] += step;

      status = ippiColorToGray_8u_C3C1R(pRow,xsize*3,pGrayImage + j*xsize,xsize,sz,coefs);
      if(ippStsNoErr != status)
        return -1;
    }
  }

  min_size = (xsize > ysize) ? ysize : xsize;

  if(min_size < 70)
  {
    // upsampling
    scale = 70.0f / min_size;

    re_xsize = (int)(xsize * scale + 0.5);
    re_ysize = (int)(ysize * scale + 0.5);

    pResampleImage = (Ipp8u*)ippMalloc(re_xsize * re_ysize);
    if(0 == pResampleImage)
      return -1;

    for(j = 0; j < re_ysize; j++)
    {
      for(i = 0; i < re_xsize; i++)
      {
        EWweight = i/scale - floor(i/scale);
        NSweight = j/scale - floor(j/scale);

        NW = pGrayImage[(int)floor(i/scale) + (int)floor(j/scale) * xsize];
        NE = pGrayImage[(int)floor(i/scale) + (int)floor(j/scale) * xsize + 1];
        SW = pGrayImage[(int)floor(i/scale) + (int)(floor(j/scale) + 1) * xsize];
        SE = pGrayImage[(int)floor(i/scale) + (int)(floor(j/scale) + 1) * xsize + 1];

        EWtop    = NW + EWweight * (NE - NW);
        EWbottom = SW + EWweight * (SE - SW);

        pResampleImage[i + j*re_xsize] = (Ipp8u)(EWtop + NSweight * (EWbottom - EWtop) + 0.5);
      }
    }

    block_size = GetBlockSize(re_xsize, re_ysize, desired_num_of_blocks);

    if(block_size < 2)
      block_size = 2;

    EdgeHistogramGeneration(pResampleImage, re_xsize, re_ysize, block_size, &pLocal_Edge, Te_Value);

    ippFree(pResampleImage);
  }
  else
  {
    block_size = GetBlockSize(xsize, ysize, desired_num_of_blocks);

    if(block_size < 2)
      block_size = 2;

    EdgeHistogramGeneration(pGrayImage, xsize, ysize, block_size, &pLocal_Edge, Te_Value);
  }

  for(i = 0; i < 80; i++)
    ehd[i] = pLocal_Edge.Local_Edge[i];

  ippFree(pRow);
  ippFree(pGrayImage);

  return 0;
} // ipp_edge_histogram_descriptor()

} // namespace ISF

