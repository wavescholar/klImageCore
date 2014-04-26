/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVFloodDlg.cpp : implementation of the CParmCVFloodDlg class.
// CParmCVFloodDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmCVFloodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVFloodDlg dialog


CParmCVFloodDlg::CParmCVFloodDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCVFloodDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmCVFloodDlg)
   m_NewValStr = _T("");
   m_SeedYStr = _T("");
   m_SeedXStr = _T("");
   m_SeedType = -1;
   m_LowValStr = _T("");
   m_UpValStr = _T("");
   //}}AFX_DATA_INIT

}


void CParmCVFloodDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCVFloodDlg)
   DDX_Control(pDX, IDC_UPSTATIC, m_UpValStatic);
   DDX_Control(pDX, IDC_LOWSTATIC, m_LowValStatic);
   DDX_Control(pDX, IDC_UPVAL, m_UpValEdit);
   DDX_Control(pDX, IDC_LOWVAL, m_LowValEdit);
   DDX_Control(pDX, IDC_SEED_Y, m_SeedYEdit);
   DDX_Control(pDX, IDC_SEED_X, m_SeedXEdit);
   DDX_Control(pDX, IDC_SEEDVAL, m_SeedValEdit);
   DDX_Control(pDX, IDC_SEED_SPIN_Y, m_SeedYSpin);
   DDX_Control(pDX, IDC_SEED_SPIN_X, m_SeedXSpin);
   DDX_Control(pDX, IDC_SEEDTYPE_0, m_SeedTypeButton);
   DDX_Text(pDX, IDC_NEWVAL, m_NewValStr);
   DDX_Text(pDX, IDC_SEED_Y, m_SeedYStr);
   DDX_Text(pDX, IDC_SEED_X, m_SeedXStr);
   DDX_Radio(pDX, IDC_SEEDTYPE_0, m_SeedType);
   DDX_Text(pDX, IDC_LOWVAL, m_LowValStr);
   DDX_Text(pDX, IDC_UPVAL, m_UpValStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCVFloodDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCVFloodDlg)
   ON_BN_CLICKED(IDC_SEEDTYPE_0, OnSeedtype)
   ON_BN_CLICKED(IDC_SEEDTYPE_1, OnSeedtype)
   ON_EN_CHANGE(IDC_SEED_X, OnChangeSeed)
   ON_EN_CHANGE(IDC_SEED_Y, OnChangeSeed)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVFloodDlg message handlers

BOOL CParmCVFloodDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   EnableSeedType();
   EnableSeed();
   EnableVal();
   SetSeed();
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmCVFloodDlg::OnOK()
{
   CParamDlg::OnOK();
   GetSeed();
}

void CParmCVFloodDlg::OnSeedtype()
{
   UpdateData();
   EnableSeed();
   SetSeed();
   UpdateData(FALSE);
}

void CParmCVFloodDlg::OnChangeSeed()
{
   if (!m_bInit) return;
   UpdateData();
   FormatSeedVal();
}

void CParmCVFloodDlg::EnableSeedType()
{
   if (!m_pDocSrc->GetImage()->IsCenterShift()) {
      m_SeedTypeButton.EnableWindow(FALSE);
      if (m_SeedType == 0) m_SeedType = 1;
   }
   m_SeedXSpin.SetRange(0,SHRT_MAX);
   m_SeedYSpin.SetRange(0,SHRT_MAX);
}

void CParmCVFloodDlg::EnableSeed()
{
   m_SeedXEdit.EnableWindow(m_SeedType);
   m_SeedYEdit.EnableWindow(m_SeedType);
   m_SeedXSpin.EnableWindow(m_SeedType);
   m_SeedYSpin.EnableWindow(m_SeedType);
}

void CParmCVFloodDlg::EnableVal()
{
   BOOL flag = m_Func.Found("Grad");
   m_LowValEdit.EnableWindow(flag);
   m_UpValEdit.EnableWindow(flag);
   m_LowValStatic.EnableWindow(flag);
   m_UpValStatic.EnableWindow(flag);
}

void CParmCVFloodDlg::SetSeed()
{
   if (m_SeedType == 0) {
      CPoint points[2];
      m_pDocSrc->GetImage()->GetCenterShift(points);
      m_seed.x = points[0].x;
      m_seed.y = points[0].y;
   }
   FormatSeed();
   FormatSeedVal();
}

void CParmCVFloodDlg::GetSeed()
{
   m_seed.x = atoi(m_SeedXStr);
   m_seed.y = atoi(m_SeedYStr);
}

void CParmCVFloodDlg::FormatSeed()
{
   m_SeedXStr.Format("%d", m_seed.x);
   m_SeedYStr.Format("%d", m_seed.y);
}

void CParmCVFloodDlg::FormatSeedVal()
{
   CValue seedVal;
   CString text;
   GetSeed();
   seedVal.Init(m_Func);
   m_pDocSrc->GetImage()->Get(m_seed.x, m_seed.y, 0, seedVal);
   seedVal.Get(text);
   m_SeedValEdit.SetWindowText(text);
}
