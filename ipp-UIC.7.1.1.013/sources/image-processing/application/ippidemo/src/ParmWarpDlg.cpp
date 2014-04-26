/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWarpDlg.cpp : implementation of the CParmWarpDlg class.
// CParmWarpDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "QuadDlg.h"
#include "RunWarp.h"
#include "ParmWarpDlg.h"
#include "ippiRun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {inter_nn, inter_linear, inter_cubic, inter_catmulrom}; 

/////////////////////////////////////////////////////////////////////////////
// CParmWarpDlg dialog

CParmWarpDlg::CParmWarpDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmWarpDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmWarpDlg)
   m_Inter = -1;
   m_Smooth = FALSE;
   m_Type = -1;
   //}}AFX_DATA_INIT
    m_RoiImage = NULL;
    m_QuadImage = NULL;
    m_I = 0;
    m_J = 0;
}


void CParmWarpDlg::DoDataExchange(CDataExchange* pDX)
{
    CParamDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CParmWarpDlg)
    DDX_Radio(pDX, IDC_INTER_0, m_Inter);
    DDX_Check(pDX, IDC_SMOOTH, m_Smooth);
    DDX_Radio(pDX, IDC_TYPE_0, m_Type);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_TYPE_0, m_TypeButton[0]);
    DDX_Control(pDX, IDC_TYPE_1, m_TypeButton[1]);
    DDX_Control(pDX, IDC_TYPE_2, m_TypeButton[2]);
    DDX_Control(pDX, IDC_TYPE_3, m_TypeButton[3]);

    DDX_Control(pDX, IDC_COEF_00, m_CoefEdit[0][0]);
    DDX_Control(pDX, IDC_COEF_01, m_CoefEdit[0][1]);
    DDX_Control(pDX, IDC_COEF_02, m_CoefEdit[0][2]);
    DDX_Control(pDX, IDC_COEF_03, m_CoefEdit[0][3]);
    DDX_Control(pDX, IDC_COEF_10, m_CoefEdit[1][0]);
    DDX_Control(pDX, IDC_COEF_11, m_CoefEdit[1][1]);
    DDX_Control(pDX, IDC_COEF_12, m_CoefEdit[1][2]);
    DDX_Control(pDX, IDC_COEF_13, m_CoefEdit[1][3]);
    DDX_Control(pDX, IDC_COEF_20, m_CoefEdit[2][0]);
    DDX_Control(pDX, IDC_COEF_21, m_CoefEdit[2][1]);
    DDX_Control(pDX, IDC_COEF_22, m_CoefEdit[2][2]);
    DDX_Control(pDX, IDC_COEF_23, m_CoefEdit[2][3]);

    DDX_Text(pDX, IDC_COEF_00, m_CoefStr[0][0]);
    DDX_Text(pDX, IDC_COEF_01, m_CoefStr[0][1]);
    DDX_Text(pDX, IDC_COEF_02, m_CoefStr[0][2]);
    DDX_Text(pDX, IDC_COEF_03, m_CoefStr[0][3]);
    DDX_Text(pDX, IDC_COEF_10, m_CoefStr[1][0]);
    DDX_Text(pDX, IDC_COEF_11, m_CoefStr[1][1]);
    DDX_Text(pDX, IDC_COEF_12, m_CoefStr[1][2]);
    DDX_Text(pDX, IDC_COEF_13, m_CoefStr[1][3]);
    DDX_Text(pDX, IDC_COEF_20, m_CoefStr[2][0]);
    DDX_Text(pDX, IDC_COEF_21, m_CoefStr[2][1]);
    DDX_Text(pDX, IDC_COEF_22, m_CoefStr[2][2]);
    DDX_Text(pDX, IDC_COEF_23, m_CoefStr[2][3]);

    DDX_Text(pDX, IDC_QUAD_00, m_QuadStr[0][0]);
    DDX_Text(pDX, IDC_QUAD_01, m_QuadStr[0][1]);
    DDX_Text(pDX, IDC_QUAD_10, m_QuadStr[1][0]);
    DDX_Text(pDX, IDC_QUAD_11, m_QuadStr[1][1]);
    DDX_Text(pDX, IDC_QUAD_20, m_QuadStr[2][0]);
    DDX_Text(pDX, IDC_QUAD_21, m_QuadStr[2][1]);
    DDX_Text(pDX, IDC_QUAD_30, m_QuadStr[3][0]);
    DDX_Text(pDX, IDC_QUAD_31, m_QuadStr[3][1]);

    DDX_Control(pDX, IDC_INTER_3, m_CatmullromButton);
}


BEGIN_MESSAGE_MAP(CParmWarpDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmWarpDlg)
   ON_BN_CLICKED(IDC_TYPE_0, OnType)
   ON_EN_CHANGE(IDC_COEF_00, OnChangeCoef)
   ON_BN_CLICKED(IDC_TYPE_1, OnType)
   ON_BN_CLICKED(IDC_TYPE_2, OnType)
   ON_EN_CHANGE(IDC_COEF_01, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_02, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_03, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_10, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_11, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_12, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_13, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_20, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_21, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_22, OnChangeCoef)
   ON_EN_CHANGE(IDC_COEF_23, OnChangeCoef)
   ON_BN_CLICKED(IDC_TYPE_3, OnType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmWarpDlg message handlers

BOOL CParmWarpDlg::OnInitDialog()
{
   ASSERT(m_I);
   ASSERT(m_J);

   UpdateInterpolation(FALSE);
   if (m_pDocDst == NULL) m_pDocDst = m_pDocSrc;
   if (!m_Func.Found("Back")) {
      m_RoiImage  = m_pDocSrc->GetImage();
      m_QuadImage = m_pDocDst->GetImage();
   } else {
      m_RoiImage  = m_pDocDst->GetImage();
      m_QuadImage = m_pDocSrc->GetImage();
   }
   FormatQuad();
   CParamDlg::OnInitDialog();
   ShowCoef();
   UpdateType();
   EnableCoef();
   UpdateCoef();
   EnableInter();
   UpdateMyData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmWarpDlg::OnOK()
{
   UpdateMyData();
   CParamDlg::OnOK();
   UpdateInterpolation();
}

void CParmWarpDlg::UpdateInterpolation(BOOL bUpdate)
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

void CParmWarpDlg::EnableInter()
{
    if (m_Func.BaseName() == "WarpAffine")
        return;
    m_CatmullromButton.EnableWindow(FALSE);
    if (m_Inter == inter_catmulrom)
        m_Inter = inter_cubic;
}


void CParmWarpDlg::UpdateType()
{
   if (m_NF == NF_AFF) {
      m_TypeButton[WARP_QUAD].EnableWindow(FALSE);
      if (m_Type == WARP_QUAD)
         m_Type = WARP_PARL;
   }
}

void CParmWarpDlg::OnWarpFlag()
{
    UpdateMyData();
    EnableCoef();
    UpdateCoef();
    UpdateMyData(FALSE);
}

void CParmWarpDlg::OnType()
{
    int oldType = m_Type;
    UpdateMyData();
    EnableCoef();
    UpdateCoef(oldType);
    UpdateMyData(FALSE);
}

void CParmWarpDlg::OnChangeCoef()
{
    UpdateMyData();
    QuadFromCoef();
    UpdateMyData(FALSE);
}

void CParmWarpDlg::ShowCoef()
{
    for (int i=0; i<3; i++) {
        for (int j=0; j<4; j++) {
            if (i >= m_I || j >= m_J)
                m_CoefEdit[i][j].ShowWindow(SW_HIDE);
        }
    }
}

void CParmWarpDlg::EnableCoef()
{
    BOOL flag = m_Type < WARP_CUSTOM;
    for (int i=0; i<m_I; i++)
        for (int j=0; j<m_J; j++)
            m_CoefEdit[i][j].SetReadOnly(flag);
}

void CParmWarpDlg::UpdateCoef(int oldType)
{
    QuadFromContour();
    if (CoefFromQuad(oldType >= 0)) {
        QuadFromCoef();
        return;
    }
    if (oldType >= 0)
        m_Type = oldType;
    else
        m_Type++;
    UpdateCoef(oldType);
}

void CParmWarpDlg::QuadFromContour()
{
   ASSERT(m_QuadImage);
   if (m_Type == WARP_CUSTOM) return;
   IppiRect roi = m_QuadImage->GetActualRoi();
   if (m_Type == WARP_QUAD) {
      if (!m_QuadImage->GetQuad(m_Quad))
         CQuadDlg::GetDefaultQuad(roi,m_Quad);
   } else if (m_Type == WARP_PARL) {
      if (!m_QuadImage->GetParl(m_Quad))
         CQuadDlg::GetDefaultParl(roi,m_Quad);
   }
   FormatQuad();
}

BOOL CParmWarpDlg::CoefFromQuad(BOOL errorBox)
{
    ASSERT(m_RoiImage);
    if (m_Type == WARP_CUSTOM) return TRUE;
    IppiRect roi = m_RoiImage->GetActualRoi();
    IppStatus result = ippStsNoErr;
    switch (m_NF) {
    case NF_AFF:
        result = CALL(ippiGetAffineTransform,(roi,m_Quad,m_CoefAff));
        break;
    case NF_BIL:
        result = CALL(ippiGetBilinearTransform,(roi,m_Quad,m_CoefBil));
        break;
    case NF_PER:
        result = CALL(ippiGetPerspectiveTransform,(roi,m_Quad,m_CoefPer));
        break;
    }
    if (result) {
        if (errorBox) {
            FormatCoef(FALSE);
            UpdateData(FALSE);
            CippiRun::IppErrorMessage(m_TransName, result);
        }
    } else {
        FormatCoef();
    }
    return !result;
}

void CParmWarpDlg::QuadFromCoef()
{
    ASSERT(m_RoiImage);
    if (m_Type != WARP_CUSTOM) {
       FormatQuad();
       return;
    }
    IppiRect roi = m_RoiImage->GetActualRoi();
    IppStatus result = ippStsNoErr;
    switch (m_NF) {
    case NF_AFF:
        result = CALL(ippiGetAffineQuad,(roi,m_Quad,m_CoefAff));
        break;
    case NF_BIL:
        result = CALL(ippiGetBilinearQuad,(roi,m_Quad,m_CoefBil));
        break;
    case NF_PER:
        result = CALL(ippiGetPerspectiveQuad,(roi,m_Quad,m_CoefPer));
        break;
    }
    if (result) {
        FormatQuad(FALSE);
        UpdateData(FALSE);
    } else {
        FormatQuad();
    }
}

void CParmWarpDlg::UpdateMyData(BOOL save)
{
    if (save) {
        UpdateData();
        SetQuad();
        SetCoef();
    } else {
        UpdateData(FALSE);
    }
}

void CParmWarpDlg::SetQuad()
{    for (int i=0; i<4; i++) {
        for (int j=0; j<2; j++) {
            m_Quad[i][j] = atof(m_QuadStr[i][j]);
        }
    }
}

void CParmWarpDlg::FormatQuad(BOOL show)
{
    for (int i=0; i<4; i++) {
        for (int j=0; j<2; j++) {
            if (show)
                m_QuadStr[i][j].Format("%.1f",m_Quad[i][j]);
            else
                m_QuadStr[i][j] = "";
        }
    }
}

void CParmWarpDlg::SetCoef()
{
    for (int i=0; i<m_I; i++) {
        for (int j=0; j<m_J; j++) {
            double val = atof(m_CoefStr[i][j]);
            switch (m_NF) {
            case NF_AFF: m_CoefAff[i][j] = val; break;
            case NF_BIL: m_CoefBil[i][j] = val; break;
            case NF_PER: m_CoefPer[i][j] = val; break;
            }
        }
    }
}

void CParmWarpDlg::FormatCoef(BOOL show)
{
    for (int i=0; i<m_I; i++) {
        for (int j=0; j<m_J; j++) {
            double val;
            switch (m_NF) {
            case NF_AFF: val = m_CoefAff[i][j]; break;
            case NF_BIL: val = m_CoefBil[i][j]; break;
            case NF_PER: val = m_CoefPer[i][j]; break;
            }
            if (show)
                m_CoefStr[i][j].Format("%.3g",val);
            else
                m_CoefStr[i][j] = "";
        }
    }
}


void CParmWarpDlg::OnBnClickedInter8()
{
    // TODO: Add your control notification handler code here
}
