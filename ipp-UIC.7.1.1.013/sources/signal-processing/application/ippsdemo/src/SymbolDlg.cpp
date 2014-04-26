
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// SymbolDlg.cpp : implementation of CSymbolDlg dialog
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "SymbolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog


IMPLEMENT_DYNAMIC(CSymbolDlg, CDialog)

CSymbolDlg::CSymbolDlg(UINT nID/* = IDD*/, CWnd* pParent/* = NULL*/)
   : CDialog(nID, pParent)
{
   //{{AFX_DATA_INIT(CSymbolDlg)
   m_CharStr = _T("");
   m_CodeStr = _T("");
   //}}AFX_DATA_INIT
   m_Code = 0;
   m_bInit = FALSE;
}


void CSymbolDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CSymbolDlg)
   DDX_Control(pDX, IDC_TABLE, m_SymbolWnd);
   DDX_Text(pDX, IDC_CHAR, m_CharStr);
   DDX_Text(pDX, IDC_CODE, m_CodeStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolDlg, CDialog)
   //{{AFX_MSG_MAP(CSymbolDlg)
   ON_EN_CHANGE(IDC_CODE, OnChangeCode)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg operations

void CSymbolDlg::SetChar()
{
   m_CharStr = (char)m_Code;
}

void CSymbolDlg::UpdateCode(int code)
{
   UpdateData();
   m_Code = code;
   m_CodeStr = CodeToString(code);
   SetChar();
   UpdateData(FALSE);
}

CString CSymbolDlg::CodeToString(int code)
{
   CString str;
   str.Format(_T("0x%X"),code);
   return str;
}

int CSymbolDlg::StringToCode(CString str)
{
   int code;
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   _stscanf_s(str,_T("%d"),&code);
   if (code == 0)
   _stscanf_s(str,_T("%x"),&code);
#else
   _stscanf(str,_T("%d"),&code);
   if (code == 0)
   _stscanf(str,_T("%x"),&code);
#endif
   return code;
}

CString CSymbolDlg::CodeToChar(int code)
{
   CString str;
   str = (char)code;
   return str;
}

int CSymbolDlg::CharToCode(CString str)
{
   int code;
   if (str.IsEmpty())
      code = 0;
   else
      code = (int)(unsigned char)(str[0]);
   return code;
}


/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg message handlers

BOOL CSymbolDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();
   UpdateCode(m_Code);
   m_SymbolWnd.UpdateCode(m_Code);
   UpdateData(FALSE);
   m_bInit = TRUE;
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolDlg::OnOK() 
{  
   CDialog::OnOK();
   m_Code = StringToCode(m_CodeStr);
}

void CSymbolDlg::OnChangeCode() 
{
   if (!m_bInit) return;
   UpdateData();
   m_Code = StringToCode(m_CodeStr);
   SetChar();
   m_SymbolWnd.UpdateCode(m_Code);
   UpdateData(FALSE);
}
