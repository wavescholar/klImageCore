/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSumLn.cpp : implementation of the CRunSumLn class.
// CRunSumLn class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunSumLn.h"
#include "ParmSumLnDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunSumLn::CRunSumLn()
{

}

CRunSumLn::~CRunSumLn()
{

}

BOOL CRunSumLn::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   m_value.Init(func,argDST);
   return TRUE;
}

void CRunSumLn::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmSumLnDlg *pDlg = (CParmSumLnDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      m_value.Init(m_Func,argDST);
   } else {
      m_value.Get(pDlg->m_ValStr);
   }
}

BOOL CRunSumLn::CallIpp(BOOL bMessage)
{
   CParmSumLnDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunSumLn::SetValues(CParmSumLnDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunSumLn::CallIppFunction()
{
   FUNC_CALL(ippsSumLn_32f,((Ipp32f*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsSumLn_64f,((Ipp64f*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsSumLn_32f64f,((Ipp32f*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsSumLn_16s32f,((Ipp16s*)pSrc, len, (Ipp32f*)m_value))

   return stsNoFunction;
}
