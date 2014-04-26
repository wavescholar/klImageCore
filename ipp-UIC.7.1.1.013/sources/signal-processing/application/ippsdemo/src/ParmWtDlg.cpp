/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWtDlg.cpp : implementation of the CParmWtDlg class.
// CParmWtDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmWtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmWtDlg dialog


IMPLEMENT_DYNAMIC(CParmWtDlg, CParamDlg)
CParmWtDlg::CParmWtDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmWtDlg::IDD, pParent)
{
   m_UsedScale = FALSE;
   //{{AFX_DATA_INIT(CParmWtDlg)
   //}}AFX_DATA_INIT
   for (int i=0; i<2; i++) {
      m_OffsetStr[i] = _T("");
      m_TapStr[i] = _T("");
      m_TapTitle[i] = _T("");
      m_TapLen[i] = 0;
   }
}


void CParmWtDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmWtDlg)
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_OFFSET_SPIN_0, m_OffsetSpin[0]);
   DDX_Text(pDX, IDC_OFFSET_EDIT_0, m_OffsetStr[0]);
   DDX_Text(pDX, IDC_TAPS_EDIT_0, m_TapStr[0]);
   DDX_Text(pDX, IDC_TAPS_TITLE_0, m_TapTitle[0]);
   DDX_Text(pDX, IDC_TAPSLEN_EDIT_0, m_TapLen[0]);

   DDX_Control(pDX, IDC_OFFSET_SPIN_1, m_OffsetSpin[1]);
   DDX_Text(pDX, IDC_OFFSET_EDIT_1, m_OffsetStr[1]);
   DDX_Text(pDX, IDC_TAPS_EDIT_1, m_TapStr[1]);
   DDX_Text(pDX, IDC_TAPS_TITLE_1, m_TapTitle[1]);
   DDX_Text(pDX, IDC_TAPSLEN_EDIT_1, m_TapLen[1]);
}


BEGIN_MESSAGE_MAP(CParmWtDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmWtDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmWtDlg message handlers

BOOL CParmWtDlg::OnInitDialog() 
{
   CParamDlg::OnInitDialog();
   if ((m_UsedVectors & VEC_SRC2) == 0) {
      GetDlgItem(IDC_EDIT_SRC2)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_STATIC_SRC2)->ShowWindow(SW_HIDE);
   }
   m_pDocTapsLow->GetVector()->Get(m_TapStr[0]);
   m_TapTitle[0] = m_pDocTapsLow->GetTitle();
   m_TapLen[0] = m_pDocTapsLow->GetVector()->Length();
   m_OffsetStr[0].Format("%d", m_offsLow);
   m_OffsetSpin[0].SetRange(0,m_TapLen[0] - 1);

   m_pDocTapsHigh->GetVector()->Get(m_TapStr[1]);
   m_TapTitle[1] = m_pDocTapsHigh->GetTitle();
   m_TapLen[1] = m_pDocTapsHigh->GetVector()->Length();
   m_OffsetStr[1].Format("%d", m_offsHigh);
   m_OffsetSpin[1].SetRange(1,m_TapLen[1] - 1);

   UpdateData(FALSE);
   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmWtDlg::OnOK() 
{
   CParamDlg::OnOK();
   m_offsLow = atoi(m_OffsetStr[0]);   
   m_offsHigh = atoi(m_OffsetStr[1]);  
}
