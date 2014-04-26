/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWt.cpp : implementation of the CRunWt class.
// CRunWt class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "Histo.h"
#include "RunWT.h"
#include "ParmWtDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunWt::CRunWt()
{
   m_pState = NULL;
   m_pDocTapsLow = NULL;
   m_pDocTapsHigh = NULL;
   m_offsLow = 0;
   m_offsHigh = 0;
   m_pTapsLow = NULL;
   m_pTapsHigh = NULL;
   m_tapsLenLow = 0;
   m_tapsLenHigh = 0;
}

CRunWt::~CRunWt()
{

}

BOOL CRunWt::Open(CFunc func)
{
   if (!CRunWtHaar::Open(func)) return FALSE;
   m_InitFunc = m_Fwd ? "ippsWTFwdInitAlloc_" : "ippsWTInvInitAlloc_";
   m_FreeFunc = m_Fwd ? "ippsWTFwdFree_" : "ippsWTInvFree_";
   m_InitFunc += func.TypeName();
   m_FreeFunc += func.TypeName();
   return TRUE;
}

CParamDlg* CRunWt::CreateDlg() 
{
   return new CParmWtDlg;
}

void CRunWt::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmWtDlg *pDlg = (CParmWtDlg*)parmDlg;
   if (save) {
      m_offsLow      = pDlg->m_offsLow   ;
      m_offsHigh     = pDlg->m_offsHigh   ;
   } else {
      pDlg->m_pDocTapsLow   = m_pDocTapsLow;
      pDlg->m_pDocTapsHigh  = m_pDocTapsHigh;
      pDlg->m_offsLow       = m_pDocTapsLow->GetVector()->Length() >> 1;
      pDlg->m_offsHigh      = m_pDocTapsHigh->GetVector()->Length() >> 1;
   }
}

CString CRunWt::VectorTerm()
{ 
   return (m_PickVecPos == VEC_TAPS_LOW || m_PickVecPos == VEC_TAPS_HIGH) ? "vector" : "signal";
}

BOOL CRunWt::PickNextPos()
{
   if (m_UsedVectors & VEC_SRC2) {
      if (m_PickVecPos < VEC_SRC2) {
         m_PickVecPos = VEC_SRC2;
         return TRUE;
      }
   }
   if (m_PickVecPos < VEC_TAPS_LOW) {
      m_PickVecPos = VEC_TAPS_LOW;
      return TRUE;
   }
   if (m_PickVecPos < VEC_TAPS_HIGH) {
      m_PickVecPos = VEC_TAPS_HIGH;
      return TRUE;
   }
   m_PickVecPos = -1;
   return FALSE;
}

BOOL CRunWt::PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID)
{
   if (m_PickVecPos == VEC_TAPS_LOW) {
      vectorName = "TapsLow";
      pickID = IDC_CURSOR_PTL;
      grabID = IDC_CURSOR_TL;
      return TRUE;
   }
   if (m_PickVecPos == VEC_TAPS_HIGH) {
      vectorName = "TapsHigh";
      pickID = IDC_CURSOR_PTH;
      grabID = IDC_CURSOR_TH;
      return TRUE;
   }
   return CippsRun::PickSetID(vectorName, pickID, grabID);
}

BOOL CRunWt::PickMarkDoc(CDemoDoc* pDoc)
{
   if (m_PickVecPos == VEC_TAPS_LOW ||
      m_PickVecPos == VEC_TAPS_HIGH) {
      ppType tapsType = m_Func.Found("Inv") ? m_Func.SrcType() : m_Func.DstType();
      BOOL flag = pDoc->GetVector()->Type() == tapsType;
      pDoc->MayBePicked(flag);
      return flag;
   }
   return CippsRun::PickMarkDoc(pDoc);
}

void CRunWt::GrabDoc(CDemoDoc* pDoc)
{
   if (m_PickVecPos == VEC_TAPS_LOW) {
      m_pDocTapsLow = pDoc;
      pDoc->IsPicked(TRUE);
   } else if (m_PickVecPos == VEC_TAPS_HIGH) {
      m_pDocTapsHigh = pDoc;
      pDoc->IsPicked(TRUE);
   } else {
      CippsRun::GrabDoc(pDoc);
   }
}

int CRunWt::GetDstLength()
{
   int length = CippsRun::GetDstLength();
   if (m_Fwd) 
      return length >> 1;
   else
      return length << 1;
}

BOOL CRunWt::BeforeCall()
{
   if (m_Fwd)
      len = GetDstLength();
   m_pTapsLow = m_pDocTapsLow->GetVector()->GetData();
   m_pTapsHigh = m_pDocTapsHigh->GetVector()->GetData();
   m_tapsLenLow = m_pDocTapsLow->GetVector()->Length();
   m_tapsLenHigh = m_pDocTapsHigh->GetVector()->Length();

   IppStatus status = CallInit();
   IppErrorMessage(m_InitFunc, status);
   if (status < 0) return FALSE;
   return TRUE;
}

BOOL CRunWt::AfterCall(BOOL bOk)
{
   IppStatus status = CallFree(); m_pState = NULL;
   IppErrorMessage(m_FreeFunc, status);
   return TRUE;
}

IppStatus CRunWt::CallInit()
{
   MATE_CALL(m_InitFunc, ippsWTFwdInitAlloc_32f, (
        (IppsWTFwdState_32f**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTFwdInitAlloc_8s32f, (
        (IppsWTFwdState_8s32f**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTFwdInitAlloc_8u32f, (
        (IppsWTFwdState_8u32f**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTFwdInitAlloc_16s32f, (
        (IppsWTFwdState_16s32f**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTFwdInitAlloc_16u32f, (
        (IppsWTFwdState_16u32f**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTInvInitAlloc_32f, (
        (IppsWTInvState_32f**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTInvInitAlloc_32f8s, (
        (IppsWTInvState_32f8s**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTInvInitAlloc_32f8u, (
        (IppsWTInvState_32f8u**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTInvInitAlloc_32f16s, (
        (IppsWTInvState_32f16s**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   MATE_CALL(m_InitFunc, ippsWTInvInitAlloc_32f16u, (
        (IppsWTInvState_32f16u**)&m_pState,
        (Ipp32f*)m_pTapsLow,  m_tapsLenLow,  m_offsLow,
        (Ipp32f*)m_pTapsHigh, m_tapsLenHigh, m_offsHigh))
   return stsNoFunction;
}

IppStatus CRunWt::CallFree()
{
   MATE_CALL(m_FreeFunc, ippsWTFwdFree_32f, ((IppsWTFwdState_32f*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTFwdFree_8s32f, ((IppsWTFwdState_8s32f*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTFwdFree_8u32f, ((IppsWTFwdState_8u32f*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTFwdFree_16s32f, ((IppsWTFwdState_16s32f*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTFwdFree_16u32f, ((IppsWTFwdState_16u32f*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTInvFree_32f, ((IppsWTInvState_32f*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTInvFree_32f8s, ((IppsWTInvState_32f8s*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTInvFree_32f8u, ((IppsWTInvState_32f8u*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTInvFree_32f16s, ((IppsWTInvState_32f16s*)m_pState))
   MATE_CALL(m_FreeFunc, ippsWTInvFree_32f16u, ((IppsWTInvState_32f16u*)m_pState))
   return stsNoFunction;
}

IppStatus CRunWt::CallIppFunction()
{
   FUNC_CALL(ippsWTFwd_32f, ((Ipp32f*)pSrc,
        (Ipp32f*)pDst, (Ipp32f*)pDst2, len,
        (IppsWTFwdState_32f*)m_pState))
   FUNC_CALL(ippsWTFwd_8s32f, ((Ipp8s*)pSrc,
        (Ipp32f*)pDst, (Ipp32f*)pDst2, len,
        (IppsWTFwdState_8s32f*)m_pState))
   FUNC_CALL(ippsWTFwd_8u32f, ((Ipp8u*)pSrc,
        (Ipp32f*)pDst, (Ipp32f*)pDst2, len, 
        (IppsWTFwdState_8u32f*)m_pState))
   FUNC_CALL(ippsWTFwd_16s32f, ((Ipp16s*)pSrc,
        (Ipp32f*)pDst, (Ipp32f*)pDst2, len,
        (IppsWTFwdState_16s32f*)m_pState))
   FUNC_CALL(ippsWTFwd_16u32f, (
        (Ipp16u*)pSrc, (Ipp32f*)pDst, (Ipp32f*)pDst2, len,
        (IppsWTFwdState_16u32f*)m_pState))

   FUNC_CALL(ippsWTInv_32f, (
        (Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp32f*)pDst,
        (IppsWTInvState_32f*)m_pState))
   FUNC_CALL(ippsWTInv_32f8s, (
        (Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp8s*)pDst,
        (IppsWTInvState_32f8s*)m_pState))
   FUNC_CALL(ippsWTInv_32f8u, (
        (Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp8u*)pDst,
        (IppsWTInvState_32f8u*)m_pState))
   FUNC_CALL(ippsWTInv_32f16s, (
        (Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp16s*)pDst,
        (IppsWTInvState_32f16s*)m_pState))
   FUNC_CALL(ippsWTInv_32f16u, (
        (Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp16u*)pDst,
        (IppsWTInvState_32f16u*)m_pState))

   return stsNoFunction;
}

void CRunWt::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString parms;
   parms << "pTapsLow, "
         << m_tapsLenLow << ", " << m_offsLow << ", "
         << m_tapsLenHigh << ", " << m_offsHigh << ", "
         << ".." ; 
   pHisto->AddFuncString(m_InitFunc, parms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}

CString CRunWt::GetHistoryParms()
{
   CMyString parms;
   return parms << len;
}
