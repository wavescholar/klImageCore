/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunNorm.cpp: implementation of the CRunNorm class.
// CRunNorm class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunNorm.h"
#include "ParmNormDlg.h"
#include "ParmNormDiffDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunNorm::CRunNorm()
{
   hint = ippAlgHintFast;
}

CRunNorm::~CRunNorm()
{

}

static BOOL OneImage(CFunc func)
{
   return func.Found("Norm_");
}

BOOL CRunNorm::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   if (OneImage(m_Func))
      m_UsedVectors = VEC_SRC;
   else
      m_UsedVectors = VEC_SRC | VEC_SRC2;
   return TRUE;
}

void CRunNorm::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmNormDlg *pDlg = (CParmNormDlg*)parmDlg;
   if (save) {
      hint = (IppHintAlgorithm)pDlg->m_Hint;
      m_Func = pDlg->m_Func;
   } else {
      BOOL bQuality = m_Func.Found("Quality");
      pDlg->m_Hint = (int)hint;
      for (int i=0; i<4; i++)
         (pDlg->m_Value)[i] = bQuality ? m_qualityIndex[i] : m_norm[i];
   }
}

BOOL CRunNorm::CallIpp(BOOL bMessage)
{
   CParmNormDlg* pDlg;
   if (OneImage(m_Func))
      pDlg = new CParmNormDlg(this);
   else
      pDlg = new CParmNormDiffDlg(this);
   UpdateData(pDlg,FALSE);
   if (pDlg->DoModal() != IDOK) return TRUE;
   UpdateData(pDlg);
   delete pDlg;
   return TRUE;
}

void CRunNorm::SetNorm(CParmNormDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippiRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunNorm::CallIppFunction()
{

   FUNC_CALL(ippiNorm_Inf_8u_C1R  ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_8u_C3R  ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_8u_C4R ,((Ipp8u *)pSrc, srcStep,  roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_8u_AC4R ,((Ipp8u *)pSrc, srcStep,  roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16u_C1R ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16u_C3R ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16u_C4R,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16u_AC4R,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16s_C1R ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16s_C3R ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16s_C4R,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_16s_AC4R,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_32s_C1R ,((Ipp32s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_32f_C1R ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_32f_C3R ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_32f_C4R,((Ipp32f*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_Inf_32f_AC4R,((Ipp32f*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_8u_C1R   ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_8u_C3R   ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_8u_C4R  ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_8u_AC4R  ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16u_C1R  ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16u_C3R  ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16u_C4R ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16u_AC4R ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16s_C1R  ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16s_C3R  ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16s_C4R ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_16s_AC4R ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L1_32f_C1R  ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L1_32f_C3R  ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L1_32f_C4R ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L1_32f_AC4R ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L2_8u_C1R   ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_8u_C3R   ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_8u_C4R  ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_8u_AC4R  ,((Ipp8u *)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16u_C1R  ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16u_C3R  ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16u_C4R ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16u_AC4R ,((Ipp16u*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16s_C1R  ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16s_C3R  ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16s_C4R ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_16s_AC4R ,((Ipp16s*)pSrc, srcStep, roiSize, m_norm))
   FUNC_CALL(ippiNorm_L2_32f_C1R  ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L2_32f_C3R  ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L2_32f_C4R ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNorm_L2_32f_AC4R ,((Ipp32f*)pSrc, srcStep, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_Inf_8u_C1R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_8u_C3R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_8u_C4R ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_8u_AC4R ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16u_C1R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16u_C3R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16s_C1R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16s_C3R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_32f_C1R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_32f_C3R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_Inf_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_8u_C1R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_8u_C3R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_8u_C4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_8u_AC4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16u_C1R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16u_C3R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16u_C4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16u_AC4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16s_C1R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16s_C3R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16s_C4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_16s_AC4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L1_32f_C1R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L1_32f_C3R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L1_32f_C4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L1_32f_AC4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L2_8u_C1R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_8u_C3R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_8u_C4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_8u_AC4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16u_C1R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16u_C3R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16u_C4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16u_AC4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16s_C1R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16s_C3R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16s_C4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_16s_AC4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormDiff_L2_32f_C1R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L2_32f_C3R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L2_32f_C4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormDiff_L2_32f_AC4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_Inf_8u_C1R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_8u_C3R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_8u_C4R ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_8u_AC4R ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16u_C1R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16u_C3R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16s_C1R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16s_C3R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_32f_C1R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_32f_C3R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_Inf_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_8u_C1R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_8u_C3R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_8u_C4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_8u_AC4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16u_C1R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16u_C3R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16u_C4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16u_AC4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16s_C1R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16s_C3R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16s_C4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_16s_AC4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L1_32f_C1R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L1_32f_C3R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L1_32f_C4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L1_32f_AC4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L2_8u_C1R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_8u_C3R   ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_8u_C4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_8u_AC4R  ,((Ipp8u *)pSrc, srcStep, (Ipp8u *)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16u_C1R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16u_C3R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16u_C4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16u_AC4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16s_C1R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16s_C3R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16s_C4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_16s_AC4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_norm))
   FUNC_CALL(ippiNormRel_L2_32f_C1R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L2_32f_C3R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L2_32f_C4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiNormRel_L2_32f_AC4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, m_norm, hint))
   FUNC_CALL(ippiQualityIndex_8u32f_C1R,((Ipp8u*)pSrc, srcStep,(Ipp8u*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_8u32f_C3R,((Ipp8u*)pSrc, srcStep,(Ipp8u*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_8u32f_AC4R,((Ipp8u*)pSrc, srcStep,(Ipp8u*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_16u32f_C1R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_16u32f_C3R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_16u32f_AC4R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_32f_C1R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_32f_C3R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))
   FUNC_CALL(ippiQualityIndex_32f_AC4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2, roiSize,  m_qualityIndex ))

   return stsNoFunction;
}

CString CRunNorm::GetHistoryParms()
{
   CMyString parms;
   if (m_pDocSrc->GetImage()->Float() && !m_Func.Found("Inf") && !m_Func.Found("Quality"))
      parms << hint;
   return parms;
}

