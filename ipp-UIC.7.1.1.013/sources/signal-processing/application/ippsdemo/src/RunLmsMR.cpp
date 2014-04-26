/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunLmsMR.cpp : implementation of the CRunLmsMR class.
// CRunLmsMR class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunLmsMR.h"
#include "ippsDemoDoc.h"
#include "MainFrm.h"
#include "Histo.h"
#include "RunFir.h"
#include "ParmLmsMRDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunLmsMR::CRunLmsMR()
{
   pState_16s = NULL;
   pState_16sc = NULL;
   m_dlyStep = 2;
   m_updateDly = 0;

}

CRunLmsMR::~CRunLmsMR()
{

}

BOOL CRunLmsMR::Open(CFunc func) 
{
   if (!CRunLms::Open(func)) return FALSE;

   m_mu.Init(pp32s);

   m_UpdateTapsName  = GetSpecName("UpdateTaps"); 
   m_PutValName  = GetSpecName("PutVal"); 
   return TRUE;
}

CParamDlg* CRunLmsMR::CreateDlg()
{
   return new CParmLmsMRDlg;
}

void CRunLmsMR::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CRunLms::UpdateData((CParmLmsDlg*)parmDlg, save);
   CParmLmsMRDlg *pDlg = (CParmLmsMRDlg*)parmDlg;
   if (save) {
      m_dlyStep   = atoi(pDlg->m_StepStr);
      m_updateDly = atoi(pDlg->m_UpdateStr);
   } else {
      pDlg->m_StepStr  .Format("%d", m_dlyStep  );
      pDlg->m_UpdateStr.Format("%d", m_updateDly);
   }
}

int CRunLmsMR::GetDstLength()
{
   int dstLen = CRunLms::GetDstLength();
   return dstLen / m_dlyStep;
}

BOOL CRunLmsMR::BeforeCall()
{
   m_pDocTaps = DEMO_APP->CreateTextDoc(m_TapsType, m_tapLen);
   if (!m_pDocTaps) return FALSE;
   m_pDocTaps->GetFrame()->ShowWindow(SW_HIDE);
   m_pTaps = m_pDocTaps->GetVector()->GetData();

   int offset = CRunFir::CreateDelayLine(m_pDelay, m_tapLen, m_pDocSrc->GetVector(),
      m_DlyTypeDlg, m_Func);
   pSrc = CRunFir::GetOffsetPtr(m_pDocSrc->GetVector(), offset);
   pSrc2 = CRunFir::GetOffsetPtr(m_pDocSrc2->GetVector(), offset);

   if (m_bDirect) return TRUE;

   IppStatus status = CallInit();
   IppErrorMessage(m_InitName, status);
   m_dlyIndex = 0;
   if (status < 0) return FALSE;

   return TRUE;
}

BOOL CRunLmsMR::AfterCall(BOOL bOk)
{
   m_pDelay.Deallocate();
   if (m_bDirect) return TRUE;

   IppStatus status = ippStsNoErr;
   BOOL result = TRUE;
   if (bOk) {
      status = CallGetTaps();
      IppErrorMessage(m_GetTapsName, status);
      if (status < 0) result = FALSE;
   }
   status = CallFree(); 
   IppErrorMessage(m_FreeName, status);
   if (status < 0) result = FALSE;
   pState_16s = NULL;
   pState_16sc = NULL;

   return result;
}

BOOL CRunLmsMR::CallIpp(BOOL bMessage)
{
   IppStatus result;
   BOOL bVal = m_Func.Found("Val");
   try {
      m_iDst = 0;
      for (m_iSrc=0; m_iSrc<len; m_iSrc++) {
         if (m_iSrc % m_dlyStep == 0) {
            if (!bVal) {
               result = CallPutVal();
               if (result < 0) break;
            }
            result = CallIppFunction();
            if (result < 0) break;
            result = CallUpdateTaps();
            if (result < 0) break;
            m_iDst++;
         } else {
            result = CallPutVal();
            if (result < 0) break;
         }
      }
   } catch (CSExcept except) {
      ExcIppMessage(except);
      return FALSE;
   }
   IppErrorMessage(result);
   return result >= 0 ? TRUE : FALSE;
}

IppStatus CRunLmsMR::CallInit()
{
   MATE_CALL(m_InitName, ippsFIRLMSMRInitAlloc32s_16s,( &pState_16s,
      (Ipp32s*)m_pTaps, m_tapLen, (Ipp16s*)m_pDelay, m_dlyIndex,
      m_dlyStep, m_updateDly, (int)m_mu ))
   MATE_CALL(m_InitName, ippsFIRLMSMRInitAlloc32sc_16sc,(&pState_16sc,
      (Ipp32sc*)m_pTaps, m_tapLen, (Ipp16sc*)m_pDelay, m_dlyIndex,
      m_dlyStep, m_updateDly, (int)m_mu))
   return stsNoFunction;
}

IppStatus CRunLmsMR::CallFree()
{
   MATE_CALL(m_FreeName, ippsFIRLMSMRFree32s_16s,(pState_16s))
   MATE_CALL(m_FreeName, ippsFIRLMSMRFree32sc_16sc,(pState_16sc))
   return stsNoFunction;
}

IppStatus CRunLmsMR::CallGetTaps()
{
   MATE_CALL(m_GetTapsName, ippsFIRLMSMRGetTaps32s_16s,(pState_16s, (Ipp32s*)m_pTaps))
   MATE_CALL(m_GetTapsName, ippsFIRLMSMRGetTaps32sc_16sc,(pState_16sc, (Ipp32sc*)m_pTaps))
   return stsNoFunction;
}

static Ipp32sc sub_32sc(Ipp32sc a, Ipp32sc b)
{
   Ipp32sc c;
   c.re = a.re - b.re;
   c.im = a.im - b.im;
   return c;
}

IppStatus CRunLmsMR::CallUpdateTaps()
{
   MATE_CALL(m_UpdateTapsName, ippsFIRLMSMRUpdateTaps32s_16s,
      (((Ipp32s*)pSrc2)[m_iSrc] - ((Ipp32s*)pDst)[m_iDst], pState_16s))
   MATE_CALL(m_UpdateTapsName, ippsFIRLMSMRUpdateTaps32sc_16sc,
      (sub_32sc(((Ipp32sc*)pSrc2)[m_iSrc], ((Ipp32sc*)pDst)[m_iDst]), pState_16sc))
   return stsNoFunction;
}

IppStatus CRunLmsMR::CallPutVal()
{
   MATE_CALL(m_PutValName, ippsFIRLMSMRPutVal32s_16s,
      (((Ipp16s*)pSrc)[m_iSrc], pState_16s))
   MATE_CALL(m_PutValName, ippsFIRLMSMRPutVal32sc_16sc,
      (((Ipp16sc*)pSrc)[m_iSrc], pState_16sc))
   return stsNoFunction;
}

IppStatus CRunLmsMR::CallIppFunction()
{
   FUNC_CALL(ippsFIRLMSMROne32s_16s,
      ((Ipp32s*)pDst + m_iDst, pState_16s))
   FUNC_CALL(ippsFIRLMSMROneVal32s_16s,
      (((Ipp16s*)pSrc)[m_iSrc], (Ipp32s*)pDst + m_iDst, pState_16s))
   FUNC_CALL(ippsFIRLMSMROne32sc_16sc,
      ((Ipp32sc*)pDst + m_iDst, pState_16sc))
   FUNC_CALL(ippsFIRLMSMROneVal32sc_16sc,
      (((Ipp16sc*)pSrc)[m_iSrc], (Ipp32sc*)pDst + m_iDst, pState_16sc))

   return stsNoFunction;
}

void CRunLmsMR::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString parms;
   parms << ".., pTaps, " << m_tapLen
         << ", pDelay=" << (GetSrcOffset() ? "Src" : "Zero")
         << ", " << m_dlyIndex
         << ", " << m_dlyStep
         << ", " << m_updateDly
         << ", " << (int)m_mu;
   pHisto->AddFuncString(m_InitName, parms);

   if (!m_Func.Found("Val"))
      pHisto->AddFuncString(m_PutValName);
   pHisto->AddFuncString(m_Func);
   pHisto->AddFuncString(m_UpdateTapsName, "", "", m_Perf, "", m_PerfUnit);
}

void CRunLmsMR::SetFilterHistory()
{
   ASSERT(m_pDocTaps);
   CHisto* pHisto = m_pDocTaps->GetVector()->GetHisto();
   pHisto->RemoveAll();
   pHisto->AddTail(m_pDocSrc->GetTitle());
   pHisto->AddTail(m_pDocSrc2->GetTitle());
   AddHistoFunc(pHisto);
   pHisto->AddFuncString(m_GetTapsName);
   pHisto->AddTail(m_pDocTaps->GetTitle());
   m_pDocTaps->UpdateFrameTitle(m_Func + " - " + m_Func.VecName(VEC_DST2));
}


