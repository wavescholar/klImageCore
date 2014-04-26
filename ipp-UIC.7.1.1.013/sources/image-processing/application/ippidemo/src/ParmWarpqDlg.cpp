/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWarpqDlg.cpp : implementation of the CParmWarpqDlg class.
// CParmWarpqDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "QuadDlg.h"
#include "RunWarp.h"
#include "ParmWarpqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmWarpqDlg dialog


CParmWarpqDlg::CParmWarpqDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmWarpqDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmWarpqDlg)
   m_Inter = -1;
   m_Smooth = FALSE;
   m_SType = -1;
   m_DType = -1;
   //}}AFX_DATA_INIT
}


void CParmWarpqDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmWarpqDlg)
   DDX_Radio(pDX, IDC_INTER_0, m_Inter);
   DDX_Check(pDX, IDC_SMOOTH, m_Smooth);
   DDX_Radio(pDX, IDC_STYPE_0, m_SType);
   DDX_Radio(pDX, IDC_DTYPE_0, m_DType);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_STYPE_0, m_STypeButton[0]);
   DDX_Control(pDX, IDC_STYPE_1, m_STypeButton[1]);
   DDX_Control(pDX, IDC_STYPE_2, m_STypeButton[2]);
   DDX_Control(pDX, IDC_STYPE_3, m_STypeButton[3]);
   DDX_Control(pDX, IDC_DTYPE_0, m_DTypeButton[0]);
   DDX_Control(pDX, IDC_DTYPE_1, m_DTypeButton[1]);
   DDX_Control(pDX, IDC_DTYPE_2, m_DTypeButton[2]);
   DDX_Control(pDX, IDC_DTYPE_3, m_DTypeButton[3]);
   DDX_Control(pDX, IDC_SQUAD_00, m_SQuadEdit[0][0]);
   DDX_Control(pDX, IDC_SQUAD_01, m_SQuadEdit[0][1]);
   DDX_Control(pDX, IDC_SQUAD_10, m_SQuadEdit[1][0]);
   DDX_Control(pDX, IDC_SQUAD_11, m_SQuadEdit[1][1]);
   DDX_Control(pDX, IDC_SQUAD_20, m_SQuadEdit[2][0]);
   DDX_Control(pDX, IDC_SQUAD_21, m_SQuadEdit[2][1]);
   DDX_Control(pDX, IDC_SQUAD_30, m_SQuadEdit[3][0]);
   DDX_Control(pDX, IDC_SQUAD_31, m_SQuadEdit[3][1]);
   DDX_Text(pDX, IDC_SQUAD_00, m_SQuadStr[0][0]);
   DDX_Text(pDX, IDC_SQUAD_01, m_SQuadStr[0][1]);
   DDX_Text(pDX, IDC_SQUAD_10, m_SQuadStr[1][0]);
   DDX_Text(pDX, IDC_SQUAD_11, m_SQuadStr[1][1]);
   DDX_Text(pDX, IDC_SQUAD_20, m_SQuadStr[2][0]);
   DDX_Text(pDX, IDC_SQUAD_21, m_SQuadStr[2][1]);
   DDX_Text(pDX, IDC_SQUAD_30, m_SQuadStr[3][0]);
   DDX_Text(pDX, IDC_SQUAD_31, m_SQuadStr[3][1]);

   DDX_Control(pDX, IDC_DQUAD_00, m_DQuadEdit[0][0]);
   DDX_Control(pDX, IDC_DQUAD_01, m_DQuadEdit[0][1]);
   DDX_Control(pDX, IDC_DQUAD_10, m_DQuadEdit[1][0]);
   DDX_Control(pDX, IDC_DQUAD_11, m_DQuadEdit[1][1]);
   DDX_Control(pDX, IDC_DQUAD_20, m_DQuadEdit[2][0]);
   DDX_Control(pDX, IDC_DQUAD_21, m_DQuadEdit[2][1]);
   DDX_Control(pDX, IDC_DQUAD_30, m_DQuadEdit[3][0]);
   DDX_Control(pDX, IDC_DQUAD_31, m_DQuadEdit[3][1]);
   DDX_Text(pDX, IDC_DQUAD_00, m_DQuadStr[0][0]);
   DDX_Text(pDX, IDC_DQUAD_01, m_DQuadStr[0][1]);
   DDX_Text(pDX, IDC_DQUAD_10, m_DQuadStr[1][0]);
   DDX_Text(pDX, IDC_DQUAD_11, m_DQuadStr[1][1]);
   DDX_Text(pDX, IDC_DQUAD_20, m_DQuadStr[2][0]);
   DDX_Text(pDX, IDC_DQUAD_21, m_DQuadStr[2][1]);
   DDX_Text(pDX, IDC_DQUAD_30, m_DQuadStr[3][0]);
   DDX_Text(pDX, IDC_DQUAD_31, m_DQuadStr[3][1]);
}


BEGIN_MESSAGE_MAP(CParmWarpqDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmWarpqDlg)
   ON_BN_CLICKED(IDC_STYPE_0, OnSType)
   ON_BN_CLICKED(IDC_STYPE_1, OnSType)
   ON_BN_CLICKED(IDC_STYPE_2, OnSType)
   ON_BN_CLICKED(IDC_DTYPE_0, OnDType)
   ON_BN_CLICKED(IDC_DTYPE_1, OnDType)
   ON_BN_CLICKED(IDC_DTYPE_2, OnDType)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmWarpqDlg message handlers

void CParmWarpqDlg::UpdateTypes()
{
   if (m_Func.Found("Affine")) {
       m_STypeButton[WARP_QUAD].EnableWindow(FALSE);
       m_DTypeButton[WARP_QUAD].EnableWindow(FALSE);
       if (m_SType == WARP_QUAD || m_DType == WARP_QUAD) {
          if (m_SType == m_DType) {
             m_SType = m_DType = WARP_PARL;
          } else if (m_DType == WARP_QUAD) {
             m_DType = WARP_PARL;
             m_SType = WARP_ROI;
          } else {
             m_SType = WARP_PARL;
             m_DType = WARP_ROI;
          }
       }
   }
}

static void EnableQuad(int type, CEdit quadEdit[4][2])
{
    BOOL flag = type < WARP_CUSTOM;
    for (int i=0; i<4; i++)
        for (int j=0; j<2; j++)
            quadEdit[i][j].SetReadOnly(flag);
}

static void UpdateQuad(CImage* pImage, int type, double quad[4][2])
{
   if (type == WARP_CUSTOM) return;
   if (type == WARP_QUAD) {
      if (!pImage->GetQuad(quad))
         CQuadDlg::GetDefaultQuad(pImage->GetActualRoi(), quad);
   } else if (type == WARP_PARL) {
      if (!pImage->GetParl(quad))
         CQuadDlg::GetDefaultParl(pImage->GetActualRoi(), quad);
   } else {
      IppiRect roi = pImage->GetActualRoi();
      quad[0][0] = roi.x;
      quad[0][1] = roi.y;
      quad[2][0] = quad[0][0] + roi.width  - 1;
      quad[2][1] = quad[0][1] + roi.height - 1;
      quad[1][0] = quad[2][0];
      quad[1][1] = quad[0][1];
      quad[3][0] = quad[0][0];
      quad[3][1] = quad[2][1];
   }
}

static void QuadToString(double quad[4][2], CString quadString[4][2])
{
    for (int i=0; i<4; i++)
        for (int j=0; j<2; j++)
            quadString[i][j].Format("%.1f",quad[i][j]);
}

static void StringToQuad(CString quadString[4][2], double quad[4][2])
{
   for (int i=0; i<4; i++)
      for (int j=0; j<2; j++)
         quad[i][j] = atof(quadString[i][j]);
}

BOOL CParmWarpqDlg::OnInitDialog()
{
    if (m_pDocDst == NULL) m_pDocDst = m_pDocSrc;
    QuadToString(m_srcQuad, m_SQuadStr);
    QuadToString(m_dstQuad, m_DQuadStr);
    CParamDlg::OnInitDialog();
    UpdateTypes();
    EnableQuad(m_SType,m_SQuadEdit);
    EnableQuad(m_DType,m_DQuadEdit);
    UpdateQuad(m_pDocSrc->GetImage(),m_SType,m_srcQuad);
    UpdateQuad(m_pDocDst->GetImage(),m_DType,m_dstQuad);
    UpdateMyData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmWarpqDlg::OnOK()
{
    UpdateMyData();
   CParamDlg::OnOK();
}

void CParmWarpqDlg::OnSType()
{
    UpdateMyData();
    EnableQuad(m_SType,m_SQuadEdit);
    UpdateQuad(m_pDocSrc->GetImage(),m_SType,m_srcQuad);
    UpdateMyData(FALSE);
}

void CParmWarpqDlg::OnDType()
{
    UpdateMyData();
    EnableQuad(m_DType,m_DQuadEdit);
    UpdateQuad(m_pDocDst->GetImage(),m_DType,m_dstQuad);
    UpdateMyData(FALSE);
}

void CParmWarpqDlg::UpdateMyData(BOOL save)
{
    if (save) {
        UpdateData();
        StringToQuad(m_SQuadStr, m_srcQuad);
        StringToQuad(m_DQuadStr, m_dstQuad);
    } else {
        if (m_SType != WARP_CUSTOM)
            QuadToString(m_srcQuad, m_SQuadStr);
        if (m_DType != WARP_CUSTOM)
            QuadToString(m_dstQuad, m_DQuadStr);
        UpdateData(FALSE);
    }
}
        
