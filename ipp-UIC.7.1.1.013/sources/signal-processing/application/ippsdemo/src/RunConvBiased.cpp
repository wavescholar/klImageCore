
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConvBiased.cpp : implementation of the CRunConvBiasedclass.
// CRunConvBiasedclass processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunConvBiased.h"
#include "ParmConvBiasedDlg.h"

CRunConvBiased::CRunConvBiased()
{
   m_bias = 0;
   m_biasAuto = TRUE;
}

CRunConvBiased::~CRunConvBiased()
{

}

BOOL CRunConvBiased::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

int CRunConvBiased::GetDstLength()
{
   return m_lenDst;
}

CParamDlg* CRunConvBiased::CreateDlg()
{
   return new CParmConvBiasedDlg;
}

void CRunConvBiased::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmConvBiasedDlg *pDlg = (CParmConvBiasedDlg*)parmDlg;
   if (save) {
      m_biasAuto = pDlg->m_biasAuto;
      m_lenDst = pDlg->m_lenDst;
      m_bias = pDlg->m_bias;
   } else {
      pDlg->m_biasAuto = m_biasAuto;
      pDlg->m_bias = m_bias;
      pDlg->m_lenSrc = m_pDocSrc->GetVector()->Length();
      pDlg->m_lenSrc2 = m_pDocSrc2->GetVector()->Length();
      pDlg->m_lenDst = pDlg->m_lenSrc2  - m_bias;
   }
}

IppStatus CRunConvBiased::CallIppFunction()
{
   FUNC_CALL(ippsConvBiased_32f, ((Ipp32f*)pSrc, m_lenSrc, (Ipp32f*)pSrc2 + m_bias, m_lenSrc2,
                                  (Ipp32f*)pDst, m_lenDst, m_bias))

   return stsNoFunction;
}

CString CRunConvBiased::GetHistoryParms()
{
   CMyString parm;
   parm << m_lenSrc << ", " << m_lenSrc2 << ", " << m_lenDst << ", " << m_bias;
   return parm;
}
