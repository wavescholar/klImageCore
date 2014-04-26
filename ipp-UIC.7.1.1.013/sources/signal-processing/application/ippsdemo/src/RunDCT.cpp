/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDCT.cpp : implementation of the CRunDCT class.
// CRunDCT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Histo.h"
#include "RunDCT.h"
#include "ParmHintDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDCT::CRunDCT()
{
   m_pSpec = NULL;
   m_pBuffer  = NULL;
   m_hint  = ippAlgHintFast;
}

CRunDCT::~CRunDCT()
{

}

BOOL CRunDCT::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;

   m_pBuffer = NULL;
   int type = (int)func.DstType() & ~PP_CPLX;
   m_InitName = "";
   m_FreeName = "";
   m_GetBufSizeName = "";

   CString sBegin = "ipps" + func.BaseName();
   CString sEnd = "_" + func.TypeName();
   m_InitName = sBegin + "InitAlloc" + sEnd;
   m_FreeName = sBegin + "Free" + sEnd;
   m_GetBufSizeName = sBegin + "GetBufSize" + sEnd;
   return TRUE;
}

CParamDlg* CRunDCT::CreateDlg()
{
   return new CParmHintDlg;
}

void CRunDCT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmHintDlg *pDlg = (CParmHintDlg*)parmDlg;
   if (save) {
      m_hint  = (IppHintAlgorithm)pDlg->m_hint ;
   } else {
      pDlg->m_hint  = (int)m_hint ;
   }
}

BOOL CRunDCT::BeforeCall()
{
   IppStatus status = CallInit();
   IppErrorMessage(m_InitName, status);
   if (status < 0) return FALSE;
   int size;
   status = CallGetBufSize(size);
   IppErrorMessage(m_GetBufSizeName, status);
   if (status < 0) return FALSE;
   if (size) m_pBuffer = (Ipp8u*)ippMalloc(size);
   return TRUE;
}

BOOL CRunDCT::AfterCall(BOOL bOk)
{
   IppStatus status = CallFree(); m_pSpec = NULL;
   IppErrorMessage(m_FreeName, status);
   ippFree(m_pBuffer); m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunDCT::CallInit()
{
   MATE_CALL(m_InitName, ippsDCTFwdInitAlloc_16s,
                   ( (IppsDCTFwdSpec_16s**)&m_pSpec,
                     len, m_hint))
   MATE_CALL(m_InitName, ippsDCTInvInitAlloc_16s,
                   ( (IppsDCTInvSpec_16s**)&m_pSpec,
                     len, m_hint))
   MATE_CALL(m_InitName, ippsDCTFwdInitAlloc_32f,
                   ( (IppsDCTFwdSpec_32f**)&m_pSpec,
                     len, m_hint))
   MATE_CALL(m_InitName, ippsDCTInvInitAlloc_32f,
                   ( (IppsDCTInvSpec_32f**)&m_pSpec,
                     len, m_hint))
   MATE_CALL(m_InitName, ippsDCTFwdInitAlloc_64f,
                   ( (IppsDCTFwdSpec_64f**)&m_pSpec,
                     len, m_hint))
   MATE_CALL(m_InitName, ippsDCTInvInitAlloc_64f,
                   ( (IppsDCTInvSpec_64f**)&m_pSpec,
                     len, m_hint))

   return stsNoFunction;
}

IppStatus CRunDCT::CallFree()
{
   MATE_CALL(m_FreeName, ippsDCTFwdFree_16s, ( (IppsDCTFwdSpec_16s*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDCTInvFree_16s, ( (IppsDCTInvSpec_16s*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDCTFwdFree_32f, ( (IppsDCTFwdSpec_32f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDCTInvFree_32f, ( (IppsDCTInvSpec_32f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDCTFwdFree_64f, ( (IppsDCTFwdSpec_64f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDCTInvFree_64f, ( (IppsDCTInvSpec_64f*) m_pSpec ))

   return stsNoFunction;
}

IppStatus CRunDCT::CallGetBufSize(int& size)
{
   MATE_CALL(m_GetBufSizeName, ippsDCTFwdGetBufSize_16s,
                   ( (IppsDCTFwdSpec_16s*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDCTInvGetBufSize_16s,
                   ( (IppsDCTInvSpec_16s*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDCTFwdGetBufSize_32f,
                   ( (IppsDCTFwdSpec_32f*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDCTInvGetBufSize_32f,
                   ( (IppsDCTInvSpec_32f*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDCTFwdGetBufSize_64f,
                   ( (IppsDCTFwdSpec_64f*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDCTInvGetBufSize_64f,
                   ( (IppsDCTInvSpec_64f*)m_pSpec, &size ))

   return stsNoFunction;
}

IppStatus CRunDCT::CallIppFunction()
{
   FUNC_CALL(ippsDCTFwd_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDCTFwdSpec_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippsDCTInv_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDCTInvSpec_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippsDCTFwd_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDCTFwdSpec_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippsDCTInv_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDCTInvSpec_32f*)m_pSpec, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippsDCTFwd_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDCTFwdSpec_64f*)m_pSpec, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippsDCTInv_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDCTInvSpec_64f*)m_pSpec, (Ipp8u*)m_pBuffer ))

   return stsNoFunction;
}

void CRunDCT::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString initParms;
   initParms << len << ", " 
             << m_hint ;
   pHisto->AddFuncString(m_InitName, initParms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}





