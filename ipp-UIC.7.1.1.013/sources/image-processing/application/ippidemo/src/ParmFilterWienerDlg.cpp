/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterWienerDlg.cpp : implementation of the CParmFilterWienerDlg class.
// CParmFilterWienerDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmFilterWienerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterWienerDlg dialog


CParmFilterWienerDlg::CParmFilterWienerDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParmFilterBoxDlg(CParmFilterWienerDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmFilterWienerDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   for (int i=0; i<4; i++)
      m_ValStr[i] = _T("");
}


void CParmFilterWienerDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFilterBoxDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFilterWienerDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
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


BEGIN_MESSAGE_MAP(CParmFilterWienerDlg, CParmFilterBoxDlg)
   //{{AFX_MSG_MAP(CParmFilterWienerDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterWienerDlg message handlers

BOOL CParmFilterWienerDlg::OnInitDialog()
{
   CParmFilterBoxDlg::OnInitDialog();

   int i;
   for (i=0; i < m_value.Length(); i++)
      m_value.Get(i,m_ValStr[i], "%s,  %s");
   for (; i < 4; i++)
      m_ValEdit[i].ShowWindow(SW_HIDE);
   UpdateData(FALSE);
   return TRUE;
}

void CParmFilterWienerDlg::OnOK()
{
   CParmFilterBoxDlg::OnOK();
   for (int i=0; i < m_value.Length(); i++)
      m_value.Set(i,m_ValStr[i]);
}
