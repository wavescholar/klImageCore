
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChSplit dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

// ParmSplitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChSplitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChSplitDlg dialog

IMPLEMENT_DYNAMIC(CParmChSplitDlg, CParamDlg)
CParmChSplitDlg::CParmChSplitDlg(CWnd* pParent /*=NULL*/)
   : CParmChValueDlg(CParmChSplitDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmChSplitDlg)
   m_LenStr = _T("");
   m_NumStr = _T("");
   m_NumName = _T("");
   //}}AFX_DATA_INIT
}


void CParmChSplitDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmChValueDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChSplitDlg)
   DDX_Control(pDX, IDC_NUM_SPIN, m_NumSpin);
   DDX_Control(pDX, IDC_LEN_EDIT, m_LenEdit);
   DDX_Control(pDX, IDC_LEN_STATIC, m_LenStatic);
   DDX_Text(pDX, IDC_LEN_EDIT, m_LenStr);
   DDX_Text(pDX, IDC_NUM_EDIT, m_NumStr);
   DDX_Text(pDX, IDC_NUM_STATIC, m_NumName);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmChSplitDlg, CParmChValueDlg)
   //{{AFX_MSG_MAP(CParmChSplitDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//   Implementation

void CParmChSplitDlg::EnableLen(BOOL flag)
{
   m_LenEdit.EnableWindow(flag);
   m_LenStatic.EnableWindow(flag);
}

/////////////////////////////////////////////////////////////////////////////
// CParmChSplitDlg message handlers

BOOL CParmChSplitDlg::OnInitDialog() 
{
   m_NumName = m_Func.Found(_T("Split")) ? _T("Num Dst") : _T("Num Src"); 
   CParmChValueDlg::OnInitDialog();

   EnableValue(m_Func.Found(_T("C_")));
   EnableLen(m_Func.Found(_T("Split")));
   m_NumSpin.SetRange(1, 1000);
   
   return TRUE;
}
