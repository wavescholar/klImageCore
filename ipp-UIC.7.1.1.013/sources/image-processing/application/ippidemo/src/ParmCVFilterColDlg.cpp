/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVFilterColDlg.cpp : implementation of the CParmCVFilterColDlg class.
// CParmCVFilterColDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVFilterColDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVFilterColDlg dialog


CParmCVFilterColDlg::CParmCVFilterColDlg(UINT nID)
   : CParmFilterColDlg(nID)
{
   //{{AFX_DATA_INIT(CParmCVFilterColDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmCVFilterColDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFilterColDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVFilterColDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVFilterColDlg, CParmFilterColDlg)
   //{{AFX_MSG_MAP(CParmCVFilterColDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVFilterColDlg message handlers
