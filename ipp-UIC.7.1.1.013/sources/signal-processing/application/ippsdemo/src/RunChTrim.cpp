
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChTrim.cpp : implementation of the CRunChTrimclass.
// CRunChTrimclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunChTrim.h"
#include "ParmChValueDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChTrim::CRunChTrim()
{
   m_odd.Init(pp8u);
   m_odd.Set((double)(int)' ');
   m_bAny = FALSE;
}

CRunChTrim::~CRunChTrim()
{

}

BOOL CRunChTrim::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_bAny = func.Found(_T("Any"));
   if (m_bAny)
      m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   m_odd.Init(func);
   return TRUE;
}

CParamDlg* CRunChTrim::CreateDlg()
{
   return m_bAny ? NULL : new CParmChValueDlg;
}


void CRunChTrim::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChValueDlg *pDlg = (CParmChValueDlg*)parmDlg;
   if (save) {
      m_odd.Set(pDlg->GetCode());
   } else {
      pDlg->SetCode(m_odd.GetInt());
      pDlg->m_Name = _T("Odd");
   }
}

int CRunChTrim::GetDstLength()
{
   return m_pDocSrc->GetVector()->Length();
}

CMyString CRunChTrim::GetVectorName(int vecPos)
{
   if (vecPos == VEC_SRC2)
      return _T("Trim");
   else
      return CippsRun::GetVectorName(vecPos);
}

BOOL CRunChTrim::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
   if (vecPos == VEC_SRC2) {
      pickID = IDC_CURSOR_PTR;
      grabID = IDC_CURSOR_TR;
      return TRUE;
   } 
   return CippsRun::GetPickID(vecPos, pickID, grabID);
}

BOOL CRunChTrim::BeforeCall()
{
   m_len = m_lenSrc;
   return TRUE;
}

BOOL CRunChTrim::AfterCall(BOOL bOk)
{
   if (!bOk) return FALSE;
   CDemoDoc* pDoc = m_pDocDst;
   if (m_Func.Inplace()) 
      pDoc = m_pDocSrc2 ? m_pDocSrc2 : m_pDocSrc;
   pDoc->GetVector()->SetLength(m_len);
   return TRUE;
}

IppStatus CRunChTrim::CallIppFunction()
{
   FUNC_CH_CALL(ippsTrimC_8u_I,  ((Ipp8u*)pSrc, &m_len, (Ipp8u)m_odd ))
   FUNC_CH_CALL(ippsTrimC_16u_I, ((Ipp16u*)pSrc, &m_len, (Ipp16u)m_odd ))
   FUNC_CH_CALL(ippsTrimC_8u,  ((Ipp8u*)pSrc, m_lenSrc, (Ipp8u)m_odd, (Ipp8u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimC_16u, ((Ipp16u*)pSrc, m_lenSrc, (Ipp16u)m_odd, (Ipp16u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimCAny_8u, ( (Ipp8u*)pSrc, m_lenSrc,
        (Ipp8u*)pSrc2, m_lenSrc2, (Ipp8u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimCAny_16u, ( (Ipp16u*)pSrc, m_lenSrc,
        (Ipp16u*)pSrc2, m_lenSrc2, (Ipp16u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimEndCAny_8u, ( (Ipp8u*)pSrc, m_lenSrc,
        (Ipp8u*)pSrc2, m_lenSrc2, (Ipp8u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimEndCAny_16u, ( (Ipp16u*)pSrc, m_lenSrc,
        (Ipp16u*)pSrc2, m_lenSrc2, (Ipp16u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimStartCAny_8u, ( (Ipp8u*)pSrc, m_lenSrc,
        (Ipp8u*)pSrc2, m_lenSrc2, (Ipp8u*)pDst, &m_len ))
   FUNC_CH_CALL(ippsTrimStartCAny_16u, ( (Ipp16u*)pSrc, m_lenSrc,
        (Ipp16u*)pSrc2, m_lenSrc2, (Ipp16u*)pDst, &m_len ))

   return stsNoFunction;
}
 
CString CRunChTrim::GetHistoryParms()
{
   CMyString parms;
   if (!m_bAny)
      parms << (char)m_odd.GetInt();
   return parms;
}
