/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStatCount.cpp: implementation of the CRunStatCount class.
// CRunStatCount class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunStatCount.h"
#include "ParmStatCountDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunStatCount::CRunStatCount()
{
   m_lowerBound.Init(pp8u ,3);
   m_upperBound.Init(pp8u ,3);
   m_lowerBound.Init(pp32f,3);
   m_upperBound.Init(pp32f,3);
   for (int i=0; i<3; i++) {
      m_lowerBound.Vector(pp8u ).Set(i, 0);
      m_upperBound.Vector(pp8u ).Set(i, 127);
      m_lowerBound.Vector(pp32f).Set(i, 0);
      m_upperBound.Vector(pp32f).Set(i, 0.5);
   }
}

CRunStatCount::~CRunStatCount()
{

}

BOOL CRunStatCount::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   m_lowerBound.Init(func.SrcType());
   m_upperBound.Init(func.SrcType());
   return TRUE;
}

void CRunStatCount::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmStatCountDlg *pDlg = (CParmStatCountDlg*)parmDlg;
   if (save) {
   } else {
      m_value.Get(pDlg->m_valueStr);
   }
}

void CRunStatCount::UpdateBounds(CParmStatCountDlg* pDlg, CFunc func)
{
   if (func.Found("Count")) {
      m_lowerBound.Set(pDlg->m_xStr);
      m_upperBound.Set(pDlg->m_yStr);
   }
}

void CRunStatCount::UpdateBounds(CParmStatCountDlg* pDlg, BOOL bSave)
{
   if (m_Func.Found("Count")) {
      if (bSave) {
         m_lowerBound.Set(pDlg->m_xStr);
         m_upperBound.Set(pDlg->m_yStr);
      } else {
         m_lowerBound.Get(pDlg->m_xStr);
         m_upperBound.Get(pDlg->m_yStr);
      }
   }
}

BOOL CRunStatCount::CallIpp(BOOL bMessage)
{
   CParmStatCountDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunStatCount::SetValues(CParmStatCountDlg* pDlg)
{
   UpdateData(pDlg);
   m_value.Init(pp32s, 3);
   PrepareParameters();
   if (CippiRun::CallIpp() < 0) return;
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunStatCount::CallIppFunction()
{
   FUNC_CALL(ippiCountInRange_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize,
                                            m_value, m_lowerBound, m_upperBound))
   FUNC_CALL(ippiCountInRange_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize,
                                            m_value, m_upperBound, m_upperBound))
   FUNC_CALL(ippiCountInRange_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize,
                                            m_value, m_upperBound, m_upperBound))
   FUNC_CALL(ippiCountInRange_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize,
                                            m_value, m_lowerBound, m_upperBound))
   FUNC_CALL(ippiCountInRange_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize,
                                            m_value, m_upperBound, m_upperBound))
   FUNC_CALL(ippiCountInRange_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize,
                                            m_value, m_upperBound, m_upperBound))

   return stsNoFunction;
}

CString CRunStatCount::GetHistoryParms()
{
   CMyString parms;
   return parms;
}
