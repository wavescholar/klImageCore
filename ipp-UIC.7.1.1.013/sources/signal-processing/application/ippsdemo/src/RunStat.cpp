/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStat.cpp : implementation of the CRunStat class.
// CRunStat class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunStat.h"
#include "ParmStatDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunStat::CRunStat()
{
   m_hint = ippAlgHintFast;
}

CRunStat::~CRunStat()
{

}

BOOL CRunStat::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   m_value.Init(func,argDST);
   return TRUE;
}

void CRunStat::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmStatDlg *pDlg = (CParmStatDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      m_value.Init(m_Func,argDST);
      m_hint = (IppHintAlgorithm)pDlg->m_hint;
   } else {
      m_value.Get(pDlg->m_Re, pDlg->m_Im);
      pDlg->m_hint = (int)m_hint;
   }
}

BOOL CRunStat::CallIpp(BOOL bMessage)
{
   CParmStatDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunStat::SetValues(CParmStatDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunStat::CallIppFunction()
{
   FUNC_CALL(ippsStdDev_32f,((Ipp32f*)pSrc,len,(Ipp32f*)m_value,m_hint))
   FUNC_CALL(ippsStdDev_64f,((Ipp64f*)pSrc,len,(Ipp64f*)m_value))
   FUNC_CALL(ippsStdDev_16s32s_Sfs,((Ipp16s*)pSrc,len,
                                        (Ipp32s*)m_value,scaleFactor))
   FUNC_CALL(ippsStdDev_16s_Sfs,((Ipp16s*)pSrc,len,
                                     (Ipp16s*)m_value,scaleFactor))
   FUNC_CALL(ippsSum_32f, ((Ipp32f*) pSrc,len, (Ipp32f*)m_value,m_hint))
   FUNC_CALL(ippsSum_64f, ((Ipp64f*) pSrc,len, (Ipp64f*)m_value))
   FUNC_CALL(ippsSum_32fc,((Ipp32fc*)pSrc,len, (Ipp32fc*)m_value,m_hint))
   FUNC_CALL(ippsSum_64fc,((Ipp64fc*)pSrc,len, (Ipp64fc*)m_value))

   FUNC_CALL(ippsSum_16s32s_Sfs, ((Ipp16s*) pSrc, len,
                                       (Ipp32s*) m_value, scaleFactor))
   FUNC_CALL(ippsSum_16sc32sc_Sfs,((Ipp16sc*)pSrc, len,
                                       (Ipp32sc*)m_value, scaleFactor))
   FUNC_CALL(ippsSum_16s_Sfs, ((Ipp16s*) pSrc, len,
                                       (Ipp16s*) m_value, scaleFactor))
   FUNC_CALL(ippsSum_32s_Sfs, ((Ipp32s*) pSrc, len,
                                       (Ipp32s*) m_value, scaleFactor))
   FUNC_CALL(ippsSum_16sc_Sfs, ((Ipp16sc*)pSrc, len,
                                       (Ipp16sc*)m_value, scaleFactor))
   FUNC_CALL(ippsMean_32f, ((Ipp32f*) pSrc,len,(Ipp32f*) m_value,m_hint))
   FUNC_CALL(ippsMean_32fc,((Ipp32fc*)pSrc,len,(Ipp32fc*)m_value,m_hint))
   FUNC_CALL(ippsMean_64f, ((Ipp64f*) pSrc,len,(Ipp64f*) m_value))
   FUNC_CALL(ippsMean_64fc, ((Ipp64fc*) pSrc,len,(Ipp64fc*) m_value))

   FUNC_CALL(ippsMean_16s_Sfs, ((Ipp16s*) pSrc,len,
                                    (Ipp16s*) m_value,scaleFactor))
   FUNC_CALL(ippsMean_16sc_Sfs,((Ipp16sc*)pSrc,len,
                                    (Ipp16sc*)m_value,scaleFactor))
   return stsNoFunction;
}
