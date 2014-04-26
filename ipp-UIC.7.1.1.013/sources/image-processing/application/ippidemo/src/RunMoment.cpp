/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMoment.cpp: implementation of the CRunMoment class.
// CRunMoment class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunMoment.h"
#include "ParmMomentDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunMoment::CRunMoment()
{
   m_pState = NULL;
   m_hint = ippAlgHintFast;
   m_Channel = 0;
   m_GetFunc = "ippiGetSpatialMoment";
   m_valNum = 0;
}

CRunMoment::~CRunMoment()
{

}

BOOL CRunMoment::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;

   m_bFloat = func.Found("Moments64f");
   CString typeStr = m_bFloat ? "_64f" : "_64s";
   m_InitName = "ippiMomentInitAlloc" + typeStr;
   m_FreeName = "ippiMomentFree" + typeStr;
   m_pState = NULL;
   return TRUE;
}

void CRunMoment::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmMomentDlg *pDlg = (CParmMomentDlg*)parmDlg;
   if (save) {
      m_GetFunc = pDlg->m_GetFunc;
      m_Channel = pDlg->m_Channel;
      m_hint    = (IppHintAlgorithm)pDlg->m_Hint   ;
   } else {
      pDlg->m_GetFunc = m_GetFunc;
      pDlg->m_Channel = m_Channel;
      pDlg->m_Hint    = (int)m_hint   ;
      for (int i=0; i<VAL_NUM; i++) {
         if (i < m_valNum) {
            if (m_bFloat)
               pDlg->m_MomentStr[i].Format("%.10g",m_value64f[i]);
            else
               pDlg->m_MomentStr[i].Format("%d",m_value64s[i]);
         } else {
            pDlg->m_MomentStr[i] = "";
         }
      }
   }
}

BOOL CRunMoment::CallIpp(BOOL bMessage)
{
   CParmMomentDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   CallFree();
   return TRUE;
}

void CRunMoment::SetMoments(CParmMomentDlg* pDlg)
{
   IppHintAlgorithm hintPrev = m_hint;
   UpdateData(pDlg);
   if (!m_pState || hintPrev != m_hint) {
      IppStatus status;
      status = CallFree();
      IppErrorMessage(m_FreeName,status);
      if (status < 0) return;
      status = CallInit();
      IppErrorMessage(m_InitName,status);
      if (status < 0) return;
      if (CippiRun::CallIpp() < 0) return;
      Timing();
      SetHistory();
   }
   IppStatus status = GetMoments();
   IppErrorMessage(m_GetFunc,status);
   UpdateData(pDlg,FALSE);
}

IppStatus CRunMoment::GetMoments()
{
   IppStatus status;
   if (m_GetFunc.Found("Hu")) {
      m_valNum = 7;
      if (m_bFloat)
         status = CallGet(0,0,m_value64f);
      else
         status = CallGet(0,0,m_value64s);
      if (status < 0) m_valNum = 0;
      return status;
   }
   if (m_bFloat) {
      m_valNum = 0;
      for (int i=0; i<4; i++) {
         for (int j=0; j<4-i; j++) {
            status = CallGet(i,j,m_value64f + m_valNum);
            if (status != ippStsNoErr) return status;
            m_valNum++;
         }
      }
   } else {
      m_valNum = 0;
      for (int i=0; i<4; i++) {
         for (int j=0; j<4-i; j++) {
            status = CallGet(i,j,m_value64s + m_valNum);
            if (status != ippStsNoErr) return status;
            m_valNum++;
         }
      }
   }
   return status;
}

IppStatus CRunMoment::CallInit()
{
   MATE_CALL(m_InitName, ippiMomentInitAlloc_64f, ((IppiMomentState_64f**)&m_pState, m_hint))
   MATE_CALL(m_InitName, ippiMomentInitAlloc_64s, ((IppiMomentState_64s**)&m_pState, m_hint))
   return stsNoFunction;
}

IppStatus CRunMoment::CallFree()
{
   if (!m_pState) return ippStsNoErr;
   MATE_CALL(m_FreeName, ippiMomentFree_64f, ((IppiMomentState_64f*)m_pState))
   MATE_CALL(m_FreeName, ippiMomentFree_64s, ((IppiMomentState_64s*)m_pState))
   return stsNoFunction;
}

IppStatus CRunMoment::CallIppFunction()
{
   FUNC_CALL(ippiMoments64f_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_16u_AC4R,((Ipp16u*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_8u_AC4R,((Ipp8u*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64f_32f_AC4R,((Ipp32f*)pSrc, srcStep, roiSize, (IppiMomentState_64f*)m_pState))
   FUNC_CALL(ippiMoments64s_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, (IppiMomentState_64s*)m_pState))
   FUNC_CALL(ippiMoments64s_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, (IppiMomentState_64s*)m_pState))
   FUNC_CALL(ippiMoments64s_16u_AC4R,((Ipp16u*)pSrc, srcStep, roiSize, (IppiMomentState_64s*)m_pState))
   FUNC_CALL(ippiMoments64s_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, (IppiMomentState_64s*)m_pState))
   FUNC_CALL(ippiMoments64s_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, (IppiMomentState_64s*)m_pState))
   FUNC_CALL(ippiMoments64s_8u_AC4R,((Ipp8u*)pSrc, srcStep, roiSize, (IppiMomentState_64s*)m_pState))
   return stsNoFunction;
}

IppStatus CRunMoment::CallGet(int mOrd, int nOrd, void* pValue)
{
   IppiPoint roiOffset = {m_srcROI.x, m_srcROI.y};

   MATE_CALL(m_GetFunc, ippiGetSpatialMoment_64f,((IppiMomentState_64f*)m_pState,
                                       mOrd, nOrd, m_Channel,
                                       roiOffset, (Ipp64f*)pValue))
   MATE_CALL(m_GetFunc, ippiGetCentralMoment_64f,((IppiMomentState_64f*)m_pState,
                                       mOrd, nOrd, m_Channel,
                                       (Ipp64f*)pValue))
   MATE_CALL(m_GetFunc, ippiGetSpatialMoment_64s,((IppiMomentState_64s*)m_pState,
                                       mOrd, nOrd, m_Channel,
                                       roiOffset, (Ipp64s*)pValue, scaleFactor))
   MATE_CALL(m_GetFunc, ippiGetCentralMoment_64s,((IppiMomentState_64s*)m_pState,
                                       mOrd, nOrd, m_Channel,
                                       (Ipp64s*)pValue, scaleFactor))
   MATE_CALL(m_GetFunc, ippiGetNormalizedSpatialMoment_64f,((IppiMomentState_64f*)m_pState,
                                   mOrd, nOrd, m_Channel,
                                   roiOffset, (Ipp64f*)pValue))
   MATE_CALL(m_GetFunc, ippiGetNormalizedCentralMoment_64f,((IppiMomentState_64f*)m_pState,
                                   mOrd, nOrd, m_Channel,
                                   (Ipp64f*)pValue))
   MATE_CALL(m_GetFunc, ippiGetNormalizedSpatialMoment_64s,((IppiMomentState_64s*)m_pState,
                                   mOrd, nOrd, m_Channel,
                                   roiOffset, (Ipp64s*)pValue, scaleFactor))
   MATE_CALL(m_GetFunc, ippiGetNormalizedCentralMoment_64s,((IppiMomentState_64s*)m_pState,
                                   mOrd, nOrd, m_Channel,
                                   (Ipp64s*)pValue, scaleFactor))
   MATE_CALL(m_GetFunc, ippiGetHuMoments_64f,((IppiMomentState_64f*)m_pState,
                                   m_Channel, (Ipp64f*)pValue))
   MATE_CALL(m_GetFunc, ippiGetHuMoments_64s,((IppiMomentState_64s*)m_pState,
                                   m_Channel, (Ipp64s*)pValue, scaleFactor))

   return stsNoFunction;
}

CString CRunMoment::GetHistoryParms()
{
   CMyString parms;
   return parms << m_hint << ",..";
}
