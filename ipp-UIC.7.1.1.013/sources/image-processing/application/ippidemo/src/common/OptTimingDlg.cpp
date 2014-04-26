/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// OptTimingDlg.cpp : implementation of the Timing Options dialog.
// Command: Menu-Options-Timing
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "demo.h"
#include "OptTimingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptTimingDlg dialog


COptTimingDlg::COptTimingDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COptTimingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(COptTimingDlg)
    m_ShowStatus = FALSE;
    m_ShowHisto = FALSE;
    m_UnitType = 0;
    //}}AFX_DATA_INIT
}


void COptTimingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COptTimingDlg)
    DDX_Control(pDX, IDC_UNIT_0, m_unitRadio[0]);
    DDX_Control(pDX, IDC_UNIT_1, m_unitRadio[1]);
    DDX_Control(pDX, IDC_UNIT_2, m_unitRadio[2]);
    DDX_Control(pDX, IDC_UNIT2_STATIC, m_unitStatic2);

    DDX_Check(pDX, IDC_SHOW_0, m_ShowStatus);
    DDX_Check(pDX, IDC_SHOW_1, m_ShowHisto);
    DDX_Radio(pDX, IDC_UNIT_0, m_UnitType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptTimingDlg, CDialog)
    ON_BN_CLICKED(IDC_SHOW_0, &COptTimingDlg::OnBnClickedShow)
    ON_BN_CLICKED(IDC_SHOW_1, &COptTimingDlg::OnBnClickedShow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptTimingDlg message handlers

BOOL COptTimingDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    EnableUnit();
    return TRUE;
}

void COptTimingDlg::OnOK() 
{
    CDialog::OnOK();
}

void COptTimingDlg::OnBnClickedShow()
{
    UpdateData();
    EnableUnit();
}

void COptTimingDlg::EnableUnit() 
{
    BOOL flag = m_ShowStatus ||  m_ShowHisto;
    for (int i=0; i < 3; i++)
        m_unitRadio[i].EnableWindow(flag);
    m_unitStatic2.EnableWindow(flag);
}
