/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFind.cpp : implementation of the CRunFind class.
// CRunFind class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunFind.h"
#include "ParmFindDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFind::CRunFind()
{
   m_pIndex = 0;
}

CRunFind::~CRunFind()
{

}

BOOL CRunFind::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   return TRUE;
}

BOOL CRunFind::BeforeCall()
{
   m_pIndex = (int*)malloc(m_lenSrc*sizeof(int));
   return TRUE;
}

BOOL CRunFind::AfterCall(BOOL bOk)
{
   if (m_pIndex) free(m_pIndex);
   return TRUE;
}

void CRunFind::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CRunFindOne::UpdateData((CParmFindOneDlg*)parmDlg,save);
   CParmFindDlg *pDlg = (CParmFindDlg*)parmDlg;
   if (save) {
   } else {
      pDlg->m_pSrc = (Ipp16u*)pSrc;
      pDlg->m_pDst = (Ipp16u*)pDst;
      pDlg->m_pIndex = m_pIndex;
   }
}

BOOL CRunFind::CallIpp(BOOL bMessage)
{
   if (!IsIncreasingTable()) {
      CreateEvenTable(m_Table.Length());
   }
   CParmFindDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return FALSE;
   UpdateData(&dlg);
   return TRUE;
}

IppStatus CRunFind::CallIppFunction()
{
   FUNC_CALL(ippsFindNearest_16u,(
      (Ipp16u*)pSrc, (Ipp16u*)pDst, m_pIndex, len, (Ipp16u*)m_Table, m_Table.Length()))
   return stsNoFunction;
}

CString CRunFind::GetHistoryParms()
{
   CMyString parm;
   parm << m_Table.Length();
   return parm;
}
