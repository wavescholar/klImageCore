/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmAlphaCDlg.cpp : implementation of the CParmAlphaCDlg class.
// CParmAlphaCDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmAlphaCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaCDlg dialog


CParmAlphaCDlg::CParmAlphaCDlg(CWnd* pParent /*=NULL*/)
   : CParmAlphaDlg(CParmAlphaCDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmAlphaCDlg)
   m_Alpha1Str = _T("");
   m_Alpha2Str = _T("");
   //}}AFX_DATA_INIT
}


void CParmAlphaCDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmAlphaDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmAlphaCDlg)
   DDX_Text(pDX, IDC_ALPHA_A, m_Alpha1Str);
   DDX_Text(pDX, IDC_ALPHA_B, m_Alpha2Str);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmAlphaCDlg, CParmAlphaDlg)
   //{{AFX_MSG_MAP(CParmAlphaCDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaCDlg message handlers

BOOL CParmAlphaCDlg::OnInitDialog()
{
   CParmAlphaDlg::OnInitDialog();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmAlphaCDlg::OnOK()
{
   // TODO: Add extra validation here

   CParmAlphaDlg::OnOK();
}
