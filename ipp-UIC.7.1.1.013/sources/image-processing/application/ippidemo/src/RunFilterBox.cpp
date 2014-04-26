/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterBox.cpp: implementation of the CRunFilterBox class.
// CRunFilterBox class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunFilterBox.h"
#include "ParmFilterBoxDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFilterBox::CRunFilterBox()
{
   m_maskSize.width = 3;
   m_maskSize.height = 3;
   m_anchor.x = 1;
   m_anchor.y = 1;
   m_bCenter = TRUE;
}

CRunFilterBox::~CRunFilterBox()
{

}

CParamDlg* CRunFilterBox::CreateDlg() { return new CParmFilterBoxDlg;}

void CRunFilterBox::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmFilterBoxDlg* pDlg = (CParmFilterBoxDlg*)parmDlg;
   if (save) {
      m_maskSize = pDlg->m_maskSize;
      m_anchor   = pDlg->m_anchor  ;
      m_bCenter   = pDlg->m_bCenter  ;
   } else {
      pDlg->m_maskSize = m_maskSize;
      pDlg->m_anchor   = m_anchor  ;
      pDlg->m_bCenter   = m_bCenter  ;
   }
}


int CRunFilterBox::GetSrcBorder(int idx)
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

IppStatus CRunFilterBox::CallIppFunction()
{
   FUNC_CALL(ippiFilterMax_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMax_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMin_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,  roiSize,  m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_8u_C1R,((Ipp8u*)pSrc,srcStep , (Ipp8u*)pDst,dstStep,roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_8u_C3R,((Ipp8u*)pSrc,srcStep , (Ipp8u*)pDst,dstStep,roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_8u_C4R,((Ipp8u*)pSrc,srcStep , (Ipp8u*)pDst,dstStep,roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_8u_AC4R,((Ipp8u*)pSrc,srcStep , (Ipp8u*)pDst,dstStep,roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16u_C1R,((Ipp16u*)pSrc,srcStep , (Ipp16u*)pDst,dstStep,roiSize,m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_16u_C3R,((Ipp16u*)pSrc,srcStep , (Ipp16u*)pDst,dstStep,roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16u_C4R,((Ipp16u*)pSrc,srcStep , (Ipp16u*)pDst,dstStep,roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16u_AC4R,((Ipp16u*)pSrc,srcStep , (Ipp16u*)pDst,dstStep,roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16s_C1R,((Ipp16s*)pSrc,srcStep , (Ipp16s*)pDst,dstStep,roiSize,m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_16s_C3R,((Ipp16s*)pSrc,srcStep , (Ipp16s*)pDst,dstStep,roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16s_C4R,((Ipp16s*)pSrc,srcStep , (Ipp16s*)pDst,dstStep,roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16s_AC4R,((Ipp16s*)pSrc,srcStep , (Ipp16s*)pDst,dstStep,roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, m_maskSize, m_anchor))

   FUNC_CALL(ippiFilterBox_8u_C1IR,((Ipp8u*)pSrc,srcStep , roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_8u_C3IR,((Ipp8u*)pSrc,srcStep , roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_8u_C4IR,((Ipp8u*)pSrc,srcStep , roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_8u_AC4IR,((Ipp8u*)pSrc,srcStep , roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16u_C1IR,((Ipp16u*)pSrc,srcStep , roiSize,m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_16u_C3IR,((Ipp16u*)pSrc,srcStep , roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16u_C4IR,((Ipp16u*)pSrc,srcStep , roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16u_AC4IR,((Ipp16u*)pSrc,srcStep , roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16s_C1IR,((Ipp16s*)pSrc,srcStep , roiSize,m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_16s_C3IR,((Ipp16s*)pSrc,srcStep , roiSize, m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16s_C4IR,((Ipp16s*)pSrc,srcStep , roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_16s_AC4IR,((Ipp16s*)pSrc,srcStep , roiSize,m_maskSize, m_anchor ))
   FUNC_CALL(ippiFilterBox_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterBox_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, m_maskSize, m_anchor))

   FUNC_CALL(ippiFilterMedian_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiFilterMedian_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_maskSize, m_anchor))

   FUNC_CALL(ippiFilterMedian_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor))


   return stsNoFunction;
}

CString CRunFilterBox::GetHistoryParms()
{
   CMyString parms;
   return parms << m_maskSize << ", (" << m_anchor << ")";
}

BOOL CRunFilterBox::IsMovie(CFunc func)
{
   return TRUE;
}

int CRunFilterBox::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                          int& numReps)
{
   m_maskSize_Save = m_maskSize;
   m_anchor_Save   = m_anchor  ;
   if (tim < 0) {
      m_maskSize.width = 5;
      m_maskSize.height = 5;
   } else {
      m_maskSize.width = GetMovieStepReverse(tim,4)*2 + 3;
      m_maskSize.height = m_maskSize.width;
   }
   m_anchor.x = m_maskSize.width >> 1;
   m_anchor.y = m_maskSize.height >> 1;
   dstInfo << m_maskSize;
   return tim;
}

void CRunFilterBox::ResetMovieArgs()
{
   m_maskSize = m_maskSize_Save;
   m_anchor   = m_anchor_Save  ;
}

