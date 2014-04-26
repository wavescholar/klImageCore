/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConvert1u.cpp: implementation of the CRunConvert1u class.
// CRunConvert1u class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippidemodoc.h"
#include "RunConvert1u.h"
#include "ParmConvert1uDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunConvert1u::CRunConvert1u()
{
   m_offset = 0;
   m_threshold = 127;
}

CRunConvert1u::~CRunConvert1u()
{

}

BOOL CRunConvert1u::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_bToBits = func.Found("8u1u");
   return TRUE;
}

CParamDlg* CRunConvert1u::CreateDlg() { return new CParmConvert1uDlg;}

void CRunConvert1u::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmConvert1uDlg* pDlg = (CParmConvert1uDlg*)parmDlg;
   if (save) {
      m_threshold = atoi(pDlg->m_ThreshStr);
   } else {
      pDlg->m_ThreshStr.Format("%d",m_threshold);
   }
}

BOOL CRunConvert1u::PrepareDst()
{
   if (m_pDocDst && m_bToBits) {
      CImage* pDstImage = m_pDocDst->GetImage();
      if (!pDstImage->IsBitImage()) {
         pDstImage->IsBitImage(TRUE);
         pDstImage->BitOffset(0);
         pDstImage->BitWidth(pDstImage->Width() << 3);
         pDstImage->BitRoiWidth(pDstImage->GetActualRoi().width << 3);
      }
   }
   return CippiRun::PrepareDst();
}


ImgHeader CRunConvert1u::GetNewDstHeader()
{
   CImage* pSrcImage = m_pDocSrc->GetImage();
   ImgHeader header = pSrcImage->GetHeader();

   BOOL bSizeAsRoi = DEMO_APP->GetNewSize() == NEWSIZE_ROI;
   if (bSizeAsRoi) {
      const IppiRect* pRoi = pSrcImage->GetRoi();
      if (pRoi)
         header.SetSize(pRoi->width, pRoi->height);
   }

   if (m_bToBits) {
      header.width = (header.width + 7)>>3;
   } else if (pSrcImage->IsBitImage()) {
      header.width = bSizeAsRoi ? pSrcImage->BitRoiWidth() : pSrcImage->BitWidth();
   } else {
      header.width = header.width<<3;
   }
   return header;
}

void CRunConvert1u::CopyContoursToNewDst()
{
   CImage* pSrcImage = m_pDocSrc->GetImage();
   CImage* pDstImage = m_pDocDst->GetImage();
   if (m_bToBits) {
      pDstImage->IsBitImage(TRUE);
      if (DEMO_APP->GetNewSize() == NEWSIZE_ROI || !pSrcImage->IsRoi()) {
         IppiRect srcRoi = m_pDocSrc->GetImage()->GetActualRoi();
         pDstImage->BitOffset(0);
         pDstImage->BitWidth(srcRoi.width);
         pDstImage->BitRoiWidth(srcRoi.width);
      } else {
         IppiRect srcRoi = *pSrcImage->GetRoi();
         IppiRect dstRoi = srcRoi;
         pDstImage->BitOffset(srcRoi.x & 7);
         pDstImage->BitWidth(pSrcImage->Width());
         pDstImage->BitRoiWidth(srcRoi.width);
         dstRoi.x = srcRoi.x >> 3;
         dstRoi.width = ((srcRoi.x + srcRoi.width + 7)>>3) - dstRoi.x;
         pDstImage->SetRoi(&dstRoi);
      }
   } else {
      if (DEMO_APP->GetNewSize() == NEWSIZE_ROI) return;
      if (!pSrcImage->IsRoi()) return;
      IppiRect dstRoi = *pSrcImage->GetRoi();
      if (pSrcImage->IsBitImage()) {
         dstRoi.x = (dstRoi.x<<3) + pSrcImage->BitOffset();
         dstRoi.width = pSrcImage->BitRoiWidth();
      } else {
         dstRoi.x = dstRoi.x<<3;
         dstRoi.width = dstRoi.width<<3;
      }
      pDstImage->SetRoi(&dstRoi);
   }
}

void CRunConvert1u::PrepareParameters()
{
   CImage* pSrcImage = m_pDocSrc->GetImage();
   CImage* pDstImage = m_pDocDst->GetImage();
   SetImageParameters(pSrcImage, pSrc, srcStep);
   SetImageParameters(pDstImage, pDst, dstStep);
   if (m_bToBits) {
      roiSize.width = pSrcImage->GetActualRoi().width;
      roiSize.height = pSrcImage->GetActualRoi().height;
      IntersectBitRoi(pDstImage, roiSize);
      m_offset = pDstImage->IsBitImage() ? pDstImage->BitOffset() : 0;
   } else {
      roiSize.width = pDstImage->GetActualRoi().width;
      roiSize.height = pDstImage->GetActualRoi().height;
      IntersectBitRoi(pSrcImage, roiSize);
      m_offset = pSrcImage->IsBitImage() ? pSrcImage->BitOffset() : 0;
   }
}

void CRunConvert1u::IntersectBitRoi(CImage* pBitImage, IppiSize& roiSize)
{
   IppiRect roi = pBitImage->GetActualRoi();
   if (roiSize.height > roi.height)
      roiSize.height = roi.height;
   if (pBitImage->IsBitImage()) {
      if (roiSize.width > pBitImage->BitRoiWidth())
         roiSize.width = pBitImage->BitRoiWidth();
   } else {
      if (roiSize.width > roi.width << 3)
         roiSize.width = roi.width << 3;
   }
}

IppStatus CRunConvert1u::CallIppFunction()
{
   FUNC_CALL(ippiConvert_1u8u_C1R, ((Ipp8u*)pSrc, srcStep, m_offset, (Ipp8u*)pDst, dstStep, roiSize ))
   return stsNoFunction;
}

CString CRunConvert1u::GetHistoryParms()
{
   CMyString parms;
   parms << m_offset;
   if (m_bToBits)
      parms << m_threshold;
   return parms;
}



