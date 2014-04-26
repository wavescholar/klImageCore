/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmTwistDlg.cpp : implementation of the CParmTwistDlg class.
// CParmTwistDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmTwistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RunTwist.h"
enum {TW_COPY   = CRunTwist::TW_COPY  ,
      TW_CYCLE  = CRunTwist::TW_CYCLE ,
      TW_AVER   = CRunTwist::TW_AVER  ,
      TW_EMPH   = CRunTwist::TW_EMPH  ,
      TW_CUSTOM = CRunTwist::TW_CUSTOM,
      TW_NUM    = CRunTwist::TW_NUM   };


/////////////////////////////////////////////////////////////////////////////
// CParmTwistDlg dialog


CParmTwistDlg::CParmTwistDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmTwistDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmTwistDlg)
   m_twistType = -1;
   //}}AFX_DATA_INIT
}


void CParmTwistDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmTwistDlg)
   DDX_Radio(pDX, IDC_TYPE_COPY, m_twistType);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_Twist_00, m_twistStr[0][0]);
   DDX_Text(pDX, IDC_Twist_01, m_twistStr[0][1]);
   DDX_Text(pDX, IDC_Twist_02, m_twistStr[0][2]);
   DDX_Text(pDX, IDC_Twist_03, m_twistStr[0][3]);
   DDX_Text(pDX, IDC_Twist_10, m_twistStr[1][0]);
   DDX_Text(pDX, IDC_Twist_11, m_twistStr[1][1]);
   DDX_Text(pDX, IDC_Twist_12, m_twistStr[1][2]);
   DDX_Text(pDX, IDC_Twist_13, m_twistStr[1][3]);
   DDX_Text(pDX, IDC_Twist_20, m_twistStr[2][0]);
   DDX_Text(pDX, IDC_Twist_21, m_twistStr[2][1]);
   DDX_Text(pDX, IDC_Twist_22, m_twistStr[2][2]);
   DDX_Text(pDX, IDC_Twist_23, m_twistStr[2][3]);
   DDX_Control(pDX, IDC_Twist_00, m_twistEdit[0][0]);
   DDX_Control(pDX, IDC_Twist_01, m_twistEdit[0][1]);
   DDX_Control(pDX, IDC_Twist_02, m_twistEdit[0][2]);
   DDX_Control(pDX, IDC_Twist_03, m_twistEdit[0][3]);
   DDX_Control(pDX, IDC_Twist_10, m_twistEdit[1][0]);
   DDX_Control(pDX, IDC_Twist_11, m_twistEdit[1][1]);
   DDX_Control(pDX, IDC_Twist_12, m_twistEdit[1][2]);
   DDX_Control(pDX, IDC_Twist_13, m_twistEdit[1][3]);
   DDX_Control(pDX, IDC_Twist_20, m_twistEdit[2][0]);
   DDX_Control(pDX, IDC_Twist_21, m_twistEdit[2][1]);
   DDX_Control(pDX, IDC_Twist_22, m_twistEdit[2][2]);
   DDX_Control(pDX, IDC_Twist_23, m_twistEdit[2][3]);
}


BEGIN_MESSAGE_MAP(CParmTwistDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmTwistDlg)
   ON_BN_CLICKED(IDC_TYPE_AVER, OnType)
   ON_BN_CLICKED(IDC_TYPE_COPY, OnType)
   ON_BN_CLICKED(IDC_TYPE_CUSTOM, OnType)
   ON_BN_CLICKED(IDC_TYPE_CYCLE, OnType)
   ON_BN_CLICKED(IDC_TYPE_EMPH, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmTwistDlg message handlers

BOOL CParmTwistDlg::OnInitDialog()
{
   SetTwist();
   CParamDlg::OnInitDialog();
   EnableTwist();
   return TRUE;
}

void CParmTwistDlg::OnType()
{
   UpdateData();
   EnableTwist();
   SetTwist();
   UpdateData(FALSE);
}

void CParmTwistDlg::EnableTwist()
{
   BOOL flag = m_twistType != TW_CUSTOM;
   for (int i=0; i<3; i++)
      for (int j=0; j<4; j++)
         m_twistEdit[i][j].SetReadOnly(flag);
}

void CParmTwistDlg::SetTwist()
{
   switch (m_twistType) {
   case TW_COPY : SetCopy (); break;
   case TW_CYCLE: SetCycle(); break;
   case TW_AVER : SetAver (); break;
   case TW_EMPH : SetEmph (); break;
   }
}

void CParmTwistDlg::SetCopy()
{
   for (int i=0; i<3; i++)
      for (int j=0; j<4; j++)
         m_twistStr[i][j] = "0";
   m_twistStr[0][0] = m_twistStr[1][1] = m_twistStr[2][2] = "1";
}

void CParmTwistDlg::SetCycle()
{
   for (int i=0; i<3; i++)
      for (int j=0; j<4; j++)
         m_twistStr[i][j] = "0";
   m_twistStr[0][1] = m_twistStr[1][2] = m_twistStr[2][0] = "1";
}

void CParmTwistDlg::SetAver()
{
   CString data[3][4] =
   {{"0.75 ", "0.125", "0.125", "0.75",},
    {"0.125", "0.75 ", "0.125", "1.0 ",},
    {"0.125", "0.125", "0.75 ", "0.75",}};
   for (int i=0; i<3; i++)
      for (int j=0; j<4; j++)
         m_twistStr[i][j] = data[i][j];
}

void CParmTwistDlg::SetEmph()
{
   CString data[3][4] =
   {{" 1.4", "-0.2", "-0.2", "-0.4 ",},
    {"-0.2", " 1.4", "-0.2", "-0.55",},
    {"-0.2", "-0.2", " 1.4", "-0.4 ",}};
   for (int i=0; i<3; i++)
      for (int j=0; j<4; j++)
         m_twistStr[i][j] = data[i][j];
}

void CParmTwistDlg::TwistToFloat(float twist[3][4])
{
   for (int i=0; i<3; i++)
      for (int j=0; j<4; j++)
         twist[i][j] = (float)atof(m_twistStr[i][j]);
}

void CParmTwistDlg::GetAver(float twist[3][4])
{
   SetAver();
   TwistToFloat(twist);
}

void CParmTwistDlg::GetEmph(float twist[3][4])
{
   SetEmph();
   TwistToFloat(twist);
}
