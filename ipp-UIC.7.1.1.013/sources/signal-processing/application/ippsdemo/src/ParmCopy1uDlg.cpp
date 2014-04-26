
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmCopy1uDlg  dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmCopy1uDlg.h"

IMPLEMENT_DYNAMIC(CParmCopy1uDlg, CParamDlg)

CParmCopy1uDlg::CParmCopy1uDlg(UINT nID)
   : CParamDlg(nID)
   , m_numBits(0)
{
   m_bitOffset[0] = m_bitOffset[1] = 0;
}

CParmCopy1uDlg::~CParmCopy1uDlg()
{
}

void CParmCopy1uDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_LEN_EDIT, m_numBits);
   DDX_Text(pDX, IDC_OFFSET_EDIT_0, m_bitOffset[0]);
   DDX_Text(pDX, IDC_OFFSET_EDIT_1, m_bitOffset[1]);
   DDV_MinMaxInt(pDX, m_bitOffset[0], 0, m_itemBits - 1);
   DDV_MinMaxInt(pDX, m_bitOffset[1], 0, m_itemBits - 1);
   DDX_Control(pDX, IDC_OFFSET_SPIN_0, m_bitOffsetSpin[0]);
   DDX_Control(pDX, IDC_OFFSET_SPIN_1, m_bitOffsetSpin[1]);
   DDX_Control(pDX, IDC_LEN_SPIN, m_numBitsSpin);
}


BEGIN_MESSAGE_MAP(CParmCopy1uDlg, CParamDlg)
   ON_EN_CHANGE(IDC_OFFSET_EDIT_0, OnEnChangeOffsetEdit0)
END_MESSAGE_MAP()


// CParmCopy1uDlg message handlers

BOOL CParmCopy1uDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   for (int i=0; i<2; i++) m_bitOffsetSpin[i].SetRange(0, m_itemBits - 1);
   m_numBitsSpin.SetRange(1, m_maxBits - m_bitOffset[0]);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmCopy1uDlg::OnEnChangeOffsetEdit0()
{
   if (m_bInit) {
      m_numBitsSpin.SetRange(1, m_maxBits - m_bitOffset[0]);
   }

}
