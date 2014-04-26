/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMirrorDlg.cpp : implementation of the CParmMirrorDlg class.
// CParmMirrorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmMirrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmMirrorDlg dialog


CParmMirrorDlg::CParmMirrorDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmMirrorDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmMirrorDlg)
   m_Flip = -1;
   //}}AFX_DATA_INIT
}


void CParmMirrorDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmMirrorDlg)
   DDX_Radio(pDX, IDC_F0, m_Flip);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmMirrorDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmMirrorDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmMirrorDlg message handlers

BOOL CParmMirrorDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmMirrorDlg::OnOK()
{
   // TODO: Add extra validation here

   CParamDlg::OnOK();
}
