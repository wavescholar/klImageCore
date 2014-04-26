/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRandDlg.cpp : implementation of the CParmRandDlg class.
// CParmRandDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmRandDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRandDlg dialog


IMPLEMENT_DYNAMIC(CParmRandDlg, CParamDlg)
CParmRandDlg::CParmRandDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmRandDlg::IDD, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmRandDlg)
   m_seed = 0;
   //}}AFX_DATA_INIT
}


void CParmRandDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmRandDlg)
   DDX_Control(pDX, IDC_SEED_SPIN, m_SeedSpin);
   DDX_Text(pDX, IDC_SEED, m_seed);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_PARM_0, m_ParmStr[0]);
   DDX_Text(pDX, IDC_PARM_1, m_ParmStr[1]);
   DDX_Text(pDX, IDC_PARM_STATIC_0, m_ParmName[0]);
   DDX_Text(pDX, IDC_PARM_STATIC_1, m_ParmName[1]);
}


BEGIN_MESSAGE_MAP(CParmRandDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRandDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRandDlg message handlers

BOOL CParmRandDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   m_SeedSpin.SetRange32(0, UINT_MAX);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
