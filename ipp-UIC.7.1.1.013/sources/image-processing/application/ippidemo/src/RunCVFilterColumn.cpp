/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterColumn.cpp: implementation of the CRunCVFilterColumn class.
// CRunCVFilterColumn class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "DemoDoc.h"
#include "RunCVFilterColumn.h"
#include "ParmCVFilterColDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVFilterColumn::CRunCVFilterColumn()
{
   m_ppSrc = NULL;
   m_pipeSize.width = 0;
   m_pipeSize.height = 0;

   m_pBuffer = NULL;
   m_bufSize = 0;

   ppType kernelType[] = {pp32f, pp16s};
   int    kernelNum = sizeof(kernelType) / sizeof(ppType);
   m_kernelSize = 3;
   for (int k=0; k<kernelNum; k++) {
      m_pKernel.Init(kernelType[k], m_kernelSize);
      m_pKernel.Vector().Set(0,-1.);
      m_pKernel.Vector().Set(1, 2.);
      m_pKernel.Vector().Set(2,-1.);
   }
   m_divisor = 1;
   m_DivType = 0;
}

CRunCVFilterColumn::~CRunCVFilterColumn()
{

}

BOOL CRunCVFilterColumn::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   m_pKernel.Init(func.SrcType());

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

CParamDlg* CRunCVFilterColumn::CreateDlg() { return new CParmCVFilterColDlg;}

void CRunCVFilterColumn::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmCVFilterColDlg* pDlg = (CParmCVFilterColDlg*)parmDlg;
   if (save) {
      m_kernelSize = pDlg->m_maskSize.height;
      m_pKernel.Vector() = pDlg->m_kernel;
   } else {
      pDlg->m_maskSize.height = m_kernelSize;
      pDlg->m_maskSize.width = 1;
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
}

BOOL CRunCVFilterColumn::BeforeCall()
{
   m_pipeSize.width = roiSize.width;
   m_pipeSize.height = roiSize.height - m_kernelSize + 1;
   m_ppSrc = (void**)ippMalloc(roiSize.height*sizeof(void*));
   Ipp8u* pLine = (Ipp8u*)pSrc;
   int step = m_pDocSrc->GetImage()->Step();
   for (int i=0; i<roiSize.height; i++) {
      m_ppSrc[i] = pLine;
      pLine += step;
   }

   IppStatus status = CallGetBufSize(m_GetBufSizeName);
   IppErrorMessage(m_GetBufSizeName, status);
   if (status < 0) return FALSE;
   m_pBuffer = (Ipp8u*)ippMalloc(m_bufSize);
   if (!m_pBuffer) return FALSE;

   return TRUE;
}

BOOL CRunCVFilterColumn::AfterCall(BOOL bOK)
{
   if (m_pBuffer)
      ippFree(m_pBuffer);
   m_pBuffer = NULL;
   if (m_ppSrc)
      ippFree(m_ppSrc);
   m_ppSrc = NULL;
   return TRUE;
}

IppStatus CRunCVFilterColumn::CallGetBufSize(CString name)
{
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_16s_C1R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_16s_C3R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_Low_16s_C1R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_Low_16s_C3R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_16s8u_C1R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_16s8u_C3R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_32f_C1R, (m_pipeSize, m_kernelSize, &m_bufSize))
   MATE_CV_CALL(name, ippiFilterColumnPipelineGetBufferSize_32f_C3R, (m_pipeSize, m_kernelSize, &m_bufSize))

   return stsNoFunction;
}

IppStatus CRunCVFilterColumn::CallIppFunction()
{
   FUNC_CV_CALL( ippiFilterColumnPipeline_16s_C1R, ((const Ipp16s**)(m_ppSrc), (Ipp16s*)pDst, dstStep, m_pipeSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_16s_C3R, ((const Ipp16s**)(m_ppSrc), (Ipp16s*)pDst, dstStep, m_pipeSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_Low_16s_C1R, ((const Ipp16s**)(m_ppSrc), (Ipp16s*)pDst, dstStep, m_pipeSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_Low_16s_C3R, ((const Ipp16s**)(m_ppSrc), (Ipp16s*)pDst, dstStep, m_pipeSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_16s8u_C1R, ((const Ipp16s**)(m_ppSrc), (Ipp8u*)pDst, dstStep, m_pipeSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_16s8u_C3R, ((const Ipp16s**)(m_ppSrc), (Ipp8u*)pDst, dstStep, m_pipeSize, (const Ipp16s*)m_pKernel, m_kernelSize, m_divisor, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_32f_C1R, ((const Ipp32f**)(m_ppSrc), (Ipp32f*)pDst, dstStep, m_pipeSize, (const Ipp32f*)m_pKernel, m_kernelSize, m_pBuffer))
   FUNC_CV_CALL( ippiFilterColumnPipeline_32f_C3R, ((const Ipp32f**)(m_ppSrc), (Ipp32f*)pDst, dstStep, m_pipeSize, (const Ipp32f*)m_pKernel, m_kernelSize, m_pBuffer))

   return stsNoFunction;
}


CString CRunCVFilterColumn::GetHistoryParms()
{
   CMyString parms;
   parms << m_kernelSize;
   if (m_Func.SrcType() != pp32f)
      parms << ", " << m_divisor;
   return parms;
}

