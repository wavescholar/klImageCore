/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWarpq.cpp: implementation of the CRunWarpq class.
// CRunWarpq class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunWarp.h"
#include "RunWarpq.h"
#include "ParmWarpqDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunWarpq::CRunWarpq()
{
     m_SType    = WARP_ROI;
     m_DType    = WARP_QUAD;
     m_interpolate = IPPI_INTER_NN;
     for (int i=0; i<4; i++) {
         for (int j=0; j<2; j++) {
                m_srcQuad[i][j] = 1.;
                m_dstQuad[i][j] = 1.;
         }
     }
}

CRunWarpq::~CRunWarpq()
{

}

CParamDlg* CRunWarpq::CreateDlg() { return new CParmWarpqDlg;}

void CRunWarpq::UpdateData(CParamDlg* pParmDlg, BOOL save)
{
     CippiRun::UpdateData(pParmDlg,save);
     CParmWarpqDlg* pDlg = (CParmWarpqDlg*)pParmDlg;
     if (save) {
          m_SType = pDlg->m_SType;
          m_DType = pDlg->m_DType;
          m_interpolate = GetInterpolate(pDlg->m_Inter,pDlg->m_Smooth);
          for (int i=0; i<4; i++) {
              for (int j=0; j<2; j++) {
                     m_srcQuad[i][j] = pDlg->m_srcQuad[i][j];
                     m_dstQuad[i][j] = pDlg->m_dstQuad[i][j];
              }
          }
     } else {
          pDlg->m_SType      = m_SType  ;
          pDlg->m_DType      = m_DType  ;
          SetInterpolate(m_interpolate,pDlg->m_Inter,pDlg->m_Smooth);
          for (int i=0; i<4; i++) {
              for (int j=0; j<2; j++) {
                     pDlg->m_srcQuad[i][j] = m_srcQuad[i][j];
                     pDlg->m_dstQuad[i][j] = m_dstQuad[i][j];
              }
          }
     }
}

ImgHeader CRunWarpq::GetNewDstHeader()
{
    ImgHeader header = CippiRun::GetNewDstHeader();
    header.SetSize(m_pDocSrc->GetImage()->Width(),
                        m_pDocSrc->GetImage()->Height());
    return header;
}


void CRunWarpq::CopyContoursToNewDst()
{
     CImage* srcImage = m_pDocSrc->GetImage();
     CImage* dstImage = m_pDocDst->GetImage();
     if (m_DType == WARP_QUAD)
          dstImage->CopyQuad(srcImage);
     if (m_DType == WARP_PARL)
          dstImage->CopyParl(srcImage);
}

IppStatus CRunWarpq::CallIppFunction()
{
    FUNC_CALL(ippiWarpAffineQuad_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_16u_C4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_16u_AC4R,((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_8u_C4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_8u_AC4R,((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpAffineQuad_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpPerspectiveQuad_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    FUNC_CALL(ippiWarpBilinearQuad_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, m_srcQuad, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_dstQuad, m_interpolate))
    return stsNoFunction;
}

CString CRunWarpq::GetHistoryParms()
{
     return GetInterParm(m_interpolate);
}

BOOL CRunWarpq::IsMovie(CFunc func)
{
    if (func.BaseName() == "WarpAffineQuad" || func.BaseName() == "WarpPerspectiveQuad" || func.BaseName() == "WarpBilinearQuad")
        return RUNMOVIE_REDRAW | RUNMOVIE_COPYSRC;
    return FALSE;
}

static void copyQuad(Ipp64f src[4][2], Ipp64f dst[4][2])
{
    for (int i=0; i<4; i++)
        for (int j=0; j<2; j++)
            dst[i][j] = src[i][j];
}

static void setRoiQuad(IppiRect roi, Ipp64f quad[4][2])
{
    quad[0][0] = roi.x;
    quad[0][1] = roi.y;
    quad[1][0] = roi.x + roi.width;
    quad[1][1] = roi.y;
    quad[2][0] = roi.x + roi.width;
    quad[2][1] = roi.y + roi.height;
    quad[3][0] = roi.x;
    quad[3][1] = roi.y + roi.height;
}

static void setParlQuad(double p, IppiRect roi, Ipp64f quad[4][2])
{
    double pw = 1./3.;
    double ph = 1./3.;
    double x = p*pw*roi.width;
    double y = p*ph*roi.height;

    quad[0][0] = roi.x;
    quad[0][1] = roi.y;
    quad[1][0] = roi.x + roi.width - x;
    quad[1][1] = roi.y + y;
    quad[2][0] = roi.x + roi.width;
    quad[2][1] = roi.y + roi.height;
    quad[3][0] = roi.x + x;
    quad[3][1] = roi.y + roi.height - y;
}

static void setTrapezQuad(double p, IppiRect roi, Ipp64f quad[4][2])
{
    double pw = 1./3.;
    double ph = 1./2.;
    double x = p*pw*roi.width;
    double y = p*ph*roi.height;

    quad[0][0] = roi.x;
    quad[0][1] = roi.y;
    quad[1][0] = roi.x + roi.width;
    quad[1][1] = roi.y;
    quad[2][0] = roi.x + roi.width - x;
    quad[2][1] = roi.y + roi.height - y;
    quad[3][0] = roi.x + x;
    quad[3][1] = roi.y + roi.height - y;
}

int CRunWarpq::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps)
{
    copyQuad(m_srcQuad, m_srcQuad_Save);
    copyQuad(m_dstQuad, m_dstQuad_Save);
    m_interpolate_Save = m_interpolate;

    double parm = tim < 0 ? 1 : GetMovieValueReverse(tim, 1);
    if (m_Func.Found("Affine"))
        setParlQuad(parm, m_srcROI, m_dstQuad);
    else
        setTrapezQuad(parm, m_srcROI, m_dstQuad);
    setRoiQuad(m_srcROI, m_srcQuad);
    m_interpolate = IPPI_INTER_LINEAR;
    dstInfo << "linear";
    return 1;
}

void CRunWarpq::ResetMovieArgs()
{
    copyQuad(m_srcQuad_Save, m_srcQuad);
    copyQuad(m_dstQuad_Save, m_dstQuad);
    m_interpolate = m_interpolate_Save;
}
