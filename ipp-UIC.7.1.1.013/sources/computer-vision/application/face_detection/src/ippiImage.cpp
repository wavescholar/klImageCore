/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives Face Detection Sample for Windows*
//
//   By downloading and installing this sample, you hereby agree that the
//   accompanying Materials are being provided to you under the terms and
//   conditions of the End User License Agreement for the Intel(R) Integrated
//   Performance Primitives product previously accepted by you. Please refer
//   to the file ippEULA.rtf located in the root directory of your Intel(R) IPP
//   product installation for more information.
//
//
*/

// IppiImage.cpp: implementation of the CIppiImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiImage.h"
#include "ippiImageStore.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIppiImage::CIppiImage()
{
    m_pData = NULL;
    Reset();
}

void CIppiImage::Reset()
{
    m_Updated = TRUE;
    m_type = ppNONE;
    m_width = 0;
    m_height = 0;
    m_channels = 0;
    m_plane = 0;
    FreeData();
}

CIppiImage::CIppiImage(const CIppiImage& img) 
{
    m_pData = NULL;
   *this = img;
}

CIppiImage::~CIppiImage()
{
    FreeData();
}

CIppiImage& CIppiImage::operator =(const CIppiImage& image)
{
   Reset();
   m_type     = image.m_type    ;
   m_width    = image.m_width   ;
   m_height   = image.m_height  ;
   m_channels = image.m_channels;
   m_plane    = image.m_plane   ;
    m_Updated  = image.m_Updated ;
   if (image.m_pData) {
      AllocateData();
      memcpy(m_pData,image.m_pData,DataSize());
   }
   return *this;
}

BOOL CIppiImage::CreateImage(int width, int height, 
                         int channels, ppType type, BOOL plane)
{
    Reset();
    m_type = type;
    m_channels = channels;
    m_width  = width;
    m_height = height;
    m_plane = plane;
    AllocateData();
    return m_pData ? TRUE : FALSE;
}

BOOL CIppiImage::CreateImage(CImgHeader header)
{
   return CreateImage(header.m_width, header.m_height, 
                      header.m_channels, header.m_type, header.m_plane);
}

BOOL CIppiImage::SetSize(int width, int height) 
{
    if (width<=0 || height<=0) return FALSE;
    if (m_width*m_height <= width*height) return FALSE;
    m_width  = width;
    m_height = height;
    return TRUE;
}

BOOL CIppiImage::SaveImage(CFile* pFile)
{
   CIppiImageStore store(this);
   return store.Save(pFile);
}

BOOL CIppiImage::LoadImage(CFile* pFile)
{
   CIppiImageStore store(this);
   return store.Load(pFile);
}
 
int CIppiImage::Step() const 
{
    if (m_plane) 
       return ((m_width*(Depth()>>3)+3)>>2)<<2;
    else if (m_type & PP_CPLX)
       return ((m_width*m_channels*2*(Depth()>>3)+3)>>2)<<2;
    else
       return ((m_width*m_channels*(Depth()>>3)+3)>>2)<<2;
}

int CIppiImage::DataSize()  const
{
    if (m_plane) return Step()*m_height*m_channels;
    else         return Step()*m_height;
}

void CIppiImage::AllocateData() 
{
   Ipp8u value = 0;
   m_pData = malloc(DataSize());
   if (!m_pData) return;
   memset(m_pData,value,DataSize());
}

void CIppiImage::FreeData() 
{
    if (m_pData) free(m_pData);
    m_pData = NULL;
}

void* CIppiImage::DataPtr() 
{
    return m_pData;
}

CString CIppiImage::TypeString() const
{
   switch (m_type) {
   case pp8u  : return "8u";
   case pp8s  : return "8s";
   case pp8sc : return "8sc";
   case pp16u : return "16u";
   case pp16s : return "16s";
   case pp16sc: return "16sc";
   case pp32u : return "32u";
   case pp32s : return "32s";
   case pp32sc: return "32sc";
   case pp32f : return "32f";
   case pp32fc: return "32fc";
   case pp64s : return "64s";
   case pp64sc: return "64sc";
   case pp64f : return "64f";
   case pp64fc: return "64fc";
   default: return "";
   }
}

CString CIppiImage::ChannelString() const 
{
   CString C = Plane() ? "P" : "C";
   switch (m_channels) {
   case 1: return C + "1";
   case 2: return C + "2";
   case 3: return C + "3";
   case 4: return "A" + C + "4";
   default: return "";
   }
}
