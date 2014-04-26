/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmHistoEvenDlg.cpp : implementation of the CParmHistoEvenDlg class.
// CParmHistoEvenDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmHistoEvenDlg.h"
#include "RunHistoEven.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmHistoEvenDlg dialog


CParmHistoEvenDlg::CParmHistoEvenDlg(CRunHistoEven* pRun, UINT nID)
   : CParamDlg(nID, NULL)
{
   m_pRun = pRun;
   m_bInit = FALSE;
   //{{AFX_DATA_INIT(CParmHistoEvenDlg)
   //}}AFX_DATA_INIT
   for (int idx = 0; idx < 4; idx++) {
      m_LevelNumStr[idx] = "";
      m_ValStr[idx]   = "";
      m_UpperStr[idx] = "";
      m_LowerStr[idx] = "";
   }
}


void CParmHistoEvenDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmHistoEvenDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_VAL_TITLE_0, m_ValTitleEdit[0]);
   DDX_Control(pDX, IDC_VAL_TITLE_1, m_ValTitleEdit[1]);
   DDX_Control(pDX, IDC_VAL_TITLE_2, m_ValTitleEdit[2]);
   DDX_Control(pDX, IDC_VAL_TITLE_3, m_ValTitleEdit[3]);
   DDX_Control(pDX, IDC_VAL_EDIT_0, m_ValEdit[0]);
   DDX_Control(pDX, IDC_VAL_EDIT_1, m_ValEdit[1]);
   DDX_Control(pDX, IDC_VAL_EDIT_2, m_ValEdit[2]);
   DDX_Control(pDX, IDC_VAL_EDIT_3, m_ValEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_0, m_LevelNumSpin[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_1, m_LevelNumSpin[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_2, m_LevelNumSpin[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_SPIN_3, m_LevelNumSpin[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_0, m_LevelNumEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_1, m_LevelNumEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_2, m_LevelNumEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_3, m_LevelNumEdit[3]);
   DDX_Control(pDX, IDC_GROUP_STATIC_0, m_GroupStatic[0]);
   DDX_Control(pDX, IDC_GROUP_STATIC_1, m_GroupStatic[1]);
   DDX_Control(pDX, IDC_GROUP_STATIC_2, m_GroupStatic[2]);
   DDX_Control(pDX, IDC_GROUP_STATIC_3, m_GroupStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_0, m_LevelNumStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_1, m_LevelNumStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_2, m_LevelNumStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_3, m_LevelNumStatic[3]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_0, m_LevelNumStr[0]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_1, m_LevelNumStr[1]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_2, m_LevelNumStr[2]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_3, m_LevelNumStr[3]);
   DDX_Text(pDX, IDC_VAL_EDIT_0, m_ValStr[0]);
   DDX_Text(pDX, IDC_VAL_EDIT_1, m_ValStr[1]);
   DDX_Text(pDX, IDC_VAL_EDIT_2, m_ValStr[2]);
   DDX_Text(pDX, IDC_VAL_EDIT_3, m_ValStr[3]);
   DDX_Text(pDX, IDC_VAL_TITLE_0, m_ValTitleStr[0]);
   DDX_Text(pDX, IDC_VAL_TITLE_1, m_ValTitleStr[1]);
   DDX_Text(pDX, IDC_VAL_TITLE_2, m_ValTitleStr[2]);
   DDX_Text(pDX, IDC_VAL_TITLE_3, m_ValTitleStr[3]);

   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_0, m_UpperStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_1, m_UpperStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_2, m_UpperStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_STATIC_3, m_UpperStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_0, m_UpperEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_1, m_UpperEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_2, m_UpperEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_UPPER_EDIT_3, m_UpperEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_0, m_LowerStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_1, m_LowerStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_2, m_LowerStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_STATIC_3, m_LowerStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_0, m_LowerEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_1, m_LowerEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_2, m_LowerEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_LOWER_EDIT_3, m_LowerEdit[3]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_0, m_LowerStr[0]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_1, m_LowerStr[1]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_2, m_LowerStr[2]);
   DDX_Text(pDX, IDC_LEVEL_LOWER_EDIT_3, m_LowerStr[3]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_0, m_UpperStr[0]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_1, m_UpperStr[1]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_2, m_UpperStr[2]);
   DDX_Text(pDX, IDC_LEVEL_UPPER_EDIT_3, m_UpperStr[3]);

}


BEGIN_MESSAGE_MAP(CParmHistoEvenDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmHistoEvenDlg)
   ON_EN_CHANGE(IDC_LEVEL_NUM_EDIT_0, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_NUM_EDIT_1, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_NUM_EDIT_2, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_NUM_EDIT_3, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_UPPER_EDIT_0, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_UPPER_EDIT_1, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_UPPER_EDIT_2, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_UPPER_EDIT_3, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_LOWER_EDIT_0, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_LOWER_EDIT_1, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_LOWER_EDIT_2, OnChangeLevel)
   ON_EN_CHANGE(IDC_LEVEL_LOWER_EDIT_3, OnChangeLevel)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmHistoEvenDlg message handlers

BOOL CParmHistoEvenDlg::OnInitDialog()
{
   ASSERT(m_pLUT);
   CParamDlg::OnInitDialog();

   for (int idx=0; idx<4; idx++) {
      m_LevelNumSpin[idx].SetRange32(2, INT_MAX);
   }
   ShowVal();
   FormatVal(m_pRun->SetValues(this));
   UpdateData(FALSE);
   return TRUE;
}

void CParmHistoEvenDlg::OnOK()
{
   UpdateData();
   if (!m_pRun->SetValues(this, TRUE))  return;
   CParamDlg::OnOK();
}

void CParmHistoEvenDlg::OnChangeLevel() {
   if (!m_bInit) return;
   UpdateData();
   FormatVal(m_pRun->SetValues(this));
   UpdateData(FALSE);
}

void CParmHistoEvenDlg::ShowVal()
{
   int idx;
   for (idx = 0; idx < 4; idx++) {
      m_ValTitleStr[idx] = "Levels\tHist";
   }
   if (m_pLUT->m_num == 1)
      m_GroupStatic[0].SetWindowText("");
   for (idx = m_pLUT->m_num; idx < 4; idx++) {
      m_GroupStatic[idx].EnableWindow(FALSE);
      m_LevelNumStatic[idx].EnableWindow(FALSE);
      m_LevelNumSpin[idx].EnableWindow(FALSE);
      m_LevelNumEdit[idx].EnableWindow(FALSE);
      m_ValTitleEdit[idx].EnableWindow(FALSE);
      m_ValEdit[idx].EnableWindow(FALSE);
      m_UpperStatic[idx].EnableWindow(FALSE);
      m_UpperEdit[idx].EnableWindow(FALSE);
      m_LowerStatic[idx].EnableWindow(FALSE);
      m_LowerEdit[idx].EnableWindow(FALSE);
   }
}

void CParmHistoEvenDlg::FormatVal(BOOL bValid)
{
   if (bValid)
      m_pLUT->GetLevelsHist(m_ValStr);
   else
      for (int i=0; i<4; i++) m_ValStr[i] = "";
}

