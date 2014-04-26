/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// BorderConstDlg.cpp : implementation of the Set Border Values dialog.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiRun.h"
#include "BorderConstDlg.h"


// CBorderConstDlg dialog

IMPLEMENT_DYNAMIC(CBorderConstDlg, CDialog)

CBorderConstDlg::CBorderConstDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBorderConstDlg::IDD, pParent)
{

}

CBorderConstDlg::~CBorderConstDlg()
{
}

void CBorderConstDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_VAL_0, m_ValueEdit[0]);
    DDX_Control(pDX, IDC_VAL_1, m_ValueEdit[1]);
    DDX_Control(pDX, IDC_VAL_2, m_ValueEdit[2]);
    DDX_Control(pDX, IDC_VAL_3, m_ValueEdit[3]);
    DDX_Control(pDX, IDC_STATIC_0, m_Static[0]);
    DDX_Control(pDX, IDC_STATIC_1, m_Static[1]);
    DDX_Control(pDX, IDC_STATIC_2, m_Static[2]);
    DDX_Control(pDX, IDC_STATIC_3, m_Static[3]);
    DDX_Text(pDX, IDC_VAL_0, m_ValueStr[0]);
    DDX_Text(pDX, IDC_VAL_1, m_ValueStr[1]);
    DDX_Text(pDX, IDC_VAL_2, m_ValueStr[2]);
    DDX_Text(pDX, IDC_VAL_3, m_ValueStr[3]);
}


BEGIN_MESSAGE_MAP(CBorderConstDlg, CDialog)
    ON_BN_CLICKED(IDC_OTHER, &CBorderConstDlg::OnBnClickedOther)
END_MESSAGE_MAP()


// CBorderConstDlg message handlers

BOOL CBorderConstDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
 
    SetWindowText(m_Text);

    for (int i=m_Len; i < 4; i++)
    {
        m_ValueEdit[i].ShowWindow(SW_HIDE);
        m_Static[i].ShowWindow(SW_HIDE);
    }
    return TRUE;
}

void CBorderConstDlg::OnBnClickedOther()
{
    if (CippiRun::DoOptBorderDlg() == IDOK)
        OnOK();
}
