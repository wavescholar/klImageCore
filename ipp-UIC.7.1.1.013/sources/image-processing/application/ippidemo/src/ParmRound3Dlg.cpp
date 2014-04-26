/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRound3Dlg.cpp : implementation of the CParmRound3Dlg class.
// CParmRound3Dlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmRound3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRound3Dlg dialog


CParmRound3Dlg::CParmRound3Dlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParmRoundDlg(nID, pParent)
{
}


void CParmRound3Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParmRoundDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParmRound3Dlg, CParmRoundDlg)
   //{{AFX_MSG_MAP(CParmRound3Dlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRound3Dlg message handlers
