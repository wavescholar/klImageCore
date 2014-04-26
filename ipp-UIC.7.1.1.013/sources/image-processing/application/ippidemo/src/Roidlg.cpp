/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RoiDlg.cpp : implementation of the Set ROI dialog.
// Command: Menu-Contour-ROI-Set
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RoiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoiDlg dialog


CRoiDlg::CRoiDlg(CDemoDoc* pDoc)
   : CDialog(CRoiDlg::IDD, NULL)
{
   m_pDoc = pDoc;
   //{{AFX_DATA_INIT(CRoiDlg)
   //}}AFX_DATA_INIT
   m_Roi.width = 1;
   m_Roi.x = 0;
   m_Roi.y = 0;
   m_Roi.height = 1;
    m_ImageWidth = 1;
    m_ImageHeight = 1;
}


void CRoiDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CRoiDlg)
   DDX_Control(pDX, IDC_ROI_Y, m_YEdit);
   DDX_Control(pDX, IDC_ROI_X, m_XEdit);
   DDX_Control(pDX, IDC_ROI_Width, m_WidthEdit);
   DDX_Control(pDX, IDC_ROI_Height, m_HeightEdit);
   DDX_Control(pDX, IDC_SPIN_Y, m_SpinY);
   DDX_Control(pDX, IDC_SPIN_X, m_SpinX);
   DDX_Control(pDX, IDC_SPIN_Width, m_SpinWidth);
   DDX_Control(pDX, IDC_SPIN_Height, m_SpinHeight);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_ROI_X,      m_Roi.x     );
   DDX_Text(pDX, IDC_ROI_Y,      m_Roi.y     );
   DDX_Text(pDX, IDC_ROI_Width,  m_Roi.width );
   DDX_Text(pDX, IDC_ROI_Height, m_Roi.height);
   DDV_MinMaxInt(pDX, m_Roi.x     , 0, GetMaxX());
   DDV_MinMaxInt(pDX, m_Roi.y     , 0, GetMaxY());
   DDV_MinMaxInt(pDX, m_Roi.width , 1, GetMaxWidth());
   DDV_MinMaxInt(pDX, m_Roi.height, 1, GetMaxHeight());
}


BEGIN_MESSAGE_MAP(CRoiDlg, CDialog)
   //{{AFX_MSG_MAP(CRoiDlg)
   ON_EN_KILLFOCUS(IDC_ROI_Height, OnKillfocusROIHeight)
   ON_EN_KILLFOCUS(IDC_ROI_Width, OnKillfocusROIWidth)
   ON_EN_KILLFOCUS(IDC_ROI_X, OnKillfocusRoiX)
   ON_EN_KILLFOCUS(IDC_ROI_Y, OnKillfocusRoiY)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoiDlg message handlers

BOOL CRoiDlg::OnInitDialog()
{
   ASSERT(m_pDoc);
    CImage* image = m_pDoc->GetImage();
    const IppiRect*   roi = image->GetRoi();
    if (roi) {
        m_Roi.x = roi->x;
        m_Roi.y = roi->y;
        m_Roi.width  = roi->width ;
        m_Roi.height = roi->height;
    } else if (image->Width() > 8 && image->Height() > 8) {
        m_Roi.x = 8;
        m_Roi.y = 8;
        m_Roi.width  = image->Width()  - 16;
        m_Roi.height = image->Height() - 16;
    } else {
        m_Roi.x = 0;
        m_Roi.y = 0;
        m_Roi.width  = image->Width() ;
        m_Roi.height = image->Height();
    }
    m_ImageWidth  = image->Width() ;
    m_ImageHeight = image->Height();

   CDialog::OnInitDialog();

    UpdateRange();
   return TRUE;
}

void CRoiDlg::OnOK()
{
   CDialog::OnOK();
    m_pDoc->GetImage()->SetRoi(&m_Roi);
    m_pDoc->UpdateView();
}

void CRoiDlg::OnCancel()
{
   CDialog::OnCancel();
}

void CRoiDlg::UpdateRange()
{
    m_SpinX.SetRange(0,GetMaxX());
    m_SpinY.SetRange(0,GetMaxY());
    m_SpinWidth.SetRange(1,GetMaxWidth());
    m_SpinHeight.SetRange(1,GetMaxHeight());
}

int CRoiDlg::GetMaxX() {
    int max = m_ImageWidth  - m_Roi.width;
    if (max < 0) max = 0;
    return max;
}

int CRoiDlg::GetMaxY() {
   int max = m_ImageHeight - m_Roi.height;
    if (max < 0) max = 0;
    return max;
}

int CRoiDlg::GetMaxWidth() {
   int max = m_ImageWidth  - m_Roi.x;
    if (max < 1) max = 1;
    return max;
}

int CRoiDlg::GetMaxHeight() {
   int max = m_ImageHeight - m_Roi.y;
    if (max < 1) max = 1;
    return max;
}

static int GetInt(CWnd* pWnd)
{
    CString text;
    pWnd->GetWindowText(text);
    return atoi(text);
}

static void UpdateValue(int& value, int min, int max,
                        CSpinButtonCtrl* pSpin)
{
//    if (value < min) value = min;
//    if (value > max) value = max;
    if (pSpin)
        pSpin->SetRange(min,max);
}

static void UpdateValue(int& value, int min, int max)
{
    UpdateValue(value, min, max, NULL);
}

void CRoiDlg::OnKillfocusROIHeight()
{
    m_Roi.height = GetInt(&m_HeightEdit);
    m_Roi.y = GetInt(&m_YEdit);
    UpdateValue(m_Roi.height, 1, GetMaxHeight());
    UpdateValue(m_Roi.y, 0, GetMaxY(), &m_SpinY);
    UpdateData(FALSE);
}

void CRoiDlg::OnKillfocusRoiY()
{
    m_Roi.height = GetInt(&m_HeightEdit);
    m_Roi.y = GetInt(&m_YEdit);
    UpdateValue(m_Roi.y, 0, GetMaxY());
    UpdateValue(m_Roi.height, 1, GetMaxHeight(), &m_SpinHeight);
    UpdateData(FALSE);
}

void CRoiDlg::OnKillfocusROIWidth()
{
    m_Roi.width = GetInt(&m_WidthEdit);
    m_Roi.x = GetInt(&m_XEdit);
    UpdateValue(m_Roi.width, 1, GetMaxWidth());
    UpdateValue(m_Roi.x, 0, GetMaxX(), &m_SpinX);
    UpdateData(FALSE);
}

void CRoiDlg::OnKillfocusRoiX()
{
    m_Roi.width = GetInt(&m_WidthEdit);
    m_Roi.x = GetInt(&m_XEdit);
    UpdateValue(m_Roi.x, 0, GetMaxX());
    UpdateValue(m_Roi.width, 1, GetMaxWidth(), &m_SpinWidth);
    UpdateData(FALSE);
}
