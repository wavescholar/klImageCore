/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThresh.cpp: implementation of the CRunThresh class.
// CRunThresh class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunThresh.h"
#include "ParmThreshDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRunThresh::CRunThresh()
{
   m_numCoi = 0;

   m_cmpOp = ippCmpLess;

   m_threshLT.Init(pp8u, 4);
   m_threshGT.Init(pp8u, 4);
   m_valueLT .Init(pp8u, 4);
   m_valueGT .Init(pp8u, 4);

   m_threshLT.Init(pp16u, 4);
   m_threshGT.Init(pp16u, 4);
   m_valueLT .Init(pp16u, 4);
   m_valueGT .Init(pp16u, 4);
   
   m_threshLT.Init(pp16s, 4);
   m_threshGT.Init(pp16s, 4);
   m_valueLT .Init(pp16s, 4);
   m_valueGT .Init(pp16s, 4);

   m_threshLT.Init(pp32f, 4);
   m_threshGT.Init(pp32f, 4);
   m_valueLT .Init(pp32f, 4);
   m_valueGT .Init(pp32f, 4);

   for (int i=0; i<4; i++) {
      m_threshLT.Vector(pp8u).Set(i, 64);
      m_threshGT.Vector(pp8u).Set(i, 192);
      m_valueLT .Vector(pp8u).Set(i, 0);
      m_valueGT .Vector(pp8u).Set(i, 255);

      m_threshLT.Vector(pp16u).Set(i, 16000);
      m_threshGT.Vector(pp16u).Set(i, 48000);
      m_valueLT .Vector(pp16u).Set(i, 0);
      m_valueGT .Vector(pp16u).Set(i, 64000);

      m_threshLT.Vector(pp16s).Set(i, -16000);
      m_threshGT.Vector(pp16s).Set(i, 16000);
      m_valueLT .Vector(pp16s).Set(i, -32000);
      m_valueGT .Vector(pp16s).Set(i, 32000);


      m_threshLT.Vector(pp32f).Set(i, 0.25);
      m_threshGT.Vector(pp32f).Set(i, 0.75);
      m_valueLT .Vector(pp32f).Set(i, 0);
      m_valueGT .Vector(pp32f).Set(i, 1);
   }

}

CRunThresh::~CRunThresh()
{

}

BOOL CRunThresh::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_numCoi = m_Func.SrcChannels() - m_Func.SrcAlpha();
   m_threshLT.Init(m_Func.SrcType());
   m_threshGT.Init(m_Func.SrcType());
   m_valueLT .Init(m_Func.SrcType());
   m_valueGT   .Init(m_Func.SrcType());
   return TRUE;
}

CParamDlg* CRunThresh::CreateDlg() { return new CParmThreshDlg;}

void CRunThresh::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmThreshDlg* pDlg = (CParmThreshDlg*)parmDlg;
   if (save) {
      m_cmpOp = (IppCmpOp)pDlg->m_cmpOp;
      m_valueLT .Set(pDlg->m_ValueLTStr, m_numCoi);
      m_valueGT .Set(pDlg->m_ValueGTStr, m_numCoi);
      m_threshLT.Set(pDlg->m_ThreshLTStr, m_numCoi);
      m_threshGT.Set(pDlg->m_ThreshGTStr, m_numCoi);
   } else {
      pDlg->m_cmpOp = (int)m_cmpOp;
      m_valueLT .Get(pDlg->m_ValueLTStr, m_numCoi);
      m_valueGT .Get(pDlg->m_ValueGTStr, m_numCoi);
      m_threshLT.Get(pDlg->m_ThreshLTStr, m_numCoi);
      m_threshGT.Get(pDlg->m_ThreshGTStr, m_numCoi);
   }
}

IppStatus CRunThresh::CallIppFunction()
{
   FUNC_CALL(ippiThreshold_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_8u_C4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, m_cmpOp))
//   FUNC_CALL(ippiThreshold_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_GT_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_8u_C4R,((Ipp8u*)pSrc, srcStep, //    (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_16u_C4R,((Ipp16u*)pSrc, srcStep, //    (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_16s_C4R,((Ipp16s*)pSrc, srcStep, //    (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_32f_C4R,((Ipp32f*)pSrc, srcStep, //    (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_8u_C4IR,((Ipp8u*)pSrc, srcStep, //    roiSize, (Ipp8u*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_16u_C4IR,((Ipp16u*)pSrc, srcStep, //    roiSize, (Ipp16u*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_16s_C4IR,((Ipp16s*)pSrc, srcStep, //    roiSize, (Ipp16s*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_32f_C4IR,((Ipp32f*)pSrc, srcStep, //    roiSize, (Ipp32f*)m_threshGT))
   //FUNC_CALL(ippiThreshold_GT_8u_AC4IR,((Ipp8u*)pSrc, srcStep, //    roiSize, (Ipp8u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshGT))
   FUNC_CALL(ippiThreshold_GT_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshGT))
   FUNC_CALL(ippiThreshold_LT_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_8u_C4R,((Ipp8u*)pSrc, srcStep, //    (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_16u_C4R,((Ipp16u*)pSrc, srcStep, //    (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_16s_C4R,((Ipp16s*)pSrc, srcStep, //    (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_32f_C4R,((Ipp32f*)pSrc, srcStep, //    (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_8u_C4IR,((Ipp8u*)pSrc, srcStep, //    roiSize, (Ipp8u*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_16u_C4IR,((Ipp16u*)pSrc, srcStep, //    roiSize, (Ipp16u*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_16s_C4IR,((Ipp16s*)pSrc, srcStep, //    roiSize, (Ipp16s*)m_threshLT))
   //FUNC_CALL(ippiThreshold_LT_32f_C4IR,((Ipp32f*)pSrc, srcStep, //    roiSize, (Ipp32f*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT))
   FUNC_CALL(ippiThreshold_LT_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT))
   FUNC_CALL(ippiThreshold_Val_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshLT, (Ipp8u)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u)m_threshLT, (Ipp16u)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshLT, (Ipp16s)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshLT, (Ipp32f)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_8u_C4R,((Ipp8u*)pSrc, srcStep, //    (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, //    (Ipp8u*)m_valueLT, m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_16u_C4R,((Ipp16u*)pSrc, srcStep, //    (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, //    (Ipp16u*)m_valueLT, m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_16s_C4R,((Ipp16s*)pSrc, srcStep, //    (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, //    (Ipp16s*)m_valueLT, m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_32f_C4R,((Ipp32f*)pSrc, srcStep, //    (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, //    (Ipp32f*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshLT, (Ipp8u)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshLT, (Ipp16u)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshLT, (Ipp16s)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshLT, (Ipp32f)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_8u_C4IR,((Ipp8u*)pSrc, srcStep, //    roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, //    m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_16u_C4IR,((Ipp16u*)pSrc, srcStep, //    roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, //    m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_16s_C4IR,((Ipp16s*)pSrc, srcStep, //    roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, //    m_cmpOp))
   //FUNC_CALL(ippiThreshold_Val_32f_C4IR,((Ipp32f*)pSrc, srcStep, //    roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, //    m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_Val_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, m_cmpOp))
   FUNC_CALL(ippiThreshold_GTVal_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshGT, (Ipp8u)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u)m_threshGT, (Ipp16u)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshGT, (Ipp16s)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshGT, (Ipp32f)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshGT, (Ipp8u)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshGT, (Ipp16u)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshGT, (Ipp16s)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshGT, (Ipp32f)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_C4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_GTVal_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTVal_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshLT, (Ipp8u)m_valueLT)) 
   FUNC_CALL(ippiThreshold_LTVal_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u)m_threshLT, (Ipp16u)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshLT, (Ipp16s)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshLT, (Ipp32f)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_C3R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_C4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_AC4R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshLT, (Ipp8u)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshLT, (Ipp16u)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshLT, (Ipp16s)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshLT, (Ipp32f)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_C3IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_C4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_8u_AC4IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTVal_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT))
   FUNC_CALL(ippiThreshold_LTValGTVal_8u_C1R,((Ipp8u*)pSrc,srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u)m_threshLT, (Ipp8u)m_valueLT, (Ipp8u)m_threshGT, (Ipp8u)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16u_C1R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u)m_threshLT, (Ipp16u)m_valueLT, (Ipp16u)m_threshGT, (Ipp16u)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16s_C1R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_threshLT, (Ipp16s)m_valueLT, (Ipp16s)m_threshGT, (Ipp16s)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_32f_C1R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f)m_threshLT, (Ipp32f)m_valueLT, (Ipp32f)m_threshGT, (Ipp32f)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_8u_C3R,((Ipp8u*)pSrc,srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16u_C3R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16s_C3R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_32f_C3R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_8u_C4R,((Ipp8u*)pSrc,srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_16u_C4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_16s_C4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_32f_C4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_8u_AC4R,((Ipp8u*)pSrc,srcStep, (Ipp8u*)pDst, dstStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16u_AC4R,((Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16s_AC4R,((Ipp16s*)pSrc, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_32f_AC4R,((Ipp32f*)pSrc, srcStep, (Ipp32f*)pDst, dstStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_8u_C1IR,((Ipp8u*)pSrc, srcStep, roiSize, (Ipp8u)m_threshLT, (Ipp8u)m_valueLT, (Ipp8u)m_threshGT, (Ipp8u)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16u_C1IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u)m_threshLT, (Ipp16u)m_valueLT, (Ipp16u)m_threshGT, (Ipp16u)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16s_C1IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s)m_threshLT, (Ipp16s)m_valueLT, (Ipp16s)m_threshGT, (Ipp16s)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_32f_C1IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f)m_threshLT, (Ipp32f)m_valueLT, (Ipp32f)m_threshGT, (Ipp32f)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_8u_C3IR,((Ipp8u*)pSrc,srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16u_C3IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16s_C3IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_32f_C3IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_8u_C4IR,((Ipp8u*)pSrc,srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_16u_C4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_16s_C4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
//   FUNC_CALL(ippiThreshold_LTValGTVal_32f_C4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_8u_AC4IR,((Ipp8u*)pSrc,srcStep, roiSize, (Ipp8u*)m_threshLT, (Ipp8u*)m_valueLT, (Ipp8u*)m_threshGT, (Ipp8u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16u_AC4IR,((Ipp16u*)pSrc, srcStep, roiSize, (Ipp16u*)m_threshLT, (Ipp16u*)m_valueLT, (Ipp16u*)m_threshGT, (Ipp16u*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_16s_AC4IR,((Ipp16s*)pSrc, srcStep, roiSize, (Ipp16s*)m_threshLT, (Ipp16s*)m_valueLT, (Ipp16s*)m_threshGT, (Ipp16s*)m_valueGT))
   FUNC_CALL(ippiThreshold_LTValGTVal_32f_AC4IR,((Ipp32f*)pSrc, srcStep, roiSize, (Ipp32f*)m_threshLT, (Ipp32f*)m_valueLT, (Ipp32f*)m_threshGT, (Ipp32f*)m_valueGT))

   return stsNoFunction;
}

static CMyString valString(CVectorUnit& unit, int num)
{
   CMyString* vals = new CMyString[num];
   unit.Get(vals, num);
   if (num == 1) return vals[0];
   CMyString str = "{";
   for (int i=0; i<num; i++) {
      if (i) str += ", ";
      str += vals[i];
   }
   str += "}";
   delete[] vals;
   return str;
}

CString CRunThresh::GetHistoryParms()
{
   CMyString parms;
   BOOL bLT = m_Func.Found("LT");
   BOOL bGT = m_Func.Found("GT");
   BOOL bVal = m_Func.Found("Val");
   BOOL bCmp = !bLT && !bGT;
   if (bLT || bCmp) {
      parms << valString(m_threshLT,m_numCoi);
      if (bVal)
         parms << ", " << valString(m_valueLT,m_numCoi);
   } else if (bGT) {
      if (!parms.IsEmpty())
         parms << ", ";
      parms << valString(m_threshGT,m_numCoi);
      if (bVal)
         parms << ", " << valString(m_valueGT,m_numCoi);
   }
   if (bCmp) {
      parms << ", " << m_cmpOp;
   }
   return parms;
}
