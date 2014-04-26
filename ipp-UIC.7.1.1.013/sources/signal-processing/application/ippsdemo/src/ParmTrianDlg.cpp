/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmTrianDlg.cpp : implementation of the CParmTrianDlg class.
// CParmTrianDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmTrianDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmTrianDlg dialog


IMPLEMENT_DYNAMIC(CParmTrianDlg, CParamDlg)
CParmTrianDlg::CParmTrianDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmTrianDlg::IDD, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmTrianDlg)
   //}}AFX_DATA_INIT
}


void CParmTrianDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmTrianDlg)
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_PARM_0, m_ParmStr[0]);
   DDX_Text(pDX, IDC_PARM_1, m_ParmStr[1]);
   DDX_Text(pDX, IDC_PARM_2, m_ParmStr[2]);
   DDX_Text(pDX, IDC_PARM_3, m_ParmStr[3]);
}


BEGIN_MESSAGE_MAP(CParmTrianDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmTrianDlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmTrianDlg message handlers
