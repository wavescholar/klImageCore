/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCDlg.cpp : implementation of the CParmCDlg class.
// CParmCDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCDlg dialog


IMPLEMENT_DYNAMIC(CParmCDlg, CParamDlg)
CParmCDlg::CParmCDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmCDlg)
   m_ImStr = _T("");
   m_ReStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCDlg)
   DDX_Control(pDX, IDC_VALUE_RE, m_ReEdit);
   DDX_Control(pDX, IDC_VALUE_IM, m_ImEdit);
   DDX_Control(pDX, IDC_VALUE_STATIC, m_ValButton);
   DDX_Control(pDX, IDC_STATIC_IM, m_ImStatic);
   DDX_Control(pDX, IDC_STATIC_RE, m_ReStatic);
   DDX_Text(pDX, IDC_VALUE_IM, m_ImStr);
   DDX_Text(pDX, IDC_VALUE_RE, m_ReStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCDlg message handlers

BOOL CParmCDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   BOOL flag = (int)(m_Func.DstType()) & PP_CPLX ? TRUE : FALSE;
   m_ImEdit.EnableWindow(flag);
   m_ImStatic.EnableWindow(flag);

   if (m_Func.Found("Jaehne"))
      m_ValButton.SetWindowText("Magn");
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmCDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}
