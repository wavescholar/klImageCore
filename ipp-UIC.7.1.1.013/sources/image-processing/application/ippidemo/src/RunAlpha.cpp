/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunAlpha.cpp: implementation of the CRunAlpha class.
// CRunAlpha class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "ChildFrm.h"
#include "RunAlpha.h"
#include "ParmAlphaDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunAlpha::CRunAlpha()
{
   m_alphaType = ippAlphaOver;
   m_showPremul = TRUE;
   m_Inplace = FALSE;

   m_pDocMpySrc  = NULL;
   m_pDocMpySrc2 = NULL;

   m_mpySrc   = NULL;
   m_mpySrc2  = NULL;
   m_srcComp  = NULL;
   m_srcComp2 = NULL;
   m_mpySrcP   = NULL;
   m_mpySrc2P  = NULL;
   m_srcCompP  = NULL;
   m_srcComp2P = NULL;
}

CRunAlpha::~CRunAlpha()
{

}

BOOL CRunAlpha::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

BOOL CRunAlpha::PrepareMpySrc()
{
    m_pDocMpySrc = NULL;
    m_pDocMpySrc2 = NULL;
    if (m_alphaType < ippAlphaOverPremul) return TRUE;
    if (m_Inplace) return TRUE;

    m_pDocMpySrc  = CreateNewDoc(m_pDocSrc->GetImage());
    if (!m_pDocMpySrc) return FALSE;
    m_pDocMpySrc->CopyRoi(m_pDocSrc->GetImage());
    m_pDocMpySrc->GetFrame()->ShowWindow(SW_HIDE);

    m_pDocMpySrc2 = CreateNewDoc(m_pDocSrc2->GetImage());
    if (!m_pDocMpySrc2) return FALSE;
    m_pDocMpySrc2->CopyRoi(m_pDocSrc2->GetImage());
    m_pDocMpySrc2->GetFrame()->ShowWindow(SW_HIDE);

    return TRUE;
}

void CRunAlpha::PrepareMpyParms()
{
   if (m_pDocMpySrc) {
      m_mpySrc  = m_pDocMpySrc->GetRoiPtr();
      m_mpySrcP = m_pDocMpySrc->GetRoiPtrP();
   } else {
      m_mpySrc = NULL;
      m_mpySrcP = NULL;
   }
   if (m_pDocMpySrc2) {
      m_mpySrc2 = m_pDocMpySrc2->GetRoiPtr();
      m_mpySrc2P = m_pDocMpySrc2->GetRoiPtrP();
   } else {
      m_mpySrc2 = NULL;
      m_mpySrc2P = NULL;
   }
}

BOOL CRunAlpha::PrepareDst()
{
    if (!PrepareMpySrc()) return FALSE;
    if (!CippiRun::PrepareDst()) return FALSE;
    PrepareMpyParms();
    return TRUE;
}

void CRunAlpha::ActivateDst()
{
   ActivateDoc(m_pDocSrc);
   ActivateDoc(m_pDocSrc2);
   if (m_showPremul) {
      ActivateDoc(m_pDocMpySrc);
      SetMpyHistory(m_pDocSrc, m_pDocMpySrc, GetMpyParms1());
      ActivateDoc(m_pDocMpySrc2);
      SetMpyHistory(m_pDocSrc2, m_pDocMpySrc2, GetMpyParms2());
   } else {
      if (m_pDocMpySrc)
         m_pDocMpySrc->OnCloseDocument();
      if (m_pDocMpySrc2)
         m_pDocMpySrc2->OnCloseDocument();
   }
   ActivateDoc(m_pDocDst);
}

CParamDlg* CRunAlpha::CreateDlg() { return new CParmAlphaDlg;}

void CRunAlpha::UpdateData(CParamDlg* pParmDlg, BOOL save)
{
   CippiRun::UpdateData(pParmDlg,save);
   CParmAlphaDlg* pDlg = (CParmAlphaDlg*)pParmDlg;
   if (save) {
      m_alphaType = (IppiAlphaType)pDlg->m_AlphaType;
      m_showPremul = pDlg->m_ShowPremul;
      m_Inplace = pDlg->m_Inplace;
   } else {
      pDlg->m_AlphaType = m_alphaType;
      pDlg->m_ShowPremul = m_showPremul;
      pDlg->m_Inplace = m_Inplace;
   }
   m_PremulName = m_Func.FuncWithBase("AlphaPremul");
   if (m_Inplace)
      m_PremulName = m_PremulName.Left(m_PremulName.GetLength() - 1) + "IR";
}

BOOL CRunAlpha::BeforeCall()
{
   m_srcComp   = pSrc;
   m_srcCompP  = pSrcP;
   m_srcComp2  = pSrc2;
   m_srcComp2P = pSrc2P;
   IppStatus result = ippStsNoErr;
   if (m_alphaType >= ippAlphaOverPremul) {
      BOOL bPlane = m_Func.SrcPlane();
      result = CallPremul(
         bPlane ? pSrcP : pSrc, srcStep,
         bPlane ? m_mpySrcP : m_mpySrc,srcStep,
         m_alpha1);
      if (result == ippStsNoErr) {
         result = CallPremul(
            bPlane ? pSrc2P : pSrc2,srcStep2,
            bPlane ? m_mpySrc2P : m_mpySrc2,srcStep2,
            m_alpha2);
      }
      if (result != ippStsNoErr) IppErrorMessage(m_PremulName, result);
      if (!m_Inplace) {
         m_srcComp   = m_mpySrc;
         m_srcCompP  = m_mpySrcP;
         m_srcComp2  = m_mpySrc2;
         m_srcComp2P = m_mpySrc2P;
      }
   }
   return result == ippStsNoErr ? TRUE : FALSE;
}

IppStatus CRunAlpha::CallPremul(void* pSrc, int srcStep,
                                void* pDst, int dstStep,
                                CValue& alpha)
{
   MATE_CALL(m_PremulName, ippiAlphaPremul_8u_AC4R,   ((Ipp8u *)pSrc, srcStep,(Ipp8u *)pDst, dstStep,roiSize))
   MATE_CALL(m_PremulName, ippiAlphaPremul_16u_AC4R,  ((Ipp16u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep,roiSize))
   MATE_CALL(m_PremulName, ippiAlphaPremul_8u_AC4IR,  ((Ipp8u *)pSrc, srcStep,roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremul_16u_AC4IR, ((Ipp16u*)pSrc, srcStep,roiSize ))

   MATE_CALL(m_PremulName, ippiAlphaPremul_8u_AP4R, ((Ipp8u**)pSrc, srcStep, (Ipp8u**)pDst, dstStep, roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremul_16u_AP4R, ((Ipp16u**)pSrc, srcStep, (Ipp16u**)pDst, dstStep, roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremul_8u_AP4IR, ((Ipp8u**)pSrc, srcStep, roiSize ))
   MATE_CALL(m_PremulName, ippiAlphaPremul_16u_AP4IR, ((Ipp16u**)pSrc, srcStep, roiSize ))

 return stsNoFunction;
}

IppStatus CRunAlpha::CallIppFunction()
{
   FUNC_CALL(ippiAlphaComp_8u_AC4R,  ((Ipp8u *)m_srcComp, srcStep,(Ipp8u *)m_srcComp2, srcStep2,(Ipp8u *)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_8u_AC1R,  ((Ipp8u *)m_srcComp, srcStep,(Ipp8u *)m_srcComp2, srcStep2,(Ipp8u *)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_8u_AP4R,  ((const Ipp8u **)m_srcCompP, srcStep,(Ipp8u **)m_srcComp2P, srcStep2, (Ipp8u **)pDstP, dstStep, roiSize, m_alphaType))

   FUNC_CALL(ippiAlphaComp_16u_AC4R, ((Ipp16u*)m_srcComp, srcStep,(Ipp16u*)m_srcComp2, srcStep2,(Ipp16u*)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_16u_AC1R, ((Ipp16u*)m_srcComp, srcStep,(Ipp16u*)m_srcComp2, srcStep2,(Ipp16u*)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_16u_AP4R, ((const Ipp16u**)m_srcCompP, srcStep,(Ipp16u**)m_srcComp2P, srcStep2, (Ipp16u**)pDstP, dstStep, roiSize, m_alphaType))

   FUNC_CALL(ippiAlphaComp_32u_AC1R,  ((Ipp32u *)m_srcComp, srcStep,(Ipp32u *)m_srcComp2, srcStep2,(Ipp32u *)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_32u_AC4R,  ((Ipp32u *)m_srcComp, srcStep,(Ipp32u *)m_srcComp2, srcStep2,(Ipp32u *)pDst, dstStep,roiSize,m_alphaType))

   FUNC_CALL(ippiAlphaComp_8s_AC1R,  ((Ipp8s *)m_srcComp, srcStep,(Ipp8s *)m_srcComp2, srcStep2,(Ipp8s *)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_8s_AC4R,  ((Ipp8s *)m_srcComp, srcStep,(Ipp8s *)m_srcComp2, srcStep2,(Ipp8s *)pDst, dstStep,roiSize,m_alphaType))

   FUNC_CALL(ippiAlphaComp_16s_AC1R, ((Ipp16s*)m_srcComp, srcStep,(Ipp16s*)m_srcComp2, srcStep2,(Ipp16s*)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_16s_AC4R, ((Ipp16s*)m_srcComp, srcStep,(Ipp16s*)m_srcComp2, srcStep2,(Ipp16s*)pDst, dstStep,roiSize,m_alphaType))

   FUNC_CALL(ippiAlphaComp_32s_AC1R,  ((Ipp32s *)m_srcComp, srcStep,(Ipp32s *)m_srcComp2, srcStep2,(Ipp32s *)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_32s_AC4R,  ((Ipp32s *)m_srcComp, srcStep,(Ipp32s *)m_srcComp2, srcStep2,(Ipp32s *)pDst, dstStep,roiSize,m_alphaType))

   FUNC_CALL(ippiAlphaComp_32f_AC1R,  ((Ipp32f *)m_srcComp, srcStep,(Ipp32f *)m_srcComp2, srcStep2,(Ipp32f *)pDst, dstStep,roiSize,m_alphaType))
   FUNC_CALL(ippiAlphaComp_32f_AC4R,  ((Ipp32f *)m_srcComp, srcStep,(Ipp32f *)m_srcComp2, srcStep2,(Ipp32f *)pDst, dstStep,roiSize,m_alphaType))
   return stsNoFunction;
}

void CRunAlpha::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   if (m_alphaType >= ippAlphaOverPremul) {
      pHisto->AddFuncString(m_PremulName, GetMpyParms1());
      if (!GetMpyParms1().IsEmpty())
         pHisto->AddFuncString(m_PremulName, GetMpyParms2());
   }
   CippiRun::AddHistoFunc(pHisto,vecPos);
}

CString CRunAlpha::GetHistoryParms()
{
   CMyString parms;
   return parms << m_alphaType;
}

void CRunAlpha::SetMpyHistory(CDemoDoc* pDocSrc, CDemoDoc* pDocDst, CString parms)
{
   if (!pDocDst) return;

   CHisto* pHisto = pDocDst->GetHisto();
   pHisto->RemoveAll();
   pHisto->AddTail(pDocSrc->GetHisto());
   pHisto->AddFuncString(m_PremulName, parms);
   pHisto->AddTail(pDocDst->GetTitle());
   pDocDst->UpdateStatusFunc(m_PremulName, m_pPerf);
}
