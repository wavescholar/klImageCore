/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// FilterDlg.cpp : implementation of the Filter dialog.
// Command: Menu-Process-Process by + Filter button
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "FilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog


CFilterDlg::CFilterDlg(UINT nID, CString libTitle)
   : CDialog(nID, NULL), m_LibTitle(libTitle)
{
   //{{AFX_DATA_INIT(CFilterDlg)
   //}}AFX_DATA_INIT
}


void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CFilterDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_CHECK_0, m_CheckButton[0]);
   DDX_Control(pDX, IDC_CHECK_1, m_CheckButton[1]);
   DDX_Control(pDX, IDC_CHECK_2, m_CheckButton[2]);
   DDX_Control(pDX, IDC_CHECK_3, m_CheckButton[3]);
   DDX_Check(pDX, IDC_CHECK_0, m_Filter.m_Switch[0]);
   DDX_Check(pDX, IDC_CHECK_1, m_Filter.m_Switch[1]);
   DDX_Check(pDX, IDC_CHECK_2, m_Filter.m_Switch[2]);
   DDX_Check(pDX, IDC_CHECK_3, m_Filter.m_Switch[3]);
   DDX_Check(pDX, IDC_ENABLE_0, m_Filter.m_Enable[0]);
   DDX_Check(pDX, IDC_ENABLE_1, m_Filter.m_Enable[1]);
   DDX_Check(pDX, IDC_ENABLE_2, m_Filter.m_Enable[2]);
   DDX_Check(pDX, IDC_ENABLE_3, m_Filter.m_Enable[3]);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
   //{{AFX_MSG_MAP(CFilterDlg)
   ON_BN_CLICKED(IDC_ENABLE_0, OnEnable0)
   ON_BN_CLICKED(IDC_ENABLE_1, OnEnable1)
   ON_BN_CLICKED(IDC_ENABLE_2, OnEnable2)
   ON_BN_CLICKED(IDC_ENABLE_3, OnEnable3)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg message handlers

void CFilterDlg::OnEnable(int idx) 
{
   UpdateData();
   m_CheckButton[idx].EnableWindow(m_Filter.m_Enable[idx]);
}

void CFilterDlg::OnEnable0() { OnEnable(0);}
void CFilterDlg::OnEnable1() { OnEnable(1);}
void CFilterDlg::OnEnable2() { OnEnable(2);}
void CFilterDlg::OnEnable3() { OnEnable(3);}

BOOL CFilterDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   SetWindowText(m_LibTitle + _T(" Filter"));   
   for (int i=0; i<4; i++)
      OnEnable(i);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
