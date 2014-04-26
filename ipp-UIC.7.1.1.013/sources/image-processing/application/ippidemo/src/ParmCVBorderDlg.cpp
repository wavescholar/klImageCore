/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVBorderDlg.cpp : implementation of the CParmCVBorderDlg class.
// CParmCVBorderDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVBorderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVBorderDlg dialog


CParmCVBorderDlg::CParmCVBorderDlg(UINT nID) : CParamDlg(nID)
{
   //{{AFX_DATA_INIT(CParmCVBorderDlg)
   m_BorderType = -1;
   m_ValueStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCVBorderDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVBorderDlg)
   DDX_Control(pDX, IDC_VALUE, m_ValueEdit);
   DDX_Radio(pDX, IDC_BORDER_0, m_BorderType);
   DDX_Text(pDX, IDC_VALUE, m_ValueStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVBorderDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCVBorderDlg)
   ON_BN_CLICKED(IDC_BORDER_0, OnBorder)
   ON_BN_CLICKED(IDC_BORDER_1, OnBorder)
   ON_BN_CLICKED(IDC_BORDER_2, OnBorder)
   ON_BN_CLICKED(IDC_BORDER_3, OnBorder)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVBorderDlg message handlers

void CParmCVBorderDlg::OnBorder()
{
   UpdateData();
   m_ValueEdit.EnableWindow(m_BorderType == 0);
}

BOOL CParmCVBorderDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_ValueEdit.EnableWindow(m_BorderType == 0);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
