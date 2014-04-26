
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRow.cpp : implementation of the CRunRowclass.
// CRunRowclass processes vectors by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunRow.h"
#include "ParmRowDlg.h"

CRunRow::CRunRow()
{
   m_maskSize = 3;
   m_anchor = 1;
   m_bCenter = TRUE;
}

CRunRow::~CRunRow()
{

}

BOOL CRunRow::Open(CFunc func) 
{
   if (!CippiRun::Open(func)) return FALSE;
   return TRUE;
}

int CRunRow::GetSrcBorder(int idx)
{
    switch (idx)
    {
    case LEFT: 
        return m_Func.Found("Row") ? m_anchor : 0;
    case RIGHT:
        return m_Func.Found("Row") ? m_maskSize - m_anchor - 1 : 0;
    case TOP: 
        return m_Func.Found("Row") ? 0 : m_anchor;
    case BOTTOM:
        return m_Func.Found("Row") ? 0 : m_maskSize - m_anchor - 1;
    }
    return 0;
}

CParamDlg* CRunRow::CreateDlg()
{
   return new CParmRowDlg;
}

void CRunRow::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg,save);
   CParmRowDlg *pDlg = (CParmRowDlg*)parmDlg;
   if (save) {
      m_maskSize = pDlg->m_maskSize;
      m_anchor   = pDlg->m_anchor  ;
      m_bCenter  = pDlg->m_bCenter ;
   } else {
      pDlg->m_maskSize = m_maskSize;
      pDlg->m_anchor   = m_anchor  ;
      pDlg->m_bCenter  = m_bCenter ;
   }
}

IppStatus CRunRow::CallIppFunction()
{
   FUNC_CALL(ippiSumWindowRow_8u32f_C1R, ((const Ipp8u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_8u32f_C3R, ((const Ipp8u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_8u32f_C4R, ((const Ipp8u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_16u32f_C1R,((const Ipp16u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_16u32f_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_16u32f_C4R,((const Ipp16u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_16s32f_C1R,((const Ipp16s*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_16s32f_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))
   FUNC_CALL(ippiSumWindowRow_16s32f_C4R,((const Ipp16s*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize,m_maskSize,m_anchor))

   FUNC_CALL(ippiSumWindowColumn_8u32f_C1R, ((const Ipp8u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_8u32f_C3R, ((const Ipp8u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_8u32f_C4R, ((const Ipp8u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_16u32f_C1R,((const Ipp16u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_16u32f_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_16u32f_C4R,((const Ipp16u*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_16s32f_C1R,((const Ipp16s*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_16s32f_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))
   FUNC_CALL(ippiSumWindowColumn_16s32f_C4R,((const Ipp16s*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize, m_maskSize, m_anchor))

   return stsNoFunction;
}

CString CRunRow::GetHistoryParms()
{
   CMyString parm;
   parm << m_maskSize << ", " << m_anchor;
   return parm;
}
