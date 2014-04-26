/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormlzeDlg.cpp : implementation of the CParmNormlzeDlg class.
// CParmNormlzeDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmNormlzeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmNormlzeDlg dialog


IMPLEMENT_DYNAMIC(CParmNormlzeDlg, CParamDlg)
CParmNormlzeDlg::CParmNormlzeDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmNormlzeDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmNormlzeDlg)
   m_DivStr = _T("");
   //}}AFX_DATA_INIT
   m_SubStr[0] = _T("");
   m_SubStr[1] = _T("");
}


void CParmNormlzeDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmNormlzeDlg)
   DDX_Text(pDX, IDC_DIV, m_DivStr);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_SUB_STATIC_0, m_SubStatic[0]);
   DDX_Control(pDX, IDC_SUB_STATIC_1, m_SubStatic[1]);
   DDX_Control(pDX, IDC_SUB_0, m_SubEdit[0]);
   DDX_Control(pDX, IDC_SUB_1, m_SubEdit[1]);
   DDX_Text(pDX, IDC_SUB_0, m_SubStr[0]);
   DDX_Text(pDX, IDC_SUB_1, m_SubStr[1]);
}


BEGIN_MESSAGE_MAP(CParmNormlzeDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmNormlzeDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmNormlzeDlg message handlers

BOOL CParmNormlzeDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   
   BOOL bCplx = m_Func.SrcType() & PP_CPLX;
   m_SubStatic[1].EnableWindow(bCplx);
   m_SubEdit[1].EnableWindow(bCplx);
   
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmNormlzeDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}
