/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterCustom.cpp: implementation of the CRunFilterCustom class.
// CRunFilterCustom class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "Histo.h"
#include "RunFilterCustom.h"
#include "ParmFilterCustomDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRunFilterCustom::CRunFilterCustom()
{
    m_maskSize.width = 3;
    m_maskSize.height = 3;
    m_anchor.x = 1;
    m_anchor.y = 1;
    m_divisor = 1;
    m_DivType = 0;
    m_roundMode = ippRndZero;
    m_pBuffer = NULL;
    m_k = 0;

    for (int k=0; k < kNum; k++)
    {
        m_pKernel[k].Init(kernelType(k), 9);
        for (int i=0; i<9; i++)
            m_pKernel[k].Set(i,-1);
        m_pKernel[k].Set(4,8);
    }

}

CRunFilterCustom::~CRunFilterCustom()
{

}

BOOL CRunFilterCustom::Open(CFunc func)
{
    if (!CRunFilterBox::Open(func)) return FALSE;
    m_bDiv = !m_Func.Found("32f");
    m_bRound = !m_Func.Found("Round");

    m_k = 
        m_Func.Found("32f")            ? k32f :
        m_Func.BaseName().Found("16s") ? k16s : 
                                         k32s;

    m_nameGetBufSize = "";
    if (m_Func.Found("Round"))
        m_nameGetBufSize = "ippiFilterRoundGetBufSize" + m_Func.Mid(16);

    return TRUE;
}

void CRunFilterCustom::Close()
{
}

CParamDlg* CRunFilterCustom::CreateDlg() { return new CParmFilterCustomDlg;}

void CRunFilterCustom::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunFilterBox::UpdateData(parmDlg,save);
    CParmFilterCustomDlg* pDlg = (CParmFilterCustomDlg*)parmDlg;
    if (save) {
        m_pKernel[m_k] = pDlg->m_kernel;
        m_divisor = atoi(pDlg->m_DivStr);
        m_roundMode = (IppRoundMode)pDlg->m_roundMode;
        m_DivType = pDlg->m_DivType;
    } else {
        pDlg->m_kernel = m_pKernel[m_k];
        pDlg->m_DivStr.Format("%d",m_divisor);
        pDlg->m_DivType = m_DivType;
        pDlg->m_roundMode = m_roundMode  ;
        pDlg->m_bDiv   = m_bDiv  ;
        pDlg->m_bRound = m_bRound;
    }
}

BOOL CRunFilterCustom::BeforeCall()
{
    int bufSize = 0;
    IppStatus status = CallGetBufSize(m_maskSize, roiSize.width, &bufSize);
    if (status >= 0 && bufSize)
        m_pBuffer = (Ipp8u*)ippMalloc(bufSize);
    return TRUE;
}

BOOL CRunFilterCustom::AfterCall(BOOL bOK)
{
    if (m_pBuffer) ippFree(m_pBuffer); m_pBuffer = NULL;
    return TRUE;
}

IppStatus CRunFilterCustom::CallGetBufSize(IppiSize  kernelSize, int roiWidth, int* pSize)
{
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize16s_8u_C1R  ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize16s_8u_C3R  ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize16s_8u_C4R  ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize16s_8u_AC4R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16u_C1R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16u_C3R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16u_C4R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16u_AC4R,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16s_C1R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16s_C3R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16s_C4R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32s_16s_AC4R,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_8u_C1R  ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_8u_C3R  ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_8u_C4R  ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_8u_AC4R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16u_C1R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16u_C3R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16u_C4R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16u_AC4R,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16s_C1R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16s_C3R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16s_C4R ,(kernelSize, roiWidth, pSize))
    MATE_CALL(m_nameGetBufSize, ippiFilterRoundGetBufSize32f_16s_AC4R,(kernelSize, roiWidth, pSize))
    return stsNoFunction;
}

IppStatus CRunFilterCustom::CallIppFunction()
{
    FUNC_CALL(ippiFilter32f_8u_C1R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8u_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8s_C1R, ( (Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8s_C3R, ( (Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8s_C4R, ( (Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
//    FUNC_CALL(ippiFilter32f_8s_AC4R, ( (Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8u16s_C1R, ( (Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8u16s_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8u16s_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
//    FUNC_CALL(ippiFilter32f_8u16s_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8s16s_C1R, ( (Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8s16s_C3R, ( (Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_8s16s_C4R, ( (Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
//    FUNC_CALL(ippiFilter32f_8s16s_AC4R, ( (Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16s_C1R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16s_C3R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16s_C4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16s_AC4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16u_C1R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16u_C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_32s_C1R, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_32s_C3R, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter32f_32s_C4R, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
//    FUNC_CALL(ippiFilter32f_32s_AC4R, ( (Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))
    FUNC_CALL(ippiFilter_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor ))

    FUNC_CALL(ippiFilter_8u_C1R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_8u_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16u_C1R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16u_C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16s_C1R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16s_C3R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16s_C4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilter_16s_AC4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor ))

    FUNC_CALL( ippiFilter_Round16s_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp16s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round16s_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp16s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round16s_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp16s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round16s_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp16s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16u_AC4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32s_16s_AC4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_maskSize, m_anchor, m_divisor, m_roundMode, (Ipp8u*)m_pBuffer ))

    FUNC_CALL( ippiFilter_Round32f_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer))
    FUNC_CALL( ippiFilter_Round32f_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer))
    FUNC_CALL( ippiFilter_Round32f_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer))
    FUNC_CALL( ippiFilter_Round32f_16u_AC4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer))

    FUNC_CALL( ippiFilter_Round32f_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer ))
    FUNC_CALL( ippiFilter_Round32f_16s_AC4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_maskSize, m_anchor, m_roundMode, (Ipp8u*)m_pBuffer )) 
        
    return stsNoFunction;
}

CString CRunFilterCustom::GetHistoryParms()
{
    CMyString parms;
    parms << m_maskSize << ", (" << m_anchor << ")";
    if (m_bDiv)
        parms << ", " << m_divisor;
    if (m_bRound)
        parms << ", " << m_roundMode;
    return parms;
}

void CRunFilterCustom::AddHistoFunc(CHisto* pHisto, int vecPos)
{
    CRunFilterBox::AddHistoFunc(pHisto,vecPos);
}

