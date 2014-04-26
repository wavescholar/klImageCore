/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmWTDlg.cpp : implementation of the CParmWTDlg class.
// CParmWTDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmWTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmWTDlg dialog

static CString ApproxName[4] = {"Approx","DetailX","DetailY","DetailXY"};

CParmWTDlg::CParmWTDlg(CWnd* pParent /*=NULL*/)
   : CParamDlg(CParmWTDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CParmWarpqDlg)
   //}}AFX_DATA_INIT
   int i;
   for (i = 0; i<4; i++) {
      m_pDocSrcWT[i] = NULL;
      m_pDocDstWT[i] = NULL;
   }
   for (i=0; i<2; i++) {
      for (int x = 0; x < LEN; x++) {
         m_TapStr[i][x] = "0";
      }
   }
}


void CParmWTDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CParmWTDlg)
   DDX_Control(pDX, IDC_SRC_STATIC_0, m_SrcStatic0);
   DDX_Check(pDX, IDC_BORDER, m_bDstBorder);
   DDX_Control(pDX, IDC_DST_STATIC_0, m_DstStatic0);
   //}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_SRCFWD_STATIC, m_FwdFrame[0]);
   DDX_Control(pDX, IDC_DSTFWD_STATIC, m_FwdFrame[1]);
   DDX_Control(pDX, IDC_SRCINV_STATIC, m_InvFrame[0]);
   DDX_Control(pDX, IDC_DSTINV_STATIC, m_InvFrame[1]);

   DDX_Control(pDX, IDC_IS_SRC_1, m_IsSrcButton[1]);
   DDX_Control(pDX, IDC_IS_SRC_2, m_IsSrcButton[2]);
   DDX_Control(pDX, IDC_IS_SRC_3, m_IsSrcButton[3]);
   DDX_Control(pDX, IDC_IS_DST_1, m_IsDstButton[1]);
   DDX_Control(pDX, IDC_IS_DST_2, m_IsDstButton[2]);
   DDX_Control(pDX, IDC_IS_DST_3, m_IsDstButton[3]);

   DDX_Check(pDX, IDC_IS_SRC_1, m_IsSrcWT[1]);
   DDX_Check(pDX, IDC_IS_SRC_2, m_IsSrcWT[2]);
   DDX_Check(pDX, IDC_IS_SRC_3, m_IsSrcWT[3]);
   DDX_Check(pDX, IDC_IS_DST_1, m_IsDstWT[1]);
   DDX_Check(pDX, IDC_IS_DST_2, m_IsDstWT[2]);
   DDX_Check(pDX, IDC_IS_DST_3, m_IsDstWT[3]);

   DDX_Text(pDX, IDC_EDIT_SRC_0, m_StrSrcWT[0]);
   DDX_Text(pDX, IDC_EDIT_SRC_1, m_StrSrcWT[1]);
   DDX_Text(pDX, IDC_EDIT_SRC_2, m_StrSrcWT[2]);
   DDX_Text(pDX, IDC_EDIT_SRC_3, m_StrSrcWT[3]);
   DDX_Text(pDX, IDC_EDIT_DST_0, m_StrDstWT[0]);
   DDX_Text(pDX, IDC_EDIT_DST_1, m_StrDstWT[1]);
   DDX_Text(pDX, IDC_EDIT_DST_2, m_StrDstWT[2]);
   DDX_Text(pDX, IDC_EDIT_DST_3, m_StrDstWT[3]);

   DDX_Control(pDX, IDC_EDIT_SRC_0, m_EditSrcWT[0]);
   DDX_Control(pDX, IDC_EDIT_SRC_1, m_EditSrcWT[1]);
   DDX_Control(pDX, IDC_EDIT_SRC_2, m_EditSrcWT[2]);
   DDX_Control(pDX, IDC_EDIT_SRC_3, m_EditSrcWT[3]);
   DDX_Control(pDX, IDC_EDIT_DST_0, m_EditDstWT[0]);
   DDX_Control(pDX, IDC_EDIT_DST_1, m_EditDstWT[1]);
   DDX_Control(pDX, IDC_EDIT_DST_2, m_EditDstWT[2]);
   DDX_Control(pDX, IDC_EDIT_DST_3, m_EditDstWT[3]);

   DDX_Control(pDX, IDC_SIZE_SPIN_0, m_LenSpin[0]);
   DDX_Control(pDX, IDC_ANCHOR_SPIN_0, m_AnchorSpin[0]);
   DDX_Text(pDX, IDC_ANCHOR_0, m_AnchorStr[0]);
   DDX_Text(pDX, IDC_LEN_0, m_LenStr[0]);
   DDX_Control(pDX, IDC_SIZE_SPIN_1, m_LenSpin[1]);
   DDX_Control(pDX, IDC_ANCHOR_SPIN_1, m_AnchorSpin[1]);
   DDX_Text(pDX, IDC_ANCHOR_1, m_AnchorStr[1]);
   DDX_Text(pDX, IDC_LEN_1, m_LenStr[1]);

   DDX_Control(pDX, IDC_TAP_000, m_TapEdit[0][ 0]);
   DDX_Control(pDX, IDC_TAP_001, m_TapEdit[0][ 1]);
   DDX_Control(pDX, IDC_TAP_002, m_TapEdit[0][ 2]);
   DDX_Control(pDX, IDC_TAP_003, m_TapEdit[0][ 3]);
   DDX_Control(pDX, IDC_TAP_004, m_TapEdit[0][ 4]);
   DDX_Control(pDX, IDC_TAP_005, m_TapEdit[0][ 5]);
   DDX_Control(pDX, IDC_TAP_006, m_TapEdit[0][ 6]);
   DDX_Control(pDX, IDC_TAP_007, m_TapEdit[0][ 7]);
   DDX_Control(pDX, IDC_TAP_008, m_TapEdit[0][ 8]);
   DDX_Control(pDX, IDC_TAP_009, m_TapEdit[0][ 9]);
   DDX_Control(pDX, IDC_TAP_010, m_TapEdit[0][10]);
   DDX_Control(pDX, IDC_TAP_011, m_TapEdit[0][11]);

   DDX_Text(pDX, IDC_TAP_000, m_TapStr[0][ 0]);
   DDX_Text(pDX, IDC_TAP_001, m_TapStr[0][ 1]);
   DDX_Text(pDX, IDC_TAP_002, m_TapStr[0][ 2]);
   DDX_Text(pDX, IDC_TAP_003, m_TapStr[0][ 3]);
   DDX_Text(pDX, IDC_TAP_004, m_TapStr[0][ 4]);
   DDX_Text(pDX, IDC_TAP_005, m_TapStr[0][ 5]);
   DDX_Text(pDX, IDC_TAP_006, m_TapStr[0][ 6]);
   DDX_Text(pDX, IDC_TAP_007, m_TapStr[0][ 7]);
   DDX_Text(pDX, IDC_TAP_008, m_TapStr[0][ 8]);
   DDX_Text(pDX, IDC_TAP_009, m_TapStr[0][ 9]);
   DDX_Text(pDX, IDC_TAP_010, m_TapStr[0][10]);
   DDX_Text(pDX, IDC_TAP_011, m_TapStr[0][11]);

   DDX_Control(pDX, IDC_TAP_100, m_TapEdit[1][ 0]);
   DDX_Control(pDX, IDC_TAP_101, m_TapEdit[1][ 1]);
   DDX_Control(pDX, IDC_TAP_102, m_TapEdit[1][ 2]);
   DDX_Control(pDX, IDC_TAP_103, m_TapEdit[1][ 3]);
   DDX_Control(pDX, IDC_TAP_104, m_TapEdit[1][ 4]);
   DDX_Control(pDX, IDC_TAP_105, m_TapEdit[1][ 5]);
   DDX_Control(pDX, IDC_TAP_106, m_TapEdit[1][ 6]);
   DDX_Control(pDX, IDC_TAP_107, m_TapEdit[1][ 7]);
   DDX_Control(pDX, IDC_TAP_108, m_TapEdit[1][ 8]);
   DDX_Control(pDX, IDC_TAP_109, m_TapEdit[1][ 9]);
   DDX_Control(pDX, IDC_TAP_110, m_TapEdit[1][10]);
   DDX_Control(pDX, IDC_TAP_111, m_TapEdit[1][11]);

   DDX_Text(pDX, IDC_TAP_100, m_TapStr[1][ 0]);
   DDX_Text(pDX, IDC_TAP_101, m_TapStr[1][ 1]);
   DDX_Text(pDX, IDC_TAP_102, m_TapStr[1][ 2]);
   DDX_Text(pDX, IDC_TAP_103, m_TapStr[1][ 3]);
   DDX_Text(pDX, IDC_TAP_104, m_TapStr[1][ 4]);
   DDX_Text(pDX, IDC_TAP_105, m_TapStr[1][ 5]);
   DDX_Text(pDX, IDC_TAP_106, m_TapStr[1][ 6]);
   DDX_Text(pDX, IDC_TAP_107, m_TapStr[1][ 7]);
   DDX_Text(pDX, IDC_TAP_108, m_TapStr[1][ 8]);
   DDX_Text(pDX, IDC_TAP_109, m_TapStr[1][ 9]);
   DDX_Text(pDX, IDC_TAP_110, m_TapStr[1][10]);
   DDX_Text(pDX, IDC_TAP_111, m_TapStr[1][11]);
}


BEGIN_MESSAGE_MAP(CParmWTDlg, CParamDlg)
   //{{AFX_MSG_MAP(CParmWTDlg)
   ON_EN_CHANGE(IDC_TAP_000, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_100, OnChangeTap1)
   ON_EN_CHANGE(IDC_LEN_0, OnChangeLen0)
   ON_EN_CHANGE(IDC_LEN_1, OnChangeLen1)
   ON_EN_CHANGE(IDC_TAP_001, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_002, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_003, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_004, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_005, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_006, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_007, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_008, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_009, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_010, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_011, OnChangeTap0)
   ON_EN_CHANGE(IDC_TAP_101, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_102, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_103, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_104, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_105, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_106, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_107, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_108, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_109, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_110, OnChangeTap1)
   ON_EN_CHANGE(IDC_TAP_111, OnChangeTap1)
   ON_BN_CLICKED(IDC_IS_DST_1, OnIsImg)
   ON_BN_CLICKED(IDC_IS_DST_2, OnIsImg)
   ON_BN_CLICKED(IDC_IS_DST_3, OnIsImg)
   ON_BN_CLICKED(IDC_IS_SRC_1, OnIsImg)
   ON_BN_CLICKED(IDC_IS_SRC_2, OnIsImg)
   ON_BN_CLICKED(IDC_IS_SRC_3, OnIsImg)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmWTDlg message handlers

BOOL CParmWTDlg::OnInitDialog()
{
   m_bFwd = m_Func.Found("Fwd");
   CParamDlg::OnInitDialog();

   ShowImagesWT();
   InitImagesWT();
   EnableImagesWT();

   for (int i=0; i<2; i++) {
      ShowKernel(i);
      RangeLen(i);
      RangeAnchor(i);
   }
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmWTDlg::OnOK()
{
   CParamDlg::OnOK();
}

void CParmWTDlg::OnIsImg()
{
   UpdateData();
   EnableImagesWT();
}

void CParmWTDlg::OnChangeLen0()
{
   if (!m_bInit) return;
   UpdateData();
   ShowKernel(0);
   RangeAnchor(0);
   UpdateData(FALSE);
}

void CParmWTDlg::OnChangeLen1()
{
   if (!m_bInit) return;
   UpdateData();
   ShowKernel(1);
   RangeAnchor(1);
   UpdateData(FALSE);
}

void CParmWTDlg::OnChangeTap0()
{
   if (!m_bInit) return;
}

void CParmWTDlg::OnChangeTap1()
{
   if (!m_bInit) return;
}

void CParmWTDlg::ShowImagesWT()
{
   if (m_bFwd) {
      m_InvFrame[0].ShowWindow(SW_HIDE);
      m_InvFrame[1].ShowWindow(SW_HIDE);
      for (int i=0; i<4; i++) {
         m_EditSrcWT[i].ShowWindow(SW_HIDE);
         if (i > 0)
            m_IsSrcButton[i].ShowWindow(SW_HIDE);
      }
      GetDlgItem(IDC_EDIT_DST)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_STATIC_DST)->ShowWindow(SW_HIDE);
      m_SrcStatic0.ShowWindow(SW_HIDE);
   } else {
      m_FwdFrame[0].ShowWindow(SW_HIDE);
      m_FwdFrame[1].ShowWindow(SW_HIDE);
      for (int i=0; i<4; i++) {
         m_EditDstWT[i].ShowWindow(SW_HIDE);
         if (i > 0)
            m_IsDstButton[i].ShowWindow(SW_HIDE);
      }
      GetDlgItem(IDC_EDIT_SRC)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_STATIC_SRC)->ShowWindow(SW_HIDE);
      m_DstStatic0.ShowWindow(SW_HIDE);
   }
}

void CParmWTDlg::InitImagesWT()
{
   if (m_bFwd) {
      for (int i=0; i<4; i++) {
         if (m_pDocDstWT[i])
            m_StrDstWT[i] = m_pDocDstWT[i]->GetTitle();
         else
            m_StrDstWT[i] = "<New>";
      }
   } else {
      for (int i=0; i<4; i++) {
         m_StrSrcWT[i] = m_pDocSrcWT[i]->GetTitle();
      }
   }
}

void CParmWTDlg::EnableImagesWT()
{
   for (int i=1; i<4; i++) {
      m_EditSrcWT[i].EnableWindow(m_IsSrcWT[i]);
      m_EditDstWT[i].EnableWindow(m_IsDstWT[i]);
   }
}

void CParmWTDlg::RangeLen(int i)
{
   m_LenSpin[i].SetRange(1,12);
}

void CParmWTDlg::RangeAnchor(int i)
{
   m_AnchorSpin[i].SetRange(0,GetLen(i) - 1);
}

void CParmWTDlg::ShowKernel(int i)
{
   for (int x=0; x<LEN; x++) {
      int flag = x < GetLen(i) ? SW_SHOWNA : SW_HIDE;
      m_TapEdit[i][x].ShowWindow(flag);
   }
}

int CParmWTDlg::GetLen(int i)
{
   return atoi(m_LenStr[i]);
}
