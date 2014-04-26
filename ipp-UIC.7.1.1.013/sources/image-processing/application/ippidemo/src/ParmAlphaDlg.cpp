/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmAlphaDlg.cpp : implementation of the CParmAlphaDlg class.
// CParmAlphaDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmAlphaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaDlg dialog

CParmAlphaDlg::CParmAlphaDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   //{{AFX_DATA_INIT(CParmAlphaDlg)
   m_AlphaType = -1;
   m_ShowPremul = FALSE;
   m_Inplace = FALSE;
   //}}AFX_DATA_INIT
   m_AlphaType = 0;
}


void CParmAlphaDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmAlphaDlg)
   DDX_Control(pDX, IDC_SHOW_PREMUL, m_ShowButton);
   DDX_Radio(pDX, IDC_TYPE_0, m_AlphaType);
   DDX_Check(pDX, IDC_SHOW_PREMUL, m_ShowPremul);
   DDX_Check(pDX, IDC_INPLACE, m_Inplace);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmAlphaDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmAlphaDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_BN_CLICKED(IDC_TYPE_3, OnType)
   ON_BN_CLICKED(IDC_TYPE_4, OnType)
   ON_BN_CLICKED(IDC_TYPE_5, OnType)
   ON_BN_CLICKED(IDC_TYPE_6, OnType)
   ON_BN_CLICKED(IDC_TYPE_7, OnType)
   ON_BN_CLICKED(IDC_TYPE_8, OnType)
   ON_BN_CLICKED(IDC_TYPE_9, OnType)
   ON_BN_CLICKED(IDC_TYPE_10, OnType)
   ON_BN_CLICKED(IDC_TYPE_11, OnType)
   ON_BN_CLICKED(IDC_INPLACE, OnInplace)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaDlg message handlers

BOOL CParmAlphaDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();
   EnableType();
   EnableShowPremul();
   UpdateData(FALSE);
   return TRUE;  // return TRUE unless you set the focus to a control
}

void CParmAlphaDlg::OnOK()
{
   CParamDlg::OnOK();
}

void CParmAlphaDlg::EnableType()
{
    if (m_Func.SrcChannels() == 4) return;
    if (m_AlphaType >= 6) m_AlphaType -= 6;
    GetDlgItem(IDC_TYPE_6)->EnableWindow(FALSE);
    GetDlgItem(IDC_TYPE_7)->EnableWindow(FALSE);
    GetDlgItem(IDC_TYPE_8)->EnableWindow(FALSE);
    GetDlgItem(IDC_TYPE_9)->EnableWindow(FALSE);
    GetDlgItem(IDC_TYPE_10)->EnableWindow(FALSE);
    GetDlgItem(IDC_TYPE_11)->EnableWindow(FALSE);
}

void CParmAlphaDlg::OnType()
{
   UpdateData();
   EnableShowPremul();
}

void CParmAlphaDlg::OnInplace()
{
   UpdateData();
   EnableShowPremul();
}

void CParmAlphaDlg::EnableShowPremul()
{
    m_ShowButton.EnableWindow(!m_Inplace && m_AlphaType >= 6);
}
