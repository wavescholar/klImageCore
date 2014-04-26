/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRampDlg.cpp : implementation of the CParmRampDlg class.
// CParmRampDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmRampDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRampDlg dialog


IMPLEMENT_DYNAMIC(CParmRampDlg, CParamDlg)
CParmRampDlg::CParmRampDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmRampDlg::IDD, pParent)
{
   m_UsedScale   = FALSE;
   //{{AFX_DATA_INIT(CParmRampDlg)
   m_Val0Str = _T("");
   m_Val0Name = _T("");
   m_Val1Name = _T("");
   m_Val1Str = _T("");
   //}}AFX_DATA_INIT
}


void CParmRampDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmRampDlg)
   DDX_Text(pDX, IDC_VAL0, m_Val0Str);
   DDX_Text(pDX, IDC_STATIC_VAL0, m_Val0Name);
   DDX_Text(pDX, IDC_STATIC_VAL1, m_Val1Name);
   DDX_Text(pDX, IDC_VAL1, m_Val1Str);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmRampDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRampDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRampDlg message handlers
