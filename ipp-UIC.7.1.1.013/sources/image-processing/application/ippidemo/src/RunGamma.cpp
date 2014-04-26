/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunGamma.cpp: implementation of the CRunGamma class.
// CRunGamma class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunGamma.h"
#include "ParmGammaDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunGamma::CRunGamma()
{
   m_vMin = 0;
   m_vMax = 1;
}

CRunGamma::~CRunGamma()
{

}

CParamDlg* CRunGamma::CreateDlg() { return new CParmGammaDlg;}

void CRunGamma::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmGammaDlg* pDlg = (CParmGammaDlg*)parmDlg;
    if (save) {
       m_vMin = (float)atof(pDlg->m_Str0);
       m_vMax = (float)atof(pDlg->m_Str1);
    } else {
       pDlg->m_Str0.Format("%.6g", m_vMin);
       pDlg->m_Str1.Format("%.6g", m_vMax);
    }
}

IppStatus CRunGamma::CallIppFunction()
{
   FUNC_CC_CALL(ippiGammaFwd_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaFwd_32f_C3IR, ( (Ipp32f*)pSrc, srcStep, roiSize, m_vMin, m_vMax ))
   FUNC_CC_CALL(ippiGammaInv_32f_C3R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaInv_32f_C3IR, ( (Ipp32f*)pSrc, srcStep, roiSize, m_vMin, m_vMax ))
   FUNC_CC_CALL(ippiGammaFwd_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaFwd_32f_AC4IR, ( (Ipp32f*)pSrc, srcStep, roiSize, m_vMin, m_vMax ))
   FUNC_CC_CALL(ippiGammaInv_32f_AC4R, ( (Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaInv_32f_AC4IR, ( (Ipp32f*)pSrc, srcStep, roiSize, m_vMin, m_vMax ))
   FUNC_CC_CALL(ippiGammaFwd_32f_P3R, ( (const Ipp32f**)pSrcP, srcStep, (Ipp32f**)pDstP, dstStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaFwd_32f_IP3R, ( (Ipp32f**)pSrcP, srcStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaInv_32f_P3R, ( (const Ipp32f**)pSrcP, srcStep, (Ipp32f**)pDstP, dstStep, roiSize, m_vMin, m_vMax  ))
   FUNC_CC_CALL(ippiGammaInv_32f_IP3R, ( (Ipp32f**)pSrcP, srcStep, roiSize, m_vMin, m_vMax  ))

   return stsNoFunction;
}

CString CRunGamma::GetHistoryParms()
{
   CMyString parms;
   parms << m_vMin << ", "
         << m_vMax;
   return parms;
}
