/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMinDlg.cpp : implementation of the CParmMinDlg class.
// CParmMinDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmMinDlg.h"
#include "RunMin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmMinDlg dialog


IMPLEMENT_DYNAMIC(CParmMinDlg, CParamDlg)
CParmMinDlg::CParmMinDlg(CRunMin* pRun)
   : CParamDlg(CParmMinDlg::IDD, NULL)
{
   m_UsedScale   = FALSE;
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmMinDlg)
   m_FuncIndex = 0;
   m_Index = 0;
   m_Im = 0.0;
   m_Re = 0.0;
   //}}AFX_DATA_INIT
}


void CParmMinDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmMinDlg)
   DDX_Control(pDX, IDC_INDEX_STATIC, m_IndexStatic);
   DDX_Control(pDX, IDC_INDEX, m_IndexEdit);
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIndex);
   DDX_Text(pDX, IDC_INDEX, m_Index);
   DDX_Text(pDX, IDC_VALUE_IM, m_Im);
   DDX_Control(pDX, IDC_VALUE_IM, m_ImEdit);
   DDX_Control(pDX, IDC_STATIC_IM, m_ImStatic);
   DDX_Text(pDX, IDC_VALUE_RE, m_Re);
   DDX_Control(pDX, IDC_VALUE_RE, m_ReEdit);
   DDX_Control(pDX, IDC_STATIC_RE, m_ReStatic);
   //}}AFX_DATA_MAP

   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
   DDX_Control(pDX, IDC_FUNC_2, m_FuncButton[2]);
   DDX_Control(pDX, IDC_FUNC_3, m_FuncButton[3]);
   DDX_Control(pDX, IDC_FUNC_4, m_FuncButton[4]);
   DDX_Control(pDX, IDC_FUNC_5, m_FuncButton[5]);
   DDX_Control(pDX, IDC_FUNC_6, m_FuncButton[6]);
   DDX_Control(pDX, IDC_FUNC_7, m_FuncButton[7]);
   DDX_Control(pDX, IDC_FUNC_8, m_FuncButton[8]);
}


BEGIN_MESSAGE_MAP(CParmMinDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmMinDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_2, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_3, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_4, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_5, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_6, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_7, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_8, OnFunc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmMinDlg message handlers

BOOL CParmMinDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   SetWindowText("Statistic Functions");
   InitFunc();
   SetFunc();
   EnableFunc();
   EnableValues();
   EnableIndex();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   return TRUE;
}

void CParmMinDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}

void CParmMinDlg::OnFunc() 
{
   UpdateData();
   SetFunc();
   EnableIndex();
   EnableScale();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

#define FUNC_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return name; \
}

int CParmMinDlg::FuncIndex(CMyString funcName)
{
   FUNC_INDEX(MaxOrder);
   FUNC_INDEX(MinIndx);
   FUNC_INDEX(MaxIndx);
   FUNC_INDEX(MinAbsIndx);
   FUNC_INDEX(MaxAbsIndx);
   FUNC_INDEX(MinAbs);
   FUNC_INDEX(MaxAbs);
   FUNC_INDEX(Min);
   FUNC_INDEX(Max);
   return -1;
}

#define FUNC_BASE(name) { \
   if (idx == name) \
      return #name; \
}

CMyString CParmMinDlg::FuncBase(int idx)
{
   FUNC_BASE(MaxOrder);
   FUNC_BASE(MinIndx);
   FUNC_BASE(MaxIndx);
   FUNC_BASE(MinAbsIndx);
   FUNC_BASE(MaxAbsIndx);
   FUNC_BASE(MinAbs);
   FUNC_BASE(MaxAbs);
   FUNC_BASE(Min);
   FUNC_BASE(Max);
   return "";
}

void CParmMinDlg::InitFunc() 
{
   m_FuncIndex = FuncIndex(m_Func);
}

CFunc CParmMinDlg::GetFunc(int index) 
{
   CMyString funcBase = FuncBase(index);
   CMyString typeName = m_pDocSrc->GetVector()->TypeString();
   CFunc func = "ipps" + funcBase + "_" + typeName;
   return func;
}

void CParmMinDlg::SetFunc() 
{
   m_Func = GetFunc(m_FuncIndex);
}

void CParmMinDlg::EnableFunc() 
{
   for (int i=0; i<FuncNum; i++) {
      CFunc func = GetFunc(i);
      BOOL flag = DEMO_APP->GetIppList()->Find(func) ? TRUE : FALSE;
      m_FuncButton[i].EnableWindow(flag);
   }
}

void CParmMinDlg::EnableValues() 
{
   BOOL bCplx = m_Func.DstType() & PP_CPLX;
   m_ImStatic.ShowWindow(bCplx ? SW_SHOWNA : SW_HIDE);
   m_ImEdit.ShowWindow(bCplx ? SW_SHOWNA : SW_HIDE);
   m_ReStatic.SetWindowText(bCplx ? "Re" : "");
}

void CParmMinDlg::EnableIndex() 
{
   int flag = (m_Func.Found("Indx")) ? SW_SHOWNA : SW_HIDE;
   m_IndexStatic.ShowWindow(flag);
   m_IndexEdit.ShowWindow(flag);
}

