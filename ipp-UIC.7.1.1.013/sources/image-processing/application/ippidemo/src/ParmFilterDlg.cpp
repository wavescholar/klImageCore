/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterDlg.cpp : implementation of the CParmFilterDlg class.
// CParmFilterDlg dialog gets parameters for certain ippIP functions.
//
#include "stdafx.h"/////////////////////////////////////////////////////////////////////////////

#include "ippiDemo.h"
#include "ParmFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterDlg dialog

//MedianFilter

CParmFilterDlg::CParmFilterDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmFilterDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmFilterDlg)
   m_RadioMask = 2;
   //}}AFX_DATA_INIT
}

void CParmFilterDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFilterDlg)
   DDX_Radio(pDX, IDC_RADIO_1x3, m_RadioMask);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_RADIO_1x3, m_MaskButton[msk1x3]);
   DDX_Control(pDX, IDC_RADIO_3x1, m_MaskButton[msk3x1]);
   DDX_Control(pDX, IDC_RADIO_3x3, m_MaskButton[msk3x3]);
   DDX_Control(pDX, IDC_RADIO_1x5, m_MaskButton[msk1x5]);
   DDX_Control(pDX, IDC_RADIO_5x1, m_MaskButton[msk5x1]);
   DDX_Control(pDX, IDC_RADIO_5x5, m_MaskButton[msk5x5]);
}


BEGIN_MESSAGE_MAP(CParmFilterDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFilterDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterDlg message handlers

BOOL CParmFilterDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (m_Func.Found("MedianHoriz")) {
      m_MaskButton[msk1x3].EnableWindow(FALSE);
      m_MaskButton[msk1x5].EnableWindow(FALSE);
      m_MaskButton[msk3x3].EnableWindow(FALSE);
      m_MaskButton[msk5x5].EnableWindow(FALSE);
      if (m_RadioMask == msk1x3 || m_RadioMask == msk3x3)
         m_RadioMask = msk3x1;
      if (m_RadioMask == msk1x5 || m_RadioMask == msk5x5)
         m_RadioMask = msk5x1;
   } else if (m_Func.Found("MedianVert")) {
      m_MaskButton[msk3x1].EnableWindow(FALSE);
      m_MaskButton[msk5x1].EnableWindow(FALSE);
      m_MaskButton[msk3x3].EnableWindow(FALSE);
      m_MaskButton[msk5x5].EnableWindow(FALSE);
      if (m_RadioMask == msk3x1 || m_RadioMask == msk3x3)
         m_RadioMask = msk1x3;
      if (m_RadioMask == msk5x1 || m_RadioMask == msk5x5)
         m_RadioMask = msk1x5;
   } else {
      m_MaskButton[msk1x3].EnableWindow(FALSE);
      m_MaskButton[msk1x5].EnableWindow(FALSE);
      m_MaskButton[msk3x1].EnableWindow(FALSE);
      m_MaskButton[msk5x1].EnableWindow(FALSE);
      if (m_RadioMask == msk1x3 || m_RadioMask == msk3x1)
         m_RadioMask = msk3x3;
      if (m_RadioMask == msk1x5 || m_RadioMask == msk5x1)
         m_RadioMask = msk5x5;
   }
   UpdateData(FALSE);

   return TRUE;
}
