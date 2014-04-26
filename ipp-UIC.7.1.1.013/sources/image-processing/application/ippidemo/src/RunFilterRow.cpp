/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterRow.cpp: implementation of the CRunFilterRow class.
// CRunFilterRow class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunFilterRow.h"
#include "ParmFilterRowDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFilterRow::CRunFilterRow()
{
    m_kernelLen  = 3;
    m_anchor = 1;
    m_divisor = 1;
    m_DivType = 0;

    m_k = 0;
    for (int k=0; k < kNum; k++)
    {
        m_pKernel[k].Init(kernelType(k), 3);
        m_pKernel[k].Set(0,-1.);
        m_pKernel[k].Set(1, 2.);
        m_pKernel[k].Set(2,-1.);
    }
}

CRunFilterRow::~CRunFilterRow()
{

}

BOOL CRunFilterRow::Open(CFunc func)
{
    if (!CRunFilterBox::Open(func)) return FALSE;

    m_bDiv = !m_Func.Found("32f");
    m_k = 
        m_Func.Found("32f") ? k32f :
                              k32s;
    return TRUE;
}

void CRunFilterRow::Close()
{
    CRunFilterBox::Close();
}

CParamDlg* CRunFilterRow::CreateDlg() 
{
    return new CParmFilterRowDlg;
}

void CRunFilterRow::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunFilterBox::UpdateData(parmDlg,save);
    CParmFilterRowDlg* pDlg = (CParmFilterRowDlg*)parmDlg;
    if (save) {
        m_kernelLen = pDlg->m_maskSize.width;
        m_anchor    = pDlg->m_anchor.x;
        m_pKernel[m_k] = pDlg->m_kernel;
        m_divisor = atoi(pDlg->m_DivStr);
        m_DivType = pDlg->m_DivType;
    } else {
        pDlg->m_maskSize.width  = m_kernelLen;
        pDlg->m_maskSize.height = 1;
        pDlg->m_anchor.x        = m_anchor   ;
        pDlg->m_anchor.y        = 0;
        pDlg->m_kernel = m_pKernel[m_k];
        pDlg->m_DivStr.Format("%d",m_divisor);
        pDlg->m_DivType = m_DivType;
        pDlg->m_roundMode = 0;
        pDlg->m_bDiv   = m_bDiv;
        pDlg->m_bRound = FALSE;
    }
}

IppStatus CRunFilterRow::CallIppFunction()
{

    FUNC_CALL(ippiFilterRow_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_8u_C4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_8u_AC4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16u_C1R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16u_C3R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16u_C4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16u_AC4R, ((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16s_C1R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16s_C3R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16s_C4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))
    FUNC_CALL(ippiFilterRow_16s_AC4R, ((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize,(Ipp32s*)m_pKernel[m_k], m_kernelLen, m_anchor, m_divisor ))

    FUNC_CALL(ippiFilterRow32f_8u_C1R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_8u_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16u_C1R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16u_C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16s_C1R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16s_C3R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16s_C4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow32f_16s_AC4R, ( (Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow_32f_C1R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow_32f_C4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))
    FUNC_CALL(ippiFilterRow_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_pKernel[m_k], m_kernelLen, m_anchor ))


    return stsNoFunction;
}

CString CRunFilterRow::GetHistoryParms()
{
    CMyString parms;
    return parms << m_kernelLen << ", " << m_anchor;
}

void CRunFilterRow::AddHistoFunc(CHisto* pHisto, int vecPos)
{
     CRunFilterBox::AddHistoFunc(pHisto,vecPos);
}
