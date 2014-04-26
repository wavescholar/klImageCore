/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormDlg.cpp : implementation of the CParmNormDlg class.
// CParmNormDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmNormDlg.h"
#include "RunNorm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmNormDlg dialog

IMPLEMENT_DYNAMIC(CParmNormDlg, CParamDlg)
CParmNormDlg::CParmNormDlg(CRunNorm* pRun, UINT nID)
   : CParamDlg(nID, NULL)
{
   m_UsedScale   = TRUE;
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmNormDlg)
   m_FuncIndex = -1;
   m_TypeIndex = -1;
   m_Value = 0.0;
   //}}AFX_DATA_INIT
}


void CParmNormDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmNormDlg)
   DDX_Control(pDX, IDC_TYPE_STATIC, m_TypeStatic);
   DDX_Radio(pDX, IDC_FUNC_0, m_FuncIndex);
   DDX_Radio(pDX, IDC_TYPE_0, m_TypeIndex);
   DDX_Text(pDX, IDC_VALUE, m_Value);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_TYPE_0, m_TypeButton[0]);
   DDX_Control(pDX, IDC_TYPE_1, m_TypeButton[1]);
   DDX_Control(pDX, IDC_TYPE_2, m_TypeButton[2]);
   DDX_Control(pDX, IDC_TYPE_3, m_TypeButton[3]);
   DDX_Control(pDX, IDC_FUNC_0, m_FuncButton[0]);
   DDX_Control(pDX, IDC_FUNC_1, m_FuncButton[1]);
   DDX_Control(pDX, IDC_FUNC_2, m_FuncButton[2]);
   DDX_Control(pDX, IDC_FUNC_3, m_FuncButton[3]);
}


BEGIN_MESSAGE_MAP(CParmNormDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmNormDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_2, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_3, OnFunc)
   ON_BN_CLICKED(IDC_TYPE_0, OnFunc)
   ON_BN_CLICKED(IDC_TYPE_1, OnFunc)
   ON_BN_CLICKED(IDC_TYPE_2, OnFunc)
   ON_BN_CLICKED(IDC_TYPE_3, OnFunc)
   ON_EN_CHANGE(IDC_SCALE_EDIT, OnChangeScaleEdit)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmNormDlg message handlers

BOOL CParmNormDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   m_bDiff = m_UsedVectors & VEC_SRC2;
   
   SetWindowText(m_bDiff ? "NormDiff Functions" : "Norm Functions");
   InitFunc();
   EnableFunc();
   EnableType();
   EnableSrc2();
   SetFunc();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmNormDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}

#define FUNC_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return name; \
}

int CParmNormDlg::FuncIndex(CString funcName)
{
   FUNC_INDEX(_Inf);
   FUNC_INDEX(_L1);
   FUNC_INDEX(_L2Sqr);
   FUNC_INDEX(_L2);
   return -1;
}

#define FUNC_BASE(name) { \
   if (idx == name) \
      return base += #name; \
}

CString CParmNormDlg::FuncBase(int idx)
{
   CString base = m_bDiff ? "NormDiff" : "Norm";
   FUNC_BASE(_Inf);
   FUNC_BASE(_L1);
   FUNC_BASE(_L2Sqr);
   FUNC_BASE(_L2);
   return "";
}

int CParmNormDlg::TypeIndex(CString funcName)
{
   CFunc func = funcName;
   if (func.SrcType() == func.DstType())
      return -1;
   switch (func.DstType()) {
   case pp32f: return _32f;
   case pp32s: return _32s;
   case pp64f: return _64f;
   case pp64s: return _64s;
   }
   return -1;

}

#define TYPE_NAME(name) { \
   if (idx == _##name) \
      return #name; \
}

CString CParmNormDlg::TypeName(int idx)
{
   TYPE_NAME(32f);
   TYPE_NAME(32s);
   TYPE_NAME(64f);
   TYPE_NAME(64s);
   return "";
}

void CParmNormDlg::InitFunc() 
{
   m_FuncIndex = FuncIndex(m_Func);
   m_TypeIndex = TypeIndex(m_Func);
}

CFunc CParmNormDlg::CreateFuncName(int funcIndex, int typeIndex) 
{
   CMyString funcBase = FuncBase(funcIndex);
   CMyString dstTypeName = TypeName(typeIndex);
   CFunc func = "ipps" + funcBase 
      + "_" + m_pDocSrc->GetVector()->TypeString() + dstTypeName;
   if (func.DstType() & PP_SIGN) 
      func += "_Sfs";
   return func;
}

void CParmNormDlg::SetFunc() 
{
   m_Func = CreateFuncName(m_FuncIndex, m_TypeIndex);
}

void CParmNormDlg::OnFunc() 
{
   UpdateData();
   EnableType();
   EnableScale();
   SetFunc();
   m_pRun->SetValues(this);
   EnableSrc2();
   UpdateData(FALSE);
}

void CParmNormDlg::OnChangeScaleEdit() 
{
   if (!m_bInit) return;
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmNormDlg::EnableFunc() 
{
   if (m_pDocSrc->GetVector()->Type() != pp16s)
      m_FuncButton[_L2Sqr].EnableWindow(FALSE);
}

void CParmNormDlg::EnableType() 
{
   BOOL validType = m_pRun->IsFunction(CreateFuncName(m_FuncIndex, m_TypeIndex));
   for (int i=0; i<TypeNum; i++) {
      BOOL flag = m_pRun->IsFunction(CreateFuncName(m_FuncIndex, i));
      m_TypeButton[i].EnableWindow(flag);
      if (flag && !validType) {
         validType = TRUE;
         m_TypeIndex = i;
      }
   }
}

void CParmNormDlg::EnableSrc2() 
{
   GetDlgItem(IDC_EDIT_SRC2)->EnableWindow(m_bDiff);
   GetDlgItem(IDC_STATIC_SRC2)->EnableWindow(m_bDiff);
}
