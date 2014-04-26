/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmThreshDlg.cpp : implementation of the CParmThreshDlg class.
// CParmThreshDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmThreshDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmThreshDlg dialog


IMPLEMENT_DYNAMIC(CParmThreshDlg, CParamDlg)
CParmThreshDlg::CParmThreshDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmThreshDlg::IDD, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmThreshDlg)
   m_Cmp = -1;
   m_LevelStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmThreshDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmThreshDlg)
   DDX_Radio(pDX, IDC_CMP_0, m_Cmp);
   DDX_Text(pDX, IDC_LEVEL, m_LevelStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_CMP_0, m_CmpWnd[0]);
   DDX_Control(pDX, IDC_CMP_1, m_CmpWnd[1]);
   DDX_Control(pDX, IDC_CMP_STATIC, m_CmpWnd[2]);

   DDX_Text(pDX, IDC_VALUE_RE, m_ValueStr[0]);
   DDX_Text(pDX, IDC_VALUE_IM, m_ValueStr[1]);

   DDX_Control(pDX, IDC_VALUE_RE, m_ValueEdit[0]);
   DDX_Control(pDX, IDC_VALUE_IM, m_ValueEdit[1]);
   DDX_Control(pDX, IDC_STATIC_RE, m_ValueStatic[0]);
   DDX_Control(pDX, IDC_STATIC_IM, m_ValueStatic[1]);
   DDX_Control(pDX, IDC_VALUE_STATIC, m_ValueStatic[2]);
}


BEGIN_MESSAGE_MAP(CParmThreshDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmThreshDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmThreshDlg message handlers

BOOL CParmThreshDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   BOOL bValue = m_Func.Found("Val");
   BOOL bCmp = m_Func.BaseName() == "Threshold";
   BOOL bCplx = m_Func.SrcType() & PP_CPLX;

   int flag;
   int i;
   flag = bValue ? SW_SHOWNA : SW_HIDE;
   for (i=0; i<3; i++) m_ValueStatic[i].ShowWindow(flag);
   for (i=0; i<2; i++) m_ValueEdit[i].ShowWindow(flag);

   flag = bCmp ? SW_SHOWNA : SW_HIDE;
   for (i=0; i<3; i++) m_CmpWnd[i].ShowWindow(flag);

   m_ValueStatic[1].EnableWindow(bCplx);
   m_ValueEdit[1].EnableWindow(bCplx);
   return TRUE;
}

void CParmThreshDlg::OnOK() 
{
   CParamDlg::OnOK();
}
