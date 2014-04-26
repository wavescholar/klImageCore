/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmUpdateDlg.cpp : implementation of the CParmUpdateDlg class.
// CParmUpdateDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmUpdateDlg.h"
#include "RunUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmUpdateDlg dialog


IMPLEMENT_DYNAMIC(CParmUpdateDlg, CParamDlg)
CParmUpdateDlg::CParmUpdateDlg(CRunUpdate* pRun)
   : CParamDlg(CParmUpdateDlg::IDD, NULL)
{
   m_pRun = pRun;
   m_UsedScale   = FALSE;
   //{{AFX_DATA_INIT(CParmUpdateDlg)
   m_AlphaStr = _T("");
   m_hint = -1;
   m_InValStr = _T("");
   m_OutValStr = _T("");
   m_ShiftStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmUpdateDlg)
   DDX_Control(pDX, IDC_SHIFT_SPIN, m_ShiftSpin);
   DDX_Control(pDX, IDC_INVAL_SPIN, m_InValSpin);
   DDX_Control(pDX, IDC_ALPHA_SPIN, m_AlphaSpin);
   DDX_Text(pDX, IDC_ALPHA_EDIT, m_AlphaStr);
   DDX_Radio(pDX, IDC_HINT_0, m_hint);
   DDX_Text(pDX, IDC_INVAL_EDIT, m_InValStr);
   DDX_Text(pDX, IDC_OUTVAL_EDIT, m_OutValStr);
   DDX_Text(pDX, IDC_SHIFT_EDIT, m_ShiftStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmUpdateDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmUpdateDlg)
   ON_EN_CHANGE(IDC_ALPHA_EDIT, OnChange)
   ON_EN_CHANGE(IDC_EDIT_SRC, OnChange)
   ON_BN_CLICKED(IDC_HINT_0, OnChange)
   ON_BN_CLICKED(IDC_HINT_1, OnChange)
   ON_BN_CLICKED(IDC_HINT_2, OnChange)
   ON_EN_CHANGE(IDC_INVAL_EDIT, OnChange)
   ON_EN_CHANGE(IDC_SHIFT_EDIT, OnChange)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmUpdateDlg message handlers

BOOL CParmUpdateDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   m_InValSpin.SetRange(SHRT_MIN, SHRT_MAX);
   m_AlphaSpin.SetRange(SHRT_MIN, SHRT_MAX);
   m_ShiftSpin.SetRange(1, 15);
   m_pRun->SetValue(this);
   UpdateData(FALSE);
   
   return TRUE; 
}

void CParmUpdateDlg::OnChange() 
{
   if (!m_bInit) return;
   UpdateData();
   m_pRun->SetValue(this);
   UpdateData(FALSE);
}
