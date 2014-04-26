/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmStatIdxDlg.cpp : implementation of the CParmStatIdxDlg class.
// CParmStatIdxDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmStatIdxDlg.h"
#include "RunStatIdx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmStatIdxDlg dialog



CParmStatIdxDlg::CParmStatIdxDlg(CRunStatIdx* pRun)
   : CParamDlg(CParmStatIdxDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmStatIdxDlg)
   m_FuncIndex = 0;
   //}}AFX_DATA_INIT
}


void CParmStatIdxDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmStatIdxDlg)
   DDX_Control(pDX, IDC_VAL_STATIC, m_valueStatic);
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIndex);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_X_0, m_xStr[0]);
   DDX_Text(pDX, IDC_X_1, m_xStr[1]);
   DDX_Text(pDX, IDC_X_2, m_xStr[2]);
   DDX_Text(pDX, IDC_X_3, m_xStr[3]);
   DDX_Text(pDX, IDC_Y_0, m_yStr[0]);
   DDX_Text(pDX, IDC_Y_1, m_yStr[1]);
   DDX_Text(pDX, IDC_Y_2, m_yStr[2]);
   DDX_Text(pDX, IDC_Y_3, m_yStr[3]);
   DDX_Control(pDX, IDC_X_0, m_xEdit[0]);
   DDX_Control(pDX, IDC_X_1, m_xEdit[1]);
   DDX_Control(pDX, IDC_X_2, m_xEdit[2]);
   DDX_Control(pDX, IDC_X_3, m_xEdit[3]);
   DDX_Control(pDX, IDC_Y_0, m_yEdit[0]);
   DDX_Control(pDX, IDC_Y_1, m_yEdit[1]);
   DDX_Control(pDX, IDC_Y_2, m_yEdit[2]);
   DDX_Control(pDX, IDC_Y_3, m_yEdit[3]);

   DDX_Text(pDX, IDC_VAL_0, m_valueStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_valueStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_valueStr[2]);
   DDX_Text(pDX, IDC_VAL_3, m_valueStr[3]);
   DDX_Control(pDX, IDC_VAL_0, m_valueEdit[0]);
   DDX_Control(pDX, IDC_VAL_1, m_valueEdit[1]);
   DDX_Control(pDX, IDC_VAL_2, m_valueEdit[2]);
   DDX_Control(pDX, IDC_VAL_3, m_valueEdit[3]);

   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
}


BEGIN_MESSAGE_MAP(CParmStatIdxDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmStatIdxDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
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
// CParmStatIdxDlg message handlers

BOOL CParmStatIdxDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   SetWindowText("Min / Max Index");
   InitFunc();
   SetFunc();
   ShowValues();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   return TRUE;
}

void CParmStatIdxDlg::OnOK()
{
   CParamDlg::OnOK();
}

#define FUNC_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return name; \
}

int CParmStatIdxDlg::FuncIndex(CString funcName)
{
   FUNC_INDEX(MaxIndx);
   FUNC_INDEX(MinIndx);
   return -1;
}

#define FUNC_BASE(name) { \
   if (idx == name) \
      return #name; \
}

CString CParmStatIdxDlg::FuncBase(int idx)
{
   FUNC_BASE(MaxIndx);
   FUNC_BASE(MinIndx);
   return "";
}

void CParmStatIdxDlg::InitFunc()
{
   m_FuncIndex = FuncIndex(m_Func);
}

void CParmStatIdxDlg::SetFunc()
{
   m_Func = m_Func.FuncWithBase(FuncBase(m_FuncIndex));
}

void CParmStatIdxDlg::OnFunc()
{
   UpdateData();
   SetFunc();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmStatIdxDlg::ShowValues()
{
   int i = m_Func.SrcChannels() - m_Func.SrcAlpha();
   int flagValue2 = SW_HIDE;
   for (; i < 4; i++)
   {
      m_valueEdit[i].ShowWindow(SW_HIDE);
      m_xEdit[i].ShowWindow(SW_HIDE);
      m_yEdit[i].ShowWindow(SW_HIDE);
   }
}

