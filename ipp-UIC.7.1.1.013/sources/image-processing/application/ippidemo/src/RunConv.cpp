/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConv.cpp: implementation of the CRunConv class.
// CRunConv class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunConv.h"
#include "ParmConvDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunConv::CRunConv()
{
   m_divisor = 1;
}

CRunConv::~CRunConv()
{

}

BOOL CRunConv::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

CParamDlg* CRunConv::CreateDlg() { return new CParmConvDlg;}

void CRunConv::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippiRun::UpdateData(parmDlg, save);
   CParmConvDlg* pDlg = (CParmConvDlg*)parmDlg;
   if (save) {
      m_divisor = atoi(pDlg->m_DivStr);
   } else {
      pDlg->m_DivStr.Format("%d",m_divisor);
   }
}

ImgHeader CRunConv::GetNewDstHeader()
{
    ImgHeader header;
    header = m_pDocSrc->GetImage()->GetHeader();
    IppiRect roi1 = m_pDocSrc->GetImage()->GetActualRoi();
    IppiRect roi2 = m_pDocSrc2->GetImage()->GetActualRoi();
    if (m_Func.Found("Full")) {
       header.SetSize(roi1.width  + roi2.width  - 1,
                      roi1.height + roi2.height - 1);
    } else if (m_Func.Found("Valid")) {
      int width1  = roi1.width;
      int width2  = roi2.width;
      int height1 = roi1.height;
      int height2 = roi2.height;
      if (width1 <= width2 && height1 <= height2) {
         header.SetSize(width2  - width1  + 1,
                        height2 - height1 + 1);
      } else if (width1 >= width2 && height1 >= height2) {
         header.SetSize(width1  - width2  + 1,
                        height1 - height2 + 1);
      } else {
         header.SetSize(1, 1);
      }
    }
    return header;
}

void CRunConv::CopyContoursToNewDst()
{
}

IppStatus CRunConv::CallIppFunction()
{
   FUNC_CALL(ippiConvFull_32f_C1R,( (Ipp32f*)pSrc, srcStep,
        m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiConvFull_32f_C3R,( (Ipp32f*)pSrc, srcStep,
        m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiConvFull_32f_AC4R,( (Ipp32f*)pSrc, srcStep,
        m_srcRoiSize, (Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiConvFull_16s_C1R,( (Ipp16s*)pSrc, srcStep,
        m_srcRoiSize, (Ipp16s*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp16s*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvFull_16s_C3R,( (Ipp16s*)pSrc, srcStep,
        m_srcRoiSize, (Ipp16s*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp16s*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvFull_16s_AC4R,( (Ipp16s*)pSrc, srcStep,
        m_srcRoiSize, (Ipp16s*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp16s*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvFull_8u_C1R,( (Ipp8u*)pSrc, srcStep,
        m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp8u*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvFull_8u_C3R,( (Ipp8u*)pSrc, srcStep,
        m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp8u*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvFull_8u_AC4R,( (Ipp8u*)pSrc, srcStep,
        m_srcRoiSize, (Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp8u*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvValid_32f_C1R,((Ipp32f*)pSrc, srcStep,
        m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiConvValid_32f_C3R,((Ipp32f*)pSrc, srcStep,
        m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiConvValid_32f_AC4R,((Ipp32f*)pSrc, srcStep,
        m_srcRoiSize,(Ipp32f*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp32f*)pDst, dstStep ))
   FUNC_CALL(ippiConvValid_16s_C1R,((Ipp16s*)pSrc, srcStep,
        m_srcRoiSize,(Ipp16s*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp16s*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvValid_16s_C3R,((Ipp16s*)pSrc, srcStep,
        m_srcRoiSize,(Ipp16s*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp16s*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvValid_16s_AC4R,((Ipp16s*)pSrc, srcStep,
        m_srcRoiSize,(Ipp16s*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp16s*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvValid_8u_C1R,((Ipp8u*)pSrc, srcStep,
        m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp8u*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvValid_8u_C3R,((Ipp8u*)pSrc, srcStep,
        m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp8u*)pDst, dstStep, m_divisor ))
   FUNC_CALL(ippiConvValid_8u_AC4R,((Ipp8u*)pSrc, srcStep,
        m_srcRoiSize,(Ipp8u*)pSrc2, srcStep2, m_srcRoiSize2,
        (Ipp8u*)pDst, dstStep, m_divisor ))
   return stsNoFunction;
}
