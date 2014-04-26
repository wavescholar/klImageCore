/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCorrDlg.cpp : implementation of the CParmCorrDlg class.
// CParmCorrDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmCorrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmCorrDlg dialog


IMPLEMENT_DYNAMIC(CParmCorrDlg, CParamDlg)
CParmCorrDlg::CParmCorrDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmCorrDlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParmCorrDlg)
   m_LolagStr = _T("");
   m_DstlenStr = _T("");
   //}}AFX_DATA_INIT
}


void CParmCorrDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmCorrDlg)
   DDX_Control(pDX, IDC_LOLAG_STATIC, m_LolagStatic);
   DDX_Control(pDX, IDC_LOLAG_EDIT, m_LolagEdit);
   DDX_Control(pDX, IDC_SRCLEN1, m_SrcLenEdit);
   DDX_Control(pDX, IDC_SRCLEN2, m_Src2LenEdit);
   DDX_Control(pDX, IDC_DSTLEN_SPIN, m_DstlenSpin);
   DDX_Control(pDX, IDC_LOLAG_SPIN, m_LolagSpin);
   DDX_Text(pDX, IDC_LOLAG_EDIT, m_LolagStr);
   DDX_Text(pDX, IDC_DSTLEN_EDIT, m_DstlenStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmCorrDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmCorrDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmCorrDlg message handlers

BOOL CParmCorrDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();

   m_LolagSpin.SetRange32(0,INT_MAX);
   m_DstlenSpin.SetRange32(1,INT_MAX);
   EnableSrc2();
   SetLen1();
   SetLen2();
   
   return TRUE;
}

void CParmCorrDlg::OnOK() 
{
   // TODO: Add extra validation here
   
   CParamDlg::OnOK();
}

void CParmCorrDlg::SetLen1()
{
   SetLenEdit(m_SrcLenEdit, m_pDocSrc);
}

void CParmCorrDlg::SetLen2()
{
   SetLenEdit(m_Src2LenEdit, m_pDocSrc2);
}

void CParmCorrDlg::SetLenEdit(CEdit& edit, CDemoDoc* pDoc)
{
   if (!pDoc) return;
   CString str;
   str.Format("%d",pDoc->GetVector()->Length());
   edit.SetWindowText(str);
}

void CParmCorrDlg::EnableSrc2()
{
   if (!m_Func.Found("Auto")) return;
   GetDlgItem(IDC_EDIT_SRC2)->EnableWindow(FALSE);
   GetDlgItem(IDC_STATIC_SRC2)->EnableWindow(FALSE);
   GetDlgItem(IDC_SRCSTATIC2)->EnableWindow(FALSE);
   GetDlgItem(IDC_SRCLEN2)->EnableWindow(FALSE);
   m_LolagStatic.ShowWindow(SW_HIDE);
   m_LolagEdit.ShowWindow(SW_HIDE);
   m_LolagSpin.ShowWindow(SW_HIDE);
}

