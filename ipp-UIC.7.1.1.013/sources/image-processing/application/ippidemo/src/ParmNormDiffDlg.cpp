/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormDiffDlg.cpp : implementation of the CParmNormDiffDlg class.
// CParmNormDiffDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippidemodoc.h"
#include "ParmNormDiffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmNormDiffDlg dialog

   enum {
      NormDiff_Inf, NormDiff_L1, NormDiff_L2,
      NormRel_Inf, NormRel_L1, NormRel_L2,
      QualityIndex,
      NormNum};

CParmNormDiffDlg::CParmNormDiffDlg(CRunNorm* pRun)
   : CParmNormDlg(pRun, CParmNormDiffDlg::IDD)
{
   //{{AFX_DATA_INIT(CParmNormDiffDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmNormDiffDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmNormDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmNormDiffDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_NORM_3, m_NormButton[3]);
   DDX_Control(pDX, IDC_NORM_4, m_NormButton[4]);
   DDX_Control(pDX, IDC_NORM_5, m_NormButton[5]);
   DDX_Control(pDX, IDC_NORM_6, m_NormButton[6]);
}


BEGIN_MESSAGE_MAP(CParmNormDiffDlg, CParmNormDlg)
   //{{AFX_MSG_MAP(CParmNormDiffDlg)
   ON_BN_CLICKED(IDC_NORM_3, OnNorm)
   ON_BN_CLICKED(IDC_NORM_4, OnNorm)
   ON_BN_CLICKED(IDC_NORM_5, OnNorm)
   ON_BN_CLICKED(IDC_NORM_6, OnNorm)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmNormDiffDlg message handlers

BOOL CParmNormDiffDlg::OnInitDialog()
{
   CParmNormDlg::OnInitDialog();

   SetWindowText("Differencial Norms & Quality Index");
   ppType type = m_pDocSrc->GetVector()->Type();
   if (type != pp8u && type != pp32f)
      GetDlgItem(IDC_NORM_6)->EnableWindow(FALSE);

   return TRUE;
}

#define NORM_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return Norm##name; \
}

int CParmNormDiffDlg::NormIndex(CString funcName)
{
   NORM_INDEX(Diff_Inf);
   NORM_INDEX(Diff_L1 );
   NORM_INDEX(Diff_L2 );
   NORM_INDEX(Rel_Inf);
   NORM_INDEX(Rel_L1 );
   NORM_INDEX(Rel_L2 );
   return QualityIndex;
}

#define NORM_FUNC(name) { \
   if (idx == Norm##name) \
      return "Norm" #name; \
}

CString CParmNormDiffDlg::NormFunc(int idx)
{
   NORM_FUNC(Diff_Inf);
   NORM_FUNC(Diff_L1 );
   NORM_FUNC(Diff_L2 );
   NORM_FUNC(Rel_Inf);
   NORM_FUNC(Rel_L1 );
   NORM_FUNC(Rel_L2 );
   return "QualityIndex";
}
