/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmStatDlg.cpp : implementation of the CParmStatDlg class.
// CParmStatDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmStatDlg.h"
#include "RunStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmStatDlg dialog


CParmStatDlg::CParmStatDlg(CRunStat* pRun)
   : CParamDlg(CParmStatDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmStatDlg)
   m_Hint = 0;
   m_FuncIndex = 0;
   //}}AFX_DATA_INIT
}


void CParmStatDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmStatDlg)
   DDX_Control(pDX, IDC_VAL_STATIC, m_valueStatic);
   DDX_Control(pDX, IDC_VAL2_STATIC, m_value2Static);
   DDX_Control(pDX, IDC_HINT_STATIC, m_HintStatic);
   DDX_Radio(pDX, IDC_HINT_0, m_Hint);
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIndex);
   //}}AFX_DATA_MAP

   DDX_Text(pDX, IDC_VAL_0, m_valueStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_valueStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_valueStr[2]);
   DDX_Text(pDX, IDC_VAL_3, m_valueStr[3]);
   DDX_Control(pDX, IDC_VAL_0, m_valueEdit[0]);
   DDX_Control(pDX, IDC_VAL_1, m_valueEdit[1]);
   DDX_Control(pDX, IDC_VAL_2, m_valueEdit[2]);
   DDX_Control(pDX, IDC_VAL_3, m_valueEdit[3]);

   DDX_Text(pDX, IDC_VAL2_0, m_value2Str[0]);
   DDX_Text(pDX, IDC_VAL2_1, m_value2Str[1]);
   DDX_Text(pDX, IDC_VAL2_2, m_value2Str[2]);
   DDX_Text(pDX, IDC_VAL2_3, m_value2Str[3]);
   DDX_Control(pDX, IDC_VAL2_0, m_value2Edit[0]);
   DDX_Control(pDX, IDC_VAL2_1, m_value2Edit[1]);
   DDX_Control(pDX, IDC_VAL2_2, m_value2Edit[2]);
   DDX_Control(pDX, IDC_VAL2_3, m_value2Edit[3]);

   DDX_Control(pDX, IDC_HINT_0, m_HintButton[0]);
   DDX_Control(pDX, IDC_HINT_1, m_HintButton[1]);
   DDX_Control(pDX, IDC_HINT_2, m_HintButton[2]);

   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
}


BEGIN_MESSAGE_MAP(CParmStatDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmStatDlg)
   ON_BN_CLICKED(IDC_HINT_0, OnHint)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_HINT_1, OnHint)
   ON_BN_CLICKED(IDC_HINT_2, OnHint)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_2, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_3, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_4, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_5, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_6, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_7, OnFunc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmStatDlg message handlers

BOOL CParmStatDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   SetWindowText("Statistic Functions");
   InitFunc();
   SetFunc();
   ShowValues();
   EnableHint();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   return TRUE;
}

void CParmStatDlg::OnOK()
{
   CParamDlg::OnOK();
}

#define FUNC_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return name; \
}

int CParmStatDlg::FuncIndex(CString funcName)
{
   FUNC_INDEX(MinMax);
   FUNC_INDEX(Max);
   FUNC_INDEX(Min);
   FUNC_INDEX(Sum);
   FUNC_INDEX(Mean);
   return -1;
}

#define FUNC_BASE(name) { \
   if (idx == name) \
      return #name; \
}

CString CParmStatDlg::FuncBase(int idx)
{
   FUNC_BASE(MinMax);
   FUNC_BASE(Max);
   FUNC_BASE(Min);
   FUNC_BASE(Sum);
   FUNC_BASE(Mean);
   return "";
}

void CParmStatDlg::InitFunc()
{
   m_FuncIndex = FuncIndex(m_Func);
}

void CParmStatDlg::ShowValues()
{
   int nChannels = m_Func.SrcChannels() - m_Func.SrcAlpha();
   int flagValue2 = SW_HIDE;
   if (m_Func.BaseName() == "MinMax") {
      flagValue2 = SW_SHOWNA;
      m_valueStatic.SetWindowText("Min");
      m_value2Static.SetWindowText("Max");
   } else {
      m_valueStatic.SetWindowText(m_Func.BaseName());
   }
   m_value2Static.ShowWindow(flagValue2);
   for (int i=0; i<nChannels; i++) {
      m_valueEdit[i].ShowWindow(SW_SHOWNA);
      m_value2Edit[i].ShowWindow(flagValue2);
   }
}

void CParmStatDlg::EnableHint()
{
   BOOL flag =
      (m_Func.BaseName() == "Sum" ||
       m_Func.BaseName() == "Mean") &&
       m_Func.Find("32f") != -1;
   m_HintStatic.EnableWindow(flag);
   for (int i=0; i<3; i++)
      m_HintButton[i].EnableWindow(flag);
}

void CParmStatDlg::SetFunc()
{
   m_Func = m_Func.FuncWithBase(FuncBase(m_FuncIndex));
}

void CParmStatDlg::OnHint()
{
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmStatDlg::OnFunc()
{
   UpdateData();
   SetFunc();
   ShowValues();
   EnableHint();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

