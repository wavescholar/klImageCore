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
// CParmFFTDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmFFTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFFTDlg dialog


CParmFFTDlg::CParmFFTDlg(CWnd* pParent /*=NULL*/)
   : CParmHintDlg(CParmFFTDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmFFTDlg)
   m_orderX = 0;
   m_orderY = 0;
   m_FlagRadio = -1;
   //}}AFX_DATA_INIT
}


void CParmFFTDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmHintDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFFTDlg)
   DDX_Control(pDX, IDC_STATIC_Y, m_orderYStatic);
   DDX_Control(pDX, IDC_STATIC_X, m_orderXStatic);
   DDX_Control(pDX, IDC_ORDER_Y, m_orderYEdit);
   DDX_Control(pDX, IDC_ORDER_X, m_orderXEdit);
   DDX_Text(pDX, IDC_ORDER_X, m_orderX);
   DDX_Text(pDX, IDC_ORDER_Y, m_orderY);
   DDX_Radio(pDX, IDC_FLAG_0, m_FlagRadio);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_FLAG_0, m_FlagButton[0]);
   DDX_Control(pDX, IDC_FLAG_1, m_FlagButton[1]);
   DDX_Control(pDX, IDC_FLAG_2, m_FlagButton[2]);
}


BEGIN_MESSAGE_MAP(CParmFFTDlg, CParmHintDlg)
   //{{AFX_MSG_MAP(CParmFFTDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFFTDlg message handlers

BOOL CParmFFTDlg::OnInitDialog()
{
   CParmHintDlg::OnInitDialog();
   ShowOrders();
   SetOrders();
   EnableFlags();
   UpdateData(FALSE);
   return TRUE;
}

void CParmFFTDlg::OnOK()
{
   CParmHintDlg::OnOK();
}

static int setOrder(int n, CEdit& edit)
{
   int order;
   for (order = 0; order < 32; order++) {
      n >>= 1;
      if (n == 0) break;
   }
   CMyString str;
   edit.SetWindowText(str << order);
   return order;
}

void CParmFFTDlg::ShowOrders()
{
   if (m_Func.Found("FFT")) return;
   m_orderXEdit.ShowWindow(SW_HIDE);
   m_orderYEdit.ShowWindow(SW_HIDE);
   m_orderXStatic.ShowWindow(SW_HIDE);
   m_orderYStatic.ShowWindow(SW_HIDE);
}

void CParmFFTDlg::SetOrders()
{
   return;
   if (!m_Func.Found("FFT")) return;
   CImage* pSrc = (CImage*)m_pDocSrc;
   CImage* pDst = (CImage*)m_pDocDst;
   IppiRect srcRoi = pSrc->GetActualRoi();
   int x = srcRoi.width;
   int y = srcRoi.height;
   if (pDst) {
      IppiRect dstRoi = pDst->GetActualRoi();
      if (x > dstRoi.width) x = dstRoi.width;
      if (y > dstRoi.height) y = dstRoi.height;
   }
   m_orderX = setOrder(x,m_orderXEdit);
   m_orderY = setOrder(y,m_orderYEdit);
}

void CParmFFTDlg::EnableFlags()
{
   if (m_Func.Found("Fwd"))
      m_FlagButton[0].SetWindowText("Divide Fwd by N");
   else
      m_FlagButton[0].SetWindowText("Divide Inv by N");
}
