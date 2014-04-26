/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunIir.cpp : implementation of the CRunIir class.
// CRunIir class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "Histo.h"
#include "RunIir.h"
#include "RunFir.h"
#include "ParmIirDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunIir::CRunIir()
{
   m_BQ = FALSE;
   m_pState = NULL;
   m_order = 0;
   m_numQuads = 0;
}

CRunIir::~CRunIir()
{

}

CParamDlg* CRunIir::CreateDlg()
{
   return new CParmIirDlg;
}

int CRunIir::GetOrder(const CVector* pTaps)
{
   return (pTaps->Length() >> 1) - 1;
}

int CRunIir::GetNumQuads(const CVector* pTaps)
{
   return pTaps->Length() / 6;
}

void CRunIir::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmIirDlg *pDlg = (CParmIirDlg*)parmDlg;
   if (save) {
      m_BQ = pDlg->m_BQ;
      m_tapsFactor = pDlg->m_tapsFactor;
   } else {
      pDlg->m_pDocTaps    = m_pDocTaps   ;
      pDlg->m_BQ = m_BQ;
      pDlg->m_tapsFactor  = m_tapsFactor   ;
   }
}

void CRunIir::SetInitFreeNames()
{
   CString sType0 = m_Func.TypeToString(m_Func.BaseType());
   CString sType1 = m_Func.TypeName();
   if (m_BQ)
      m_InitName = "ippsIIRInitAlloc" + sType0 + "_BiQuad_" + sType1;
   else
      m_InitName = "ippsIIRInitAlloc" + sType0 + "_" + sType1;
   m_FreeName = "ippsIIRFree" + sType0 + "_" + sType1;

   if (((m_Func.BaseType()|PP_CPLX) == pp32sc) && 
      ((m_pDocTaps->GetVector()->Type()|PP_CPLX) == pp32fc))
      m_InitName += m_pDocTaps->GetVector()->TypeString();
}

BOOL CRunIir::BeforeCall()
{
   m_numQuads = GetNumQuads(m_pDocTaps->GetVector());
   m_order = GetOrder(m_pDocTaps->GetVector());
   return CRunFir::BeforeCall();
}

IppStatus CRunIir::CallInit()
{
   MATE_CALL(m_InitName, ippsIIRInitAlloc32s_16s32f, ((IppsIIRState32s_16s**)&m_pState,
      (const Ipp32f*)m_pTaps, m_order, (const Ipp32s*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32sc_16sc32fc, ((IppsIIRState32sc_16sc**)&m_pState,
      (const Ipp32fc*)m_pTaps, m_order, (const Ipp32sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32s_BiQuad_16s32f, ((IppsIIRState32s_16s**)&m_pState,
      (const Ipp32f*)m_pTaps, m_numQuads, (const Ipp32s*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32sc_BiQuad_16sc32fc, ((IppsIIRState32sc_16sc**)&m_pState,
      (const Ipp32fc*)m_pTaps, m_numQuads, (const Ipp32sc*)m_pDelay))

   MATE_CALL(m_InitName, ippsIIRInitAlloc_32f, ((IppsIIRState_32f**)&m_pState, 
       (Ipp32f*)m_pTaps, m_order, (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc_32fc, ((IppsIIRState_32fc**)&m_pState, 
       (Ipp32fc*)m_pTaps, m_order, (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32f_16s, ((IppsIIRState32f_16s**)&m_pState,
       (Ipp32f*)m_pTaps, m_order, (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32fc_16sc, ((IppsIIRState32fc_16sc**)&m_pState, 
       (Ipp32fc*)m_pTaps, m_order, (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc_64f, ((IppsIIRState_64f**)&m_pState,
       (Ipp64f*)m_pTaps, m_order, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc_64fc, ((IppsIIRState_64fc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_order, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64f_32f, ((IppsIIRState64f_32f**)&m_pState,
       (Ipp64f*)m_pTaps, m_order, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64fc_32fc, ((IppsIIRState64fc_32fc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_order, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64f_32s, ((IppsIIRState64f_32s**)&m_pState,
       (Ipp64f*)m_pTaps, m_order, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64fc_32sc, ((IppsIIRState64fc_32sc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_order, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64f_16s, ((IppsIIRState64f_16s**)&m_pState,
       (Ipp64f*)m_pTaps, m_order, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64fc_16sc, ((IppsIIRState64fc_16sc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_order, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32s_16s, ((IppsIIRState32s_16s**)&m_pState,
       (Ipp32s*)m_pTaps, m_order, m_tapsFactor, (Ipp32s*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32sc_16sc, ((IppsIIRState32sc_16sc**)&m_pState,
       (Ipp32sc*)m_pTaps, m_order, m_tapsFactor, (Ipp32sc*)m_pDelay))

   MATE_CALL(m_InitName, ippsIIRInitAlloc_BiQuad_32f, ((IppsIIRState_32f**)&m_pState,
       (Ipp32f*)m_pTaps, m_numQuads, (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc_BiQuad_32fc, ((IppsIIRState_32fc**)&m_pState,
       (Ipp32fc*)m_pTaps, m_numQuads, (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32f_BiQuad_16s, ((IppsIIRState32f_16s**)&m_pState,
       (Ipp32f*)m_pTaps, m_numQuads, (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32fc_BiQuad_16sc, ((IppsIIRState32fc_16sc**)&m_pState,
       (Ipp32fc*)m_pTaps, m_numQuads, (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc_BiQuad_64f, ((IppsIIRState_64f**)&m_pState,
       (Ipp64f*)m_pTaps, m_numQuads, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc_BiQuad_64fc, ((IppsIIRState_64fc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_numQuads, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64f_BiQuad_32f, ((IppsIIRState64f_32f**)&m_pState,
       (Ipp64f*)m_pTaps, m_numQuads, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64fc_BiQuad_32fc, ((IppsIIRState64fc_32fc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_numQuads, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64f_BiQuad_32s, ((IppsIIRState64f_32s**)&m_pState,
       (Ipp64f*)m_pTaps, m_numQuads, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64fc_BiQuad_32sc, ((IppsIIRState64fc_32sc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_numQuads, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64f_BiQuad_16s, ((IppsIIRState64f_16s**)&m_pState,
       (Ipp64f*)m_pTaps, m_numQuads, (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc64fc_BiQuad_16sc, ((IppsIIRState64fc_16sc**)&m_pState,
       (Ipp64fc*)m_pTaps, m_numQuads, (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32s_BiQuad_16s, ((IppsIIRState32s_16s**)&m_pState,
       (Ipp32s*)m_pTaps, m_numQuads, m_tapsFactor, (Ipp32s*)m_pDelay))
   MATE_CALL(m_InitName, ippsIIRInitAlloc32sc_BiQuad_16sc, ((IppsIIRState32sc_16sc**)&m_pState,
       (Ipp32sc*)m_pTaps, m_numQuads, m_tapsFactor, (Ipp32sc*)m_pDelay))
   return stsNoFunction;
}

IppStatus CRunIir::CallFree()
{
   MATE_CALL(m_FreeName, ippsIIRFree_32f, ((IppsIIRState_32f*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree_32fc, ((IppsIIRState_32fc*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree32f_16s, ((IppsIIRState32f_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree32fc_16sc, ((IppsIIRState32fc_16sc*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree_64f, ((IppsIIRState_64f*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree_64fc, ((IppsIIRState_64fc*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree64f_32f, ((IppsIIRState64f_32f*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree64fc_32fc, ((IppsIIRState64fc_32fc*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree64f_32s, ((IppsIIRState64f_32s*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree64fc_32sc, ((IppsIIRState64fc_32sc*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree64f_16s, ((IppsIIRState64f_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree64fc_16sc, ((IppsIIRState64fc_16sc*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree32s_16s, ((IppsIIRState32s_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsIIRFree32sc_16sc, ((IppsIIRState32sc_16sc*)m_pState))
   return stsNoFunction;
}

IppStatus CRunIir::CallIppFunction()
{
   FUNC_CALL(ippsIIROne_32f, (((Ipp32f*)pSrc)[m_iOne], (Ipp32f*)pDst + m_iOne, (IppsIIRState_32f*)m_pState))
   FUNC_CALL(ippsIIROne_32fc, (((Ipp32fc*)pSrc)[m_iOne], (Ipp32fc*)pDst + m_iOne, (IppsIIRState_32fc*)m_pState))
   FUNC_CALL(ippsIIROne32f_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, (IppsIIRState32f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne32fc_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne, (IppsIIRState32fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne_64f, (((Ipp64f*)pSrc)[m_iOne], (Ipp64f*)pDst + m_iOne, (IppsIIRState_64f*)m_pState))
   FUNC_CALL(ippsIIROne_64fc, (((Ipp64fc*)pSrc)[m_iOne], (Ipp64fc*)pDst + m_iOne, (IppsIIRState_64fc*)m_pState))
   FUNC_CALL(ippsIIROne64f_32f, (((Ipp32f*)pSrc)[m_iOne], (Ipp32f*)pDst + m_iOne, (IppsIIRState64f_32f*)m_pState))
   FUNC_CALL(ippsIIROne64fc_32fc, (((Ipp32fc*)pSrc)[m_iOne], (Ipp32fc*)pDst + m_iOne, (IppsIIRState64fc_32fc*)m_pState))
   FUNC_CALL(ippsIIROne64f_32s_Sfs, (((Ipp32s*)pSrc)[m_iOne], (Ipp32s*)pDst + m_iOne, (IppsIIRState64f_32s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne64fc_32sc_Sfs, (((Ipp32sc*)pSrc)[m_iOne], (Ipp32sc*)pDst + m_iOne, (IppsIIRState64fc_32sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne64f_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, (IppsIIRState64f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne64fc_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne, (IppsIIRState64fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne32s_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne,
        (IppsIIRState32s_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIROne32sc_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne,
        (IppsIIRState32sc_16sc*)m_pState, scaleFactor))

   FUNC_CALL(ippsIIR_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, 
       (IppsIIRState_32f*)m_pState))
   FUNC_CALL(ippsIIR_32f_I, ((Ipp32f*)pSrc, len, (IppsIIRState_32f*)m_pState))
   FUNC_CALL(ippsIIR_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, 
       (IppsIIRState_32fc*)m_pState))
   FUNC_CALL(ippsIIR_32fc_I, ((Ipp32fc*)pSrc, len, (IppsIIRState_32fc*)m_pState))
   FUNC_CALL(ippsIIR32f_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
       (IppsIIRState32f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32f_16s_ISfs, ((Ipp16s*)pSrc, len, 
       (IppsIIRState32f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32fc_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
       (IppsIIRState32fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32fc_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
       (IppsIIRState32fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR_64f, ((Ipp64f*)pSrc, (Ipp64f*)pDst, len, 
       (IppsIIRState_64f*)m_pState))
   FUNC_CALL(ippsIIR_64f_I, ((Ipp64f*)pSrc, len, (IppsIIRState_64f*)m_pState))
   FUNC_CALL(ippsIIR_64fc, ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, len, 
       (IppsIIRState_64fc*)m_pState))
   FUNC_CALL(ippsIIR_64fc_I, ((Ipp64fc*)pSrc, len, (IppsIIRState_64fc*)m_pState))
   FUNC_CALL(ippsIIR64f_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, 
       (IppsIIRState64f_32f*)m_pState))
   FUNC_CALL(ippsIIR64f_32f_I, ((Ipp32f*)pSrc, len, (IppsIIRState64f_32f*)m_pState))
   FUNC_CALL(ippsIIR64fc_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, 
       (IppsIIRState64fc_32fc*)m_pState))
   FUNC_CALL(ippsIIR64fc_32fc_I, ((Ipp32fc*)pSrc, len, (IppsIIRState64fc_32fc*)m_pState))
   FUNC_CALL(ippsIIR64f_32s_Sfs, ((Ipp32s*)pSrc, (Ipp32s*)pDst, len, 
       (IppsIIRState64f_32s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64f_32s_ISfs, ((Ipp32s*)pSrc, len, 
       (IppsIIRState64f_32s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64fc_32sc_Sfs, ((Ipp32sc*)pSrc, (Ipp32sc*)pDst, len, 
       (IppsIIRState64fc_32sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64fc_32sc_ISfs, ((Ipp32sc*)pSrc, len, 
       (IppsIIRState64fc_32sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64f_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
       (IppsIIRState64f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64f_16s_ISfs, ((Ipp16s*)pSrc, len, 
       (IppsIIRState64f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64fc_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
       (IppsIIRState64fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR64fc_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
       (IppsIIRState64fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32s_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
       (IppsIIRState32s_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32sc_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
       (IppsIIRState32sc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32s_16s_ISfs, ((Ipp16s*)pSrc, len,
       (IppsIIRState32s_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsIIR32sc_16sc_ISfs, ((Ipp16sc*)pSrc, len,
       (IppsIIRState32sc_16sc*)m_pState, scaleFactor))

   return stsNoFunction;
}

void CRunIir::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString parms;
   parms << "m_pTaps, "
         << (m_InitName.Find("BiQuad") != -1 ? m_numQuads : m_order)
         << ", ";
   if (m_pDocTaps->GetVector()->Type() & PP_SIGN)
      parms << m_tapsFactor << ", ";
   parms << ", Dly=Zero";
   pHisto->AddFuncString(m_InitName, parms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}

