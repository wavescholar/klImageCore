/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterCustomDlg.cpp : implementation of the CParmFilterCustomDlg class.
// CParmFilterCustomDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ParmFilterCustomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmFilterCustomDlg dialog


CParmFilterCustomDlg::CParmFilterCustomDlg(UINT nID, CWnd* pParent /*=NULL*/)
   : CParmFilterBoxDlg(nID, pParent)
{
   m_MaxSize[0] = 7;
   m_MaxSize[1] = 7;
   m_bDiv = FALSE;
   //{{AFX_DATA_INIT(CParmFilterCustomDlg)
   m_DivStr = _T("");
   m_DivType = -1;
   //}}AFX_DATA_INIT
}


void CParmFilterCustomDlg::DoDataExchange(CDataExchange* pDX)
{
   CParmFilterBoxDlg::DoDataExchange(pDX);

   DDX_Text(pDX, IDC_DIV_EDIT, m_DivStr);
   DDX_Radio(pDX, IDC_DIV_RADIO0, m_DivType);
   DDX_Control(pDX, IDC_DIV_STATIC, m_DivStatic);
   DDX_Control(pDX, IDC_DIV_SPIN, m_DivSpin);
   DDX_Control(pDX, IDC_DIV_EDIT, m_DivEdit);
   DDX_Control(pDX, IDC_DIV_RADIO0, m_DivRadio[0]);
   DDX_Control(pDX, IDC_DIV_RADIO1, m_DivRadio[1]);

   DDX_Radio(pDX, IDC_ROUND_0, m_roundMode);
   DDX_Control(pDX, IDC_ROUND_STATIC, m_RoundStatic);
   DDX_Control(pDX, IDC_ROUND_0, m_RoundRadio[0]);
   DDX_Control(pDX, IDC_ROUND_1, m_RoundRadio[1]);

   DDX_Text(pDX, IDC_SIZE_0, m_maskSize.width);
   DDX_Text(pDX, IDC_SIZE_1, m_maskSize.height);

   DDV_MinMaxInt(pDX, m_maskSize.width, 1, 7);
   DDV_MinMaxInt(pDX, m_maskSize.height, 1, 7);

   DDX_Control(pDX, IDC_KERN_00, m_KernelEdit[0][0]);
   DDX_Control(pDX, IDC_KERN_01, m_KernelEdit[0][1]);
   DDX_Control(pDX, IDC_KERN_02, m_KernelEdit[0][2]);
   DDX_Control(pDX, IDC_KERN_03, m_KernelEdit[0][3]);
   DDX_Control(pDX, IDC_KERN_04, m_KernelEdit[0][4]);
   DDX_Control(pDX, IDC_KERN_05, m_KernelEdit[0][5]);
   DDX_Control(pDX, IDC_KERN_06, m_KernelEdit[0][6]);
   DDX_Control(pDX, IDC_KERN_10, m_KernelEdit[1][0]);
   DDX_Control(pDX, IDC_KERN_11, m_KernelEdit[1][1]);
   DDX_Control(pDX, IDC_KERN_12, m_KernelEdit[1][2]);
   DDX_Control(pDX, IDC_KERN_13, m_KernelEdit[1][3]);
   DDX_Control(pDX, IDC_KERN_14, m_KernelEdit[1][4]);
   DDX_Control(pDX, IDC_KERN_15, m_KernelEdit[1][5]);
   DDX_Control(pDX, IDC_KERN_16, m_KernelEdit[1][6]);
   DDX_Control(pDX, IDC_KERN_20, m_KernelEdit[2][0]);
   DDX_Control(pDX, IDC_KERN_21, m_KernelEdit[2][1]);
   DDX_Control(pDX, IDC_KERN_22, m_KernelEdit[2][2]);
   DDX_Control(pDX, IDC_KERN_23, m_KernelEdit[2][3]);
   DDX_Control(pDX, IDC_KERN_24, m_KernelEdit[2][4]);
   DDX_Control(pDX, IDC_KERN_25, m_KernelEdit[2][5]);
   DDX_Control(pDX, IDC_KERN_26, m_KernelEdit[2][6]);
   DDX_Control(pDX, IDC_KERN_30, m_KernelEdit[3][0]);
   DDX_Control(pDX, IDC_KERN_31, m_KernelEdit[3][1]);
   DDX_Control(pDX, IDC_KERN_32, m_KernelEdit[3][2]);
   DDX_Control(pDX, IDC_KERN_33, m_KernelEdit[3][3]);
   DDX_Control(pDX, IDC_KERN_34, m_KernelEdit[3][4]);
   DDX_Control(pDX, IDC_KERN_35, m_KernelEdit[3][5]);
   DDX_Control(pDX, IDC_KERN_36, m_KernelEdit[3][6]);
   DDX_Control(pDX, IDC_KERN_40, m_KernelEdit[4][0]);
   DDX_Control(pDX, IDC_KERN_41, m_KernelEdit[4][1]);
   DDX_Control(pDX, IDC_KERN_42, m_KernelEdit[4][2]);
   DDX_Control(pDX, IDC_KERN_43, m_KernelEdit[4][3]);
   DDX_Control(pDX, IDC_KERN_44, m_KernelEdit[4][4]);
   DDX_Control(pDX, IDC_KERN_45, m_KernelEdit[4][5]);
   DDX_Control(pDX, IDC_KERN_46, m_KernelEdit[4][6]);
   DDX_Control(pDX, IDC_KERN_50, m_KernelEdit[5][0]);
   DDX_Control(pDX, IDC_KERN_51, m_KernelEdit[5][1]);
   DDX_Control(pDX, IDC_KERN_52, m_KernelEdit[5][2]);
   DDX_Control(pDX, IDC_KERN_53, m_KernelEdit[5][3]);
   DDX_Control(pDX, IDC_KERN_54, m_KernelEdit[5][4]);
   DDX_Control(pDX, IDC_KERN_55, m_KernelEdit[5][5]);
   DDX_Control(pDX, IDC_KERN_56, m_KernelEdit[5][6]);
   DDX_Control(pDX, IDC_KERN_60, m_KernelEdit[6][0]);
   DDX_Control(pDX, IDC_KERN_61, m_KernelEdit[6][1]);
   DDX_Control(pDX, IDC_KERN_62, m_KernelEdit[6][2]);
   DDX_Control(pDX, IDC_KERN_63, m_KernelEdit[6][3]);
   DDX_Control(pDX, IDC_KERN_64, m_KernelEdit[6][4]);
   DDX_Control(pDX, IDC_KERN_65, m_KernelEdit[6][5]);
   DDX_Control(pDX, IDC_KERN_66, m_KernelEdit[6][6]);

   DDX_Text(pDX, IDC_KERN_00, m_KernelStr[0][0]);
   DDX_Text(pDX, IDC_KERN_01, m_KernelStr[0][1]);
   DDX_Text(pDX, IDC_KERN_02, m_KernelStr[0][2]);
   DDX_Text(pDX, IDC_KERN_03, m_KernelStr[0][3]);
   DDX_Text(pDX, IDC_KERN_04, m_KernelStr[0][4]);
   DDX_Text(pDX, IDC_KERN_05, m_KernelStr[0][5]);
   DDX_Text(pDX, IDC_KERN_06, m_KernelStr[0][6]);
   DDX_Text(pDX, IDC_KERN_10, m_KernelStr[1][0]);
   DDX_Text(pDX, IDC_KERN_11, m_KernelStr[1][1]);
   DDX_Text(pDX, IDC_KERN_12, m_KernelStr[1][2]);
   DDX_Text(pDX, IDC_KERN_13, m_KernelStr[1][3]);
   DDX_Text(pDX, IDC_KERN_14, m_KernelStr[1][4]);
   DDX_Text(pDX, IDC_KERN_15, m_KernelStr[1][5]);
   DDX_Text(pDX, IDC_KERN_16, m_KernelStr[1][6]);
   DDX_Text(pDX, IDC_KERN_20, m_KernelStr[2][0]);
   DDX_Text(pDX, IDC_KERN_21, m_KernelStr[2][1]);
   DDX_Text(pDX, IDC_KERN_22, m_KernelStr[2][2]);
   DDX_Text(pDX, IDC_KERN_23, m_KernelStr[2][3]);
   DDX_Text(pDX, IDC_KERN_24, m_KernelStr[2][4]);
   DDX_Text(pDX, IDC_KERN_25, m_KernelStr[2][5]);
   DDX_Text(pDX, IDC_KERN_26, m_KernelStr[2][6]);
   DDX_Text(pDX, IDC_KERN_30, m_KernelStr[3][0]);
   DDX_Text(pDX, IDC_KERN_31, m_KernelStr[3][1]);
   DDX_Text(pDX, IDC_KERN_32, m_KernelStr[3][2]);
   DDX_Text(pDX, IDC_KERN_33, m_KernelStr[3][3]);
   DDX_Text(pDX, IDC_KERN_34, m_KernelStr[3][4]);
   DDX_Text(pDX, IDC_KERN_35, m_KernelStr[3][5]);
   DDX_Text(pDX, IDC_KERN_36, m_KernelStr[3][6]);
   DDX_Text(pDX, IDC_KERN_40, m_KernelStr[4][0]);
   DDX_Text(pDX, IDC_KERN_41, m_KernelStr[4][1]);
   DDX_Text(pDX, IDC_KERN_42, m_KernelStr[4][2]);
   DDX_Text(pDX, IDC_KERN_43, m_KernelStr[4][3]);
   DDX_Text(pDX, IDC_KERN_44, m_KernelStr[4][4]);
   DDX_Text(pDX, IDC_KERN_45, m_KernelStr[4][5]);
   DDX_Text(pDX, IDC_KERN_46, m_KernelStr[4][6]);
   DDX_Text(pDX, IDC_KERN_50, m_KernelStr[5][0]);
   DDX_Text(pDX, IDC_KERN_51, m_KernelStr[5][1]);
   DDX_Text(pDX, IDC_KERN_52, m_KernelStr[5][2]);
   DDX_Text(pDX, IDC_KERN_53, m_KernelStr[5][3]);
   DDX_Text(pDX, IDC_KERN_54, m_KernelStr[5][4]);
   DDX_Text(pDX, IDC_KERN_55, m_KernelStr[5][5]);
   DDX_Text(pDX, IDC_KERN_56, m_KernelStr[5][6]);
   DDX_Text(pDX, IDC_KERN_60, m_KernelStr[6][0]);
   DDX_Text(pDX, IDC_KERN_61, m_KernelStr[6][1]);
   DDX_Text(pDX, IDC_KERN_62, m_KernelStr[6][2]);
   DDX_Text(pDX, IDC_KERN_63, m_KernelStr[6][3]);
   DDX_Text(pDX, IDC_KERN_64, m_KernelStr[6][4]);
   DDX_Text(pDX, IDC_KERN_65, m_KernelStr[6][5]);
   DDX_Text(pDX, IDC_KERN_66, m_KernelStr[6][6]);
}


BEGIN_MESSAGE_MAP(CParmFilterCustomDlg, CParmFilterBoxDlg)
   //{{AFX_MSG_MAP(CParmFilterCustomDlg)
   ON_EN_CHANGE(IDC_SIZE_0, OnChangeSize)
   ON_EN_CHANGE(IDC_KERN_00, OnChangeKernel)
   ON_EN_CHANGE(IDC_SIZE_1, OnChangeSize)
   ON_EN_CHANGE(IDC_KERN_01, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_02, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_03, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_04, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_05, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_06, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_10, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_11, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_12, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_13, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_14, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_15, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_16, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_20, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_21, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_22, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_23, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_24, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_25, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_26, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_30, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_31, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_32, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_33, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_34, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_35, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_36, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_40, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_41, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_42, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_43, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_44, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_45, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_46, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_50, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_51, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_52, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_53, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_54, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_55, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_56, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_60, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_61, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_62, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_63, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_64, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_65, OnChangeKernel)
   ON_EN_CHANGE(IDC_KERN_66, OnChangeKernel)
   ON_BN_CLICKED(IDC_DIV_RADIO0, OnDivRadio)
   ON_BN_CLICKED(IDC_DIV_RADIO1, OnDivRadio)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmFilterCustomDlg message handlers

BOOL CParmFilterCustomDlg::OnInitDialog()
{
   CParmFilterBoxDlg::OnInitDialog();
   for (int i=0; i<2; i++) {
      m_SizeSpin[i].SetRange(1,m_MaxSize[i]);
      m_AnchorSpin[i].SetRange(0,m_MaxSize[i] - 1);
   }
   ShowDivisor();
   ShowRound();
   ShowKernel();
   UpdateKernel(FALSE);
   UpdateData(FALSE);

   // TODO: Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmFilterCustomDlg::OnOK()
{
   CParmFilterBoxDlg::OnOK();
   UpdateKernel(TRUE);
}

void CParmFilterCustomDlg::OnChangeSize()
{
   if (!m_bInit) return;
   CParmFilterBoxDlg::OnChangeSize();
   ShowKernel();
}

int CParmFilterCustomDlg::GetStart(int i) {
   return (m_MaxSize[i] - GetSize(i))>>1;
}

int CParmFilterCustomDlg::GetStop(int i) {
   return GetStart(i) + GetSize(i);
}

void CParmFilterCustomDlg::OnDivRadio()
{
   UpdateData();
   m_DivEdit.SetReadOnly(m_DivType == 0);
   m_DivSpin.EnableWindow(m_DivType != 0);
}

void CParmFilterCustomDlg::ShowDivisor()
{
   int flag = m_bDiv ? SW_SHOWNA : SW_HIDE;
   m_DivStatic.ShowWindow(flag);
   m_DivEdit.ShowWindow(flag);
   m_DivSpin.ShowWindow(flag);
   m_DivRadio[0].ShowWindow(flag);
   m_DivRadio[1].ShowWindow(flag);

   m_DivSpin.SetRange32(INT_MIN,INT_MAX);
   m_DivEdit.SetReadOnly(m_DivType == 0);
   m_DivSpin.EnableWindow(m_DivType != 0);
}

void CParmFilterCustomDlg::ShowRound()
{
   int flag = m_bRound ? SW_SHOWNA : SW_HIDE;
   m_RoundStatic.ShowWindow(flag);
   m_RoundRadio[0].ShowWindow(flag);
   m_RoundRadio[1].ShowWindow(flag);
}

void CParmFilterCustomDlg::ShowKernel()
{
   for (int y=0; y<m_MaxSize[1]; y++) {
      for (int x=0; x<m_MaxSize[0]; x++) {
         int flag = (GetStart(0) <= x && x < GetStop(0)
                  && GetStart(1) <= y && y < GetStop(1)) ?
                  SW_SHOWNA : SW_HIDE;
         m_KernelEdit[y][x].ShowWindow(flag);
      }
   }
}

void CParmFilterCustomDlg::UpdateKernel(BOOL bSave)
{
   if (bSave) {
      m_kernel.Allocate(GetSize(0)*GetSize(1));
      int k = 0;
      for (int y=0; y<m_MaxSize[1]; y++) {
         for (int x=0; x<m_MaxSize[0]; x++) {
            if (GetStart(0) <= x && x < GetStop(0)
             && GetStart(1) <= y && y < GetStop(1)) {
               m_kernel.Set(k++,m_KernelStr[y][x]);
            }
         }
      }
   } else {
      int k = 0;
      for (int y=0; y<m_MaxSize[1]; y++) {
         for (int x=0; x<m_MaxSize[0]; x++) {
            if (GetStart(0) <= x && x < GetStop(0)
             && GetStart(1) <= y && y < GetStop(1)) {
               m_kernel.Get(k++,m_KernelStr[y][x]);
            } else {
               m_KernelStr[y][x] = "0";
            }
         }
      }
   }
}

void CParmFilterCustomDlg::OnChangeKernel()
{
   if (!m_bDiv) return;
   if (m_DivType > 0) return;
   UpdateData();
   UpdateKernel();
   UpdateDivisor();
   UpdateData(FALSE);
}

void CParmFilterCustomDlg::UpdateDivisor()
{
   int divisor = 0;
   for (int i=0; i<m_kernel.Length(); i++)
      divisor += m_kernel.GetInt(i);
   if (divisor == 0) divisor = 1;
   m_DivStr.Format("%d",divisor);
}

