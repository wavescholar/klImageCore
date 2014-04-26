/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHistoRange.cpp: implementation of the CRunHistoRange class.
// CRunHistoRange class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunHistoRange.h"
#include "ParmHistoRangeDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunHistoRange::CRunHistoRange()
{

}

CRunHistoRange::~CRunHistoRange()
{

}

void CRunHistoRange::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmHistoRangeDlg *pDlg = (CParmHistoRangeDlg*)parmDlg;
   if (save) {
   } else {
      pDlg->m_pLUT = &m_LUT;
   }
}

BOOL CRunHistoRange::CallIpp(BOOL bMessage)
{
   UpdateLUT(FALSE);
   CParmHistoRangeDlg dlg(this);
   UpdateData(&dlg, FALSE);
   if (dlg.DoModal() != IDOK)
      return FALSE;
   ShowHistogram();
   UpdateLUT();
   return TRUE;
}

BOOL CRunHistoRange::SetValues(CParamDlg* pDlg, BOOL bMessage)
{
   UpdateData(pDlg);

   if (!CippiRun::CallIpp(bMessage))
      return FALSE;

   UpdateData(pDlg, FALSE);
   return TRUE;
}

IppStatus CRunHistoRange::CallIppFunction()
{
   FUNC_CALL(ippiHistogramRange_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize,  (Ipp32s* )m_LUT.m_pHist[0], (const Ipp32s*) m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiHistogramRange_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_8u_AC4R,((Ipp8u*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize,  (Ipp32s* )m_LUT.m_pHist[0], (const Ipp32s*) m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiHistogramRange_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_16u_AC4R,((Ipp16u*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize,  (Ipp32s* )m_LUT.m_pHist[0], (const Ipp32s*) m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiHistogramRange_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_16s_AC4R,((Ipp16s*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize,  (Ipp32s* )m_LUT.m_pHist[0], (const Ipp32f*) m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiHistogramRange_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_32f_AC4R,((Ipp32f*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiHistogramRange_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize,  (Ipp32s**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   return stsNoFunction;
}

CString CRunHistoRange::GetHistoryParms()
{
   CMyString parms;
   parms << ".., "
         << m_LUT.LevelString() << ", "
         << m_LUT.LevelNumString();
   return parms;
}

