/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunAlphaC.cpp: implementation of the CRunAlphaC class.
// CRunAlphaC class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunAlphaC.h"
#include "ParmAlphaCDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRunAlphaC::CRunAlphaC()
{
   m_alpha1.Init(pp8u, 192);
   m_alpha1.Init(pp16u, 48000);
   m_alpha2.Init(pp8u, 64);
   m_alpha2.Init(pp16u, 16000);
}

CRunAlphaC::~CRunAlphaC()
{

}

BOOL CRunAlphaC::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   m_alpha1.Init(func.SrcType());
   m_alpha2.Init(func.SrcType());
   return TRUE;
}

CParamDlg* CRunAlphaC::CreateDlg() { return new CParmAlphaCDlg;}

void CRunAlphaC::UpdateData(CParamDlg* pParmDlg, BOOL save)
{
   CRunAlpha::UpdateData(pParmDlg, save);
   CParmAlphaCDlg* pDlg = (CParmAlphaCDlg*)pParmDlg;
   if (save) {
      m_alpha1.Set(pDlg->m_Alpha1Str);
      m_alpha2.Set(pDlg->m_Alpha2Str);
   } else {
      m_alpha1.Get(pDlg->m_Alpha1Str);
      m_alpha2.Get(pDlg->m_Alpha2Str);
   }
   m_PremulName = m_Func.FuncWithBase("AlphaPremulC");
   if (m_Inplace)
      m_PremulName = m_PremulName.Left(m_PremulName.GetLength() - 1) + "IR";
}

IppStatus CRunAlphaC::CallPremul(void* pSrc, int srcStep,
                                 void* pDst, int dstStep,
                                 CValue& alpha)
{
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_AC4R,  ((Ipp8u *)pSrc, srcStep,(Ipp8u )alpha,(Ipp8u *)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_AC4R, ((Ipp16u*)pSrc, srcStep,(Ipp16u)alpha,(Ipp16u*)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_C4R,  ((Ipp8u *)pSrc, srcStep,(Ipp8u )alpha,(Ipp8u *)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_C4R, ((Ipp16u*)pSrc, srcStep,(Ipp16u)alpha,(Ipp16u*)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_C3R,   ((Ipp8u *)pSrc, srcStep,(Ipp8u )alpha,(Ipp8u *)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_C3R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u)alpha,(Ipp16u*)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_C1R,   ((Ipp8u *)pSrc, srcStep,(Ipp8u )alpha,(Ipp8u *)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_C1R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u)alpha,(Ipp16u*)pDst, dstStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_AC4IR, ((Ipp8u )alpha,(Ipp8u *)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_AC4IR,((Ipp16u)alpha,(Ipp16u*)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_C4IR, ((Ipp8u )alpha,(Ipp8u *)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_C4IR,((Ipp16u)alpha,(Ipp16u*)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_C3IR,  ((Ipp8u )alpha,(Ipp8u *)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_C3IR, ((Ipp16u)alpha,(Ipp16u*)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_C1IR,  ((Ipp8u )alpha,(Ipp8u *)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_C1IR, ((Ipp16u)alpha,(Ipp16u*)pSrc, srcStep,roiSize ))

   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_AP4R,  ((Ipp8u **)pSrc, srcStep, (Ipp8u )alpha, (Ipp8u **)pDst, dstStep, roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_AP4R, ((Ipp16u**)pSrc, srcStep, (Ipp16u)alpha, (Ipp16u**)pDst, dstStep, roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_8u_AP4IR, ((Ipp8u )alpha, (Ipp8u **)pSrc, srcStep, roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremulC_16u_AP4IR,((Ipp16u)alpha, (Ipp16u**)pSrc, srcStep, roiSize ))

   return stsNoFunction;
}

IppStatus CRunAlphaC::CallIppFunction()
{
   FUNC_CALL(ippiAlphaCompC_8u_C1R,  ((Ipp8u *)m_srcComp, srcStep,(Ipp8u )m_alpha1,(Ipp8u *)m_srcComp2, srcStep2,(Ipp8u )m_alpha2,(Ipp8u *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_16u_C1R, ((Ipp16u*)m_srcComp, srcStep,(Ipp16u)m_alpha1,(Ipp16u*)m_srcComp2, srcStep2,(Ipp16u)m_alpha2,(Ipp16u*)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_32u_C1R,  ((Ipp32u *)m_srcComp, srcStep,(Ipp32u )m_alpha1,(Ipp32u *)m_srcComp2, srcStep2,(Ipp32u )m_alpha2,(Ipp32u *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_8s_C1R,  ((Ipp8s *)m_srcComp, srcStep,(Ipp8s )m_alpha1,(Ipp8s *)m_srcComp2, srcStep2,(Ipp8s )m_alpha2,(Ipp8s *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_16s_C1R,  ((Ipp16s *)m_srcComp, srcStep,(Ipp16s )m_alpha1,(Ipp16s *)m_srcComp2, srcStep2,(Ipp16s )m_alpha2,(Ipp16s *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_32s_C1R,  ((Ipp32s *)m_srcComp, srcStep,(Ipp32s )m_alpha1,(Ipp32s *)m_srcComp2, srcStep2,(Ipp32s )m_alpha2,(Ipp32s *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_32f_C1R,  ((Ipp32f *)m_srcComp, srcStep,(Ipp32f )m_alpha1,(Ipp32f *)m_srcComp2, srcStep2,(Ipp32f )m_alpha2,(Ipp32f *)pDst, dstStep,roiSize,m_alphaType ))

   FUNC_CALL(ippiAlphaCompC_8u_AC4R, ((Ipp8u *)m_srcComp, srcStep,(Ipp8u )m_alpha1,(Ipp8u *)m_srcComp2, srcStep2,(Ipp8u )m_alpha2,(Ipp8u *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_16u_AC4R,((Ipp16u*)m_srcComp, srcStep,(Ipp16u)m_alpha1,(Ipp16u*)m_srcComp2, srcStep2,(Ipp16u)m_alpha2,(Ipp16u*)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_8u_C4R, ((Ipp8u *)m_srcComp, srcStep,(Ipp8u )m_alpha1,(Ipp8u *)m_srcComp2, srcStep2,(Ipp8u )m_alpha2,(Ipp8u *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_16u_C4R,((Ipp16u*)m_srcComp, srcStep,(Ipp16u)m_alpha1,(Ipp16u*)m_srcComp2, srcStep2,(Ipp16u)m_alpha2,(Ipp16u*)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_8u_C3R,  ((Ipp8u *)m_srcComp, srcStep,(Ipp8u )m_alpha1,(Ipp8u *)m_srcComp2, srcStep2,(Ipp8u )m_alpha2,(Ipp8u *)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_16u_C3R, ((Ipp16u*)m_srcComp, srcStep,(Ipp16u)m_alpha1,(Ipp16u*)m_srcComp2, srcStep2,(Ipp16u)m_alpha2,(Ipp16u*)pDst, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_8u_AP4R, ((Ipp8u**)m_srcCompP, srcStep,(Ipp8u )m_alpha1,(Ipp8u **)m_srcComp2P, srcStep2,(Ipp8u )m_alpha2,(Ipp8u**)pDstP, dstStep,roiSize,m_alphaType ))
   FUNC_CALL(ippiAlphaCompC_16u_AP4R,((Ipp16u**)m_srcCompP, srcStep,(Ipp16u)m_alpha1,(Ipp16u**)m_srcComp2P, srcStep2,(Ipp16u)m_alpha2,(Ipp16u**)pDstP, dstStep,roiSize,m_alphaType ))
   return stsNoFunction;
}

CString CRunAlphaC::GetHistoryParms()
{
   CMyString parms;
   return parms << m_alphaType;
}

CString CRunAlphaC::GetMpyParms1()
{
   CMyString parms;
   return parms << m_alpha1;
}

CString CRunAlphaC::GetMpyParms2()
{
   CMyString parms;
   return parms << m_alpha2;
}
