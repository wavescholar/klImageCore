/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTrian.cpp : implementation of the CRunTrian class.
// CRunTrian class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Histo.h"
#include "RunTrian.h"
#include "ParmTrianDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunTrian::CRunTrian()
{
   m_pState = NULL;

   m_parm[tr_magn].Init(pp16s,SHRT_MAX>>3);
   m_parm[tr_magn].Init(pp32f,1.);
   m_parm[tr_magn].Init(pp64f,1.);

   m_parm[tr_rfreq].Init(pp64f,0.02);
   m_parm[tr_rfreq].Init(pp16s,512);

   m_parm[tr_asym ].Init(pp64f,0.);
   m_parm[tr_asym ].Init(pp32s,0);
   
   m_parm[tr_phase].Init(pp64f,0.);
   m_parm[tr_phase].Init(pp32s,0);

   m_phase32 = 0;
   m_phase64 = 0;
}

CRunTrian::~CRunTrian()
{

}

BOOL CRunTrian::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;

   ppType mType = (ppType)(func.SrcType() & ~PP_CPLX);
   BOOL bQ15 = func.Found("Q15");
   m_parm[tr_magn ].Init(mType);
   m_parm[tr_rfreq].Init(bQ15 ? pp16s : pp64f);
   m_parm[tr_asym] .Init(bQ15 ? pp32s : pp64f);
   m_parm[tr_phase].Init(bQ15 ? pp32s : pp64f);
   return TRUE;
}

CParamDlg* CRunTrian::CreateDlg() { 
   return new CParmTrianDlg;
}

void CRunTrian::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmTrianDlg *pDlg = (CParmTrianDlg*)parmDlg;
   if (save) {
      for (int i=0; i<tr_NUM; i++) 
         m_parm[i].Set(pDlg->m_ParmStr[i]);
      m_phase64 = m_parm[tr_phase].GetDouble();
      m_phase32 = (float)m_phase64;
   } else {
      for (int i=0; i<tr_NUM; i++) 
         m_parm[i].Get(pDlg->m_ParmStr[i]);
   }
}

BOOL CRunTrian::BeforeCall()
{
   if (m_Func.Found("Direct")) 
      return TRUE;
   CHECK_CALL( ippsTriangleInitAllocQ15_16s, ((IppTriangleState_16s**)&m_pState,(Ipp16s)m_parm[tr_magn], 
                                (Ipp16s)m_parm[tr_rfreq], (Ipp32s)m_parm[tr_phase], (Ipp32s)m_parm[tr_asym]))
   return TRUE;
}

BOOL CRunTrian::AfterCall(BOOL bOk)
{
   if (m_Func.Found("Direct")) 
      return TRUE;
   CHECK_CALL( ippsTriangleFree, ((IppTriangleState_16s*)m_pState))
   return TRUE;
}

IppStatus CRunTrian::CallIppFunction()
{
   FUNC_CALL(ippsTriangle_Direct_64f, ((Ipp64f*)pSrc, len, (Ipp32f)m_parm[tr_magn],
                                (Ipp64f)m_parm[tr_rfreq].GetDouble(), (Ipp64f)m_parm[tr_asym].GetDouble(), 
                                &m_phase64))
   FUNC_CALL(ippsTriangle_Direct_64fc, ((Ipp64fc*)pSrc, len, (Ipp32f)m_parm[tr_magn],
                                (Ipp64f)m_parm[tr_rfreq].GetDouble(), (Ipp64f)m_parm[tr_asym].GetDouble(), 
                                &m_phase64))
   FUNC_CALL(ippsTriangle_Direct_32f, ((Ipp32f*)pSrc, len, (Ipp32f)m_parm[tr_magn],
                                (Ipp32f)m_parm[tr_rfreq].GetDouble(), (Ipp32f)m_parm[tr_asym].GetDouble(), 
                                &m_phase32))
   FUNC_CALL(ippsTriangle_Direct_32fc, ((Ipp32fc*)pSrc, len, (Ipp32f)m_parm[tr_magn],
                                (Ipp32f)m_parm[tr_rfreq].GetDouble(), (Ipp32f)m_parm[tr_asym].GetDouble(), 
                                &m_phase32))
   FUNC_CALL(ippsTriangle_Direct_16s, ((Ipp16s*)pSrc, len, (Ipp16s)m_parm[tr_magn],
                                (Ipp32f)m_parm[tr_rfreq].GetDouble(), (Ipp32f)m_parm[tr_asym].GetDouble(), 
                                &m_phase32))
   FUNC_CALL(ippsTriangle_Direct_16sc, ((Ipp16sc*)pSrc, len, (Ipp16s)m_parm[tr_magn],
                                (Ipp32f)m_parm[tr_rfreq].GetDouble(), (Ipp32f)m_parm[tr_asym].GetDouble(), 
                                &m_phase32))
   FUNC_CALL( ippsTriangleQ15_Direct_16s, ((Ipp16s*)pSrc, len, (Ipp16s)m_parm[tr_magn], 
                                (Ipp16s)m_parm[tr_rfreq], (Ipp32s)m_parm[tr_phase], (Ipp32s)m_parm[tr_asym]))

   FUNC_CALL( ippsTriangleQ15_16s, ((Ipp16s*)pSrc, len, (IppTriangleState_16s*)m_pState))

   return stsNoFunction;
}

CString CRunTrian::GetHistoryParms()
{
   CMyString parms;
   return parms << m_parm[tr_magn ] << ", " 
                << m_parm[tr_rfreq] << ", " 
                << m_parm[tr_asym ] << ", " 
                << m_parm[tr_phase];
}

void CRunTrian::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CippsRun::AddHistoFunc(pHisto,vecPos);
   if (m_Func.Found("Q15")) 
      return;
   CMyString parms;
   parms << "\tphase = ";
   if (m_Func.Found("64f"))
      parms << m_phase64;
   else
      parms << m_phase32;
   pHisto->AddTail(parms);
}
