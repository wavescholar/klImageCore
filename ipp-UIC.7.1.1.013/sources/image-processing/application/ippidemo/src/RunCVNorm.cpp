/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVNorm.cpp: implementation of the CRunCVNorm class.
// CRunCVNorm class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippiDemoDoc.h"
#include "RunCVNorm.h"
#include "ParmCVNormDlg.h"
#include "ParmCVNormDiffDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVNorm::CRunCVNorm()
{
   m_coi = 1;
}

CRunCVNorm::~CRunCVNorm()
{

}
BOOL CRunCVNorm::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   if (func.Mask())
      m_UsedVectors |= VEC_MASK;
   if (func.Found("Rel") || func.Found("Diff"))
      m_UsedVectors |= VEC_SRC2;
   return TRUE;
}


void CRunCVNorm::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmCVNormDlg *pDlg = (CParmCVNormDlg*)parmDlg;
   if (save) {
      m_coi = atoi(pDlg->m_CoiStr);
      m_Func = pDlg->m_Func;
   } else {
      pDlg->m_CoiStr.Format("%d", m_coi);
      pDlg->m_ValueStr.Format("%.5g", m_value);
   }
}

BOOL CRunCVNorm::CallIpp(BOOL bMessage)
{
   CParmCVNormDlg* pDlg;
   if (m_UsedVectors & VEC_SRC2)
      pDlg = new CParmCVNormDiffDlg(this);
   else
      pDlg = new CParmCVNormDlg(this);

   UpdateData(pDlg,FALSE);
   if (pDlg->DoModal() != IDOK) return TRUE;
   UpdateData(pDlg);

   delete pDlg;
   return TRUE;
}

void CRunCVNorm::SetValue(CParmCVNormDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   if (CippiRun::CallIpp() < 0) return;
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunCVNorm::CallIppFunction()
{
   FUNC_CV_CALL(  ippiNorm_Inf_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_Inf_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )


   FUNC_CV_CALL(  ippiNormDiff_Inf_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_Inf_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_Inf_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_Inf_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_Inf_8u_C3CMR, ((const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_Inf_8s_C3CMR, ((const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_Inf_16u_C3CMR, ((const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_Inf_32f_C3CMR, ((const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))


   FUNC_CV_CALL(  ippiNormRel_Inf_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_Inf_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_Inf_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_Inf_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_Inf_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_Inf_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_Inf_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_Inf_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))


   FUNC_CV_CALL(  ippiNorm_L1_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L1_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )


   FUNC_CV_CALL(  ippiNormDiff_L1_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L1_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L1_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L1_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L1_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_L1_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_L1_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_L1_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))


   FUNC_CV_CALL(  ippiNormRel_L1_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L1_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L1_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L1_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L1_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_L1_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_L1_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_L1_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))


   FUNC_CV_CALL(  ippiNorm_L2_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp8u*)pMask,maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNorm_L2_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ) )


   FUNC_CV_CALL(  ippiNormDiff_L2_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L2_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L2_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L2_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormDiff_L2_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_L2_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_L2_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormDiff_L2_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))


   FUNC_CV_CALL(  ippiNormRel_L2_8u_C1MR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L2_8s_C1MR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L2_16u_C1MR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L2_32f_C1MR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, (Ipp64f*)(&m_value) ) )

   FUNC_CV_CALL(  ippiNormRel_L2_8u_C3CMR, ( (const Ipp8u*)pSrc, srcStep,
 (const Ipp8u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_L2_8s_C3CMR, ( (const Ipp8s*)pSrc, srcStep,
 (const Ipp8s*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_L2_16u_C3CMR, ( (const Ipp16u*)pSrc, srcStep,
 (const Ipp16u*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   FUNC_CV_CALL(  ippiNormRel_L2_32f_C3CMR, ( (const Ipp32f*)pSrc, srcStep,
 (const Ipp32f*)pSrc2, srcStep2,
 (const Ipp8u*)pMask, maskStep,
 roiSize, m_coi, (Ipp64f*)(&m_value) ))

   return stsNoFunction;
}
