/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunYuv.cpp: implementation of the CRunYuv class.
// CRunYuv class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunYuv.h"
#include "ParmGammaDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunYuv::CRunYuv()
{
   m_alpha = 192;
}

CRunYuv::~CRunYuv()
{

}

CParamDlg* CRunYuv::CreateDlg() {
   if (m_Func.DescrName().Found("C4R") && !m_Func.DescrName().Found("A"))
      return new CParmGammaDlg;
   else
      return NULL;
}

void CRunYuv::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmGammaDlg* pDlg = (CParmGammaDlg*)parmDlg;
    if (save) {
       m_alpha = (Ipp8u)atoi(pDlg->m_Str0);
    } else {
       pDlg->m_is1 = FALSE;
       pDlg->m_Name0 = "Alpha";
       pDlg->m_Str0.Format("%d", m_alpha);
    }
}

ImgHeader CRunYuv::GetNewDstHeader()
{
   ImgHeader header = CippiRun::GetNewDstHeader();
   if (!header.plane) return header;
   if (m_Func.Found("422_")) {
      header.SetSize422(header.width, header.height);
   } else if (m_Func.Found("420_")) {
      header.SetSize420(header.width, header.height);
   } else {
      header.SetSize444(header.width, header.height);
   }
   return header;
}

BOOL CRunYuv::PrepareDst()
{
   if (!CippiRun::PrepareDst()) return FALSE;
   if (m_pDocDst->GetImage()->Channels() == 1) {
      if (m_Func.Found("ToRGB444") || m_Func.Found("ToBGR444"))
         m_pDocDst->GetImage()->Compressed(RGB444);
      else if (m_Func.Found("ToRGB555") || m_Func.Found("ToBGR555"))
         m_pDocDst->GetImage()->Compressed(RGB555);
      else if (m_Func.Found("ToRGB565") || m_Func.Found("ToBGR565"))
         m_pDocDst->GetImage()->Compressed(RGB565);
   }
   return TRUE;
}

IppStatus CRunYuv::CallIppFunction()
{
   FUNC_CC_CALL( ippiCbYCr422ToBGR_8u_C2C4R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize, m_alpha))
  FUNC_CC_CALL(ippiYCbCr411ToBGR_8u_P3C4R,((const Ipp8u**)pSrcP,srcStepP,
 (Ipp8u*)pDst, dstStep, roiSize, m_alpha))
   FUNC_CC_CALL( ippiYCbCr422ToBGR_8u_C2C4R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst,dstStep, roiSize, m_alpha ))
   FUNC_CC_CALL( ippiYCbCrToBGR_8u_P3C4R,( (const Ipp8u**)pSrcP,srcStep,(Ipp8u*)pDst,dstStep,roiSize, m_alpha ))

   FUNC_CC_CALL( ippiBGRToCbYCr422_8u_AC4C2R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr411ToBGR_8u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp8u*)pDst,
 dstStep, roiSize))
   FUNC_CC_CALL( ippiCbYCr422ToRGB_8u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToCbYCr422Gamma_8u_C3C2R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToCbYCr422_8u_C3C2R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB_8u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCbCr422_8u_C3C2R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB_8u_C2C3R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCbCr422_8u_P3C2R,((const Ipp8u**)pSrcP, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB_8u_C2P3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR_8u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB_8u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCbCr420_8u_C3P3R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u**)pDstP,dstStepP, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB565_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR565_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB555_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR555_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB444_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR444_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR565_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB565_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR444_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB444_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR555_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB555_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR565_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB565_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR555_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB555_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR444_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB444_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR565_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB565_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR555_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB555_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR444_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB444_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR565Dither_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB565Dither_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR555Dither_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB555Dither_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR444Dither_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB444Dither_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToBGR444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToRGB444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB555Dither_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR555Dither_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB565Dither_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR565Dither_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToRGB444Dither_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToBGR444Dither_8u16u_C2C3R,((const Ipp8u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB444Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB555Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB565Dither_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGR565ToYUV420_16u8u_C3P3R,( (const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR555ToYUV420_16u8u_C3P3R,( (const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToBGR_8u_C2C3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst,dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCbCrToBGR_8u_P3C3R,( (const Ipp8u**)pSrcP,srcStep,(Ipp8u*)pDst,dstStep,roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToBGR565_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst,dstStep,roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToBGR555_8u16u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,(Ipp16u*)pDst,dstStep,roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr411_8u_C3P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr411_8u_AC4P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR565ToYCbCr411_16u8u_C3P3R,((const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR555ToYCbCr411_16u8u_C3P3R,((const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr422_8u_C3P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize))
   FUNC_CC_CALL( ippiBGRToYCbCr422_8u_AC4P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize))
   FUNC_CC_CALL( ippiBGR565ToYCbCr422_16u8u_C3P3R,((const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize))
   FUNC_CC_CALL( ippiBGR555ToYCbCr422_16u8u_C3P3R,((const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr420_8u_C3P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr420_8u_AC4P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR565ToYCbCr420_16u8u_C3P3R,((const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR555ToYCbCr420_16u8u_C3P3R,((const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr422_8u_C3C2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCbCr422_8u_AC4C2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiBGR555ToYCbCr422_16u8u_C3C2R,( (const Ipp16u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiBGR565ToYCbCr422_16u8u_C3C2R,( (const Ipp16u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCrCb420_8u_C3P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGRToYCrCb420_8u_AC4P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR555ToYCrCb420_16u8u_C3P3R,( (const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiBGR565ToYCrCb420_16u8u_C3P3R,( (const Ipp16u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL ( ippiYCbCr422_8u_P3C2R, ( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr422_8u_P2C2R,((const Ipp8u*)(pSrcP[0]), srcStepP[0],(const Ipp8u*)(pSrcP[1]),
 srcStepP[1], (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr420ToYCrCb420_8u_P2P3R,((const Ipp8u*)(pSrcP[0]), srcStepP[0],(const Ipp8u*)(pSrcP[1]),
 srcStepP[1], (Ipp8u**)pDstP, dstStepP, roiSize))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr420_8u_C2P2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)(pDstP[0]), dstStepP[0],(Ipp8u*)(pDstP[1]),dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiCbYCr422ToYCbCr420_8u_C2P2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)(pDstP[0]), dstStepP[0],(Ipp8u*)(pDstP[1]),dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCrCb420_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,dstStepP, roiSize ))
   FUNC_CC_CALL( ippiCbYCr422ToYCrCb420_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToCbYCr422_8u_C2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiCbYCr422ToYCbCr422_8u_C2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCrCb420ToCbYCr422_8u_P3C2R,( (const Ipp8u**)pSrcP,srcStepP, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCrCb420ToYCbCr422_8u_P3C2R,( (const Ipp8u**)pSrcP,srcStepP, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCrCb420ToYCbCr420_8u_P3P2R,( (const Ipp8u**)pSrcP,srcStepP, (Ipp8u*)(pDstP[0]), dstStepP[0],(Ipp8u*)(pDstP[1]),dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToCbYCr422_8u_P2C2R,( (const Ipp8u*)(pSrcP[0]), srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1], (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCr422_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCrCb422ToYCbCr422_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiCbYCr422ToYCbCr422_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr420_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiCbYCr422ToYCbCr420_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCrCb422ToYCbCr420_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr411_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiCbYCr422ToYCbCr411_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCrCb422ToYCbCr411_8u_C2P3R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr420_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr420_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr411_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr411_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCrCb420ToYCbCr411_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr420_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr422_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCrCb420ToYCbCr422_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr422_Filter_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCrCb420ToYCbCr422_Filter_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr422_Filter_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
 (Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr422_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr420_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr420ToYCbCr411_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr422_8u_P3C2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)pDst,
 dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCrCb422_8u_P3C2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)pDst,
 dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr422_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCrCb422_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr420_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr420_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr411_8u_P3P2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr422_8u_P2C2R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr422_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCrCb420_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411ToYCbCr420_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr411_8u_P2P3R,((const Ipp8u*)(pSrcP[0]),srcStepP[0],(const Ipp8u*)(pSrcP[1]), srcStepP[1],
(Ipp8u**)pDstP, dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr411_8u_C2P2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)(pDstP[0]),
 dstStepP[0],(Ipp8u*)(pDstP[1]), dstStepP[1], roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCbCr411_8u_P3R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u**)pDstP,
 dstStepP, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCrCb422_8u_P3C2R,( (const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)pDst,
 dstStep, roiSize ))
   FUNC_CC_CALL( ippiYCbCr422ToYCrCb422_8u_C2R,( (const Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst,
 dstStep, roiSize ))
   FUNC_CC_CALL( ippiRGBToYCbCr_8u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCbCr_8u_AC4R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCbCr_8u_P3R,((const Ipp8u**)pSrcP, srcStep,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB_8u_P3C3R,((const Ipp8u**)pSrcP,srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB_8u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB_8u_AC4R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB_8u_P3R,((const Ipp8u**)pSrcP,srcStep,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR444_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB444_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR555_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB555_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToBGR565_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCbCrToRGB565_8u16u_C3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV_8u_C3R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUVToRGB_8u_C3R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV_8u_AC4R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUVToRGB_8u_AC4R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV_8u_P3R,((const Ipp8u**)pSrcP, srcStep ,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUVToRGB_8u_P3R,((const Ipp8u**)pSrcP, srcStep,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV_8u_C3P3R,( (const Ipp8u*)pSrc, srcStep ,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUVToRGB_8u_P3C3R,((const Ipp8u**)pSrcP, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB_8u_P3AC4R,((const Ipp8u**)pSrcP,srcStepP,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV422ToRGB_8u_P3AC4R,((const Ipp8u**)pSrcP,srcStepP,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV422_8u_C3P3R,((const Ipp8u*)pSrc, srcStep,
 (Ipp8u**)pDstP,dstStepP, roiSize))
   FUNC_CC_CALL( ippiYUV422ToRGB_8u_P3C3R,((const Ipp8u**)pSrcP,srcStepP,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV422_8u_P3R,((const Ipp8u**)pSrcP, srcStep ,
 (Ipp8u**)pDstP, dstStepP,roiSize))
   FUNC_CC_CALL( ippiYUV422ToRGB_8u_P3R,((const Ipp8u**)pSrcP,
 srcStepP,(Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV420_8u_C3P3R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u**)pDstP, dstStepP, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB_8u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR_8u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV420_8u_P3R,((const Ipp8u**)pSrcP, srcStep ,
 (Ipp8u**)pDstP, dstStepP, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB_8u_P3R,((const Ipp8u**)pSrcP,srcStepP,
 (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV422_8u_C3C2R,((const Ipp8u*)pSrc, srcStep ,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV422ToRGB_8u_C2C3R,((const Ipp8u*)pSrc,srcStep,
 (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR565_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR555_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToBGR444_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB565_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB555_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB444_8u16u_P3C3R,((const Ipp8u**)pSrcP, srcStepP, (Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV422_8u_P3,((const Ipp8u**)pSrcP, (Ipp8u**)pDstP, roiSize))
   FUNC_CC_CALL( ippiYUV422ToRGB_8u_P3,((const Ipp8u**)pSrcP, (Ipp8u**)pDstP, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV422_8u_C3P3,((const Ipp8u*)pSrc, (Ipp8u**)pDstP, roiSize))
   FUNC_CC_CALL( ippiYUV422ToRGB_8u_P3C3,((const Ipp8u**)pSrcP,(Ipp8u*)pDst, roiSize ))
   FUNC_CC_CALL( ippiRGBToYUV420_8u_C3P3,((const Ipp8u*)pSrc, (Ipp8u**)pDstP, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB_8u_P3C3,((const Ipp8u**)pSrcP, (Ipp8u*)pDst, roiSize))
   FUNC_CC_CALL( ippiRGBToYUV420_8u_P3,((const Ipp8u**)pSrcP, (Ipp8u**)pDstP, roiSize))
   FUNC_CC_CALL( ippiYUV420ToRGB_8u_P3,((const Ipp8u**)pSrcP, (Ipp8u**)pDstP, roiSize))
   FUNC_CC_CALL(ippiRGBToGray_8u_C3C1R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_16u_C3C1R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_16s_C3C1R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_32f_C3C1R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_8u_AC4C1R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_16u_AC4C1R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_16s_AC4C1R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep,roiSize))
   FUNC_CC_CALL(ippiRGBToGray_32f_AC4C1R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep,roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHLS_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToRGB_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_AP4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_AP4C4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_AC4P4R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_P3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_P3C3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToHLS_8u_C3P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToBGR_8u_AP4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToBGR_8u_AP4C4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToBGR_8u_AC4P4R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToBGR_8u_P3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToBGR_8u_P3C3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHLSToBGR_8u_C3P3R, ((const Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHSV_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHSVToRGB_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHSV_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHSVToRGB_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHSV_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHSVToRGB_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToHSV_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiHSVToRGB_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToYCC_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiYCCToRGB_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_16u_C3R,((const Ipp16u*)pSrc, srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToXYZ_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiXYZToRGB_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_8u_AC4R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_16u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_16u_AC4R,((const Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_16s_C3R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_16s_AC4R,((const Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_32f_C3R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiRGBToLUV_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiLUVToRGB_32f_AC4R,((const Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst, dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToLab_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL( ippiLabToBGR_8u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL( ippiBGRToLab_8u16u_C3R,((const Ipp8u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL( ippiLabToBGR_16u8u_C3R,((const Ipp16u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))


   FUNC_CC_CALL(ippiYCoCgToBGR_16s8u_P3C3R,((const Ipp16s**)pSrcP,srcStep,(Ipp8u*)pDst,dstStep,roiSize))
   FUNC_CC_CALL(ippiYCoCgToBGR_16s8u_P3C4R,((const Ipp16s**)pSrcP,srcStep,(Ipp8u*)pDst,dstStep,roiSize, (Ipp8u)m_alpha))
   FUNC_CC_CALL(ippiYCoCgToSBGR_16s_P3C3R, ((const Ipp16s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiYCoCgToSBGR_16s_P3C4R, ((const Ipp16s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_alpha))
   FUNC_CC_CALL(ippiYCoCgToSBGR_32s16s_P3C3R,((const Ipp32s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiYCoCgToSBGR_32s16s_P3C4R,((const Ipp32s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_alpha))
   FUNC_CC_CALL(ippiYCoCgToBGR_Rev_16s8u_P3C3R,((const Ipp16s**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiYCoCgToBGR_Rev_16s8u_P3C4R,((const Ipp16s**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize,(Ipp8u)m_alpha))
   FUNC_CC_CALL(ippiYCoCgToSBGR_Rev_16s_P3C3R,((const Ipp16s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiYCoCgToSBGR_Rev_16s_P3C4R,((const Ipp16s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize,(Ipp16s)m_alpha))
   FUNC_CC_CALL(ippiYCoCgToSBGR_Rev_32s16s_P3C4R,((const Ipp32s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize, (Ipp16s)m_alpha))
   FUNC_CC_CALL(ippiYCoCgToSBGR_Rev_32s16s_P3C3R,((const Ipp32s**)pSrcP, srcStep, (Ipp16s*)pDst, dstStep, roiSize))

   FUNC_CC_CALL(ippiBGRToYCoCg_8u16s_C3P3R,((const Ipp8u *)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToYCoCg_8u16s_C4P3R,((const Ipp8u *)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_16s_C3P3R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_16s_C3P3R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_16s_C4P3R, ((const Ipp16s*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_16s32s_C3P3R,((const Ipp16s*)pSrc, srcStep, (Ipp32s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_16s32s_C4P3R,((const Ipp16s*)pSrc, srcStep, (Ipp32s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToYCoCg_Rev_8u16s_C3P3R,((const Ipp8u*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToYCoCg_Rev_8u16s_C4P3R,((const Ipp8u*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_Rev_16s_C3P3R,((const Ipp16s*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_Rev_16s_C4P3R,((const Ipp16s*)pSrc, srcStep, (Ipp16s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_Rev_16s32s_C3P3R,((const Ipp16s*)pSrc, srcStep, (Ipp32s**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiSBGRToYCoCg_Rev_16s32s_C4P3R,((const Ipp16s*)pSrc, srcStep, (Ipp32s**)pDstP, dstStep, roiSize))
   return stsNoFunction;
}
 
