/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunReduce.cpp: implementation of the CRunReduce class.
// CRunReduce class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunReduce.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int DitherTypeNum = 12;
static CMyString DitherTypeStr[] = {
    "None",
    "FS",
    "JJN",
    "Stucki",
    "Bayer",
};
static IppiDitherType DitherTypeVal[] = {
    ippDitherNone,
    ippDitherFS,
    ippDitherJJN,
    ippDitherStucki,
    ippDitherBayer
};
CMyString& operator << (CMyString& str, IppiDitherType val) {
   str += ENUM_STRING(DitherType,val);
   return str;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunReduce::CRunReduce()
{
    m_noise = 0;
    m_dtype = ippDitherFS;
    m_levels = 2;
}

CRunReduce::~CRunReduce()
{

}

void CRunReduce::UpdateData(CParamDlg* pDlg, BOOL save)
{
    CippiRun::UpdateData(pDlg,save);
    CParmReduceDlg* pReduceDlg = (CParmReduceDlg*)pDlg;
    if (save) {
        m_noise = pReduceDlg->m_Noise;
        m_levels = pReduceDlg->m_Levels;
        m_dtype = (IppiDitherType)pReduceDlg->m_DitherType;
    } else {
        pReduceDlg->m_Noise      = m_noise ;
        pReduceDlg->m_Levels     = m_levels;
        pReduceDlg->m_DitherType = m_dtype ;
    }
}

IppStatus CRunReduce::CallIppFunction()
{
   FUNC_CC_CALL(ippiReduceBits_8u_C1R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_8u_C4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u_C1R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u_C4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u8u_C1R,((Ipp16u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u8u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u8u_C4R,((Ipp16u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16u8u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s_C1R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s_C4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s8u_C1R,((Ipp16s*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s8u_C3R,((Ipp16s*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s8u_C4R,((Ipp16s*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_16s8u_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f8u_C1R,((Ipp32f*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f8u_C3R,((Ipp32f*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f8u_C4R,((Ipp32f*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f8u_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16u_C1R,((Ipp32f*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16u_C3R,((Ipp32f*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16u_C4R,((Ipp32f*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16u_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16s_C1R,((Ipp32f*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16s_C3R,((Ipp32f*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16s_C4R,((Ipp32f*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
   FUNC_CC_CALL(ippiReduceBits_32f16s_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,
       m_noise, m_dtype, m_levels))
    return stsNoFunction;
}

CString CRunReduce::GetHistoryParms()
{
    CMyString parms;
    return parms << m_noise << ", " << m_dtype << ", " << m_levels;
}

BOOL CRunReduce::IsMovie(CFunc func)
{
   return TRUE;
}

int CRunReduce::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                             int& numReps)
{
   m_dtype_Save  = m_dtype;
   m_noise_Save  = m_noise;
   m_levels_Save = m_levels;

   if (tim < 0) {
      m_dtype = ippDitherBayer;
      m_levels = 2;
      if (srcInfo == "Noise")
         m_noise = 50;
      else
         m_noise = 0;
   } else {
      int step;
      if (srcInfo == "Noise") {
         step = GetMovieStep(tim, 36);
         m_noise = step % 9 + 1;
         step /= 9;
         if (m_noise > 5) m_noise = 10 - m_noise;
         m_noise *= 10;
         m_levels = 2;
      } else {
         step = GetMovieStep(tim, 8);
         m_levels = step & 1 ? 2 : 4;
         step /= 2;
         m_noise = 0;
      }
      m_dtype = (IppiDitherType)(step + 1);
   }
   dstInfo << m_dtype << "-" << m_levels << "-" << m_noise;
   return tim;
}

void CRunReduce::ResetMovieArgs()
{
   m_dtype  = m_dtype_Save ;
   m_noise  = m_noise_Save ;
   m_levels = m_levels_Save;
}
