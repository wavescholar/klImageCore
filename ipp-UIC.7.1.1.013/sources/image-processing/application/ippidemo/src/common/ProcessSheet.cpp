/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ProcessSheet.h : interface for the Process dialog sheet.
// Command: Menu-Process-Process by
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "ProcessSheet.h"
#include "ProcessPage.h"
#include "CProcess.h"
#include "Director.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessSheet

IMPLEMENT_DYNAMIC(CProcessSheet, CPropertySheet)

CProcessSheet::CProcessSheet(LPCTSTR caption)   : CPropertySheet(caption),
m_pProcess(NULL), m_startPage(-1) {}

CProcessSheet::~CProcessSheet() { DeletePages();}

void CProcessSheet::CreatePages(CProcess* pProcess, CVector* pVector)
{
   DeletePages();
   m_pProcess = pProcess;
   for (int i=0; i<pProcess->GetSize(); i++) {
      CProcessPage* pPage = new CProcessPage(
         DEMO_APP->GetLibTitle(i), pProcess->GetIdFilter(i));
      CSubProcess& subProcess = pProcess->ElementAt(i);
      pPage->m_pVector    = pVector;
      pPage->m_IdxBook    = subProcess.GetCurrentBook();
      pPage->m_IdxChapter = subProcess.GetCurrentChapter();
      pPage->m_Process = subProcess;
      pPage->m_Process.SetParentProcess(&subProcess);
#ifdef FILTER_PAGE
      pPage->m_Filter = CProcess::GetFilter(i);
#endif
      AddPage(pPage);
   }
   m_startPage = pProcess->GetCurrentProcess(); 
}

void CProcessSheet::DeletePages()
{
   while (GetPageCount()) {
      delete GetPage(0);
      RemovePage(0);
   }
}


BEGIN_MESSAGE_MAP(CProcessSheet, CPropertySheet)
   //{{AFX_MSG_MAP(CProcessSheet)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessSheet message handlers

BOOL CProcessSheet::OnInitDialog() 
{
   BOOL bResult = CPropertySheet::OnInitDialog();
   SetActivePage(m_startPage);

#ifdef FILTER_PAGE
   SetDlgItemText(ID_APPLY_NOW, _T("Filter"));
#endif
   return bResult;
}

BOOL CProcessSheet::StartFilterProblem() 
{
   CProcessPage* pPage = (CProcessPage*)GetPage(m_startPage);
   return pPage->StartFilterProblem();
}

BOOL CProcessSheet::OnFilter() 
{
   CProcessPage* pPage = (CProcessPage*)GetActivePage();
   return pPage->OnFilter();
}

void CProcessSheet::OnOK()
{
   for (int i=0; i<m_pProcess->GetSize(); i++) {
      CProcessPage* pPage = (CProcessPage*)GetPage(i);
      pPage->OnOK();
   }
   int idx = GetActiveIndex();
   CProcessPage* pPage = (CProcessPage*)GetPage(idx);
   m_Func = pPage->m_FuncName;
   m_pProcess->SetCurrentFunction(
      idx, pPage->m_IdxBook, pPage->m_IdxChapter, m_Func);
#ifdef FILTER_PAGE
   CProcess::SetFilter(pPage->m_Filter, idx);
#endif
}

BOOL CProcessSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
   if (LOWORD(wParam) == IDOK)
   {
      OnOK();
   }

   return CPropertySheet::OnCommand(wParam, lParam);
}
