/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// FileHistoDlg.cpp : implementation of the History dialog.
// Command: Menu-File-History
//
/////////////////////////////////////////////////////////////////////////////

// FileHistoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "FileHistoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileHistoDlg dialog


CFileHistoDlg::CFileHistoDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CFileHistoDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CFileHistoDlg)
   m_Title = _T("");
   m_HistoStr = _T("");
   //}}AFX_DATA_INIT
   m_pHistoList = NULL;
}


void CFileHistoDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CFileHistoDlg)
   DDX_Control(pDX, IDC_HISTO_EDIT, m_HistoEdit);
   DDX_Control(pDX, IDC_HISTO_TITLE, m_TitleEdit);
   DDX_Text(pDX, IDC_HISTO_TITLE, m_Title);
   DDX_Text(pDX, IDC_HISTO_EDIT, m_HistoStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileHistoDlg, CDialog)
   //{{AFX_MSG_MAP(CFileHistoDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileHistoDlg message handlers

BOOL CFileHistoDlg::OnInitDialog() 
{
   ASSERT(m_pHistoList);
   CDialog::OnInitDialog();
   SetWindowText(m_DocTitle + _T(" History"));

   int tabs[12];
   int i = 0;
   tabs[i++] = 32;
   tabs[i++] = 240;
   tabs[i++] = 256;
   tabs[i++] = 280;
   for (; i<12; i++) tabs[i] = tabs[i-1] + 8;
   m_TitleEdit.SetTabStops(12,tabs);
   m_HistoEdit.SetTabStops(12,tabs);
   m_Title = _T("Signal\tFunction ( Parameters ) - Comment\tPerformance");
   POSITION pos = m_pHistoList->GetHeadPosition();
   while (pos) {
      m_HistoStr += m_pHistoList->GetNext(pos) + _T("\r\n");
   }
   m_HistoEdit.SetSel(-1, 0);
   UpdateData(FALSE);
   return TRUE;  
}
