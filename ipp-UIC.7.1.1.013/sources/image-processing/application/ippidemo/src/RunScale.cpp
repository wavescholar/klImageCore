/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunScale.cpp: implementation of the CRunScale class.
// CRunScale class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunScale.h"
#include "ParmScaleDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunScale::CRunScale()
{
   m_vMin = 0;
   m_vMax = 1;
}

CRunScale::~CRunScale()
{

}

CParamDlg* CRunScale::CreateDlg() { return new CParmScaleDlg;}

void CRunScale::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmScaleDlg* pDlg = (CParmScaleDlg*)parmDlg;
    if (save) {
        m_vMin = (float)atof(pDlg->m_MinStr);
        m_vMax = (float)atof(pDlg->m_MaxStr);
    } else {
        pDlg->m_MinStr.Format("%.6g", m_vMin);
        pDlg->m_MaxStr.Format("%.6g", m_vMax);
    }
}

IppStatus CRunScale::CallIppFunction()
{
   FUNC_CALL(ippiScale_8u32f_C1R,
        ((Ipp8u*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_8u32f_C3R,
        ((Ipp8u*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_8u32f_AC4R,
        ((Ipp8u*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_8u32f_C4R,
        ((Ipp8u*)pSrc, srcStep, (Ipp32f*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_32f8u_C1R,
        ((Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_32f8u_C3R,
        ((Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_32f8u_AC4R,
        ((Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   FUNC_CALL(ippiScale_32f8u_C4R,
        ((Ipp32f*)pSrc, srcStep, (Ipp8u*)pDst, dstStep,
         roiSize, m_vMin, m_vMax ))
   return stsNoFunction;
}

CString CRunScale::GetHistoryParms()
{
   CMyString parm;
   return parm << m_vMin << ", " << m_vMax;
}
