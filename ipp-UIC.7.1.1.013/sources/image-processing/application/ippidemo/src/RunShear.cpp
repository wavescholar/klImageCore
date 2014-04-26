/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunShear.cpp: implementation of the CRunShear class.
// CRunShear class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunShear.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunShear::CRunShear()
{
    m_Type   = GEOM_CONTOUR;
    m_shearX = 0.1;
    m_shearY = 0.1;
    m_xShift = 0;
    m_yShift = 0;
    m_interpolate = IPPI_INTER_NN;

}

CRunShear::~CRunShear()
{

}

void CRunShear::UpdateData(CParamDlg* pDlg, BOOL save)
{
    CippiRun::UpdateData(pDlg,save);
    CParmShearDlg* pShearDlg = (CParmShearDlg*)pDlg;
    if (save) {
        m_Type = pShearDlg->m_Type;
        m_shearX = pShearDlg->m_Shear[0];
        m_shearY = pShearDlg->m_Shear[1];
        m_xShift = pShearDlg->m_Shift[0];
        m_yShift = pShearDlg->m_Shift[1];
        m_interpolate = pShearDlg->m_interpolate;
    } else {
        pShearDlg->m_Type     = m_Type  ;
        pShearDlg->m_Shear[0] = m_shearX;
        pShearDlg->m_Shear[1] = m_shearY;
        pShearDlg->m_Shift[0] = m_xShift;
        pShearDlg->m_Shift[1] = m_yShift;
        pShearDlg->m_interpolate = m_interpolate;
    }
}

void CRunShear::CopyContoursToNewDst()
{
    CImage* srcImage = m_pDocSrc->GetImage();
    CImage* dstImage = m_pDocDst->GetImage();
    if (m_Type == GEOM_CONTOUR)
        dstImage->CopyCenterShift(srcImage);
}

IppStatus CRunShear::CallIppFunction()
{
   FUNC_CALL(ippiShear_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_16u_C4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_8u_C4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_32f_C4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiShear_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_shearX, m_shearY, m_xShift, m_yShift, m_interpolate))
    return stsNoFunction;
}

CString CRunShear::GetHistoryParms()
{
    CString parms;
    parms.Format("%.2f, %.2f, %.1f, %.1f, %s",
        m_shearX, m_shearY, m_xShift, m_yShift, GetInterParm(m_interpolate));
    return parms;
}
