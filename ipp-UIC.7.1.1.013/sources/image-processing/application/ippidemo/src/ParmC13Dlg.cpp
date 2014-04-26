/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmC13Dlg.cpp : implementation of the CParmC13Dlg class.
// CParmC13Dlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmC13Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmC13Dlg dialog


CParmC13Dlg::CParmC13Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmC13Dlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmC13Dlg)
   m_srcChannel = -1;
   m_dstChannel = -1;
   //}}AFX_DATA_INIT
}


void CParmC13Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmC13Dlg)
   DDX_Control(pDX, IDC_SRC_STATIC, m_srcChannelStatic);
   DDX_Control(pDX, IDC_DST_STATIC, m_dstChannelStatic);
   DDX_Radio(pDX, IDC_SRC_0, m_srcChannel);
   DDX_Radio(pDX, IDC_DST_0, m_dstChannel);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_SRC_0, m_srcChannelButton[0]);
   DDX_Control(pDX, IDC_SRC_1, m_srcChannelButton[1]);
   DDX_Control(pDX, IDC_SRC_2, m_srcChannelButton[2]);
   DDX_Control(pDX, IDC_SRC_3, m_srcChannelButton[3]);
   DDX_Control(pDX, IDC_DST_0, m_dstChannelButton[0]);
   DDX_Control(pDX, IDC_DST_1, m_dstChannelButton[1]);
   DDX_Control(pDX, IDC_DST_2, m_dstChannelButton[2]);
   DDX_Control(pDX, IDC_DST_3, m_dstChannelButton[3]);
}


BEGIN_MESSAGE_MAP(CParmC13Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmC13Dlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmC13Dlg message handlers

BOOL CParmC13Dlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (m_Func.SrcChannels() == 1) {
      m_srcChannelStatic.EnableWindow(FALSE);
      for (int i=0; i<4; i++)
         m_srcChannelButton[i].EnableWindow(FALSE);
   }
   if (m_Func.DstChannels() == 1) {
      m_dstChannelStatic.EnableWindow(FALSE);
      for (int i=0; i<4; i++)
         m_dstChannelButton[i].EnableWindow(FALSE);
   }
   if (m_Func.SrcChannels() == 3) {
      m_srcChannelButton[3].EnableWindow(FALSE);
      if (m_srcChannel == 3) m_srcChannel = 0;
   }
   if (m_Func.DstChannels() == 3) {
      m_dstChannelButton[3].EnableWindow(FALSE);
      if (m_dstChannel == 3) m_dstChannel = 0;
   }
   UpdateData(FALSE);
   return TRUE;
}

void CParmC13Dlg::OnOK()
{
   CParamDlg::OnOK();
}
