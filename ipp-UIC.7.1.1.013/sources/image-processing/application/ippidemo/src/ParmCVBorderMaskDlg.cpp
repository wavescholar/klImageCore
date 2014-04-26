/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVBorderMaskDlg.cpp : implementation of the CParmCVBorderMaskDlg class.
// CParmCVBorderMaskDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVBorderMaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVBorderMaskDlg dialog


CParmCVBorderMaskDlg::CParmCVBorderMaskDlg(UINT nID)
   : CParmCVBorderDlg(nID)
{
   //{{AFX_DATA_INIT(CParmCVBorderMaskDlg)
   m_Mask = -1;
   //}}AFX_DATA_INIT
}


void CParmCVBorderMaskDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmCVBorderDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVBorderMaskDlg)
   DDX_Radio(pDX, IDC_MASK_0, m_Mask);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVBorderMaskDlg, CParmCVBorderDlg)
   //{{AFX_MSG_MAP(CParmCVBorderMaskDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVBorderMaskDlg message handlers
