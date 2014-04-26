/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFirDlg.cpp : implementation of the CParmFirDlg class.
// CParmFirDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmFirDlg.h"
#include "ippsDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFirDlg dialog


IMPLEMENT_DYNAMIC(CParmFirDlg, CParamDlg)
CParmFirDlg::CParmFirDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   m_UsedScale   = TRUE;
   m_pDocTaps = NULL;
   //{{AFX_DATA_INIT(CParmFirDlg)
   m_TapStr = _T("");
   m_FactorStr = _T("");
   m_TapLen = 0;
   m_TapTitle = _T("");
   m_MR = FALSE;
   //}}AFX_DATA_INIT
}


void CParmFirDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFirDlg)
   DDX_Control(pDX, IDC_MR, m_MRButton);
   DDX_Control(pDX, IDC_TAPS_STATIC, m_TapStatic);
   DDX_Control(pDX, IDC_DOWN_STATIC, m_DownStatic);
   DDX_Control(pDX, IDC_UP_STATIC, m_UpStatic);
   DDX_Control(pDX, IDC_FACTOR_STATIC, m_FactorStatic);
   DDX_Control(pDX, IDC_FACTOR_SPIN, m_FactorSpin);
   DDX_Control(pDX, IDC_FACTOR_EDIT, m_FactorEdit);
   DDX_Text(pDX, IDC_TAPS_EDIT, m_TapStr);
   DDX_Text(pDX, IDC_FACTOR_EDIT, m_FactorStr);
   DDX_Text(pDX, IDC_TAPSLEN_EDIT, m_TapLen);
   DDX_Text(pDX, IDC_TAPS_TITLE, m_TapTitle);
   DDX_Check(pDX, IDC_MR, m_MR);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_MR_STATIC_0, m_MrStatic[0]);
   DDX_Control(pDX, IDC_MR_STATIC_1, m_MrStatic[1]);
   DDX_Control(pDX, IDC_MR_STATIC_2, m_MrStatic[2]);
   DDX_Control(pDX, IDC_MR_STATIC_3, m_MrStatic[3]);
   DDX_Control(pDX, IDC_MR_SPIN_0, m_MrSpin[0]);
   DDX_Control(pDX, IDC_MR_SPIN_1, m_MrSpin[1]);
   DDX_Control(pDX, IDC_MR_SPIN_2, m_MrSpin[2]);
   DDX_Control(pDX, IDC_MR_SPIN_3, m_MrSpin[3]);
   DDX_Control(pDX, IDC_MR_EDIT_0, m_MrEdit[0]);
   DDX_Control(pDX, IDC_MR_EDIT_1, m_MrEdit[1]);
   DDX_Control(pDX, IDC_MR_EDIT_2, m_MrEdit[2]);
   DDX_Control(pDX, IDC_MR_EDIT_3, m_MrEdit[3]);
   DDX_Text(pDX, IDC_MR_EDIT_0, m_MrStr[0]);
   DDX_Text(pDX, IDC_MR_EDIT_1, m_MrStr[1]);
   DDX_Text(pDX, IDC_MR_EDIT_2, m_MrStr[2]);
   DDX_Text(pDX, IDC_MR_EDIT_3, m_MrStr[3]);
}


BEGIN_MESSAGE_MAP(CParmFirDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFirDlg)
   ON_BN_CLICKED(IDC_MR, OnMR)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFirDlg message handlers

BOOL CParmFirDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   InitItems();
   return TRUE;
}

void CParmFirDlg::InitItems() 
{
   ASSERT(m_pDocTaps);
   m_pDocTaps->GetVector()->Get(m_TapStr);
   m_TapTitle = m_pDocTaps->GetTitle();
   m_TapLen = m_pDocTaps->GetVector()->Length();
   m_FactorStr.Format("%d",m_tapsFactor);
   m_FactorSpin.SetRange(-32,32);
   EnableFactor();
   InitMr();
   InitMrSpin();
   EnableMr();
   UpdateMr(FALSE);
   UpdateData(FALSE);
}

void CParmFirDlg::OnOK() 
{
   UpdateData();
   UpdateMr();
   m_tapsFactor = atoi(m_FactorStr);   
   CParamDlg::OnOK();
}

void CParmFirDlg::OnMR() 
{
   UpdateData();
   EnableMr();
}

void CParmFirDlg::EnableFactor() 
{
   int flag = 
      (((m_pDocSrc->GetVector()->Type() | PP_CPLX) == pp16sc) &&
       ((m_pDocTaps->GetVector()->Type() | PP_CPLX) == pp32sc));
   flag = flag ? SW_SHOWNA : SW_HIDE;
   m_FactorStatic.ShowWindow(flag);
   m_FactorEdit.ShowWindow(flag);
   m_FactorSpin.ShowWindow(flag);
}

void CParmFirDlg::InitMr() 
{
   if (m_Func.Found("One") || 
      ((m_pDocSrc->GetVector()->Type() == pp32s) && (m_pDocTaps->GetVector()->Type() == pp32s))) {
      m_MRButton.ShowWindow(SW_HIDE);
      m_MR = FALSE;
   } else if (m_Func.Found("Direct")) {
      m_MRButton.ShowWindow(SW_HIDE);
      m_MR = m_Func.Found("MR");
   }
}

void CParmFirDlg::EnableMr() 
{
   for (int i=0; i<4; i++) {
      m_MrStatic[i].EnableWindow(m_MR);
      m_MrEdit[i].EnableWindow(m_MR);
      m_MrSpin[i].EnableWindow(m_MR);
   }
   m_UpStatic.EnableWindow(m_MR);
   m_DownStatic.EnableWindow(m_MR);
}

void CParmFirDlg::InitMrSpin() 
{
   for (int i=0; i<4; i++)
      m_MrSpin[i].SetRange((i + 1) & 1, 32000);
}

void CParmFirDlg::UpdateMr(BOOL bSave) 
{
   if (bSave) {
      if (m_MR) {
         m_upFactor   = atoi(m_MrStr[0]);
         m_upPhase    = atoi(m_MrStr[1]);
         m_downFactor = atoi(m_MrStr[2]);
         m_downPhase  = atoi(m_MrStr[3]);
      }
   } else {
      m_MrStr[0].Format("%d", m_upFactor  );
      m_MrStr[1].Format("%d", m_upPhase   );
      m_MrStr[2].Format("%d", m_downFactor);
      m_MrStr[3].Format("%d", m_downPhase );
   }
}
