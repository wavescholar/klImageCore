/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVMotionDlg.cpp : implementation of the CParmCVMotionDlg class.
// CParmCVMotionDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmCVMotionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVMotionDlg dialog


CParmCVMotionDlg::CParmCVMotionDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCVMotionDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmCVMotionDlg)
   m_DurationStr = _T("");
   m_StampStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCVMotionDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVMotionDlg)
   DDX_Text(pDX, IDC_DURATION, m_DurationStr);
   DDX_Text(pDX, IDC_STAMP, m_StampStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVMotionDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCVMotionDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVMotionDlg message handlers

void CParmCVMotionDlg::InitInplace()
{
}
