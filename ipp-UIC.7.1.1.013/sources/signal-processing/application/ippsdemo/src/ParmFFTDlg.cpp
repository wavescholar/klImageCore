/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFFTDlg.cpp : implementation of the CParmFFTDlg class.
// CParmFFTDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ippsRun.h"
#include "ParmFFTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFFTDlg dialog


IMPLEMENT_DYNAMIC(CParmFFTDlg, CParamDlg)
CParmFFTDlg::CParmFFTDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmFFTDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmFFTDlg)
   m_Order = 0;
   m_Hint = -1;
   //}}AFX_DATA_INIT
}


void CParmFFTDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFFTDlg)
   DDX_Control(pDX, IDC_ORDER_STATIC, m_OrderStatic);
   DDX_Control(pDX, IDC_ORDER_EDIT, m_OrderEdit);
   DDX_Text(pDX, IDC_ORDER_EDIT, m_Order);
   DDX_Radio(pDX, IDC_HINT_0, m_Hint);
   //}}AFX_DATA_MAP
   DDX_Check(pDX, IDC_FLAG_0, m_bFlag[0]);
   DDX_Check(pDX, IDC_FLAG_1, m_bFlag[1]);
   DDX_Check(pDX, IDC_FLAG_2, m_bFlag[2]);
   DDX_Check(pDX, IDC_FLAG_3, m_bFlag[3]);
}


BEGIN_MESSAGE_MAP(CParmFFTDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFFTDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFFTDlg message handlers

BOOL CParmFFTDlg::OnInitDialog() 
{
   SetFlags(m_Flag,m_bFlag,NUM_FLAGS);

   CParamDlg::OnInitDialog();

   if ((m_UsedVectors & VEC_SRC2) == 0) {
      GetDlgItem(IDC_EDIT_SRC2)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_STATIC_SRC2)->ShowWindow(SW_HIDE);
   }
   if (m_Func.Find("DFT") != -1) {
      m_OrderStatic.ShowWindow(SW_HIDE);
      m_OrderEdit.ShowWindow(SW_HIDE);
   }
   SetOrder();
   return TRUE;
}

void CParmFFTDlg::OnOK() 
{
   CParamDlg::OnOK();
   m_Flag = GetFlags(m_bFlag,NUM_FLAGS);
}

void CParmFFTDlg::SetOrder() 
{
   int len = m_pDocSrc->GetVector()->Length();
   if (m_pDocSrc2) {
      int lenB = m_pDocSrc2->GetVector()->Length();
      if (len > lenB) len = lenB;
   }
   for (m_Order = 0; m_Order < 32; m_Order++) {
      len >>= 1;
      if (len == 0) break;
   }
   CMyString str;
   m_OrderEdit.SetWindowText(str << m_Order);
}
