/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRand.cpp : implementation of the CRunRand class.
// CRunRand class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Histo.h"
#include "RunRand.h"
#include "ParmRandDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRand::CRunRand()
{
   m_seed = 0;

   m_low .Init(pp8u,0);
   m_high.Init(pp8u,128);
   m_low .Init(pp16s,SHRT_MIN>>3);
   m_high.Init(pp16s,SHRT_MAX>>3);
   m_low .Init(pp32f,-1.);
   m_high.Init(pp32f, 1.);
   m_low .Init(pp64f,-1.);
   m_high.Init(pp64f, 1.);

   m_mean .Init(pp8u,64);
   m_stdDev.Init(pp8u,32);
   m_mean .Init(pp16s,0);
   m_stdDev.Init(pp16s,SHRT_MAX>>4);
   m_mean .Init(pp32f,0.);
   m_stdDev.Init(pp32f,1.);
   m_mean .Init(pp64f,0.);
   m_stdDev.Init(pp64f,1.);

   m_seedSave = 0;
   m_bGauss  = FALSE;
   m_bDirect = FALSE;
}

BOOL CRunRand::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;

   m_bGauss = func.Found("Gauss");
   m_bDirect = func.Found("Direct");

   m_low.Init(func.SrcType());
   m_high.Init(func.SrcType());
   m_mean.Init(func.SrcType());
   m_stdDev.Init(func.SrcType());

   CString startName = m_Func.Prefix() + m_Func.BaseName();
   CString finishName = "_" + m_Func.TypeToString(m_Func.DstType());
   m_InitName = startName + "InitAlloc" + finishName;
   m_FreeName = startName + "Free" + finishName;
   return TRUE;
}

CParamDlg* CRunRand::CreateDlg() { 
   return new CParmRandDlg;
}

void CRunRand::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmRandDlg *pDlg = (CParmRandDlg*)parmDlg;
   if (save) {
      m_seed = pDlg->m_seed;
      if (m_bGauss) {
         m_mean .Set(pDlg->m_ParmStr[0]);
         m_stdDev.Set(pDlg->m_ParmStr[1]);
      } else {
         m_low .Set(pDlg->m_ParmStr[0]);
         m_high.Set(pDlg->m_ParmStr[1]);
      }
   } else {
      pDlg->m_seed = m_seed;
      if (m_bGauss) {
         pDlg->m_ParmName[0] = "Mean ";
         pDlg->m_ParmName[1] = "Stdev";
         m_mean .Get(pDlg->m_ParmStr[0]);
         m_stdDev.Get(pDlg->m_ParmStr[1]);
      } else {
         pDlg->m_ParmName[0] = "Low ";
         pDlg->m_ParmName[1] = "High";
         m_low .Get(pDlg->m_ParmStr[0]);
         m_high.Get(pDlg->m_ParmStr[1]);
      }
   }
}

BOOL CRunRand::BeforeCall()
{
   m_seedSave = m_seed;
   if (!m_bDirect) {
      IppStatus status = CallInit();
      IppErrorMessage(m_InitName, status);
      if (status < 0) return FALSE;
   }
   return TRUE;
}

BOOL CRunRand::AfterCall(BOOL bOk)
{
   if (!m_bDirect) {
      IppStatus status = CallFree();
      IppErrorMessage(m_InitName, status);
      if (status < 0) return FALSE;
   }
   return TRUE;
}

IppStatus CRunRand::CallInit()
{
   MATE_CALL(m_InitName, ippsRandUniformInitAlloc_8u, ((IppsRandUniState_8u**)&m_pState, (Ipp8u)m_low, (Ipp8u)m_high, m_seed))
   MATE_CALL(m_InitName, ippsRandUniformInitAlloc_16s, ((IppsRandUniState_16s**)&m_pState, (Ipp16s)m_low, (Ipp16s)m_high, m_seed))
   MATE_CALL(m_InitName, ippsRandUniformInitAlloc_32f, ((IppsRandUniState_32f**)&m_pState, (Ipp32f)m_low, (Ipp32f)m_high, m_seed))
   MATE_CALL(m_InitName, ippsRandGaussInitAlloc_8u, ((IppsRandGaussState_8u**)&m_pState, (Ipp8u)m_mean, (Ipp8u)m_stdDev, m_seed))
   MATE_CALL(m_InitName, ippsRandGaussInitAlloc_16s, ((IppsRandGaussState_16s**)&m_pState, (Ipp16s)m_mean, (Ipp16s)m_stdDev, m_seed))
   MATE_CALL(m_InitName, ippsRandGaussInitAlloc_32f, ((IppsRandGaussState_32f**)&m_pState, (Ipp32f)m_mean, (Ipp32f)m_stdDev, m_seed))

   return stsNoFunction;
}

IppStatus CRunRand::CallFree()
{
   MATE_CALL(m_FreeName, ippsRandUniformFree_8u, ((IppsRandUniState_8u*)m_pState))
   MATE_CALL(m_FreeName, ippsRandUniformFree_16s, ((IppsRandUniState_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsRandUniformFree_32f, ((IppsRandUniState_32f*)m_pState))
   MATE_CALL(m_FreeName, ippsRandGaussFree_8u, ((IppsRandGaussState_8u*)m_pState))
   MATE_CALL(m_FreeName, ippsRandGaussFree_16s, ((IppsRandGaussState_16s*)m_pState))
   MATE_CALL(m_FreeName, ippsRandGaussFree_32f, ((IppsRandGaussState_32f*)m_pState))

   return stsNoFunction;
}

IppStatus CRunRand::CallIppFunction()
{
   FUNC_CALL(ippsRandUniform_Direct_16s, ((Ipp16s*)pSrc, len, (Ipp16s)m_low, (Ipp16s)m_high, &m_seed))
   FUNC_CALL(ippsRandUniform_Direct_32f, ((Ipp32f*)pSrc, len, (Ipp32f)m_low, (Ipp32f)m_high, &m_seed))
   FUNC_CALL(ippsRandUniform_Direct_64f, ((Ipp64f*)pSrc, len, (Ipp64f)m_low, (Ipp64f)m_high, &m_seed))
   FUNC_CALL(ippsRandGauss_Direct_16s, ((Ipp16s*)pSrc, len, (Ipp16s)m_mean, (Ipp16s)m_stdDev, &m_seed))
   FUNC_CALL(ippsRandGauss_Direct_32f, ((Ipp32f*)pSrc, len, (Ipp32f)m_mean, (Ipp32f)m_stdDev, &m_seed))
   FUNC_CALL(ippsRandGauss_Direct_64f, ((Ipp64f*)pSrc, len, (Ipp64f)m_mean, (Ipp64f)m_stdDev, &m_seed))
   FUNC_CALL(ippsRandUniform_8u, ((Ipp8u*)pSrc, len, (IppsRandUniState_8u*)m_pState))
   FUNC_CALL(ippsRandUniform_16s, ((Ipp16s*)pSrc, len, (IppsRandUniState_16s*)m_pState))
   FUNC_CALL(ippsRandUniform_32f, ((Ipp32f*)pSrc, len, (IppsRandUniState_32f*)m_pState))
   FUNC_CALL(ippsRandGauss_8u, ((Ipp8u*)pSrc, len, (IppsRandGaussState_8u*)m_pState))
   FUNC_CALL(ippsRandGauss_16s, ((Ipp16s*)pSrc, len, (IppsRandGaussState_16s*)m_pState))
   FUNC_CALL(ippsRandGauss_32f, ((Ipp32f*)pSrc, len, (IppsRandGaussState_32f*)m_pState))

   return stsNoFunction;
}

CString CRunRand::GetHistoryParms()
{
   if (!m_bDirect) return "";
   CMyString parms;
   if (m_bGauss)
      return parms << m_mean.String() << ", " 
                   << m_stdDev.String() << ", " << m_seedSave;
   else
      return parms << m_low.String() << ", " 
                   << m_high.String() << ", " << m_seedSave;
}

void CRunRand::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   if (!m_bDirect) {
      m_bDirect = TRUE;
      pHisto->AddFuncString(m_InitName, GetHistoryParms());
      m_bDirect = FALSE;
   }
   CippsRun::AddHistoFunc(pHisto,vecPos);
   if (m_bDirect) {
      CMyString parms;
      parms << "\tseed = " << m_seed;
      pHisto->AddTail(parms);
   }
}
