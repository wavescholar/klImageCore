/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIirDlg.cpp : implementation of the CParmIirDlg class.
// CParmIirDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmIirDlg.h"
#include "ippsDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmIirDlg dialog


IMPLEMENT_DYNAMIC(CParmIirDlg, CParamDlg)
CParmIirDlg::CParmIirDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmIirDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   m_pDocTaps = NULL;
   //{{AFX_DATA_INIT(CParmIirDlg)
   m_TapLen = 0;
   m_TapStr = _T("");
   m_LenName = _T("");
   m_FactorStr = _T("");
   m_TapTitle = _T("");
   m_BQ = -1;
   //}}AFX_DATA_INIT
}


void CParmIirDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmIirDlg)
   DDX_Control(pDX, IDC_TYPE_STATIC, m_TypeFrame);
   DDX_Control(pDX, IDC_TYPE_0, m_TypeButton0);
   DDX_Control(pDX, IDC_TYPE_1, m_TypeButton1);
   DDX_Control(pDX, IDC_TAPS_STATIC, m_TapStatic);
   DDX_Control(pDX, IDC_FACTOR_STATIC, m_FactorStatic);
   DDX_Control(pDX, IDC_FACTOR_SPIN, m_FactorSpin);
   DDX_Control(pDX, IDC_FACTOR_EDIT, m_FactorEdit);
   DDX_Control(pDX, IDC_TAPS_EDIT, m_TapEdit);
   DDX_Text(pDX, IDC_TAPLEN_EDIT, m_TapLen);
   DDX_Text(pDX, IDC_TAPS_EDIT, m_TapStr);
   DDX_Text(pDX, IDC_LEN_STATIC, m_LenName);
   DDX_Text(pDX, IDC_FACTOR_EDIT, m_FactorStr);
   DDX_Text(pDX, IDC_TAPS_TITLE, m_TapTitle);
   DDX_Radio(pDX, IDC_TYPE_0, m_BQ);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmIirDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmIirDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnBQ)
   ON_BN_CLICKED(IDC_TYPE_1, OnBQ)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmIirDlg message handlers

BOOL CParmIirDlg::OnInitDialog() 
{
   ASSERT(m_pDocTaps);
   CParamDlg::OnInitDialog();

   if (m_Func.Found("Direct")) {
      m_TypeButton0.ShowWindow(SW_HIDE);
      m_TypeButton1.ShowWindow(SW_HIDE);
      m_TypeFrame.ShowWindow(SW_HIDE);
   }
   m_pDocTaps->GetVector()->Get(m_TapStr);
   m_TapTitle = m_pDocTaps->GetTitle();
   SetTapLen();
   m_FactorStr.Format("%d",m_tapsFactor);
   m_FactorSpin.SetRange(-32,32);
   EnableFactor();
   UpdateData(FALSE);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmIirDlg::OnOK() 
{
   UpdateData();
   m_tapsFactor = atoi(m_FactorStr);   
   CParamDlg::OnOK();
}

void CParmIirDlg::OnBQ() 
{
   UpdateData();
   SetTapLen();
   UpdateData(FALSE);
}

void CParmIirDlg::SetTapLen() 
{
   int len = m_pDocTaps->GetVector()->Length();
   if (m_BQ) {
      m_TapLen = len / 6;
      m_LenName = "Num Quads";
   } else {
      m_TapLen = len / 2 - 1;
      m_LenName = "Order";
   }
}

void CParmIirDlg::EnableFactor() 
{
   int flag = m_pDocTaps->GetVector()->Type() & PP_SIGN ? SW_SHOWNA : SW_HIDE;
   if (m_Func.Found("Direct")) flag = SW_HIDE;
   m_FactorStatic.ShowWindow(flag);
   m_FactorEdit.ShowWindow(flag);
   m_FactorSpin.ShowWindow(flag);
}

