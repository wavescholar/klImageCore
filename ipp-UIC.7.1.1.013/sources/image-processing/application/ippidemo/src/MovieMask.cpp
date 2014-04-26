/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieMask.cpp: implementation of the CMovieMask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ippcc.h>
#include "ippiDemo.h"
#include "MovieMask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMovieMask::CMovieMask()
{
   Clear();
}

void CMovieMask::Clear()
{
   m_ID = 0;
   m_Threshold = 0;
   m_Min = 0;
   m_Max = 0;
}

CMovieMask::CMovieMask(int nID, CString name)
{
   Clear();
   Init(nID, name);
}

CMovieMask::CMovieMask(CString filePath)
{
   Clear();
   Init(filePath);
}

CMovieMask& CMovieMask::operator =(const CMovieMask& mask)
{
   *(CImage*)this = mask;
   m_Img       = mask.m_Img      ;
   m_Name      = mask.m_Name     ;
   m_ID        = mask.m_ID       ;
   m_FilePath  = mask.m_FilePath ;
   m_Threshold = mask.m_Threshold;
   m_Min       = mask.m_Min      ;
   m_Max       = mask.m_Max      ;
   return *this;
}

CMovieMask::~CMovieMask()
{

}
void CMovieMask::Init(int nID, CString name)
{
   m_FilePath.Empty();
   m_ID = nID;
   m_Name = name;
}

void CMovieMask::Init(CString filePath)
{
   m_ID = 0;
   m_FilePath = filePath;
   m_Name = m_FilePath.GetTitle();
}

BOOL CMovieMask::Load(BOOL bMessage)
{
   if (m_ID) {
      LoadPattern();
      return TRUE;
   } else if (!m_FilePath.IsEmpty()) {
      if (!LoadFile(bMessage))
         return FALSE;
   } else {
      return FALSE;
   }
   m_Img.ConvertToRGB();
   ImgConvertToGray();
   *(CImage*)this = m_Img;
   InitThreshold();
   return TRUE;
}

void CMovieMask::InitThreshold() {
   IppiSize size = {m_Img.Width(), m_Img.Height()};
   ippiMinMax_8u_C1R((Ipp8u*)m_Img.GetData(), m_Img.Step(), size, &m_Min, &m_Max);
   SetThreshold((m_Max + m_Min) >> 1);
}

void CMovieMask::LoadPattern()
{
   CBitmap bitmap;
   bitmap.LoadBitmap(m_ID);
   BITMAP bm;
   bitmap.GetObject(sizeof(BITMAP), &bm);
   ASSERT(bm.bmPlanes == 1);
   int nCount = bm.bmHeight*bm.bmWidthBytes;
   char* buffer = new char[nCount];
   SetHeader(pp8u, 1, bm.bmWidth, bm.bmHeight);
   AllocateData();
   if ((int)bitmap.GetBitmapBits(nCount, buffer) == nCount) {
      Ipp8u* pSrc = (Ipp8u*)buffer;
      Ipp8u* pDst = (Ipp8u*)GetData();
      for (int y=0; y<Height(); y++) {
         for (int x=0; x<Width(); x++) {
            switch (bm.bmBitsPixel) {
            case 8:  pDst[x] = pSrc[x]; break;
            case 16: pDst[x] = ((Ipp16u*)pSrc)[x] ? 255 : 0; break;
            case 32: pDst[x] = ((Ipp32u*)pSrc)[x] ? 255 : 0; break;
            default: pDst[x] = 255; break;
            }
         }
         pSrc += bm.bmWidthBytes;
         pDst += Step();
      }
   }
   bitmap.DeleteObject();
   delete[] buffer;
}

BOOL CMovieMask::LoadFile(BOOL bMessage)
{
   return FALSE;
}

void CMovieMask::ImgConvertToGray()
{
   if (m_Img.Channels() == 1) return;
   IppiSize size = {m_Img.Width(), m_Img.Height()};
   ImgHeader header = m_Img.GetHeader();
   header.channels = 1;
   CImage srcImg = m_Img;
   m_Img.Reset();
   m_Img.SetHeader(header);
   m_Img.AllocateData();
   switch (srcImg.Channels()) {
   case 3:
      ippiRGBToGray_8u_C3C1R(
         (Ipp8u*)srcImg.GetData(), srcImg.Step(),
         (Ipp8u*)m_Img.GetData(), m_Img.Step(), size);
      break;
   case 4:
      ippiRGBToGray_8u_AC4C1R(
         (Ipp8u*)srcImg.GetData(), srcImg.Step(),
         (Ipp8u*)m_Img.GetData(), m_Img.Step(), size);
      break;
   }
}

BOOL CMovieMask::SetThreshold(Ipp8u threshold)
{
   if (IsPattern()) return FALSE;
   if (threshold == 0) threshold = 1;
   m_Threshold = threshold;
   IppiSize size = {Width(), Height()};
   ippiThreshold_LTValGTVal_8u_C1R(
      (Ipp8u*)m_Img.GetData(), m_Img.Step(),
      (Ipp8u*)GetData(), Step(), size,
      m_Threshold, 0,
      m_Threshold - 1, 255);
   return TRUE;
}
