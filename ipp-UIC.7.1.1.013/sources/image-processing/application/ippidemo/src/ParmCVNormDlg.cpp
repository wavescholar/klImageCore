/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVNormDlg.cpp : implementation of the CParmCVNormDlg class.
// CParmCVNormDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVNormDlg.h"
#include "RunCVNorm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVNormDlg dialog


CParmCVNormDlg::CParmCVNormDlg(CRunCVNorm* pRun, UINT nID)
   : CParamDlg(nID, NULL), m_pRun(pRun)
{
   //{{AFX_DATA_INIT(CParmCVNormDlg)
   m_FuncIdx = -1;
   m_ValueStr = _T("");
   m_CoiStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCVNormDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVNormDlg)
   DDX_Control(pDX, IDC_COI_S, m_CoiSpin);
   DDX_Control(pDX, IDC_COI_N, m_CoiStatic);
   DDX_Control(pDX, IDC_COI_E, m_CoiEdit);
   DDX_Radio(pDX, IDC_NORM_0, m_FuncIdx);
   DDX_Text(pDX, IDC_VALUE, m_ValueStr);
   DDX_Text(pDX, IDC_COI_E, m_CoiStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVNormDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCVNormDlg)
   ON_BN_CLICKED(IDC_NORM_0, OnFunc)
   ON_BN_CLICKED(IDC_NORM_1, OnFunc)
   ON_BN_CLICKED(IDC_NORM_2, OnFunc)
   ON_EN_CHANGE(IDC_COI_E, OnChangeCoi)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVNormDlg message handlers

BOOL CParmCVNormDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_CoiSpin.SetRange(1,3);

   EnableCoi();
   m_pRun->SetValue(this);
   m_FuncIdx = GetFuncIndex();
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmCVNormDlg::OnChangeCoi()
{
   if (m_bInit) {
      UpdateData();
      m_pRun->SetValue(this);
      UpdateData(FALSE);
   }
}

void CParmCVNormDlg::OnFunc()
{
   UpdateData();
   m_Func = GetFunc(m_FuncIdx);
   SetWindowText(m_Func);
   m_pRun->SetValue(this);
   UpdateData(FALSE);
}

void CParmCVNormDlg::EnableCoi()
{
   BOOL flag = m_Func.SrcChannels() > 1;
   m_CoiStatic.EnableWindow(flag);
   m_CoiEdit.EnableWindow(flag);
   m_CoiSpin.EnableWindow(flag);
}

CFunc CParmCVNormDlg::GetFunc(int idx)
{
   CString prefix = "ippiNorm_";
   CString suffix = "_" + m_Func.TypeName() + "_" + m_Func.DescrName();
   switch (idx) {
   case 0: return prefix + "Inf" + suffix;
   case 1: return prefix + "L1" + suffix;
   case 2: return prefix + "L2" + suffix;
   }
   return "";
}

int CParmCVNormDlg::GetFuncIndex()
{
   for (int i=0; i<GetFuncNumber(); i++) {
      CFunc func = GetFunc(i);
      if (func == m_Func) return i;
   }
   return -1;
}
      
