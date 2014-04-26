
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChFind dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

// ParmFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChFindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChFindDlg dialog

IMPLEMENT_DYNAMIC(CParmChFindDlg, CParamDlg)
CParmChFindDlg::CParmChFindDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmChFindDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmChFindDlg)
   m_index = 0;
   //}}AFX_DATA_INIT
}


void CParmChFindDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChFindDlg)
   DDX_Text(pDX, IDC_INDEX, m_index);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmChFindDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmChFindDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmChFindDlg message handlers
