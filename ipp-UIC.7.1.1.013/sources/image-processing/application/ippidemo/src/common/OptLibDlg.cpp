/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// OptLibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippiDemo.h"
#include "OptLibDlg.h"


// COptLibDlg dialog

IMPLEMENT_DYNAMIC(COptLibDlg, CDialog)

COptLibDlg::COptLibDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COptLibDlg::IDD, pParent)
    , m_NumThreads(1)
    , m_Optimization(0)
    , m_NumThreadsMode(0)
{

}

COptLibDlg::~COptLibDlg()
{
}

void COptLibDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_THREAD_EDIT, m_NumThreads);
    DDX_Control(pDX, IDC_THREAD_EDIT, m_NumThreadsEdit);
    DDX_Radio(pDX, IDC_OPT_0, m_Optimization);
    DDX_Radio(pDX, IDC_THREAD_RADIO_0, m_NumThreadsMode);
    DDX_Control(pDX, IDC_THREAD_SPIN, m_NumThreadsSpin);
}


BEGIN_MESSAGE_MAP(COptLibDlg, CDialog)
    ON_BN_CLICKED(IDC_OPT_0, &COptLibDlg::OnBnClickedOpt0)
    ON_BN_CLICKED(IDC_OPT_1, &COptLibDlg::OnBnClickedOpt1)
    ON_BN_CLICKED(IDC_THREAD_RADIO_0, &COptLibDlg::OnBnClickedThreadRadio0)
    ON_BN_CLICKED(IDC_THREAD_RADIO_1, &COptLibDlg::OnBnClickedThreadRadio1)
    ON_EN_CHANGE(IDC_THREAD_EDIT, &COptLibDlg::OnEnChangeThreadEdit)
    ON_NOTIFY(UDN_DELTAPOS, IDC_THREAD_SPIN, &COptLibDlg::OnDeltaposThreadSpin)
END_MESSAGE_MAP()


BOOL COptLibDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (m_NumThreadsMode == 0)
        SetDefaultNumThreads();
    m_NumThreadsSpin.SetRange(1,1000);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void COptLibDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    CDialog::OnOK();
}

// COptLibDlg message handlers

void COptLibDlg::OnBnClickedOpt0()
{
}

void COptLibDlg::OnBnClickedOpt1()
{
}

void COptLibDlg::OnBnClickedThreadRadio0()
{
    SetDefaultNumThreads();
}

void COptLibDlg::OnBnClickedThreadRadio1()
{
    m_NumThreadsEdit.SetReadOnly(false);
    m_NumThreadsSpin.EnableWindow(true);
}

void COptLibDlg::SetDefaultNumThreads(void)
{
    m_NumThreadsEdit.SetReadOnly(true);
    m_NumThreadsSpin.EnableWindow(false);
    m_NumThreads = m_NumThreads_Optimal;
    CString text;
    text.Format("%d", m_NumThreads);
    m_NumThreadsEdit.SetWindowTextA(text);
}

void COptLibDlg::OnEnChangeThreadEdit()
{
}

void COptLibDlg::OnDeltaposThreadSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
