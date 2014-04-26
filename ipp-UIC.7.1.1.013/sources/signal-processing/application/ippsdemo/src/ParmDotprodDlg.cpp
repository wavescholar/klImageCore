/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDotprodDlg.cpp : implementation of the CParmDotprodDlg class.
// CParmDotprodDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmDotprodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RunDotprod.h"

/////////////////////////////////////////////////////////////////////////////
// CParmDotprodDlg dialog


IMPLEMENT_DYNAMIC(CParmDotprodDlg, CParamDlg)
CParmDotprodDlg::CParmDotprodDlg(CRunDotprod* pRun)
   : CParamDlg(CParmDotprodDlg::IDD, NULL)
{
   m_UsedScale   = TRUE;
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmDotprodDlg)
   m_DstType = -1;
   m_Im = 0.0;
   m_Re = 0.0;
   //}}AFX_DATA_INIT
}


void CParmDotprodDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmDotprodDlg)
   DDX_Control(pDX, IDC_VALUE_IM, m_ImEdit);
   DDX_Control(pDX, IDC_STATIC_IM, m_ImStatic);
   DDX_Control(pDX, IDC_TYPE_STATIC, m_TypeStatic);
   DDX_Radio(pDX, IDC_TYPE_0, m_DstType);
   DDX_Text(pDX, IDC_VALUE_IM, m_Im);
   DDX_Text(pDX, IDC_VALUE_RE, m_Re);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_TYPE_0, m_TypeButton[0]);
   DDX_Control(pDX, IDC_TYPE_1, m_TypeButton[1]);
   DDX_Control(pDX, IDC_TYPE_2, m_TypeButton[2]);
   DDX_Control(pDX, IDC_TYPE_3, m_TypeButton[3]);
}


BEGIN_MESSAGE_MAP(CParmDotprodDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmDotprodDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_BN_CLICKED(IDC_TYPE_3, OnType)
   ON_EN_CHANGE(IDC_SCALE_EDIT, OnChangeScaleEdit)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmDotprodDlg message handlers

BOOL CParmDotprodDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   if (m_nModalResult == IDCANCEL) return TRUE;
   
   InitType();
   SetFunc();
   ShowType();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmDotprodDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}

void CParmDotprodDlg::OnType() 
{
   UpdateData();
   SetFunc();
   EnableScale();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmDotprodDlg::OnChangeScaleEdit() 
{
   if (!m_bInit) return;
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

int CParmDotprodDlg::TypeIndex(CFunc func)
{
   if (m_pDocSrc->GetVector()->Type() & 16) {
      if (func.Found("32s")) return _16s32s;
      if (func.Found("64s")) return _16s64s;
      if (func.Found("32f")) return _16s32f;
      return _16s;
   } else if ((m_pDocSrc->GetVector()->Depth() == 32) && m_pDocSrc->GetVector()->Float()) {
      if (func.Found("64f")) return _32f64f;
      return _32f;
   } else {
      return 0;
   }
}

CString CParmDotprodDlg::TypeName(int dstType)
{
   CString name = m_pDocSrc->GetVector()->TypeString();
   if (m_pDocSrc->GetVector()->Type() != m_pDocSrc2->GetVector()->Type())
      name += m_pDocSrc2->GetVector()->TypeString();
   CString suffix = (m_pDocSrc2->GetVector()->Type() & PP_CPLX) ? "c" : "";
   if (m_pDocSrc->GetVector()->Type() & 16) {
      switch (dstType) {
      case _16s32s: name += "32s" + suffix; break;
      case _16s64s: name += "64s" + suffix; break;
      case _16s32f: name += "32f" + suffix; break;
      }
   } else if (m_pDocSrc->GetVector()->Type() & 32) {
      switch (dstType) {
      case _32f64f: name += "64f" + suffix; break;
      }
   }
   return name;
}

void CParmDotprodDlg::InitType() 
{
   m_DstType = TypeIndex(m_Func);
}

void CParmDotprodDlg::SetFunc() 
{
   m_Func = "ippsDotProd_" + TypeName(m_DstType);
   if ((m_pDocSrc->GetVector()->Type() & 16) && 
      ((m_DstType == _16s) || (m_DstType == _16s32s)))
      m_Func += "_Sfs";
   if ((m_pDocSrc->GetVector()->Type() & 32) && m_pDocSrc->GetVector()->Sign())
      m_Func += "_Sfs";
   SetWindowText(m_Func);
}


void CParmDotprodDlg::ShowTypeButton(int i, CString text) 
{
   m_TypeButton[i].ShowWindow(SW_SHOWNA);
   m_TypeButton[i].SetWindowText(text);
}

void CParmDotprodDlg::ShowType() 
{
   BOOL bCplx = (m_pDocSrc2->GetVector()->Type() & PP_CPLX);
   CString cplxStr = bCplx ? "c" : "";
   if (m_pDocSrc->GetVector()->Type() & 16) {
      CString typeName[] = {"16s", "32s", "64s", "32f"};
      for (int i=0; i<3; i++)
         ShowTypeButton(i, typeName[i] + cplxStr);
   } else if ((m_pDocSrc->GetVector()->Type() & 32) && m_pDocSrc->GetVector()->Float()) {
      CString typeName[] = {"32f", "64f"};
      for (int i=0; i<2; i++)
         ShowTypeButton(i, typeName[i] + cplxStr);
   } else {
      ShowTypeButton(0, m_pDocSrc2->GetVector()->TypeString());
   }
   m_ImStatic.EnableWindow(bCplx);
   m_ImEdit.EnableWindow(bCplx);
}
