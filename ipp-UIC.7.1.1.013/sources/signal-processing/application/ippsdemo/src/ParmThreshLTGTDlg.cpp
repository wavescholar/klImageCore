/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmThreshLTGTDlg.cpp : implementation of the CParmThreshLTGTDlg class.
// CParmThreshLTGTDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "ParmThreshLTGTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmThreshLTGTDlg dialog


IMPLEMENT_DYNAMIC(CParmThreshLTGTDlg, CParamDlg)
CParmThreshLTGTDlg::CParmThreshLTGTDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmThreshLTGTDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmThreshLTGTDlg)
   //}}AFX_DATA_INIT
}


void CParmThreshLTGTDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmThreshLTGTDlg)
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_LEVEL_0, m_LevelStr[0]);
   DDX_Text(pDX, IDC_VAL_0  , m_ValueStr[0]);
   DDX_Text(pDX, IDC_LEVEL_1, m_LevelStr[1]);
   DDX_Text(pDX, IDC_VAL_1  , m_ValueStr[1]);
}


BEGIN_MESSAGE_MAP(CParmThreshLTGTDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmThreshLTGTDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmThreshLTGTDlg message handlers

BOOL CParmThreshLTGTDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   return TRUE;
}
