/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmValueDlg.cpp : implementation of the CParmValueDlg class.
// CParmValueDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmValueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmValueDlg dialog

CParmValueDlg::CParmValueDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmValueDlg::IDD, pParent)
{
   m_UsedScale = TRUE;
   //{{AFX_DATA_INIT(CParmValueDlg)
   //}}AFX_DATA_INIT
   for (int i=0; i<4; i++)
      m_ValStr[i] = _T("");
}

void CParmValueDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmValueDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_VAL_0, m_ValEdit[0]);
   DDX_Control(pDX, IDC_VAL_1, m_ValEdit[1]);
   DDX_Control(pDX, IDC_VAL_2, m_ValEdit[2]);
   DDX_Control(pDX, IDC_VAL_3, m_ValEdit[3]);
   DDX_Text(pDX, IDC_VAL_0, m_ValStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_ValStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_ValStr[2]);
   DDX_Text(pDX, IDC_VAL_3, m_ValStr[3]);
}


BEGIN_MESSAGE_MAP(CParmValueDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmValueDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmValueDlg message handlers

BOOL CParmValueDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   int i;
   for (i=0; i < m_value.Length(); i++)
      m_value.Get(i,m_ValStr[i], "%s,  %s");
   for (; i < 4; i++)
      m_ValEdit[i].ShowWindow(SW_HIDE);
   UpdateData(FALSE);
   return TRUE;
}

void CParmValueDlg::OnOK()
{
   CParamDlg::OnOK();
   for (int i=0; i < m_value.Length(); i++)
      m_value.Set(i,m_ValStr[i]);
}
