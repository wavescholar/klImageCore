/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDecimateDlg.cpp : implementation of the CParmDecimateDlg class.
// CParmDecimateDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmDecimateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmDecimateDlg dialog


CParmDecimateDlg::CParmDecimateDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmDecimateDlg)
   m_fraction = -1;
   //}}AFX_DATA_INIT
}


void CParmDecimateDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmDecimateDlg)
   DDX_Radio(pDX, IDC_FRACTION_0, m_fraction);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmDecimateDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmDecimateDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmDecimateDlg message handlers
