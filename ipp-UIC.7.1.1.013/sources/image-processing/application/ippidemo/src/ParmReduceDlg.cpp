/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmReduceDlg.cpp : implementation of the CParmReduceDlg class.
// CParmReduceDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmReduceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmReduceDlg dialog


CParmReduceDlg::CParmReduceDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmReduceDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmReduceDlg)
   m_Levels = 0;
   m_Noise = 0;
   m_DitherType = -1;
   m_LevelName = _T("");
   //}}AFX_DATA_INIT
    m_MaxLevel = 2;
}


void CParmReduceDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmReduceDlg)
   DDX_Text(pDX, IDC_LEVELS, m_Levels);
   DDX_Text(pDX, IDC_NOISE, m_Noise);
   DDV_MinMaxInt(pDX, m_Noise, 0, 100);
   DDX_Radio(pDX, IDC_TYPE_0, m_DitherType);
   DDX_Text(pDX, IDC_LEVEL_STATIC, m_LevelName);
   //}}AFX_DATA_MAP
   DDV_MinMaxInt(pDX, m_Levels, 2, m_MaxLevel);
}


BEGIN_MESSAGE_MAP(CParmReduceDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmReduceDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_BN_CLICKED(IDC_TYPE_3, OnType)
   ON_BN_CLICKED(IDC_TYPE_4, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmReduceDlg message handlers

BOOL CParmReduceDlg::OnInitDialog()
{
    InitMaxLevel();
   CParamDlg::OnInitDialog();
    OnType();
   return TRUE;
}

void CParmReduceDlg::OnOK()
{
   CParamDlg::OnOK();
}

void CParmReduceDlg::OnType()
{
    BOOL flag = !IsDlgButtonChecked(IDC_TYPE_0);
    GetDlgItem(IDC_LEVELS)->EnableWindow(flag);
    GetDlgItem(IDC_LEVEL_STATIC)->EnableWindow(flag);
}

void CParmReduceDlg::InitMaxLevel()
{
    int depth = m_Func.DstType() & PP_MASK;
    m_MaxLevel = 1 << depth;
    if (m_Levels > m_MaxLevel) m_Levels = m_MaxLevel;
    m_LevelName.Format("Levels [2 - %d]", m_MaxLevel);
}
