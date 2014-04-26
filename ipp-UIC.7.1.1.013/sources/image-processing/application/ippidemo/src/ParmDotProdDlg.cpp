/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDotProdDlg.cpp : implementation of the CParmDotProdDlg class.
// CParmDotProdDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmDotProdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmDotProdDlg dialog


CParmDotProdDlg::CParmDotProdDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmDotProdDlg::IDD, pParent)
{
    m_Num = 0;
   //{{AFX_DATA_INIT(CParmDotProdDlg)
   //}}AFX_DATA_INIT
}


void CParmDotProdDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_VALUE_0, m_DotProd[0]);
    DDX_Text(pDX, IDC_VALUE_1, m_DotProd[1]);
    DDX_Text(pDX, IDC_VALUE_2, m_DotProd[2]);
    DDX_Text(pDX, IDC_VALUE_3, m_DotProd[3]);
    DDX_Control(pDX, IDC_VALUE_0, m_DotProdEdit[0]);
    DDX_Control(pDX, IDC_VALUE_1, m_DotProdEdit[1]);
    DDX_Control(pDX, IDC_VALUE_2, m_DotProdEdit[2]);
    DDX_Control(pDX, IDC_VALUE_3, m_DotProdEdit[3]);
}


BEGIN_MESSAGE_MAP(CParmDotProdDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmDotProdDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmDotProdDlg message handlers

BOOL CParmDotProdDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();
   ShowDotProd();
   return TRUE;
}

void CParmDotProdDlg::ShowDotProd()
{
    for (int i=m_Num; i<4; i++)
        m_DotProdEdit[i].ShowWindow(FALSE);
}
