/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmScaleDlg.cpp : implementation of the CParmScaleDlg class.
// CParmScaleDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmScaleDlg dialog


CParmScaleDlg::CParmScaleDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmScaleDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmScaleDlg)
   m_MaxStr = _T("");
   m_MinStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmScaleDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmScaleDlg)
   DDX_Text(pDX, IDC_VMAX, m_MaxStr);
   DDX_Text(pDX, IDC_VMIN, m_MinStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmScaleDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmScaleDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmScaleDlg message handlers

BOOL CParmScaleDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmScaleDlg::OnOK()
{
   // TODO: Add extra validation here

   CParamDlg::OnOK();
}
