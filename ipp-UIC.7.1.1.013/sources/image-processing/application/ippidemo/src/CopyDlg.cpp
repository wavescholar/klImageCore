/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// CopyDlg.h : interface for the Copy contour dialog.
// Command: Menu-Contour-{ROI|Quadrangle|Parallelogramm|Center&Shift}-Copy
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "CopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg dialog


CCopyDlg::CCopyDlg(CippiDemoDoc* pSrcDoc, CString title)
   : CDialog(CCopyDlg::IDD, NULL)
{
   m_pSrcDoc = pSrcDoc;
   m_Title.Format("Copy %s to", title);
   //{{AFX_DATA_INIT(CCopyDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CCopyDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CCopyDlg)
   DDX_Control(pDX, IDC_IMAGE, m_BoxImage);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyDlg, CDialog)
   //{{AFX_MSG_MAP(CCopyDlg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg message handlers

BOOL CCopyDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   SetWindowText(m_Title);
   ASSERT(m_pSrcDoc);
   MY_POSITION pos = DEMO_APP->GetFirstDocPosition();
   while (pos) {
        CippiDemoDoc* pDoc = (CippiDemoDoc*)DEMO_APP->GetNextDoc(pos);
        if (pDoc && pDoc != m_pSrcDoc)
            m_BoxImage.AddString(pDoc->GetTitle());
    }
    m_BoxImage.SetCurSel(0);
   if (m_BoxImage.GetCount() == 1) {
      SetDstDoc();
      EndDialog(IDOK);
   }
   return TRUE;
}

void CCopyDlg::OnOK()
{
   SetDstDoc();
   CDialog::OnOK();
}

void CCopyDlg::SetDstDoc()
{
    CString docName;
    int     index = m_BoxImage.GetCurSel();
    ASSERT(index >= 0);
    m_BoxImage.GetLBText(index, docName);
    m_pDstDoc = (CippiDemoDoc*)DEMO_APP->FindDoc(docName);
    m_pDstImage = m_pDstDoc;
}
