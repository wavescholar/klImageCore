/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFlood.cpp: implementation of the CRunCVPyr class.
// CRunCVPyr class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCVPyr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVPyr::CRunCVPyr()
{
   m_pBuffer = NULL;
}

CRunCVPyr::~CRunCVPyr()
{

}

ImgHeader CRunCVPyr::GetNewDstHeader()
{
   ImgHeader header;
   header = m_pDocSrc->GetImage()->GetHeader();
   IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
   if (m_Func.Found("Up")) {
      header.SetSize(roi.width  << 1, roi.height << 1);
   } else {
      header.SetSize(roi.width  >> 1, roi.height >> 1);
   }
   return header;
}

BOOL CRunCVPyr::PrepareSrc()
{
   if (m_Func.Found("Up")) return TRUE;
   IppiRect roi = m_pDocSrc->GetImage()->GetActualRoi();
   BOOL bUpdate = FALSE;
   if (roi.width & 1) {
      bUpdate = TRUE;
      roi.width --;
   }
   if (roi.height & 1) {
      bUpdate = TRUE;
      roi.height --;
   }
   if (!bUpdate) return TRUE;
   if (DEMO_APP->MessageBox(
      (m_pDocSrc->GetImage()->GetRoi() ?
       " ROI will be updated in " :
       " ROI will be created in ") +
      m_pDocSrc->GetTitle(),
      MB_OKCANCEL) != IDOK) return FALSE;
   m_pDocSrc->GetImage()->SetRoi(&roi);
   return TRUE;
}

BOOL CRunCVPyr::BeforeCall()
{
   IppDataType dataType = m_Func.TypeToIpp(m_Func.SrcType());
   int channels = m_Func.SrcChannels();
   int bufferSize;
   IppStatus status;
   if (m_Func.Found("Up")) {
      status = CV_CALL(ippiPyrUpGetBufSize_Gauss5x5  , (m_srcRoiSize.width, dataType, channels, &bufferSize));
      IppErrorMessage("ippiPyrUpGetBufSize_Gauss5x5", status);
   } else {
      status = CV_CALL(ippiPyrDownGetBufSize_Gauss5x5, (m_srcRoiSize.width, dataType, channels, &bufferSize));
      IppErrorMessage("ippiPyrDownGetBufSize_Gauss5x5", status);
   }
   if (status < 0) return FALSE;
   if (bufferSize) m_pBuffer = (Ipp8u*)ippMalloc(bufferSize);
   return TRUE;
}

BOOL CRunCVPyr::AfterCall(BOOL bOK)
{
   ippFree(m_pBuffer); m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunCVPyr::CallIppFunction()
{
   FUNC_CV_CALL(ippiPyrUp_Gauss5x5_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrUp_Gauss5x5_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrUp_Gauss5x5_8s_C1R, ((Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrUp_Gauss5x5_8s_C3R, ((Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrUp_Gauss5x5_32f_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrUp_Gauss5x5_32f_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrDown_Gauss5x5_8u_C1R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrDown_Gauss5x5_8u_C3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrDown_Gauss5x5_8s_C1R, ((Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrDown_Gauss5x5_8s_C3R, ((Ipp8s*)pSrc, srcStep, (Ipp8s*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrDown_Gauss5x5_32f_C1R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   FUNC_CV_CALL(ippiPyrDown_Gauss5x5_32f_C3R, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, m_srcRoiSize, m_pBuffer ))
   return stsNoFunction;
}
