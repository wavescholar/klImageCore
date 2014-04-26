/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCorr.cpp: implementation of the CRunCorr class.
// CRunCorr class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunCorr.h"
#include "ParmImg3Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCorr::CRunCorr()
{
}

CRunCorr::~CRunCorr()
{

}

BOOL CRunCorr::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

CParamDlg* CRunCorr::CreateDlg() { return new CParmImg3Dlg;}

ImgHeader CRunCorr::GetNewDstHeader()
{
   ImgHeader header = CippiRun::GetNewDstHeader();
   IppiRect roi1 = m_pDocSrc->GetImage()->GetActualRoi();
   IppiRect roi2 = m_pDocSrc2->GetImage()->GetActualRoi();
   if (m_Func.Found("Full")) {
      header.SetSize(roi1.width  + roi2.width  - 1,
                     roi1.height + roi2.height - 1);
   } else if (m_Func.Found("Valid")) {
      header.SetSize(roi1.width  - roi2.width  + 1,
                     roi1.height - roi2.height + 1);
   } else if (m_Func.Found("Same")) {
      header.SetSize(roi1.width, roi1.height);
   }
   return header;
}

BOOL CRunCorr::PrepareSrc()
{
   IppiRect roi1 = m_pDocSrc->GetImage()->GetActualRoi();
   IppiRect roi2 = m_pDocSrc2->GetImage()->GetActualRoi();
   if ((roi2.width <= roi1.width) &&
       (roi2.height <= roi1.height)) return TRUE;
   if (roi2.width > roi1.width)
      roi2.width = roi1.width;
   if (roi2.height > roi1.height)
      roi2.height = roi1.height;
   if (DEMO_APP->MessageBox(
      (m_pDocSrc2->GetImage()->GetRoi() ?
       " ROI will be updated in " :
       " ROI will be created in ") +
      m_pDocSrc2->GetTitle(),
      MB_OKCANCEL) != IDOK) return FALSE;
   m_pDocSrc2->GetImage()->SetRoi(&roi2);
   return TRUE;
}

void CRunCorr::CopyContoursToNewDst()
{
}

IppStatus CRunCorr::CallIppFunction()
{
   FUNC_CALL(ippiCrossCorrFull_Norm_32f_C1R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_32f_C3R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_16u32f_C1R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_16u32f_C3R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_16u32f_AC4R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u32f_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u32f_C3R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u32f_AC4R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u_C1RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u_C3RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiCrossCorrValid_Norm_32f_C1R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_32f_C3R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_16u32f_C1R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_16u32f_C3R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_16u32f_AC4R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u32f_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u32f_C3R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u32f_AC4R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u_C1RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u_C3RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

    FUNC_CALL( ippiCrossCorrValid_32f_C1R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
    FUNC_CALL( ippiCrossCorrValid_8u32f_C1R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
    FUNC_CALL( ippiCrossCorrValid_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize, (Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
    FUNC_CALL( ippiCrossCorrValid_16u32f_C1R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))

   FUNC_CALL(ippiCrossCorrSame_Norm_32f_C1R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_32f_C3R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_16u32f_C1R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_16u32f_C3R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_16u32f_AC4R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u32f_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u32f_C3R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u32f_AC4R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u_C1RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u_C3RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))


   FUNC_CALL(ippiSqrDistanceFull_Norm_32f_C1R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_32f_C3R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_16u32f_C1R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_16u32f_C3R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_16u32f_AC4R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u32f_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u32f_C3R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u32f_AC4R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u_C1RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u_C3RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_32f_C1R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_32f_C3R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_16u32f_C1R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_16u32f_C3R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_16u32f_AC4R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u32f_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u32f_C3R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u32f_AC4R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u_C1RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u_C3RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_32f_C1R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_32f_C3R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_16u32f_C1R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_16u32f_C3R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_16u32f_AC4R, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u_C1RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u_C3RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u_AC4RSfs, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u32f_C1R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u32f_C3R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u32f_AC4R, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))


   FUNC_CALL(ippiCrossCorrFull_Norm_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_Norm_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiCrossCorrValid_Norm_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_Norm_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiCrossCorrSame_Norm_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_Norm_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))


   FUNC_CALL(ippiCrossCorrFull_NormLevel_32f_C1R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_32f_C3R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_32f_AC4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_16u32f_C1R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_16u32f_C3R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_16u32f_AC4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u32f_C1R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u32f_C3R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u32f_AC4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u_C1RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u_C3RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrFull_NormLevel_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiCrossCorrValid_NormLevel_32f_C1R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_32f_C3R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_32f_AC4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_16u32f_C1R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_16u32f_C3R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_16u32f_AC4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u32f_C1R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u32f_C3R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u32f_AC4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u_C1RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u_C3RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrValid_NormLevel_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiCrossCorrSame_NormLevel_32f_C1R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_32f_C3R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_32f_AC4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_16u32f_C1R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_16u32f_C3R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_16u32f_AC4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u32f_C1R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u32f_C3R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u32f_AC4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u_C1RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u_C3RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))
   FUNC_CALL(ippiCrossCorrSame_NormLevel_8u_AC4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiSqrDistanceFull_Norm_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceFull_Norm_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiSqrDistanceValid_Norm_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceValid_Norm_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))

   FUNC_CALL(ippiSqrDistanceSame_Norm_32f_C4R, ((Ipp32f*)pSrc, srcStep, m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_16u32f_C4R, ((Ipp16u*)pSrc, srcStep, m_srcRoiSize,(Ipp16u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u32f_C4R, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8s32f_C1R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8s32f_C3R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8s32f_C4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8s32f_AC4R, ((Ipp8s*)pSrc, srcStep, m_srcRoiSize,(Ipp8s*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiSqrDistanceSame_Norm_8u_C4RSfs, ((Ipp8u*)pSrc, srcStep, m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2, (Ipp8u*)pDst, dstStep, scaleFactor ))


   return stsNoFunction;
}
