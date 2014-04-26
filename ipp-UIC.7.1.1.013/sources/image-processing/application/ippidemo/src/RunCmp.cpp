/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCmp.cpp: implementation of the CRunCmp class.
// CRunCmp class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCmp.h"
#include "ParmCmpDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCmp::CRunCmp()
{
   m_numCoi = 0;

   m_cmpOp = ippCmpLess;
   m_eps = 0.05f;

   m_value.Init(pp8u, 3);
   m_value.Init(pp16s, 3);
   m_value.Init(pp16u, 3);
   m_value.Init(pp32f, 3);

   for (int i=0; i<3; i++) {
      m_value  .Vector(pp8u).Set(i, 128);
      m_value  .Vector(pp16u).Set(i, 1600);
      m_value  .Vector(pp16s).Set(i, 1600);
      m_value  .Vector(pp32f).Set(i, 0.5);
   }

}

CRunCmp::~CRunCmp()
{

}

BOOL CRunCmp::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   if (m_Func.Found("C_")) {
       m_numCoi = m_Func.SrcChannels() - m_Func.SrcAlpha();
      m_value.Init(m_Func.SrcType());
   } else {
      m_numCoi = 0;
      m_value.Init(ppNONE);
      m_UsedVectors = VEC_SRC|VEC_SRC2|VEC_DST;
   }
   return TRUE;
}

CParamDlg* CRunCmp::CreateDlg() { return new CParmCmpDlg;}

void CRunCmp::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmCmpDlg* pDlg = (CParmCmpDlg*)parmDlg;
   if (save) {
      m_cmpOp = (IppCmpOp)pDlg->m_Cmp;
      m_value.Set(pDlg->m_ValueStr, m_numCoi);
      if (m_Func.Found("Eps"))
         m_eps = (float)atof(pDlg->m_EpsStr);
   } else {
      pDlg->m_numCoi = m_numCoi;
      pDlg->m_Cmp = (int)m_cmpOp;
      m_value.Get(pDlg->m_ValueStr, m_numCoi);
      if (m_Func.Found("Eps"))
         pDlg->m_EpsStr.Format("%.6g", m_eps);
   }
}

IppStatus CRunCmp::CallIppFunction()
{
   FUNC_CALL(ippiCompare_8u_C1R,  ( (Ipp8u*)pSrc, srcStep,(Ipp8u*) pSrc2, srcStep2,(Ipp8u*)pDst,  dstStep,roiSize,   m_cmpOp))
   FUNC_CALL(ippiCompare_8u_C3R,  ( (Ipp8u*)pSrc, srcStep,(Ipp8u*) pSrc2, srcStep2,(Ipp8u*)pDst,  dstStep,roiSize,   m_cmpOp))
   FUNC_CALL(ippiCompare_8u_C4R, ( (Ipp8u*)pSrc, srcStep,(Ipp8u*) pSrc2, srcStep2,(Ipp8u*)pDst,  dstStep,roiSize,   m_cmpOp))
   FUNC_CALL(ippiCompare_8u_AC4R, ( (Ipp8u*)pSrc, srcStep,(Ipp8u*) pSrc2, srcStep2,(Ipp8u*)pDst,  dstStep,roiSize,   m_cmpOp))
   FUNC_CALL(ippiCompare_16u_C1R, ((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16u_C3R, ((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16u_C4R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16u_AC4R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16s_C1R, ((Ipp16s*)pSrc, srcStep,(Ipp16s*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16s_C3R, ((Ipp16s*)pSrc, srcStep,(Ipp16s*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16s_C4R,((Ipp16s*)pSrc, srcStep,(Ipp16s*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_16s_AC4R,((Ipp16s*)pSrc, srcStep,(Ipp16s*)pSrc2, srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,    m_cmpOp))
   FUNC_CALL(ippiCompare_32f_C1R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2,(Ipp8u*) pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompare_32f_C3R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2,(Ipp8u*) pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompare_32f_C4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2,(Ipp8u*) pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompare_32f_AC4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2, srcStep2,(Ipp8u*) pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareEqualEps_32f_C1R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2,srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,m_eps))
   FUNC_CALL(ippiCompareEqualEps_32f_C3R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)pSrc2,srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,m_eps))
   FUNC_CALL(ippiCompareEqualEps_32f_C4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pSrc2,srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,m_eps))
   FUNC_CALL(ippiCompareEqualEps_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pSrc2,srcStep2,(Ipp8u*) pDst,  dstStep,roiSize,m_eps))

   FUNC_CALL(ippiCompareC_8u_C1R,  ((Ipp8u*) pSrc, srcStep,(Ipp8u ) m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_8u_C3R,  ((Ipp8u*) pSrc, srcStep,(Ipp8u*) m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_8u_C4R, ((Ipp8u*) pSrc, srcStep,(Ipp8u*) m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_8u_AC4R, ((Ipp8u*) pSrc, srcStep,(Ipp8u*) m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16u_C1R, ((Ipp16u*)pSrc, srcStep,(Ipp16u )m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16u_C3R, ((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16u_C4R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16u_AC4R,((Ipp16u*)pSrc, srcStep,(Ipp16u*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16s_C1R, ((Ipp16s*)pSrc, srcStep,(Ipp16s )m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16s_C3R, ((Ipp16s*)pSrc, srcStep,(Ipp16s*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16s_C4R,((Ipp16s*)pSrc, srcStep,(Ipp16s*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_16s_AC4R,((Ipp16s*)pSrc, srcStep,(Ipp16s*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_32f_C1R, ((Ipp32f*)pSrc, srcStep,(Ipp32f )m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_32f_C3R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_32f_C4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareC_32f_AC4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value,(Ipp8u*)pDst, dstStep,roiSize,m_cmpOp))
   FUNC_CALL(ippiCompareEqualEpsC_32f_C1R, ((Ipp32f*)pSrc, srcStep,(Ipp32f )m_value,(Ipp8u*) pDst, dstStep,roiSize,m_eps))
   FUNC_CALL(ippiCompareEqualEpsC_32f_C3R, ((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value,(Ipp8u*) pDst, dstStep,roiSize,m_eps))
   FUNC_CALL(ippiCompareEqualEpsC_32f_C4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value,(Ipp8u*) pDst, dstStep,roiSize,m_eps))
   FUNC_CALL(ippiCompareEqualEpsC_32f_AC4R,((Ipp32f*)pSrc, srcStep,(Ipp32f*)m_value,(Ipp8u*) pDst, dstStep,roiSize,m_eps))

   return stsNoFunction;
}

BOOL CRunCmp::BeforeCall()
{
   return TRUE;
}

BOOL CRunCmp::AfterCall(BOOL bOk)
{
   if (bOk) {
      CALL(ippiMulC_8u_C1IRSfs,
         (255, (Ipp8u*)pDst, dstStep, roiSize, 0));
   }
   return TRUE;
}

void CRunCmp::Close()
{
   if (m_pDocDst)
      m_pDocDst->UpdateStatusFunc(m_Func + " * 255", m_pPerf);
}

static CMyString valString(CMyString* vals, int num)
{
   if (num == 1) return vals[0];
   CMyString str = "{";
   for (int i=0; i<num; i++) {
      if (i) str += ", ";
      str += vals[i];
   }
   str += "}";
   return str;
}

CString CRunCmp::GetHistoryParms()
{
   CMyString parms;

   if (m_numCoi) {
      CMyString vals[3];
      m_value.Get(vals);
      parms << valString(vals,m_numCoi) << ", ..";
   }
   if (m_Func.Found("Eps")) {
      parms << ", " << m_eps;
   } else {
      parms << ", " << m_cmpOp;
   }
   return parms;
}

