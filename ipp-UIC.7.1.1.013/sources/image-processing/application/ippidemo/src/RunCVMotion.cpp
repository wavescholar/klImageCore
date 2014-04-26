/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVMotion.cpp: implementation of the CRunCVMotion class.
// CRunCVMotion class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "DemoDoc.h"
#include "RunCVMotion.h"
#include "ParmCVMotionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVMotion::CRunCVMotion()
{
   m_timestamp = 1;
   m_mhiDuration = 0.1f;

}

CRunCVMotion::~CRunCVMotion()
{

}

BOOL CRunCVMotion::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2;
   return TRUE;
}

CParamDlg* CRunCVMotion::CreateDlg() {
   return new CParmCVMotionDlg();
}

BOOL CRunCVMotion::PickNextPos()
{
   if (m_PickVecPos == VEC_SRC) {
      m_PickVecPos = VEC_SRC2;
   } else {
      m_PickVecPos = -1;
      return FALSE;
   }
   return TRUE;
}

CMyString CRunCVMotion::GetVectorName(int vecPos)
{
   if (vecPos == VEC_SRC2) {
      return "MHI";
   } else {
      return "";
   }
}

BOOL CRunCVMotion::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
   if (vecPos == VEC_SRC2) {
      pickID = IDC_CURSOR_PMHI;
      grabID = IDC_CURSOR_MHI;
   } else {
      return FALSE;
   }
   return TRUE;
}

void CRunCVMotion::GrabDoc(CDemoDoc* pDoc)
{
   if (m_PickVecPos == VEC_SRC2)
      m_pDocSrc2 = pDoc;
   else
      return;
   pDoc->IsPicked(TRUE);
}

void CRunCVMotion::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmCVMotionDlg* pDlg = (CParmCVMotionDlg*)parmDlg;
   if (save) {
      m_timestamp = (Ipp32f)atof(pDlg->m_StampStr);
      m_mhiDuration = (Ipp32f)atof(pDlg->m_DurationStr);
   } else {
      pDlg->m_StampStr.Format("%4g", m_timestamp);
      pDlg->m_DurationStr.Format("%4g", m_mhiDuration);
   }
}

CString CRunCVMotion::GetHistoryParms()
{
   CMyString parms;
   parms << m_timestamp << ", " << m_mhiDuration;
   return parms;
}

IppStatus CRunCVMotion::CallIppFunction()
{
   FUNC_CV_CALL(ippiUpdateMotionHistory_8u32f_C1IR,
                        ((Ipp8u*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize,
                          m_timestamp, m_mhiDuration ))
   return stsNoFunction;
}
