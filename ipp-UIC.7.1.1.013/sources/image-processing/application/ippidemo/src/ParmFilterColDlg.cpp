/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterColDlg.cpp : implementation of the CParmFilterColDlg class.
// CParmFilterColDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmFilterColDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterColDlg dialog


CParmFilterColDlg::CParmFilterColDlg(UINT nID)
   : CParmFilterCustomDlg(nID, NULL)
{
   m_MaxSize[0] = 1;
   m_MaxSize[1] = 7;
   //{{AFX_DATA_INIT(CParmFilterColDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmFilterColDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFilterCustomDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFilterColDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmFilterColDlg, CParmFilterCustomDlg)
   //{{AFX_MSG_MAP(CParmFilterColDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterColDlg message handlers

BOOL CParmFilterColDlg::OnInitDialog()
{
   CParmFilterCustomDlg::OnInitDialog();

   m_SizeStatic[0].ShowWindow(SW_HIDE);
   m_SizeEdit  [0].ShowWindow(SW_HIDE);
   m_SizeSpin  [0].ShowWindow(SW_HIDE);
   m_AnchorStatic[0].ShowWindow(SW_HIDE);
   m_AnchorEdit  [0].ShowWindow(SW_HIDE);
   m_AnchorSpin  [0].ShowWindow(SW_HIDE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmFilterColDlg::OnOK()
{
   // TODO: Add extra validation here

   CParmFilterCustomDlg::OnOK();
}
