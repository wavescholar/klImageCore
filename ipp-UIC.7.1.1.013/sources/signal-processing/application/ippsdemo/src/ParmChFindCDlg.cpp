
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmChFindC dialog gets parameters for certain ippCH functions.
//
/////////////////////////////////////////////////////////////////////////////

// ParmFindCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmChFindCDlg.h"
#include "RunChFindC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmChFindCDlg dialog

IMPLEMENT_DYNAMIC(CParmChFindCDlg, CParamDlg)
CParmChFindCDlg::CParmChFindCDlg(CRunChFindC* pRun)
   : CParmChValueDlg(CParmChFindCDlg::IDD, NULL)
{
   m_pRun = pRun;
   //{{AFX_DATA_INIT(CParmChFindCDlg)
   m_index = 0;
   //}}AFX_DATA_INIT
}


void CParmChFindCDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmChValueDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmChFindCDlg)
   DDX_Text(pDX, IDC_INDEX, m_index);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmChFindCDlg, CParmChValueDlg)
   //{{AFX_MSG_MAP(CParmChFindCDlg)
   ON_EN_CHANGE(IDC_CHAR, OnChangeChar)
   ON_EN_CHANGE(IDC_CODE, OnChangeCode)
   ON_BN_CLICKED(IDC_SYMBOL, OnSymbol)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmChFindCDlg message handlers

BOOL CParmChFindCDlg::OnInitDialog() 
{
   CParmChValueDlg::OnInitDialog();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
   
   return TRUE;
}

void CParmChFindCDlg::OnChangeChar() 
{
   CParmChValueDlg::OnChangeChar();
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmChFindCDlg::OnChangeCode() 
{
   CParmChValueDlg::OnChangeCode();
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}

void CParmChFindCDlg::OnSymbol() 
{
   CParmChValueDlg::OnSymbol();
   UpdateData();
   m_pRun->SetValues(this);
   UpdateData(FALSE);
}
