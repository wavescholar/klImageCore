/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStatIdx.cpp: implementation of the CRunStatIdx class.
// CRunStatIdx class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunStatIdx.h"
#include "ParmStatIdxDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunStatIdx::CRunStatIdx()
{
   for (int i=0; i<4; i++) {
      m_indexX[i] = 0;
      m_indexY[i] = 0;
   }
}

CRunStatIdx::~CRunStatIdx()
{

}

BOOL CRunStatIdx::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

void CRunStatIdx::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmStatIdxDlg *pDlg = (CParmStatIdxDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
   } else {
      m_value.Get(pDlg->m_valueStr);
      for (int i=0; i<4; i++) {
         pDlg->m_xStr[i].Format("%d", m_indexX[i]);
         pDlg->m_yStr[i].Format("%d", m_indexY[i]);
      }
   }
}

BOOL CRunStatIdx::CallIpp(BOOL bMessage)
{
   CParmStatIdxDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunStatIdx::SetValues(CParmStatIdxDlg* pDlg)
{
   UpdateData(pDlg);
   m_value.Init(m_Func.SrcType(),4);
   PrepareParameters();
   if (CippiRun::CallIpp() < 0) return;
   m_pDocSrc->GetImage()->SetMarker(m_indexX, m_indexY, m_Func.Found("Max"));
   m_pDocSrc->GetImage()->IsUpdated(TRUE);
   m_pDocSrc->UpdateView();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunStatIdx::CallIppFunction()
{
   FUNC_CALL(ippiMinIndx_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16u_AC4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_16s_AC4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMinIndx_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_8u_AC4R, ((Ipp8u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16u_AC4R, ((Ipp16u*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_16s_AC4R, ((Ipp16s*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   FUNC_CALL(ippiMaxIndx_32f_AC4R, ((Ipp32f*)pSrc, srcStep, roiSize, m_value, m_indexX, m_indexY))
   return stsNoFunction;
}
