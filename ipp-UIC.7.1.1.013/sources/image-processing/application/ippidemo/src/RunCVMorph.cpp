/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVMorph.cpp: implementation of the CRunCVMorph class.
// CRunCVMorph class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Histo.h"
#include "RunCVMorph.h"
#include "ParmCVMorphDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVMorph::CRunCVMorph()
{
   m_pState = NULL;
   m_borderType = ippBorderRepl;
}

CRunCVMorph::~CRunCVMorph()
{

}

//IppiShape CRunCVMorph::GetShape(CFunc func)
//{
//   if      (func.Found("Rect"   )) return ippiShapeRect   ;
//   else if (func.Found("Cross"  )) return ippiShapeCross  ;
//   else if (func.Found("Ellipse")) return ippiShapeEllipse;
//   else return ippiShapeCustom;
//}

BOOL CRunCVMorph::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;
   m_InitName = "ippiMorphologyInitAlloc_"
              + func.TypeName() + "_" + func.DescrName();
   return TRUE;
}

CParamDlg* CRunCVMorph::CreateDlg() { return new CParmCVMorphDlg;}

void CRunCVMorph::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunMorph::UpdateData(parmDlg, save);
    CParmCVMorphDlg* pDlg = (CParmCVMorphDlg*)parmDlg;
    if (save) {
        m_borderType = (IppiBorderType)pDlg->m_BorderType;
    } else {
        pDlg->m_BorderType = (int)m_borderType;
    }
}

BOOL CRunCVMorph::BeforeCall()
{
   IppStatus status = CallInitAlloc(m_InitName);
   IppErrorMessage(m_InitName, status);
   if (status < 0) return FALSE;
   return TRUE;
}

BOOL CRunCVMorph::AfterCall(BOOL bOK)
{
   if (m_pState)
      ippiMorphologyFree(m_pState);
   m_pState = NULL;
   return TRUE;
}

IppStatus CRunCVMorph::CallInitAlloc(CString name)
{
   MATE_CV_CALL(name, ippiMorphologyInitAlloc_8u_C1R,( roiSize.width, m_mask, m_maskSize,
 m_anchor, &m_pState ))
   MATE_CV_CALL(name, ippiMorphologyInitAlloc_8u_C3R,( roiSize.width, m_mask, m_maskSize,
 m_anchor, &m_pState ))
   MATE_CV_CALL(name, ippiMorphologyInitAlloc_8u_C4R,( roiSize.width, m_mask, m_maskSize,
 m_anchor, &m_pState ))
   MATE_CV_CALL(name, ippiMorphologyInitAlloc_32f_C1R,( roiSize.width, m_mask, m_maskSize,
 m_anchor, &m_pState ))
   MATE_CV_CALL(name, ippiMorphologyInitAlloc_32f_C3R,( roiSize.width, m_mask, m_maskSize,
 m_anchor, &m_pState ))
   MATE_CV_CALL(name, ippiMorphologyInitAlloc_32f_C4R,( roiSize.width, m_mask, m_maskSize,
 m_anchor, &m_pState ))
   return stsNoFunction;
}

IppStatus CRunCVMorph::CallIppFunction()
{
   FUNC_CV_CALL(  ippiDilateBorderReplicate_8u_C1R,( (const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiDilateBorderReplicate_8u_C3R,( (const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiDilateBorderReplicate_8u_C4R,( (const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiDilateBorderReplicate_32f_C1R,( (const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiDilateBorderReplicate_32f_C3R,( (const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiDilateBorderReplicate_32f_C4R,( (const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))

   FUNC_CV_CALL(  ippiErodeBorderReplicate_8u_C1R,( (const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiErodeBorderReplicate_8u_C3R,( (const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiErodeBorderReplicate_8u_C4R,( (const Ipp8u*)pSrc, srcStep,
 (Ipp8u*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiErodeBorderReplicate_32f_C1R,( (const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiErodeBorderReplicate_32f_C3R,( (const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   FUNC_CV_CALL(  ippiErodeBorderReplicate_32f_C4R,( (const Ipp32f*)pSrc, srcStep,
 (Ipp32f*)pDst, dstStep, roiSize,
 m_borderType, m_pState ))
   return stsNoFunction;
}

CString CRunCVMorph::GetHistoryParms()
{
   CMyString parms;
   return parms;
}

