/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmLmsMRDlg.cpp : implementation of the CParmLmsMRDlg class.
// CParmLmsMRDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmLmsMRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmLmsMRDlg dialog


IMPLEMENT_DYNAMIC(CParmLmsMRDlg, CParmLmsDlg)
CParmLmsMRDlg::CParmLmsMRDlg(CWnd* pParent /*=NULL*/)
   : CParmLmsDlg(CParmLmsMRDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmLmsMRDlg)
   m_StepStr = _T("");
   m_UpdateStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmLmsMRDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmLmsDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmLmsMRDlg)
   DDX_Control(pDX, IDC_UPDATE_SPIN, m_UpdateSpin);
   DDX_Control(pDX, IDC_STEP_SPIN, m_StepSpin);
   DDX_Text(pDX, IDC_STEP_EDIT, m_StepStr);
   DDX_Text(pDX, IDC_UPDATE_EDIT, m_UpdateStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmLmsMRDlg, CParmLmsDlg)
   //{{AFX_MSG_MAP(CParmLmsMRDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmLmsMRDlg message handlers

BOOL CParmLmsMRDlg::OnInitDialog() 
{
   CParmLmsDlg::OnInitDialog();
   
   m_StepSpin.SetRange(1, SHRT_MAX);
   m_UpdateSpin.SetRange(0, SHRT_MAX);
   
   return TRUE; 
}
