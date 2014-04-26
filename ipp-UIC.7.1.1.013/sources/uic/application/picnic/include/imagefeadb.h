/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __IMAGEFEADB_H__
#define __IMAGEFEADB_H__

#ifndef __IPPIMAGE_H__
#include "ippimage.h"
#endif
#ifndef __IMAGEFEA_H__
#include "imagefea.h"
#endif


namespace ISF
{

class ImageDescriptorImpl;
class ImageDescriptor
{
public:
  ImageDescriptor(void);
  virtual ~ImageDescriptor(void);

  Status Init(void);
  Status Clear(void);

  bool IsEmpty(void) const;
  int Size(void) const;

  Status Insert(ImageFeatureBase& feature);
  Status Remove(FeatureId id);

  const ImageFeatureBase& Feature(int id) const;
  const ImageFeatureBase& Feature(FeatureId id) const;

private:
  ImageDescriptorImpl* m_pImpl;
};


class ImageDescriptorDBImpl;
class ImageDescriptorDB
{
public:
  ImageDescriptorDB(void);
  virtual ~ImageDescriptorDB(void);

  Status Init(void);
  Status Clear(void);

  int Version(void) const { return 10; }
  int Size(void) const;
  bool IsEmpty(void) const;

  Status Insert(ImageDescriptor& descriptor);
  Status Remove(int id);

  const ImageDescriptor& Descriptor(int id) const;

  Status Index(const char* name,int& index);

  int FindSimilar(const char** names, const float* inRatings, int inCount,
                  const char** mostSimilar, float* outRatings,
                  int maxOutCount, FeatureId featureId = ImageColorLayout );

private:
  ImageDescriptorDBImpl* m_pImpl;
};

} // namespace ISF

#endif
