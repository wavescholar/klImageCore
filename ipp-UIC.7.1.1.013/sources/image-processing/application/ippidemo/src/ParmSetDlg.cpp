/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSetDlg.cpp : implementation of the CParmSetDlg class.
// CParmSetDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmSetDlg dialog

CParmSetDlg::CParmSetDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmSetDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmSetDlg)
   m_channel = -1;
   //}}AFX_DATA_INIT
   for (int i=0; i<4; i++)
      m_ValStr[i] = _T("");
}

void CParmSetDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmSetDlg)
   DDX_Control(pDX, IDC_CHAN_STATIC, m_ChanStatic);
   DDX_Control(pDX, IDC_VAL_NAME, m_ValName);
   DDX_Control(pDX, IDC_VAL_GROUP, m_ValGroup);
   DDX_Radio(pDX, IDC_CHAN_0, m_channel);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_VAL_0, m_ValEdit[0]);
   DDX_Control(pDX, IDC_VAL_1, m_ValEdit[1]);
   DDX_Control(pDX, IDC_VAL_2, m_ValEdit[2]);
   DDX_Control(pDX, IDC_VAL_3, m_ValEdit[3]);
   DDX_Text(pDX, IDC_VAL_0, m_ValStr[0]);
   DDX_Text(pDX, IDC_VAL_1, m_ValStr[1]);
   DDX_Text(pDX, IDC_VAL_2, m_ValStr[2]);
   DDX_Text(pDX, IDC_VAL_3, m_ValStr[3]);
   DDX_Control(pDX, IDC_CHAN_0, m_ChannelButton[0]);
   DDX_Control(pDX, IDC_CHAN_1, m_ChannelButton[1]);
   DDX_Control(pDX, IDC_CHAN_2, m_ChannelButton[2]);
   DDX_Control(pDX, IDC_CHAN_3, m_ChannelButton[3]);
}


BEGIN_MESSAGE_MAP(CParmSetDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmSetDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmSetDlg message handlers

BOOL CParmSetDlg::OnInitDialog()
{
   int i;
   for (i=0; i < m_value.Length(); i++)
      m_value.Get(i,m_ValStr[i]);

   CParamDlg::OnInitDialog();

   m_StaticSrc.SetWindowText(m_Func.Inplace() ? "SrcDst" : "Dst");
   if (!(m_UsedVectors & VEC_MASK)) {
      GetDlgItem(IDC_STATIC_MASK)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_EDIT_MASK)->ShowWindow(SW_HIDE);
   }

   if (m_value.Length() == 1)
      m_ValGroup.ShowWindow(SW_HIDE);
   else
      m_ValName.ShowWindow(SW_HIDE);
   if (m_pDocSrc->GetImage()->Channels() == 1 ||
       m_pDocSrc->GetImage()->GetCoi() >= 0 ||
       m_value.Length() > 1) {
      m_ChanStatic.ShowWindow(SW_HIDE);
      for (i=0; i < 4; i++)
         m_ChannelButton[i].ShowWindow(SW_HIDE);
   } else {
      for (i=m_pDocSrc->GetImage()->Channels(); i < 4; i++)
         m_ChannelButton[i].ShowWindow(SW_HIDE);
   }
   for (i=m_value.Length(); i < 4; i++)
      m_ValEdit[i].ShowWindow(SW_HIDE);

   return TRUE;
}

void CParmSetDlg::OnOK()
{
   CParamDlg::OnOK();
   for (int i=0; i < m_value.Length(); i++)
      m_value.Set(i,m_ValStr[i]);
}
