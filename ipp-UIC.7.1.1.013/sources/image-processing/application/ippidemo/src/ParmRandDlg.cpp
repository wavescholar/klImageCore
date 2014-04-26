/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRandDlg.cpp : implementation of the CParmRandDlg class.
// CParmRandDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmRandDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRandDlg dialog


CParmRandDlg::CParmRandDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmRandDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmRandDlg)
   m_SeedStr = _T("");
   //}}AFX_DATA_INIT
   for (int i=0; i<2; i++) {
      m_ParmStr[i] = _T("");
      m_Name[i] = _T("");
   }
}


void CParmRandDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmRandDlg)
   DDX_Control(pDX, IDC_SEED_SPIN, m_SeedSpin);
   DDX_Text(pDX, IDC_SEED_EDIT, m_SeedStr);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_PAR_0, m_ParmStr[0]);
   DDX_Text(pDX, IDC_PAR_1, m_ParmStr[1]);
   DDX_Text(pDX, IDC_STATIC_0, m_Name[0]);
   DDX_Text(pDX, IDC_STATIC_1, m_Name[1]);
}


BEGIN_MESSAGE_MAP(CParmRandDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRandDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRandDlg message handlers

BOOL CParmRandDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (m_Func.Found("Gauss")) {
      m_Name[0] = "Mean";
      m_Name[1] = "Stdev";
   } else if (m_Func.Found("Uniform")) {
      m_Name[0] = "Low";
      m_Name[1] = "High";
   }
   m_SeedSpin.SetRange32(0, INT_MAX);
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmRandDlg::OnOK()
{
   // TODO: Add extra validation here

   CParamDlg::OnOK();
}
