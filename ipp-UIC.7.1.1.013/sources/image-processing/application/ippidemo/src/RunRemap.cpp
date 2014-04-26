/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRemap.cpp: implementation of the CRunRemap class.
// CRunRemap class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunRemap.h"
#include "Mapping.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRemap::CRunRemap()
{
     m_Type    = MOD_EXPLODE;
     m_interpolate = IPPI_INTER_NN;
     m_ShowMaps = FALSE;

     m_pDocXMap = NULL;
     m_pDocYMap = NULL;
     m_pImgXMap = NULL;
     m_pImgYMap = NULL;
     m_xMap = NULL;
     m_yMap = NULL;
     m_xMapStep = 0;
     m_yMapStep = 0;

     m_MapPtrArray.Create();
}

CRunRemap::~CRunRemap()
{
     DeleteMaps();
}

IppiRect CRunRemap::GetDstRoi()
{
    IppiRect srcRoi, dstRoi;
    ImgHeader srcHeader = m_pDocSrc->GetImage()->GetHeader();
    srcRoi = m_pDocSrc->GetImage()->GetActualRoi();
    if (m_pDocDst) {
         dstRoi = m_pDocDst->GetImage()->GetActualRoi();
    } else {
        ImgHeader dstHeader = GetNewDstHeader();
        if (dstHeader.width  == srcHeader.width &&
             dstHeader.height == srcHeader.height) {
            dstRoi = srcRoi;
        } else {
            dstRoi.x = dstRoi.y = 0;
            dstRoi.width  = dstHeader.width;
            dstRoi.height = dstHeader.height;
        }
    }
    if (dstRoi.width > srcRoi.width) dstRoi.width = srcRoi.width;
    if (dstRoi.height > srcRoi.height) dstRoi.height = srcRoi.height;
    return dstRoi;
}

CImage* CRunRemap::Create32fC1(ImgHeader header, IppiRect roi)
{
     CImage* dstImg = new CImage;
     if (!dstImg) return NULL;
     header.type = pp32f;
     header.channels = 1;
     header.SetSize(roi.width, roi.height);
     dstImg->SetHeader(header);
     dstImg->AllocateData();
     return dstImg;
}

void CRunRemap::Delete32fC1(CImage* img)
{
     delete img;
}

CImage* CRunRemap::CreateMapImage(BOOL bY)
{
     IppiRect dstRoi = GetDstRoi();
     CImage* mapImg = Create32fC1(m_pDocSrc->GetImage()->GetHeader(), dstRoi);
     if (!mapImg) return NULL;
     m_MapPtrArray[m_Type]->SetDocs(m_pDocSrc, m_pDocDst);
     if (!m_MapPtrArray[m_Type]->SetData(mapImg,bY)) {
          Delete32fC1(mapImg);
          return NULL;
     }
     return mapImg;
}

void CRunRemap::DeleteMapImage(CImage* img)
{
     Delete32fC1(img);
}

void CRunRemap::DeleteMaps()
{
     if (!m_pDocXMap) DeleteMapImage(m_pImgXMap);
     m_pImgXMap = NULL;
     if (!m_pDocYMap) DeleteMapImage(m_pImgYMap);
     m_pImgYMap = NULL;
}

BOOL CRunRemap::PrepareMaps()
{
     DeleteMaps();
     m_pDocXMap = NULL;
     m_pDocYMap = NULL;
     m_pImgXMap = CreateMapImage(0);
     m_pImgYMap = CreateMapImage(1);
     if (!m_pImgXMap || !m_pImgYMap) {
          DeleteMaps();
          return FALSE;
     }
     if (!m_ShowMaps) return TRUE;
     m_pDocXMap = CreateNewDoc(m_pImgXMap, "xMap");
     if (!m_pDocXMap) return FALSE;
     m_pDocYMap = CreateNewDoc(m_pImgYMap, "yMap");
     if (!m_pDocYMap) return FALSE;
     DeleteMapImage(m_pImgXMap); m_pImgXMap = m_pDocXMap;
     DeleteMapImage(m_pImgYMap); m_pImgYMap = m_pDocYMap;
     return TRUE;
}

void CRunRemap::SetMapsTitle()
{
     if (!m_pDocXMap || !m_pDocYMap) return;
     CString title = "Map (" +
          m_pDocSrc->GetTitle() + " - " +
          m_pDocDst->GetTitle() + ")";
     m_pDocXMap->SetTitle("x" + title);
     m_pDocYMap->SetTitle("y" + title);
}

static void SetMapParms(CImage* pImg, float*& ptr, int& step)
{
     ASSERT(pImg);
     ptr = (float*)pImg->GetRoiPtr();
     step = pImg->Step();
}

void CRunRemap::PrepareMapsParms()
{
     SetMapParms(m_pImgXMap, m_xMap, m_xMapStep);
     SetMapParms(m_pImgYMap, m_yMap, m_yMapStep);
}

BOOL CRunRemap::PrepareDst()
{
     if (!PrepareMaps()) return FALSE;
     if (!CippiRun::PrepareDst()) return FALSE;
     SetMapsTitle();
     PrepareMapsParms();
     return TRUE;
}

void CRunRemap::ActivateDst()
{
    ActivateDoc(m_pDocSrc);
    ActivateDoc(m_pDocXMap);
    ActivateDoc(m_pDocYMap);
    ActivateDoc(m_pDocDst);
}

BOOL CRunRemap::AfterCall(BOOL bOK)
{
     if (m_pDocXMap) {
          m_pDocXMap->UpdateView();
     }
     if (m_pDocYMap) {
          m_pDocYMap->UpdateView();
     }
     return TRUE;
}

void CRunRemap::UpdateData(CParamDlg* pDlg, BOOL save)
{
     CippiRun::UpdateData(pDlg,save);
     CParmRemapDlg* pRemapDlg = (CParmRemapDlg*)pDlg;
     if (save) {
          m_ShowMaps = pRemapDlg->m_ShowMaps;
          m_Type = pRemapDlg->m_Type;
          m_interpolate = pRemapDlg->m_interpolate;
     } else {
          pRemapDlg->Init(&m_MapPtrArray);
          pRemapDlg->m_ShowMaps  = m_ShowMaps  ;
          pRemapDlg->m_Type  = m_Type  ;
          pRemapDlg->m_interpolate = m_interpolate;
     }
}

void CRunRemap::CopyContoursToNewDst()
{
     CImage* srcImage = m_pDocSrc->GetImage();
     CImage* dstImage = m_pDocDst->GetImage();
     //if (m_Type == MOD_R_TO_Q)
     //     dstImage->CopyQuad(srcImage);
     //else
         CippiRun::CopyContoursToNewDst();
}

IppStatus CRunRemap::CallIppFunction()
{
    FUNC_CALL(ippiRemap_8u_C1R ,((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp8u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_8u_C3R ,((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp8u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_8u_C4R ,((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp8u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_8u_AC4R    ,((Ipp8u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp8u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_8u_P3R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp8u**)pDstP, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_8u_P4R, ((Ipp8u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp8u**)pDstP, dstStep, m_dstRoiSize, m_interpolate))

    FUNC_CALL(ippiRemap_16u_C1R ,((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16u_C3R ,((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16u_C4R ,((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16u_AC4R    ,((Ipp16u*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16u*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16u_P3R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16u**)pDstP, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16u_P4R, ((Ipp16u**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16u**)pDstP, dstStep, m_dstRoiSize, m_interpolate))

    FUNC_CALL(ippiRemap_16s_C1R ,((Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16s*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16s_C3R ,((Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16s*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16s_C4R ,((Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16s*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16s_AC4R    ,((Ipp16s*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16s*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16s_P3R, ((Ipp16s**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16s**)pDstP, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_16s_P4R, ((Ipp16s**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp16s**)pDstP, dstStep, m_dstRoiSize, m_interpolate))

    FUNC_CALL(ippiRemap_32f_C1R    ,((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp32f*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_32f_C3R    ,((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp32f*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_32f_C4R,((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp32f*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_32f_AC4R,((Ipp32f*)pSrcImg, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp32f*)pDst, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_32f_P3R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp32f**)pDstP, dstStep, m_dstRoiSize, m_interpolate))
    FUNC_CALL(ippiRemap_32f_P4R, ((Ipp32f**)pSrcImgP, m_srcSize, srcStep, m_srcROI, (Ipp32f*)m_xMap, m_xMapStep, (Ipp32f*)m_yMap, m_yMapStep, (Ipp32f**)pDstP, dstStep, m_dstRoiSize, m_interpolate))
     return stsNoFunction;
}

CString CRunRemap::GetHistoryParms()
{
     CString parms;
     parms.Format("%s", GetInterParm(m_interpolate));
     return parms;
}

CString CRunRemap::GetHistoryInfo()
{
     CString info = m_MapPtrArray[m_Type]->GetInfo();
     return info;
}
