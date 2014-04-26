
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmRowDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmRowDlg.h"

IMPLEMENT_DYNAMIC(CParmRowDlg, CParamDlg)
CParmRowDlg::CParmRowDlg(UINT nID)
   : CParamDlg(nID)
{
}

CParmRowDlg::~CParmRowDlg()
{
}

void CParmRowDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);

   DDX_Check(pDX, IDC_CENTER, m_bCenter);

   DDX_Control(pDX, IDC_ANCHOR_SPIN_0, m_AnchorSpin);
   DDX_Control(pDX, IDC_ANCHOR_0, m_AnchorEdit);
   DDX_Control(pDX, IDC_ANCHOR_STATIC_0, m_AnchorStatic);

   DDX_Control(pDX, IDC_SIZE_SPIN_0, m_SizeSpin);
   DDX_Control(pDX, IDC_SIZE_0, m_SizeEdit);
   DDX_Control(pDX, IDC_SIZE_STATIC_0, m_SizeStatic);
}


BEGIN_MESSAGE_MAP(CParmRowDlg, CParamDlg)
   ON_BN_CLICKED(IDC_CENTER, OnCenter)
   ON_EN_CHANGE(IDC_SIZE_0, OnChangeSize)
END_MESSAGE_MAP()


// CParmRowDlg message handlers


BOOL CParmRowDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_SizeSpin.SetRange(1,32000);
   m_AnchorSpin.SetRange(0,32000);
   m_SizeStatic.SetWindowText(m_Func.Found("Row") ? "Width" : "Height");
   m_AnchorStatic.SetWindowText(m_Func.Found("Row") ? "X" : "Y");

   SetSize(m_maskSize);
   SetAnchor(m_anchor);
   EnableAnchor();
   CenterAnchor();
   UpdateData(FALSE);

   return TRUE;
}

void CParmRowDlg::OnOK()
{
   CParamDlg::OnOK();
   m_maskSize = GetSize();
   m_anchor = GetAnchor();
}

void CParmRowDlg::OnCenter()
{
   EnableAnchor();
   CenterAnchor();
}

void CParmRowDlg::OnChangeSize()
{
   CenterAnchor();
}

void CParmRowDlg::EnableAnchor()
{
   m_AnchorEdit.SetReadOnly(GetCenter());
   m_AnchorSpin.EnableWindow(!GetCenter());
}

void CParmRowDlg::CenterAnchor()
{
   if (!GetCenter()) return;
   int n = GetSize();
   SetAnchor(n>>1);
}

BOOL CParmRowDlg::GetCenter()
{
   return IsDlgButtonChecked(IDC_CENTER);
}

int CParmRowDlg::GetSize()
{
   CString text;
   m_SizeEdit.GetWindowText(text);
   return atoi(text);
}

void CParmRowDlg::SetSize(int val)
{
   CString text;
   text.Format("%d",val);
   m_SizeEdit.SetWindowText(text);
}

int CParmRowDlg::GetAnchor()
{
   CString text;
   m_AnchorEdit.GetWindowText(text);
   return atoi(text);
}

void CParmRowDlg::SetAnchor(int val)
{
   CString text;
   text.Format("%d",val);
   m_AnchorEdit.SetWindowText(text);
}
