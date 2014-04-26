/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTranspose.cpp: implementation of the CRunTranspose class.
// CRunTranspose class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunTranspose.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunTranspose::CRunTranspose()
{
}

CRunTranspose::~CRunTranspose()
{

}

ImgHeader CRunTranspose::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
    header.SetSize(roi.height, roi.width);
    return header;
}

BOOL CRunTranspose::PrepareSrc()
{
   CImage* pSrcImage = m_pDocSrc->GetImage();
   IppiRect srcRoi = pSrcImage->GetActualRoi();
   BOOL bSrcRoi = pSrcImage->IsRoi();
   if (m_Func.Inplace()) {
      if (srcRoi.width != srcRoi.height) {
         if (srcRoi.width > srcRoi.height)
            srcRoi.width = srcRoi.height;
         else
            srcRoi.height = srcRoi.width;
         CString message =
            (bSrcRoi ? "ROI will be reduced to square in " :
                       "Square ROI will be created in ")
            + m_pDocSrc->GetTitle() + " image";
         if (DEMO_APP->MessageBox(message, MB_OKCANCEL) == IDOK) {
            pSrcImage->SetRoi(&srcRoi);
         } else {
            return FALSE;
         }
      }
   } else if (m_pDocDst) {
      IppiRect dstRoi = m_pDocDst->GetImage()->GetActualRoi();
      if (srcRoi.width > dstRoi.height || srcRoi.height > dstRoi.width) {
         if (srcRoi.width > dstRoi.height)
            srcRoi.width = dstRoi.height;
         if (srcRoi.height > dstRoi.width)
            srcRoi.height = dstRoi.width;
         if (!SetReducedRoi(m_pDocSrc, srcRoi))
             return false;
      }
   }
   return TRUE;
}

IppStatus CRunTranspose::CallIppFunction()
{
   FUNC_CALL (  ippiTranspose_8u_C1R, ( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_8u_C3R, ( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_8u_C4R, ( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_8u_C1IR, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_8u_C3IR, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_8u_C4IR, ( (Ipp8u*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16u_C1R, ( (const Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16u_C3R, ( (const Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16u_C4R, ( (const Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16u_C1IR, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16u_C3IR, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16u_C4IR, ( (Ipp16u*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32s_C1R, ( (const Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32s_C3R, ( (const Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32s_C4R, ( (const Ipp32s*)pSrc, srcStep, (Ipp32s*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32s_C1IR, ( (Ipp32s*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32s_C3IR, ( (Ipp32s*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32s_C4IR, ( (Ipp32s*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16s_C1R, ( (const Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16s_C3R, ( (const Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16s_C4R, ( (const Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16s_C1IR, ( (Ipp16s*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16s_C3IR, ( (Ipp16s*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_16s_C4IR, ( (Ipp16s*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32f_C1R, ( (const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32f_C3R, ( (const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32f_C4R, ( (const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32f_C1IR, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32f_C3IR, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize ))
   FUNC_CALL (  ippiTranspose_32f_C4IR, ( (Ipp32f*)pSrc, srcStep, m_srcRoiSize ))

   return stsNoFunction;
}
