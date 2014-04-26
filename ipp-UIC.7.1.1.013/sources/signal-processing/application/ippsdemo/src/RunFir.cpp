/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFir.cpp : implementation of the CRunFir class.
// CRunFir class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "Histo.h"
#include "RunFir.h"
#include "ParmFirDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFir::CRunFir()
{
   m_pState = NULL;
}

CRunFir::~CRunFir()
{

}

BOOL CRunFir::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;

   m_pDelay.Init(m_Func.DstType());

   m_InitName = "";
   m_FreeName = "";
   return TRUE;
}

CParamDlg* CRunFir::CreateDlg()
{
   return new CParmFirDlg;
}

void CRunFir::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CRunFirD::UpdateData(parmDlg,save);
   CParmFirDlg *pDlg = (CParmFirDlg*)parmDlg;
   if (save) {
      m_MR = pDlg->m_MR;
   } else {
      pDlg->m_MR = m_MR;
   }
}

void CRunFir::SetInitFreeNames()
{
   CString tapsTypeStr = CFunc::TypeToString(m_Func.BaseType()) + "_" + m_Func.TypeName();
   m_FreeName = "ippsFIRFree" + tapsTypeStr;
   if (((m_Func.BaseType()|PP_CPLX) == pp32sc) && 
      ((m_pDocTaps->GetVector()->Type()|PP_CPLX) == pp32fc))
      tapsTypeStr += m_pDocTaps->GetVector()->TypeString();
   m_InitName = (m_MR ? "ippsFIRMRInitAlloc" : "ippsFIRInitAlloc") + tapsTypeStr;
}

BOOL CRunFir::BeforeCall()
{
   CRunFirD::BeforeCall();

   SetInitFreeNames();

   IppStatus status = CallInit();
   IppErrorMessage(m_InitName, status);

   m_pDelay.Deallocate();
   if (status < 0) return FALSE;
   return TRUE;
}

BOOL CRunFir::AfterCall(BOOL bOk)
{
   IppStatus status = CallFree(); m_pState = NULL;
   IppErrorMessage(m_FreeName, status);
   return TRUE;
}

IppStatus CRunFir::CallInit()
{
   MATE_CALL(m_InitName, ippsFIRInitAlloc32s_16s32f, ((IppsFIRState32s_16s**)&m_pState,
      (const Ipp32f*)m_pTaps, m_tapsLen, (const Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32s_16s32f, ((IppsFIRState32s_16s**)&m_pState,
      (const Ipp32f*)m_pTaps, m_tapsLen, m_upFactor, m_upPhase,
      m_downFactor, m_downPhase, (const Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc32sc_16sc, ((IppsFIRState32sc_16sc**)&m_pState,
      (const Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor, (const Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32sc_16sc, ((IppsFIRState32sc_16sc**)&m_pState,
      (const Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor, m_upFactor,
      m_upPhase, m_downFactor, m_downPhase, (const Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc32sc_16sc32fc, ((IppsFIRState32sc_16sc**)&m_pState,
      (const Ipp32fc*)m_pTaps, m_tapsLen, (const Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32sc_16sc32fc, ((IppsFIRState32sc_16sc**)&m_pState,
      (const Ipp32fc*)m_pTaps, m_tapsLen, m_upFactor, m_upPhase,
      m_downFactor, m_downPhase, (const Ipp16sc*)m_pDelay))

   MATE_CALL(m_InitName, ippsFIRInitAlloc_32s, ((IppsFIRState_32s**)&m_pState, (Ipp32s*)m_pTaps, m_tapsLen,
       (Ipp32s*)m_pDelay))

   MATE_CALL(m_InitName, ippsFIRInitAlloc_32f, ((IppsFIRState_32f**)&m_pState, (Ipp32f*)m_pTaps, m_tapsLen,
       (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc_32f, ((IppsFIRState_32f**)&m_pState, (Ipp32f*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc_32fc, ((IppsFIRState_32fc**)&m_pState, (Ipp32fc*)m_pTaps, m_tapsLen,
       (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc_32fc, ((IppsFIRState_32fc**)&m_pState, (Ipp32fc*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc32f_16s, ((IppsFIRState32f_16s**)&m_pState, (Ipp32f*)m_pTaps, m_tapsLen,
       (Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32f_16s, ((IppsFIRState32f_16s**)&m_pState, (Ipp32f*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc32fc_16sc, ((IppsFIRState32fc_16sc**)&m_pState, (Ipp32fc*)m_pTaps, m_tapsLen,
       (Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32fc_16sc, ((IppsFIRState32fc_16sc**)&m_pState, (Ipp32fc*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc_64f, ((IppsFIRState_64f**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc_64f, ((IppsFIRState_64f**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp64f*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc_64fc, ((IppsFIRState_64fc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc_64fc, ((IppsFIRState_64fc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp64fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc64f_32f, ((IppsFIRState64f_32f**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc64f_32f, ((IppsFIRState64f_32f**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp32f*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc64fc_32fc, ((IppsFIRState64fc_32fc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc64fc_32fc, ((IppsFIRState64fc_32fc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp32fc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc64f_32s, ((IppsFIRState64f_32s**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       (Ipp32s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc64f_32s, ((IppsFIRState64f_32s**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp32s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc64fc_32sc, ((IppsFIRState64fc_32sc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       (Ipp32sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc64fc_32sc, ((IppsFIRState64fc_32sc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp32sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc64f_16s, ((IppsFIRState64f_16s**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       (Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc64f_16s, ((IppsFIRState64f_16s**)&m_pState, (Ipp64f*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc64fc_16sc, ((IppsFIRState64fc_16sc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       (Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc64fc_16sc, ((IppsFIRState64fc_16sc**)&m_pState, (Ipp64fc*)m_pTaps, m_tapsLen,
       m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc32s_16s, ((IppsFIRState32s_16s**)&m_pState, (Ipp32s*)m_pTaps, m_tapsLen,
       m_tapsFactor, (Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32s_16s, ((IppsFIRState32s_16s**)&m_pState, (Ipp32s*)m_pTaps, m_tapsLen,
       m_tapsFactor, m_upFactor, m_upPhase, m_downFactor, m_downPhase,
       (Ipp16s*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRInitAlloc32sc_16sc, ((IppsFIRState32sc_16sc**)&m_pState, (Ipp32sc*)m_pTaps, m_tapsLen, 
        m_tapsFactor, (Ipp16sc*)m_pDelay))
   MATE_CALL(m_InitName, ippsFIRMRInitAlloc32sc_16sc, ((IppsFIRState32sc_16sc**)&m_pState, (Ipp32sc*)m_pTaps,
       m_tapsLen, m_tapsFactor, m_upFactor, m_upPhase, m_downFactor,
       m_downPhase, (Ipp16sc*)m_pDelay))
   return stsNoFunction;
}

IppStatus CRunFir::CallFree()
{
   MATE_CALL(m_FreeName, ippsFIRFree_32s, ((IppsFIRState_32s*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree_32f, ((IppsFIRState_32f*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree_32fc, ((IppsFIRState_32fc*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree32f_16s, ((IppsFIRState32f_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree32fc_16sc, ((IppsFIRState32fc_16sc*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree_64f, ((IppsFIRState_64f*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree_64fc, ((IppsFIRState_64fc*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree64f_32f, ((IppsFIRState64f_32f*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree64fc_32fc, ((IppsFIRState64fc_32fc*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree64f_32s, ((IppsFIRState64f_32s*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree64fc_32sc, ((IppsFIRState64fc_32sc*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree64f_16s, ((IppsFIRState64f_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree64fc_16sc, ((IppsFIRState64fc_16sc*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree32s_16s, ((IppsFIRState32s_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsFIRFree32sc_16sc, ((IppsFIRState32sc_16sc*)m_pState))
   return stsNoFunction;
}

IppStatus CRunFir::CallIppFunction()
{
   FUNC_CALL(ippsFIROne_32f, (((Ipp32f*)pSrc)[m_iOne], (Ipp32f*)pDst + m_iOne, (IppsFIRState_32f*)m_pState))
   FUNC_CALL(ippsFIROne_32fc, (((Ipp32fc*)pSrc)[m_iOne], (Ipp32fc*)pDst + m_iOne, (IppsFIRState_32fc*)m_pState))
   FUNC_CALL(ippsFIROne32f_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne,
        (IppsFIRState32f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne32fc_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne,
        (IppsFIRState32fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne_64f, (((Ipp64f*)pSrc)[m_iOne], (Ipp64f*)pDst + m_iOne, (IppsFIRState_64f*)m_pState))
   FUNC_CALL(ippsFIROne_64fc, (((Ipp64fc*)pSrc)[m_iOne], (Ipp64fc*)pDst + m_iOne, (IppsFIRState_64fc*)m_pState))
   FUNC_CALL(ippsFIROne64f_32f, (((Ipp32f*)pSrc)[m_iOne], (Ipp32f*)pDst + m_iOne, (IppsFIRState64f_32f*)m_pState))
   FUNC_CALL(ippsFIROne64fc_32fc, (((Ipp32fc*)pSrc)[m_iOne], (Ipp32fc*)pDst + m_iOne, (IppsFIRState64fc_32fc*)m_pState))
   FUNC_CALL(ippsFIROne64f_32s_Sfs, (((Ipp32s*)pSrc)[m_iOne], (Ipp32s*)pDst + m_iOne,
        (IppsFIRState64f_32s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne64fc_32sc_Sfs, (((Ipp32sc*)pSrc)[m_iOne], (Ipp32sc*)pDst + m_iOne,
        (IppsFIRState64fc_32sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne64f_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne,
        (IppsFIRState64f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne64fc_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne,
        (IppsFIRState64fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne32s_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, 
        (IppsFIRState32s_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIROne32sc_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne, 
        (IppsFIRState32sc_16sc*)m_pState, scaleFactor))
   FUNC_CALL( ippsFIROne_32s_Sfs,(((Ipp32s*)pSrc)[m_iOne], (Ipp32s*)pDst + m_iOne,
         (IppsFIRState_32s*)m_pState, scaleFactor))

   FUNC_CALL(ippsFIR_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len,
        (IppsFIRState_32f*)m_pState))
   FUNC_CALL(ippsFIR_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len,
        (IppsFIRState_32fc*)m_pState))
   FUNC_CALL(ippsFIR32f_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
        (IppsFIRState32f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR32fc_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
        (IppsFIRState32fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR_32f_I, ((Ipp32f*)pSrc, len, 
        (IppsFIRState_32f*)m_pState))
   FUNC_CALL(ippsFIR_32fc_I, ((Ipp32fc*)pSrc, len, 
        (IppsFIRState_32fc*)m_pState))
   FUNC_CALL(ippsFIR32f_16s_ISfs, ((Ipp16s*)pSrc, len,
        (IppsFIRState32f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR32fc_16sc_ISfs, ((Ipp16sc*)pSrc, len,
        (IppsFIRState32fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR_64f, ((Ipp64f*)pSrc, (Ipp64f*)pDst, len, 
        (IppsFIRState_64f*)m_pState))
   FUNC_CALL(ippsFIR_64fc, ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, len,
        (IppsFIRState_64fc*)m_pState))
   FUNC_CALL(ippsFIR_64f_I, ((Ipp64f*)pSrc, len,
        (IppsFIRState_64f*)m_pState))
   FUNC_CALL(ippsFIR_64fc_I, ((Ipp64fc*)pSrc, len,
        (IppsFIRState_64fc*)m_pState))
   FUNC_CALL(ippsFIR64f_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len,
        (IppsFIRState64f_32f*)m_pState))
   FUNC_CALL(ippsFIR64fc_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len,
        (IppsFIRState64fc_32fc*)m_pState))
   FUNC_CALL(ippsFIR64f_32f_I, ((Ipp32f*)pSrc, len,
        (IppsFIRState64f_32f*)m_pState))
   FUNC_CALL(ippsFIR64fc_32fc_I, ((Ipp32fc*)pSrc, len,
        (IppsFIRState64fc_32fc*)m_pState))
   FUNC_CALL(ippsFIR64f_32s_Sfs, ((Ipp32s*)pSrc, (Ipp32s*)pDst, len, 
        (IppsFIRState64f_32s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64fc_32sc_Sfs, ((Ipp32sc*)pSrc, (Ipp32sc*)pDst, len,
        (IppsFIRState64fc_32sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64f_32s_ISfs, ((Ipp32s*)pSrc, len,
        (IppsFIRState64f_32s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64fc_32sc_ISfs, ((Ipp32sc*)pSrc, len,
        (IppsFIRState64fc_32sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64f_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
        (IppsFIRState64f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64fc_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
        (IppsFIRState64fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64f_16s_ISfs, ((Ipp16s*)pSrc, len,
        (IppsFIRState64f_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR64fc_16sc_ISfs, ((Ipp16sc*)pSrc, len,
        (IppsFIRState64fc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR32s_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, 
        len, (IppsFIRState32s_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR32sc_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, 
        len, (IppsFIRState32sc_16sc*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR32s_16s_ISfs, ((Ipp16s*)pSrc, len, 
        (IppsFIRState32s_16s*)m_pState, scaleFactor))
   FUNC_CALL(ippsFIR32sc_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
        (IppsFIRState32sc_16sc*)m_pState, scaleFactor))
   FUNC_CALL( ippsFIR_32s_Sfs,((const Ipp32s*)pSrc, (Ipp32s*)pDst,
         len, (IppsFIRState_32s*)m_pState, scaleFactor))
   FUNC_CALL( ippsFIR_32s_ISfs,((Ipp32s*)pSrc, len,
         (IppsFIRState_32s*)m_pState, scaleFactor))

   return stsNoFunction;
}

void CRunFir::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString parms;
   parms << m_pDocTaps->GetTitle() 
         << ", " << ((CippsDemoDoc*)m_pDocTaps)->Length();
   if (((CVector*)m_pDocTaps)->Type() & PP_SIGN)
      parms << m_tapsFactor << ", ";
   if (m_InitName.Found("MR"))
      parms << ", " << m_upFactor
            << ", " << m_upPhase
            << ", " << m_downFactor
            << ", " << m_downPhase;
   parms << ", Dly=Zero";
   pHisto->AddFuncString(m_InitName, parms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}

CString CRunFir::GetHistoryParms()
{
   CMyString parms;
   parms << len;
   if (m_Func.Scale())
      parms << ", " << scaleFactor;
   return parms;
}


//////////////////////////////////////////////////////////////////////////

