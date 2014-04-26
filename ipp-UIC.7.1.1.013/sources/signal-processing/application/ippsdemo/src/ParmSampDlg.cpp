/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSampDlg.cpp : implementation of the CParmSampDlg class.
// CParmSampDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmSampDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmSampDlg dialog


IMPLEMENT_DYNAMIC(CParmSampDlg, CParamDlg)
CParmSampDlg::CParmSampDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmSampDlg::IDD, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmSampDlg)
   m_phase = 0;
   m_factor = 0;
   //}}AFX_DATA_INIT
}


void CParmSampDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmSampDlg)
   DDX_Control(pDX, IDC_MR_SPIN_1, m_phaseSpin);
   DDX_Control(pDX, IDC_MR_SPIN_0, m_factorSpin);
   DDX_Text(pDX, IDC_MR_EDIT_1, m_phase);
   DDV_MinMaxInt(pDX, m_phase, 0, 999999);
   DDX_Text(pDX, IDC_MR_EDIT_0, m_factor);
   DDV_MinMaxInt(pDX, m_factor, 1, 999999);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmSampDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmSampDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmSampDlg message handlers

BOOL CParmSampDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   m_factorSpin.SetRange(1, 32000);
   m_phaseSpin.SetRange(0, 32000);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmSampDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}
