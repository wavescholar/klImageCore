/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDct.cpp: implementation of the CRunDct class.
// CRunDct class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunDct.h"
#include "ParmHintDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDct::CRunDct()
{
   m_hint = ippAlgHintFast;
   m_bufSize = 0;
   m_pBuffer = NULL;
}

CRunDct::~CRunDct()
{

}

CParamDlg* CRunDct::CreateDlg() { return new CParmHintDlg;}

void CRunDct::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmHintDlg* pDlg = (CParmHintDlg*)parmDlg;
   if (save) {
      m_hint = (IppHintAlgorithm)pDlg->m_Hint;
   } else {
      pDlg->m_Hint = (int)m_hint;
   }
}

BOOL CRunDct::BeforeCall()
{
   CString initName = m_Func.Prefix()
                    + m_Func.BaseName() + "InitAlloc_"
                    + m_Func.TypeName();
   CString getBufSizeName = m_Func.Prefix()
                    + m_Func.BaseName() + "GetBufSize_"
                    + m_Func.TypeName();
   IppStatus status = CallInit(initName);
   IppErrorMessage(initName, status);
   if (status < 0) return FALSE;
   status = CallGetBufSize(getBufSizeName);
   IppErrorMessage(getBufSizeName, status);
   if (status < 0) return FALSE;
   if (m_bufSize) m_pBuffer = (Ipp8u*)ippMalloc(m_bufSize);
   return TRUE;
}

BOOL CRunDct::AfterCall(BOOL bOK)
{
   CString freeName = m_Func.Prefix()
                    + m_Func.BaseName() + "Free_"
                    + m_Func.TypeName();
   IppStatus status = CallFree(freeName); m_pSpec = NULL;
   IppErrorMessage(freeName, status);
   ippFree(m_pBuffer); m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunDct::CallInit(CString name)
{
   MATE_CALL(name, ippiDCTFwdInitAlloc_32f,((IppiDCTFwdSpec_32f**)&m_pSpec,roiSize,(IppHintAlgorithm) m_hint))
   MATE_CALL(name, ippiDCTInvInitAlloc_32f,((IppiDCTInvSpec_32f**)&m_pSpec,roiSize,(IppHintAlgorithm) m_hint))
   return stsNoFunction;
}

IppStatus CRunDct::CallFree(CString name)
{
   MATE_CALL(name, ippiDCTFwdFree_32f, ((IppiDCTFwdSpec_32f*) m_pSpec ))
   MATE_CALL(name, ippiDCTInvFree_32f, ((IppiDCTInvSpec_32f*) m_pSpec ))
   return stsNoFunction;
}

IppStatus CRunDct::CallGetBufSize(CString name)
{
   MATE_CALL(name, ippiDCTFwdGetBufSize_32f,((IppiDCTFwdSpec_32f*)m_pSpec, &m_bufSize ))
   MATE_CALL(name, ippiDCTInvGetBufSize_32f,((IppiDCTInvSpec_32f*)m_pSpec, &m_bufSize ))
   return stsNoFunction;
}

IppStatus CRunDct::CallIppFunction()
{
   FUNC_CALL(ippiDCTFwd_32f_C1R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTFwdSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTFwd_32f_C3R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTFwdSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTFwd_32f_C4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTFwdSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTFwd_32f_AC4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTFwdSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTInv_32f_C1R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTInvSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTInv_32f_C3R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTInvSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTInv_32f_C4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTInvSpec_32f*)m_pSpec,m_pBuffer ))
   FUNC_CALL(ippiDCTInv_32f_AC4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pDst, dstStep,(IppiDCTInvSpec_32f*)m_pSpec,m_pBuffer ))
   return stsNoFunction;
}

CString CRunDct::GetHistoryParms()
{
   CMyString parms;
   return parms << m_hint;
}



