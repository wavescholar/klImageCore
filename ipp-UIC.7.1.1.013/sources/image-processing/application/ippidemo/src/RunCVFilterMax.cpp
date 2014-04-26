/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterMax.cpp: implementation of the CRunCVFilterMax class.
// CRunCVFilterMax class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "RunCVFilterMax.h"
#include "ParmFilterBoxDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVFilterMax::CRunCVFilterMax()
{
   m_bCenter = TRUE;
   m_maskSize.width = 3;
   m_maskSize.height = 3;
   m_anchor.x = 1;
   m_anchor.y = 1;
   m_bufSize = 0;
   m_pBuffer = NULL;
}

CRunCVFilterMax::~CRunCVFilterMax()
{

}

BOOL CRunCVFilterMax::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   CString baseName = func.BaseName();
   baseName = baseName.Mid(0,baseName.Find("Border"));
   m_GetBufSizeName = "ippi" + baseName + "GetBufferSize_"
              + func.TypeName() + "_" + func.DescrName();
   return TRUE;
}

CParamDlg* CRunCVFilterMax::CreateDlg() { return new CParmFilterBoxDlg;}

void CRunCVFilterMax::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmFilterBoxDlg* pDlg = (CParmFilterBoxDlg*)parmDlg;
   if (save) {
      m_maskSize = pDlg->m_maskSize;
      m_anchor   = pDlg->m_anchor  ;
      m_bCenter   = pDlg->m_bCenter  ;
   } else {
      pDlg->m_maskSize = m_maskSize;
      pDlg->m_anchor   = m_anchor  ;
      pDlg->m_bCenter   = m_bCenter  ;
   }
}

BOOL CRunCVFilterMax::BeforeCall()
{
   IppStatus status = CallGetBufSize(m_GetBufSizeName);
   IppErrorMessage(m_GetBufSizeName, status);
   if (status < 0) return FALSE;
   m_pBuffer = (Ipp8u*)ippMalloc(m_bufSize);
   if (!m_pBuffer) return FALSE;
   return TRUE;
}

BOOL CRunCVFilterMax::AfterCall(BOOL bOK)
{
   if (m_pBuffer)
      ippFree(m_pBuffer);
   m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunCVFilterMax::CallGetBufSize(CString name)
{
   MATE_CV_CALL(name, ippiFilterMinGetBufferSize_8u_C1R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMaxGetBufferSize_8u_C1R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMinGetBufferSize_32f_C1R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMaxGetBufferSize_32f_C1R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMinGetBufferSize_8u_C3R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMaxGetBufferSize_8u_C3R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMinGetBufferSize_32f_C3R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMaxGetBufferSize_32f_C3R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMinGetBufferSize_8u_C4R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMaxGetBufferSize_8u_C4R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMinGetBufferSize_32f_C4R, (roiSize.width, m_maskSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterMaxGetBufferSize_32f_C4R, (roiSize.width, m_maskSize, &m_bufSize))

   return stsNoFunction;
}

IppStatus CRunCVFilterMax::CallIppFunction()
{
   FUNC_CV_CALL( ippiFilterMinBorderReplicate_8u_C1R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMaxBorderReplicate_8u_C1R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMinBorderReplicate_32f_C1R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMaxBorderReplicate_32f_C1R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMinBorderReplicate_8u_C3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMaxBorderReplicate_8u_C3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMinBorderReplicate_32f_C3R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMaxBorderReplicate_32f_C3R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMinBorderReplicate_8u_C4R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMaxBorderReplicate_8u_C4R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMinBorderReplicate_32f_C4R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterMaxBorderReplicate_32f_C4R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_maskSize, m_anchor, m_pBuffer))

   return stsNoFunction;
}

CString CRunCVFilterMax::GetHistoryParms()
{
   CMyString parms;
   parms << m_maskSize << ", " << m_anchor;
   return parms;
}


