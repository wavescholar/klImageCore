/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// OptDstDlg.cpp : implementation of the New Dst Conception dialog.
// Command: Menu-Options-NewDst
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "OptDstDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptDstDlg dialog


COptDstDlg::COptDstDlg(CWnd* pParent /*=NULL*/)
   : CDialog(COptDstDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(COptDstDlg)
   m_PickDst = -1;
   m_NewSize = -1;
   //}}AFX_DATA_INIT
}


void COptDstDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(COptDstDlg)
   DDX_Radio(pDX, IDC_NEW_0, m_PickDst);
   DDX_Radio(pDX, IDC_SIZE_0, m_NewSize);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptDstDlg, CDialog)
   //{{AFX_MSG_MAP(COptDstDlg)
   ON_BN_CLICKED(IDC_NEW_0, OnCreate)
   ON_BN_CLICKED(IDC_NEW_1, OnCreate)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptDstDlg message handlers

BOOL COptDstDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   OnCreate();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void COptDstDlg::OnCreate()
{
   UpdateData();
    BOOL flag = !m_PickDst;
    GetDlgItem(IDC_SIZE_0)->EnableWindow(flag);
    GetDlgItem(IDC_SIZE_1)->EnableWindow(flag);
}
