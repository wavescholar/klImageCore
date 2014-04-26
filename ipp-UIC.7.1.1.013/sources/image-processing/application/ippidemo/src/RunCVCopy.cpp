/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVCopy.cpp: implementation of the CRunCVCopy class.
// CRunCVCopy class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippidemo.h"
#include "RunCVCopy.h"
#include "ParmCVCopyDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCVCopy::CRunCVCopy()
{
   m_dx = 0.25;
   m_dy = 0.75;
}

CRunCVCopy::~CRunCVCopy()
{

}

CParamDlg* CRunCVCopy::CreateDlg() { return new CParmCVCopyDlg;}

void CRunCVCopy::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmCVCopyDlg* pDlg = (CParmCVCopyDlg*)parmDlg;
    if (save) {
        m_dx = (Ipp32f)atof(pDlg->m_dxStr);
        m_dy = (Ipp32f)atof(pDlg->m_dyStr);
    } else {
        pDlg->m_dxStr.Format("%.2f", m_dx);
        pDlg->m_dyStr.Format("%.2f", m_dy);
    }
}

IppStatus CRunCVCopy::CallIppFunction()
{

   FUNC_CV_CALL( ippiCopySubpix_8u_C1R, ((const Ipp8u*)pSrc, srcStep,
      (Ipp8u*)pDst, dstStep, roiSize, m_dx, m_dy))
   FUNC_CV_CALL( ippiCopySubpix_8u16u_C1R_Sfs, ((const Ipp8u*)pSrc, srcStep,
      (Ipp16u*)pDst, dstStep, roiSize, m_dx, m_dy, scaleFactor))
   FUNC_CV_CALL( ippiCopySubpix_8u32f_C1R, ((const Ipp8u*)pSrc, srcStep,
      (Ipp32f*)pDst, dstStep, roiSize, m_dx, m_dy))
   FUNC_CV_CALL( ippiCopySubpix_16u_C1R, ((const Ipp16u*)pSrc, srcStep,
      (Ipp16u*)pDst, dstStep, roiSize, m_dx, m_dy))
   FUNC_CV_CALL( ippiCopySubpix_16u32f_C1R, ((const Ipp16u*)pSrc, srcStep,
      (Ipp32f*)pDst, dstStep, roiSize, m_dx, m_dy))
   FUNC_CV_CALL( ippiCopySubpix_32f_C1R, ((const Ipp32f*)pSrc, srcStep,
      (Ipp32f*)pDst, dstStep, roiSize, m_dx, m_dy))
   return stsNoFunction;
}

CString CRunCVCopy::GetHistoryParms()
{
   CMyString parms;
   parms << m_dx << ", " << m_dy;
   return parms;
}

