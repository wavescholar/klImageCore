/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmStatCountDlg.cpp : implementation of the CParmStatCountDlg class.
// CParmStatCountDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunStatCount.h"
#include "ParmStatCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmStatCountDlg dialog


CParmStatCountDlg::CParmStatCountDlg(CRunStatCount* pRun)
   : CParamDlg(CParmStatCountDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmStatCountDlg)
   //}}AFX_DATA_INIT
}


void CParmStatCountDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmStatCountDlg)
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_X_0, m_xStr[0]);
   DDX_Text(pDX, IDC_X_1, m_xStr[1]);
   DDX_Text(pDX, IDC_X_2, m_xStr[2]);
   DDX_Text(pDX, IDC_Y_0, m_yStr[0]);
   DDX_Text(pDX, IDC_Y_1, m_yStr[1]);
   DDX_Text(pDX, IDC_Y_2, m_yStr[2]);
   DDX_Control(pDX, IDC_X_0, m_xEdit[0]);
   DDX_Control(pDX, IDC_X_1, m_xEdit[1]);
   DDX_Control(pDX, IDC_X_2, m_xEdit[2]);
   DDX_Control(pDX, IDC_Y_0, m_yEdit[0]);
   DDX_Control(pDX, IDC_Y_1, m_yEdit[1]);
   DDX_Control(pDX, IDC_Y_2, m_yEdit[2]);

   DDX_Text(pDX, IDC_VAL_0, m_valueStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_valueStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_valueStr[2]);
   DDX_Control(pDX, IDC_VAL_0, m_valueEdit[0]);
   DDX_Control(pDX, IDC_VAL_1, m_valueEdit[1]);
   DDX_Control(pDX, IDC_VAL_2, m_valueEdit[2]);
}


BEGIN_MESSAGE_MAP(CParmStatCountDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmStatCountDlg)
   ON_EN_CHANGE(IDC_X_0, OnChangeXY)
   ON_EN_CHANGE(IDC_X_1, OnChangeXY)
   ON_EN_CHANGE(IDC_X_2, OnChangeXY)
   ON_EN_CHANGE(IDC_Y_0, OnChangeXY)
   ON_EN_CHANGE(IDC_Y_1, OnChangeXY)
   ON_EN_CHANGE(IDC_Y_2, OnChangeXY)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmStatCountDlg message handlers

BOOL CParmStatCountDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_pRun->SetValues(this);
   m_pRun->UpdateBounds(this,FALSE);
   ShowValues();
   UpdateData(FALSE);
   return TRUE;
}

void CParmStatCountDlg::OnOK()
{
   CParamDlg::OnOK();
}

void CParmStatCountDlg::OnChangeXY()
{
   UpdateData();
   m_pRun->UpdateBounds(this);
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmStatCountDlg::ShowValues()
{
   for (int i = m_Func.SrcChannels(); i < 3; i++) {
      m_xEdit[i].ShowWindow(SW_HIDE);
      m_yEdit[i].ShowWindow(SW_HIDE);
      m_valueEdit[i].ShowWindow(SW_HIDE);
   }
}
