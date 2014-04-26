/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVMorphDlg.cpp : implementation of the CParmCVMorphDlg class.
// CParmCVMorphDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunCVMorph.h"
#include "ParmCVMorphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVMorphDlg dialog


CParmCVMorphDlg::CParmCVMorphDlg()
   : CParmMorphDlg(CParmCVMorphDlg::IDD)
{
   //{{AFX_DATA_INIT(CParmCVMorphDlg)
   m_BorderType = -1;
   //}}AFX_DATA_INIT
}


void CParmCVMorphDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmMorphDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVMorphDlg)
   DDX_Radio(pDX, IDC_BORDER_0, m_BorderType);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVMorphDlg, CParmMorphDlg)
   //{{AFX_MSG_MAP(CParmCVMorphDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVMorphDlg message handlers

BOOL CParmCVMorphDlg::OnInitDialog()
{

   CParmMorphDlg::OnInitDialog();

   // TODO: Add extra initialization here

   return TRUE;
}

void CParmCVMorphDlg::OnOK()
{
   CParmMorphDlg::OnOK();
}
