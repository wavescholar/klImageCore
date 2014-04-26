/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmImg3Dlg.cpp : implementation of the CParmImg3Dlg class.
// CParmImg3Dlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmImg3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmImg3Dlg dialog

//  Add
//  Mpy
//  Sub
//  MpyScale

CParmImg3Dlg::CParmImg3Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmImg3Dlg::IDD, pParent)
{
   m_UsedScale = TRUE;
   //{{AFX_DATA_INIT(CParmImg3Dlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParmImg3Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmImg3Dlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParmImg3Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmImg3Dlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmImg3Dlg message handlers


BOOL CParmImg3Dlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (!m_Func.Scale()) {
      CParamDlg::OnOK();
   }
   if (m_Func.Mask()) {
      m_StaticSrc.SetWindowText("Src");
      m_StaticSrc2.SetWindowText("Mask");
   }
   return TRUE;
}

