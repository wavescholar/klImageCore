
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChFindC.cpp : implementation of the CRunChFindCclass.
// CRunChFindCclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunChFindC.h"
#include "ParmChFindCDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChFindC::CRunChFindC()
{
   m_valFind = 'a';
}

CRunChFindC::~CRunChFindC()
{

}

BOOL CRunChFindC::Open(CFunc func)
{
   if (!CRunChFind::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

void CRunChFindC::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChFindCDlg *pDlg = (CParmChFindCDlg*)parmDlg;
   if (save) {
      m_valFind = pDlg->GetCode();
   } else {
      pDlg->SetCode(m_valFind);
      pDlg->m_index = m_index;
   }
}

BOOL CRunChFindC::CallIpp(BOOL bMessage)
{
   CParmChFindCDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunChFindC::SetValues(CParmChFindCDlg* pDlg)
{
   UpdateData(pDlg);
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunChFindC::CallIppFunction()
{
   FUNC_CH_CALL(ippsFindC_16u, ((Ipp16u*)pSrc, len, (Ipp16u)m_valFind, &m_index))
   FUNC_CH_CALL(ippsFindRevC_16u, ((Ipp16u*)pSrc, len, (Ipp16u)m_valFind, &m_index))
   FUNC_CH_CALL(ippsFindC_8u, ((Ipp8u*)pSrc, len, (Ipp8u)m_valFind, &m_index))
   FUNC_CH_CALL(ippsFindRevC_8u, ((Ipp8u*)pSrc, len, (Ipp8u)m_valFind, &m_index))

   return stsNoFunction;
}
