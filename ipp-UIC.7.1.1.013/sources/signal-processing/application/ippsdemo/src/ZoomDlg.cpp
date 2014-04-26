/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ZoomDlg.cpp : implementation of the Zoom dialog.
// Command: Menu-Zoom-CustomZoom
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ZoomDlg.h"
#include "ippsDemoDoc.h"
#include "ippsDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog


CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CZoomDlg::IDD, pParent)
{
   m_pDoc = NULL;
   //{{AFX_DATA_INIT(CZoomDlg)
   m_PixelH = 0;
   m_PixelW = 0;
   m_ScaleHStr = _T("");
   m_ScaleWStr = _T("");
   //}}AFX_DATA_INIT
}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CZoomDlg)
   DDX_Control(pDX, IDC_SCALE_W, m_ScaleWEdit);
   DDX_Control(pDX, IDC_SCALE_H, m_ScaleHEdit);
   DDX_Text(pDX, IDC_PIXEL_H, m_PixelH);
   DDX_Text(pDX, IDC_PIXEL_W, m_PixelW);
   DDX_Text(pDX, IDC_SCALE_H, m_ScaleHStr);
   DDX_Text(pDX, IDC_SCALE_W, m_ScaleWStr);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
   //{{AFX_MSG_MAP(CZoomDlg)
   ON_EN_CHANGE(IDC_SCALE_H, OnChangeScaleH)
   ON_EN_CHANGE(IDC_SCALE_W, OnChangeScaleW)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg message handlers

BOOL CZoomDlg::OnInitDialog() 
{
   ASSERT(m_pDoc);
   m_ScaleHStr.Format("%.3g", m_ScaleH);
   m_ScaleWStr.Format("%.3g", m_ScaleW);
   UpdatePixelH();
   UpdatePixelW();
   CDialog::OnInitDialog();
   return TRUE;
}

void CZoomDlg::OnOK() 
{
   UpdateData();
   m_ScaleH = EtoD(m_ScaleHEdit);
   if (m_ScaleH <= 0) return;
   m_ScaleW = EtoD(m_ScaleWEdit);
   if (m_ScaleW <= 0) return;
   CDialog::OnOK();
}

void CZoomDlg::OnChangeScaleH() 
{
   UpdateData();
   m_ScaleH = atof(m_ScaleHStr);
   UpdatePixelH();
   UpdateData(FALSE);
}

void CZoomDlg::OnChangeScaleW() 
{
   UpdateData();
   m_ScaleW = atof(m_ScaleWStr);
   UpdatePixelW();
   UpdateData(FALSE);
}

void CZoomDlg::UpdatePixelH() 
{
   if (m_pDoc->GetDemoView() && m_ScaleH > 0)
      m_PixelH = m_pDoc->GetDemoView()->GetScaleHeight(m_ScaleH);
   else
      m_PixelH = 0;
}

void CZoomDlg::UpdatePixelW() 
{
   if (m_pDoc->GetDemoView() && m_ScaleW > 0)
      m_PixelW = m_pDoc->GetDemoView()->GetScaleWidth(m_ScaleW);
   else
      m_PixelW = 0;
}

double CZoomDlg::EtoD(CEdit& edit)
{
   CString str;
   edit.GetWindowText(str);
   double val = atof(str);
   if (val <= 0) {
      AfxMessageBox("Enter float value greater zero");
      edit.SetFocus();
   }
   return val;
}
