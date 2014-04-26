/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunLms.cpp : implementation of the CRunLms class.
// CRunLms class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "MainFrm.h"
#include "Histo.h"
#include "RunLms.h"
#include "RunFir.h"
#include "ParmLmsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunLms::CRunLms()
{
   m_pDocTaps = NULL;
   m_pTaps = NULL;
   pState_32f = NULL;
   pState_16s = NULL;
   m_dlyIndex = 0;
   m_tapLen = 39;
   m_mu.Init(pp32f, 0.003f);
   m_mu.Init(pp32s, 64);
   m_bDirect = FALSE;

   m_DlyTypeDlg = CRunFir::DLY_ZERO;
}

CRunLms::~CRunLms()
{
}

BOOL CRunLms::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   m_pDocTaps = NULL;
   m_pTaps = NULL;

   m_bDirect = func.Found("Direct");

   if (func.Found("Q15")) 
      m_mu.Init(pp32s);
   else
      m_mu.Init(pp32f);

   m_TapsType = m_Func.BaseType();
   if (m_TapsType == ppNONE) m_TapsType = m_Func.SrcType();
   if (m_Func.BaseName().Found("Q15")) m_TapsType = pp32s;

   if (m_bDirect) {
      m_InitName = "";
      m_FreeName = "";
      m_GetTapsName = "";
   } else {
      m_InitName  = GetSpecName("InitAlloc"); 
      m_FreeName  = GetSpecName("Free"); 
      m_GetTapsName  = GetSpecName("GetTaps"); 
   }
   ppType dlyType = func.SrcType();
   m_pDelay.Init(dlyType);
   return TRUE;
}

CFunc CRunLms::GetSpecName(CString specName)
{
   ppType baseType = m_Func.BaseType();
   if (baseType == ppNONE) baseType = m_Func.SrcType();
   CString sType = "_" + m_Func.TypeName();
   if (baseType != m_Func.SrcType())
      sType = m_Func.TypeToString(baseType) + sType;

   return "ipps" + GetBaseName() + specName + sType; 
}

CParamDlg* CRunLms::CreateDlg()
{
   return new CParmLmsDlg;
}

void CRunLms::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmLmsDlg *pDlg = (CParmLmsDlg*)parmDlg;
   if (save) {
      m_mu          = pDlg->m_mu         ;
      m_tapLen      = pDlg->m_tapLen     ;
      m_DlyTypeDlg = pDlg->m_DlyTypeDlg;
   } else {
      pDlg->m_InitName  = m_InitName;
      pDlg->m_tapLen    = m_tapLen  ;
      pDlg->m_mu        = m_mu      ;
      pDlg->m_DlyTypeDlg = m_DlyTypeDlg;
   }
}

int CRunLms::GetSrcOffset()
{
   return CRunFir::GetSrcOffset(m_DlyTypeDlg, m_tapLen, m_lenSrc);
}

int CRunLms::GetDstLength()
{
   return CippsRun::GetDstLength() - GetSrcOffset();
}

BOOL CRunLms::BeforeCall()
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

BOOL CRunLms::AfterCall(BOOL bOk)
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
   pState_32f = NULL;
   pState_16s = NULL;

   return result;
}

void CRunLms::ActivateDst()
{
   CippsRun::ActivateDst();
   ActivateDoc((CippsDemoDoc*)m_pDocTaps);
   SetFilterHistory();
}

IppStatus CRunLms::CallInit()
{
   MATE_CALL(m_InitName, ippsFIRLMSInitAlloc_32f,(&pState_32f, (Ipp32f*)m_pTaps, m_tapLen, 
          (Ipp32f*)m_pDelay, m_dlyIndex))
   MATE_CALL(m_InitName, ippsFIRLMSInitAlloc32f_16s,(&pState_16s, (Ipp32f*)m_pTaps, m_tapLen, 
          (Ipp16s*)m_pDelay, m_dlyIndex ))
   return stsNoFunction;
}

IppStatus CRunLms::CallFree()
{
   MATE_CALL(m_FreeName, ippsFIRLMSFree_32f,(pState_32f))
   MATE_CALL(m_FreeName, ippsFIRLMSFree32f_16s,(pState_16s))
   return stsNoFunction;
}

IppStatus CRunLms::CallGetTaps()
{
   MATE_CALL(m_GetTapsName, ippsFIRLMSGetTaps_32f,(pState_32f, (Ipp32f*)m_pTaps))
   MATE_CALL(m_GetTapsName, ippsFIRLMSGetTaps32f_16s,(pState_16s, (Ipp32f*)m_pTaps))
   return stsNoFunction;
}

IppStatus CRunLms::CallIppFunction()
{
   FUNC_CALL(ippsFIRLMSOne_Direct_32f,(((Ipp32f*)pSrc)[m_iOne], ((Ipp32f*)pSrc2)[m_iOne], 
       (Ipp32f*)pDst + m_iOne, (Ipp32f*)m_pTaps, m_tapLen, (float)m_mu , (Ipp32f*)m_pDelay, 
       &m_dlyIndex))
   FUNC_CALL(ippsFIRLMSOne_Direct32f_16s,(((Ipp16s*)pSrc)[m_iOne], ((Ipp16s*)pSrc2)[m_iOne], 
       (Ipp16s*)pDst + m_iOne, (Ipp32f*)m_pTaps, m_tapLen, (float)m_mu, (Ipp16s*)m_pDelay, 
       &m_dlyIndex ))
   FUNC_CALL(ippsFIRLMSOne_DirectQ15_16s,(((Ipp16s*)pSrc)[m_iOne], ((Ipp16s*)pSrc2)[m_iOne], 
       (Ipp16s*)pDst + m_iOne, (Ipp32s*)m_pTaps, m_tapLen, (int)m_mu, (Ipp16s*)m_pDelay, 
       &m_dlyIndex ))

   FUNC_CALL(ippsFIRLMS_32f,( (Ipp32f*)pSrc, (Ipp32f*)pSrc2, 
       (Ipp32f*)pDst, len, (float)m_mu, pState_32f ))
   FUNC_CALL(ippsFIRLMS32f_16s,( (Ipp16s*)pSrc, (Ipp16s*)pSrc2, 
       (Ipp16s*)pDst, len, (float)m_mu, pState_16s ))

   return stsNoFunction;
}

CString CRunLms::GetHistoryParms()
{
   CMyString parms;
   if (m_Func.Found("One")) {
      parms << "pTapsInv, " << m_tapLen 
            << ", Dly=" << (GetSrcOffset() ? "Src" : "Zero")
            << ", " << m_dlyIndex 
            << ", " << m_mu.String();
   } else {
      parms << m_mu.String();
   }
   return parms;
}

void CRunLms::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   if (!m_InitName.IsEmpty()) {
      CMyString parms;
      parms << "pTapsInv, " << m_tapLen
            << ", pDelay=" << (GetSrcOffset() ? "Src" : "Zero")
            << ", " << m_dlyIndex << ", .." ; 
      pHisto->AddFuncString(m_InitName, parms);
   }
   CippsRun::AddHistoFunc(pHisto,vecPos);
}

void CRunLms::SetFilterHistory()
{
   ASSERT(m_pDocTaps);
   CHisto* pHisto = m_pDocTaps->GetVector()->GetHisto();
   pHisto->RemoveAll();
   pHisto->AddTail(m_pDocSrc->GetTitle());
   pHisto->AddTail(m_pDocSrc2->GetTitle());
   AddHistoFunc(m_pDocTaps->GetVector()->GetHisto(), VEC_DST2);
   pHisto->AddFuncString(m_GetTapsName);
   pHisto->AddTail(m_pDocTaps->GetTitle());
   m_pDocTaps->UpdateFrameTitle(m_Func + " - " + m_Func.VecName(VEC_DST2));
}


