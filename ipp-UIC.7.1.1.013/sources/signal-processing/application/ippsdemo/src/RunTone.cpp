/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTone.cpp : implementation of the CRunTone class.
// CRunTone class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Histo.h"
#include "RunTone.h"
#include "ParmToneDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunTone::CRunTone()
{
   m_pState = NULL;

   m_hint = ippAlgHintFast;

   m_parm[tn_magn].Init(pp16s,SHRT_MAX>>3);
   m_parm[tn_magn].Init(pp32f,1.);
   m_parm[tn_magn].Init(pp64f,1.);

   m_parm[tn_rfreq].Init(pp16s,512);
   m_parm[tn_rfreq].Init(pp64f,0.02);

   m_parm[tn_phase].Init(pp64f,0.);
   m_parm[tn_phase].Init(pp32s,0.);

   m_phase32f = 0;
   m_phase64f = 0;
}

CRunTone::~CRunTone()
{

}

BOOL CRunTone::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;

   ppType mType = (ppType)(func.SrcType() & ~PP_CPLX);
   BOOL bQ15 = func.Found("Q15");
   m_parm[tn_magn ].Init(mType);
   m_parm[tn_rfreq].Init(bQ15 ? pp16s : pp64f);
   m_parm[tn_phase].Init(bQ15 ? pp32s : pp64f);
   return TRUE;
}

CParamDlg* CRunTone::CreateDlg() { 
   return new CParmToneDlg;
}

void CRunTone::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmToneDlg *pDlg = (CParmToneDlg*)parmDlg;
   if (save) {
      m_hint = (IppHintAlgorithm)pDlg->m_hint;
      for (int i=0; i<tn_NUM; i++) 
         m_parm[i].Set(pDlg->m_ParmStr[i]);
      m_phase64f = m_parm[tn_phase].GetDouble();
      m_phase32f = (float)m_phase64f;
   } else {
      pDlg->m_hint = (int)m_hint;
      for (int i=0; i<tn_NUM; i++) 
         m_parm[i].Get(pDlg->m_ParmStr[i]);
   }
}

BOOL CRunTone::BeforeCall()
{
   if (m_Func.Found("Direct")) 
      return TRUE;
   CHECK_CALL(ippsToneInitAllocQ15_16s, ((IppToneState_16s**)&m_pState, (Ipp16s)m_parm[tn_magn], 
                             (Ipp16s)m_parm[tn_rfreq], (Ipp32s)m_parm[tn_phase]))
   return TRUE;
}

BOOL CRunTone::AfterCall(BOOL bOk)
{
   if (m_Func.Found("Direct")) 
      return TRUE;
   CHECK_CALL(ippsToneFree, ((IppToneState_16s*)m_pState))
   return TRUE;
}

IppStatus CRunTone::CallIppFunction()
{
   FUNC_CALL(ippsTone_Direct_64f, ((Ipp64f*)pSrc, len, (Ipp64f)m_parm[tn_magn],
                             (Ipp64f)m_parm[tn_rfreq].GetDouble(), &m_phase64f, m_hint))
   FUNC_CALL(ippsTone_Direct_64fc, ((Ipp64fc*)pSrc, len, (Ipp64f)m_parm[tn_magn],
                             (Ipp64f)m_parm[tn_rfreq].GetDouble(), &m_phase64f, m_hint))
   FUNC_CALL(ippsTone_Direct_32f, ((Ipp32f*)pSrc, len, (Ipp32f)m_parm[tn_magn],
                             (Ipp32f)m_parm[tn_rfreq].GetDouble(), &m_phase32f, m_hint))
   FUNC_CALL(ippsTone_Direct_32fc, ((Ipp32fc*)pSrc, len, (Ipp32f)m_parm[tn_magn],
                             (Ipp32f)m_parm[tn_rfreq].GetDouble(), &m_phase32f, m_hint))
   FUNC_CALL(ippsTone_Direct_16s, ((Ipp16s*)pSrc, len, (Ipp16s)m_parm[tn_magn],
                             (Ipp32f)m_parm[tn_rfreq].GetDouble(), &m_phase32f, m_hint))
   FUNC_CALL(ippsTone_Direct_16sc, ((Ipp16sc*)pSrc, len, (Ipp16s)m_parm[tn_magn],
                             (Ipp32f)m_parm[tn_rfreq].GetDouble(), &m_phase32f, m_hint))
   FUNC_CALL( ippsToneQ15_Direct_16s, ((Ipp16s*)pSrc, len, (Ipp16s)m_parm[tn_magn], 
                             (Ipp16s)m_parm[tn_rfreq], (Ipp32s)m_parm[tn_phase]))
   FUNC_CALL( ippsToneQ15_16s, ((Ipp16s*)pSrc, len, (IppToneState_16s*)m_pState))
   return stsNoFunction;
}

CString CRunTone::GetHistoryParms()
{
   CMyString parms;
   return parms << m_parm[tn_magn ] << ", " 
                << m_parm[tn_rfreq] << ", " 
                << m_parm[tn_phase] << ", " 
                << m_hint;
}

void CRunTone::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CippsRun::AddHistoFunc(pHisto,vecPos);
   if (m_Func.Found("Q15")) 
      return;
   CMyString parms;
   parms << "\tphase = ";
   if (m_Func.Found("64f"))
      parms << m_phase64f;
   else if (m_Func.Found("32f"))
      parms << m_phase32f;
   pHisto->AddTail(parms);
}
