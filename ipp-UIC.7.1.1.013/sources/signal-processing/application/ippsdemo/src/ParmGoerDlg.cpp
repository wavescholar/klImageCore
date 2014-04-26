/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmGoerDlg.cpp : implementation of the CParmGoerDlg class.
// CParmGoerDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmGoerDlg.h"
#include "RunGoer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmGoerDlg dialog


IMPLEMENT_DYNAMIC(CParmGoerDlg, CParamDlg)
CParmGoerDlg::CParmGoerDlg(CRunGoer* pRun)
   : CParamDlg(CParmGoerDlg::IDD, NULL)
{
   m_UsedScale   = TRUE;
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmGoerDlg)
   //}}AFX_DATA_INIT
}


void CParmGoerDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmGoerDlg)
   DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_VALUE_STATIC, m_ValStatic[0]);
   DDX_Control(pDX, IDC_VALUE_RE, m_ReEdit[0]);
   DDX_Control(pDX, IDC_VALUE_IM, m_ImEdit[0]);
   DDX_Control(pDX, IDC_STATIC_RE, m_ReStatic[0]);
   DDX_Control(pDX, IDC_STATIC_IM, m_ImStatic[0]);
   DDX_Control(pDX, IDC_FREQ_STATIC, m_FreqStatic[0]);
   DDX_Control(pDX, IDC_FREQ_EDIT, m_FreqEdit[0]);
   DDX_Text(pDX, IDC_FREQ_EDIT, m_FreqStr[0]);
   DDX_Text(pDX, IDC_VALUE_IM, m_Im[0]);
   DDX_Text(pDX, IDC_VALUE_RE, m_Re[0]);

   DDX_Control(pDX, IDC_VALUE_STATIC2, m_ValStatic[1]);
   DDX_Control(pDX, IDC_VALUE_RE2, m_ReEdit[1]);
   DDX_Control(pDX, IDC_VALUE_IM2, m_ImEdit[1]);
   DDX_Control(pDX, IDC_STATIC_RE2, m_ReStatic[1]);
   DDX_Control(pDX, IDC_STATIC_IM2, m_ImStatic[1]);
   DDX_Control(pDX, IDC_FREQ_STATIC2, m_FreqStatic[1]);
   DDX_Control(pDX, IDC_FREQ_EDIT2, m_FreqEdit[1]);
   DDX_Text(pDX, IDC_FREQ_EDIT2, m_FreqStr[1]);
   DDX_Text(pDX, IDC_VALUE_IM2, m_Im[1]);
   DDX_Text(pDX, IDC_VALUE_RE2, m_Re[1]);
}


BEGIN_MESSAGE_MAP(CParmGoerDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmGoerDlg)
   ON_EN_CHANGE(IDC_SCALE_EDIT, OnChange)
   ON_EN_CHANGE(IDC_FREQ_EDIT, OnChange)
   ON_EN_CHANGE(IDC_FREQ_EDIT2, OnChange)
   ON_BN_CLICKED(IDC_APPLY, OnApply)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmGoerDlg message handlers

BOOL CParmGoerDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   if (m_nModalResult == IDCANCEL) return TRUE;
   if (m_Func.Find("Two") == -1) {
      m_FreqStatic[0].SetWindowText("Freq");
      m_ValStatic[0].SetWindowText("DFT");
      m_FreqStatic[1].ShowWindow(SW_HIDE);
      m_FreqEdit[1].ShowWindow(SW_HIDE);
      m_ValStatic[1].ShowWindow(SW_HIDE);
      m_ReEdit[1].ShowWindow(SW_HIDE);
      m_ImEdit[1].ShowWindow(SW_HIDE);
      m_ReStatic[1].ShowWindow(SW_HIDE);
      m_ImStatic[1].ShowWindow(SW_HIDE);
   }
   OnApply();
   return TRUE; 
}

void CParmGoerDlg::OnOK() 
{
   CParamDlg::OnOK();
}

void CParmGoerDlg::OnChange() 
{
   if (!m_bInit) return;
   m_ApplyButton.EnableWindow(TRUE);
}

void CParmGoerDlg::OnApply() 
{
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   m_ApplyButton.EnableWindow(FALSE);
}
