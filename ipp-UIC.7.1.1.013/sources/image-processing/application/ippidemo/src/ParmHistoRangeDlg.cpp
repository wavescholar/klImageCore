/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmHistoRangeDlg.cpp : implementation of the CParmHistoRangeDlg class.
// CParmHistoRangeDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmHistoRangeDlg.h"
#include "RunHistoRange.h"
#include "HistEvenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmHistoRangeDlg dialog

CParmHistoRangeDlg::CParmHistoRangeDlg(CRunHistoRange* pRun, UINT nID)
   : CParamDlg(nID, NULL)
{
   m_pRun = pRun;
   m_bInit = FALSE;
   //{{AFX_DATA_INIT(CParmHistoRangeDlg)
   //}}AFX_DATA_INIT
   for (int idx = 0; idx < 4; idx++) {
      m_ValStr [idx] = _T("");
      m_LevelsStr[idx] = _T("");
      m_LevelsFlag[idx] = 0;
   }
}


void CParmHistoRangeDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmHistoRangeDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_GROUP_STATIC_0, m_GroupStatic[0]);
   DDX_Control(pDX, IDC_GROUP_STATIC_1, m_GroupStatic[1]);
   DDX_Control(pDX, IDC_GROUP_STATIC_2, m_GroupStatic[2]);
   DDX_Control(pDX, IDC_GROUP_STATIC_3, m_GroupStatic[3]);
   DDX_Control(pDX, IDC_VAL_EDIT_0, m_ValEdit[0]);
   DDX_Control(pDX, IDC_VAL_EDIT_1, m_ValEdit[1]);
   DDX_Control(pDX, IDC_VAL_EDIT_2, m_ValEdit[2]);
   DDX_Control(pDX, IDC_VAL_EDIT_3, m_ValEdit[3]);
   DDX_Control(pDX, IDC_VAL_BUTTON_0, m_ValButton[0]);
   DDX_Control(pDX, IDC_VAL_BUTTON_1, m_ValButton[1]);
   DDX_Control(pDX, IDC_VAL_BUTTON_2, m_ValButton[2]);
   DDX_Control(pDX, IDC_VAL_BUTTON_3, m_ValButton[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_0, m_LevelsNumStatic[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_1, m_LevelsNumStatic[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_2, m_LevelsNumStatic[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_STATIC_3, m_LevelsNumStatic[3]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_0, m_LevelsNumEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_1, m_LevelsNumEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_2, m_LevelsNumEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_NUM_EDIT_3, m_LevelsNumEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_EDIT_0, m_LevelsEdit[0]);
   DDX_Control(pDX, IDC_LEVEL_EDIT_1, m_LevelsEdit[1]);
   DDX_Control(pDX, IDC_LEVEL_EDIT_2, m_LevelsEdit[2]);
   DDX_Control(pDX, IDC_LEVEL_EDIT_3, m_LevelsEdit[3]);
   DDX_Control(pDX, IDC_LEVEL_BUTTON_0, m_LevelsButton[0]);
   DDX_Control(pDX, IDC_LEVEL_BUTTON_1, m_LevelsButton[1]);
   DDX_Control(pDX, IDC_LEVEL_BUTTON_2, m_LevelsButton[2]);
   DDX_Control(pDX, IDC_LEVEL_BUTTON_3, m_LevelsButton[3]);
   DDX_Text(pDX, IDC_LEVEL_EDIT_0, m_LevelsStr[0]);
   DDX_Text(pDX, IDC_LEVEL_EDIT_1, m_LevelsStr[1]);
   DDX_Text(pDX, IDC_LEVEL_EDIT_2, m_LevelsStr[2]);
   DDX_Text(pDX, IDC_LEVEL_EDIT_3, m_LevelsStr[3]);
   DDX_Text(pDX, IDC_VAL_EDIT_0, m_ValStr[0]);
   DDX_Text(pDX, IDC_VAL_EDIT_1, m_ValStr[1]);
   DDX_Text(pDX, IDC_VAL_EDIT_2, m_ValStr[2]);
   DDX_Text(pDX, IDC_VAL_EDIT_3, m_ValStr[3]);

   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_0, m_pLUT->m_nLevels[0]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_1, m_pLUT->m_nLevels[1]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_2, m_pLUT->m_nLevels[2]);
   DDX_Text(pDX, IDC_LEVEL_NUM_EDIT_3, m_pLUT->m_nLevels[3]);
}


BEGIN_MESSAGE_MAP(CParmHistoRangeDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmHistoRangeDlg)
   ON_BN_CLICKED(IDC_EVEN, OnEven)
   ON_BN_CLICKED(IDC_LEVEL_BUTTON_0, OnLevelsButton0)
   ON_BN_CLICKED(IDC_LEVEL_BUTTON_1, OnLevelsButton1)
   ON_BN_CLICKED(IDC_LEVEL_BUTTON_2, OnLevelsButton2)
   ON_BN_CLICKED(IDC_LEVEL_BUTTON_3, OnLevelsButton3)
   ON_BN_CLICKED(IDC_VAL_BUTTON_0, OnValButton0)
   ON_BN_CLICKED(IDC_VAL_BUTTON_1, OnValButton1)
   ON_BN_CLICKED(IDC_VAL_BUTTON_2, OnValButton2)
   ON_BN_CLICKED(IDC_VAL_BUTTON_3, OnValButton3)
   ON_EN_CHANGE(IDC_LEVEL_EDIT_0, OnChangeLevelEdit0)
   ON_EN_CHANGE(IDC_LEVEL_EDIT_1, OnChangeLevelEdit1)
   ON_EN_CHANGE(IDC_LEVEL_EDIT_2, OnChangeLevelEdit2)
   ON_EN_CHANGE(IDC_LEVEL_EDIT_3, OnChangeLevelEdit3)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmHistoRangeDlg message handlers

BOOL CParmHistoRangeDlg::OnInitDialog()
{
   ASSERT(m_pLUT);
   CParamDlg::OnInitDialog();

   EnableGroups();
   ShowVal();
   FormatVal(m_pRun->SetValues(this));
   UpdateLevels(FALSE);
   UpdateData(FALSE);
   return TRUE;
}

void CParmHistoRangeDlg::OnOK()
{
   UpdateData();
   UpdateLevels();
   if (!m_pRun->SetValues(this, TRUE))
      return;
   CParamDlg::OnOK();
}

void CParmHistoRangeDlg::OnEven()
{
   UpdateData();

   CHistEvenDlg dlg;
   dlg.m_num = m_pLUT->m_num;
   m_pLUT->m_Lower.Get(dlg.m_LowerStr);
   m_pLUT->m_Upper.Get(dlg.m_UpperStr);
   m_pLUT->GetNumLevels(dlg.m_NumStr);
   if (dlg.DoModal() != IDOK) return;
   m_pLUT->m_Lower.Set(dlg.m_LowerStr);
   m_pLUT->m_Upper.Set(dlg.m_UpperStr);
   m_pLUT->SetNumLevels(dlg.m_NumStr);
   m_pLUT->SetEvenLevels();

   FormatVal(m_pRun->SetValues(this));
   UpdateLevels(FALSE);
   UpdateData(FALSE);
}

void CParmHistoRangeDlg::OnLevelsButton0() { OnLevelsButton(0);}
void CParmHistoRangeDlg::OnLevelsButton1() { OnLevelsButton(1);}
void CParmHistoRangeDlg::OnLevelsButton2() { OnLevelsButton(2);}
void CParmHistoRangeDlg::OnLevelsButton3() { OnLevelsButton(3);}

void CParmHistoRangeDlg::OnValButton0() { OnValButton(0);}
void CParmHistoRangeDlg::OnValButton1() { OnValButton(1);}
void CParmHistoRangeDlg::OnValButton2() { OnValButton(2);}
void CParmHistoRangeDlg::OnValButton3() { OnValButton(3);}

void CParmHistoRangeDlg::OnChangeLevelEdit0() { OnChangeLevelEdit(0);}
void CParmHistoRangeDlg::OnChangeLevelEdit1() { OnChangeLevelEdit(1);}
void CParmHistoRangeDlg::OnChangeLevelEdit2() { OnChangeLevelEdit(2);}
void CParmHistoRangeDlg::OnChangeLevelEdit3() { OnChangeLevelEdit(3);}

void CParmHistoRangeDlg::OnLevelsButton(int idx)
{
   m_LevelsFlag[idx] = 1;
   ShowVal(idx);
   UpdateData(FALSE);
}

void CParmHistoRangeDlg::OnValButton(int idx)
{
   UpdateData();
   m_LevelsFlag[idx] = 0;
   ShowVal(idx);
   UpdateLevels();
   FormatVal(m_pRun->SetValues(this));
   UpdateData(FALSE);
}

void CParmHistoRangeDlg::OnChangeLevelEdit(int idx)
{
   if (!m_bInit) return;
   UpdateData();
   UpdateLevels();
   FormatNumLevels(idx);
}

void CParmHistoRangeDlg::EnableGroups()
{
   if (m_pLUT->m_num == 1)
      m_GroupStatic[0].SetWindowText("");
   for (int idx = m_pLUT->m_num; idx < 4; idx++) {
      m_GroupStatic[idx].EnableWindow(FALSE);
      m_LevelsNumStatic[idx].EnableWindow(FALSE);
      m_LevelsNumEdit[idx].EnableWindow(FALSE);
      m_LevelsButton[idx].EnableWindow(FALSE);
      m_LevelsEdit[idx].EnableWindow(FALSE);
      m_ValButton[idx].EnableWindow(FALSE);
      m_ValEdit[idx].EnableWindow(FALSE);
   }
}

void CParmHistoRangeDlg::ShowVal()
{
   for (int idx = 0; idx < m_pLUT->m_num; idx++)
      ShowVal(idx);
}

void CParmHistoRangeDlg::ShowVal(int idx)
{
   m_LevelsEdit[idx].ShowWindow(m_LevelsFlag[idx] ? SW_SHOWNA : SW_HIDE);
   m_ValEdit[idx]  .ShowWindow((!m_LevelsFlag[idx]) ? SW_SHOWNA : SW_HIDE);
   m_LevelsButton[idx].EnableWindow(!m_LevelsFlag[idx]);
   m_ValButton[idx]   .EnableWindow(m_LevelsFlag[idx]);
}

void CParmHistoRangeDlg::FormatVal(BOOL bValid)
{
   if (bValid)
      m_pLUT->GetLevelsHist(m_ValStr);
   else
      m_pLUT->GetLevels(m_ValStr);
}

void CParmHistoRangeDlg::UpdateLevels(BOOL bSave)
{
   if (bSave)
      m_pLUT->SetLevels(m_LevelsStr);
   else
      m_pLUT->GetLevels(m_LevelsStr);
}

void CParmHistoRangeDlg::FormatNumLevels(int idx)
{
   CMyString str;
   str << m_pLUT->m_Levels[idx].Length();
   m_LevelsNumEdit[idx].SetWindowText(str);
}
