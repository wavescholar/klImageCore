/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVStat.cpp: implementation of the CRunCVStat class.
// CRunCVStat class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunCVStat.h"
#include "ParmCVStatDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVStat::CRunCVStat()
{
   m_coi = 1;
}

CRunCVStat::~CRunCVStat()
{

}

BOOL CRunCVStat::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   if (func.Mask())
      m_UsedVectors |= VEC_MASK;
   return TRUE;
}


void CRunCVStat::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmCVStatDlg *pDlg = (CParmCVStatDlg*)parmDlg;
   if (save) {
      m_coi = atoi(pDlg->m_coiStr);
      m_Func = GetFuncName(pDlg->m_funcIndex);
   } else {
      pDlg->m_funcIndex = GetFuncIndex();
      pDlg->m_coiStr.Format("%d", m_coi);
      for (int i=0; i<2; i++) {
         if (m_Func.Found("MinMax"))
            pDlg->m_valStr[i].Format("%.4g", m_pVal32f[i]);
         else
            pDlg->m_valStr[i].Format("%.4g", m_pVal64f[i]);
         pDlg->m_pIndex[i] = m_pIndex[i];
      }
   }
}

static CString getBaseName(int index)
{
   switch(index) {
   case 0: return "Mean";
   case 1: return "Mean_StdDev";
   case 2: return "MinMaxIndx";
   }
   return "";
}

CFunc CRunCVStat::GetFuncName(int index)
{
   return "ippi" + getBaseName(index)
      + "_" + m_Func.TypeName()
      + "_" + m_Func.DescrName();
}

int CRunCVStat::GetFuncIndex()
{
   CString baseName = m_Func.BaseName();
   if (baseName == "Mean") return 0;
   if (baseName == "Mean_StdDev") return 1;
   if (baseName == "MinMaxIndx") return 2;
   return -1;
}

BOOL CRunCVStat::CallIpp(BOOL bMessage)
{
   CParmCVStatDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunCVStat::SetValues(CParmCVStatDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   if (CippiRun::CallIpp() < 0) return;
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunCVStat::CallIppFunction()
{
   FUNC_CV_CALL(ippiMean_8u_C1MR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_8u_C3CMR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_8s_C1MR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_8s_C3CMR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_16u_C1MR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_16u_C3CMR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_32f_C1MR,((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f ))
   FUNC_CV_CALL(ippiMean_32f_C3CMR,((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f ))

   FUNC_CV_CALL(ippiMean_StdDev_8u_C1R,((Ipp8u*)pSrc, srcStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_8u_C3CR,((Ipp8u*)pSrc, srcStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_8s_C1R,((Ipp8s*)pSrc, srcStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_8s_C3CR,((Ipp8s*)pSrc, srcStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_16u_C1R,((Ipp16u*)pSrc, srcStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_16u_C3CR,((Ipp16u*)pSrc, srcStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_32f_C1R,((Ipp32f*)pSrc, srcStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_32f_C3CR,((Ipp32f*)pSrc, srcStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))

   FUNC_CV_CALL(ippiMean_StdDev_8u_C1MR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_8u_C3CMR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_8s_C1MR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_8s_C3CMR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_16u_C1MR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_16u_C3CMR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_32f_C1MR,((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal64f, m_pVal64f + 1 ))
   FUNC_CV_CALL(ippiMean_StdDev_32f_C3CMR,((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal64f, m_pVal64f + 1 ))

   FUNC_CV_CALL(ippiMinMaxIndx_8u_C1R,((Ipp8u*)pSrc, srcStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_8u_C3CR,((Ipp8u*)pSrc, srcStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_8s_C1R,((Ipp8s*)pSrc, srcStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_8s_C3CR,((Ipp8s*)pSrc, srcStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_16u_C1R,((Ipp16u*)pSrc, srcStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_16u_C3CR,((Ipp16u*)pSrc, srcStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_32f_C1R,((Ipp32f*)pSrc, srcStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_32f_C3CR,((Ipp32f*)pSrc, srcStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))

   FUNC_CV_CALL(ippiMinMaxIndx_8u_C1MR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_8u_C3CMR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_8s_C1MR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_8s_C3CMR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_16u_C1MR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_16u_C3CMR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_32f_C1MR,((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))
   FUNC_CV_CALL(ippiMinMaxIndx_32f_C3CMR,((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, roiSize, m_coi, m_pVal32f, m_pVal32f + 1, m_pIndex, m_pIndex + 1))

   return stsNoFunction;
}
