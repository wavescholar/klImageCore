/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// FilePropDlg.cpp : implementation of the File properties dialog.
// Command: Menu-File-Properties
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "FilePropDlg.h"
#include "ippsDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilePropDlg dialog


CFilePropDlg::CFilePropDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CFilePropDlg::IDD, pParent)
{
   m_pDoc = NULL;
   //{{AFX_DATA_INIT(CFilePropDlg)
   m_FileName = _T("");
   m_IMax = 0.0;
   m_Type = _T("");
   m_Len = 0;
   //}}AFX_DATA_INIT
}


void CFilePropDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CFilePropDlg)
   DDX_Text(pDX, IDC_FILE, m_FileName);
   DDX_Text(pDX, IDC_TYPE, m_Type);
   DDX_Text(pDX, IDC_LEN, m_Len);
   DDX_Control(pDX, IDC_IMAX_STATIC, m_IMaxStatic);
   DDX_Control(pDX, IDC_IMAX, m_IMaxEdit);
   DDX_Control(pDX, IDC_RMAX_STATIC, m_RMaxStatic);
   DDX_Control(pDX, IDC_RMAX, m_RMaxEdit);
   DDX_Control(pDX, IDC_IMIN_STATIC, m_IMinStatic);
   DDX_Control(pDX, IDC_IMIN, m_IMinEdit);
   DDX_Control(pDX, IDC_RMIN_STATIC, m_RMinStatic);
   DDX_Control(pDX, IDC_RMIN, m_RMinEdit);
   DDX_Text(pDX, IDC_IMAX, m_IMax);
   DDX_Text(pDX, IDC_RMAX, m_RMax);
   DDX_Text(pDX, IDC_IMIN, m_IMin);
   DDX_Text(pDX, IDC_RMIN, m_RMin);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilePropDlg, CDialog)
   //{{AFX_MSG_MAP(CFilePropDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilePropDlg message handlers

BOOL CFilePropDlg::OnInitDialog() 
{
   ASSERT(m_pDoc);
   m_FileName = m_pDoc->GetPathName();
   m_Type = m_pDoc->TypeString();
   m_Len = m_pDoc->Length();
   Ipp64fc min = m_pDoc->Min();
   Ipp64fc max = m_pDoc->Max();
   m_RMin = min.re;
   m_RMax = max.re;
   m_IMin = min.im;
   m_IMax = max.im;
   CDialog::OnInitDialog();

   SetWindowText(m_pDoc->GetTitle() + " Properties");
   if (!m_pDoc->Complex()) {
      m_RMinStatic.SetWindowText("Min");
      m_RMaxStatic.SetWindowText("Max");
      m_IMinStatic.ShowWindow(SW_HIDE);
      m_IMaxStatic.ShowWindow(SW_HIDE);
      m_IMinEdit.ShowWindow(SW_HIDE);
      m_IMaxEdit.ShowWindow(SW_HIDE);
   }
   return TRUE;
}

void CFilePropDlg::OnOK() 
{
   CDialog::OnCancel();
}
