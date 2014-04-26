/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRotate.cpp: implementation of the CRunRotate class.
// CRunRotate class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunRotate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRotate::CRunRotate()
{
    m_Type   = GEOM_CONTOUR;
    m_angle  = 30.;
    m_xShift = 0;
    m_yShift = 0;
    m_interpolate = IPPI_INTER_NN;
}

CRunRotate::~CRunRotate()
{

}

void CRunRotate::UpdateData(CParamDlg* pDlg, BOOL save)
{
    CippiRun::UpdateData(pDlg,save);
    CParmRotateDlg* pRotateDlg = (CParmRotateDlg*)pDlg;
    if (save) {
        m_Type = pRotateDlg->m_Type;
        m_angle = pRotateDlg->m_Angle;
        m_xShift = pRotateDlg->m_Shift[0];
        m_yShift = pRotateDlg->m_Shift[1];
        m_interpolate = pRotateDlg->m_interpolate;
    } else {
        pRotateDlg->m_Type     = m_Type  ;
        pRotateDlg->m_Angle    = m_angle ;
        pRotateDlg->m_Shift[0] = m_xShift;
        pRotateDlg->m_Shift[1] = m_yShift;
        pRotateDlg->m_interpolate = m_interpolate;
    }
}

ImgHeader CRunRotate::GetNewDstHeader()
{
   ImgHeader header = CippiRun::GetNewDstHeader();
   header.SetSize(m_pDocSrc->GetImage()->Width(),
                  m_pDocSrc->GetImage()->Height());
   return header;
}

void CRunRotate::CopyContoursToNewDst()
{
    if (m_Type == GEOM_CONTOUR)
        m_pDocDst->GetImage()->CopyCenterShift(m_pDocSrc->GetImage());
}

IppStatus CRunRotate::CallIppFunction()
{
   FUNC_CALL(ippiRotate_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_8u_C4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_16u_C4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_32f_C4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotate_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))

   FUNC_CALL(ippiRotateCenter_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_8u_C4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_16u_C4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_32f_C4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
   FUNC_CALL(ippiRotateCenter_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_angle, m_xShift, m_yShift, m_interpolate))
  return stsNoFunction;
}

CString CRunRotate::GetHistoryParms()
{
    CString parms;
    parms.Format("%.1f, %.1f, %.1f, %s",
        m_angle, m_xShift, m_yShift, GetInterParm(m_interpolate));
    return parms;
}

BOOL CRunRotate::IsMovie(CFunc func)
{
   if (func.BaseName() == "Rotate")
      return RUNMOVIE_REDRAW | RUNMOVIE_COPYSRC;
   return FALSE;
}

int CRunRotate::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                             int& numReps)
{
   m_angle_Save       = m_angle      ;
   m_xShift_Save      = m_xShift     ;
   m_yShift_Save      = m_yShift     ;
   m_interpolate_Save = m_interpolate;

   if (tim < 0) {
      m_angle = 30;
   } else {
      m_angle = GetMovieValue(tim, 360);
   }
   if (m_Func.Found("Center")) {
      m_xShift = m_srcROI.x + (m_srcROI.width  >> 1);
      m_yShift = m_srcROI.y + (m_srcROI.height >> 1);
   } else {
      m_xShift = 0;
      m_yShift = 0;
      ippiAddRotateShift(
         m_srcROI.x + (m_srcROI.width  >> 1),
         m_srcROI.y + (m_srcROI.height >> 1),
         m_angle, &m_xShift, &m_yShift);
   }
   m_interpolate = IPPI_INTER_LINEAR;
   dstInfo << "linear";
   return 1;
}

void CRunRotate::ResetMovieArgs()
{
   m_angle       = m_angle_Save      ;
   m_xShift      = m_xShift_Save     ;
   m_yShift      = m_yShift_Save     ;
   m_interpolate = m_interpolate_Save;
}
