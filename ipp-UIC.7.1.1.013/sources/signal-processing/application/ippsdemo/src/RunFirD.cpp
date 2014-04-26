/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFirD.cpp : implementation of the CRunFirD class.
// CRunFirD class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "ParmFirDlg.h"
#include "RunFirD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFirD::CRunFirD()
{
   m_tapsFactor = 0;
   m_pDocTaps = NULL;
   m_pTaps = NULL;
   m_tapsLen = 0;
   m_MR = FALSE;
   m_upFactor   = 2;
   m_upPhase    = 0;
   m_downFactor = 2;
   m_downPhase  = 0;
   m_delayIndex = 0;

   m_scaleFactorSave = 0;
   m_tapsFactorSave= 0;
   m_docFactor = 0;
   m_bScaleFactor = FALSE;
   m_bTapsFactor = FALSE;
}

CRunFirD::~CRunFirD()
{

}

BOOL CRunFirD::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;

   m_pDelay.Init(m_Func.DstType());

   m_MR = func.Found("MR");

   m_scaleFactorSave = scaleFactor;
   m_tapsFactorSave = m_tapsFactor;

   return TRUE;
}

void CRunFirD::Close() 
{
   if (m_docFactor) {
      if (m_bScaleFactor && (scaleFactor == -m_docFactor))
         scaleFactor = m_scaleFactorSave;
      if (m_bTapsFactor && (m_tapsFactor == m_docFactor))
         m_tapsFactor = m_tapsFactorSave;
   }
}

CString CRunFirD::VectorTerm()
{ 
   return m_PickVecPos == VEC_TAPS ? "vector" : "signal";
}

BOOL CRunFirD::PickNextPos()
{
   if (m_PickVecPos < VEC_TAPS) {
      m_PickVecPos = VEC_TAPS;
      return TRUE;
   }
   m_PickVecPos = -1;
   return FALSE;
}

BOOL CRunFirD::PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID)
{
   if (m_PickVecPos == VEC_TAPS) {
      vectorName = "Taps";
      pickID = IDC_CURSOR_PT;
      grabID = IDC_CURSOR_T;
      return TRUE;
   }
   return CippsRun::PickSetID(vectorName, pickID, grabID);
}

BOOL CRunFirD::PickMarkDoc(CDemoDoc* pDoc)
{
   if (m_PickVecPos == VEC_TAPS) {
      ppType tapsType = m_Func.BaseType();
      if (tapsType == ppNONE) 
         tapsType = m_Func.DstType();
      BOOL flag = pDoc->GetVector()->Type() == tapsType;
      if (!m_Func.Found("Direct") &&
         ((m_pDocSrc->GetVector()->Type() | PP_CPLX) == pp16sc) &&
         ((tapsType | PP_CPLX) == pp32sc)) {
            tapsType = (ppType)((tapsType & PP_CPLX) | pp32f);
            if (pDoc->GetVector()->Type() == tapsType)
               flag = TRUE;
      }
      pDoc->MayBePicked(flag);
      return flag;
   }
   return CippsRun::PickMarkDoc(pDoc);
}

void CRunFirD::GrabDoc(CDemoDoc* pDoc)
{
   if (m_PickVecPos == VEC_TAPS) {
      m_pDocTaps = pDoc;
      pDoc->IsPicked(TRUE);
   } else {
      CippsRun::GrabDoc(pDoc);
   }
}

CParamDlg* CRunFirD::CreateDlg()
{
   return new CParmFirDlg;
}

void CRunFirD::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   m_bTapsFactor = 
      (((m_pDocSrc->GetVector()->Type() | PP_CPLX) == pp16sc) &&
       ((m_pDocTaps->GetVector()->Type() | PP_CPLX) == pp32sc));
   m_bScaleFactor = 
      (((m_pDocSrc->GetVector()->Type() | PP_CPLX) == pp32sc) &&
       ((m_pDocTaps->GetVector()->Type() | PP_CPLX) == pp32sc));
   m_docFactor = m_pDocTaps->GetVector()->GetTapsFactor();


   if (!save && m_bScaleFactor && m_docFactor)
      scaleFactor = -m_docFactor;
   CippsRun::UpdateData(parmDlg,save);

   CParmFirDlg *pDlg = (CParmFirDlg*)parmDlg;
   if (save) {
      m_tapsFactor  = pDlg->m_tapsFactor;
      m_upFactor    = pDlg->m_upFactor   ;
      m_upPhase     = pDlg->m_upPhase    ;
      m_downFactor  = pDlg->m_downFactor ;
      m_downPhase   = pDlg->m_downPhase  ;
   } else {
      pDlg->m_pDocTaps    = m_pDocTaps   ;
      pDlg->m_tapsFactor  = m_docFactor ? m_docFactor : m_tapsFactor;
      pDlg->m_upFactor    = m_upFactor   ;
      pDlg->m_upPhase     = m_upPhase    ;
      pDlg->m_downFactor  = m_downFactor ;
      pDlg->m_downPhase   = m_downPhase  ;      
   }
}

BOOL CRunFirD::BeforeCall()
{
   m_pDelay.Allocate(m_pDocTaps->GetVector()->Length()*2);
   return TRUE;
}

BOOL CRunFirD::AfterCall(BOOL bOk)
{
   m_pDelay.Deallocate();
   return TRUE;
}

int CRunFirD::GetMRLen()
{
   int length = m_lenSrc;
   if (!m_MR) return length;
   int factor = m_downFactor;
   if (m_Func.Inplace() && (m_downFactor < m_upFactor))
      factor = m_upFactor;
   return length / factor;
}

int CRunFirD::GetDstLength()
{
   m_lenSrc = m_pDocSrc->GetVector()->Length();
   int length = m_lenSrc;
   if (!m_MR) return length;
   length = GetMRLen()*m_upFactor;
   if (length < 1) length = 1;
   return length;
}

void CRunFirD::PrepareParameters()
{
   CippsRun::PrepareParameters();
   SetParams((CippsDemoDoc*)m_pDocTaps, m_pTaps, m_tapsLen);
   len = GetMRLen();
}

IppStatus CRunFirD::CallIppFunction()
{
   FUNC_CALL(ippsFIRMR_Direct_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, 
        (Ipp32f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32f*)m_pDelay))
   FUNC_CALL(ippsFIRMR_Direct_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, 
        (Ipp32fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32fc*)m_pDelay))

   FUNC_CALL(ippsFIRMR_Direct_32f_I, ((Ipp32f*)pSrc, len, 
        (Ipp32f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32f*)m_pDelay))
   FUNC_CALL(ippsFIRMR_Direct_32fc_I, ((Ipp32fc*)pSrc, len, 
        (Ipp32fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32fc*)m_pDelay))

   FUNC_CALL(ippsFIRMR32f_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
        (Ipp32f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR32fc_Direct_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
        (Ipp32fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR32f_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, 
        (Ipp32f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR32fc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
        (Ipp32fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR_Direct_64f, ((Ipp64f*)pSrc, (Ipp64f*)pDst, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp64f*)m_pDelay))
   FUNC_CALL(ippsFIRMR_Direct_64fc, ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp64fc*)m_pDelay))

   FUNC_CALL(ippsFIRMR_Direct_64f_I, ((Ipp64f*)pSrc, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp64f*)m_pDelay))
   FUNC_CALL(ippsFIRMR_Direct_64fc_I, ((Ipp64fc*)pSrc, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp64fc*)m_pDelay))

   FUNC_CALL(ippsFIRMR64f_Direct_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32f*)m_pDelay))
   FUNC_CALL(ippsFIRMR64fc_Direct_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32fc*)m_pDelay))

   FUNC_CALL(ippsFIRMR64f_Direct_32f_I, ((Ipp32f*)pSrc, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32f*)m_pDelay))
   FUNC_CALL(ippsFIRMR64fc_Direct_32fc_I, ((Ipp32fc*)pSrc, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32fc*)m_pDelay))

   FUNC_CALL(ippsFIRMR64f_Direct_32s_Sfs, ((Ipp32s*)pSrc, (Ipp32s*)pDst, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR64fc_Direct_32sc_Sfs, ((Ipp32sc*)pSrc, (Ipp32sc*)pDst, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR64f_Direct_32s_ISfs, ((Ipp32s*)pSrc, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR64fc_Direct_32sc_ISfs, ((Ipp32sc*)pSrc, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp32sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR64f_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR64fc_Direct_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR64f_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, 
        (Ipp64f*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR64fc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
        (Ipp64fc*)m_pTaps, m_tapsLen, m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR32s_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
        (Ipp32s*)m_pTaps, m_tapsLen, m_tapsFactor,
        m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR32sc_Direct_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
        (Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor,
        m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIRMR32s_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, 
        (Ipp32s*)m_pTaps, m_tapsLen, m_tapsFactor,
        m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16s*)m_pDelay, scaleFactor))
   FUNC_CALL(ippsFIRMR32sc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
        (Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor,
        m_upFactor,m_upPhase, m_downFactor, m_downPhase, 
        (Ipp16sc*)m_pDelay, scaleFactor))

   FUNC_CALL(ippsFIROne_Direct_32f, (((Ipp32f*)pSrc)[m_iOne], (Ipp32f*)pDst + m_iOne, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIROne_Direct_32fc, (((Ipp32fc*)pSrc)[m_iOne], (Ipp32fc*)pDst + m_iOne, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIROne_Direct_32f_I, ((Ipp32f*)pSrc + m_iOne, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIROne_Direct_32fc_I, ((Ipp32fc*)pSrc + m_iOne, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIROne32f_Direct_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne32fc_Direct_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne32f_Direct_16s_ISfs, ((Ipp16s*)pSrc + m_iOne, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne32fc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc + m_iOne, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne_Direct_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, (Ipp16s*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne_Direct_16s_ISfs, ((Ipp16s*)pSrc + m_iOne, (Ipp16s*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne_Direct_64f, (((Ipp64f*)pSrc)[m_iOne], (Ipp64f*)pDst + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp64f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIROne_Direct_64fc, (((Ipp64fc*)pSrc)[m_iOne], (Ipp64fc*)pDst + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp64fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIROne_Direct_64f_I, ((Ipp64f*)pSrc + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp64f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIROne_Direct_64fc_I, ((Ipp64fc*)pSrc + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp64fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIROne64f_Direct_32f, (((Ipp32f*)pSrc)[m_iOne], (Ipp32f*)pDst + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIROne64fc_Direct_32fc, (((Ipp32fc*)pSrc)[m_iOne], (Ipp32fc*)pDst + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIROne64f_Direct_32f_I, ((Ipp32f*)pSrc + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIROne64fc_Direct_32fc_I, ((Ipp32fc*)pSrc + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIROne64f_Direct_32s_Sfs, (((Ipp32s*)pSrc)[m_iOne], (Ipp32s*)pDst + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne64fc_Direct_32sc_Sfs, (((Ipp32sc*)pSrc)[m_iOne], (Ipp32sc*)pDst + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne64f_Direct_32s_ISfs, ((Ipp32s*)pSrc + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne64fc_Direct_32sc_ISfs, ((Ipp32sc*)pSrc + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne64f_Direct_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne64fc_Direct_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne64f_Direct_16s_ISfs, ((Ipp16s*)pSrc + m_iOne, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne64fc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc + m_iOne, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne32s_Direct_16s_Sfs, (((Ipp16s*)pSrc)[m_iOne], (Ipp16s*)pDst + m_iOne, 
        (Ipp32s*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne32sc_Direct_16sc_Sfs, (((Ipp16sc*)pSrc)[m_iOne], (Ipp16sc*)pDst + m_iOne, 
        (Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIROne32s_Direct_16s_ISfs, ((Ipp16s*)pSrc + m_iOne, 
        (Ipp32s*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIROne32sc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc + m_iOne, 
        (Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))
       
        
        
   FUNC_CALL(ippsFIR_Direct_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIR_Direct_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIR_Direct_32f_I, ((Ipp32f*)pSrc, len, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIR_Direct_32fc_I, ((Ipp32fc*)pSrc, len, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIR32f_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR32fc_Direct_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR32f_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, (Ipp32f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR32fc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc, len, (Ipp32fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (Ipp16s*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, (Ipp16s*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR_Direct_64f, ((Ipp64f*)pSrc, (Ipp64f*)pDst, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp64f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIR_Direct_64fc, ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp64fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIR_Direct_64f_I, ((Ipp64f*)pSrc, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp64f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIR_Direct_64fc_I, ((Ipp64fc*)pSrc, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp64fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIR64f_Direct_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIR64fc_Direct_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIR64f_Direct_32f_I, ((Ipp32f*)pSrc, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32f*)m_pDelay, &m_delayIndex))
   FUNC_CALL(ippsFIR64fc_Direct_32fc_I, ((Ipp32fc*)pSrc, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32fc*)m_pDelay, &m_delayIndex))

   FUNC_CALL(ippsFIR64f_Direct_32s_Sfs, ((Ipp32s*)pSrc, (Ipp32s*)pDst, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR64fc_Direct_32sc_Sfs, ((Ipp32sc*)pSrc, (Ipp32sc*)pDst, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR64f_Direct_32s_ISfs, ((Ipp32s*)pSrc, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp32s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR64fc_Direct_32sc_ISfs, ((Ipp32sc*)pSrc, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp32sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR64f_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR64fc_Direct_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR64f_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, (Ipp64f*)m_pTaps, m_tapsLen,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR64fc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc, len, (Ipp64fc*)m_pTaps, m_tapsLen,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR32s_Direct_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pDst, len, 
        (Ipp32s*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR32sc_Direct_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pDst, len, 
        (Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   FUNC_CALL(ippsFIR32s_Direct_16s_ISfs, ((Ipp16s*)pSrc, len, 
        (Ipp32s*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16s*)m_pDelay, &m_delayIndex, scaleFactor ))
   FUNC_CALL(ippsFIR32sc_Direct_16sc_ISfs, ((Ipp16sc*)pSrc, len, 
        (Ipp32sc*)m_pTaps, m_tapsLen, m_tapsFactor,
        (Ipp16sc*)m_pDelay, &m_delayIndex, scaleFactor ))

   return stsNoFunction;
}

CString CRunFirD::GetHistoryParms()
{
   CMyString parms;
   parms << len 
         << ", " << m_pDocTaps->GetTitle()
         << ", " << m_tapsLen;
   if ((m_pDocTaps->GetVector()->Type() | PP_CPLX) == pp32sc)
      parms << ", " << m_tapsFactor;
   if (m_MR) {
      parms << ", " << m_upFactor
            << ", " << m_upPhase
            << ", " << m_downFactor
            << ", " << m_downPhase;
   }
   parms << ", Dly=Zero";
   if (m_Func.Scale())
      parms << ", " << scaleFactor;
   return parms;
}

int CRunFirD::GetSrcOffset(int dlyType, int dlyLen, int srcLen)
{
   if ((dlyType == DLY_SRC) && srcLen > dlyLen) 
      return dlyLen;
   else
      return 0;
}

int CRunFirD::CreateDelayLine(CVector& pDelay, int dlyLen, CVector* pSrcVector, 
                              int dlyType, CFunc& func)
{
   BOOL bDirect = func.Found("Direct");
   pDelay.Allocate(dlyLen*(bDirect + 1));
   int srcOffset = GetSrcOffset(dlyType, dlyLen, pSrcVector->Length());
   if (srcOffset) {
      int j = bDirect ? srcOffset - 1 : 0;
      for (int i=0; i<srcOffset; i++) {
         pDelay.Set(i, pSrcVector->Get(j));
         j += bDirect ? -1 : 1;
      }
   }
   return srcOffset;
}

void* CRunFirD::GetOffsetPtr(CVector* pSrcVector, int srcOffset)
{
   return (char*)pSrcVector->GetData() + srcOffset*pSrcVector->ItemSize();
}
