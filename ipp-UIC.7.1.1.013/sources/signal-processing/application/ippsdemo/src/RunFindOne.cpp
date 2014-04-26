/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFindOne.cpp : implementation of the CRunFindOne class.
// CRunFindOne class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunFindOne.h"
#include "ParmFindOneDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFindOne::CRunFindOne()
{
   m_TableMode = CParmFindOneDlg::TBL_EVEN;
   CreateEvenTable(64);

   m_inVal = 55;
}

CRunFindOne::~CRunFindOne()
{

}

void CRunFindOne::CreateEvenTable(int tblLen)
{
   if (tblLen < 1) {
      tblLen = m_Table.Length();
      if (tblLen < 1) tblLen = 1;
   }
   double tblIncr = USHRT_MAX / tblLen;
   m_Table.Init(pp16u, tblLen);
   double val = 0;
   for (int i=0; i < tblLen; i++) {
      m_Table.Set(i, val);
      val += tblIncr;
   }
}

BOOL CRunFindOne::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = 0;
   return TRUE;
}

void CRunFindOne::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFindOneDlg *pDlg = (CParmFindOneDlg*)parmDlg;
   if (save) {
      m_TableMode = pDlg->m_TableMode;

      m_inVal = atoi(pDlg->m_InValStr);
   } else {
      pDlg->m_TableMode = m_TableMode;
      pDlg->m_pTable = &m_Table;

      pDlg->m_OutValStr.Format("%d", m_outVal);
      pDlg->m_IndexStr.Format("%d", m_outIndex);
   }
}

BOOL CRunFindOne::IsIncreasingTable()
{
   Ipp16u* table = (Ipp16u*)m_Table.GetData();
   int tableLen = m_Table.Length();
   for (int i=1; i < tableLen; i++) {
      if (table[i] < table[i - 1]) 
         return FALSE;
   }
   return TRUE;
}

BOOL CRunFindOne::CallIpp(BOOL bMessage)
{
   if (!IsIncreasingTable()) {
      CreateEvenTable(m_Table.Length());
   }
   CParmFindOneDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return FALSE;
   UpdateData(&dlg);
   return TRUE;
}

BOOL CRunFindOne::SetValues(CParmFindOneDlg* pDlg)
{
   UpdateData(pDlg);
   if (!IsIncreasingTable()) return FALSE;
   CippsRun::CallIpp();
   UpdateData(pDlg,FALSE);
   return TRUE;
}

IppStatus CRunFindOne::CallIppFunction()
{
   FUNC_CALL(ippsFindNearestOne_16u,(
      m_inVal, &m_outVal, &m_outIndex, 
      (Ipp16u*)m_Table, m_Table.Length()))
   return stsNoFunction;
}

CString CRunFindOne::GetHistoryParms()
{
   CMyString parm;
   parm << m_inVal << ", .., " << m_Table.Length();
   return parm;
}
