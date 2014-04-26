/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCV2.cpp: implementation of the CRunCV2 class.
// CRunCV2 class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunCV2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCV2::CRunCV2()
{

}

CRunCV2::~CRunCV2()
{

}

BOOL CRunCV2::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

IppStatus CRunCV2::CallIppFunction()
{
   FUNC_CV_CALL( ippiAbsDiff_8u_C1R, ((const Ipp8u*)pSrc, srcStep, (const Ipp8u*)pSrc2, srcStep2, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CV_CALL( ippiAbsDiff_16u_C1R, ( (const Ipp16u*)pSrc, srcStep, (const Ipp16u*)pSrc2, srcStep2, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CV_CALL( ippiAbsDiff_32f_C1R, ( (const Ipp32f*)pSrc, srcStep, (const Ipp32f*)pSrc2, srcStep2, (Ipp32f*)pDst, dstStep, roiSize ))
   return stsNoFunction;
}
