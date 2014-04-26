
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCh12.cpp : implementation of the CRunCh12class.
// CRunCh12class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunCh12.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCh12::CRunCh12()
{

}

CRunCh12::~CRunCh12()
{

}

BOOL CRunCh12::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_DST;
   return TRUE;
}

IppStatus CRunCh12::CallIppFunction()
{
   FUNC_CH_CALL(ippsUppercaseLatin_8u_I, ( (Ipp8u*)pSrc, len ))
   FUNC_CH_CALL(ippsLowercaseLatin_8u_I, ( (Ipp8u*)pSrc, len ))
   FUNC_CH_CALL(ippsLowercaseLatin_8u,  ((Ipp8u*)pSrc, (Ipp8u*)pDst, len))
   FUNC_CH_CALL(ippsUppercaseLatin_8u,  ((Ipp8u*)pSrc, (Ipp8u*)pDst, len))
   FUNC_CH_CALL(ippsUppercaseLatin_16u_I,( (Ipp16u*)pSrc, len ))
   FUNC_CH_CALL(ippsLowercaseLatin_16u_I,( (Ipp16u*)pSrc, len ))
   FUNC_CH_CALL(ippsUppercaseLatin_16u, ((Ipp16u*)pSrc, (Ipp16u*)pDst, len))
   FUNC_CH_CALL(ippsLowercaseLatin_16u, ((Ipp16u*)pSrc, (Ipp16u*)pDst, len))
   FUNC_CH_CALL(ippsUppercase_16u_I,( (Ipp16u*)pSrc, len ))
   FUNC_CH_CALL(ippsLowercase_16u_I,( (Ipp16u*)pSrc, len ))
   FUNC_CH_CALL(ippsUppercase_16u, ((Ipp16u*)pSrc, (Ipp16u*)pDst, len))
   FUNC_CH_CALL(ippsLowercase_16u, ((Ipp16u*)pSrc, (Ipp16u*)pDst, len))

   return stsNoFunction;
}
