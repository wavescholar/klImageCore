/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCopyManagedDlg.cpp : implementation of the CParmCopyManagedDlg class.
// CParmCopyManagedDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmCopyManagedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCopyManagedDlg dialog


CParmCopyManagedDlg::CParmCopyManagedDlg(UINT nID, CWnd* pParent /*=NULL*/)
    : CParamDlg(nID, pParent)
{
    //{{AFX_DATA_INIT(CParmCopyManagedDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CParmCopyManagedDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmCopyManagedDlg)
    DDX_Check(pDX, IDC_FLAG_0, m_Flag[0]);
    DDX_Check(pDX, IDC_FLAG_1, m_Flag[1]);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCopyManagedDlg, CParamDlg)
    //{{AFX_MSG_MAP(CParmCopyManagedDlg)
        // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCopyManagedDlg message handlers

BOOL CParmCopyManagedDlg::OnInitDialog()
{
    for (int i=0; i<2; i++)
        m_Flag[i] = (m_flags & (1<<i)) ? 1 : 0;

    CParamDlg::OnInitDialog();
    return TRUE;
}

void CParmCopyManagedDlg::OnOK()
{
    CParamDlg::OnOK();

    m_flags = 0;
    for (int i=0; i<2; i++)
        if (m_Flag[i])
            m_flags |= (1<<i);
}
