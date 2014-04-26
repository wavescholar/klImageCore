
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// CParmFirSparseDlg dialog gets parameters for certain ippSP functions.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmFirSparseDlg.h"
#include "RunFirSparse.h"

IMPLEMENT_DYNAMIC(CParmFirSparseDlg, CParamDlg)
CParmFirSparseDlg::CParmFirSparseDlg(UINT nID)
   : CParamDlg(nID)
   , m_NZTapsLen(0)
   , m_epsilonStr(_T(""))
{
   m_pFirSparse = NULL;
   m_TapTitle = _T("");
   m_TapStr = _T("");
   m_TapLen = 0;
}

CParmFirSparseDlg::~CParmFirSparseDlg()
{
}

void CParmFirSparseDlg::DoDataExchange(CDataExchange* pDX)
{
   CParamDlg::DoDataExchange(pDX);
   DDX_Text(pDX, IDC_TAPS_EDIT, m_TapStr);
   DDX_Text(pDX, IDC_TAPSLEN_EDIT, m_TapLen);
   DDX_Text(pDX, IDC_TAPS_TITLE, m_TapTitle);
   DDX_Text(pDX, IDC_NONZERO_LEN, m_NZTapsLen);
   DDX_Text(pDX, IDC_EPSILON, m_epsilonStr);
}


BEGIN_MESSAGE_MAP(CParmFirSparseDlg, CParamDlg)
   ON_EN_CHANGE(IDC_EPSILON, OnChangeEpsilon)
END_MESSAGE_MAP()


// CParmFirSparseDlg message handlers

BOOL CParmFirSparseDlg::OnInitDialog()
{
   ASSERT(m_pFirSparse);
   CParamDlg::OnInitDialog();
   m_pFirSparse->Sparse(m_epsilon);
   m_TapLen = m_pFirSparse->GetSparsedVector()->Length();
   m_NZTapsLen = m_pFirSparse->GetNZTapsLen();
   m_pFirSparse->GetSparsedVector()->Get(m_TapStr);
   m_epsilonStr.Format("%.3g", m_epsilon);
   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmFirSparseDlg::OnChangeEpsilon()
{
   if (m_bInit) {
      UpdateData();
      m_epsilon = atof(m_epsilonStr);
      m_pFirSparse->Sparse(m_epsilon);
      m_NZTapsLen = m_pFirSparse->GetNZTapsLen();
      m_pFirSparse->GetSparsedVector()->Get(m_TapStr);
      UpdateData(FALSE);
   }
}
