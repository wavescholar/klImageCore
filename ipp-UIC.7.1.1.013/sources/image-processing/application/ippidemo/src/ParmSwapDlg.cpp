/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSwapDlg.cpp : implementation of the CParmSwapDlg class.
// CParmSwapDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmSwapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmSwapDlg dialog


CParmSwapDlg::CParmSwapDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmSwapDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmSwapDlg)
   //}}AFX_DATA_INIT
   for (int i=0; i<3; i++)
      m_dstOrder[i] = 0;
}

void CParmSwapDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmSwapDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_CHAN_SPIN_0, m_OrderSpin[0]);
   DDX_Control(pDX, IDC_CHAN_SPIN_1, m_OrderSpin[1]);
   DDX_Control(pDX, IDC_CHAN_SPIN_2, m_OrderSpin[2]);
   DDX_Control(pDX, IDC_CHAN_EDIT_0, m_OrderEdit[0]);
   DDX_Control(pDX, IDC_CHAN_EDIT_1, m_OrderEdit[1]);
   DDX_Control(pDX, IDC_CHAN_EDIT_2, m_OrderEdit[2]);
   DDX_Text(pDX, IDC_CHAN_EDIT_0, m_dstOrder[0]);
   DDX_Text(pDX, IDC_CHAN_EDIT_1, m_dstOrder[1]);
   DDX_Text(pDX, IDC_CHAN_EDIT_2, m_dstOrder[2]);
   DDV_MinMaxInt(pDX, m_dstOrder[0], 0, 2);
   DDV_MinMaxInt(pDX, m_dstOrder[1], 0, 2);
   DDV_MinMaxInt(pDX, m_dstOrder[2], 0, 2);
}


BEGIN_MESSAGE_MAP(CParmSwapDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmSwapDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmSwapDlg message handlers

BOOL CParmSwapDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   for (int i=0; i<3; i++)
      m_OrderSpin[i].SetRange(0, 2);

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
