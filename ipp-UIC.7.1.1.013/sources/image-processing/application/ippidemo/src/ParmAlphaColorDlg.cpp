/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmAlphaColorDlg.cpp : implementation of the CParmAlphaColorDlg class.
// CParmAlphaColorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmAlphaColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaColorDlg dialog


CParmAlphaColorDlg::CParmAlphaColorDlg(CWnd* pParent /*=NULL*/)
   : CParmCompColorDlg(CParmAlphaColorDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmAlphaColorDlg)
   m_alpha1Str = _T("");
   m_alpha2Str = _T("");
   //}}AFX_DATA_INIT
}


void CParmAlphaColorDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmCompColorDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmAlphaColorDlg)
   DDX_Radio(pDX, IDC_TYPE_0, m_alphaType);
   DDX_Text(pDX, IDC_ALPHA_A, m_alpha1Str);
   DDX_Text(pDX, IDC_ALPHA_B, m_alpha2Str);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmAlphaColorDlg, CParmCompColorDlg)
   //{{AFX_MSG_MAP(CParmAlphaColorDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaColorDlg message handlers

BOOL CParmAlphaColorDlg::OnInitDialog()
{
   CParmCompColorDlg::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmAlphaColorDlg::OnOK()
{
   // TODO: Add extra validation here

   CParmCompColorDlg::OnOK();
}
