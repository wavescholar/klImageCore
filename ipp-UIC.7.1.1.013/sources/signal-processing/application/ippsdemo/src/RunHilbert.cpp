/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHilbert.cpp : implementation of the CRunHilbert class.
// CRunHilbert class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "Histo.h"
#include "RunHilbert.h"
#include "ParmHintDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunHilbert::CRunHilbert()
{
   m_hint  = ippAlgHintFast;
   m_pSpec = NULL;
}

CRunHilbert::~CRunHilbert()
{

}

BOOL CRunHilbert::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;

   m_InitName = "ippsHilbertInitAlloc_" + func.TypeName();
   m_FreeName = "ippsHilbertFree_" + func.TypeName();
   return TRUE;
}

CParamDlg* CRunHilbert::CreateDlg()
{
   return new CParmHintDlg;
}

void CRunHilbert::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmHintDlg *pDlg = (CParmHintDlg*)parmDlg;
   if (save) {
      m_hint  = (IppHintAlgorithm)pDlg->m_hint ;
   } else {
      pDlg->m_hint  = (int)m_hint ;
   }
}

BOOL CRunHilbert::BeforeCall()
{
   IppStatus status = CallInit();
   IppErrorMessage(m_InitName, status);
   if (status < 0) return FALSE;
   return TRUE;
}

BOOL CRunHilbert::AfterCall(BOOL bOk)
{
   IppStatus status = CallFree(); m_pSpec = NULL;
   IppErrorMessage(m_FreeName, status);
   return TRUE;
}

IppStatus CRunHilbert::CallInit()
{
   MATE_CALL(m_InitName, ippsHilbertInitAlloc_32f32fc, ((IppsHilbertSpec_32f32fc**)&m_pSpec, len, m_hint))
   MATE_CALL(m_InitName, ippsHilbertInitAlloc_16s32fc, ((IppsHilbertSpec_16s32fc**)&m_pSpec, len, m_hint))
   MATE_CALL(m_InitName, ippsHilbertInitAlloc_16s16sc, ((IppsHilbertSpec_16s16sc**)&m_pSpec, len, m_hint))
   return stsNoFunction;
}

IppStatus CRunHilbert::CallFree()
{
   MATE_CALL(m_FreeName, ippsHilbertFree_32f32fc, ((IppsHilbertSpec_32f32fc*)m_pSpec))
   MATE_CALL(m_FreeName, ippsHilbertFree_16s32fc, ((IppsHilbertSpec_16s32fc*)m_pSpec))
   MATE_CALL(m_FreeName, ippsHilbertFree_16s16sc, ((IppsHilbertSpec_16s16sc*)m_pSpec))
   return stsNoFunction;
}

IppStatus CRunHilbert::CallIppFunction()
{
   FUNC_CALL(ippsHilbert_32f32fc, ((Ipp32f*)pSrc, (Ipp32fc*)pDst, (IppsHilbertSpec_32f32fc*)m_pSpec))
   FUNC_CALL(ippsHilbert_16s32fc, ((Ipp16s*)pSrc, (Ipp32fc*)pDst, (IppsHilbertSpec_16s32fc*)m_pSpec))
   FUNC_CALL(ippsHilbert_16s16sc_Sfs, ((Ipp16s*)pSrc, (Ipp16sc*)pDst, (IppsHilbertSpec_16s16sc*)m_pSpec, scaleFactor))

   return stsNoFunction;
}

void CRunHilbert::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString initParms;
   initParms << m_hint ;
   pHisto->AddFuncString(m_InitName, initParms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}


