/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "precomp.h"
#include <math.h>
#include <float.h>
#include <QtGui/QtGui>
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __COLORLAYOUTFEA_H__
#include "colorlayoutfea.h"
#endif
#ifndef __EDGEHISTOGRAMFEA_H__
#include "edgehistogramfea.h"
#endif
#ifndef __IMAGEFEADB_H__
#include "imagefeadb.h"
#endif


namespace ISF
{

static const int DEF_NUM_FEATURES = 6;

static FakeFeature    fakeFeature;

class FakeDescriptor
{
public:
  FakeDescriptor(void) {}
  virtual ~FakeDescriptor(void) {}

  Status Init(void)  { return Ok; }
  Status Clear(void) { return Ok; }

  bool IsEmpty(void) { return true; }
  int Size(void)     { return 0; }
private:
  Status Insert(ImageFeatureBase&) { return Ok; }
  Status Remove(FeatureId) { return Ok; }
  const ImageFeatureBase& Feature(int/* id*/) const { return fakeFeature; }
  const ImageFeatureBase& Feature(FeatureId) const { return fakeFeature; }
};

static FakeDescriptor fakeDescriptor;

/////////////////////////////////////////////////////////////////////
// ImageDescriptorImpl

class ImageDescriptorImpl
{
public:
  ImageDescriptorImpl(void)  { m_size = 0; }
  ~ImageDescriptorImpl(void) {}

  Status Init(void)
  {
    // TODO: need careful desing for extendable vector with features on fixed indexes
    m_pFeatures.resize(DEF_NUM_FEATURES);

    for(int index = 0; index < Size(); index++)
      m_pFeatures.replace(index,&fakeFeature);

    m_size = 0;

    return Ok;
  }

  Status Clear(void)
  {
    if(IsEmpty())
      return Ok;

    for(int index = 0; index < Size(); index++)
      m_pFeatures.replace(index,&fakeFeature);

    m_size = 0;

    return Ok;
  }

  bool IsEmpty(void) const  { return (m_size == 0); }
  int Size(void) const      { return m_size; }

  Status Insert(ImageFeatureBase& feature)
  {
    int index = (int)feature.Id();

    m_pFeatures[index] = &feature;

    m_size++;

    return Ok;
  }

  Status Remove(int index)
  {
    if(index < 0 && index >= Size())
      return BadParams;

    m_pFeatures[index] = &fakeFeature;

    m_size--;

    return Ok;
  }

  const ImageFeatureBase& Feature(int index)
  {
    if(index < 0 || index >= Size())
      return fakeFeature;

    if(m_pFeatures.isEmpty())
      return fakeFeature;

    const ImageFeatureBase& feature = *m_pFeatures[index];

    return feature;
  }

  const ImageFeatureBase& Feature(FeatureId id) { return Feature((int)id); }

private:
  int m_size;
  QVector<ImageFeatureBase*> m_pFeatures;
};


/////////////////////////////////////////////////////////////////////
// ImageDescriptor

ImageDescriptor::ImageDescriptor(void)
{
  m_pImpl = 0;
  return;
} // ctor


ImageDescriptor::~ImageDescriptor(void)
{
  if(0 != m_pImpl)
    delete m_pImpl;
  return;
} // dtor


Status ImageDescriptor::Init(void)
{
  m_pImpl = new ImageDescriptorImpl;
  if(0 == m_pImpl)
    return AllocFailed;

  return m_pImpl->Init();
} // ImageDescriptor::Init()


Status ImageDescriptor::Clear(void)
{
  return m_pImpl->Clear();
} // ImageDescriptor::Clear()


int ImageDescriptor::Size(void) const
{
  return m_pImpl->Size();
} // ImageDescriptor::Size()


bool ImageDescriptor::IsEmpty(void) const
{
  return m_pImpl->IsEmpty();
} // ImageDescriptor::IsEmpty()


Status ImageDescriptor::Insert(ImageFeatureBase& feature)
{
  return m_pImpl->Insert(feature);
} // ImageDescriptor::Insert()


Status ImageDescriptor::Remove(FeatureId id)
{
  return m_pImpl->Remove(id);
} // ImageDescriptor::Remove()


const ImageFeatureBase& ImageDescriptor::Feature(int index) const
{
  return m_pImpl->Feature(index);
} // ImageDescriptor::Feature()


const ImageFeatureBase& ImageDescriptor::Feature(FeatureId id) const
{
  return m_pImpl->Feature(id);
} // ImageDescriptor::Feature()



/////////////////////////////////////////////////////////////////////
// ImageDescriptorDBImpl

class ImageDescriptorDBImpl
{
public:
  ImageDescriptorDBImpl(void) {}
  ~ImageDescriptorDBImpl(void) { Clear(); }

  Status Clear(void)
  {
    if(!m_descriptors.isEmpty())
      m_descriptors.clear();

    return Ok;
  }

  int Size(void) const     { return m_descriptors.size(); }
  bool IsEmpty(void) const { return m_descriptors.isEmpty(); }

  Status Insert(ImageDescriptor& descriptor)
  {
    m_descriptors.append(&descriptor);
    return Ok;
  }

  Status Remove(int index)
  {
    m_descriptors.removeAt(index);
    return Ok;
  }

  const ImageDescriptor& Descriptor(int index) { return *m_descriptors.at(index); }

  Status Index(const char* name, int& index)
  {
    index = -1;
    for(int i = 0; i < Size(); i++)
    {
      QByteArray b1(reinterpret_cast<const FileNameFeature&>(Descriptor(i).Feature(ImageFileName)).Get());
      QByteArray b2(name);
      if(b1 == b2)
      {
        index = i;
        return Ok;
      }
    }
    return Ok;
  }

  int ComputeRatingsUsingNaiveKNN(
        const ImageFeatureBase** inFeatures, const float* inRatings, int inCount, int* outIdx,
        float* outRatings, int maxOutCount, FeatureId featureId );

private:
  QList<ImageDescriptor*> m_descriptors;
};


static int knn_compare( const void* _a, const void* _b )
{
  float a = **(const float**)_a;
  float b = **(const float**)_b;

  return (a < b) - (a > b);
} // knn_compare()


int ImageDescriptorDBImpl::ComputeRatingsUsingNaiveKNN(
  const ImageFeatureBase** inFeatures,
  const float*             inRatings,
        int                inCount,
        int*               outIdx,
        float*             outRatings,
        int                maxOutCount,
        FeatureId          featureId)
{
  int     i;
  int     j;
  int     n;
  float*  r;
  float** rp;
  double  scale;
  double  min_max = 0;
  double  distance;

  n = Size();

  r  = new float[n*2];
  rp = new float*[n];

  for( i = 0; i < n; i++ )
  {
    double min_dist    = FLT_MAX;
    double best_rating = 0;
    const ImageFeatureBase& curr = Descriptor(i).Feature(featureId);

    if(curr.IsEmpty())
      return 0;

    for( j = 0; j < inCount; j++ )
    {
      const ImageFeatureBase& ref = *inFeatures[j];
      if(ref.IsEmpty())
        return 0;

      distance = 0;

      ref.Compare(curr, distance);

      if( min_dist > distance )
      {
        min_dist    = distance;
        best_rating = inRatings[j];
      }
    }

    rp[i]  = &r[i];
    r[i]   = (float)best_rating;
    r[i+n] = (float)min_dist;

    if( min_max < min_dist )
      min_max = min_dist;
  }

  scale = -3./(min_max + FLT_EPSILON);

  // objects close to the
  for( i = 0; i < n; i++ )
    r[i] = (float)(r[i] * exp(r[i+n] * scale));

  qsort( rp, n, sizeof(rp[0]), knn_compare );

  n = IPP_MIN(n, maxOutCount);

  for( i = 0; i < n; i++ )
  {
    outIdx[i]     = (int)(rp[i] - r);
    outRatings[i] = *rp[i];
  }

  delete[] r;
  delete[] rp;

  return n;
} // ImageDescriptorDBImpl::ComputeRatingsUsingNaiveKNN()


/////////////////////////////////////////////////////////////////////
// ImageDescriptorDB

ImageDescriptorDB::ImageDescriptorDB(void)
{
  m_pImpl = 0;
  return;
} // ctor


ImageDescriptorDB::~ImageDescriptorDB(void)
{
  if(0 != m_pImpl)
    delete m_pImpl;

  return;
} // dtor


Status ImageDescriptorDB::Init(void)
{
  if(0 == m_pImpl)
  {
    m_pImpl = new ImageDescriptorDBImpl;
    if(0 == m_pImpl)
      return AllocFailed;
  }

  return Ok;
} // ImageDescriptorDB::Init()


Status ImageDescriptorDB::Clear(void)
{
  return m_pImpl->Clear();
} // ImageDescriptorDB::Clear()


int ImageDescriptorDB::Size(void) const
{
  return m_pImpl->Size();
} // ImageDescriptorDB::Size()


bool ImageDescriptorDB::IsEmpty(void) const
{
  return m_pImpl->IsEmpty();
} // ImageDescriptorDB::IsEmpty()


Status ImageDescriptorDB::Insert(ImageDescriptor& descriptor)
{
  return m_pImpl->Insert(descriptor);
} // ImageDescriptorDB::Insert()


Status ImageDescriptorDB::Remove(int id)
{
  return m_pImpl->Remove(id);
} // ImageDescriptorDB::Remove()


const ImageDescriptor& ImageDescriptorDB::Descriptor(int id) const
{
  return m_pImpl->Descriptor(id);
} // ImageDescriptorDB::Descriptor()


Status ImageDescriptorDB::Index(const char* name, int& index)
{
  return m_pImpl->Index(name,index);
} // ImageDescriptorDB::Index()


int ImageDescriptorDB::FindSimilar(
  const char**    names,
  const float*    inRatings0,
        int       inCount0,
  const char**    mostSimilar,
        float*    outRatings,
        int       maxOutCount,
        FeatureId featureId)
{
  int    i;
  int    n;
  int    idx;
  int    inCount;
  int*   outIdx;
  float* inRatings;

  const ImageFeatureBase** inFeatures = new const ImageFeatureBase*[inCount0];

  n = Size();

  outIdx    = new int[n];
  inRatings = new float[inCount0];

  n = IPP_MIN(Size(),maxOutCount);

  inCount = 0;

  for( i = 0; i < inCount0; i++ )
  {
    m_pImpl->Index(names[i], idx);
    if( idx >= 0 )
    {
      inFeatures[inCount] = &Descriptor(idx).Feature(featureId);
      inRatings[inCount]  = inRatings0[i];
      inCount++;
    }
  }

  n = m_pImpl->ComputeRatingsUsingNaiveKNN(inFeatures,inRatings,inCount,outIdx,outRatings,n,featureId);

  for( i = 0; i < n; i++ )
    mostSimilar[i] = ((FileNameFeature&)Descriptor(outIdx[i]).Feature(ImageFileName)).Get();

  delete[] inFeatures;
  delete[] inRatings;
  delete[] outIdx;

  return n;
} // ImageDescriptorDB::FindSimilar()

} // namespace ISF

