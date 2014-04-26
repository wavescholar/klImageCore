/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVCopyDlg.cpp : implementation of the CParmCVCopyDlg class.
// CParmCVCopyDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVCopyDlg dialog


CParmCVCopyDlg::CParmCVCopyDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCVCopyDlg::IDD, pParent)
{
   m_UsedScale = TRUE;
   //{{AFX_DATA_INIT(CParmCVCopyDlg)
   m_dxStr = _T("");
   m_dyStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCVCopyDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVCopyDlg)
   DDX_Text(pDX, IDC_DX, m_dxStr);
   DDX_Text(pDX, IDC_DY, m_dyStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVCopyDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCVCopyDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVCopyDlg message handlers
