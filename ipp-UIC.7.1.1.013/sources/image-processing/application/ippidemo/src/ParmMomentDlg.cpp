/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMomentDlg.cpp : implementation of the CParmMomentDlg class.
// CParmMomentDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmMomentDlg.h"
#include "RunMoment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmMomentDlg dialog


CParmMomentDlg::CParmMomentDlg(CRunMoment* pRun)
   : CParamDlg(CParmMomentDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmMomentDlg)
   m_Get = 0;
   m_Channel = 0;
   //}}AFX_DATA_INIT
   for (int i=0; i<10; i++)
      m_MomentStr[i] = "";
}


void CParmMomentDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmMomentDlg)
   DDX_Radio(pDX, IDC_GET_0, m_Get);
   DDX_Radio(pDX, IDC_CHAN_0, m_Channel);
   //}}AFX_DATA_MAP
   DDX_Radio(pDX, IDC_HINT_0, m_Hint);
   DDX_Control(pDX, IDC_CHAN_0, m_ChannelButton[0]);
   DDX_Control(pDX, IDC_CHAN_1, m_ChannelButton[1]);
   DDX_Control(pDX, IDC_CHAN_2, m_ChannelButton[2]);
   DDX_Control(pDX, IDC_CHAN_3, m_ChannelButton[3]);
   DDX_Control(pDX, IDC_MM_0 , m_MomentEdit[0 ]);
   DDX_Control(pDX, IDC_MM_1 , m_MomentEdit[1 ]);
   DDX_Control(pDX, IDC_MM_2 , m_MomentEdit[2 ]);
   DDX_Control(pDX, IDC_MM_3 , m_MomentEdit[3 ]);
   DDX_Control(pDX, IDC_MM_4 , m_MomentEdit[4 ]);
   DDX_Control(pDX, IDC_MM_5 , m_MomentEdit[5 ]);
   DDX_Control(pDX, IDC_MM_6 , m_MomentEdit[6 ]);
   DDX_Control(pDX, IDC_MM_7 , m_MomentEdit[7 ]);
   DDX_Control(pDX, IDC_MM_8 , m_MomentEdit[8 ]);
   DDX_Control(pDX, IDC_MM_9 , m_MomentEdit[9 ]);
   DDX_Text(pDX, IDC_MM_0 , m_MomentStr[0 ]);
   DDX_Text(pDX, IDC_MM_1 , m_MomentStr[1 ]);
   DDX_Text(pDX, IDC_MM_2 , m_MomentStr[2 ]);
   DDX_Text(pDX, IDC_MM_3 , m_MomentStr[3 ]);
   DDX_Text(pDX, IDC_MM_4 , m_MomentStr[4 ]);
   DDX_Text(pDX, IDC_MM_5 , m_MomentStr[5 ]);
   DDX_Text(pDX, IDC_MM_6 , m_MomentStr[6 ]);
   DDX_Text(pDX, IDC_MM_7 , m_MomentStr[7 ]);
   DDX_Text(pDX, IDC_MM_8 , m_MomentStr[8 ]);
   DDX_Text(pDX, IDC_MM_9 , m_MomentStr[9 ]);
}


BEGIN_MESSAGE_MAP(CParmMomentDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmMomentDlg)
   ON_BN_CLICKED(IDC_GET_0, OnGet)
   ON_BN_CLICKED(IDC_HINT_0, OnHint)
   ON_BN_CLICKED(IDC_CHAN_0, OnChan)
   ON_BN_CLICKED(IDC_GET_1, OnGet)
   ON_BN_CLICKED(IDC_GET_2, OnGet)
   ON_BN_CLICKED(IDC_GET_3, OnGet)
   ON_BN_CLICKED(IDC_GET_4, OnGet)
   ON_BN_CLICKED(IDC_HINT_1, OnHint)
   ON_BN_CLICKED(IDC_CHAN_1, OnChan)
   ON_BN_CLICKED(IDC_CHAN_2, OnChan)
   ON_BN_CLICKED(IDC_CHAN_3, OnChan)
   ON_BN_CLICKED(IDC_HINT_2, OnHint)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmMomentDlg message handlers

BOOL CParmMomentDlg::OnInitDialog()
{
   m_TypeStr = m_Func.Found("Moments64f") ? "_64f" : "_64s";
   CParamDlg::OnInitDialog();
   SetWindowText(m_pDocSrc->GetTitle() + " " + m_Func.BaseName());
   SetFunc();
   EnableChannels();
   ShowMoments();
   m_pRun->SetMoments(this);
   UpdateData(FALSE);
   return TRUE;
}

void CParmMomentDlg::OnOK()
{
   CParamDlg::OnOK();
}

#define GET_INDEX(name) { \
   if (funcName.Find(#name) != -1) \
      return name; \
}

int CParmMomentDlg::MomentIndex(CString funcName)
{
   GET_INDEX(NormalizedSpatialMoment);
   GET_INDEX(NormalizedCentralMoment);
   GET_INDEX(SpatialMoment);
   GET_INDEX(CentralMoment);
   GET_INDEX(HuMoments);
   return -1;
}

#define GET_FUNC(name) { \
   if (idx == name) \
      return "ippiGet" #name + m_TypeStr; \
}

CString CParmMomentDlg::MomentFunc(int idx)
{
   GET_FUNC(NormalizedSpatialMoment);
   GET_FUNC(NormalizedCentralMoment);
   GET_FUNC(SpatialMoment);
   GET_FUNC(CentralMoment);
   GET_FUNC(HuMoments);
   return "";
}

void CParmMomentDlg::OnHint()
{
   UpdateData();
   m_pRun->SetMoments(this);
   UpdateData(FALSE);
}

void CParmMomentDlg::OnGet()
{
   UpdateData();
   SetFunc();
   ShowMoments();
   m_pRun->SetMoments(this);
   UpdateData(FALSE);
}

void CParmMomentDlg::OnChan()
{
   UpdateData();
   m_pRun->SetMoments(this);
   UpdateData(FALSE);
}

void CParmMomentDlg::EnableChannels()
{
   int numChannels = m_pDocSrc->GetImage()->Channels();
   if (numChannels == 4) numChannels = 3;
   for (int i=0; i<4; i++)
      m_ChannelButton[i].EnableWindow(i < numChannels);
   if (m_Channel >= numChannels) m_Channel = 0;
}

void CParmMomentDlg::ShowMoments()
{
   int flag = (m_Get == HuMoments) ? SW_HIDE : SW_SHOWNA;
   for (int i=7; i<10; i++)
      m_MomentEdit[i].ShowWindow(flag);
}

void CParmMomentDlg::SetFunc()
{
   m_GetFunc = MomentFunc(m_Get);
}

