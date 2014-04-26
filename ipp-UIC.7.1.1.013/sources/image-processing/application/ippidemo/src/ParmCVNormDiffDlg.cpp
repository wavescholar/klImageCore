/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVNormDiffDlg.cpp : implementation of the CParmCVNormDiffDlg class.
// CParmCVNormDiffDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVNormDiffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVNormDiffDlg dialog


CParmCVNormDiffDlg::CParmCVNormDiffDlg(CRunCVNorm* pRun, UINT nID)
   : CParmCVNormDlg(pRun, nID)
{
   //{{AFX_DATA_INIT(CParmCVNormDiffDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmCVNormDiffDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmCVNormDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVNormDiffDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVNormDiffDlg, CParmCVNormDlg)
   //{{AFX_MSG_MAP(CParmCVNormDiffDlg)
   ON_BN_CLICKED(IDC_NORM_3, OnFunc)
   ON_BN_CLICKED(IDC_NORM_4, OnFunc)
   ON_BN_CLICKED(IDC_NORM_5, OnFunc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVNormDiffDlg message handlers

CFunc CParmCVNormDiffDlg::GetFunc(int idx)
{
   CString prefix = "ippiNorm";
   CString suffix = "_" + m_Func.TypeName() + "_" + m_Func.DescrName();
   switch (idx) {
   case 0: return prefix + "Diff_Inf" + suffix;
   case 1: return prefix + "Diff_L1" + suffix;
   case 2: return prefix + "Diff_L2" + suffix;
   case 3: return prefix + "Rel_Inf" + suffix;
   case 4: return prefix + "Rel_L1" + suffix;
   case 5: return prefix + "Rel_L2" + suffix;
   }
   return "";
}
