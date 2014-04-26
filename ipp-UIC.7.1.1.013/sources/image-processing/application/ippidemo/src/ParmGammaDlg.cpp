/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmGammaDlg.cpp : implementation of the CParmGammaDlg class.
// CParmGammaDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmGammaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmGammaDlg dialog


CParmGammaDlg::CParmGammaDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmGammaDlg::IDD, pParent)
{
   m_is1 = TRUE;
   //{{AFX_DATA_INIT(CParmGammaDlg)
   m_Str0 = _T("");
   m_Str1 = _T("");
   m_Name0 = _T("Val Min");
   m_Name1 = _T("Val Max");
   //}}AFX_DATA_INIT
}


void CParmGammaDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmGammaDlg)
   DDX_Control(pDX, IDC_STATIC_1, m_Static1);
   DDX_Control(pDX, IDC_VAL_1, m_Edit1);
   DDX_Text(pDX, IDC_VAL_0, m_Str0);
   DDX_Text(pDX, IDC_VAL_1, m_Str1);
   DDX_Text(pDX, IDC_STATIC_0, m_Name0);
   DDX_Text(pDX, IDC_STATIC_1, m_Name1);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmGammaDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmGammaDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmGammaDlg message handlers

BOOL CParmGammaDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (!m_is1) {
      m_Edit1.ShowWindow(SW_HIDE);
      m_Static1.ShowWindow(SW_HIDE);
   }
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
