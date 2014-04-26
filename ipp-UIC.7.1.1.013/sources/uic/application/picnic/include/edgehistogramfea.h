/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//    Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __EDGEHISTOGRAMFEA_H__
#define __EDGEHISTOGRAMFEA_H__

#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IMAGEFEA_H__
#include "imagefea.h"
#endif

namespace ISF
{

class EdgeHistogramFeatureImpl;
class EdgeHistogramFeature : public ImageFeatureBase
{
public:
  EdgeHistogramFeature(void);
  virtual ~EdgeHistogramFeature(void);

  Status Init(void);
  Status Clear(void);

  bool IsEmpty(void) const;
  const FeatureId Id(void) const { return ImageEdgeHistogram; }

  Status Extract(const CIppImage& src);
  Status Compare(const ImageFeatureBase& another, double& distance) const;

  int Set(float ehd[80]);
  int Get(float ehd[80]) const;

private:
  EdgeHistogramFeatureImpl* m_pEHImpl;
};


int ipp_edge_histogram_descriptor(
  const Ipp8u*   pSrc[4],
        int      step,
        IppiSize roi,
        int      nchannels,
        Ipp32f   ehd[80]);

} // namespace ISF

#endif
