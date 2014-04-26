/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCV3.cpp: implementation of the CRunCV2Value class.
// CRunCV2Value class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCV2Value.h"
#include "Parm2ValueDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCV2Value::CRunCV2Value()
{
   m_valAdd.Init(pp32f, .5);
   m_valAbs.Init(pp32f, .5);
   m_valAbs.Init(pp32s, 127);
}

CRunCV2Value::~CRunCV2Value()
{

}

BOOL CRunCV2Value::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   if      (m_Func.Found("Add"))
      m_UsedVectors = VEC_SRC | VEC_SRC2;
   else if (m_Func.Found("Abs"))
      m_UsedVectors = VEC_SRC | VEC_DST;
   if (m_Func.Mask())
      m_UsedVectors |= VEC_MASK;

   m_valAdd.Init(pp32f);
   m_valAbs.Init(func.SrcType() == pp32f ? pp32f : pp32s);

   if      (m_Func.Found("Add")) m_value = m_valAdd;
   else if (m_Func.Found("Abs")) m_value = m_valAbs;
   return TRUE;
}

void CRunCV2Value::Close()
{
   if      (m_Func.Found("Add")) m_value = m_valAdd;
   else if (m_Func.Found("Abs")) m_value = m_valAbs;
}

CParamDlg* CRunCV2Value::CreateDlg() {
   return new CParm2ValueDlg();
}

void CRunCV2Value::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParm2ValueDlg* pDlg = (CParm2ValueDlg*)parmDlg;
   if (save) {
      m_value.Set(pDlg->m_ValStr);
   } else {
      m_value.Get(pDlg->m_ValStr);
      if (m_Func.Found("Add")) pDlg->m_ValName = "Alpha";
   }
}

CString CRunCV2Value::GetHistoryParms()
{
   CMyString parms;
   parms << m_value;
   return parms;
}

IppStatus CRunCV2Value::CallIppFunction()
{

   FUNC_CV_CALL(ippiAddWeighted_8u32f_C1IR, ((Ipp8u*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_8s32f_C1IR, ((Ipp8s*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_16u32f_C1IR, ((Ipp16u*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_32f_C1IR, ((Ipp32f*)pSrc, srcStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_8u32f_C1IMR,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_8s32f_C1IMR,((Ipp8s*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_16u32f_C1IMR,((Ipp16u*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))
   FUNC_CV_CALL(ippiAddWeighted_32f_C1IMR, ((Ipp32f*)pSrc, srcStep, (Ipp8u*)pMask, maskStep, (Ipp32f*)pSrc2, srcStep2, roiSize, (Ipp32f)m_value ))

   FUNC_CV_CALL( ippiAbsDiffC_8u_C1R, ( (const Ipp8u*)pSrc, srcStep,(Ipp8u*)pDst, dstStep, roiSize, (int)m_value ))
   FUNC_CV_CALL( ippiAbsDiffC_16u_C1R, ( (const Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (int)m_value ))
   FUNC_CV_CALL( ippiAbsDiffC_32f_C1R, ( (const Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_value ))
   return stsNoFunction;
}
