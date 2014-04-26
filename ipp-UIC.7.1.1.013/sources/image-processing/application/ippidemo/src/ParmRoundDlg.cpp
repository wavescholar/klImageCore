/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRoundDlg.cpp : implementation of the CParmRoundDlg class.
// CParmRoundDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmRoundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRoundDlg dialog


CParmRoundDlg::CParmRoundDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   m_UsedScale = TRUE;
   //{{AFX_DATA_INIT(CParmRoundDlg)
   m_roundMode = -1;
   //}}AFX_DATA_INIT
}


void CParmRoundDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmRoundDlg)
   DDX_Radio(pDX, IDC_RND_0, m_roundMode);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmRoundDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRoundDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRoundDlg message handlers
