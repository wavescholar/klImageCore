/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResize.cpp: implementation of the CRunResize class.
// CRunResize class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Movie.h"
#include "RunResize.h"
#include "ParmResizeDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunResize::CRunResize()
{
    m_Type   = GEOM_CONTOUR;
    m_xFactor = 0.5;
    m_yFactor = 0.5;
    m_interpolate = IPPI_INTER_NN;
}

CRunResize::~CRunResize()
{

}

CParamDlg* CRunResize::CreateDlg() { return new CParmResizeDlg;}

void CRunResize::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmResizeDlg* pDlg = (CParmResizeDlg*)parmDlg;
    if (save) {
        m_Type = pDlg->m_Type;
        m_xFactor = pDlg->m_Fract[0];
        m_yFactor = pDlg->m_Fract[1];
        m_interpolate = GetInterpolate(pDlg->m_Inter);
    } else {
        pDlg->m_Type     = m_Type  ;
        pDlg->m_Fract[0] = m_xFactor;
        pDlg->m_Fract[1] = m_yFactor;
        SetInterpolate(m_interpolate, pDlg->m_Inter);
    }
}

ImgHeader CRunResize::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    header.SetSize((int)(roi.width * m_xFactor + 0.5),
                   (int)(roi.height* m_yFactor + 0.5));
    return header;
}

void CRunResize::CopyContoursToNewDst()
{
}

IppStatus CRunResize::CallIppFunction()
{
   return stsNoFunction;
}

void CRunResize::SetIntFract(double fract, int& up, int& low)
{
    double minDelta = 0;
    for (int i=1; i<1000; i++) {
        int j = (int)(fract*(double)i + 0.5);
        double delta = (double)j / (double)i - fract;
        if (delta < 0) delta = -delta;
        if (i == 1 || minDelta > delta) {
            low = i;
            up = j;
            minDelta = delta;
        }
        if (minDelta < 1.e-6) break;
    }
}

CString CRunResize::GetHistoryParms()
{
    CString parms;
    parms.Format("%.2f, %.2f, %s",
        m_xFactor, m_yFactor, GetInterParm(m_interpolate));
    return parms;
}

double CRunResize::GetPerfDivisor()
{
   IppiRect srcRoi = m_pDocSrc->GetImage()->GetActualRoi();
   IppiRect dstRoi = m_pDocDst->GetImage()->GetActualRoi();
   srcRoi.width  = (int)((double)srcRoi.width *m_xFactor);
   srcRoi.height = (int)((double)srcRoi.height*m_yFactor);
   if (dstRoi.width > srcRoi.width) dstRoi.width = srcRoi.width;
   if (dstRoi.height > srcRoi.height) dstRoi.height = srcRoi.height;
   return dstRoi.width*dstRoi.height;
}

BOOL CRunResize::IsMovie(CFunc func)
{
   return RUNMOVIE_REDRAW | RUNMOVIE_COPYSRC;
}

void CRunResize::PrepareMovieParameters(CImage* pImgSrc, CImage* pImgDst)
{
   IppiRect srcRoiSave;
   IppiRect dstRoiSave;
   IppiRect* pSrcRoi = CMovie::SaveRoi(pImgSrc, srcRoiSave);
   IppiRect* pDstRoi = CMovie::SaveRoi(pImgDst, dstRoiSave);

   CMovie::SetZoomRoi(pImgSrc,pImgDst,m_xFactor);
   roiSize.width = pImgSrc->GetActualRoi().width;
   roiSize.height = pImgSrc->GetActualRoi().height;
   SetImageParameters(pImgSrc, pSrcImg, pSrc, srcStep,
                      pSrcImgP, pSrcP, srcStepP,
                      m_srcROI, m_srcRoiSize, m_srcSize);
   SetImageParameters(pImgDst, pDstImg, pDst, dstStep,
                      pDstImgP, pDstP, dstStepP,
                      m_dstROI, m_dstRoiSize, m_dstSize);

   pImgSrc->SetRoi(pSrcRoi);
   pImgDst->SetRoi(pDstRoi);
}

int CRunResize::SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                             int& numReps)
{
   m_xFactor_Save     = m_xFactor    ;
   m_yFactor_Save     = m_yFactor    ;
   m_interpolate_Save = m_interpolate;
   if (tim < 0) {
      m_xFactor     = 1.5;
      m_yFactor     = 1.5;
   } else {
      double val = GetMovieValue(tim, 4);
      if (val < 1)
         val = val;
      else if (val < 3)
         val = 2 - val;
      else
         val = val - 4;
      val *= 2;
      m_xFactor     = pow((double)2, (double)val);
      m_yFactor     = m_xFactor;
   }
   if (srcInfo == "Super")
   {
      m_interpolate = IPPI_INTER_SUPER;
      dstInfo << "";
   }
   else if (srcInfo == "Cubic")
   {
      m_interpolate = IPPI_INTER_CUBIC;
      dstInfo << "cubic";
   }
   else if (srcInfo == "Lanczos")
   {
      m_interpolate = IPPI_INTER_LANCZOS;
      dstInfo << "lanczos";
   }
   else if (srcInfo == "NN")
   {
      m_interpolate = IPPI_INTER_NN;
      dstInfo << "nn";
   }
   else
   {
      m_interpolate = IPPI_INTER_LINEAR;
      dstInfo << "linear";
   }
   return 1;
}

void CRunResize::ResetMovieArgs()
{
   m_xFactor     = m_xFactor_Save    ;
   m_yFactor     = m_yFactor_Save    ;
   m_interpolate = m_interpolate_Save;
}
