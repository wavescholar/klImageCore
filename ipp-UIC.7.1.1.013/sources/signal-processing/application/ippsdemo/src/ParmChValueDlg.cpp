
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChValue dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChValueDlg.h"
#include "SymbolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChValueDlg dialog

IMPLEMENT_DYNAMIC(CParmChValueDlg, CParamDlg)
CParmChValueDlg::CParmChValueDlg(UINT nID, CWnd* pParent)
   : CParamDlg(nID, pParent)
{
   //{{AFX_DATA_INIT(CParmChValueDlg)
   m_CharStr = _T("");
   m_CodeStr = _T("");
   //}}AFX_DATA_INIT
   m_Name = _T("Value");
}


void CParmChValueDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChValueDlg)
   DDX_Control(pDX, IDC_SYMBOL, m_SymbolButton);
   DDX_Control(pDX, IDC_CODE_STATIC, m_CodeStatic);
   DDX_Control(pDX, IDC_CODE, m_CodeEdit);
   DDX_Control(pDX, IDC_CHAR_STATIC, m_CharStatic);
   DDX_Control(pDX, IDC_CHAR, m_CharEdit);
   DDX_Control(pDX, IDC_NAME, m_NameButton);
   DDX_Text(pDX, IDC_CHAR, m_CharStr);
   DDX_Text(pDX, IDC_CODE, m_CodeStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmChValueDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmChValueDlg)
   ON_EN_CHANGE(IDC_CHAR, OnChangeChar)
   ON_EN_CHANGE(IDC_CODE, OnChangeCode)
   ON_BN_CLICKED(IDC_SYMBOL, OnSymbol)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CParmChValueDlg::GetCode() 
{
   return CSymbolDlg::StringToCode(m_CodeStr);
}

void CParmChValueDlg::SetCode(int code) 
{
   SetCodeStr(code);
   SetCharStr(code);
}

void CParmChValueDlg::SetCodeStr(int code) 
{
   m_CodeStr = CSymbolDlg::CodeToString(code);
}

void CParmChValueDlg::SetCharStr(int code) 
{
   m_CharStr = (char)code;
}

void CParmChValueDlg::EnableValue(BOOL flag)
{
   m_SymbolButton.EnableWindow(flag);
   m_CodeStatic.EnableWindow(flag);
   m_CodeEdit.EnableWindow(flag);
   m_CharStatic.EnableWindow(flag);
   m_CharEdit.EnableWindow(flag);
   m_NameButton.EnableWindow(flag);
}
 
/////////////////////////////////////////////////////////////////////////////
// CParmChValueDlg message handlers

BOOL CParmChValueDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   m_NameButton.SetWindowText(m_Name);
   
   return TRUE;
}

void CParmChValueDlg::OnChangeChar() 
{
   if (!m_bInit) return;
   UpdateData();
   SetCodeStr(CSymbolDlg::CharToCode(m_CharStr));
   UpdateData(FALSE);
}

void CParmChValueDlg::OnChangeCode() 
{
   if (!m_bInit) return;
   UpdateData();
   m_CharStr = CSymbolDlg::CodeToChar(GetCode());
   UpdateData(FALSE);
}

void CParmChValueDlg::OnSymbol() 
{
   UpdateData();
   CSymbolDlg dlg;
   dlg.m_Code = GetCode();
   if (dlg.DoModal() != IDOK) return;
   SetCodeStr(dlg.m_Code);
   SetCharStr(dlg.m_Code);
   UpdateData(FALSE);
}
