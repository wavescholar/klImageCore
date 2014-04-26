/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHint.cpp: implementation of the CRunHint class.
// CRunHint class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunHint.h"
#include "ParmHintDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunHint::CRunHint()
{
   m_hint = ippAlgHintFast;
}

CRunHint::~CRunHint()
{

}

CParamDlg* CRunHint::CreateDlg() { return new CParmHintDlg;}

void CRunHint::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmHintDlg* pDlg = (CParmHintDlg*)parmDlg;
   if (save) {
      m_hint = (IppHintAlgorithm)pDlg->m_Hint;
   } else {
      pDlg->m_Hint = (int)m_hint;
   }
}

IppStatus CRunHint::CallIppFunction()
{
   FUNC_CALL(ippiScale_16u8u_C1R,
        ((Ipp16u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16s8u_C1R,
        ((Ipp16s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_32s8u_C1R,
        ((Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16u8u_C3R,
        ((Ipp16u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16s8u_C3R,
        ((Ipp16s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_32s8u_C3R,
        ((Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16u8u_AC4R,
        ((Ipp16u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16s8u_AC4R,
        ((Ipp16s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_32s8u_AC4R,
        ((Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16u8u_C4R,
        ((Ipp16u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_16s8u_C4R,
        ((Ipp16s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))
   FUNC_CALL(ippiScale_32s8u_C4R,
        ((Ipp32s*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_hint))

   return stsNoFunction;
}

CString CRunHint::GetHistoryParms()
{
   CMyString parms;
   return parms << m_hint;
}

