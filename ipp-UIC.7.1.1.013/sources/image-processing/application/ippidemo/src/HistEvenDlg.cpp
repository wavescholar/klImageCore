/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// HistEvenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippiDemo.h"
#include "HistEvenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistEvenDlg dialog


CHistEvenDlg::CHistEvenDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CHistEvenDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CHistEvenDlg)
   //}}AFX_DATA_INIT
   for (int i=0; i<4; i++) {
      m_LowerStr[i] = _T("");
      m_NumStr[i] = _T("");
      m_UpperStr[i] = _T("");
   }
   m_num = 0 ;
}


void CHistEvenDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CHistEvenDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_0, m_LowerStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_1, m_LowerStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_2, m_LowerStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_3, m_LowerStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_0, m_LowerEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_1, m_LowerEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_2, m_LowerEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_3, m_LowerEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_0, m_UpperStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_1, m_UpperStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_2, m_UpperStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_3, m_UpperStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_0, m_UpperEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_1, m_UpperEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_2, m_UpperEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_3, m_UpperEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_0, m_NumStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_1, m_NumStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_2, m_NumStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_3, m_NumStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_0, m_NumSpin[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_1, m_NumSpin[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_2, m_NumSpin[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_3, m_NumSpin[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_0, m_NumEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_1, m_NumEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_2, m_NumEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_3, m_NumEdit[3]);
   DDX_Control(pDX, IDC_GROUP_STATIC_0, m_GroupStatic[0]);
   DDX_Control(pDX, IDC_GROUP_STATIC_1, m_GroupStatic[1]);
   DDX_Control(pDX, IDC_GROUP_STATIC_2, m_GroupStatic[2]);
   DDX_Control(pDX, IDC_GROUP_STATIC_3, m_GroupStatic[3]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_0, m_LowerStr[0]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_1, m_LowerStr[1]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_2, m_LowerStr[2]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_3, m_LowerStr[3]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_0, m_NumStr[0]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_1, m_NumStr[1]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_2, m_NumStr[2]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_3, m_NumStr[3]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_0, m_UpperStr[0]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_1, m_UpperStr[1]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_2, m_UpperStr[2]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_3, m_UpperStr[3]);
}


BEGIN_MESSAGE_MAP(CHistEvenDlg, CDialog)
   //{{AFX_MSG_MAP(CHistEvenDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistEvenDlg message handlers

BOOL CHistEvenDlg::OnInitDialog()
{
   CDialog::OnInitDialog();
   int i;
   for (i = 0; i < m_num; i++) {
      m_NumSpin[i].SetRange32(2, INT_MAX);
   }
   for ( ; i < 4; i++) {
      m_UpperStatic[i].EnableWindow(FALSE);
      m_UpperEdit[i].EnableWindow(FALSE);
      m_NumStatic[i].EnableWindow(FALSE);
      m_NumSpin[i].EnableWindow(FALSE);
      m_NumEdit[i].EnableWindow(FALSE);
      m_LowerStatic[i].EnableWindow(FALSE);
      m_LowerEdit[i].EnableWindow(FALSE);
      m_GroupStatic[i].EnableWindow(FALSE);
   }
   return TRUE;
}
