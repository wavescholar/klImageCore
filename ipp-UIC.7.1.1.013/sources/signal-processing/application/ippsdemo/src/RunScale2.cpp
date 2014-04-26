
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunScale2.cpp : implementation of the CRunScale2class.
// CRunScale2class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunScale2.h"
#include "ParmScale2Dlg.h"

CRunScale2::CRunScale2()
{
   m_scaleFactor1 = 0;
   m_scaleFactor2 = 0;
}

CRunScale2::~CRunScale2()
{

}

BOOL CRunScale2::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

CParamDlg* CRunScale2::CreateDlg()
{
   return new CParmScale2Dlg;
}

void CRunScale2::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmScale2Dlg *pDlg = (CParmScale2Dlg*)parmDlg;
   if (save) {
      m_scaleFactor1 = pDlg->m_scaleFactor1;
      m_scaleFactor2 = pDlg->m_scaleFactor2;
   } else {
      pDlg->m_scaleFactor1 = m_scaleFactor1;
      pDlg->m_scaleFactor2 = m_scaleFactor2;
   }
}

IppStatus CRunScale2::CallIppFunction()
{
   FUNC_CALL(ippsPolarToCart_16sc_Sfs, ((const Ipp16s*)pSrc, (const Ipp16s*)pSrc2, (Ipp16sc*)pDst, len, 
      m_scaleFactor1, m_scaleFactor2))

   return stsNoFunction;
}

CString CRunScale2::GetHistoryParms()
{
   CMyString parm;
   parm << m_scaleFactor1 << m_scaleFactor2;
   return parm;
}
