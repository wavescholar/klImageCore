
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// StringDlg.cpp : implementation of CStringDlg dialog
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "StringDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStringDlg dialog


IMPLEMENT_DYNAMIC(CStringDlg, CSymbolDlg)

CStringDlg::CStringDlg(CWnd* pParent /*=NULL*/)
   : CSymbolDlg(CStringDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CStringDlg)
   m_String = _T("");
   //}}AFX_DATA_INIT
   m_bSetFocus = FALSE;
   m_start = m_end = 0;
}


void CStringDlg::DoDataExchange(CDataExchange* pDX)
{
   CSymbolDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CStringDlg)
   DDX_Control(pDX, IDC_STRING, m_StringEdit);
   DDX_Text(pDX, IDC_STRING, m_String);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStringDlg, CSymbolDlg)
   //{{AFX_MSG_MAP(CStringDlg)
   ON_EN_SETFOCUS(IDC_STRING, OnSetfocusString)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStringDlg operations

void CStringDlg::UpdateString()
{
   UpdateData();
   int start, end;
   m_StringEdit.GetSel(start,end);
   m_String = m_String.Mid(0,start) + (char)m_Code + m_String.Mid(end);
   UpdateData(FALSE);
   m_StringEdit.SetSel(start+1,start+1);
}

/////////////////////////////////////////////////////////////////////////////
// CStringDlg message handlers

BOOL CStringDlg::OnInitDialog() 
{
   CSymbolDlg::OnInitDialog();
   m_StringEdit.SetSel(m_start, m_end);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CStringDlg::OnOK() 
{
   m_StringEdit.GetSel(m_start, m_end);
   
   CSymbolDlg::OnOK();
}

void CStringDlg::OnSetfocusString() 
{
   if (!m_bSetFocus)
      m_StringEdit.SetSel(m_start, m_end);
   m_bSetFocus = TRUE;
}
