/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVPyrLayer.cpp: implementation of the CRunCVPyrLayer class.
// CRunCVPyrLayer class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCVPyrLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVPyrLayer::CRunCVPyrLayer()
{
   m_pState = NULL;
   m_rate = 2;
//   m_pKernel;
   m_kerSize = 3;
   m_mode = IPPI_INTER_LINEAR;
}

CRunCVPyrLayer::~CRunCVPyrLayer()
{

}

BOOL CRunCVPyrLayer::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   CString startStr = m_Func.Prefix() + m_Func.BaseName();
   CString endStr = "_" + m_Func.TypeName() + "_" + m_Func.DescrName();
   m_initName = startStr + "InitAlloc" + endStr;
   m_freeName = startStr + "Free" + endStr;

   return TRUE;
}

ImgHeader CRunCVPyrLayer::GetNewDstHeader()
{
//IPPAPI(IppStatus, ippiGetPyramidDownROI,(IppiSize m_srcRoiSize, IppiSize *pDstRoi, Ipp32f rate))
//IPPAPI(IppStatus, ippiGetPyramidUpROI,(IppiSize m_srcRoiSize, IppiSize *pDstRoiMin, IppiSize *pDstRoiMax, Ipp32f rate))
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

BOOL CRunCVPyrLayer::PrepareSrc()
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

BOOL CRunCVPyrLayer::BeforeCall()
{
   IppStatus status = CallInit(m_initName);
   IppErrorMessage(m_initName, status);
   if (status < 0) return FALSE;
   return TRUE;
}

BOOL CRunCVPyrLayer::AfterCall(BOOL bOK)
{
   IppStatus status = CallFree(m_freeName); m_pState = NULL;
   IppErrorMessage(m_freeName, status);
   return TRUE;
}

IppStatus CRunCVPyrLayer::CallInit(CString name)
{
   MATE_CALL(name,ippiPyramidLayerDownInitAlloc_8u_C1R, ((IppiPyramidDownState_8u_C1R**)&m_pState, m_srcRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerDownInitAlloc_16u_C1R,((IppiPyramidDownState_16u_C1R**)&m_pState, m_srcRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerDownInitAlloc_32f_C1R,((IppiPyramidDownState_32f_C1R**)&m_pState, m_srcRoiSize, m_rate, (Ipp32f*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerDownInitAlloc_8u_C3R, ((IppiPyramidDownState_8u_C3R**)&m_pState, m_srcRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerDownInitAlloc_16u_C3R,((IppiPyramidDownState_16u_C3R**)&m_pState, m_srcRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerDownInitAlloc_32f_C3R,((IppiPyramidDownState_32f_C3R**)&m_pState, m_srcRoiSize, m_rate, (Ipp32f*)m_pKernel, m_kerSize, m_mode))

   MATE_CALL(name,ippiPyramidLayerUpInitAlloc_8u_C1R, ((IppiPyramidUpState_8u_C1R**)&m_pState, m_dstRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerUpInitAlloc_16u_C1R,((IppiPyramidUpState_16u_C1R**)&m_pState, m_dstRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerUpInitAlloc_32f_C1R,((IppiPyramidUpState_32f_C1R**)&m_pState, m_dstRoiSize, m_rate, (Ipp32f*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerUpInitAlloc_8u_C3R, ((IppiPyramidUpState_8u_C3R**)&m_pState, m_dstRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerUpInitAlloc_16u_C3R,((IppiPyramidUpState_16u_C3R**)&m_pState, m_dstRoiSize, m_rate, (Ipp16s*)m_pKernel, m_kerSize, m_mode))
   MATE_CALL(name,ippiPyramidLayerUpInitAlloc_32f_C3R,((IppiPyramidUpState_32f_C3R**)&m_pState, m_dstRoiSize, m_rate, (Ipp32f*)m_pKernel, m_kerSize, m_mode))

   return stsNoFunction;
}

IppStatus CRunCVPyrLayer::CallFree(CString name)
{
   MATE_CALL(name,ippiPyramidLayerDownFree_8u_C1R, ((IppiPyramidDownState_8u_C1R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerDownFree_16u_C1R,((IppiPyramidDownState_16u_C1R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerDownFree_32f_C1R,((IppiPyramidDownState_32f_C1R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerDownFree_8u_C3R, ((IppiPyramidDownState_8u_C3R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerDownFree_16u_C3R,((IppiPyramidDownState_16u_C3R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerDownFree_32f_C3R,((IppiPyramidDownState_32f_C3R*)m_pState))

   MATE_CALL(name,ippiPyramidLayerUpFree_8u_C1R, ((IppiPyramidUpState_8u_C1R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerUpFree_16u_C1R,((IppiPyramidUpState_16u_C1R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerUpFree_32f_C1R,((IppiPyramidUpState_32f_C1R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerUpFree_8u_C3R, ((IppiPyramidUpState_8u_C3R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerUpFree_16u_C3R,((IppiPyramidUpState_16u_C3R*)m_pState))
   MATE_CALL(name,ippiPyramidLayerUpFree_32f_C3R,((IppiPyramidUpState_32f_C3R*)m_pState))
   return stsNoFunction;
}

IppStatus CRunCVPyrLayer::CallIppFunction()
{
   FUNC_CV_CALL( ippiPyramidLayerDown_8u_C1R, ((const Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidDownState_8u_C1R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerDown_8u_C3R, ((const Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidDownState_8u_C3R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerDown_16u_C1R,((const Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidDownState_16u_C1R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerDown_16u_C3R,((const Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidDownState_16u_C3R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerDown_32f_C1R,((const Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (IppiPyramidDownState_32f_C1R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerDown_32f_C3R,((const Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (IppiPyramidDownState_32f_C3R*)m_pState))

   FUNC_CV_CALL( ippiPyramidLayerUp_8u_C1R, ((const Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidUpState_8u_C1R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerUp_8u_C3R, ((const Ipp8u*)pSrc, srcStep, m_srcRoiSize, (Ipp8u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidUpState_8u_C3R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerUp_16u_C1R,((const Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidUpState_16u_C1R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerUp_16u_C3R,((const Ipp16u*)pSrc, srcStep, m_srcRoiSize, (Ipp16u*)pDst, dstStep, m_dstRoiSize, (IppiPyramidUpState_16u_C3R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerUp_32f_C1R,((const Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (IppiPyramidUpState_32f_C1R*)m_pState))
   FUNC_CV_CALL( ippiPyramidLayerUp_32f_C3R,((const Ipp32f*)pSrc, srcStep, m_srcRoiSize, (Ipp32f*)pDst, dstStep, m_dstRoiSize, (IppiPyramidUpState_32f_C3R*)m_pState))
   return stsNoFunction;
}
/* ///////////////////////////////////////////////////////////////////////////////////////
//  Name:       ippiPyramidLayerDownInitAlloc_8u_C1R,   ippiPyramidLayerDownInitAlloc_8u_C3R
//              ippiPyramidLayerDownInitAlloc_16u_C1R,  ippiPyramidLayerDownInitAlloc_16u_C3R
//              ippiPyramidLayerDownInitAlloc_32f_C1R,  ippiPyramidLayerDownInitAlloc_32f_C3R
//              ippiPyramidLayerUpInitAlloc_8u_C1R,     ippiPyramidLayerUpInitAlloc_8u_C3R
//              ippiPyramidLayerUpInitAlloc_16u_C1R,    ippiPyramidLayerUpInitAlloc_16u_C3R
//              ippiPyramidLayerUpInitAlloc_32f_C1R,    ippiPyramidLayerUpInitAlloc_32f_C3R
//  Purpose:    Initializes structure for pyramid layer calculation
//  Return:
//    ippStsNoErr              Ok
//    ippStsNullPtrErr         One of pointers is NULL
//    ippStsSizeErr            The width or height of images is less or equal zero
//    ippStsBadArgErr          Bad mode, rate or kernel size
//    ippStsMemAllocErr        Memory allocation error
//  Arguments:
//    ppState                  Pointer to the pointer to initialized structure
//    m_srcRoiSize                   Source image ROI size.
//    m_dstRoiSize                   Destination image ROI size.
//    rate                     Neighbour levels ratio (1<rate<4)
//    pKernel                  Separable symmetric kernel of odd length
//    kerSize                  Kernel size
//    mode                     IPPI_INTER_LINEAR - bilinear interpolation
*/
//  Notes:                     For up case destination size belongs to interval
//                             max((int)((float)((src-1)*rate)),src+1)<=dst<=
//                             max((int)((float)(src)*rate)),src+1)


/* ///////////////////////////////////////////////////////////////////////////////////////
//  Name:       ippiPyramidLayerDown_8u_C1R, ippiPyramidLayerDown_16u_C1R, ippiPyramidLayerDown_32f_C1R
//              ippiPyramidLayerDown_8u_C3R, ippiPyramidLayerDown_16u_C3R, ippiPyramidLayerDown_32f_C3R
//              ippiPyramidLayerUp_8u_C1R,   ippiPyramidLayerUp_16u_C1R,   ippiPyramidLayerUp_32f_C1R
//              ippiPyramidLayerUp_8u_C3R,   ippiPyramidLayerUp_16u_C3R,   ippiPyramidLayerUp_32f_C3R
//  Purpose:    Perform downsampling/upsampling of the image with 5x5 gaussian.
//  Return:
//    ippStsNoErr              Ok
//    ippStsNullPtrErr         One of pointers is NULL
//    ippStsSizeErr            The width or height of images is less or equal zero
//    ippStsStepErr            The steps in images are too small
//    ippStsBadArgErr          Wrong pyramid rate value
//    ippStsNotEvenStepErr     Step is not multiple of element.
//  Arguments:
//    pSrc                     Pointer to source image
//    srcStep                  Step in source image
//    m_srcRoiSize                   Source image ROI size.
//    m_dstRoiSize                   Destination image ROI size.
//    pDst                     Pointer to destination image
//    dstStep                  Step in destination image
//    m_dstRoiSize                   destination image ROI size.
//    m_pState                   Pointer to data structure for pyramid calculation
*/
