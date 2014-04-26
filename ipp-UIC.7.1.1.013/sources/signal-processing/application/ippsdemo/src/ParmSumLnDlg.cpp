/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSumLnDlg.cpp : implementation of the CParmSumLnDlg class.
// CParmSumLnDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmSumLnDlg.h"
#include "RunSumLn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmSumLnDlg dialog


IMPLEMENT_DYNAMIC(CParmSumLnDlg, CParamDlg)
CParmSumLnDlg::CParmSumLnDlg(CRunSumLn* pRun)
   : CParamDlg(CParmSumLnDlg::IDD, NULL)
{
   m_pRun = pRun;
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmSumLnDlg)
   m_DstType = -1;
   m_ValStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmSumLnDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmSumLnDlg)
   DDX_Control(pDX, IDC_TYPE_STATIC, m_TypeStatic);
   DDX_Radio(pDX, IDC_TYPE_0, m_DstType);
   DDX_Text(pDX, IDC_VALUE, m_ValStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_TYPE_0, m_TypeButton[0]);
   DDX_Control(pDX, IDC_TYPE_1, m_TypeButton[1]);
}


BEGIN_MESSAGE_MAP(CParmSumLnDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmSumLnDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CParmSumLnDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   InitType();
   SetFunc();
   ShowType();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmSumLnDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}

void CParmSumLnDlg::OnType() 
{
   UpdateData();
   SetFunc();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

int CParmSumLnDlg::TypeIndex(CFunc func)
{
   if (m_pDocSrc->GetVector()->Type() & 32) {
      if (func.Found("64f")) return _32f64f;
      return _32f;
   } else {
      return 0;
   }
}

CString CParmSumLnDlg::TypeName(int dstType)
{
   CString name = m_pDocSrc->GetVector()->TypeString();
   if (m_pDocSrc->GetVector()->Type() & 16) {
      name += "32f";
   } else if (m_pDocSrc->GetVector()->Type() & 32) {
      switch (dstType) {
      case _32f64f: name += "64f"; break;
      }
   }
   return name;
}

void CParmSumLnDlg::InitType() 
{
   m_DstType = TypeIndex(m_Func);
}

void CParmSumLnDlg::SetFunc() 
{
   m_Func = "ippsSumLn_" + TypeName(m_DstType);
   SetWindowText(m_Func);
}


void CParmSumLnDlg::ShowTypeButton(int i, CString text) 
{
   m_TypeButton[i].ShowWindow(SW_SHOWNA);
   m_TypeButton[i].SetWindowText(text);
}

void CParmSumLnDlg::ShowType() 
{
   if (m_pDocSrc->GetVector()->Type() & 32) {
      CString typeName[] = {"32f", "64f"};
      for (int i=0; i<2; i++)
         ShowTypeButton(i, typeName[i]);
   } else {
      ShowTypeButton(0, m_Func.TypeToString(m_Func.DstType()));
   }
}

/////////////////////////////////////////////////////////////////////////////
// CParmSumLnDlg message handlers
