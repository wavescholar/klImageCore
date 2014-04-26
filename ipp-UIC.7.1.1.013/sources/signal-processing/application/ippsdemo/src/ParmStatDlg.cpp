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
// CParmStatDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmStatDlg.h"
#include "RunStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmStatDlg dialog


IMPLEMENT_DYNAMIC(CParmStatDlg, CParamDlg)
CParmStatDlg::CParmStatDlg(CRunStat* pRun)
   : CParamDlg(CParmStatDlg::IDD, NULL)
{
   m_UsedScale   = TRUE;
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmStatDlg)
   m_FuncIndex = 0;
   m_DstType = -1;
   m_Im = 0.0;
   m_Re = 0.0;
   m_hint = -1;
   //}}AFX_DATA_INIT
}


void CParmStatDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmStatDlg)
   DDX_Control(pDX, IDC_HINT_STATIC, m_HintStatic);
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIndex);
   DDX_Control(pDX, IDC_TYPE_STATIC, m_TypeStatic);
   DDX_Radio(pDX, IDC_TYPE_0, m_DstType);
   DDX_Text(pDX, IDC_VALUE_IM, m_Im);
   DDX_Control(pDX, IDC_VALUE_IM, m_ImEdit);
   DDX_Control(pDX, IDC_STATIC_IM, m_ImStatic);
   DDX_Text(pDX, IDC_VALUE_RE, m_Re);
   DDX_Control(pDX, IDC_VALUE_RE, m_ReEdit);
   DDX_Control(pDX, IDC_STATIC_RE, m_ReStatic);
   DDX_Radio(pDX, IDC_HINT_0, m_hint);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_TYPE_0, m_TypeButton[0]);
   DDX_Control(pDX, IDC_TYPE_1, m_TypeButton[1]);

   DDX_Control(pDX, IDC_HINT_0, m_HintButton[0]);
   DDX_Control(pDX, IDC_HINT_1, m_HintButton[1]);
   DDX_Control(pDX, IDC_HINT_2, m_HintButton[2]);

   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
   DDX_Control(pDX, IDC_FUNC_2, m_FuncButton[2]);
}


BEGIN_MESSAGE_MAP(CParmStatDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmStatDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_2, OnFunc)
   ON_BN_CLICKED(IDC_TYPE_0, OnFunc)
   ON_BN_CLICKED(IDC_TYPE_1, OnFunc)
   ON_EN_CHANGE(IDC_SCALE_EDIT, OnChangeScaleEdit)
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
   ShowType();
   ShowHint();
   EnableFunc();
   EnableType();
   EnableHint();
   EnableValues();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   return TRUE;
}

void CParmStatDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}

void CParmStatDlg::OnFunc() 
{
   UpdateData();
   SetFunc();
   EnableType();
   EnableHint();
   EnableScale();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmStatDlg::OnChangeScaleEdit() 
{
   if (!m_bInit) return;
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

#define FUNC_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return name; \
}

int CParmStatDlg::FuncIndex(CMyString funcName)
{
   FUNC_INDEX(Sum);
   FUNC_INDEX(StdDev);
   FUNC_INDEX(Mean);
   return -1;
}

#define FUNC_BASE(name) { \
   if (idx == name) \
      return #name; \
}

CMyString CParmStatDlg::FuncBase(int idx)
{
   FUNC_BASE(StdDev);
   FUNC_BASE(Sum);
   FUNC_BASE(Mean);
   return "";
}

int CParmStatDlg::TypeIndex(CMyString funcName)
{
   FUNC_INDEX(_16sc32sc);
   FUNC_INDEX(_16sc);
   FUNC_INDEX(_16s32s);
   FUNC_INDEX(_16s);
   return -1;
}

CMyString CParmStatDlg::TypeName(int dstType)
{
   CMyString name = m_pDocSrc->GetVector()->TypeString();
   ppType srcType  = m_pDocSrc->GetVector()->Type();
   CMyString suffix = (srcType & PP_CPLX) ? "c" : "";
   switch (dstType) {
   case _16s32s: name += "32s" + suffix; break;
   }
   return name;
}

void CParmStatDlg::InitFunc() 
{
   m_FuncIndex = FuncIndex(m_Func);
   m_DstType = TypeIndex(m_Func);
}

CFunc CParmStatDlg::GetFunc(int index, int dstType) 
{
   CMyString funcBase = FuncBase(index);
   CMyString typeName;
   if (TypeFlag(funcBase))
      typeName = TypeName(dstType);
   CFunc func = "ipps" + funcBase + "_" + typeName;
   if (typeName.IsEmpty())
      func += m_pDocSrc->GetVector()->TypeString();
   if (func.DstType() & PP_SIGN)
      func += "_Sfs";
   return func;
}

void CParmStatDlg::SetFunc() 
{
   m_Func = GetFunc(m_FuncIndex, m_DstType);
}

void CParmStatDlg::EnableFunc() 
{
   for (int i=0; i<FuncNum; i++) {
      CFunc func = GetFunc(i, -1);
      BOOL flag = DEMO_APP->GetIppList()->Find(func) ? TRUE : FALSE;
      m_FuncButton[i].EnableWindow(flag);
   }
}

void CParmStatDlg::EnableValues() 
{
   BOOL bCplx = m_Func.DstType() & PP_CPLX;
   m_ImStatic.ShowWindow(bCplx ? SW_SHOWNA : SW_HIDE);
   m_ImEdit.ShowWindow(bCplx ? SW_SHOWNA : SW_HIDE);
   m_ReStatic.SetWindowText(bCplx ? "Re" : "");
}

BOOL CParmStatDlg::TypeFlag(CMyString funcBase)
{
   return funcBase == "Sum" || funcBase == "StdDev";
}

void CParmStatDlg::ShowType() 
{
   ppType type = m_pDocSrc->GetVector()->Type();
   CMyString cplxStr = (type & PP_CPLX) ? "c" : "";
   if (type & 16) {
      CMyString typeName[] = {"16s", "32s"};
      for (int i=0; i<2; i++)
         ShowTypeButton(i, typeName[i] + cplxStr);
      m_TypeStatic.ShowWindow(SW_SHOWNA);
   } 
}

void CParmStatDlg::EnableType() 
{
   BOOL flag = TypeFlag(m_Func.BaseName());
   m_TypeStatic.EnableWindow(flag);
   for (int i=0; i<TypeNum; i++)
      m_TypeButton[i].EnableWindow(flag);
}

void CParmStatDlg::ShowHint() 
{
   ppType type = m_pDocSrc->GetVector()->Type();
   if (type == pp32f || type == pp32fc) {
      for (int i=0; i<3; i++)
         m_HintButton[i].ShowWindow(SW_SHOWNA);
      m_HintStatic.ShowWindow(SW_SHOWNA);
   }
}

void CParmStatDlg::EnableHint() 
{
   BOOL flag = !m_Func.Found("Min") && !m_Func.Found("Max");
   m_HintStatic.EnableWindow(flag);
   for (int i=0; i<3; i++)
      m_HintButton[i].EnableWindow(flag);
}

void CParmStatDlg::ShowTypeButton(int i, CMyString text) 
{
   m_TypeButton[i].ShowWindow(SW_SHOWNA);
   m_TypeButton[i].SetWindowText(text);
}
