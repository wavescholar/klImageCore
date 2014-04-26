/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMorph.cpp: implementation of the CRunMorph class.
// CRunMorph class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunMorph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunMorph::CRunMorph()
{
   m_maskSize.width = 3;
   m_maskSize.height = 3;
   m_anchor.x = 1;
   m_anchor.y = 1;
   m_AnchorCustom = 0;
   for (int i=0; i<MAX_SIZE; i++)  m_mask[i] = 1;
}

CRunMorph::~CRunMorph()
{

}

void CRunMorph::UpdateData(CParamDlg* pDlg, BOOL save)
{
    CippiRun::UpdateData(pDlg,save);
   CParmMorphDlg* pMorphDlg = (CParmMorphDlg*)pDlg;
   if (save) {
      m_maskSize.width  = pMorphDlg->m_W;
      m_maskSize.height = pMorphDlg->m_H;
      m_anchor.x = pMorphDlg->m_X;
      m_anchor.y = pMorphDlg->m_Y;
      m_AnchorCustom = pMorphDlg->m_AnchorCustom;
      for (int i=0; i<m_maskSize.width*m_maskSize.height; i++)
         m_mask[i] = (char)(pMorphDlg->m_Kernel[i]);
   } else {
      pMorphDlg->m_W = m_maskSize.width ;
      pMorphDlg->m_H = m_maskSize.height;
      pMorphDlg->m_X = m_anchor.x;
      pMorphDlg->m_Y = m_anchor.y;
      pMorphDlg->m_AnchorCustom = m_AnchorCustom;
      for (int i=0; i<m_maskSize.width*m_maskSize.height; i++) {
         pMorphDlg->m_Kernel[i] = m_mask[i] ? 1 : 0;
      }
   }
}

int CRunMorph::GetSrcBorder(int idx)
{
    switch (idx)
    {
    case LEFT: 
        return m_anchor.x;
    case RIGHT:
        return m_maskSize.width - m_anchor.x - 1;
    case TOP: 
        return m_anchor.y;
    case BOTTOM:
       return m_maskSize.height - m_anchor.y - 1;
    }
    return 0;
}


IppStatus CRunMorph::CallIppFunction()
{
   FUNC_CALL(ippiDilate_16u_C1R   ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_16u_C3R   ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_16u_C4R   ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_16u_AC4R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_C1R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_C3R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_C4R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_AC4R  ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_C1R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_C3R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_C4R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_AC4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_16u_C1IR  ,((Ipp16u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_16u_C3IR  ,((Ipp16u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_16u_AC4IR ,((Ipp16u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_C1IR  ,((Ipp8u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_C3IR  ,((Ipp8u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_8u_AC4IR ,((Ipp8u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_C1IR ,((Ipp32f*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_C3IR ,((Ipp32f*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiDilate_32f_AC4IR   ,((Ipp32f*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_C1R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_C3R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_C4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_AC4R   ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_C1R ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_C3R ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_C4R ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_AC4R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_C1R   ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_C3R   ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_C4R   ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_AC4R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_C1IR   ,((Ipp16u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_C3IR   ,((Ipp16u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_16u_AC4IR  ,((Ipp16u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_C1IR   ,((Ipp8u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_C3IR   ,((Ipp8u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_8u_AC4IR  ,((Ipp8u*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_C1IR  ,((Ipp32f*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_C3IR  ,((Ipp32f*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
   FUNC_CALL(ippiErode_32f_AC4IR ,((Ipp32f*)pSrc, srcStep, roiSize, m_mask, m_maskSize, m_anchor))
    return stsNoFunction;
}

CString CRunMorph::GetHistoryParms()
{
    CString parms;
    parms.Format("%dx%d, (%d,%d)",
                  m_maskSize.width, m_maskSize.height,
                  m_anchor.x, m_anchor.y);
    return parms;
}
