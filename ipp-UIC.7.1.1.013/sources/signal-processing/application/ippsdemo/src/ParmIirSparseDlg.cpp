
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmIirSparseDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmIirSparseDlg.h"
#include "RunIirSparse.h"

IMPLEMENT_DYNAMIC(CParmIirSparseDlg, CParamDlg)
CParmIirSparseDlg::CParmIirSparseDlg(UINT nID)
   : CParamDlg(nID)
   , m_NZTapsLen1(0)
   , m_NZTapsLen2(0)
   , m_epsilonStr(_T(""))
   , m_pIirSparse(NULL)
   , m_TapTitle(_T(""))
   , m_TapStr (_T(""))
   , m_Order (0)
{
}

CParmIirSparseDlg::~CParmIirSparseDlg()
{
}

void CParmIirSparseDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_TAPS_EDIT, m_TapStr);
   DDX_Text(pDX, IDC_ORDER, m_Order);
   DDX_Text(pDX, IDC_TAPS_TITLE, m_TapTitle);
   DDX_Text(pDX, IDC_NZ_LEN1, m_NZTapsLen1);
   DDX_Text(pDX, IDC_NZ_LEN2, m_NZTapsLen2);
   DDX_Text(pDX, IDC_EPSILON, m_epsilonStr);
}


BEGIN_MESSAGE_MAP(CParmIirSparseDlg, CParamDlg)
   ON_EN_CHANGE(IDC_EPSILON, OnChangeEpsilon)
END_MESSAGE_MAP()


// CParmIirSparseDlg message handlers

BOOL CParmIirSparseDlg::OnInitDialog()
{
   ASSERT(m_pIirSparse);
   CParamDlg::OnInitDialog();
   m_pIirSparse->Sparse(m_epsilon);
   m_Order = (m_pIirSparse->GetSparsedVector()->Length() - 2)>>1;
   m_NZTapsLen1 = m_pIirSparse->GetNZTapsLen1();
   m_NZTapsLen2 = m_pIirSparse->GetNZTapsLen2();
   m_pIirSparse->GetSparsedVector()->Get(m_TapStr);
   m_epsilonStr.Format("%.3g", m_epsilon);
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmIirSparseDlg::OnChangeEpsilon()
{
   if (m_bInit) {
      UpdateData();
      m_epsilon = atof(m_epsilonStr);
      m_pIirSparse->Sparse(m_epsilon);
      m_NZTapsLen1 = m_pIirSparse->GetNZTapsLen1();
      m_NZTapsLen2 = m_pIirSparse->GetNZTapsLen2();
      m_pIirSparse->GetSparsedVector()->Get(m_TapStr);
      UpdateData(FALSE);
   }
}
