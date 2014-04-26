/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunColor.cpp: implementation of the CRunColor class.
// CRunColor class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunColor.h"
#include "ParmColorDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunColor::CRunColor()
{
   for (int i=0; i<3; i++)
      m_coeffs[i] = 0.33f;
}

CRunColor::~CRunColor()
{

}

CParamDlg* CRunColor::CreateDlg() { return new CParmColorDlg;}

void CRunColor::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmColorDlg* pDlg = (CParmColorDlg*)parmDlg;
    if (save) {
       for (int i=0; i<3; i++)
          m_coeffs[i] = pDlg->m_coeffs[i];
    } else {
       for (int i=0; i<3; i++)
          pDlg->m_coeffs[i] = m_coeffs[i];
    }
}

IppStatus CRunColor::CallIppFunction()
{
   FUNC_CC_CALL(ippiColorToGray_8u_C3C1R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,m_coeffs))
   FUNC_CC_CALL(ippiColorToGray_16u_C3C1R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,m_coeffs))
   FUNC_CC_CALL(ippiColorToGray_16s_C3C1R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,m_coeffs))
   FUNC_CC_CALL(ippiColorToGray_32f_C3C1R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep,roiSize,m_coeffs))

   FUNC_CC_CALL(ippiColorToGray_8u_AC4C1R,((Ipp8u*)pSrc,srcStep,(Ipp8u*)pDst,dstStep,roiSize,m_coeffs))
   FUNC_CC_CALL(ippiColorToGray_16u_AC4C1R,((Ipp16u*)pSrc,srcStep,(Ipp16u*)pDst,dstStep,roiSize,m_coeffs))
   FUNC_CC_CALL(ippiColorToGray_16s_AC4C1R,((Ipp16s*)pSrc,srcStep,(Ipp16s*)pDst,dstStep,roiSize,m_coeffs))
   FUNC_CC_CALL(ippiColorToGray_32f_AC4C1R,((Ipp32f*)pSrc,srcStep,(Ipp32f*)pDst,dstStep,roiSize,m_coeffs))
   return stsNoFunction;
}

CString CRunColor::GetHistoryParms()
{
   CMyString parms;
   return parms << "{" << m_coeffs[0] << ", "
                       << m_coeffs[1] << ", "
                       << m_coeffs[2] << "}";
}
