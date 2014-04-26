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
#ifndef __IPPDEFS_H__
#include "ippdefs.h"
#endif
#ifndef __IPPCORE_H__
#include "ippcore.h"
#endif
#ifndef __IPPS_H__
#include "ipps.h"
#endif
#ifndef __IMAGEFEA_H__
#include "imagefea.h"
#endif

namespace ISF
{

static const int DEF_NUM_FEATURES = 6;

static FakeFeature fakeFeature;

/////////////////////////////////////////////////////////////////////
// FileNameFeatureImpl

class FileNameFeatureImpl
{
public:
  FileNameFeatureImpl(void) {}
  ~FileNameFeatureImpl(void) { Clear(); }

  Status Clear(void)
  {
    m_FileName.clear();
    return Ok;
  }

  bool IsEmpty(void) const { return m_FileName.isEmpty(); }

  Status Set(const char* name)
  {
    m_FileName.resize(strlen(name));
    qstrcpy(m_FileName.data(),name);
    return Ok;
  }

  const char* Get(void) const { return m_FileName.data(); }

private:
  QByteArray m_FileName;
};


/////////////////////////////////////////////////////////////////////
// FileNameFeature

FileNameFeature::FileNameFeature(void)
{
  m_pFileName = 0;
  return;
} // ctor


FileNameFeature::~FileNameFeature(void)
{
  if(0 != m_pFileName)
    delete m_pFileName;

  return;
} // dtor


Status FileNameFeature::Init(void)
{
  m_pFileName = new FileNameFeatureImpl;
  if(0 == m_pFileName)
    return AllocFailed;

  return Ok;
} // FileNameFeature::Init()


Status FileNameFeature::Clear(void)
{
  return m_pFileName->Clear();
} // FileNameFeature::Clear()


bool FileNameFeature::IsEmpty(void) const
{
  return m_pFileName->IsEmpty();
} // FileNameFeature::IsEmpty()


Status FileNameFeature::Set(const char* name)
{
  return m_pFileName->Set(name);
} // FileNameFeature::Set()


const char* FileNameFeature::Get(void) const
{
  return m_pFileName->Get();
} // FileNameFeature::Get()


Status FileNameFeature::Compare(const ImageFeatureBase& another, double& distance) const
{
  if(another.Id() != Id())
    return BadParams;

  distance = strcmp(Get(), ((FileNameFeature&)another).Get()) == 0 ? 0 : 1;

  return Ok;
} // FileNameFeature::Compare()


Status FileNameDistance::Distance(const ImageFeatureBase& src1,const ImageFeatureBase& src2, double& distance)
{
  if(ImageFileName != src1.Id() || ImageFileName != src2.Id())
    return BadParams;

  const FileNameFeature& fn1 = reinterpret_cast<const FileNameFeature&>(src1);
  const FileNameFeature& fn2 = reinterpret_cast<const FileNameFeature&>(src2);

  distance = strcmp(fn1.Get(), fn2.Get()) == 0 ? 0.0 : 1.0;

  return Ok;
} // FileNameDistance::Distance()

} // namespace ISF

