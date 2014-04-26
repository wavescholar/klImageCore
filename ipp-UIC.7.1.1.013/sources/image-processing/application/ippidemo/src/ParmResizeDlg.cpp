/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeDlg.cpp : implementation of the CParmResizeDlg class.
// CParmResizeDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ippiRun.h"
#include "ParmResizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmResizeDlg dialog


CParmResizeDlg::CParmResizeDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParamDlg(nID, pParent)
{
   //{{AFX_DATA_INIT(CParmResizeDlg)
   m_Inter = -1;
   m_Type = -1;
   //}}AFX_DATA_INIT
   m_FractStr[0] = _T("");
   m_FractStr[1] = _T("");
}


void CParmResizeDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmResizeDlg)
   DDX_Radio(pDX, IDC_INTER_0, m_Inter);
   DDX_Radio(pDX, IDC_TYPE_0, m_Type);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_FRACT_0, m_FractStr[0]);
   DDX_Text(pDX, IDC_FRACT_1, m_FractStr[1]);
   DDX_Control(pDX, IDC_FRACT_0, m_FractEdit[0]);
   DDX_Control(pDX, IDC_FRACT_1, m_FractEdit[1]);
}


BEGIN_MESSAGE_MAP(CParmResizeDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmResizeDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmResizeDlg message handlers

BOOL CParmResizeDlg::OnInitDialog()
{
    FormatFract();
    CParamDlg::OnInitDialog();
    UpdateType();
    EnableFract();
    UpdateFract();
    UpdateMyData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmResizeDlg::OnOK()
{
   UpdateMyData();
   for (int i=0; i<2; i++) {
      if (m_Fract[i] == 0) {
         AfxMessageBox("Bad fraction");
         m_FractEdit[i].SetFocus();
         return;
      }
   }
   CParamDlg::OnOK();
}

void CParmResizeDlg::OnType()
{
    UpdateMyData();
    EnableFract();
    UpdateFract();
    UpdateMyData(FALSE);
}

void CParmResizeDlg::UpdateType()
{
    GetDlgItem(IDC_TYPE_0)->EnableWindow((BOOL)m_pDocDst);
    if (m_pDocDst) {
        if (m_Type == GEOM_ROI) m_Type = GEOM_CONTOUR;
    } else {
        if (m_Type == GEOM_CONTOUR) m_Type = GEOM_ROI;
    }
}

void CParmResizeDlg::EnableFract()
{
    BOOL flag = m_Type < GEOM_CUSTOM;
    for (int i=0; i<2; i++)
        m_FractEdit[i].SetReadOnly(flag);
}

void CParmResizeDlg::UpdateFract()
{
    if (m_Type == GEOM_CUSTOM) return;
    if (m_Type == GEOM_CONTOUR) {
        IppiRect srcRoi = m_pDocSrc->GetImage()->GetActualRoi();
        IppiRect dstRoi = m_pDocDst->GetImage()->GetActualRoi();
        m_Fract[0] = (double)dstRoi.width / (double)srcRoi.width;
        m_Fract[1] = (double)dstRoi.height / (double)srcRoi.height;
    } else {
        m_Fract[0] = 0.5;
        m_Fract[1] = 0.5;
    }
}

static double getFract(CString str)
{
   int i = str.Find('/');
   if (i == -1) return atof(str);
   CString upStr = str.Left(i);
   CString lowStr = str.Mid(i+1);
   double up = atof(upStr);
   double low = atof(lowStr);
   if (low == 0) return 0.;
   return up/low;
}

void CParmResizeDlg::UpdateMyData(BOOL save)
{
    if (save) {
        UpdateData();
        for (int i=0; i<2; i++)
            m_Fract[i] = getFract(m_FractStr[i]);
    } else {
        if (m_Type != GEOM_CUSTOM)
            FormatFract();
        UpdateData(FALSE);
    }
}

void CParmResizeDlg::FormatFract()
{
    for (int i=0; i<2; i++)
        m_FractStr[i].Format("%.2f",m_Fract[i]);
}
        
