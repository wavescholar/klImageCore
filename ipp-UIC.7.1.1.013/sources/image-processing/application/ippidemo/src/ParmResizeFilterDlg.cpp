/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeFilterDlg.cpp : implementation of the CParmResizeFilterDlg class.
// CParmResizeFilterDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmResizeFilterDlg.h"


// CParmResizeFilterDlg dialog

IMPLEMENT_DYNAMIC(CParmResizeFilterDlg, CParmSuperDlg)

CParmResizeFilterDlg::CParmResizeFilterDlg(CWnd* pParent /*=NULL*/)
    : CParmSuperDlg(CParmResizeFilterDlg::IDD, pParent)
{

}

CParmResizeFilterDlg::~CParmResizeFilterDlg()
{
}

void CParmResizeFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CParmSuperDlg::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_FILTER_0, m_filter);
}


BEGIN_MESSAGE_MAP(CParmResizeFilterDlg, CParmSuperDlg)
END_MESSAGE_MAP()


// CParmResizeFilterDlg message handlers

BOOL CParmResizeFilterDlg::OnInitDialog()
{
    CParmSuperDlg::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
