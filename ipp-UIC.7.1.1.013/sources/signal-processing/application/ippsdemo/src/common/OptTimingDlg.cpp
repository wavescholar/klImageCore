/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// OptTimingDlg.cpp : implementation of the Timing Options dialog.
// Command: Menu-Options-Timing
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "demo.h"
#include "OptTimingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptTimingDlg dialog


COptTimingDlg::COptTimingDlg(CWnd* pParent /*=NULL*/)
   : CDialog(COptTimingDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(COptTimingDlg)
   m_Do = FALSE;
   m_Method = -1;
   m_NumberStr = _T("");
   //}}AFX_DATA_INIT
}


void COptTimingDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(COptTimingDlg)
   DDX_Control(pDX, IDC_NUM_STATIC, m_NumberStatic);
   DDX_Control(pDX, IDC_NUM_SPIN, m_NumberSpin);
   DDX_Control(pDX, IDC_NUM_EDIT, m_NumberEdit);
   DDX_Control(pDX, IDC_METHOD_STATIC, m_MethodStatic);
   DDX_Check(pDX, IDC_DO, m_Do);
   DDX_Radio(pDX, IDC_METHOD_0, m_Method);
   DDX_Text(pDX, IDC_NUM_EDIT, m_NumberStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_METHOD_0, m_MethodButton[0]);
   DDX_Control(pDX, IDC_METHOD_1, m_MethodButton[1]);
   DDX_Control(pDX, IDC_METHOD_2, m_MethodButton[2]);
}


BEGIN_MESSAGE_MAP(COptTimingDlg, CDialog)
   //{{AFX_MSG_MAP(COptTimingDlg)
   ON_BN_CLICKED(IDC_DO, OnDo)
   ON_BN_CLICKED(IDC_METHOD_0, OnMethod)
   ON_BN_CLICKED(IDC_METHOD_1, OnMethod)
   ON_BN_CLICKED(IDC_METHOD_2, OnMethod)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptTimingDlg message handlers

BOOL COptTimingDlg::OnInitDialog() 
{
   m_NumberStr.Format(_T("%d"), m_Number);
   CDialog::OnInitDialog();
   EnableMethod();
   EnableNumber();
   m_NumberSpin.SetRange(1,32000);
   return TRUE;
}

void COptTimingDlg::OnOK() 
{
   CDialog::OnOK();
   m_Number = _ttoi(m_NumberStr);
   if (m_Number < 1) m_Number = 1;
}

void COptTimingDlg::OnDo() 
{
   UpdateData();
   EnableMethod();
   EnableNumber();
}

void COptTimingDlg::OnMethod() 
{
   UpdateData();
   EnableNumber();
}

void COptTimingDlg::EnableMethod() 
{
   BOOL flag = m_Do;
   m_MethodStatic.EnableWindow(flag);
   for (int i=0; i<timNUM; i++)
      m_MethodButton[i].EnableWindow(flag);
}

void COptTimingDlg::EnableNumber() 
{
   BOOL flag = m_Do && m_Method != timAUTO;
   m_NumberStatic.EnableWindow(flag);
   m_NumberEdit.EnableWindow(flag);
   m_NumberSpin.EnableWindow(flag);
}
