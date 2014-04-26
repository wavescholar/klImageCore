/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterCol.cpp: implementation of the CRunFilterCol class.
// CRunFilterCol class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunFilterCol.h"
#include "ParmFilterColDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFilterCol::CRunFilterCol()
{
}

CRunFilterCol::~CRunFilterCol()
{

}

CParamDlg* CRunFilterCol::CreateDlg() 
{
    return new CParmFilterColDlg;
}

void CRunFilterCol::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunFilterRow::UpdateData(parmDlg,save);
    CParmFilterColDlg* pDlg = (CParmFilterColDlg*)parmDlg;
    if (save) {
        m_kernelLen = pDlg->m_maskSize.height;
        m_anchor    = pDlg->m_anchor.y;
    } else {
        pDlg->m_maskSize.width  = 1;
        pDlg->m_maskSize.height = m_kernelLen;
        pDlg->m_anchor.x        = 0   ;
        pDlg->m_anchor.y        = m_anchor;
    }
}

IppStatus CRunFilterCol::CallIppFunction()
{

    FUNC_CALL(ippiFilterColumn32f_8u_C1R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_8u_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16s_C1R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16s_C3R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16s_C4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16s_AC4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16u_C1R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16u_C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn32f_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterColumn_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    
    FUNC_CALL(ippiFilterColumn_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16u_AC4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterColumn_16s_AC4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))

    return stsNoFunction;
}
