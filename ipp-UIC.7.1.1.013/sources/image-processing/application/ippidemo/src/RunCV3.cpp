/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCV3.cpp: implementation of the CRunCV3 class.
// CRunCV3 class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCV3.h"
#include "ParmIMDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCV3::CRunCV3()
{
   m_pDocSrc3 = NULL;
   pSrc3 = NULL;
}

CRunCV3::~CRunCV3()
{

}

BOOL CRunCV3::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_bThird = m_Func.Found("Product");
   m_bMask  = m_Func.Mask();
   if (m_Func.Found("Abs"))
      m_UsedVectors = VEC_SRC;
   else
      m_UsedVectors = VEC_SRC | VEC_SRC2;
   m_pDocSrc3 = NULL;
   pSrc3 = NULL;
   return TRUE;
}

CParamDlg* CRunCV3::CreateDlg() {
   return NULL;
}

BOOL CRunCV3::PickNextPos()
{
   if (m_bThird) {
      if (m_PickVecPos == VEC_SRC) {
         m_PickVecPos = VEC_SRC2;
      } else if (m_PickVecPos == VEC_SRC2) {
         if (m_bMask)
            m_PickVecPos = VEC_MASK;
         else
            m_PickVecPos = VEC_SRC3;
      } else if (m_PickVecPos == VEC_MASK) {
         m_PickVecPos = VEC_SRC3;
      } else {
         m_PickVecPos = -1;
         return FALSE;
      }
   } else {
      if (m_PickVecPos == VEC_SRC) {
         if (m_bMask)
            m_PickVecPos = VEC_MASK;
         else
            m_PickVecPos = VEC_SRC2;
      } else if (m_PickVecPos == VEC_MASK) {
         m_PickVecPos = VEC_SRC2;
      } else {
         m_PickVecPos = -1;
         return FALSE;
      }
   }
   return TRUE;
}


BOOL CRunCV3::GetEqualDocPos(int& firstPos, int& secondPos)
{
   if (m_bThird) {
      secondPos = VEC_SRC3;
      if (m_pDocSrc == m_pDocSrc3) {
         firstPos = VEC_SRC;
         return TRUE;
      }
      if (m_pDocSrc2 == m_pDocSrc3) {
         firstPos = VEC_SRC2;
         return TRUE;
      }
      if (m_pDocMask == m_pDocSrc3) {
         firstPos = VEC_MASK;
         return TRUE;
      }
   } else {
      secondPos = VEC_SRC2;
      if (m_pDocSrc == m_pDocSrc2) {
         firstPos = VEC_SRC;
         return TRUE;
      }
      if (m_pDocMask == m_pDocSrc2) {
         firstPos = VEC_MASK;
         return TRUE;
      }
   }
   return FALSE;
}

CMyString CRunCV3::GetVectorName(int vecPos)
{
   if (vecPos == VEC_SRC2) {
      if (m_bThird) {
         return "Src2";
      } else {
         return "SrcDst";
      }
   } else if (vecPos == VEC_MASK) {
      return "Mask";
   } else if (vecPos == VEC_SRC3) {
      return "SrcDst";
   } else {
      return "";
   }
}

BOOL CRunCV3::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
   if (vecPos == VEC_SRC2) {
      if (m_bThird) {
         pickID = IDC_CURSOR_PS2;
         grabID = IDC_CURSOR_S2;
      } else {
         pickID = IDC_CURSOR_PSD;
         grabID = IDC_CURSOR_SD;
      }
   } else if (vecPos == VEC_MASK) {
      pickID = IDC_CURSOR_PM;
      grabID = IDC_CURSOR_M;
   } else if (vecPos == VEC_SRC3) {
      pickID = IDC_CURSOR_PSD;
      grabID = IDC_CURSOR_SD;
   } else {
      return FALSE;
   }
   return TRUE;
}

int CRunCV3::GetPickVecPosToValid()
{
   if (m_PickVecPos == VEC_MASK)
      return VEC_MASK;
   else if (m_bThird && (m_PickVecPos == VEC_SRC2))
      return VEC_SRC;
   else
      return VEC_DST;
}

void CRunCV3::GrabDoc(CDemoDoc* pDoc)
{
   if (m_PickVecPos == VEC_SRC2)
      m_pDocSrc2 = pDoc;
   if (m_PickVecPos == VEC_SRC3) {
      m_pDocSrc3 = pDoc;
      pDoc->IsPicked(TRUE);
   } else {
      CippiRun::GrabDoc(pDoc);
   }
}

void CRunCV3::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmIMDlg* pDlg = (CParmIMDlg*)parmDlg;
   if (save) {
   } else {
      pDlg->m_pDocSrc3 = m_pDocSrc3;
   }
}

void CRunCV3::PrepareParameters()
{
   roiSize.width  = -1;
   roiSize.height = -1;
   IntersectRoi((CippiDemoDoc*)m_pDocSrc , roiSize);
   IntersectRoi((CippiDemoDoc*)m_pDocSrc2, roiSize);
   IntersectRoi((CippiDemoDoc*)m_pDocMask, roiSize);
   IntersectRoi((CippiDemoDoc*)m_pDocSrc3, roiSize);
   SetImageParameters((CippiDemoDoc*)m_pDocSrc, pSrc, srcStep);
   SetImageParameters((CippiDemoDoc*)m_pDocSrc2, pSrc2, srcStep2);
   SetImageParameters((CippiDemoDoc*)m_pDocMask, pMask, maskStep);
   SetImageParameters((CippiDemoDoc*)m_pDocSrc3, pSrc3, srcStep3);

}

void CRunCV3::ActivateDst()
{
   if (m_bThird) {
      ActivateDoc(m_pDocSrc3);
   } else {
      ActivateDoc(m_pDocMask);
      ActivateDoc(m_pDocSrc2);
   }
}

void CRunCV3::SaveInplaceImage()
{
   if (m_pDocSrc3) {
      m_InplaceImage = *((CImage*)m_pDocSrc3);
      SetImageParameters(&m_InplaceImage, pSrc3, srcStep3);
   } else {
      m_InplaceImage = *((CImage*)m_pDocSrc2);
      SetImageParameters(&m_InplaceImage, pSrc2, srcStep2);
   }
}

void CRunCV3::RestoreInplaceImage()
{
   if (m_pDocSrc3) {
      SetImageParameters((CippiDemoDoc*)m_pDocSrc3, pSrc3, srcStep3);
   } else {
      SetImageParameters((CippiDemoDoc*)m_pDocSrc2, pSrc2, srcStep2);
   }
}

void CRunCV3::SetHistory()
{
   if (m_bThird)
      SetToHisto(m_pDocSrc3, VEC_SRC2);
   else
      SetToHisto(m_pDocSrc2, VEC_SRC3);
}

void CRunCV3::SetToHistoSrc(CDemoDoc* pDoc, int vecPos)
{
   CHisto* pHisto = pDoc->GetHisto();
   pHisto->RemoveAll();
   pHisto->AddTail(m_pDocSrc->GetHisto());
   if (m_bThird)
      pHisto->AddTail(m_pDocSrc2->GetTitle());
   if (m_bMask)
      pHisto->AddTail(m_pDocMask->GetTitle());
   pHisto->AddTail(pDoc->GetTitle());
}

IppStatus CRunCV3::CallIppFunction()
{
   FUNC_CV_CALL(ippiAdd_8u32f_C1IR,        ((Ipp8u *) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAdd_8s32f_C1IR,        ((Ipp8s *) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAdd_16u32f_C1IR,        ((Ipp16u *) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_8u32f_C1IR,  ((Ipp8u *) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_8s32f_C1IR,  ((Ipp8s *) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_16u32f_C1IR,  ((Ipp16u *) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_32f_C1IR,    ((Ipp32f*) pSrc, srcStep, (Ipp32f*) pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAdd_8u32f_C1IMR,       ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAdd_8s32f_C1IMR,       ((Ipp8s *)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAdd_16u32f_C1IMR,       ((Ipp16u *)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAdd_32f_C1IMR,         ((Ipp32f*)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_8u32f_C1IMR, ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_8s32f_C1IMR, ((Ipp8s *)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_16u32f_C1IMR, ((Ipp16u *)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddSquare_32f_C1IMR,   ((Ipp32f*)pSrc, srcStep, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_8u32f_C1IR, ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_8s32f_C1IR, ((Ipp8s *)pSrc, srcStep, (Ipp8s *)pSrc2, srcStep2, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_16u32f_C1IR, ((Ipp16u *)pSrc, srcStep, (Ipp16u *)pSrc2, srcStep2, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_32f_C1IR,   ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_8u32f_C1IMR,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_8s32f_C1IMR,((Ipp8s *)pSrc, srcStep, (Ipp8s *)pSrc2, srcStep2, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_16u32f_C1IMR,((Ipp16u *)pSrc, srcStep, (Ipp16u *)pSrc2, srcStep2, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   FUNC_CV_CALL(ippiAddProduct_32f_C1IMR,  ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, (Ipp8u *)pMask, maskStep, (Ipp32f*)pSrc3, srcStep3, roiSize ))
   return stsNoFunction;
}
