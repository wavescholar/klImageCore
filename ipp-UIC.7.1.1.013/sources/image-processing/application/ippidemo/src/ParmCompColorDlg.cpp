/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCompColorDlg.cpp : implementation of the CParmCompColorDlg class.
// CParmCompColorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmCompColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCompColorDlg dialog

CParmCompColorDlg::CParmCompColorDlg(UINT nID, CWnd* pParent /*=NULL*/)
    : CParamDlg(nID, pParent)
{
    //{{AFX_DATA_INIT(CParmCompColorDlg)
    //}}AFX_DATA_INIT
}


void CParmCompColorDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_VAL_0, m_colorKeyEdit[0]);
    DDX_Control(pDX, IDC_VAL_1, m_colorKeyEdit[1]);
    DDX_Control(pDX, IDC_VAL_2, m_colorKeyEdit[2]);
    DDX_Control(pDX, IDC_VAL_3, m_colorKeyEdit[3]);

    DDX_Text(pDX, IDC_VAL_0, m_colorKeyStr[0]);
    DDX_Text(pDX, IDC_VAL_1, m_colorKeyStr[1]);
    DDX_Text(pDX, IDC_VAL_2, m_colorKeyStr[2]);
    DDX_Text(pDX, IDC_VAL_3, m_colorKeyStr[3]);
}


BEGIN_MESSAGE_MAP(CParmCompColorDlg, CParamDlg)
    //{{AFX_MSG_MAP(CParmCompColorDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCompColorDlg message handlers

BOOL CParmCompColorDlg::OnInitDialog()
{
    CParamDlg::OnInitDialog();
    ShowColorKey();
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CParmCompColorDlg::OnOK()
{
    CParamDlg::OnOK();
}

void CParmCompColorDlg::ShowColorKey()
{
    int channels = m_pDocSrc->GetImage()->Channels();
    for (int i=channels; i < 4; i++)
        m_colorKeyEdit[i].ShowWindow(SW_HIDE);
}
