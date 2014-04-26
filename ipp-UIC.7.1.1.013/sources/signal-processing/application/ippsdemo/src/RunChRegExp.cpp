
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChRegExp.cpp : implementation of the CRunChRegExpclass.
// CRunChRegExpclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////
//typedef struct {
// void *pFind;
// int lenFind;
//} IppRegExpFind;

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunChRegExp.h"
#include "ParmChRegExpDlg.h"

CRunChRegExp::CRunChRegExp()
{
   m_pPattern;
   m_pOptions,
   m_pState = NULL;
   m_errOffset = -1;
   m_numFindIn = 9;
   m_pFind = NULL;
   m_numFind = 0;
}

CRunChRegExp::~CRunChRegExp()
{

}

BOOL CRunChRegExp::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

void CRunChRegExp::Close()
{
   if (m_pFind)
      delete[] m_pFind;
}

void CRunChRegExp::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChRegExpDlg *pDlg = (CParmChRegExpDlg*)parmDlg;
   if (save) {
      m_pPattern = pDlg->m_pPattern;
      m_pOptions = pDlg->m_pOptions;
      m_numFindIn = pDlg->m_numFindIn;
   } else {
      pDlg->m_pPattern = m_pPattern;
      pDlg->m_pOptions = m_pOptions;
      pDlg->m_numFindIn = m_numFindIn;
      pDlg->m_errOffset = m_errOffset;
      pDlg->m_pFind = m_pFind;
      pDlg->m_numFind = m_numFind;
   }
}

BOOL CRunChRegExp::CallIpp(BOOL bMessage)
{
   CParmChRegExpDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

BOOL CRunChRegExp::SetValues(CParmChRegExpDlg* pDlg)
{
   UpdateData(pDlg);
   BOOL result = FALSE;
   if (Init()) {
      if (CippsRun::CallIpp()) {
         result = TRUE;
         Timing();
         SetHistory();
      }
   }
   Free();
   UpdateData(pDlg,FALSE);
   return result;
}

BOOL CRunChRegExp::Init()
{
   const char* pOptions = m_pOptions.GetLength() ? (const char*)m_pOptions : (const char*)NULL;
   CHECK_CH_CALL(ippsRegExpInitAlloc, ((const char*)m_pPattern, pOptions,
                 &m_pState, &m_errOffset));
   if (m_pFind)
      delete[] m_pFind;
   m_pFind = new IppRegExpFind[m_numFindIn];
   m_numFind = m_numFindIn;
   
   m_errOffset = -1;
   return TRUE;
}

BOOL CRunChRegExp::Free()
{
   CH_CALL(ippsRegExpFree, (m_pState));
   return TRUE;
}

IppStatus CRunChRegExp::CallIppFunction()
{
   FUNC_CH_CALL(ippsRegExpFind_8u, ((const Ipp8u*)pSrc, len, m_pState,
      m_pFind, &m_numFind))

   return stsNoFunction;
}

CString CRunChRegExp::GetHistoryParms()
{
   CMyString parm;
   return parm;
}
