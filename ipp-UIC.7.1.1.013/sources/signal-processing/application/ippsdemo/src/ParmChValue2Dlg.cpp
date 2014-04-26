
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChValue2 dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

// ParmValue2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChValue2Dlg.h"
#include "SymbolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChValue2Dlg dialog

IMPLEMENT_DYNAMIC(CParmChValue2Dlg, CParamDlg)
CParmChValue2Dlg::CParmChValue2Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmChValue2Dlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmChValue2Dlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   m_CharStr[0] = _T("");
   m_CodeStr[0] = _T("");
   m_Name[0] = _T("Value");
   m_CharStr[1] = _T("");
   m_CodeStr[1] = _T("");
   m_Name[1] = _T("Value");
}


void CParmChValue2Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChValue2Dlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_NAME_0, m_NameButton[0]);
   DDX_Text(pDX, IDC_CHAR_0, m_CharStr[0]);
   DDX_Text(pDX, IDC_CODE_0, m_CodeStr[0]);
   DDX_Control(pDX, IDC_NAME_1, m_NameButton[1]);
   DDX_Text(pDX, IDC_CHAR_1, m_CharStr[1]);
   DDX_Text(pDX, IDC_CODE_1, m_CodeStr[1]);
}


BEGIN_MESSAGE_MAP(CParmChValue2Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmChValue2Dlg)
   ON_EN_CHANGE(IDC_CHAR_0, OnChangeChar0)
   ON_EN_CHANGE(IDC_CODE_0, OnChangeCode0)
   ON_BN_CLICKED(IDC_SYMBOL_0, OnSymbol0)
   ON_EN_CHANGE(IDC_CHAR_1, OnChangeChar1)
   ON_EN_CHANGE(IDC_CODE_1, OnChangeCode1)
   ON_BN_CLICKED(IDC_SYMBOL_1, OnSymbol1)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CParmChValue2Dlg::GetCode(int idx) 
{
   return CSymbolDlg::StringToCode(m_CodeStr[idx]);
}

void CParmChValue2Dlg::SetCode(int code, int idx) 
{
   SetCodeStr(code, idx);
   SetCharStr(code, idx);
}

void CParmChValue2Dlg::SetNames(CString name0, CString name1) 
{
   m_Name[0] = name0;
   m_Name[1] = name1;
}

void CParmChValue2Dlg::SetCodeStr(int code, int idx) 
{
   m_CodeStr[idx] = CSymbolDlg::CodeToString(code);
}

void CParmChValue2Dlg::SetCharStr(int code, int idx) 
{
   m_CharStr[idx] = CSymbolDlg::CodeToChar(code);
}

void CParmChValue2Dlg::OnChangeChar(int idx) 
{
   if (!m_bInit) return;
   UpdateData();
   SetCodeStr(CSymbolDlg::CharToCode(m_CharStr[idx]), idx);
   UpdateData(FALSE);
}

void CParmChValue2Dlg::OnChangeCode(int idx) 
{
   if (!m_bInit) return;
   UpdateData();
   m_CharStr[idx] = CSymbolDlg::CodeToChar(GetCode(idx));
   UpdateData(FALSE);
}

void CParmChValue2Dlg::OnSymbol(int idx) 
{
   UpdateData();
   CSymbolDlg dlg;
   dlg.m_Code = CSymbolDlg::StringToCode(m_CodeStr[idx]);
   if (dlg.DoModal() != IDOK) return;
   SetCodeStr(dlg.m_Code, idx);
   SetCharStr(dlg.m_Code, idx);
   UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CParmChValue2Dlg message handlers

BOOL CParmChValue2Dlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   for (int i=0; i<2; i++)
      m_NameButton[i].SetWindowText(m_Name[i]);
   
   return TRUE;
}

void CParmChValue2Dlg::OnChangeChar0() {   OnChangeChar(0);}
void CParmChValue2Dlg::OnChangeCode0() {   OnChangeCode(0);}
void CParmChValue2Dlg::OnSymbol0() {   OnSymbol(0);}
void CParmChValue2Dlg::OnChangeChar1() {   OnChangeChar(1);}
void CParmChValue2Dlg::OnChangeCode1() {   OnChangeCode(1);}
void CParmChValue2Dlg::OnSymbol1() {   OnSymbol(1);}
