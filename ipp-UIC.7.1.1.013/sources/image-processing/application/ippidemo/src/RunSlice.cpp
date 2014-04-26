/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSlice.cpp: implementation of the CRunSlice class.
// CRunSlice class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippidemodoc.h"
#include "RunSlice.h"
#include "ParmSliceDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunSlice::CRunSlice()
{
   m_slice = 16;
}

CRunSlice::~CRunSlice()
{

}

CParamDlg* CRunSlice::CreateDlg() {
   return new CParmSliceDlg;
}

void CRunSlice::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmSliceDlg* pDlg = (CParmSliceDlg*)parmDlg;
   if (save) {
      m_slice = atoi(pDlg->m_SliceStr);
      m_slice &= ~15;
      if (m_slice == 0) m_slice = 16;
   } else {
      pDlg->m_SliceStr.Format("%d",m_slice);
   }
}

BOOL CRunSlice::PrepareSrc()
{
   CImage* pSrcImage = m_pDocSrc->GetImage();
   IppiRect roi = pSrcImage->GetActualRoi();
   int height = roi.height & ~15;

   if (height == roi.height) return TRUE;
   if (height == 0) {
      if (pSrcImage->Height() >= 16) {
         roi.y -= 16 - roi.height;
         if (roi.y < 0) roi.y = 0;
         height = 16;
      } else {
         DEMO_APP->MessageBox(
             " Height of the image is too small for " + m_Func,
            MB_OK | MB_ICONEXCLAMATION);
         return FALSE;
      }
   }
   roi.height = height;
   if (DEMO_APP->MessageBox(
      (m_pDocSrc->GetImage()->GetRoi() ?
      " ROI will be updated in " :
      " ROI will be created in ") +
      m_pDocSrc->GetTitle(),
      MB_OKCANCEL) != IDOK) return FALSE;
   m_pDocSrc->GetImage()->SetRoi(&roi);
   return TRUE;
}

static int getNumSlices(int height, int slice)
{
   int n = height/slice;
   int d = height - slice*n;
   if (d >= 16) n++;
   return n;
}

static int getLastSlice(int height, int slice)
{
   int n = height/slice;
   int d = height - slice*n;
   if (d < 16) return slice;
   return d & ~15;
}

BOOL CRunSlice::CallIpp(BOOL bMessage)
{
   BOOL result = FALSE;
   int n       = getNumSlices(roiSize.height,m_slice);
   int sliceY  = m_slice;
   int sliceUV = m_slice>>1;
   m_pSrcY  = (char*)pSrcP[0];
   m_pSrcUV = (char*)pSrcP[1];
   m_pDst   = (char*)pDst;
   m_pDstP[0] = (char*)pDstP[0];
   m_pDstP[1] = (char*)pDstP[1];
   m_pDstP[2] = (char*)pDstP[2];
   m_roiSize = roiSize;
   if (n == 0) {
      m_layout = IPP_LOWER && IPP_UPPER && IPP_CENTER;
      return CippiRun::CallIpp(bMessage);
   }
   m_roiSize.height = sliceY;
   m_layout = IPP_UPPER;
   for (int i=0; i<n; i++) {
      if (i == n - 1) {
         m_layout = IPP_LOWER;
         if (i == 0)
            m_layout = IPP_LOWER && IPP_UPPER && IPP_CENTER;
         m_roiSize.height = getLastSlice(roiSize.height, m_slice);
      }
      result = CippiRun::CallIpp(bMessage);
      if (result == FALSE) break;
      m_pSrcY  += srcStepP[0]*sliceY;
      m_pSrcUV += srcStepP[1]*sliceUV;
      m_pDst += dstStep*sliceY;
      m_pDstP[0] += dstStepP[0]*sliceY;
      m_pDstP[1] += dstStepP[1]*sliceUV;
      m_pDstP[2] += dstStepP[2]*sliceUV;
      m_layout = IPP_CENTER;
   }
   return result;
}


void CRunSlice::Loop(int num)
{
   while (num--) CallIpp(FALSE);
}

IppStatus CRunSlice::CallIppFunction()
{
   FUNC_CC_CALL(ippiYCbCr420ToYCbCr422_Filter_8u_P2C2R,(
       (Ipp8u*)m_pSrcY , srcStepP[0],
       (Ipp8u*)m_pSrcUV, srcStepP[1],
       (Ipp8u*)m_pDst, dstStep,
       m_roiSize, m_layout))
   FUNC_CC_CALL(ippiYCbCr420ToYCrCb420_Filter_8u_P2P3R,(
       (Ipp8u*)m_pSrcY , srcStepP[0],
       (Ipp8u*)m_pSrcUV, srcStepP[1],
       (Ipp8u**)m_pDstP, dstStepP,
       m_roiSize, m_layout))

   return stsNoFunction;
}

CString CRunSlice::GetHistoryInfo() {
   CMyString info;
   return info << "slice=" << m_slice;
}

