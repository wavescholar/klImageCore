/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStat.cpp: implementation of the CRunStat class.
// CRunStat class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
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
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   m_value2.Init(func.SrcType(), 4);
   return TRUE;
}

void CRunStat::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmStatDlg *pDlg = (CParmStatDlg*)parmDlg;
   if (save) {
      m_hint = (IppHintAlgorithm)pDlg->m_Hint;
      m_Func = pDlg->m_Func;
   } else {
      pDlg->m_Hint = (int)m_hint;
      m_value.Get(pDlg->m_valueStr);
      m_value2.Get(pDlg->m_value2Str);
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
   if (m_Func.Found("Mean") || m_Func.Found("Sum"))
      m_value.Init(pp64f, 4);
   else
      m_value.Init(m_Func.SrcType(),4);
   PrepareParameters();
   if (CippiRun::CallIpp() < 0) return;
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunStat::CallIppFunction()
{
   FUNC_CALL(ippiSum_8u_C1R, ((Ipp8u*)pSrc, srcStep,   roiSize, m_value))
   FUNC_CALL(ippiSum_8u_C3R, ((Ipp8u*)pSrc, srcStep,   roiSize, m_value))
   FUNC_CALL(ippiSum_8u_C4R, ((Ipp8u*)pSrc, srcStep,  roiSize, m_value))
   FUNC_CALL(ippiSum_8u_AC4R, ((Ipp8u*)pSrc, srcStep,  roiSize, m_value))
   FUNC_CALL(ippiSum_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiSum_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiSum_16u_C4R, ((Ipp16u*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiSum_16u_AC4R, ((Ipp16u*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiSum_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiSum_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiSum_16s_C4R, ((Ipp16s*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiSum_16s_AC4R, ((Ipp16s*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiSum_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_hint))
   FUNC_CALL(ippiSum_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_hint))
   FUNC_CALL(ippiSum_32f_C4R, ((Ipp32f*)pSrc, srcStep,roiSize, m_value, m_hint))
   FUNC_CALL(ippiSum_32f_AC4R, ((Ipp32f*)pSrc, srcStep,roiSize, m_value, m_hint))
   FUNC_CALL(ippiMean_8u_C1R, ((Ipp8u*)pSrc, srcStep,  roiSize, m_value))
   FUNC_CALL(ippiMean_8u_C3R, ((Ipp8u*)pSrc, srcStep,  roiSize, m_value))
   FUNC_CALL(ippiMean_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMean_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMean_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMean_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMean_16u_C4R, ((Ipp16u*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiMean_16u_AC4R, ((Ipp16u*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiMean_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMean_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMean_16s_C4R, ((Ipp16s*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiMean_16s_AC4R, ((Ipp16s*)pSrc, srcStep,roiSize, m_value))
   FUNC_CALL(ippiMean_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_hint))
   FUNC_CALL(ippiMean_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_hint))
   FUNC_CALL(ippiMean_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize,m_value, m_hint))
   FUNC_CALL(ippiMean_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize,m_value, m_hint))

   FUNC_CALL(ippiMin_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16u_AC4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_16s_AC4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMin_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16u_AC4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_16s_AC4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))
   FUNC_CALL(ippiMax_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value))

   FUNC_CALL(ippiMinMax_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16u_AC4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_16s_AC4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_value2))
   FUNC_CALL(ippiMinMax_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_value2))

   return stsNoFunction;
}
