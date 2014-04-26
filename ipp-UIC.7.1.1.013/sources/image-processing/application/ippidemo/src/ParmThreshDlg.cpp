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
// CParmThreshDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmThreshDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmThreshDlg dialog


CParmThreshDlg::CParmThreshDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmThreshDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmThreshDlg)
   m_cmpOp = -1;
   //}}AFX_DATA_INIT
   m_bLT = 0;
   m_bGT = 0;
   m_bCmp = 0;
   m_bVal = 0;
}


void CParmThreshDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmThreshDlg)
   DDX_Control(pDX, IDC_VALLT_STATIC, m_ValueLTStatic);
   DDX_Control(pDX, IDC_VALGT_STATIC, m_ValueGTStatic);
   DDX_Control(pDX, IDC_THRESHLT_STATIC, m_ThreshLTStatic);
   DDX_Control(pDX, IDC_THRESHGT_STATIC, m_ThreshGTStatic);
   DDX_Control(pDX, IDC_CMP_STATIC, m_CmpStatic);
   DDX_Radio(pDX, IDC_CMP0, m_cmpOp);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_THRESHGT_0, m_ThreshGTEdit[0]);
   DDX_Control(pDX, IDC_THRESHGT_1, m_ThreshGTEdit[1]);
   DDX_Control(pDX, IDC_THRESHGT_2, m_ThreshGTEdit[2]);
   DDX_Control(pDX, IDC_THRESHGT_3, m_ThreshGTEdit[3]);
   DDX_Control(pDX, IDC_THRESHLT_0, m_ThreshLTEdit[0]);
   DDX_Control(pDX, IDC_THRESHLT_1, m_ThreshLTEdit[1]);
   DDX_Control(pDX, IDC_THRESHLT_2, m_ThreshLTEdit[2]);
   DDX_Control(pDX, IDC_THRESHLT_3, m_ThreshLTEdit[3]);
   DDX_Control(pDX, IDC_VALLT_0, m_ValueLTEdit[0]);
   DDX_Control(pDX, IDC_VALLT_1, m_ValueLTEdit[1]);
   DDX_Control(pDX, IDC_VALLT_2, m_ValueLTEdit[2]);
   DDX_Control(pDX, IDC_VALLT_3, m_ValueLTEdit[3]);
   DDX_Control(pDX, IDC_VALGT_0, m_ValueGTEdit[0]);
   DDX_Control(pDX, IDC_VALGT_1, m_ValueGTEdit[1]);
   DDX_Control(pDX, IDC_VALGT_2, m_ValueGTEdit[2]);
   DDX_Control(pDX, IDC_VALGT_3, m_ValueGTEdit[3]);
   DDX_Text(pDX, IDC_THRESHGT_0, m_ThreshGTStr[0]);
   DDX_Text(pDX, IDC_THRESHGT_1, m_ThreshGTStr[1]);
   DDX_Text(pDX, IDC_THRESHGT_2, m_ThreshGTStr[2]);
   DDX_Text(pDX, IDC_THRESHGT_3, m_ThreshGTStr[3]);
   DDX_Text(pDX, IDC_THRESHLT_0, m_ThreshLTStr[0]);
   DDX_Text(pDX, IDC_THRESHLT_1, m_ThreshLTStr[1]);
   DDX_Text(pDX, IDC_THRESHLT_2, m_ThreshLTStr[2]);
   DDX_Text(pDX, IDC_THRESHLT_3, m_ThreshLTStr[3]);
   DDX_Text(pDX, IDC_VALGT_0, m_ValueGTStr[0]);
   DDX_Text(pDX, IDC_VALGT_1, m_ValueGTStr[1]);
   DDX_Text(pDX, IDC_VALGT_2, m_ValueGTStr[2]);
   DDX_Text(pDX, IDC_VALGT_3, m_ValueGTStr[3]);
   DDX_Text(pDX, IDC_VALLT_0, m_ValueLTStr[0]);
   DDX_Text(pDX, IDC_VALLT_1, m_ValueLTStr[1]);
   DDX_Text(pDX, IDC_VALLT_2, m_ValueLTStr[2]);
   DDX_Text(pDX, IDC_VALLT_3, m_ValueLTStr[3]);
   DDX_Control(pDX, IDC_CMP0, m_CmpButton[0]);
   DDX_Control(pDX, IDC_CMP1, m_CmpButton[1]);
   DDX_Control(pDX, IDC_CMP2, m_CmpButton[2]);
   DDX_Control(pDX, IDC_CMP3, m_CmpButton[3]);
   DDX_Control(pDX, IDC_CMP4, m_CmpButton[4]);
}


BEGIN_MESSAGE_MAP(CParmThreshDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmThreshDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmThreshDlg message handlers

BOOL CParmThreshDlg::OnInitDialog()
{
   m_bLT = m_Func.Found("LT");
   m_bGT = m_Func.Found("GT");
   m_bCmp = FALSE;
   if (!m_bLT && !m_bGT) {
      m_bCmp = TRUE;
      m_bLT  = TRUE;
   }
   m_bVal  = m_Func.Found("Val");
   CParamDlg::OnInitDialog();

   EnableThreshLT();
   EnableThreshGT();
   EnableValueLT();
   EnableValueGT();
   EnableCmp();
   return TRUE;
}

void CParmThreshDlg::OnOK()
{
   CParamDlg::OnOK();
}

void CParmThreshDlg::EnableThreshLT()
{
   if (m_bCmp)
      m_ThreshLTStatic.SetWindowText("Threshold");
   EnableEdit(m_ThreshLTEdit, m_ThreshLTStatic, m_bLT);
}

void CParmThreshDlg::EnableThreshGT()
{
   EnableEdit(m_ThreshGTEdit, m_ThreshGTStatic, m_bGT);
}

void CParmThreshDlg::EnableValueLT()
{
   if (m_bCmp)
      m_ValueLTStatic.SetWindowText("Value");
   EnableEdit(m_ValueLTEdit, m_ValueLTStatic, m_bVal && m_bLT);
}

void CParmThreshDlg::EnableValueGT()
{
   EnableEdit(m_ValueGTEdit, m_ValueGTStatic, m_bVal && m_bGT);
}

void CParmThreshDlg::EnableCmp()
{
   m_CmpStatic.EnableWindow(m_bCmp);
   for (int i=0; i<5; i++)
      m_CmpButton[i].EnableWindow(m_bCmp);
}

void CParmThreshDlg::EnableEdit(CEdit wnd[], CButton& frame, BOOL flag)
{
   frame.EnableWindow(flag);
   int i;
   for (i=0; i<m_Func.SrcChannels() - m_Func.SrcAlpha(); i++)
       wnd[i].EnableWindow(flag);
   for (; i<4; i++)
       wnd[i].ShowWindow(SW_HIDE);
}
