/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResizeSqrPixel.cpp: implementation of the CRunResizeSqrPixel class.
// CRunResizeSqrPixel class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "ippidemodoc.h"
#include "RunResizeSqrPixel.h"
#include "ParmResizeSqrPixelDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunResizeSqrPixel::CRunResizeSqrPixel()
{
     m_ShiftType    = GEOM_CONTOUR;
     m_xShift = 10;
     m_yShift = 10;
     m_pBuffer = NULL;
     m_bMovie = FALSE;
}

BOOL  CRunResizeSqrPixel::Open(CFunc func)
{
    if (!CRunResizeShift::Open(func)) 
        return FALSE;
    return TRUE;
}

CRunResizeSqrPixel::~CRunResizeSqrPixel()
{

}

CParamDlg* CRunResizeSqrPixel::CreateDlg() { return new CParmResizeSqrPixelDlg;}

void CRunResizeSqrPixel::UpdateData(CParamDlg* parmDlg, BOOL save)
{
     CRunResizeShift::UpdateData(parmDlg,save);
     CParmResizeSqrPixelDlg* pDlg = (CParmResizeSqrPixelDlg*)parmDlg;
     if (save) {
          m_interpolate = pDlg->m_interpolate;
     } else {
          pDlg->m_interpolate = m_interpolate;
     }
}

ImgHeader CRunResizeSqrPixel::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    double xShift = (int)(roi.x*m_xFactor + m_xShift + 0.5);
    double yShift = (int)(roi.y*m_yFactor + m_yShift + 0.5);
    int w = (int)(roi.width  * m_xFactor + xShift);
    int h = (int)(roi.height * m_yFactor + yShift);
    header.SetSize(w, h);
    return header;
}

BOOL CRunResizeSqrPixel::PrepareSrc()
{
    return TRUE;
}

BOOL CRunResizeSqrPixel::PrepareDst()
{
    if (!CRunResizeShift::PrepareDst()) return FALSE;

    return TRUE;
}

void CRunResizeSqrPixel::PrepareParameters()
{
    CRunResizeShift::PrepareParameters();
    if (m_ShiftType == GEOM_ROI)
    {
        m_xShift = m_dstROI.x - m_srcROI.x*m_xFactor;
        m_yShift = m_dstROI.y - m_srcROI.y*m_yFactor;
    }
}

BOOL CRunResizeSqrPixel::BeforeCall()
{
    int bufSize = 0;
    CHECK_CALL(ippiResizeGetBufSize, (m_srcROI, m_dstROI, m_Func.SrcChannels(), 
                                      m_interpolate, &bufSize))
    if (bufSize)
        m_pBuffer = ippMalloc(bufSize);
    return TRUE;
}

BOOL CRunResizeSqrPixel::AfterCall(BOOL bOk)
{
    if (m_pBuffer) {
         ippFree(m_pBuffer);
         m_pBuffer = NULL;
    }
    if (!m_bMovie && m_xShift && m_yShift) {
        CImage* pDstImg = m_pDocDst->GetImage();
        if (!pDstImg->IsCenterShift()) {
            IppiRect roi = pDstImg->GetActualRoi();
            double center[2] = {0, 0};
            double shift[2] = {m_xShift, m_yShift};
            pDstImg->SetCenterShift(center, shift);
        }
    }
    return TRUE;
}

IppStatus CRunResizeSqrPixel::CallIppFunction()
{

    FUNC_CALL(ippiResizeSqrPixel_8u_C1R, ( (const Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_8u_C3R, ( (const Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_8u_C4R, ( (const Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_8u_AC4R, ( (const Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_8u_P3R, ( (const Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_8u_P4R, ( (const Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))

    FUNC_CALL(ippiResizeSqrPixel_16u_C1R, ( (const Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16u_C3R, ( (const Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16u_C4R, ( (const Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16u_AC4R, ( (const Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16u_P3R, ( (const Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16u_P4R, ( (const Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))

    FUNC_CALL(ippiResizeSqrPixel_16s_C1R, ( (const Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16s*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16s_C3R, ( (const Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16s*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16s_C4R, ( (const Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16s*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16s_AC4R, ( (const Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16s*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16s_P3R, ( (const Ipp16s**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16s**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_16s_P4R, ( (const Ipp16s**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16s**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))

    FUNC_CALL(ippiResizeSqrPixel_32f_C1R, ( (const Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_32f_C3R, ( (const Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_32f_C4R, ( (const Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_32f_AC4R, ( (const Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_32f_P3R, ( (const Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))
    FUNC_CALL(ippiResizeSqrPixel_32f_P4R, ( (const Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_xFactor, m_yFactor, m_xShift, m_yShift, m_interpolate, (Ipp8u*)m_pBuffer))


    return stsNoFunction;
}

CString CRunResizeSqrPixel::GetHistoryParms()
{
     CMyString parms;
     parms << m_xFactor << ", "
           << m_yFactor << ", "
           << m_xShift << ", "
           << m_yShift << ", "
           << GetInterParm(m_interpolate);
     return parms;
}


BOOL CRunResizeSqrPixel::IsMovie(CFunc func)
{
    return RUNMOVIE_REDRAW | RUNMOVIE_COPYSRC;
}

void CRunResizeSqrPixel::PrepareMovieParameters(CImage* pImgSrc, CImage* pImgDst)
{
   SetImageParameters(pImgSrc, pSrcImg, pSrc, srcStep,
                      pSrcImgP, pSrcP, srcStepP,
                      m_srcROI, m_srcRoiSize, m_srcSize);
   SetImageParameters(pImgDst, pDstImg, pDst, dstStep,
                      pDstImgP, pDstP, dstStepP,
                      m_dstROI, m_dstRoiSize, m_dstSize);
}

int CRunResizeSqrPixel::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                                             int& numReps)
{
    CRunResizeShift::SetMovieArgs(tim, srcInfo, dstInfo, numReps);
    m_xShift_Save = m_xShift;
    m_yShift_Save = m_yShift;

    m_xShift = (int)(m_dstROI.x - m_srcROI.x*m_xFactor + 0.5*m_dstROI.width *(1. - m_xFactor) + 0.5);
    m_yShift = (int)(m_dstROI.y - m_srcROI.y*m_yFactor + 0.5*m_dstROI.height*(1. - m_yFactor) + 0.5);
    m_bMovie = TRUE;
    return 1;
}

void CRunResizeSqrPixel::ResetMovieArgs()
{
    CRunResizeShift::ResetMovieArgs();
    m_xShift        = m_xShift_Save     ;
    m_yShift        = m_yShift_Save     ;
    m_bMovie = FALSE;
}
