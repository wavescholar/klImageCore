/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeCDlg.cpp : implementation of the CParmResizeCDlg class.
// CParmResizeCDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ippiRun.h"
#include "ParmResizeCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmResizeCDlg dialog


CParmResizeCDlg::CParmResizeCDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParmResizeDlg(nID, pParent)
{
   //{{AFX_DATA_INIT(CParmResizeCDlg)
   m_CenterType = -1;
   //}}AFX_DATA_INIT
   m_CenterStr[0] = _T("");
   m_CenterStr[1] = _T("");
   m_Mode = CENTER;
}


void CParmResizeCDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmResizeDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmResizeCDlg)
   DDX_Radio(pDX, IDC_CTYPE_0, m_CenterType);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_SHIFT0, m_CenterEdit[0]);
   DDX_Control(pDX, IDC_SHIFT1, m_CenterEdit[1]);
   DDX_Text(pDX, IDC_SHIFT0, m_CenterStr[0]);
   DDX_Text(pDX, IDC_SHIFT1, m_CenterStr[1]);
}


BEGIN_MESSAGE_MAP(CParmResizeCDlg, CParmResizeDlg)
   //{{AFX_MSG_MAP(CParmResizeCDlg)
   ON_BN_CLICKED(IDC_CTYPE_0, OnCtype)
   ON_BN_CLICKED(IDC_CTYPE_1, OnCtype)
   ON_BN_CLICKED(IDC_CTYPE_2, OnCtype)
   //}}AFX_MSG_MAP
   ON_EN_CHANGE(IDC_FRACT_0, OnChangeFract)
   ON_EN_CHANGE(IDC_FRACT_1, OnChangeFract)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmResizeCDlg message handlers

BOOL CParmResizeCDlg::OnInitDialog()
{
   m_Mode = m_Func.Found("Center") ? CENTER
          : m_Func.Found("Shift")  ? SHIFT
          :                          SQR;
   CParmResizeDlg::OnInitDialog();

    UpdateCType();
    EnableCenter();
    FormatCenter();
    UpdateCenter();
 
    if (m_Mode == SHIFT) {
       GetDlgItem(IDC_SHIFT_STATIC)->SetWindowText("Src ROI Shift");
       GetDlgItem(IDC_CTYPE_1)->SetWindowText("Quater ROI");
       GetDlgItem(IDC_INTER_3)->EnableWindow(FALSE);
       if (m_Inter == 3) m_Inter = 0;
    } else if (m_Mode == SQR) {
       GetDlgItem(IDC_SHIFT_STATIC)->SetWindowText("Dst ROI Shift");
       GetDlgItem(IDC_CTYPE_1)->SetWindowText("Clip to bottom-left");
    }

   UpdateMyData(FALSE);
   return TRUE;
}

void CParmResizeCDlg::OnOK()
{
   // TODO: Add extra validation here

   CParmResizeDlg::OnOK();
}

void CParmResizeCDlg::OnCtype()
{
    UpdateMyData();
    EnableCenter();
    UpdateCenter();
    UpdateMyData(FALSE);
}

void CParmResizeCDlg::UpdateCType()
{
    BOOL isCenter = m_pDocSrc->GetImage()->IsCenterShift();
    GetDlgItem(IDC_CTYPE_0)->EnableWindow(isCenter);
    if (isCenter) {
        if (m_CenterType == GEOM_ROI) m_CenterType = GEOM_CONTOUR;
    } else {
        if (m_CenterType == GEOM_CONTOUR) m_CenterType = GEOM_ROI;
    }
}

void CParmResizeCDlg::EnableCenter()
{ 
    BOOL flag = m_CenterType < GEOM_CUSTOM;
    for (int i=0; i<2; i++)
        m_CenterEdit[i].SetReadOnly(flag);
}

void CParmResizeCDlg::UpdateCenter()
{
   if (m_CenterType == GEOM_CUSTOM) return;
   if (m_CenterType == GEOM_CONTOUR) {
      double shift[2] = {0, 0};
      if (m_Mode == CENTER)
         m_pDocSrc->GetImage()->GetCenterShift(m_Center, shift);
      else
         m_pDocSrc->GetImage()->GetCenterShift(shift, m_Center);
   } else {
      IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
      if (m_Mode == CENTER) {
         m_Center[0] = roi.x + ((roi.width *2)/5);
         m_Center[1] = roi.y + ((roi.height*2)/5);
      } else if (m_Mode == SHIFT) {
         m_Center[0] = roi.width>>2;
         m_Center[1] = roi.height>>2;
      } else {
         m_Center[0] = -(int)(roi.x*m_Fract[0] + 0.5);
         m_Center[1] = -(int)(roi.y*m_Fract[1] + 0.5);
      }
   }
}

void CParmResizeCDlg::FormatCenter()
{
    for (int i=0; i<2; i++) {
        m_CenterStr[i].Format("%.2f",m_Center[i]);
    }
}

void CParmResizeCDlg::UpdateMyData(BOOL save)
{
   if (save) {
       CParmResizeDlg::UpdateMyData(save);
       m_Center[0] = atof(m_CenterStr[0]);
       m_Center[1] = atof(m_CenterStr[1]);
   } else {
       if (m_CenterType != GEOM_CUSTOM)
           FormatCenter();
       CParmResizeDlg::UpdateMyData(save);
   }
}

void CParmResizeCDlg::OnChangeFract()
{
   if (m_bInit) {
      UpdateMyData();
      UpdateCenter();
      UpdateMyData(FALSE);
   }
}

