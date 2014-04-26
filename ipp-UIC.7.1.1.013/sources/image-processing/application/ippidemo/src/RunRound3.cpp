/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRound3.cpp: implementation of the CRunRound3 class.
// CRunRound3 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CRunRound classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "CProcess.h"
#include "RunRound3.h"
#include "ParmRound3Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRound3::CRunRound3()
{
}

CRunRound3::~CRunRound3()
{

}

BOOL CRunRound3::Open(CFunc func)
{
   if (!CRunRound::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

CParamDlg* CRunRound3::CreateDlg() { return new CParmRound3Dlg;}

IppStatus CRunRound3::CallIppFunction()
{
    FUNC_CALL( ippiDiv_Round_16s_C1RSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_C3RSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_C4RSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_AC4RSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_C1RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_C3RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_C1RSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_C3RSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_C4RSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_AC4RSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_C1IRSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_C3IRSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_C4IRSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16s_AC4IRSfs, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_C1IRSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_C3IRSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_C4IRSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_8u_AC4IRSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_C1IRSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_C3IRSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_C4IRSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL( ippiDiv_Round_16u_AC4IRSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pSrc2, srcStep2, roiSize, m_roundMode, scaleFactor))

 return stsNoFunction;
}

