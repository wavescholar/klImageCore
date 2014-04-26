/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// COptBorderDlg.cpp : implementation of the Border Behaviour dialog.
// Command: Menu-Options-Border
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "OptBorderDlg.h"


// COptBorderDlg dialog

IMPLEMENT_DYNAMIC(COptBorderDlg, CDialog)

COptBorderDlg::COptBorderDlg(CWnd* pParent /*=NULL*/)
    : CDialog(COptBorderDlg::IDD, pParent)
    , m_Type(0)
{

}

COptBorderDlg::~COptBorderDlg()
{
}

void COptBorderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_BORDER_0, m_Type);
}


BEGIN_MESSAGE_MAP(COptBorderDlg, CDialog)
END_MESSAGE_MAP()


// COptBorderDlg message handlers

BOOL COptBorderDlg::OnInitDialog()
{
    m_Type--;
    CDialog::OnInitDialog();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void COptBorderDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    CDialog::OnOK();
    m_Type++;
}
