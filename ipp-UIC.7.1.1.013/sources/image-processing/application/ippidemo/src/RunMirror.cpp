/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMirror.cpp: implementation of the CRunMirror class.
// CRunMirror class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunMirror.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunMirror::CRunMirror()
{
    m_flip = ippAxsHorizontal;
}

CRunMirror::~CRunMirror()
{

}

void CRunMirror::UpdateData(CParamDlg* pDlg, BOOL save)
{
    CippiRun::UpdateData(pDlg,save);
    CParmMirrorDlg* pMirrorDlg = (CParmMirrorDlg*)pDlg;
    if (save) {
        m_flip = (IppiAxis)pMirrorDlg->m_Flip;
    } else {
        pMirrorDlg->m_Flip = m_flip;
    }
}

IppStatus CRunMirror::CallIppFunction()
{

   FUNC_CALL(ippiMirror_8u_C1R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_C3R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_C4R   ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_AC4R  ,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_C1IR  ,((Ipp8u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_C3IR  ,((Ipp8u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_C4IR  ,((Ipp8u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_8u_AC4IR ,((Ipp8u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_C1R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_C3R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_C4R  ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_AC4R ,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_C1IR ,((Ipp16u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_C3IR ,((Ipp16u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_C4IR ,((Ipp16u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16u_AC4IR   ,((Ipp16u*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_C1R  ,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_C3R  ,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_C4R  ,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_AC4R ,((Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_C1IR ,((Ipp32s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_C3IR ,((Ipp32s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_C4IR ,((Ipp32s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32s_AC4IR   ,((Ipp32s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_C1R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_C3R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_C4R  ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_AC4R ,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_C1IR ,((Ipp16s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_C3IR ,((Ipp16s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_C4IR ,((Ipp16s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_16s_AC4IR   ,((Ipp16s*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_C1R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_C3R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_C4R  ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_AC4R ,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_C1IR ,((Ipp32f*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_C3IR ,((Ipp32f*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_C4IR ,((Ipp32f*)pSrc, srcStep, roiSize, m_flip))
   FUNC_CALL(ippiMirror_32f_AC4IR   ,((Ipp32f*)pSrc, srcStep, roiSize, m_flip))
    return stsNoFunction;
}

CString CRunMirror::GetHistoryParms()
{
    switch (m_flip) {
    case ippAxsHorizontal: return "horizontal";
    case ippAxsVertical  : return "vertical";
    case ippAxsBoth      : return "both";
    default:           return "???";
    }
}

BOOL CRunMirror::IsMovie(CFunc func)
{
   return RUNMOVIE_REDRAW;
}

int CRunMirror::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                             int& numReps)
{
   m_flip_Save = m_flip;
   if (tim < 0) {
      m_flip = ippAxsVertical;
   } else {
      int step = GetMovieStep(tim, 8) % 4;
      switch (step) {
      case 0: m_flip = ippAxsHorizontal; break;
      case 1: m_flip = ippAxsBoth; break;
      case 2: m_flip = ippAxsVertical; break;
      case 3: numReps = -1; return tim;
      }
   }
   dstInfo << m_flip;
   return tim;
}

void CRunMirror::ResetMovieArgs()
{
   m_flip = m_flip_Save;
}
