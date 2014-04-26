/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRound.cpp: implementation of the CRunRound class.
// CRunRound class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "common\CProcess.h"
#include "RunRound.h"
#include "ParmRoundDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRound::CRunRound()
{
    m_roundMode = ippRndZero;
}

CRunRound::~CRunRound()
{

}

CParamDlg* CRunRound::CreateDlg() { return new CParmRoundDlg;}

void CRunRound::UpdateData(CParamDlg* parmDlg, BOOL save)
{
     CippiRun::UpdateData(parmDlg, save);
     CParmRoundDlg* pDlg = (CParmRoundDlg*)parmDlg;
     if (save) {
          m_roundMode = (IppRoundMode)pDlg->m_roundMode;
     } else {
          pDlg->m_roundMode = (int)m_roundMode;
     }
}

BOOL CRunRound::BeforeCall()
{
    return TRUE;
}

BOOL CRunRound::AfterCall(BOOL bOk)
{
    if (!bOk) 
        return FALSE;
    if (!m_Func.Found("32f32u_C1I")) {
        m_pDocSrc->GetImage()->ChangeType(pp32u);
        m_pDocSrc->GetProcess()->ApplyVector(m_pDocSrc->GetImage());
    }
    return TRUE;
}

IppStatus CRunRound::CallIppFunction()
{
    FUNC_CALL(ippiConvert_32f8u_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8u_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8u_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8u_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16s_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16s_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16s_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16s_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8s_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8s_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8s_AC4R, ((Ipp32f*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f8s_C4R, ((Ipp32f*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16u_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16u_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16u_AC4R, ((Ipp32f*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode ))
    FUNC_CALL(ippiConvert_32f16u_C4R, ((Ipp32f*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode ))

    FUNC_CALL ( ippiConvert_8u8s_C1RSfs, ((Ipp8u*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_16s8s_C1RSfs, ((Ipp16s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_16u8s_C1RSfs, ((Ipp16u*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_16u16s_C1RSfs, ((Ipp16u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32s16s_C1RSfs, ((Ipp32s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32s16u_C1RSfs, ((Ipp32s*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32u8s_C1RSfs, ((Ipp32u*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32u8u_C1RSfs, ((Ipp32u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32u16s_C1RSfs, ((Ipp32u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32u16u_C1RSfs, ((Ipp32u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32u32s_C1RSfs, ((Ipp32u*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32f32u_C1RSfs, ((Ipp32f*)pSrc, srcStep, (Ipp32u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32f8s_C1RSfs, ((Ipp32f*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32f8u_C1RSfs, ((Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32f16s_C1RSfs, ((Ipp32f*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32f16u_C1RSfs, ((Ipp32f*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))
    FUNC_CALL ( ippiConvert_32f32s_C1RSfs, ((Ipp32f*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, m_roundMode, scaleFactor))

    FUNC_CALL ( ippiConvert_32f32u_C1IRSfs, ((Ipp32u*)pSrc, srcStep, roiSize, m_roundMode, scaleFactor))


 return stsNoFunction;
}

CString CRunRound::GetHistoryParms()
{
    CMyString parm;
    return parm << m_roundMode;
}

