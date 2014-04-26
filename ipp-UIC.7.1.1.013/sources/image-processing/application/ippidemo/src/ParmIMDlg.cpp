/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIMDlg.cpp : implementation of the CParmIMDlg class.
// CParmIMDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "DemoDoc.h"
#include "ParmIMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmIMDlg dialog


CParmIMDlg::CParmIMDlg(UINT nID) : CParamDlg(nID)
{
   m_pDocSrc3 = NULL;
   //{{AFX_DATA_INIT(CParmIMDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmIMDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmIMDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
   if (m_pDocSrc3)
      DDX_Text(pDX, IDC_EDIT_SRC3, m_StrSrc3);
}


BEGIN_MESSAGE_MAP(CParmIMDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmIMDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmIMDlg message handlers

BOOL CParmIMDlg::OnInitDialog()
{
   if (m_pDocSrc3)
      m_StrSrc3 = m_pDocSrc3->GetTitle();
   CParamDlg::OnInitDialog();
   if (!m_pDocMask) {
      GetDlgItem(IDC_STATIC_MASK)->EnableWindow(FALSE);
      GetDlgItem(IDC_EDIT_MASK)->EnableWindow(FALSE);
   }
   return TRUE;
}

void CParmIMDlg::OnOK()
{
   CParamDlg::OnOK();
}

void CParmIMDlg::InitInplace()
{
   return;
}
