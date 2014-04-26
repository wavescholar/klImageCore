/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterRowDlg.cpp : implementation of the CParmFilterRowDlg class.
// CParmFilterRowDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmFilterRowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterRowDlg dialog


CParmFilterRowDlg::CParmFilterRowDlg(UINT nID)
   : CParmFilterCustomDlg(nID, NULL)
{
   m_MaxSize[0] = 7;
   m_MaxSize[1] = 1;
   //{{AFX_DATA_INIT(CParmFilterRowDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmFilterRowDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFilterCustomDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFilterRowDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmFilterRowDlg, CParmFilterCustomDlg)
   //{{AFX_MSG_MAP(CParmFilterRowDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterRowDlg message handlers

BOOL CParmFilterRowDlg::OnInitDialog()
{
   CParmFilterCustomDlg::OnInitDialog();

   m_SizeStatic[1].ShowWindow(SW_HIDE);
   m_SizeEdit  [1].ShowWindow(SW_HIDE);
   m_SizeSpin  [1].ShowWindow(SW_HIDE);
   m_AnchorStatic[1].ShowWindow(SW_HIDE);
   m_AnchorEdit  [1].ShowWindow(SW_HIDE);
   m_AnchorSpin  [1].ShowWindow(SW_HIDE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmFilterRowDlg::OnOK()
{
   CParmFilterCustomDlg::OnOK();
}
