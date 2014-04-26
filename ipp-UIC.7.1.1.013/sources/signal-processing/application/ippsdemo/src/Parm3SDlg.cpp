/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm3SDlg.cpp : implementation of the CParm3SDlg class.
// CParm3SDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "Parm3SDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParm3SDlg dialog


IMPLEMENT_DYNAMIC(CParm3SDlg, CParamDlg)
CParm3SDlg::CParm3SDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParm3SDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParm3SDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParm3SDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParm3SDlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParm3SDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParm3SDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParm3SDlg message handlers
