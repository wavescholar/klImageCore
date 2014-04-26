/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterBoxDlg.cpp : implementation of the CParmFilterBoxDlg class.
// CParmFilterBoxDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmFilterBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterBoxDlg dialog


CParmFilterBoxDlg::CParmFilterBoxDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   //{{AFX_DATA_INIT(CParmFilterBoxDlg)
   m_bCenter = FALSE;
   //}}AFX_DATA_INIT
}


void CParmFilterBoxDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmFilterBoxDlg)
   DDX_Check(pDX, IDC_CENTER, m_bCenter);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_ANCHOR_SPIN_0, m_AnchorSpin[0]);
   DDX_Control(pDX, IDC_ANCHOR_SPIN_1, m_AnchorSpin[1]);
   DDX_Control(pDX, IDC_ANCHOR_0, m_AnchorEdit[0]);
   DDX_Control(pDX, IDC_ANCHOR_1, m_AnchorEdit[1]);
   DDX_Control(pDX, IDC_ANCHOR_STATIC_0, m_AnchorStatic[0]);
   DDX_Control(pDX, IDC_ANCHOR_STATIC_1, m_AnchorStatic[1]);

   DDX_Control(pDX, IDC_SIZE_SPIN_0, m_SizeSpin[0]);
   DDX_Control(pDX, IDC_SIZE_SPIN_1, m_SizeSpin[1]);
   DDX_Control(pDX, IDC_SIZE_0, m_SizeEdit[0]);
   DDX_Control(pDX, IDC_SIZE_1, m_SizeEdit[1]);
   DDX_Control(pDX, IDC_SIZE_STATIC_0, m_SizeStatic[0]);
   DDX_Control(pDX, IDC_SIZE_STATIC_1, m_SizeStatic[1]);
}


BEGIN_MESSAGE_MAP(CParmFilterBoxDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmFilterBoxDlg)
   ON_BN_CLICKED(IDC_CENTER, OnCenter)
   ON_EN_CHANGE(IDC_SIZE_0, OnChangeSize)
   ON_EN_CHANGE(IDC_SIZE_1, OnChangeSize)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterBoxDlg message handlers

BOOL CParmFilterBoxDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   for (int i=0; i<2; i++) {
      m_SizeSpin[i].SetRange(1,32000);
      m_AnchorSpin[i].SetRange(0,32000);
   }
   SetSize(0,m_maskSize.width);
   SetSize(1,m_maskSize.height);
   SetAnchor(0,m_anchor.x);
   SetAnchor(1,m_anchor.y);
   EnableAnchor();
   CenterAnchor();
   UpdateData(FALSE);

   return TRUE;
}

void CParmFilterBoxDlg::OnOK()
{
   CParamDlg::OnOK();
   m_maskSize.width  = GetSize(0);
   m_maskSize.height = GetSize(1);
   m_anchor.x = GetAnchor(0);
   m_anchor.y = GetAnchor(1);
}

void CParmFilterBoxDlg::OnCenter()
{
   EnableAnchor();
   CenterAnchor();
}

void CParmFilterBoxDlg::OnChangeSize()
{
   CenterAnchor();
}

void CParmFilterBoxDlg::EnableAnchor()
{
   for (int i=0; i<2; i++) {
      m_AnchorEdit[i].SetReadOnly(GetCenter());
      m_AnchorSpin[i].EnableWindow(!GetCenter());
   }
}

void CParmFilterBoxDlg::CenterAnchor()
{
   if (!GetCenter()) return;
   for (int i=0; i<2; i++) {
      int n = GetSize(i);
      SetAnchor(i, n>>1);
   }
}

BOOL CParmFilterBoxDlg::GetCenter()
{
   return IsDlgButtonChecked(IDC_CENTER);
}

int CParmFilterBoxDlg::GetSize(int i)
{
   CString text;
   m_SizeEdit[i].GetWindowText(text);
   return atoi(text);
}

void CParmFilterBoxDlg::SetSize(int i, int val)
{
   CString text;
   text.Format("%d",val);
   m_SizeEdit[i].SetWindowText(text);
}

int CParmFilterBoxDlg::GetAnchor(int i)
{
   CString text;
   m_AnchorEdit[i].GetWindowText(text);
   return atoi(text);
}

void CParmFilterBoxDlg::SetAnchor(int i, int val)
{
   CString text;
   text.Format("%d",val);
   m_AnchorEdit[i].SetWindowText(text);
}
