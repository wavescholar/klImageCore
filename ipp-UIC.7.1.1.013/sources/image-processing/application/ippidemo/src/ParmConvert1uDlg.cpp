/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmConvert1uDlg.cpp : implementation of the CParmConvert1uDlg class.
// CParmConvert1uDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ParmConvert1uDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmConvert1uDlg dialog


CParmConvert1uDlg::CParmConvert1uDlg(CWnd* pParent /*=NULL*/)
    : CParamDlg(CParmConvert1uDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CParmConvert1uDlg)
    m_ThreshStr = _T("128");
    //}}AFX_DATA_INIT
}


void CParmConvert1uDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmConvert1uDlg)
    DDX_Control(pDX, IDC_THRESH, m_ThreshEdit);
    DDX_Control(pDX, IDC_STATIC_THRESH, m_ThreshStatic);
    DDX_Control(pDX, IDC_SPIN_THRESH, m_ThreshSpin);
    DDX_Text(pDX, IDC_THRESH, m_ThreshStr);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmConvert1uDlg, CParamDlg)
    //{{AFX_MSG_MAP(CParmConvert1uDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmConvert1uDlg message handlers

BOOL CParmConvert1uDlg::OnInitDialog()
{
    CParamDlg::OnInitDialog();

    if (m_Func.Found("1u8u")) {
        m_ThreshStatic.ShowWindow(SW_HIDE);
        m_ThreshSpin.ShowWindow(SW_HIDE);
        m_ThreshEdit.ShowWindow(SW_HIDE);
    }
    if (m_Func.Found("Convert"))
        m_ThreshSpin.SetRange(0,255);
    else if (m_Func.Found("DeinterlaceFilter"))
        m_ThreshSpin.SetRange32(0,IPP_MAX_16U);
    return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmConvert1uDlg::OnOK()
{
    // TODO: Add extra validation here

    CParamDlg::OnOK();
}
