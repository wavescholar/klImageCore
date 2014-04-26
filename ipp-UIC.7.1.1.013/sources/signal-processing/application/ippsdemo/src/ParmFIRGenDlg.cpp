/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFIRGenDlg.cpp : implementation of the CParmFIRGenDlg class.
// CParmFIRGenDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "ParmFIRGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFIRGenDlg dialog


IMPLEMENT_DYNAMIC(CParmFIRGenDlg, CParamDlg)
CParmFIRGenDlg::CParmFIRGenDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmFIRGenDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmFIRGenDlg)
   m_doNormal = FALSE;
   m_winType = -1;
   //}}AFX_DATA_INIT
   m_FreqStr[0] = _T("");
   m_FreqStr[1] = _T("");
}


void CParmFIRGenDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFIRGenDlg)
   DDX_Check(pDX, IDC_NORMAL, m_doNormal);
   DDX_Radio(pDX, IDC_TYPE_0, m_winType);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_FREQ_STATIC_0, m_FreqStatic[0]);
   DDX_Control(pDX, IDC_FREQ_EDIT_0, m_FreqEdit[0]);
   DDX_Text(pDX, IDC_FREQ_EDIT_0, m_FreqStr[0]);
   DDX_Control(pDX, IDC_FREQ_STATIC_1, m_FreqStatic[1]);
   DDX_Control(pDX, IDC_FREQ_EDIT_1, m_FreqEdit[1]);
   DDX_Text(pDX, IDC_FREQ_EDIT_1, m_FreqStr[1]);
}


BEGIN_MESSAGE_MAP(CParmFIRGenDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFIRGenDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFIRGenDlg message handlers

BOOL CParmFIRGenDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   int idx = -1;
   if (m_Func.Found("Lowpass")) idx = 1;
   if (m_Func.Found("Highpass")) idx = 0;
   if (idx >= 0) {
      m_FreqStatic[idx].ShowWindow(SW_HIDE);
      m_FreqEdit[idx].ShowWindow(SW_HIDE);
   }
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
