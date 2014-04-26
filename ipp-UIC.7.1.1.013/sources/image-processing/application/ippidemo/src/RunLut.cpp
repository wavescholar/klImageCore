/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunLut.cpp: implementation of the CRunLut class.
// CRunLut class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunLut.h"
#include "ParmLutDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunLut::CRunLut()
{
   for (int i=0; i<4; i++) {
      m_A[i] = -1;
      m_F[i] = -1;
   }
   m_LUT_8u .Init32s(4, 257, 0, UCHAR_MAX + 1);
   m_LUT_16s.Init32s(4, 257, SHRT_MIN, SHRT_MAX + 1);
   m_LUT_32f.Init32f(4, 257, 0, 1);


}

CRunLut::~CRunLut()
{

}

CParamDlg* CRunLut::CreateDlg() { return new CParmLutDlg;}

void CRunLut::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmLutDlg* pDlg = (CParmLutDlg*)parmDlg;
    UpdateLUT(save);
    if (save) {
       pDlg->GetA(m_A);
       pDlg->GetF(m_F);
    } else {
       pDlg->m_pLUT = &m_LUT;
       pDlg->SetA(m_A);
       pDlg->SetF(m_F);
    }
}

void CRunLut::UpdateLUT(BOOL bSave)
{
   if (bSave) {
      switch (m_Func.SrcType()) {
      case pp8u:  m_LUT_8u  = m_LUT; break;
      case pp16s: m_LUT_16s = m_LUT; break;
      case pp32f: m_LUT_32f = m_LUT; break;
      }
   } else {
      int num = m_Func.SrcChannels() - m_Func.SrcAlpha();
      switch (m_Func.SrcType()) {
      case pp8u : m_LUT_8u .m_num = num; m_LUT = m_LUT_8u ; break;
      case pp16s: m_LUT_16s.m_num = num; m_LUT = m_LUT_16s; break;
      case pp32f: m_LUT_32f.m_num = num; m_LUT = m_LUT_32f; break;
      }
   }
}

BOOL CRunLut::BeforeCall()
{
   m_LUT.CreateTone(m_F, m_A, m_Func.Found("Linear") || m_Func.Found("Cubic"));
   return TRUE;
}

IppStatus CRunLut::CallIppFunction()
{
   FUNC_CALL(ippiLUT_8u_C1R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_8u_C3R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_8u_AC4R, ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_8u_C4R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_32f_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_LUT.m_pHist[0], (Ipp32f*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_32f_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_32f_C4R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))

   FUNC_CALL(ippiLUT_Linear_8u_C1R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Linear_8u_C3R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_8u_AC4R, ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_8u_C4R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Linear_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Linear_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_32f_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_LUT.m_pHist[0], (Ipp32f*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Linear_32f_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Linear_32f_C4R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))

   FUNC_CALL(ippiLUT_Cubic_8u_C1R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Cubic_8u_C3R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_8u_AC4R, ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_8u_C4R,  ((Ipp8u *)pSrc, srcStep, (Ipp8u *)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Cubic_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_LUT.m_pHist[0], (Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Cubic_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_32f_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_LUT.m_pHist[0], (Ipp32f*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL(ippiLUT_Cubic_32f_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL(ippiLUT_Cubic_32f_C4R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))

   FUNC_CALL( ippiLUT_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_8u_C4IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f*)m_LUT.m_pHist[0], (const Ipp32f*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))

   FUNC_CALL( ippiLUT_Linear_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Linear_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_8u_C4IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Linear_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Linear_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f*)m_LUT.m_pHist[0], (const Ipp32f*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Linear_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Linear_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))

   FUNC_CALL( ippiLUT_Cubic_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Cubic_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_8u_C4IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Cubic_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s*)m_LUT.m_pHist[0], (const Ipp32s*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Cubic_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (const Ipp32s**)m_LUT.m_pHist, (const Ipp32s**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f*)m_LUT.m_pHist[0], (const Ipp32f*)m_LUT.m_pLevels[0], m_LUT.m_nLevels[0]))
   FUNC_CALL( ippiLUT_Cubic_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))
   FUNC_CALL( ippiLUT_Cubic_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (const Ipp32f**)m_LUT.m_pHist, (const Ipp32f**)m_LUT.m_pLevels, m_LUT.m_nLevels))

return stsNoFunction;
}

CString CRunLut::GetHistoryParms()
{
   CMyString parms;
   parms << ".., "
         << m_LUT.LevelString() << ", "
         << m_LUT.LevelNumString();
   return parms;
}

CString CRunLut::GetHistoInfo()
{
   CMyString info;
   return info;
}

