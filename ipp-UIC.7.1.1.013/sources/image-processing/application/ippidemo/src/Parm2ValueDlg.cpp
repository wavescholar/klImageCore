/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm2ValueDlg.cpp : implementation of the CParm2ValueDlg class.
// CParm2ValueDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "Parm2ValueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParm2ValueDlg dialog


CParm2ValueDlg::CParm2ValueDlg()
   : CParmIMDlg(CParm2ValueDlg::IDD)
{

   //{{AFX_DATA_INIT(CParm2ValueDlg)
   m_ValStr = _T("");
   m_ValName = _T("");
   //}}AFX_DATA_INIT
}


void CParm2ValueDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmIMDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParm2ValueDlg)
   DDX_Text(pDX, IDC_VALUE_EDIT, m_ValStr);
   DDX_Text(pDX, IDC_VALUE_STATIC, m_ValName);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParm2ValueDlg, CParmIMDlg)
   //{{AFX_MSG_MAP(CParm2ValueDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParm2ValueDlg message handlers
BOOL CParm2ValueDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();
   if (!m_pDocSrc2) {
      GetDlgItem(IDC_STATIC_SRC2)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_EDIT_SRC2)->ShowWindow(SW_HIDE);
   }
   if (!m_pDocDst) {
      GetDlgItem(IDC_STATIC_DST)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_EDIT_DST)->ShowWindow(SW_HIDE);
   }
   return TRUE;
}
