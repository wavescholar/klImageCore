/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmColorDlg.cpp : implementation of the CParmColorDlg class.
// CParmColorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmColorDlg dialog


CParmColorDlg::CParmColorDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmColorDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmColorDlg)
   //}}AFX_DATA_INIT
   for (int i=0; i<3; i++)
      m_CoeffStr[i] = _T("");
}


void CParmColorDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmColorDlg)
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_COEFF_0, m_CoeffStr[0]);
   DDX_Text(pDX, IDC_COEFF_1, m_CoeffStr[1]);
   DDX_Text(pDX, IDC_COEFF_2, m_CoeffStr[2]);
}


BEGIN_MESSAGE_MAP(CParmColorDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmColorDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmColorDlg message handlers

BOOL CParmColorDlg::OnInitDialog()
{
   for (int i=0; i<3; i++)
      m_CoeffStr[i].Format("%.6g", (double)m_coeffs[i]);
   CParamDlg::OnInitDialog();
   return TRUE;
}

void CParmColorDlg::OnOK()
{
   CParamDlg::OnOK();
   for (int i=0; i<3; i++)
      m_coeffs[i] = (float)atof(m_CoeffStr[i]);
}
