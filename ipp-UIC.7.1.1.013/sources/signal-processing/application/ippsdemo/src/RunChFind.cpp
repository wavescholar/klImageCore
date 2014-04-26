
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChFind.cpp : implementation of the CRunChFindclass.
// CRunChFindclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunChFind.h"
#include "ParmChFindDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChFind::CRunChFind()
{
   m_index = 0;
}

CRunChFind::~CRunChFind()
{

}

BOOL CRunChFind::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2;
   return TRUE;
}

CMyString CRunChFind::GetVectorName(int vecPos)
{
   if (vecPos == VEC_SRC2)
      return _T("Find");
   else
      return CippsRun::GetVectorName(vecPos);
}

BOOL CRunChFind::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
   if (vecPos == VEC_SRC2) {
      pickID = IDC_CURSOR_PF;
      grabID = IDC_CURSOR_F;
      return TRUE;
   } 
   return CippsRun::GetPickID(vecPos, pickID, grabID);
}

void CRunChFind::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChFindDlg *pDlg = (CParmChFindDlg*)parmDlg;
   if (save) {
      m_index = pDlg->m_index;
   } else {
      pDlg->m_index = m_index;
   }
}

BOOL CRunChFind::AfterCall(BOOL bOk)
{
   if (!bOk) return FALSE;
   CParmChFindDlg dlg;
   UpdateData(&dlg,FALSE);
   dlg.DoModal();
   return TRUE;
}

IppStatus CRunChFind::CallIppFunction()
{
   FUNC_CH_CALL(ippsFind_8u, ((Ipp8u*)pSrc, m_lenSrc, (Ipp8u*)pSrc2, m_lenSrc2, &m_index))
   FUNC_CH_CALL(ippsFind_16u, ((Ipp16u*)pSrc, m_lenSrc, (Ipp16u*)pSrc2, m_lenSrc2, &m_index))
   FUNC_CH_CALL(ippsFindRev_8u, ((Ipp8u*)pSrc, m_lenSrc, (Ipp8u*)pSrc2, m_lenSrc2, &m_index))
   FUNC_CH_CALL(ippsFindRev_16u, ((Ipp16u*)pSrc, m_lenSrc, (Ipp16u*)pSrc2, m_lenSrc2, &m_index))
   FUNC_CH_CALL(ippsFindCAny_8u, ( (Ipp8u*)pSrc, m_lenSrc, (Ipp8u*)pSrc2, m_lenSrc2, &m_index ))
   FUNC_CH_CALL(ippsFindCAny_16u, ( (Ipp16u*)pSrc, m_lenSrc, (Ipp16u*)pSrc2, m_lenSrc2, &m_index ))
   FUNC_CH_CALL(ippsFindRevCAny_8u, ( (Ipp8u*)pSrc, m_lenSrc, (Ipp8u*)pSrc2, m_lenSrc2, &m_index ))
   FUNC_CH_CALL(ippsFindRevCAny_16u, ( (Ipp16u*)pSrc, m_lenSrc, (Ipp16u*)pSrc2, m_lenSrc2, &m_index ))

   return stsNoFunction;
}
