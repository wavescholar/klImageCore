/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVStatDlg.cpp : implementation of the CParmCVStatDlg class.
// CParmCVStatDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmCVStatDlg.h"
#include "RunCVStat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCVStatDlg dialog


CParmCVStatDlg::CParmCVStatDlg(CRunCVStat* pRun)
   : m_pRun(pRun), CParamDlg(CParmCVStatDlg::IDD, NULL)
{
   //{{AFX_DATA_INIT(CParmCVStatDlg)
   m_funcIndex = -1;
   m_coiStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCVStatDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_X_EDIT_0, m_pIndex[0].x);
   DDX_Text(pDX, IDC_Y_EDIT_0, m_pIndex[0].y);
   DDX_Text(pDX, IDC_X_EDIT_1, m_pIndex[1].x);
   DDX_Text(pDX, IDC_Y_EDIT_1, m_pIndex[1].y);
   //{{AFX_DATA_MAP(CParmCVStatDlg)
   DDX_Control(pDX, IDC_COI_STATIC, m_coiStatic);
   DDX_Control(pDX, IDC_COI_EDIT, m_coiEdit);
   DDX_Radio(pDX, IDC_FUNC_RADIO_0, m_funcIndex);
   DDX_Text(pDX, IDC_COI_EDIT, m_coiStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_FUNC_RADIO_0, m_funcRadio[0]);
   DDX_Control(pDX, IDC_FUNC_RADIO_1, m_funcRadio[1]);
   DDX_Control(pDX, IDC_FUNC_RADIO_2, m_funcRadio[2]);

   DDX_Control(pDX, IDC_Y_STATIC_0, m_yStatic[0]);
   DDX_Control(pDX, IDC_Y_EDIT_0, m_yEdit[0]);
   DDX_Control(pDX, IDC_X_EDIT_0, m_xEdit[0]);
   DDX_Control(pDX, IDC_X_STATIC_0, m_xStatic[0]);
   DDX_Control(pDX, IDC_VAL_STATIC_0, m_valStatic[0]);
   DDX_Control(pDX, IDC_VAL_EDIT_0, m_valEdit[0]);
   DDX_Control(pDX, IDC_INDEX_STATIC_0, m_IndexStatic[0]);
   DDX_Text(pDX, IDC_VAL_EDIT_0, m_valStr[0]);
   DDX_Text(pDX, IDC_VAL_STATIC_0, m_valName[0]);

   DDX_Control(pDX, IDC_Y_STATIC_1, m_yStatic[1]);
   DDX_Control(pDX, IDC_Y_EDIT_1, m_yEdit[1]);
   DDX_Control(pDX, IDC_X_EDIT_1, m_xEdit[1]);
   DDX_Control(pDX, IDC_X_STATIC_1, m_xStatic[1]);
   DDX_Control(pDX, IDC_VAL_STATIC_1, m_valStatic[1]);
   DDX_Control(pDX, IDC_VAL_EDIT_1, m_valEdit[1]);
   DDX_Control(pDX, IDC_INDEX_STATIC_1, m_IndexStatic[1]);
   DDX_Text(pDX, IDC_VAL_EDIT_1, m_valStr[1]);
   DDX_Text(pDX, IDC_VAL_STATIC_1, m_valName[1]);

}


BEGIN_MESSAGE_MAP(CParmCVStatDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCVStatDlg)
   ON_EN_CHANGE(IDC_COI_EDIT, OnChange)
   ON_BN_CLICKED(IDC_FUNC_RADIO_0, OnChange)
   ON_BN_CLICKED(IDC_FUNC_RADIO_1, OnChange)
   ON_BN_CLICKED(IDC_FUNC_RADIO_2, OnChange)
   ON_NOTIFY(UDN_DELTAPOS, IDC_COI_SPIN, OnDeltaposCoiSpin)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCVStatDlg message handlers

BOOL CParmCVStatDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_pRun->SetValues(this);
   ShowValues();
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmCVStatDlg::OnChange()
{
   UpdateData();
   m_pRun->SetValues(this);
   ShowValues();
   SetWindowText(m_Func);
   UpdateData(FALSE);
}

void CParmCVStatDlg::ShowValues()
{
   BOOL isMean = m_Func.Mask();
   m_funcRadio[0].EnableWindow(isMean);

   BOOL isCoi = m_Func.SrcChannels() > 1;
   m_coiStatic.EnableWindow(isCoi);
   m_coiEdit.EnableWindow(isCoi);
   GetDlgItem(IDC_COI_SPIN)->EnableWindow(isCoi);

   BOOL isVal2 = m_funcIndex > 0;
   m_valStatic[1].EnableWindow(isVal2);
   m_valEdit[1].EnableWindow(isVal2);
   if (!isVal2)
      m_valStr[1] = "";

   BOOL isIndx = m_funcIndex == 2;
   for (int i=0; i<2; i++) {
      m_IndexStatic[i].EnableWindow(isIndx);
      m_xStatic[i].EnableWindow(isIndx);
      m_xEdit[i].EnableWindow(isIndx);
      m_yStatic[i].EnableWindow(isIndx);
      m_yEdit[i].EnableWindow(isIndx);
   }
   SetNames();
}

void CParmCVStatDlg::SetNames()
{
   switch (m_funcIndex) {
   case 0:
      m_valName[0] = "Mean";
      m_valName[1] = "";
      break;
   case 1:
      m_valName[0] = "Mean";
      m_valName[1] = "StdDev";
      break;
   case 2:
      m_valName[0] = "Min";
      m_valName[1] = "Max";
      break;
   }
}

void CParmCVStatDlg::OnDeltaposCoiSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
   NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
   UpdateData();
   int coi = atoi(m_coiStr);
   coi -= pNMUpDown->iDelta;
   if (coi < 1) coi = 1;
   if (coi > 3) coi = 3;
   m_coiStr.Format("%d", coi);
   UpdateData(FALSE);
   OnChange();
   *pResult = 0;
}
