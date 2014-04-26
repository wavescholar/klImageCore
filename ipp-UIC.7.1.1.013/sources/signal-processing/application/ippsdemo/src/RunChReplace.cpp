
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChReplace.cpp : implementation of the CRunChReplaceclass.
// CRunChReplaceclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunChReplace.h"
#include "ParmChValue2Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChReplace::CRunChReplace()
{
   m_oldVal.Init(pp8u, 'a');
   m_newVal.Init(pp8u, 'b');
}

CRunChReplace::~CRunChReplace()
{

}

BOOL CRunChReplace::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_oldVal.Init(func);
   m_newVal.Init(func);
   return TRUE;
}

CParamDlg* CRunChReplace::CreateDlg()
{
   return new CParmChValue2Dlg;
}


void CRunChReplace::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChValue2Dlg *pDlg = (CParmChValue2Dlg*)parmDlg;
   if (save) {
      m_oldVal.Set(pDlg->GetCode(0));
      m_newVal.Set(pDlg->GetCode(1));
   } else {
      pDlg->SetCode(m_oldVal.GetInt(), 0);
      pDlg->SetCode(m_newVal.GetInt(), 1);
      pDlg->SetNames(_T("OldVal"), _T("NewVal"));
   }
}

IppStatus CRunChReplace::CallIppFunction()
{
   FUNC_CH_CALL(ippsReplaceC_8u, ((Ipp8u*)pSrc, (Ipp8u*)pDst, len,
        (Ipp8u)m_oldVal, (Ipp8u)m_newVal ))
   FUNC_CH_CALL(ippsReplaceC_16u, ((Ipp16u*)pSrc, (Ipp16u*)pDst, len,
        (Ipp16u)m_oldVal, (Ipp16u)m_newVal ))

   return stsNoFunction;
}

CString CRunChReplace::GetHistoryParms()
{
   CMyString parms;
   return parms << (char)m_oldVal.GetInt() << _T(", ") << (char)m_newVal.GetInt();
}
