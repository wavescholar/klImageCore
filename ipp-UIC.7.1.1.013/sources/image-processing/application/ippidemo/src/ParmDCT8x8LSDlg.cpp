/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDCT8x8LSDlg.cpp : implementation of the CParmDCT8x8LSDlg class.
// CParmDCT8x8LSDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmDCT8x8LSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmDCT8x8LSDlg dialog


CParmDCT8x8LSDlg::CParmDCT8x8LSDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmDCT8x8LSDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmDCT8x8LSDlg)
   m_ValStr = _T("");
   //}}AFX_DATA_INIT
   m_ClipStr[0] = _T("");
   m_ClipStr[1] = _T("");
}


void CParmDCT8x8LSDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmDCT8x8LSDlg)
   DDX_Text(pDX, IDC_VAL, m_ValStr);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_EDIT_CLIP_0, m_ClipStr[0]);
   DDX_Text(pDX, IDC_EDIT_CLIP_1, m_ClipStr[1]);
   DDX_Control(pDX, IDC_EDIT_CLIP_0, m_SpinEdit[0]);
   DDX_Control(pDX, IDC_EDIT_CLIP_1, m_SpinEdit[1]);
   DDX_Control(pDX, IDC_SPIN_CLIP_0, m_ClipSpin[0]);
   DDX_Control(pDX, IDC_SPIN_CLIP_1, m_ClipSpin[1]);
   DDX_Control(pDX, IDC_STATIC_CLIP_0, m_ClipStatic[0]);
   DDX_Control(pDX, IDC_STATIC_CLIP_1, m_ClipStatic[1]);
}


BEGIN_MESSAGE_MAP(CParmDCT8x8LSDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmDCT8x8LSDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmDCT8x8LSDlg message handlers

BOOL CParmDCT8x8LSDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   if (m_Func.Found("Clip")) {
      for (int i=0; i<2; i++) {
         m_ClipSpin[i].SetRange(0,255);
      }
   } else {
      for (int i=0; i<2; i++) {
         m_SpinEdit[i].EnableWindow(FALSE);
         m_ClipStatic[i].EnableWindow(FALSE);
         m_ClipSpin[i].EnableWindow(FALSE);
      }
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
