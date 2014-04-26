/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilter.cpp: implementation of the CRunFilter class.
// CRunFilter class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "ParmFilterDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFilter::CRunFilter()
{
    m_mask = ippMskSize3x3;
}

CRunFilter::~CRunFilter()
{

}

CParamDlg* CRunFilter::CreateDlg() { return new CParmFilterDlg;}

void CRunFilter::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmFilterDlg* pDlg = (CParmFilterDlg*)parmDlg;
    if (save) {
        switch (pDlg->m_RadioMask) {
        case msk1x3: m_mask = ippMskSize1x3; break;
        case msk3x1: m_mask = ippMskSize3x1; break;
        case msk3x3: m_mask = ippMskSize3x3; break;
        case msk1x5: m_mask = ippMskSize1x5; break;
        case msk5x1: m_mask = ippMskSize5x1; break;
        case msk5x5: m_mask = ippMskSize5x5; break;
        default: ASSERT(0);
        }
    } else {
        switch (m_mask) {
        case ippMskSize1x3: pDlg->m_RadioMask = msk1x3; break;
        case ippMskSize3x1: pDlg->m_RadioMask = msk3x1; break;
        case ippMskSize3x3: pDlg->m_RadioMask = msk3x3; break;
        case ippMskSize1x5: pDlg->m_RadioMask = msk1x5; break;
        case ippMskSize5x1: pDlg->m_RadioMask = msk5x1; break;
        case ippMskSize5x5: pDlg->m_RadioMask = msk5x5; break;
        default: ASSERT(0);
        }
    }
}

int CRunFilter::GetSrcBorder(int idx)
{
    switch (idx)
    {
    case LEFT: case RIGHT:
        return ((int)m_mask/10) >> 1;
    case TOP: case BOTTOM:
       return ((int)m_mask%10) >> 1;
    }
    return 0;
}

IppStatus CRunFilter::CallIppFunction()
{
   FUNC_CALL(ippiFilterMedianHoriz_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianHoriz_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   
   FUNC_CALL(ippiFilterMedianVert_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianVert_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   
   FUNC_CALL(ippiFilterMedianCross_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterMedianCross_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterMedianCross_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterMedianCross_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianCross_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   
   FUNC_CALL(ippiFilterMedianColor_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterMedianColor_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianColor_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianColor_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterMedianColor_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianColor_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianColor_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterMedianColor_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterMedianColor_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))

   FUNC_CALL(ippiFilterLaplace_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLaplace_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
    FUNC_CALL(ippiFilterLaplace_8u16s_C1R,((Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
    FUNC_CALL(ippiFilterLaplace_8s16s_C1R,((Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))

   FUNC_CALL(ippiFilterGauss_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterGauss_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   
   FUNC_CALL(ippiFilterHipass_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterHipass_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   
   FUNC_CALL(ippiFilterLowpass_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterLowpass_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterLowpass_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterLowpass_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
//   FUNC_CALL(ippiFilterLowpass_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))
   FUNC_CALL(ippiFilterLowpass_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask))

   FUNC_CALL(ippiFilterSobelVert_8u16s_C1R,((Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelHoriz_8u16s_C1R,((Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))

   FUNC_CALL(ippiFilterSobelVertSecond_8u16s_C1R,((Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelHorizSecond_8u16s_C1R,((Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelCross_8u16s_C1R,((Ipp8u*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelVert_8s16s_C1R,((Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelHoriz_8s16s_C1R,((Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelVertSecond_8s16s_C1R,((Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelHorizSecond_8s16s_C1R,((Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelCross_8s16s_C1R,((Ipp8s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelVertMask_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelHorizMask_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelVertSecond_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelHorizSecond_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask ))
   FUNC_CALL(ippiFilterSobelCross_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_mask ))
   return stsNoFunction;
}

BOOL CRunFilter::BeforeCall()
{
    switch (m_mask) {
    case ippMskSize1x3: m_nCols = 1; m_nRows = 3; break;
    case ippMskSize3x1: m_nCols = 3; m_nRows = 1; break;
    case ippMskSize3x3: m_nCols = 3; m_nRows = 3; break;
    case ippMskSize1x5: m_nCols = 1; m_nRows = 5; break;
    case ippMskSize5x1: m_nCols = 5; m_nRows = 1; break;
    case ippMskSize5x5: m_nCols = 5; m_nRows = 5; break;
    default:         m_nCols = 0; m_nRows = 0; break;
    }
    m_anchorX = m_nCols >> 1;
    m_anchorY = m_nRows >> 1;
    return TRUE;
}

CString CRunFilter::GetHistoryParms()
{
   CMyString parms;
   return parms << m_mask;
}

BOOL CRunFilter::IsMovie(CFunc func)
{
   return TRUE;
}

int CRunFilter::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                             int& numReps)
{
   m_mask_Save = m_mask;
   IppiMaskSize mask3 = m_mask = ippMskSize3x3;
   IppiMaskSize mask5 = m_mask = ippMskSize5x5;
   if (m_Func.Found("Horiz")) {
      mask3 = ippMskSize3x1;
      mask5 = ippMskSize5x1;
   } else if (m_Func.Found("Vert")) {
      mask3 = ippMskSize1x3;
      mask5 = ippMskSize1x5;
   }
   if (tim < 0) {
      m_mask = mask5;
   } else {
      m_mask = GetMovieStep(tim,8) & 1 ? mask5 : mask3;
   }
   dstInfo << m_mask;
   return tim;
}

void CRunFilter::ResetMovieArgs()
{
   m_mask = m_mask_Save;
}

