/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunIirDirect.cpp : implementation of the CRunIirDirect class.
// CRunIirDirect class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "RunIir.h"
#include "RunIirDirect.h"
#include "ParmIirDlg.h"
#include "ippsDemoDoc.h"
//#include "Histo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunIirDirect::CRunIirDirect()
{
   m_BQ = FALSE;
   m_order = 0;
   m_numQuads = 0;
}

CRunIirDirect::~CRunIirDirect()
{

}

BOOL CRunIirDirect::Open(CFunc func) 
{
   if (!CRunFirD::Open(func)) return FALSE;
   m_BQ = func.Found("BiQuad");
   if (m_pDelay.Type() == pp16s)
      m_pDelay.Init(pp32s);
   return TRUE;
}

CParamDlg* CRunIirDirect::CreateDlg()
{
   return new CParmIirDlg;
}

void CRunIirDirect::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmIirDlg *pDlg = (CParmIirDlg*)parmDlg;
   if (save) {
   } else {
      pDlg->m_pDocTaps = m_pDocTaps   ;
      pDlg->m_BQ = m_BQ;
   }
}

int CRunIirDirect::GetTapsLen()
{
   int tapsLen = m_pDocTaps->GetVector()->Length();
   if (m_BQ) tapsLen = (int)(tapsLen/6) * 6;
   return tapsLen;
}

BOOL CRunIirDirect::BeforeCall()
{
   m_numQuads = CRunIir::GetNumQuads(m_pDocTaps->GetVector());
   m_order = CRunIir::GetOrder(m_pDocTaps->GetVector());
   return CRunFirD::BeforeCall();
}

IppStatus CRunIirDirect::CallIppFunction()
{
   FUNC_CALL(ippsIIR_Direct_16s,((Ipp16s*)pSrc, (Ipp16s*)pDst,
                 len, (Ipp16s*)m_pTaps, m_order, (Ipp32s*)m_pDelay ))
   FUNC_CALL(ippsIIR_Direct_16s_I,((Ipp16s*)pSrc, len,
                          (Ipp16s*)m_pTaps, m_order, (Ipp32s*)m_pDelay ))
   FUNC_CALL(ippsIIR_BiQuadDirect_16s,((Ipp16s*)pSrc, (Ipp16s*)pDst,
             len, (Ipp16s*)m_pTaps, m_numQuads, (Ipp32s*)m_pDelay ))
   FUNC_CALL(ippsIIR_BiQuadDirect_16s_I,((Ipp16s*)pSrc, len,
                     (Ipp16s *)m_pTaps, m_numQuads, (Ipp32s*)m_pDelay ))

   FUNC_CALL(ippsIIROne_Direct_16s,(*((Ipp16s*)pSrc + m_iOne), (Ipp16s*)pDst + m_iOne,
                          (Ipp16s*)m_pTaps, m_order, (Ipp32s*)m_pDelay ))
   FUNC_CALL(ippsIIROne_Direct_16s_I,((Ipp16s*)pSrc + m_iOne,
                          (Ipp16s*)m_pTaps, m_order, (Ipp32s*)m_pDelay ))
   FUNC_CALL(ippsIIROne_BiQuadDirect_16s,(*((Ipp16s*)pSrc + m_iOne), (Ipp16s*)pDst + m_iOne,
                      (Ipp16s*)m_pTaps, m_numQuads, (Ipp32s*)m_pDelay ))
   FUNC_CALL(ippsIIROne_BiQuadDirect_16s_I,((Ipp16s*)pSrc + m_iOne,
                      (Ipp16s*)m_pTaps, m_numQuads, (Ipp32s*)m_pDelay ))

   return stsNoFunction;
}

CString CRunIirDirect::GetHistoryParms()
{
   CMyString parms;
   if (m_BQ)
      parms << m_numQuads;
   else
      parms << m_order;
   parms << ", Dly=Zero";
   return parms;
}



