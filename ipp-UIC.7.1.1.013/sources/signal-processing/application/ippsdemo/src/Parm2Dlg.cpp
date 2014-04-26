/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm2Dlg.cpp : implementation of the CParm2Dlg class.
// CParm2Dlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Parm2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParm2Dlg dialog


IMPLEMENT_DYNAMIC(CParm2Dlg, CParamDlg)
CParm2Dlg::CParm2Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParm2Dlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParm2Dlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParm2Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParm2Dlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParm2Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParm2Dlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParm2Dlg message handlers

BOOL CParm2Dlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   if (m_nModalResult == IDCANCEL) return TRUE;
   if (!m_Func.Scale()) OnOK();
   return TRUE;
}

void CParm2Dlg::OnOK() 
{
   CParamDlg::OnOK();
}
