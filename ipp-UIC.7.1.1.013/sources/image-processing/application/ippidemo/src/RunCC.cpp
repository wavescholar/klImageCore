/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCC.cpp: implementation of the CRunCC class.
// CRunCC class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "ParmImg2Dlg.h"
#include "RunCC.h"

#include <limits.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCC::CRunCC()
{
}

CRunCC::~CRunCC()
{

}

IppStatus CRunCC::CallIppFunction()
{
   FUNC_CC_CALL(ippiGammaFwd_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_8u_C3IR, ( (Ipp8u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_8u_C3R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_8u_C3IR, ( (Ipp8u*)pSrc, srcStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaFwd_8u_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaFwd_8u_C4IR, ( (Ipp8u*)pSrc, srcStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaInv_8u_C4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaInv_8u_C4IR, ( (Ipp8u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_8u_AC4IR, ( (Ipp8u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_8u_AC4R, ( (Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_8u_AC4IR, ( (Ipp8u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_8u_P3R, ( (const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_8u_IP3R, ( (Ipp8u**)pSrcP, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_8u_P3R, ( (const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_8u_IP3R, ( (Ipp8u**)pSrcP, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_16u_C3IR, ( (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_16u_C3R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_16u_C3IR, ( (Ipp16u*)pSrc, srcStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaFwd_16u_C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaFwd_16u_C4IR, ( (Ipp16u*)pSrc, srcStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaInv_16u_C4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
//   FUNC_CC_CALL(ippiGammaInv_16u_C4IR, ( (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_16u_AC4IR, ( (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_16u_AC4R, ( (Ipp16u*)pSrc, srcStep, (Ipp16u*)pDst, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_16u_AC4IR, ( (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_16u_P3R, ( (const Ipp16u**)pSrcP, srcStep, (Ipp16u**)pDstP, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaFwd_16u_IP3R, ( (Ipp16u**)pSrcP, srcStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_16u_P3R, ( (const Ipp16u**)pSrcP, srcStep, (Ipp16u**)pDstP, dstStep, roiSize ))
   FUNC_CC_CALL(ippiGammaInv_16u_IP3R, ( (Ipp16u**)pSrcP, srcStep, roiSize ))

   FUNC_CC_CALL(ippiRGBToHLS_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToHLS_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToRGB_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))

   FUNC_CC_CALL(ippiBGRToHLS_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst, dstStep, roiSize))

   FUNC_CC_CALL(ippiBGRToHLS_8u_AP4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToHLS_8u_AP4C4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToHLS_8u_AC4P4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToHLS_8u_P3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToHLS_8u_P3C3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToHLS_8u_C3P3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))

   FUNC_CC_CALL(ippiHLSToBGR_8u_AP4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToBGR_8u_AP4C4R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToBGR_8u_AC4P4R, ((Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToBGR_8u_P3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToBGR_8u_P3C3R, ((const Ipp8u**)pSrcP, srcStep, (Ipp8u*)pDst, dstStep, roiSize))
   FUNC_CC_CALL(ippiHLSToBGR_8u_C3P3R, ((Ipp8u*)pSrc, srcStep, (Ipp8u**)pDstP, dstStep, roiSize))

   FUNC_CC_CALL(ippiRGBToYCC_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToYCC_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiYCCToRGB_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToXYZ_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiXYZToRGB_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_8u_AC4R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_16u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_16u_AC4R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_16s_C3R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_16s_AC4R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_32f_C3R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiRGBToLUV_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLUVToRGB_32f_AC4R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep, roiSize))

   FUNC_CC_CALL(ippiBGRToLab_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLabToBGR_8u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiBGRToLab_8u16u_C3R,((Ipp8u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep, roiSize))
   FUNC_CC_CALL(ippiLabToBGR_16u8u_C3R,((Ipp16u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep, roiSize))

   return stsNoFunction;
}

