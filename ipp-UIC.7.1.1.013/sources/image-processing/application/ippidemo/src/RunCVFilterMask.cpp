/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterMask.cpp: implementation of the CRunCVFilterMask class.
// CRunCVFilterMask class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "RunCVFilterMask.h"
#include "ParmCVBorderMaskDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVFilterMask::CRunCVFilterMask()
{
   m_mask = ippMskSize3x3;
}

CRunCVFilterMask::~CRunCVFilterMask()
{

}

CParamDlg* CRunCVFilterMask::CreateDlg() { return new CParmCVBorderMaskDlg;}

void CRunCVFilterMask::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunCVFilter::UpdateData(parmDlg, save);
    CParmCVBorderMaskDlg* pDlg = (CParmCVBorderMaskDlg*)parmDlg;
    if (save) {
        switch (pDlg->m_Mask) {
        case 0: m_mask = ippMskSize1x3; break;
        case 1: m_mask = ippMskSize3x1; break;
        case 2: m_mask = ippMskSize3x3; break;
        case 3: m_mask = ippMskSize1x5; break;
        case 4: m_mask = ippMskSize5x1; break;
        case 5: m_mask = ippMskSize5x5; break;
        default: ASSERT(0);
        }
    } else {
        switch (m_mask) {
        case ippMskSize1x3: pDlg->m_Mask = 0; break;
        case ippMskSize3x1: pDlg->m_Mask = 1; break;
        case ippMskSize3x3: pDlg->m_Mask = 2; break;
        case ippMskSize1x5: pDlg->m_Mask = 3; break;
        case ippMskSize5x1: pDlg->m_Mask = 4; break;
        case ippMskSize5x5: pDlg->m_Mask = 5; break;
        default: ASSERT(0);
        }
    }
}

IppStatus CRunCVFilterMask::CallGetBufSize(CString name)
{
   MATE_CV_CALL(name, ippiFilterSobelHorizGetBufferSize_8u16s_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelVertGetBufferSize_8u16s_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelHorizSecondGetBufferSize_8u16s_C1R,(roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelVertSecondGetBufferSize_8u16s_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelCrossGetBufferSize_8u16s_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterLaplacianGetBufferSize_8u16s_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterLowpassGetBufferSize_8u_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelHorizGetBufferSize_32f_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelVertGetBufferSize_32f_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelHorizSecondGetBufferSize_32f_C1R,(roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelVertSecondGetBufferSize_32f_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterSobelCrossGetBufferSize_32f_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterLaplacianGetBufferSize_32f_C1R, (roiSize, m_mask, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterLowpassGetBufferSize_32f_C1R, (roiSize, m_mask, &m_bufSize))

   return stsNoFunction;
}

IppStatus CRunCVFilterMask::CallIppFunction()
{
   FUNC_CV_CALL( ippiFilterSobelHorizBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelVertBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelHorizSecondBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelVertSecondBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelCrossBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterLaplacianBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterLowpassBorder_8u_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelHorizBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelVertBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelHorizSecondBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelVertSecondBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterSobelCrossBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterLowpassBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterLaplacianBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize, m_mask,
 m_borderType, m_borderValue, m_pBuffer))
   return stsNoFunction;
}

CString CRunCVFilterMask::GetHistoryParms()
{
   CMyString parms;
   parms << m_mask << ", " << CRunCVFilter::GetHistoryParms();
   return parms;
}

