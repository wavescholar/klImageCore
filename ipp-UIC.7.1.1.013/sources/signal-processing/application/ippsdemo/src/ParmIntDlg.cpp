/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIntDlg.cpp : implementation of the CParmIntDlg class.
// CParmIntDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmIntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmIntDlg dialog


IMPLEMENT_DYNAMIC(CParmIntDlg, CParamDlg)
CParmIntDlg::CParmIntDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmIntDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   m_MinVal = -1000000;
   m_MaxVal = 1000000;
   //{{AFX_DATA_INIT(CParmIntDlg)
   m_Val = 0;
   m_ValName = _T("");
   //}}AFX_DATA_INIT
}


void CParmIntDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmIntDlg)
   DDX_Control(pDX, IDC_VAL_SPIN, m_ValSpin);
   DDX_Control(pDX, IDC_VAL_EDIT, m_ValEdit);
   DDX_Text(pDX, IDC_VAL_EDIT, m_Val);
   DDX_Text(pDX, IDC_VAL_STATIC, m_ValName);
   //}}AFX_DATA_MAP
   DDV_MinMaxInt(pDX, m_Val, m_MinVal, m_MaxVal);
}


BEGIN_MESSAGE_MAP(CParmIntDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmIntDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmIntDlg message handlers

BOOL CParmIntDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   m_ValSpin.SetRange(m_MinVal,m_MaxVal);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
