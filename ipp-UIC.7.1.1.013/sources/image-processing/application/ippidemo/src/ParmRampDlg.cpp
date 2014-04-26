/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRampDlg.cpp : implementation of the CParmRampDlg class.
// CParmRampDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmRampDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmRampDlg dialog


CParmRampDlg::CParmRampDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmRampDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmRampDlg)
   m_axis = -1;
   m_offsetStr = _T("");
   m_slopeStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmRampDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmRampDlg)
   DDX_Radio(pDX, IDC_F0, m_axis);
   DDX_Text(pDX, IDC_OFFSET, m_offsetStr);
   DDX_Text(pDX, IDC_SLOPE, m_slopeStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmRampDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRampDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRampDlg message handlers
