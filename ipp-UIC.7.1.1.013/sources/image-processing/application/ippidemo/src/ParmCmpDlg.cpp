/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCmpDlg.cpp : implementation of the CParmCmpDlg class.
// CParmCmpDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmCmpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCmpDlg dialog


CParmCmpDlg::CParmCmpDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCmpDlg::IDD, pParent)
{
   m_numCoi = 0;
   //{{AFX_DATA_INIT(CParmCmpDlg)
   m_Cmp = -1;
   m_EpsStr = _T("");
   //}}AFX_DATA_INIT
   for (int i=0; i<4; i++)
      m_ValueStr[i] = _T("");
}


void CParmCmpDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCmpDlg)
   DDX_Control(pDX, IDC_EPS, m_EpsEdit);
   DDX_Control(pDX, IDC_VAL_GROUP, m_ValueStatic);
   DDX_Control(pDX, IDC_EPS_STATIC, m_EpsStatic);
   DDX_Control(pDX, IDC_CMP_STATIC, m_CmpStatic);
   DDX_Radio(pDX, IDC_CMP_0, m_Cmp);
   DDX_Text(pDX, IDC_EPS, m_EpsStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_VAL_0, m_ValueEdit[0]);
   DDX_Control(pDX, IDC_VAL_1, m_ValueEdit[1]);
   DDX_Control(pDX, IDC_VAL_2, m_ValueEdit[2]);
   DDX_Control(pDX, IDC_VAL_3, m_ValueEdit[3]);
   DDX_Text(pDX, IDC_VAL_0, m_ValueStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_ValueStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_ValueStr[2]);
   DDX_Text(pDX, IDC_VAL_3, m_ValueStr[3]);
   DDX_Control(pDX, IDC_CMP_0, m_CmpButton[0]);
   DDX_Control(pDX, IDC_CMP_1, m_CmpButton[1]);
   DDX_Control(pDX, IDC_CMP_2, m_CmpButton[2]);
   DDX_Control(pDX, IDC_CMP_3, m_CmpButton[3]);
   DDX_Control(pDX, IDC_CMP_4, m_CmpButton[4]);
}


BEGIN_MESSAGE_MAP(CParmCmpDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCmpDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCmpDlg message handlers

BOOL CParmCmpDlg::OnInitDialog()
{

   CParamDlg::OnInitDialog();

   if (m_UsedVectors & VEC_SRC2) {
      m_ValueStatic.ShowWindow(SW_HIDE);
      for (int i=0; i<4; i++)
         m_ValueEdit[i].ShowWindow(SW_HIDE);
   } else {
      GetDlgItem(IDC_STATIC_SRC2)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_EDIT_SRC2)->ShowWindow(SW_HIDE);
      for (int i=m_numCoi; i<4; i++)
         m_ValueEdit[i].ShowWindow(SW_HIDE);
   }

   if (m_Func.Found("Eps")) {
      m_CmpStatic.ShowWindow(SW_HIDE);
      for (int i=0; i<5; i++)
         m_CmpButton[i].ShowWindow(SW_HIDE);
   } else {
      m_EpsStatic.ShowWindow(SW_HIDE);
      m_EpsEdit.ShowWindow(SW_HIDE);
   }

   return TRUE;
}

void CParmCmpDlg::OnOK()
{
   CParamDlg::OnOK();
}
