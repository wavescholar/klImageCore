/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilter.cpp: implementation of the CRunCVFilter class.
// CRunCVFilter class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "RunCVFilter.h"
#include "ParmCVBorderDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVFilter::CRunCVFilter()
{
   m_borderType = ippBorderRepl;
   m_borderValue.Init(pp8u, 192);
   m_borderValue.Init(pp32f, 192);
   m_pBuffer = NULL;
   m_bufSize = 0;
}

CRunCVFilter::~CRunCVFilter()
{

}

BOOL CRunCVFilter::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   CString baseName = func.BaseName();
   baseName = baseName.Mid(0,baseName.Find("Border"));
   m_GetBufSizeName = "ippi" + baseName + "GetBufferSize_"
              + func.TypeName() + "_" + func.DescrName();
   m_borderValue.Init(func);
   return TRUE;
}

CParamDlg* CRunCVFilter::CreateDlg() { return new CParmCVBorderDlg;}

void CRunCVFilter::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmCVBorderDlg* pDlg = (CParmCVBorderDlg*)parmDlg;
    if (save) {
        m_borderType = (IppiBorderType)pDlg->m_BorderType;
        m_borderValue.Set(pDlg->m_ValueStr);
    } else {
        pDlg->m_BorderType = (int)m_borderType;
        m_borderValue.Get(pDlg->m_ValueStr);
    }
}

BOOL CRunCVFilter::BeforeCall()
{
   IppStatus status = CallGetBufSize(m_GetBufSizeName);
   IppErrorMessage(m_GetBufSizeName, status);
   if (status < 0) return FALSE;
   m_pBuffer = (Ipp8u*)ippMalloc(m_bufSize);
   if (!m_pBuffer) return FALSE;
   return TRUE;
}

BOOL CRunCVFilter::AfterCall(BOOL bOK)
{
   if (m_pBuffer)
      ippFree(m_pBuffer);
   m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunCVFilter::CallGetBufSize(CString name)
{
   MATE_CV_CALL(name, ippiFilterScharrHorizGetBufferSize_8u16s_C1R, (roiSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterScharrVertGetBufferSize_8u16s_C1R, (roiSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterScharrHorizGetBufferSize_32f_C1R, (roiSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterScharrVertGetBufferSize_32f_C1R, (roiSize, &m_bufSize))


   return stsNoFunction;
}

IppStatus CRunCVFilter::CallIppFunction()
{

   FUNC_CV_CALL( ippiFilterScharrHorizBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterScharrVertBorder_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep,
 (Ipp16s*)pDst, dstStep, roiSize,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterScharrHorizBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterScharrVertBorder_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_borderValue, m_pBuffer))

   return stsNoFunction;
}

CString CRunCVFilter::GetHistoryParms()
{
   CMyString parms;
   parms << m_borderType << ", " << m_borderValue;
   return parms;
}

