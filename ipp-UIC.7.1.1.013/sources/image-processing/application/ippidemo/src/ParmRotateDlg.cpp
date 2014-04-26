/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRotateDlg.cpp : implementation of the CParmRotateDlg class.
// CParmRotateDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmRotateDlg.h"
#include "ippiRun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {inter_nn, inter_linear, inter_cubic, inter_catmulrom}; 

/////////////////////////////////////////////////////////////////////////////
// CParmRotateDlg dialog


CParmRotateDlg::CParmRotateDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmRotateDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmRotateDlg)
   m_AngleStr = _T("");
   m_Inter = -1;
   m_Smooth = FALSE;
   m_Type = -1;
   //}}AFX_DATA_INIT
}


void CParmRotateDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmRotateDlg)
   DDX_Control(pDX, IDC_SHIFT_STATIC, m_ShiftStatic);
   DDX_Text(pDX, IDC_ANGLE, m_AngleStr);
   DDX_Radio(pDX, IDC_INTER_0, m_Inter);
   DDX_Check(pDX, IDC_SMOOTH, m_Smooth);
   DDX_Radio(pDX, IDC_TYPE_0, m_Type);
   //}}AFX_DATA_MAP
   DDX_Text(pDX, IDC_SHIFT0, m_ShiftStr[0]);
   DDX_Text(pDX, IDC_SHIFT1, m_ShiftStr[1]);
   DDX_Control(pDX, IDC_SHIFT0, m_ShiftEdit[0]);
   DDX_Control(pDX, IDC_SHIFT1, m_ShiftEdit[1]);
}


BEGIN_MESSAGE_MAP(CParmRotateDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmRotateDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_EN_CHANGE(IDC_ANGLE, OnChangeAngle)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmRotateDlg message handlers

BOOL CParmRotateDlg::OnInitDialog()
{
    UpdateInterpolation(FALSE);
    FormatAngle();
    FormatShift();
   CParamDlg::OnInitDialog();
    if (m_Func.Found("Center"))
       m_ShiftStatic.SetWindowText("Center");
    UpdateType();
    EnableShift();
    UpdateShift();
    UpdateMyData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmRotateDlg::OnOK()
{
    UpdateMyData();
   CParamDlg::OnOK();
    UpdateInterpolation();
}

void CParmRotateDlg::UpdateInterpolation(BOOL bUpdate)
{
    if (bUpdate)
    {
        m_interpolate = 0;
        switch (m_Inter)
        {
        case inter_nn       : m_interpolate = IPPI_INTER_NN                ; break;
        case inter_linear   : m_interpolate = IPPI_INTER_LINEAR            ; break;
        case inter_cubic    : m_interpolate = IPPI_INTER_CUBIC             ; break;
        case inter_catmulrom: m_interpolate = IPPI_INTER_CUBIC2P_CATMULLROM; break;
        }
        if (m_Smooth) m_interpolate += IPPI_SMOOTH_EDGE  ;
    }
    else
    {
        int inter = m_interpolate & 0xFFFF;
        int flags = m_interpolate - inter;
        switch (inter)
        {
        case IPPI_INTER_NN                : m_Inter = inter_nn       ; break;
        case IPPI_INTER_LINEAR            : m_Inter = inter_linear   ; break;
        case IPPI_INTER_CUBIC             : m_Inter = inter_cubic    ; break;
        case IPPI_INTER_CUBIC2P_CATMULLROM: m_Inter = inter_catmulrom; break;
        }
        m_Smooth = (flags & IPPI_SMOOTH_EDGE) ? TRUE : FALSE;
    }
}

void CParmRotateDlg::OnType()
{
    UpdateMyData();
    EnableShift();
    UpdateShift();
    UpdateMyData(FALSE);
}

void CParmRotateDlg::OnChangeAngle()
{
    UpdateMyData();
    UpdateShift();
    UpdateMyData(FALSE);
}

void CParmRotateDlg::UpdateType()
{
    BOOL isCenter = m_pDocSrc->GetImage()->IsCenterShift();
    GetDlgItem(IDC_TYPE_0)->EnableWindow(isCenter);
    if (isCenter) {
        if (m_Type == GEOM_ROI) m_Type = GEOM_CONTOUR;
    } else {
        if (m_Type == GEOM_CONTOUR) m_Type = GEOM_ROI;
    }
}

void CParmRotateDlg::EnableShift()
{
    BOOL flag = m_Type < GEOM_CUSTOM;
    for (int i=0; i<2; i++)
        m_ShiftEdit[i].SetReadOnly(flag);
}

void CParmRotateDlg::UpdateShift()
{
    if (m_Type == GEOM_CUSTOM) return;
    double center[2] = {0, 0};
    double shift[2] = {0, 0};
    if (m_Type == GEOM_CONTOUR) {
        m_pDocSrc->GetImage()->GetCenterShift(center, shift);
    } else {
        IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
        //center[0] = roi.x + (roi.width>>1);
        //center[1] = roi.y + (roi.height>>1);
        center[0] = roi.x + (roi.width-1)/2.0;
        center[1] = roi.y + (roi.height-1)/2.0;
    }
    if (m_Func.Found("Center")) {
       m_Shift[0] = center[0];
       m_Shift[1] = center[1];
       return;
    }
    IppStatus result = CALL(ippiAddRotateShift,(center[0], center[1], m_Angle,
                                         shift, shift+1));
    if (result) {
        CippiRun::IppErrorMessage("ippiAddRotateShift", result);
        m_Type = GEOM_CUSTOM;
        return;
    }
    m_Shift[0] = shift[0];
    m_Shift[1] = shift[1];
}

void CParmRotateDlg::UpdateMyData(BOOL save)
{
    if (save) {
        UpdateData();
        m_Shift[0] = atof(m_ShiftStr[0]);
        m_Shift[1] = atof(m_ShiftStr[1]);
        m_Angle = atof(m_AngleStr);
    } else {
        if (m_Type != GEOM_CUSTOM)
            FormatShift();
        UpdateData(FALSE);
    }
}

void CParmRotateDlg::FormatShift()
{
    for (int i=0; i<2; i++) {
        m_ShiftStr[i].Format("%.2f",m_Shift[i]);
    }
}

void CParmRotateDlg::FormatAngle()
{
    m_AngleStr.Format("%.2f",m_Angle);
}
