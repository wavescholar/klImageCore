/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __COLORLAYOUTFEA_H__
#define __COLORLAYOUTFEA_H__

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IMAGEFEA_H__
#include "imagefea.h"
#endif

namespace ISF
{

class ColorLayoutFeatureImpl;
class ColorLayoutFeature : public ImageFeatureBase
{
public:
  ColorLayoutFeature(void);
  virtual ~ColorLayoutFeature(void);

  Status Init(void);
  Status Clear(void);

  bool IsEmpty(void) const;
  const FeatureId Id(void) const { return ImageColorLayout; }

  Status Extract(const CIppImage& src);
  Status Compare(const ImageFeatureBase& another, double& distance) const;

  Status SetParams(int ny = 6, int nc = 3);
  Status GetParams(int& ny, int& nc) const;

  Status Set(int cld[3][64]);
  Status Get(int cld[3][64]) const;

private:
  ColorLayoutFeatureImpl* m_pCLImpl;
};


class ColorLayoutDistance : public FeatureDistanceBase
{
public:
  ColorLayoutDistance(void) {}
  virtual ~ColorLayoutDistance(void) {}

  Status SetParams(Ipp16s weight[3][64]) { ippsCopy_8u((const Ipp8u*)weight,(Ipp8u*)m_weight,sizeof(weight)); return Ok; }
  Status Distance(const ImageFeatureBase& src1,const ImageFeatureBase& src2,double& distance);

private:
  Ipp16s m_weight[3][64];
};


Status Distance(const ColorLayoutFeature& f1, const ColorLayoutFeature& f2, double distance);

} // namespace ISF

#endif
