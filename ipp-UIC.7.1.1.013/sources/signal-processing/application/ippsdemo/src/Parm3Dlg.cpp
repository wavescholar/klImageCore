/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm3Dlg.cpp : implementation of the CParm3Dlg class.
// CParm3Dlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Parm3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParm3Dlg dialog


IMPLEMENT_DYNAMIC(CParm3Dlg, CParamDlg)
CParm3Dlg::CParm3Dlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParm3Dlg::IDD, pParent)
{
   m_UsedScale   = TRUE;
   //{{AFX_DATA_INIT(CParm3Dlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CParm3Dlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParm3Dlg)
      // NOTE: the ClassWizard will add DDX and DDV calls here
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParm3Dlg, CParamDlg)
   //{{AFX_MSG_MAP(CParm3Dlg)
      // NOTE: the ClassWizard will add message map macros here
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParm3Dlg message handlers

BOOL CParm3Dlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   if (m_nModalResult == IDCANCEL) return TRUE;

   if (!m_Func.Scale()) OnOK();

   if (m_Func.BaseName() == "Magnitude" ||
       m_Func.BaseName() == "Phase" ||
       m_Func.BaseName() == "RealToCplx" ||
       m_Func.BaseName() == "CartToPolar") {
      m_StaticSrc.SetWindowText("SrcRe");
      m_StaticSrc2.SetWindowText("SrcIm");
   } else if (m_Func.BaseName() == "PolarToCart") {
      m_StaticSrc.SetWindowText("SrcMagn");
      m_StaticSrc2.SetWindowText("SrcPhase");
   }
   return TRUE;
}

void CParm3Dlg::OnOK() 
{
   CParamDlg::OnOK();
}
