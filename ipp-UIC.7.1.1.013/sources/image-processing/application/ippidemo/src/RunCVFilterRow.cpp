/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterRow.cpp: implementation of the CRunCVFilterRow class.
// CRunCVFilterRow class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "DemoDoc.h"
#include "RunCVFilterRow.h"
#include "ParmCVFilterRowDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVFilterRow::CRunCVFilterRow()
{
   m_ppDst = NULL;

   m_pBuffer = NULL;
   m_bufSize = 0;

   ppType kernelType[] = {pp32f, pp16s};
   int    kernelNum = sizeof(kernelType) / sizeof(ppType);
   m_kernelSize = 3;
   m_anchor = 1;
   m_bCenter = TRUE;
   for (int k=0; k<kernelNum; k++) {
      m_pKernel.Init(kernelType[k], m_kernelSize);
      m_pKernel.Vector().Set(0,-1.);
      m_pKernel.Vector().Set(1, 2.);
      m_pKernel.Vector().Set(2,-1.);
   }

   m_divisor = 1;
   m_DivType = 0;

   m_borderType = ippBorderRepl;
}

CRunCVFilterRow::~CRunCVFilterRow()
{

}

BOOL CRunCVFilterRow::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   m_pKernel.Init(func.DstType());
   m_borderValue.Init(func.SrcType(), func.SrcChannels());

   CString baseName = func.BaseName();
   int i = baseName.Find("_");
   if (i > 0)
      baseName = baseName.Mid(0,i) + "GetBufferSize" + baseName.Mid(i);
   else
      baseName = baseName + "GetBufferSize";
   m_GetBufSizeName = "ippi" + baseName + "_"
              + func.TypeName() + "_" + func.DescrName();
   return TRUE;
}

CParamDlg* CRunCVFilterRow::CreateDlg() { return new CParmCVFilterRowDlg;}

void CRunCVFilterRow::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmCVFilterRowDlg* pDlg = (CParmCVFilterRowDlg*)parmDlg;
   if (save) {
      m_kernelSize = pDlg->m_maskSize.width;
      m_anchor = pDlg->m_anchor.x;
      m_bCenter = pDlg->m_bCenter;
      m_pKernel.Vector() = pDlg->m_kernel;
   } else {
      pDlg->m_maskSize.width = m_kernelSize;
      pDlg->m_maskSize.height = 1;
      pDlg->m_anchor.x = m_anchor;
      pDlg->m_bCenter = m_bCenter;
      pDlg->m_kernel = m_pKernel.Vector();
   }
   if (m_Func.SrcType() != pp32f) {
      if (save) {
         m_divisor = atoi(pDlg->m_DivStr);
         m_DivType = pDlg->m_DivType;
      } else {
         pDlg->m_DivStr.Format("%d",m_divisor);
         pDlg->m_DivType = m_DivType;
      }
   }
    if (save) {
        m_borderType = (IppiBorderType)pDlg->m_BorderType;
        m_borderValue.Set(pDlg->m_ValueStr, m_Func.SrcChannels());
    } else {
        pDlg->m_BorderType = (int)m_borderType;
        m_borderValue.Get(pDlg->m_ValueStr, m_Func.SrcChannels());
    }
}

BOOL CRunCVFilterRow::BeforeCall()
{
   m_ppDst = (void**)ippMalloc(roiSize.height*sizeof(void*));
   Ipp8u* pLine = (Ipp8u*)pDst;
   int step = m_pDocDst->GetImage()->Step();
   for (int i=0; i<roiSize.height; i++) {
      m_ppDst[i] = pLine;
      pLine += step;
   }

   IppStatus status = CallGetBufSize(m_GetBufSizeName);
   IppErrorMessage(m_GetBufSizeName, status);
   if (status < 0) return FALSE;
   m_pBuffer = (Ipp8u*)ippMalloc(m_bufSize);
   if (!m_pBuffer) return FALSE;

   return TRUE;
}

BOOL CRunCVFilterRow::AfterCall(BOOL bOK)
{
   if (m_pBuffer)
      ippFree(m_pBuffer);
   m_pBuffer = NULL;
   if (m_ppDst)
      ippFree(m_ppDst);
   m_ppDst = NULL;
   return TRUE;
}

IppStatus CRunCVFilterRow::CallGetBufSize(CString name)
{
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_8u16s_C1R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_8u16s_C3R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_16s_C1R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_16s_C3R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_Low_16s_C1R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_Low_16s_C3R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_32f_C1R, (roiSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterRowBorderPipelineGetBufferSize_32f_C3R, (roiSize, m_kernelSize, &m_bufSize))

   return stsNoFunction;
}

IppStatus CRunCVFilterRow::CallIppFunction()
{
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_8u16s_C1R, ((const Ipp8u*)pSrc, srcStep, (Ipp16s**)m_ppDst, roiSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp8u)m_borderValue, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_8u16s_C3R, ((const Ipp8u*)pSrc, srcStep, (Ipp16s**)m_ppDst, roiSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp8u*)m_borderValue, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_16s_C1R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)m_ppDst, roiSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp16s)m_borderValue, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_16s_C3R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)m_ppDst, roiSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp16s*)m_borderValue, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_Low_16s_C1R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)m_ppDst, roiSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp16s)m_borderValue, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_Low_16s_C3R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)m_ppDst, roiSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp16s*)m_borderValue, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_32f_C1R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f**)m_ppDst, roiSize, (const Ipp32f*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp32f)m_borderValue, m_pBuffer))
   FUNC_CV_CALL( ippiFilterRowBorderPipeline_32f_C3R, ((const Ipp32f*)pSrc, srcStep, (Ipp32f**)m_ppDst, roiSize, (const Ipp32f*)m_pKernel, m_kernelSize, m_anchor, m_borderType, (Ipp32f*)m_borderValue, m_pBuffer))
   return stsNoFunction;
}


CString CRunCVFilterRow::GetHistoryParms()
{
   CMyString parms;
   parms
      << m_kernelSize
      << ", "
      << m_anchor
      << ", "
      << m_borderType
//      << ", "
//      << m_borderValue.Vector()
      ;
   if (m_Func.SrcType() != pp32f)
      parms << ", " << m_divisor;
   return parms;
}

