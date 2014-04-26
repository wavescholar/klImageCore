/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFlood.cpp: implementation of the CRunCVFlood class.
// CRunCVFlood class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCVFlood.h"
#include "ParmCVFloodDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVFlood::CRunCVFlood()
{
   m_newVal.Init(pp32f, .5);
   m_lowVal.Init(pp32f, .01);
   m_upVal .Init(pp32f, .01);
   m_newVal.Init(pp32s, 127);
   m_lowVal.Init(pp32s,  2);
   m_upVal .Init(pp32s,  2);
   m_seed.x = 0;
   m_seed.y = 0;
   m_SeedType = 0;
   m_pBuffer = NULL;
}

CRunCVFlood::~CRunCVFlood()
{

}

BOOL CRunCVFlood::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   if (m_Func.SrcType() == pp32f) {
      m_newVal.Init(pp32f);
      m_lowVal.Init(pp32f);
      m_upVal .Init(pp32f);
   } else {
      m_newVal.Init(pp32s);
      m_lowVal.Init(pp32s);
      m_upVal .Init(pp32s);
   }
   return TRUE;
}

CParamDlg* CRunCVFlood::CreateDlg() { return new CParmCVFloodDlg;}

void CRunCVFlood::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmCVFloodDlg* pDlg = (CParmCVFloodDlg*)parmDlg;
    if (save) {
        m_SeedType = pDlg->m_SeedType;
        m_seed = pDlg->m_seed;
        m_newVal.Set(pDlg->m_NewValStr);
        m_lowVal.Set(pDlg->m_LowValStr);
        m_upVal .Set(pDlg->m_UpValStr );
    } else {
        pDlg->m_SeedType = m_SeedType;
        pDlg->m_seed = m_seed;
        m_newVal.Get(pDlg->m_NewValStr);
        m_lowVal.Get(pDlg->m_LowValStr);
        m_upVal .Get(pDlg->m_UpValStr );
    }
}

BOOL CRunCVFlood::BeforeCall()
{
   int bufferSize;
   IppStatus status = CV_CALL(ippiFloodFillGetSize,(roiSize, &bufferSize));
   IppErrorMessage("ippiFloodFillGetSize", status);
   if (status < 0) return FALSE;
   if (bufferSize) m_pBuffer = (Ipp8u*)ippMalloc(bufferSize);
   return TRUE;
}

BOOL CRunCVFlood::AfterCall(BOOL bOK)
{
   ippFree(m_pBuffer); m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunCVFlood::CallIppFunction()
{
   FUNC_CV_CALL(ippiFloodFill_4Con_8u_C1IR ,((Ipp8u *)pSrc, srcStep, roiSize, m_seed, (Ipp8u)m_newVal,&m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_8Con_8u_C1IR ,((Ipp8u *)pSrc, srcStep, roiSize, m_seed, (Ipp8u)m_newVal,&m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_4Con_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, m_seed, (Ipp32f)m_newVal,&m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_8Con_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, m_seed, (Ipp32f)m_newVal,&m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_Grad4Con_8u_C1IR,  ((Ipp8u *)pSrc, srcStep, roiSize, m_seed, (Ipp8u)m_newVal, (Ipp8u)m_lowVal, (Ipp8u)m_upVal, &m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_Grad8Con_8u_C1IR,  ((Ipp8u *)pSrc, srcStep, roiSize, m_seed, (Ipp8u)m_newVal, (Ipp8u)m_lowVal, (Ipp8u)m_upVal, &m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_Grad4Con_32f_C1IR, ((Ipp32f*)pSrc, srcStep, roiSize, m_seed, (Ipp32f)m_newVal, (Ipp32f)m_lowVal, (Ipp32f)m_upVal, &m_region, m_pBuffer))
   FUNC_CV_CALL(ippiFloodFill_Grad8Con_32f_C1IR, ((Ipp32f*)pSrc, srcStep, roiSize, m_seed, (Ipp32f)m_newVal, (Ipp32f)m_lowVal, (Ipp32f)m_upVal, &m_region, m_pBuffer))
   return stsNoFunction;
}

CString CRunCVFlood::GetHistoryParms()
{
   CMyString parms;
   parms << m_seed << ", " << m_newVal;
   if (m_Func.Found("Grad"))
      parms << ", " << m_lowVal << ", " << m_upVal;
   return parms;
}


void CRunCVFlood::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CippiRun::AddHistoFunc(pHisto,vecPos);
   CMyString info;
   info << "\t" << "region: {"
        << "area = " << m_region.area << "; "
        << "value = " << m_region.value << "; "
        << "rect = " << m_region.rect << "}";

   pHisto->AddTail(info);
}
