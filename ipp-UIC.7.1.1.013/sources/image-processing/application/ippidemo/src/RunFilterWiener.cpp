/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterWiener.cpp: implementation of the CRunFilterWiener class.
// CRunFilterWiener class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "RunFilterWiener.h"
#include "ParmFilterWienerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFilterWiener::CRunFilterWiener() : CRunFilterBox()
{
   m_noise.Init(pp32f, 4);
   for (int i=0; i<4; i++) m_noise.Set(i, 0.5);
   m_pBuffer = NULL;
}

CRunFilterWiener::~CRunFilterWiener()
{

}

BOOL CRunFilterWiener::Open(CFunc func)
{
   if (!CRunFilterBox::Open(func)) return FALSE;
   m_channels = func.SrcAlpha() ? func.SrcChannels() - 1 : func.SrcChannels();
   return TRUE;
}

void CRunFilterWiener::Close()
{
   CRunFilterBox::Close();
}

CParamDlg* CRunFilterWiener::CreateDlg() { return new CParmFilterWienerDlg;}

void CRunFilterWiener::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CRunFilterBox::UpdateData(parmDlg,save);
   CParmFilterWienerDlg* pDlg = (CParmFilterWienerDlg*)parmDlg;
   if (save) {
      for (int i=0; i<m_channels; i++)
         m_noise.Set(i, pDlg->m_value.Get(i));
   } else {
      pDlg->m_value.Init(pp32f, m_channels);
      for (int i=0; i<m_channels; i++)
         pDlg->m_value.Set(i, m_noise.Get(i));
   }
}


BOOL CRunFilterWiener::BeforeCall()
{
   int bufSize;
   CHECK_CALL(ippiFilterWienerGetBufferSize,(
      roiSize, m_maskSize, m_Func.SrcChannels(), &bufSize))
   if (bufSize)
      m_pBuffer = ippMalloc(bufSize);
   return TRUE;
}

BOOL CRunFilterWiener::AfterCall(BOOL bOK)
{
   if (m_pBuffer) ippFree(m_pBuffer); m_pBuffer = NULL;
   return TRUE;
}

IppStatus CRunFilterWiener::CallIppFunction()
{
   FUNC_CALL(ippiFilterWiener_8u_C1R,((Ipp8u*)pSrc, srcStep,
              (Ipp8u*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_8u_C3R,((Ipp8u*)pSrc, srcStep,
              (Ipp8u*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_8u_AC4R,((Ipp8u*)pSrc, srcStep,
              (Ipp8u*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_8u_C4R,((Ipp8u*)pSrc, srcStep,
              (Ipp8u*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_16s_C1R,((Ipp16s*)pSrc, srcStep,
             (Ipp16s*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_16s_C3R,((Ipp16s*)pSrc, srcStep,
             (Ipp16s*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_16s_AC4R,((Ipp16s*)pSrc, srcStep,
             (Ipp16s*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_16s_C4R,((Ipp16s*)pSrc, srcStep,
             (Ipp16s*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_32f_C1R,((Ipp32f*)pSrc, srcStep,
             (Ipp32f*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_32f_C3R,((Ipp32f*)pSrc, srcStep,
             (Ipp32f*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_32f_AC4R,((Ipp32f*)pSrc, srcStep,
             (Ipp32f*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))
   FUNC_CALL(ippiFilterWiener_32f_C4R,((Ipp32f*)pSrc, srcStep,
             (Ipp32f*)pDst, dstStep, roiSize, m_maskSize,
                           m_anchor, (Ipp32f*)m_noise, (Ipp8u*)m_pBuffer ))

   return stsNoFunction;
}

CString CRunFilterWiener::GetHistoryParms()
{
   CMyString parms;

   m_noise.Get(parms,", ");
   if (m_noise.Length() > 1)
      parms = "{" + parms + "}";

   parms =  CRunFilterBox::GetHistoryParms() + ", " + parms;
   return parms;
}
