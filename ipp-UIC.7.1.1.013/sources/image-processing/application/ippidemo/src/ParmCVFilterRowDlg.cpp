/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVFilterRowDlg.cpp : implementation of the CParmCVFilterRowDlg class.
// CParmCVFilterRowDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVFilterRowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVFilterRowDlg dialog


CParmCVFilterRowDlg::CParmCVFilterRowDlg(UINT nID)
   : CParmFilterRowDlg(nID)
{
   //{{AFX_DATA_INIT(CParmCVFilterRowDlg)
   m_BorderType = -1;
   //}}AFX_DATA_INIT
}


void CParmCVFilterRowDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFilterRowDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVFilterRowDlg)
   DDX_Radio(pDX, IDC_BORDER_0, m_BorderType);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_VALUE_0, m_ValueStr[0]);
   DDX_Text(pDX, IDC_VALUE_1, m_ValueStr[1]);
   DDX_Text(pDX, IDC_VALUE_2, m_ValueStr[2]);
   DDX_Control(pDX, IDC_VALUE_0, m_ValueEdit[0]);
   DDX_Control(pDX, IDC_VALUE_1, m_ValueEdit[1]);
   DDX_Control(pDX, IDC_VALUE_2, m_ValueEdit[2]);
}


BEGIN_MESSAGE_MAP(CParmCVFilterRowDlg, CParmFilterRowDlg)
   //{{AFX_MSG_MAP(CParmCVFilterRowDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVFilterRowDlg message handlers

BOOL CParmCVFilterRowDlg::OnInitDialog()
{
   CParmFilterRowDlg::OnInitDialog();

   for (int i=0; i<m_Func.SrcChannels(); i++)
      m_ValueEdit[i].EnableWindow();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
