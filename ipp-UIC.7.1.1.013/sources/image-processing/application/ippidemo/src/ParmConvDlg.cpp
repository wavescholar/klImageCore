/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmConvDlg.cpp : implementation of the CParmConvDlg class.
// CParmConvDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmConvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmConvDlg dialog


CParmConvDlg::CParmConvDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmConvDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmConvDlg)
   m_DivStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmConvDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmConvDlg)
   DDX_Control(pDX, IDC_DIV_STATIC, m_DivStatic);
   DDX_Control(pDX, IDC_DIV_SPIN, m_DivSpin);
   DDX_Control(pDX, IDC_DIV_EDIT, m_DivEdit);
   DDX_Text(pDX, IDC_DIV_EDIT, m_DivStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmConvDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmConvDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmConvDlg message handlers

BOOL CParmConvDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (m_Func.SrcType() & PP_FLOAT) {
      m_DivStatic.ShowWindow(SW_HIDE);
      m_DivEdit.ShowWindow(SW_HIDE);
      m_DivSpin.ShowWindow(SW_HIDE);
   } else {
      m_DivSpin.SetRange32(1,INT_MAX);
   }
   return TRUE;
}
