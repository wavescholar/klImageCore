/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopyM.cpp: implementation of the CRunCopyM class.
// CRunCopyM class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "RunCopyM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCopyM::CRunCopyM()
{
}

CRunCopyM::~CRunCopyM()
{

}

BOOL CRunCopyM::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_MASK | VEC_DST;
   return TRUE;
}

IppStatus CRunCopyM::CallIppFunction()
{
   FUNC_CALL(ippiCopy_8u_C1MR, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_8u_C3MR, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_8u_C4MR, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_8u_AC4MR, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16u_C1MR, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16u_C3MR, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16u_C4MR, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16u_AC4MR, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16s_C1MR, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16s_C3MR, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16s_C4MR, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_16s_AC4MR, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32s_C1MR, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32s_C3MR, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32s_C4MR, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32s_AC4MR, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32f_C1MR, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32f_C3MR, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32f_C4MR, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiCopy_32f_AC4MR, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,roiSize, (Ipp8u*)pMask, maskStep ))

   return stsNoFunction;
}


