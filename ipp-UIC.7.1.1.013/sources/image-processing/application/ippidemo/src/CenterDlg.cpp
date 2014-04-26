/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// CenterDlg.cpp : implementation of the Set Center dialog.
// Command: Menu-Contour-Center&Shift-Set
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "CenterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCenterDlg dialog

CCenterDlg::CCenterDlg(CippiDemoDoc* pDoc)
   : CDialog(CCenterDlg::IDD, NULL)
{
   m_pDoc = pDoc;
   //{{AFX_DATA_INIT(CCenterDlg)
   //}}AFX_DATA_INIT
}


void CCenterDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CCenterDlg)
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_CENTER_0, m_Center[0]);
   DDX_Text(pDX, IDC_CENTER_1, m_Center[1]);
   DDX_Text(pDX,  IDC_SHIFT_0, m_Shift [0]);
   DDX_Text(pDX,  IDC_SHIFT_1, m_Shift [1]);
}


BEGIN_MESSAGE_MAP(CCenterDlg, CDialog)
   //{{AFX_MSG_MAP(CCenterDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCenterDlg message handlers

BOOL CCenterDlg::OnInitDialog()
{
   ASSERT(m_pDoc);
    if (!m_pDoc->GetCenterShift(m_Center, m_Shift)) {
        int width  = m_pDoc->Width();
        int height = m_pDoc->Height();
        m_Center[0] = width >> 1;
        m_Center[1] = height >> 1;
        m_Shift[0] = 0;
        m_Shift[1] = 0;
    }

   CDialog::OnInitDialog();

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CCenterDlg::OnOK()
{
   CDialog::OnOK();
    m_pDoc->SetCenterShift(m_Center, m_Shift);
}
