
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmConvBiasedDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ParmConvBiasedDlg.h"

IMPLEMENT_DYNAMIC(CParmConvBiasedDlg, CParamDlg)

CParmConvBiasedDlg::CParmConvBiasedDlg(UINT nID)
   : CParamDlg(nID)
   , m_bias(0)
   , m_lenSrc(0)
   , m_lenSrc2(0)
   , m_lenDst(0)
   , m_biasAuto(FALSE)
   , m_NewDst(_T("New Signal"))
{
}

CParmConvBiasedDlg::~CParmConvBiasedDlg()
{
}

void CParmConvBiasedDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_BIAS, m_bias);
   DDX_Control(pDX, IDC_BIAS_SPIN, m_biasSpin);
   DDX_Text(pDX, IDC_LEN_SRC1, m_lenSrc);
   DDX_Text(pDX, IDC_LEN_SRC2, m_lenSrc2);
   DDX_Text(pDX, IDC_LEN_DST, m_lenDst);
   DDV_MinMaxInt(pDX, m_bias, 0, m_lenSrc2 - 1);
   DDV_MinMaxInt(pDX, m_lenDst, 1, m_bias + m_lenSrc + m_lenSrc2);
   DDX_Control(pDX, IDC_LEN_DST_SPIN, m_lenDstSpin);
   DDX_Check(pDX, IDC_BIAS_CHECK, m_biasAuto);
   DDX_Text(pDX, IDC_EDIT1, m_NewDst);
}


BEGIN_MESSAGE_MAP(CParmConvBiasedDlg, CParamDlg)
   ON_BN_CLICKED(IDC_BIAS_CHECK, OnClickedBiasAuto)
   ON_EN_CHANGE(IDC_LEN_DST, OnChangeLenDst)
END_MESSAGE_MAP()


// CParmConvBiasedDlg message handlers

BOOL CParmConvBiasedDlg::OnInitDialog()
{
   CParamDlg::OnInitDialog();

   m_biasSpin.SetRange32(0, m_lenSrc2 - 1);
   m_lenDstSpin.SetRange32(1, m_bias + m_lenSrc + m_lenSrc2);
   EnableBias();
   SetBias();

   return TRUE; 
}

void CParmConvBiasedDlg::OnClickedBiasAuto()
{
   UpdateData();
   EnableBias();
   SetBias();
   UpdateData(FALSE);
}

void CParmConvBiasedDlg::OnChangeLenDst()
{
   if (m_bInit) {
      UpdateData();
      SetBias();
      UpdateData(FALSE);
   }
}

void CParmConvBiasedDlg::EnableBias()
{
   ((CEdit*)GetDlgItem(IDC_BIAS))->SetReadOnly(m_biasAuto);
   GetDlgItem(IDC_BIAS_SPIN)->EnableWindow(!m_biasAuto);
}

void CParmConvBiasedDlg::SetBias()
{
   if (m_biasAuto) {
      m_bias = m_lenSrc2 - m_lenDst;
   }
}
