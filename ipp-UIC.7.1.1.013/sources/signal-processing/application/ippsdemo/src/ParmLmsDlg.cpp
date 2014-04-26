/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmLmsDlg.cpp : implementation of the CParmLmsDlg class.
// CParmLmsDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmLmsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmLmsDlg dialog


IMPLEMENT_DYNAMIC(CParmLmsDlg, CParamDlg)
CParmLmsDlg::CParmLmsDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmLmsDlg)
   m_MuStr = _T("");
   m_MuName = _T("");
   m_TapLenStr = _T("");
   m_DlyTypeDlg = -1;
   //}}AFX_DATA_INIT
}


void CParmLmsDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmLmsDlg)
   DDX_Control(pDX, IDC_LEN_SPIN, m_TapLenSpin);
   DDX_Text(pDX, IDC_MU_EDIT, m_MuStr);
   DDX_Text(pDX, IDC_MU_STATIC, m_MuName);
   DDX_Text(pDX, IDC_LEN_EDIT, m_TapLenStr);
   DDX_Radio(pDX, IDC_DLYTYPE_0, m_DlyTypeDlg);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_DLYTYPE_0, m_DlyTypeButton[0]);
   DDX_Control(pDX, IDC_DLYTYPE_1, m_DlyTypeButton[1]);
}


BEGIN_MESSAGE_MAP(CParmLmsDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmLmsDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmLmsDlg message handlers

BOOL CParmLmsDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   InitMu();
   EnableDly();
   m_TapLenStr.Format("%d", m_tapLen);
   m_TapLenSpin.SetRange(1,32000);
   UpdateData(FALSE);
   return TRUE;
}

void CParmLmsDlg::OnOK() 
{
   UpdateData();
   m_mu.Set(m_MuStr);
   m_tapLen = atoi(m_TapLenStr);
   CParamDlg::OnOK();
}

void CParmLmsDlg::InitMu() 
{
   CFunc func = m_InitName.IsEmpty() ? m_Func : m_InitName;
   if (m_Func.BaseName().Find("Q15") != -1) {
      m_MuName = "MuQ15";
   } else {
      m_MuName = "Mu";
   }
   m_MuStr = m_mu.String();
}

void CParmLmsDlg::EnableDly() 
{
   int flag = m_Func.Inplace() ? SW_HIDE : SW_SHOWNA;
   for (int i=0; i<2; i++)
      m_DlyTypeButton[i].ShowWindow(flag);
}

