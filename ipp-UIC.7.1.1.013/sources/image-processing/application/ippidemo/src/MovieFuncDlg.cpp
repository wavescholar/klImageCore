/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MovieFuncDlg.cpp : implementation of the Customize Movie dialog.
// Command: Menu-Movie-Customize
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippiDemo.h"
#include "Movie.h"
#include "MovieMask.h"
#include "MovieFuncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovieFuncDlg dialog


CMovieFuncDlg::CMovieFuncDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMovieFuncDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMovieFuncDlg)
    m_Speed = 0;
    m_MaskIdx = -1;
    //}}AFX_DATA_INIT
}


void CMovieFuncDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMovieFuncDlg)
    DDX_Control(pDX, IDC_SPEED, m_SpeedSlider);
    DDX_Control(pDX, IDC_FUNC_LIST, m_FuncBox);
    DDX_Slider(pDX, IDC_SPEED, m_Speed);
    DDX_Radio(pDX, IDC_MASK_0, m_MaskIdx);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_SPEED_PERF, m_SpeedEditPerf);
    DDX_Control(pDX, IDC_SPEED_OTHER, m_SpeedEditRythm);
}


BEGIN_MESSAGE_MAP(CMovieFuncDlg, CDialog)
    //{{AFX_MSG_MAP(CMovieFuncDlg)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_SELECT_ALL, &CMovieFuncDlg::OnBnClickedSelectAll)
    ON_BN_CLICKED(IDC_DESELECT_ALL, &CMovieFuncDlg::OnBnClickedDeselectAll)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SPEED, &CMovieFuncDlg::OnNMCustomdrawSpeed)
    ON_BN_CLICKED(IDC_SELECT_PERF, &CMovieFuncDlg::OnBnClickedSelectPerf)
//    ON_EN_CHANGE(IDC_SPEED_PERF, &CMovieFuncDlg::OnEnChangeSpeedPerf)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovieFuncDlg message handlers

BOOL CMovieFuncDlg::OnInitDialog()
{
    InitMask();

    CDialog::OnInitDialog();

    InitFuncBox();
    InitSpeed();

    return TRUE;
}


void CMovieFuncDlg::OnOK()
{
    CDialog::OnOK();
    SaveFunctions();
    SaveMask();
    SaveSpeed();
}

void CMovieFuncDlg::InitFuncBox()
{
//    LONG style = GetWindowLong(m_FuncBox.m_hWnd, GWL_STYLE);
//    style &= ~(LVS_ICON | LVS_LIST | LVS_REPORT | LVS_SMALLICON);
//    style |= LVS_LIST;
//    SetWindowLong(m_FuncBox.m_hWnd, GWL_STYLE, style);
    m_FuncBox.SetExtendedStyle(LVS_EX_CHECKBOXES);
    CMovieFuncArray* pFullArr = CMovie::GetFullArray();
    for (int i=0; i<pFullArr->GetSize(); i++) {
        CMovieFunc func = pFullArr->GetAt(i);
        int index = m_FuncBox.InsertItem(i, func);
        SetCheckState(index, func.bHot);
    }
}

void CMovieFuncDlg::SaveFunctions()
{
    CMovie::ClearFunctions();
    for (int i=0; i<m_FuncBox.GetItemCount(); i++) {
        CMovie::AddFunction(m_FuncBox.GetItemText(i,0),
                                  GetCheckState(i));
    }
}

void CMovieFuncDlg::InitMask()
{
    CMovieMaskArray* pMaskArr = CMovie::GetMaskArray();
    m_MaskIdx = pMaskArr->GetCurIndex();
}

void CMovieFuncDlg::SaveMask()
{
    CMovieMaskArray* pMaskArr = CMovie::GetMaskArray();
    pMaskArr->SetCurIndex(m_MaskIdx);
//    pMaskArr->ElementAt(m_MaskIdx).Load();
}

void CMovieFuncDlg::InitSpeed()
{
    int min = 0;
    int max = CMovie::GetMaxCycle() - CMovie::GetMinCycle();
    m_SpeedSlider.SetRange(min,max);
    m_SpeedSlider.SetTicFreq(CMovie::GetMinCycle());
    m_SpeedSlider.SetPos(CMovie::GetMaxCycle() - CMovie::GetCycle());
}

void CMovieFuncDlg::SaveSpeed()
{
    int speed = CMovie::GetMaxCycle() - m_SpeedSlider.GetPos();
    CMovie::SetCycle(speed);
}

BOOL CMovieFuncDlg::GetCheckState(int i)
{
    ASSERT(m_FuncBox.m_hWnd);
    return ListView_GetCheckState(m_FuncBox.m_hWnd, i);
}

#ifndef ListView_SetCheckState
    #define ListView_SetCheckState(hwndLV, i, fCheck) \
        ListView_SetItemState(hwndLV, i, \
        INDEXTOSTATEIMAGEMASK((fCheck)+1), LVIS_STATEIMAGEMASK)
#endif

void CMovieFuncDlg::SetCheckState(int i, BOOL state)
{
    ASSERT(m_FuncBox.m_hWnd);
    ListView_SetCheckState(m_FuncBox.m_hWnd, i, state);
}

void CMovieFuncDlg::OnBnClickedSelectAll()
{
    for (int i=0; i<m_FuncBox.GetItemCount(); i++) {
        SetCheckState(i, TRUE);
    }
}

void CMovieFuncDlg::OnBnClickedDeselectAll()
{
    for (int i=0; i<m_FuncBox.GetItemCount(); i++) {
        SetCheckState(i, FALSE);
    }
}

void CMovieFuncDlg::OnBnClickedSelectPerf()
{
    CMovieFuncArray* pFullArr = CMovie::GetFullArray();
    for (int i=0; i<pFullArr->GetSize(); i++) {
        CMovieFunc func = pFullArr->GetAt(i);
        SetCheckState(i, func.bPerf);
    }
}

void CMovieFuncDlg::OnNMCustomdrawSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    int speed = CMovie::GetMaxCycle() - m_SpeedSlider.GetPos();
    CString text;
    int perfRate = CMovie::GetPerfCycle(speed);
    double rythmRate = CMovie::GetRythmTimeSec(speed);
    text.Format("%d", perfRate); 
    m_SpeedEditPerf.SetWindowTextA(text);
    text.Format("%.1f", rythmRate); 
    m_SpeedEditRythm.SetWindowTextA(text);

    *pResult = 0;
}

