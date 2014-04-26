/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCnvrtDlg.cpp : implementation of the CParmCnvrtDlg class.
// CParmCnvrtDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmCnvrtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCnvrtDlg dialog


IMPLEMENT_DYNAMIC(CParmCnvrtDlg, CParamDlg)
CParmCnvrtDlg::CParmCnvrtDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCnvrtDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmCnvrtDlg)
   m_Rnd = -1;
   //}}AFX_DATA_INIT
}


void CParmCnvrtDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCnvrtDlg)
   DDX_Radio(pDX, IDC_RND_0, m_Rnd);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCnvrtDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCnvrtDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCnvrtDlg message handlers

BOOL CParmCnvrtDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   // TODO: Add extra initialization here
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmCnvrtDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}
