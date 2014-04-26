/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHistoEven.cpp: implementation of the CRunHistoEven class.
// CRunHistoEven class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunHistoEven.h"
#include "ParmHistoEvenDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CRunHistoEven::m_NextHistNumber = 1;

CRunHistoEven::CRunHistoEven()
{
   m_pDocHist = NULL;
   m_LUT_8u.Init32s(4, 32, 0, UCHAR_MAX + 1);
   m_LUT_16s.Init32s(4, 32, SHRT_MIN, SHRT_MAX + 1);
   m_LUT_32f.Init32f32s(4, 32, 0, 1);
}

CRunHistoEven::~CRunHistoEven()
{
}

BOOL CRunHistoEven::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

void CRunHistoEven::UpdateLUT(BOOL bSave)
{
   if (bSave) {
      switch (m_Func.SrcType()) {
      case pp8u:  m_LUT_8u  = m_LUT; break;
      case pp16s: m_LUT_16s = m_LUT; break;
      case pp32f: m_LUT_32f = m_LUT; break;
      }
   } else {
      int num = m_Func.SrcChannels() - m_Func.SrcAlpha();
      switch (m_Func.SrcType()) {
      case pp8u : m_LUT_8u .m_num = num; m_LUT = m_LUT_8u ; break;
      case pp16s: m_LUT_16s.m_num = num; m_LUT = m_LUT_16s; break;
      case pp32f: m_LUT_32f.m_num = num; m_LUT = m_LUT_32f; break;
      }
   }
}

void CRunHistoEven::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmHistoEvenDlg *pDlg = (CParmHistoEvenDlg*)parmDlg;
   if (save) {
      m_LUT.SetNumLevels(pDlg->m_LevelNumStr);
      m_LUT.m_Lower.Set(pDlg->m_LowerStr);
      m_LUT.m_Upper.Set(pDlg->m_UpperStr);
   } else {
      pDlg->m_pLUT = &m_LUT;
      m_LUT.GetNumLevels(pDlg->m_LevelNumStr);
      m_LUT.m_Lower.Get(pDlg->m_LowerStr);
      m_LUT.m_Upper.Get(pDlg->m_UpperStr);
   }
}

BOOL CRunHistoEven::CallIpp(BOOL bMessage)
{
   UpdateLUT(FALSE);
   CParmHistoEvenDlg dlg(this);
   UpdateData(&dlg, FALSE);
   if (dlg.DoModal() != IDOK)
      return FALSE;
   ShowHistogram();
   UpdateLUT();
   return TRUE;
}

BOOL CRunHistoEven::SetValues(CParamDlg* pDlg, BOOL bMessage)
{
   UpdateData(pDlg);

   if (!m_LUT.Create()) {
      if (bMessage)
         AfxMessageBox("Can't apply " + m_Func + ": nLevels is invalid");
      return FALSE;
   }

   if (!CippiRun::CallIpp(bMessage))
      return FALSE;

   UpdateData(pDlg, FALSE);
   return TRUE;
}

void CRunHistoEven::ShowHistogram()
{
   DEMO_APP->SetCursorWait();
   m_pDocHist = CreateNewDoc(m_LUT.GetImageHeader(),
                             GetNextHistTitle());
   if (!m_pDocHist) return;
   m_LUT.SetDiagram(m_pDocHist->GetImage());
   DEMO_APP->SetCursorArrow();
}

void CRunHistoEven::ActivateDst()
{
   ActivateDoc(m_pDocSrc);
   ActivateDoc(m_pDocHist);
}

void CRunHistoEven::SetHistory()
{
   CippiRun::SetToHisto(m_pDocHist,  VEC_DST);
}

CString CRunHistoEven::GetNextHistTitle()
{
   CString title;
   title.Format("Hist%d", m_NextHistNumber++);
   return title;
}

IppStatus CRunHistoEven::CallIppFunction()
{
   FUNC_CALL(ippiHistogramEven_8u_C1R,  ((Ipp8u *)pSrc, srcStep, roiSize, (Ipp32s* )m_LUT.m_pHist[0], (Ipp32s* )m_LUT.m_pLevels[0], m_LUT.m_nLevels[0], ((Ipp32s*)m_LUT.m_Lower)[0], ((Ipp32s*)m_LUT.m_Upper)[0]))
   FUNC_CALL(ippiHistogramEven_8u_C3R,  ((Ipp8u *)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_8u_AC4R, ((Ipp8u *)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_8u_C4R,  ((Ipp8u *)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp32s* )m_LUT.m_pHist[0], (Ipp32s* )m_LUT.m_pLevels[0], m_LUT.m_nLevels[0], ((Ipp32s*)m_LUT.m_Lower)[0], ((Ipp32s*)m_LUT.m_Upper)[0]))
   FUNC_CALL(ippiHistogramEven_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_16u_AC4R,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp32s* )m_LUT.m_pHist[0], (Ipp32s* )m_LUT.m_pLevels[0], m_LUT.m_nLevels[0], ((Ipp32s*)m_LUT.m_Lower)[0], ((Ipp32s*)m_LUT.m_Upper)[0]))
   FUNC_CALL(ippiHistogramEven_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_16s_AC4R,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   FUNC_CALL(ippiHistogramEven_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, (Ipp32s**)m_LUT.m_pHist,    (Ipp32s**)m_LUT.m_pLevels,    m_LUT.m_nLevels,    (Ipp32s*)m_LUT.m_Lower,    (Ipp32s*)m_LUT.m_Upper))
   return stsNoFunction;
}

CString CRunHistoEven::GetHistoryParms()
{
   CMyString parms;
   parms << ".., "
         << m_LUT.LevelNumString() << ", "
         << m_LUT.LowerString() << ", "
         << m_LUT.UpperString();
   return parms;
}

