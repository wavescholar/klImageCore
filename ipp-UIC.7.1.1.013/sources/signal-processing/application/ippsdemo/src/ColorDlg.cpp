/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ColorDlg.cpp : implementation of the Color dialog.
// Command: Menu-Options-Color
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ColorDlg.h"
#include "ippsDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

COLORREF CColorDlg::m_CustomColors[16] = {
   RGB(201,160,160), RGB(201,192,160), RGB(176,201,160), RGB(160,201,176), 
   RGB(160,192,201), RGB(160,160,201), RGB(192,160,201), RGB(201,160,176), 
   RGB(208,193,193), RGB(208,205,193), RGB(200,208,193), RGB(193,208,200), 
   RGB(193,205,208), RGB(193,193,208), RGB(205,193,208), RGB(208,193,200), 
};

CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CColorDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CColorDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CColorDlg)
   DDX_Control(pDX, IDC_SAMPLE, m_Sample);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_BUTTON_COL_0, m_ColorButton[0]);
   DDX_Control(pDX, IDC_BUTTON_COL_1, m_ColorButton[1]);
   DDX_Control(pDX, IDC_BUTTON_COL_2, m_ColorButton[2]);
   DDX_Control(pDX, IDC_BUTTON_COL_3, m_ColorButton[3]);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
   //{{AFX_MSG_MAP(CColorDlg)
   ON_BN_CLICKED(IDC_BUTTON_COL_0, OnButtonCol0)
   ON_BN_CLICKED(IDC_BUTTON_COL_1, OnButtonCol1)
   ON_BN_CLICKED(IDC_BUTTON_COL_2, OnButtonCol2)
   ON_BN_CLICKED(IDC_BUTTON_COL_3, OnButtonCol3)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
   m_Color[0] = CippsDemoView::GetColorSignal();
   m_Color[1] = CippsDemoView::GetColorSignalBack();
   m_Color[2] = CippsDemoView::GetColorAxis();
   m_Color[3] = CippsDemoView::GetColorAxisBack();
   m_Sample.SetColor(m_Color);
   CDialog::OnInitDialog();

//   RedrawSample() ;
   return TRUE;
}

void CColorDlg::OnOK() 
{
   CDialog::OnOK();
   CippsDemoView::SetColorSignal    (m_Color[0]);
   CippsDemoView::SetColorSignalBack(m_Color[1]);
   CippsDemoView::SetColorAxis      (m_Color[2]);
   CippsDemoView::SetColorAxisBack  (m_Color[3]);
}

void CColorDlg::OnButtonCol0() 
{
   OnColor(0);
}

void CColorDlg::OnButtonCol1() 
{
   OnColor(1);
}

void CColorDlg::OnButtonCol2() 
{
   OnColor(2);
}

void CColorDlg::OnButtonCol3() 
{
   OnColor(3);
}

void CColorDlg::OnColor(int i) 
{
   CColorDialog dlg(m_Color[i]);
   dlg.m_cc.lpCustColors = m_CustomColors;
   dlg.m_cc.Flags |= CC_FULLOPEN;
   if (dlg.DoModal() != IDOK) return;
   m_Color[i] = dlg.GetColor();
   m_Sample.SetColor(m_Color);
   RedrawSample();
}

void CColorDlg::RedrawSample() 
{
    CRect rect;
    m_Sample.GetWindowRect(&rect);
    POINT origin = rect.TopLeft();   
    ::ScreenToClient(m_hWnd, &origin);
    rect = CRect(origin, rect.Size());
    InvalidateRect(&rect, FALSE);
    UpdateWindow();
}

