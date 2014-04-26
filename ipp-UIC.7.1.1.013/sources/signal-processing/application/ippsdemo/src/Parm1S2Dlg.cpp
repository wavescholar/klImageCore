/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm1S2Dlg.cpp : implementation of the CParm1S2Dlg class.
// CParm1S2Dlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "Parm1S2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParm1S2Dlg dialog


IMPLEMENT_DYNAMIC(CParm1S2Dlg, CParamDlg)
CParm1S2Dlg::CParm1S2Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParm1S2Dlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParm1S2Dlg)
   magnScaleFactor = 0;
   phaseScaleFactor = 0;
   //}}AFX_DATA_INIT
}


void CParm1S2Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParm1S2Dlg)
   DDX_Control(pDX, IDC_SCALE_SPIN2, m_PhaseFactorSpin);
   DDX_Control(pDX, IDC_SCALE_SPIN1, m_MagnFactorSpin );
   DDX_Text(pDX, IDC_SCALE_EDIT1, magnScaleFactor);
   DDX_Text(pDX, IDC_SCALE_EDIT2, phaseScaleFactor);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParm1S2Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParm1S2Dlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParm1S2Dlg message handlers

BOOL CParm1S2Dlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   m_PhaseFactorSpin.SetRange(-999, 999);
   m_MagnFactorSpin .SetRange(-999, 999);

   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
