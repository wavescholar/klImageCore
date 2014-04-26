/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmToneDlg.cpp : implementation of the CParmToneDlg class.
// CParmToneDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmToneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmToneDlg dialog


IMPLEMENT_DYNAMIC(CParmToneDlg, CParamDlg)
CParmToneDlg::CParmToneDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmToneDlg::IDD, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmToneDlg)
   m_hint = -1;
   //}}AFX_DATA_INIT
}


void CParmToneDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmToneDlg)
   DDX_Radio(pDX, IDC_HINT_0, m_hint);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_PARM_0, m_ParmStr[0]);
   DDX_Text(pDX, IDC_PARM_1, m_ParmStr[1]);
   DDX_Text(pDX, IDC_PARM_2, m_ParmStr[2]);
}


BEGIN_MESSAGE_MAP(CParmToneDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmToneDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmToneDlg message handlers

BOOL CParmToneDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (m_Func.Found("Q15")) {
      GetDlgItem(IDC_HINT_0)->EnableWindow(FALSE);
      GetDlgItem(IDC_HINT_1)->EnableWindow(FALSE);
      GetDlgItem(IDC_HINT_2)->EnableWindow(FALSE);
   }

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
