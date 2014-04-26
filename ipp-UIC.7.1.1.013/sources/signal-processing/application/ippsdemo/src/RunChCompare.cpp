
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChCompare.cpp : implementation of the CRunChCompareclass.
// CRunChCompareclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunChCompare.h"
#include "ParmChCompareDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChCompare::CRunChCompare()
{

}

CRunChCompare::~CRunChCompare()
{

}

BOOL CRunChCompare::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2;
   return TRUE;
}

void CRunChCompare::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChCompareDlg *pDlg = (CParmChCompareDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      m_Comp = pDlg->m_Comp;
   } else {
      pDlg->m_Func = m_Func;
      pDlg->m_Comp = m_Comp;
   }
}

BOOL CRunChCompare::CallIpp(BOOL bMessage)
{
   CParmChCompareDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunChCompare::SetValues(CParmChCompareDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunChCompare::CallIppFunction()
{
   FUNC_CH_CALL(ippsCompare_8u, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len, &m_Comp))
   FUNC_CH_CALL(ippsCompare_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len, &m_Comp))
   FUNC_CH_CALL(ippsEqual_8u, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len, &m_Comp))
   FUNC_CH_CALL(ippsEqual_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len, &m_Comp))
   FUNC_CH_CALL(ippsCompareIgnoreCase_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len, &m_Comp))
   FUNC_CH_CALL(ippsCompareIgnoreCaseLatin_8u, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len, &m_Comp))
   FUNC_CH_CALL(ippsCompareIgnoreCaseLatin_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len, &m_Comp))

   return stsNoFunction;
}
