/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __IMAGEFEA_H__
#define __IMAGEFEA_H__

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif

namespace ISF
{

typedef enum FeatureId
{
  ImageFileName      =  0,
  ImageColorLayout   =  1,
  ImageEdgeHistogram =  2,
  ImageHomoTexture   =  3,
  UndefinedFeature   = -1

};


typedef enum Status
{
  Ok             =  0,
  Fail           = -1,
  BadParams      = -2,
  NotImplemented = -3,
  AllocFailed    = -4,

};


class ImageFeatureBase
{
public:
  ImageFeatureBase(void) {}
  virtual ~ImageFeatureBase(void) {}

  virtual bool IsEmpty(void) const = 0;
  virtual const FeatureId Id(void) const = 0;

  virtual Status Extract(const CIppImage& src) = 0;
  virtual Status Clear(void) = 0;
  virtual Status Compare(const ImageFeatureBase& another, double& distance) const = 0;
};


class FeatureDistanceBase
{
public:
  FeatureDistanceBase(void) {}
  virtual ~FeatureDistanceBase(void) {}

  virtual Status Distance(const ImageFeatureBase& src1,const ImageFeatureBase& src2, double& distance) = 0;

};


class FakeFeature : public ImageFeatureBase
{
public:
  FakeFeature(void) {}
  ~FakeFeature(void) {}

  bool IsEmpty(void) const       { return true; }
  const FeatureId Id(void) const { return UndefinedFeature; }

  Status Compare(const ImageFeatureBase& another, double& distance) const
  {
    if( Id() != another.Id())
      return BadParams;

    distance = 0;

    return Ok;
  }

private:
  Status Clear(void)               { return NotImplemented; }
  Status Extract(const CIppImage&) { return NotImplemented; }
};


class FileNameFeatureImpl;
class FileNameFeature : public ImageFeatureBase
{
public:
  FileNameFeature(void);
  virtual ~FileNameFeature(void);

  Status Init(void);
  Status Clear(void);

  bool IsEmpty(void) const;

  const FeatureId Id(void) const { return ImageFileName; }
  Status Compare(const ImageFeatureBase& another, double& distance) const;

  Status Set(const char* name);
  const char* Get(void) const;

private:
  Status Extract(const CIppImage&) { return NotImplemented; }

  FileNameFeatureImpl* m_pFileName;
};


class FileNameDistance : public FeatureDistanceBase
{
public:
  FileNameDistance(void) {}
  virtual ~FileNameDistance(void) {}

  Status Distance(const ImageFeatureBase& src1,const ImageFeatureBase& src2, double& distance);
};

} // namespace ISF

#endif
