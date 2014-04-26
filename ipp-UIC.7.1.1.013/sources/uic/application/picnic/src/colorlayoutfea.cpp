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
#ifndef __COLORLAYOUTFEA_H__
#include "colorlayoutfea.h"
#endif


namespace ISF
{

static const Ipp16s INVALID = -256;
static const Ipp8u zigzag_scan[64] =
{
  0,  1,  8, 16,  9,  2,  3, 10,
  17, 24, 32, 25, 18, 11,  4,  5,
  12, 19, 26, 33, 40, 48, 41, 34,
  27, 20, 13,  6,  7, 14, 21, 28,
  35, 42, 49, 56, 57, 50, 43, 36,
  29, 22, 15, 23, 30, 37, 44, 51,
  58, 59, 52, 45, 38, 31, 39, 46,
  53, 60, 61, 54, 47, 55, 62, 63
};

static const int weight[3][64] =
{
  {
    3, 3, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },

  {
    2, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  },

  {
    4, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  }
};


/////////////////////////////////////////////////////////////////////
// ColorLayoutFeatureImpl

class ColorLayoutFeatureImpl
{
public:
  ColorLayoutFeatureImpl(void) {}
  ~ColorLayoutFeatureImpl(void) { Clear(); }

  bool IsEmpty(void) { return false; }
  Status Init(void)
  {
    m_ny = 6;
    m_nc = 3;
    ippsZero_8u((Ipp8u*)m_cld, sizeof(m_cld));
    ippsCopy_8u((const Ipp8u*)weight,(Ipp8u*)m_weight,sizeof(m_weight));
    return Ok;
  };
  Status Clear(void) { return Ok; }
  Status Extract(const CIppImage& src);

  Status SetParams(int ny, int nc)   { m_ny = ny; m_nc = nc; return Ok; }
  Status GetParams(int& ny, int& nc) { ny = m_ny; nc = m_nc; return Ok; }
  Status Set(int cld[3][64])       { ippsCopy_8u((Ipp8u*)cld,(Ipp8u*)m_cld,sizeof(int[3][64])); return Ok; }
  Status Get(int cld[3][64]) const { ippsCopy_8u((Ipp8u*)m_cld,(Ipp8u*)cld,sizeof(int[3][64])); return Ok; }

protected:
  Status ipp_color_layout_descriptor(const Ipp8u* pSrc[4],int step, IppiSize roi, int nchannels,int cld[3][64]);

private:
  int m_ny;
  int m_nc;
  int m_cld[3][64];
  int m_weight[3][64];
};


Status ColorLayoutFeatureImpl::Extract(const CIppImage& image)
{
  Ipp8u*    pBuf = 0;
  Ipp8u*    pSrc = 0;
  int       step;
  int       nchannels;
  int       rgbStep;
  Ipp8u*    pRgb[4] = { 0, 0, 0, 0 };
  IppiSize  roi;
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
      pRgb[3] = &pBuf[3 * roi.height * rgbStep];

    status = ippiCopy_8u_C3P3R(pSrc,step,pRgb,rgbStep,roi);
    if(ippStsNoErr != status)
      return Fail;
  }
  else
  {
    rgbStep = src.Step();
    pRgb[0] = pRgb[1] = pRgb[2] = (Ipp8u*)src;
    nchannels = 3;
  }

  ipp_color_layout_descriptor((const Ipp8u**)pRgb,rgbStep,roi,nchannels,m_cld);

  if(pBuf != 0)
    ippiFree(pBuf);

  return Ok;
} // ColorLayoutFeatureImpl::Extract()


Status ColorLayoutFeatureImpl::ipp_color_layout_descriptor(
  const Ipp8u*    pSrc[4],
        int       srcStep,
        IppiSize  roi,
        int       nchannels,
        int       cld[3][64])
{
  int       n;
  int       step = 1;
  Ipp16s    mean8x8[3][64];

  for( n = 0; n < 64; n++ )
  {
    int i = n / 8;
    int j = n % 8;
    int k;

    int x0 = j   > 0 ? (int)ceil( j    * (roi.width  / 8.)) : 0;
    int x1 = j+1 < 8 ? (int)ceil((j+1) * (roi.width  / 8.)) : roi.width;

    int y0 = i   > 0 ? (int)ceil( i    * (roi.height / 8.)) : 0;
    int y1 = i+1 < 8 ? (int)ceil((i+1) * (roi.height / 8.)) : roi.height;

    Ipp32f R;
    Ipp32f G;
    Ipp32f B;
    Ipp32f Y;
    Ipp32f Cb;
    Ipp32f Cr;
    Ipp64f val[3] = { 0, 0, 0 };
    IppiSize tileSize = { x1 - x0, y1 - y0 };

    const Ipp8u* pTile[4] =
    {
      pSrc[0] + y0*srcStep + x0,
      pSrc[1] + y0*srcStep + x0,
      pSrc[2] + y0*srcStep + x0,

      nchannels == 4 ? pSrc[3] + y0*srcStep + x0 : 0
    };

    if( tileSize.width <= 0 || tileSize.height <= 0 )
    {
      mean8x8[0][n] = INVALID;
      mean8x8[1][n] = INVALID;
      mean8x8[2][n] = INVALID;
      continue;
    }

    for( k = 0; k < 3; k++ )
    {
      IppStatus status;
      if( nchannels == 3 )
        status = ippiMean_8u_C1R(pTile[k],srcStep,tileSize,&val[k]);
      else
        status = ippiMean_8u_C1MR(pTile[k],srcStep,pTile[3],srcStep,tileSize,&val[k]);
      assert( status >= 0 ); // do not check status in runtime in release mode,
                             // as we are in the parallel region
    }

    R = (Ipp32f)val[0];
    G = (Ipp32f)val[1];
    B = (Ipp32f)val[2];

    Y  =  0.257f*R + 0.504f*G + 0.098f*B + 16.f;
    Cr =  0.439f*R - 0.368f*G - 0.071f*B + 128.f;
    Cb = -0.148f*R - 0.291f*G + 0.439f*B + 128.f;

    mean8x8[0][n] = (Ipp16s)Y;
    mean8x8[1][n] = (Ipp16s)Cb;
    mean8x8[2][n] = (Ipp16s)Cr;
  }

  for(;;)
  {
    int k, have_invalids = 0;
    for( n = step > 0 ? 0 : 63; (n & ~63) != 0; n += step )
    {
      for( k = 0; k < 3; k++ )
      {
        if( mean8x8[k][n] == INVALID )
        {
          int v;
          int j = n % 8;
          int sum   = 0;
          int count = 0;

          if( j-1 >= 0 && (v = mean8x8[k][n-1]) != INVALID )
          {
            sum += v;
            count++;
          }

          if( j+1 < 8 && (v = mean8x8[k][n+1]) != INVALID )
          {
            sum += v;
            count++;
          }

          if( n-8 >= 0 && (v = mean8x8[k][n-8]) != INVALID )
          {
            sum += v;
            count++;
          }

          if( n+8 < 64 && (v = mean8x8[k][n+8]) != INVALID )
          {
            sum += v;
            count++;
          }

          if( count != 0 )
            mean8x8[k][n] = (Ipp16s)((sum + count/2)/count);
          else
            have_invalids = 1;
        }
      }
    }

    if( !have_invalids )
      break;

    step = -step;
  }

  {
  int k;
  for( k = 0; k < 3; k++ )
  {
    ippiDCT8x8Fwd_16s_C1I(mean8x8[k]);

    int dc = mean8x8[k][0] / 8;

    if( k == 0 )
    {
      // quantize luma dc
      dc = (dc >= 192 ? 112 + (dc - 192) / 4 :
        dc >= 160 ? 96 + (dc - 160) / 2 :
        dc >= 96 ? 32 + (dc - 96) :
        dc >= 64 ? 16 + (dc - 64) / 2 :
        dc / 4) >> 1;
    }
    else
    {
      // quantize chroma dc
      dc = dc >= 192 ? 63 :
        dc >= 160 ? 56 + (dc - 160) / 4 :
        dc >= 144 ? 48 + (dc - 144) / 2 :
        dc >= 112 ? 16 + (dc - 112) :
        dc >= 96 ? 8 + (dc - 96) / 2 :
        dc >= 64 ? (dc - 64) / 4 :
        0;
    }

    cld[k][0] = dc;

    for( n = 1; n < 64; n++ )
    {
      int aac;
      int ac = mean8x8[k][zigzag_scan[n]];

      if( k == 0 )
        ac /= 2;

      aac = ac  > 239 ? 239 : ac < -256 ? 256 : abs(ac);
      aac = aac > 127 ? 64 + aac / 4 : aac > 63 ? 32 + aac / 2 : aac;

      cld[k][n] = ((ac < 0 ? -aac : aac) + 132) >> 3;
    }
  }
  }

  return Ok;
} // ColorLayoutFeatureImpl::ipp_color_layout_descriptor()


/////////////////////////////////////////////////////////////////////
// ColorLayoutFeature

ColorLayoutFeature::ColorLayoutFeature(void)
{
  m_pCLImpl = 0;
  return;
} // ctor


ColorLayoutFeature::~ColorLayoutFeature(void)
{
  if(0 != m_pCLImpl)
    delete m_pCLImpl;

  return;
} // dtor


bool ColorLayoutFeature::IsEmpty(void) const
{
  return m_pCLImpl->IsEmpty();
} // ColorLayoutFeature::IsEmpty()


Status ColorLayoutFeature::Init(void)
{
  if(0 == m_pCLImpl)
  {
    m_pCLImpl = new ColorLayoutFeatureImpl;
    if(0 == m_pCLImpl)
      return AllocFailed;
  }

  return m_pCLImpl->Init();
} // ColorLayoutFeature::Init()


Status ColorLayoutFeature::Clear(void)
{
  return m_pCLImpl->Clear();
} // ColorLayoutFeature::Clear()


Status ColorLayoutFeature::Extract(const CIppImage& src)
{
  return m_pCLImpl->Extract(src);
} // ColorLayoutFeature::Extract()


Status ColorLayoutFeature::SetParams(int ny, int nc)
{
  return m_pCLImpl->SetParams(ny,nc);
} // ColorLayoutFeature::SetParams()


Status ColorLayoutFeature::GetParams(int& ny, int& nc) const
{
  return m_pCLImpl->GetParams(ny,nc);
} // ColorLayoutFeature::GetParams()


Status ColorLayoutFeature::Set(int cld[3][64])
{
  return m_pCLImpl->Set(cld);
} // ColorLayoutFeature::Set()


Status ColorLayoutFeature::Get(int cld[3][64]) const
{
  return m_pCLImpl->Get(cld);
} // ColorLayoutFeature::Get()


Status ColorLayoutFeature::Compare(const ImageFeatureBase& another, double& distance) const
{
  int i;
  int ny, ny1, ny2;
  int nc, nc1, nc2;
  int diff;
  int sum[3];
  int cld1[3][64];
  int cld2[3][64];
  const ColorLayoutFeature& clfea2 = reinterpret_cast<const ColorLayoutFeature&>(another);

  if( Id() != another.Id() )
    return BadParams;

  GetParams(ny1,nc1);
  clfea2.GetParams(ny2,nc2);

  ny = (ny1 < ny2) ? ny1 : ny2;
  nc = (nc1 < nc2) ? nc1 : nc2;

  Get(cld1);
  clfea2.Get(cld2);

  sum[0] = 0;
  for(i = 0; i < ny; i++)
  {
    diff = (cld1[0][i] - cld2[0][i]);
    sum[0] += (weight[0][i] * diff * diff);
  }

  sum[1] = 0;
  for(i = 0; i < nc; i++)
  {
    diff = (cld1[1][i] - cld2[1][i]);
    sum[1] += (weight[1][i] * diff * diff);
  }

  sum[2] = 0;
  for(i = 0; i < nc; i++)
  {
    diff = (cld1[2][i] - cld2[2][i]);
    sum[2] += (weight[2][i] * diff * diff);
  }

  distance = sqrt((double)sum[0]) + sqrt((double)sum[1]) + sqrt((double)sum[2]);

  return Ok;
} // Compare()


Status ColorLayoutDistance::Distance(const ImageFeatureBase& src1,const ImageFeatureBase& src2,double& distance)
{
  int i;
  int ny, ny1, ny2;
  int nc, nc1, nc2;
  int diff;
  int sum[3];
  int cld1[3][64];
  int cld2[3][64];

  if(ImageColorLayout != src1.Id() || ImageColorLayout != src2.Id())
    return BadParams;

  const ColorLayoutFeature& clf1 = reinterpret_cast<const ColorLayoutFeature&>(src1);
  const ColorLayoutFeature& clf2 = reinterpret_cast<const ColorLayoutFeature&>(src2);

  clf1.GetParams(ny1,nc1);
  clf2.GetParams(ny2,nc2);

  ny = (ny1 < ny2) ? ny1 : ny2;
  nc = (nc1 < nc2) ? nc1 : nc2;

  clf1.Get(cld1);
  clf2.Get(cld2);

  sum[0] = 0;
  for(i = 0; i < ny; i++)
  {
    diff = (cld1[0][i] - cld2[0][i]);
    sum[0] += (m_weight[0][i] * diff * diff);
  }

  sum[1] = 0;
  for(i = 0; i < nc; i++)
  {
    diff = (cld1[1][i] - cld2[1][i]);
    sum[1] += (m_weight[1][i] * diff * diff);
  }

  sum[2] = 0;
  for(i = 0; i < nc; i++)
  {
    diff = (cld1[2][i] - cld2[2][i]);
    sum[2] += (m_weight[2][i] * diff * diff);
  }

  distance = sqrt((double)sum[0]) + sqrt((double)sum[1]) + sqrt((double)sum[2]);

  return Ok;
} // ColorLayoutDistance::Distance()

} // namespace ISF

