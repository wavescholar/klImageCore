/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWarp.cpp: implementation of the CRunWarp class.
// CRunWarp class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunWarp.h"
#include "ParmWarpDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunWarp::CRunWarp()
{
    int i, j;
    m_Type   = WARP_QUAD;
    m_interpolate = IPPI_INTER_NN;
    m_NF = -1;
    m_I[NF_AFF] = I_AFF;
    m_I[NF_BIL] = I_BIL;
    m_I[NF_PER] = I_PER;
    m_J[NF_AFF] = J_AFF;
    m_J[NF_BIL] = J_BIL;
    m_J[NF_PER] = J_PER;
    m_BaseName[NF_AFF] = "Affine";
    m_BaseName[NF_BIL] = "Bilinear";
    m_BaseName[NF_PER] = "Perspective";
    for (i=0; i<NF_NUM; i++)
        m_TransName[i] = "ippGet" + m_BaseName[i] + "Transform";
    for (i=0; i<m_I[NF_AFF]; i++)
        for (j=0; j<m_J[NF_AFF]; j++)
            m_coefAff[i][j] = 0.;
    for (i=0; i<m_I[NF_BIL]; i++)
        for (j=0; j<m_J[NF_BIL]; j++)
            m_coefBil[i][j] = 0.;
    for (i=0; i<m_I[NF_PER]; i++)
        for (j=0; j<m_J[NF_PER]; j++)
            m_coefPer[i][j] = 0.;
}

CRunWarp::~CRunWarp()
{

}

CParamDlg* CRunWarp::CreateDlg() { return new CParmWarpDlg;}

void CRunWarp::UpdateData(CParamDlg* pParmDlg, BOOL save)
{
    CippiRun::UpdateData(pParmDlg,save);
    for (m_NF=0; m_NF<NF_NUM; m_NF++) {
        if (m_Func.Find(m_BaseName[m_NF]) >= 0)
            break;
    }
    ASSERT(m_NF < NF_NUM);

    CParmWarpDlg* pDlg = (CParmWarpDlg*)pParmDlg;
    if (save) {
        m_Type = pDlg->m_Type;
        m_interpolate = pDlg->m_interpolate;
    } else {
        pDlg->m_NF = m_NF;
        pDlg->m_I  = m_I[m_NF];
        pDlg->m_J  = m_J[m_NF];
        pDlg->m_TransName = m_TransName[m_NF];

        pDlg->m_Type     = m_Type  ;
        pDlg->m_interpolate = m_interpolate;
    }
    UpdateAff(pDlg, save);
    UpdateBil(pDlg, save);
    UpdatePer(pDlg, save);
}

void CRunWarp::UpdateAff(CParamDlg* pParmDlg, BOOL save)
{
    CParmWarpDlg* pDlg = (CParmWarpDlg*)pParmDlg;
    int nI = m_I[NF_AFF];
    int nJ = m_J[NF_AFF];
    if (save) {
        for (int i=0; i<nI; i++)
            for (int j=0; j<nJ; j++)
                m_coefAff[i][j] = pDlg->m_CoefAff[i][j];
    } else {
        for (int i=0; i<nI; i++)
            for (int j=0; j<nJ; j++)
                pDlg->m_CoefAff[i][j] = m_coefAff[i][j];
    }
}

void CRunWarp::UpdateBil(CParamDlg* pParmDlg, BOOL save)
{
    CParmWarpDlg* pDlg = (CParmWarpDlg*)pParmDlg;
    int nI = m_I[NF_BIL];
    int nJ = m_J[NF_BIL];
    if (save) {
        for (int i=0; i<nI; i++)
            for (int j=0; j<nJ; j++)
                m_coefBil[i][j] = pDlg->m_CoefBil[i][j];
    } else {
        for (int i=0; i<nI; i++)
            for (int j=0; j<nJ; j++)
                pDlg->m_CoefBil[i][j] = m_coefBil[i][j];
    }
}

void CRunWarp::UpdatePer(CParamDlg* pParmDlg, BOOL save)
{
    CParmWarpDlg* pDlg = (CParmWarpDlg*)pParmDlg;
    int nI = m_I[NF_PER];
    int nJ = m_J[NF_PER];
    if (save) {
        for (int i=0; i<nI; i++)
            for (int j=0; j<nJ; j++)
                m_coefPer[i][j] = pDlg->m_CoefPer[i][j];
    } else {
        for (int i=0; i<nI; i++)
            for (int j=0; j<nJ; j++)
                pDlg->m_CoefPer[i][j] = m_coefPer[i][j];
    }
}

ImgHeader CRunWarp::GetNewDstHeader()
{
   ImgHeader header = CippiRun::GetNewDstHeader();
   header.SetSize(m_pDocSrc->GetImage()->Width(),
                  m_pDocSrc->GetImage()->Height());
   return header;
}

void CRunWarp::CopyContoursToNewDst()
{
   CImage* srcImage = m_pDocSrc->GetImage();
   CImage* dstImage = m_pDocDst->GetImage();
   if (m_Type == WARP_ROI && !m_Func.Found("Back")) {
      const IppiRect* pRoi = srcImage->GetRoi();
      if (pRoi) {
         double quad[4][2];
         quad[0][0] = pRoi->x;               quad[0][1] = pRoi->y;
         quad[1][0] = pRoi->x + pRoi->width; quad[1][1] = pRoi->y;
         quad[2][0] = pRoi->x + pRoi->width; quad[2][1] = pRoi->y + pRoi->height;
         quad[3][0] = pRoi->x;               quad[3][1] = pRoi->y + pRoi->height;
         dstImage->SetQuad(quad);
      }
   }
   if (m_Type == WARP_QUAD && !m_Func.Found("Back"))
      dstImage->CopyQuad(srcImage);
   if (m_Type == WARP_PARL && !m_Func.Found("Back"))
      dstImage->CopyParl(srcImage);
}

IppStatus CRunWarp::CallIppFunction()
{
   FUNC_CALL(ippiWarpAffine_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_16u_C4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_16u_AC4R,((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_8u_C4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_8u_AC4R,((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffine_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpAffineBack_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefAff, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspective_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpPerspectiveBack_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefPer, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinear_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_16u_C1R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_16u_C3R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_16u_C4R,  ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_16u_AC4R, ((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp16u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp16u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_8u_C1R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_8u_C3R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_8u_C4R,  ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_8u_AC4R, ((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp8u*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp8u**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_32f_C1R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_32f_C3R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_32f_C4R,  ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_32f_AC4R, ((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)pDstImg, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
   FUNC_CALL(ippiWarpBilinearBack_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f**)pDstImgP, dstStep, m_dstROI, m_coefBil, m_interpolate))
    return stsNoFunction;
}

CString CRunWarp::GetHistoryParms()
{
    return GetInterParm(m_interpolate);
}
