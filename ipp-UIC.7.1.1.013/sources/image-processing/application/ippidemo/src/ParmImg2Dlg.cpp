/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmImg2Dlg.cpp : implementation of the CParmImg2Dlg class.
// CParmImg2Dlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmImg2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmImg2Dlg dialog

CParmImg2Dlg::CParmImg2Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmImg2Dlg::IDD, pParent)
{
   m_UsedScale = TRUE;
   //{{AFX_DATA_INIT(CParmImg2Dlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmImg2Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmImg2Dlg)
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmImg2Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmImg2Dlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmImg2Dlg message handlers

BOOL CParmImg2Dlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (!m_Func.Scale()) {
      CParamDlg::OnOK();
   }
   return TRUE;
}
