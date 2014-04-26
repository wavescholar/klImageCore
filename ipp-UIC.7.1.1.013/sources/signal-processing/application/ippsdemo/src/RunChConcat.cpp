
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChConcat.cpp : implementation of the CRunChConcatclass.
// CRunChConcatclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunChConcat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChConcat::CRunChConcat()
{

}

CRunChConcat::~CRunChConcat()
{

}

BOOL CRunChConcat::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

int CRunChConcat::GetDstLength()
{
   return m_pDocSrc->GetVector()->Length() + m_pDocSrc2->GetVector()->Length();
}

IppStatus CRunChConcat::CallIppFunction()
{
   FUNC_CH_CALL(ippsConcat_8u, ((Ipp8u*)pSrc, m_lenSrc, (Ipp8u*)pSrc2, m_lenSrc2, (Ipp8u*)pDst))
   FUNC_CH_CALL(ippsConcat_16u, ((Ipp16u*)pSrc, m_lenSrc, (Ipp16u*)pSrc2, m_lenSrc2, (Ipp16u*)pDst))

   return stsNoFunction;
}
