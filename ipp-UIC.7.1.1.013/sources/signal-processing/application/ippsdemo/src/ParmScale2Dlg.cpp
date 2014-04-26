
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmScale2Dlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmScale2Dlg.h"

IMPLEMENT_DYNAMIC(CParmScale2Dlg, CParamDlg)

CParmScale2Dlg::CParmScale2Dlg(UINT nID)
   : CParamDlg(nID)
   , m_scaleFactor2(0)
   , m_scaleFactor1(0)
{
}

CParmScale2Dlg::~CParmScale2Dlg()
{
}

void CParmScale2Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_SCALE_EDIT_2, m_scaleFactor2);
   DDX_Text(pDX, IDC_SCALE_EDIT_1, m_scaleFactor1);
   DDX_Control(pDX, IDC_SCALE_SPIN_1, m_scaleSpin1);
   DDX_Control(pDX, IDC_SCALE_SPIN_2, m_scaleSpin2);
}


BEGIN_MESSAGE_MAP(CParmScale2Dlg, CParamDlg)
END_MESSAGE_MAP()


// CParmScale2Dlg message handlers

BOOL CParmScale2Dlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_scaleSpin1.SetRange(SHRT_MIN, SHRT_MAX);
   m_scaleSpin2.SetRange(SHRT_MIN, SHRT_MAX);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}
