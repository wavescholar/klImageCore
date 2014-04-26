/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMinMaxDlg.cpp : implementation of the CParmMinMaxDlg class.
// CParmMinMaxDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "ParmMinMaxDlg.h"
#include "RunMinMax.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmMinMaxDlg dialog


IMPLEMENT_DYNAMIC(CParmMinMaxDlg, CParamDlg)
CParmMinMaxDlg::CParmMinMaxDlg(CRunMinMax* pRun)
   : CParamDlg(CParmMinMaxDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmMinMaxDlg)
   m_IsIdx = -1;
   //}}AFX_DATA_INIT
}


void CParmMinMaxDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmMinMaxDlg)
   DDX_Radio(pDX, IDC_FUNC_0, m_IsIdx);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_IM_STATIC_0, m_ImStatic[0]);
   DDX_Control(pDX, IDC_IM_0, m_ImEdit[0]);
   DDX_Control(pDX, IDC_IDX_STATIC_0, m_IdxStatic[0]);
   DDX_Control(pDX, IDC_IDX_0, m_IdxEdit[0]);
   DDX_Text(pDX, IDC_IDX_0, m_IdxStr[0]);
   DDX_Text(pDX, IDC_IM_0, m_ImStr[0]);
   DDX_Text(pDX, IDC_RE_0, m_ReStr[0]);
   DDX_Text(pDX, IDC_RE_STATIC_0, m_ReName[0]);
   DDX_Control(pDX, IDC_IM_STATIC_1, m_ImStatic[1]);
   DDX_Control(pDX, IDC_IM_1, m_ImEdit[1]);
   DDX_Control(pDX, IDC_IDX_STATIC_1, m_IdxStatic[1]);
   DDX_Control(pDX, IDC_IDX_1, m_IdxEdit[1]);
   DDX_Text(pDX, IDC_IDX_1, m_IdxStr[1]);
   DDX_Text(pDX, IDC_IM_1, m_ImStr[1]);
   DDX_Text(pDX, IDC_RE_1, m_ReStr[1]);
   DDX_Text(pDX, IDC_RE_STATIC_1, m_ReName[1]);

}


BEGIN_MESSAGE_MAP(CParmMinMaxDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmMinMaxDlg)
   ON_BN_CLICKED(IDC_FUNC_0, OnFunc)
   ON_BN_CLICKED(IDC_FUNC_1, OnFunc)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmMinMaxDlg message handlers

BOOL CParmMinMaxDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   SetWindowText("MinMax Functions");
   EnableIm();
   EnableIdx();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   return TRUE;
}

void CParmMinMaxDlg::OnFunc() 
{
   UpdateData();
   EnableIdx();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmMinMaxDlg::EnableIm() 
{
   int cplx = m_Func.SrcType() & PP_CPLX; 
   int flag = cplx ? SW_SHOWNA : SW_HIDE;
   for (int i=0; i<2; i++) {
      m_ReName[i] = cplx ? "Re" : "Val";
      m_ImStatic[i].ShowWindow(flag);
      m_ImEdit[i].ShowWindow(flag);
   }
}

void CParmMinMaxDlg::EnableIdx() 
{
   int flag = m_IsIdx ? SW_SHOWNA : SW_HIDE;
   for (int i=0; i<2; i++) {
      m_IdxStatic[i].ShowWindow(flag);
      m_IdxEdit[i].ShowWindow(flag);
   }
}
