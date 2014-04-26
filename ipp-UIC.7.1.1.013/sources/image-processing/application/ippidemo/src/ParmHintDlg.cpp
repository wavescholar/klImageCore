/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmHintDlg.cpp : implementation of the CParmHintDlg class.
// CParmHintDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmHintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmHintDlg dialog


CParmHintDlg::CParmHintDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   m_UsedScale = TRUE;
   //{{AFX_DATA_INIT(CParmHintDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmHintDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmHintDlg)
   DDX_Radio(pDX, IDC_HINT_0, m_Hint);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmHintDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmHintDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmHintDlg message handlers
