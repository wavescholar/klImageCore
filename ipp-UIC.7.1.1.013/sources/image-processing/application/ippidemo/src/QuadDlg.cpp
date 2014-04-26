/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// QuadDlg.cpp : implementation of the Set Quadrangle or Parallelogramm dialog.
// Command: Menu-Contour-{Quadrangle|Parallelogramm}-Set
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "QuadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuadDlg dialog


CQuadDlg::CQuadDlg(CDemoDoc* pDoc, BOOL isParl)
   : CDialog(CQuadDlg::IDD, NULL)
{
   m_pDoc = pDoc;
    m_IsParl = isParl;
   //{{AFX_DATA_INIT(CQuadDlg)
   //}}AFX_DATA_INIT
}


void CQuadDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CQuadDlg)
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_X0, m_Quad[0][0]);
   DDX_Text(pDX, IDC_X1, m_Quad[1][0]);
   DDX_Text(pDX, IDC_X2, m_Quad[2][0]);
   DDX_Text(pDX, IDC_X3, m_Quad[3][0]);
   DDX_Text(pDX, IDC_Y0, m_Quad[0][1]);
   DDX_Text(pDX, IDC_Y1, m_Quad[1][1]);
   DDX_Text(pDX, IDC_Y2, m_Quad[2][1]);
   DDX_Text(pDX, IDC_Y3, m_Quad[3][1]);
   DDX_Control(pDX, IDC_X0, m_Edit[0][0]);
   DDX_Control(pDX, IDC_X1, m_Edit[1][0]);
   DDX_Control(pDX, IDC_X2, m_Edit[2][0]);
   DDX_Control(pDX, IDC_X3, m_Edit[3][0]);
   DDX_Control(pDX, IDC_Y0, m_Edit[0][1]);
   DDX_Control(pDX, IDC_Y1, m_Edit[1][1]);
   DDX_Control(pDX, IDC_Y2, m_Edit[2][1]);
   DDX_Control(pDX, IDC_Y3, m_Edit[3][1]);
}


BEGIN_MESSAGE_MAP(CQuadDlg, CDialog)
   //{{AFX_MSG_MAP(CQuadDlg)
   ON_EN_CHANGE(IDC_X0, OnChange)
   ON_EN_CHANGE(IDC_X1, OnChange)
   ON_EN_CHANGE(IDC_X2, OnChange)
   ON_EN_CHANGE(IDC_Y0, OnChange)
   ON_EN_CHANGE(IDC_Y1, OnChange)
   ON_EN_CHANGE(IDC_Y2, OnChange)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuadDlg message handlers

BOOL CQuadDlg::OnInitDialog()
{
   ASSERT(m_pDoc);
   CImage* image = m_pDoc->GetImage();
   BOOL isQuad = m_IsParl ? image->GetParl(m_Quad) : image->GetQuad(m_Quad);
   if (!isQuad) {
      IppiRect roi = {0, 0, image->Width(), image->Height()};
      if (m_IsParl)
         GetDefaultParl(roi,m_Quad);
      else
         GetDefaultQuad(roi,m_Quad);
   }

   CDialog::OnInitDialog();

   if (m_IsParl) {
       SetWindowText("Set Parallelogramm");
       m_Edit[3][0].SetReadOnly();
       m_Edit[3][1].SetReadOnly();
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuadDlg::OnOK()
{
   CDialog::OnOK();
   if (m_IsParl)
      m_pDoc->GetImage()->SetParl(m_Quad);
   else
      m_pDoc->GetImage()->SetQuad(m_Quad);
}

void CQuadDlg::GetDefaultParl(IppiRect roi, double quad[4][2])
{
   quad[0][0] = roi.x + (roi.width >> 4);
   quad[0][1] = roi.y + (roi.height >> 4);
   quad[1][0] = roi.x + roi.width - (roi.width >> 3);
   quad[1][1] = roi.y + (roi.height >> 3);
   quad[2][0] = roi.x + roi.width - (roi.width >> 4);
   quad[2][1] = roi.y + roi.height - (roi.height >> 4);
   quad[3][0] = roi.x + (roi.width >> 3);
   quad[3][1] = roi.y + roi.height - (roi.height >> 3);
}

void CQuadDlg::GetDefaultQuad(IppiRect roi, double quad[4][2])
{
   quad[0][0] = roi.x + (roi.width >> 4);
   quad[0][1] = roi.y + (roi.height >> 4);
   quad[1][0] = roi.x + roi.width - (roi.width >> 4);
   quad[1][1] = roi.y + (roi.height >> 4);
   quad[2][0] = roi.x + roi.width - (roi.width >> 2);
   quad[2][1] = roi.y + roi.height - (roi.height >> 4);
   quad[3][0] = roi.x + (roi.width >> 2);
   quad[3][1] = roi.y + roi.height - (roi.height >> 4);
}

void CQuadDlg::OnChange()
{
    if (m_IsParl) {
        UpdateVals();
        CImage::SetLastParlPoint(m_Quad);
        FormatVal3();
    }
}

void CQuadDlg::FormatVal3()
{
    int i = 3;
    for (int j=0; j<2; j++) {
        CString text;
        text.Format("%.1f", m_Quad[i][j]);
        m_Edit[i][j].SetWindowText(text);
    }
}

void CQuadDlg::UpdateVals()
{
    for (int i=0; i<4; i++) {
        for (int j=0; j<2; j++) {
            CString text;
            m_Edit[i][j].GetWindowText(text);
            m_Quad[i][j] = atof(text);
        }
    }
}
