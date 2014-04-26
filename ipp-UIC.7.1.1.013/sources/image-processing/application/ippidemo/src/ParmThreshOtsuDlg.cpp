/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmThreshOtsuDlg.cpp : implementation of the CParmThreshOtsuDlg class.
// CParmThreshOtsuDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmThreshOtsuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmThreshOtsuDlg dialog


CParmThreshOtsuDlg::CParmThreshOtsuDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmThreshOtsuDlg::IDD, pParent)
{
    m_Num = 0;
   //{{AFX_DATA_INIT(CParmThreshOtsuDlg)
   //}}AFX_DATA_INIT
}


void CParmThreshOtsuDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_VALUE_0, m_Threshold[0]);
    DDX_Text(pDX, IDC_VALUE_1, m_Threshold[1]);
    DDX_Text(pDX, IDC_VALUE_2, m_Threshold[2]);
    DDX_Text(pDX, IDC_VALUE_3, m_Threshold[3]);
    DDX_Control(pDX, IDC_VALUE_0, m_ThresholdEdit[0]);
    DDX_Control(pDX, IDC_VALUE_1, m_ThresholdEdit[1]);
    DDX_Control(pDX, IDC_VALUE_2, m_ThresholdEdit[2]);
    DDX_Control(pDX, IDC_VALUE_3, m_ThresholdEdit[3]);
}


BEGIN_MESSAGE_MAP(CParmThreshOtsuDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmThreshOtsuDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmThreshOtsuDlg message handlers

BOOL CParmThreshOtsuDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();
   ShowThreshold();
   return TRUE;
}

void CParmThreshOtsuDlg::ShowThreshold()
{
    for (int i=m_Num; i<4; i++)
        m_ThresholdEdit[i].ShowWindow(FALSE);
}
