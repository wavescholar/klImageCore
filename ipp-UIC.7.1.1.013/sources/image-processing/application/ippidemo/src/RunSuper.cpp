/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSuper.cpp: implementation of the CRunSuper class.
// CRunSuper class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunSuper.h"
#include "ParmSuperDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunSuper::CRunSuper()
{
     m_pBuffer = NULL;
}

CRunSuper::~CRunSuper()
{

}

CParamDlg* CRunSuper::CreateDlg() { return new CParmSuperDlg;}

void CRunSuper::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmSuperDlg* pDlg = (CParmSuperDlg*)parmDlg;
    if (save) {
        m_dstSize = pDlg->m_dstSize;
    } else {
    }
}

ImgHeader CRunSuper::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    header.SetSize(m_dstSize.width, m_dstSize.height);
    return header;
}
BOOL CRunSuper::BeforeCall()
{
    int bufSize = 0;
    CHECK_CALL(ippiSuperSamplingGetBufSize, (
        m_srcRoiSize, m_dstRoiSize, m_Func.SrcChannels(), &bufSize))
    if (bufSize)
        m_pBuffer = ippMalloc(bufSize);
    return TRUE;
}

BOOL CRunSuper::AfterCall(BOOL bOk)
{
    if (m_pBuffer) {
         ippFree(m_pBuffer);
         m_pBuffer = NULL;
    }
    return TRUE;
}

IppStatus CRunSuper::CallIppFunction()
{
    FUNC_CALL(ippiSuperSampling_8u_C1R,( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_8u_C3R,( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_8u_C4R,( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_8u_AC4R,( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_8u_P3R,( (Ipp8u**)pSrcP, srcStep, m_srcRoiSize, (Ipp8u**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_8u_P4R,( (Ipp8u**)pSrcP, srcStep, m_srcRoiSize, (Ipp8u**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16u_C1R,( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16u_C3R,( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16u_C4R,( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16u_AC4R,( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16u_P3R,( (Ipp16u**)pSrcP, srcStep, m_srcRoiSize, (Ipp16u**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16u_P4R,( (Ipp16u**)pSrcP, srcStep, m_srcRoiSize, (Ipp16u**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16s_C1R,( (Ipp16s*)pSrc, srcStep, m_srcRoiSize, (Ipp16s*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16s_C3R,( (Ipp16s*)pSrc, srcStep, m_srcRoiSize, (Ipp16s*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16s_C4R,( (Ipp16s*)pSrc, srcStep, m_srcRoiSize, (Ipp16s*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16s_AC4R,( (Ipp16s*)pSrc, srcStep, m_srcRoiSize, (Ipp16s*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16s_P3R,( (Ipp16s**)pSrcP, srcStep, m_srcRoiSize, (Ipp16s**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_16s_P4R,( (Ipp16s**)pSrcP, srcStep, m_srcRoiSize, (Ipp16s**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_32f_C1R,( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_32f_C3R,( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_32f_C4R,( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_32f_AC4R,( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_32f_P3R,( (Ipp32f**)pSrcP, srcStep, m_srcRoiSize, (Ipp32f**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiSuperSampling_32f_P4R,( (Ipp32f**)pSrcP, srcStep, m_srcRoiSize, (Ipp32f**)pDstP, dstStep, m_dstRoiSize, (Ipp8u*)m_pBuffer))
    return stsNoFunction;
}
