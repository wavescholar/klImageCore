/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRamp.cpp: implementation of the CRunRamp class.
// CRunRamp class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "RunRamp.h"
#include "ParmRampDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRamp::CRunRamp()
{
   m_axis = ippAxsHorizontal;
   m_offset.Init(pp32f, 0);
   m_slope.Init(pp32f, 1);
}

CRunRamp::~CRunRamp()
{

}

CParamDlg* CRunRamp::CreateDlg() { return new CParmRampDlg;}

void CRunRamp::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmRampDlg* pDlg = (CParmRampDlg*)parmDlg;
    if (save) {
        m_axis = (IppiAxis)pDlg->m_axis;
        m_offset.Set(pDlg->m_offsetStr);
        m_slope.Set(pDlg->m_slopeStr);
    } else {
        pDlg->m_axis = (int)m_axis;
        m_offset.Get(pDlg->m_offsetStr);
        m_slope.Get(pDlg->m_slopeStr);
    }
}

IppStatus CRunRamp::CallIppFunction()
{
   FUNC_CALL(ippiImageRamp_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8u_C3R, ((Ipp8u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8u_C4R, ((Ipp8u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8u_AC4R,((Ipp8u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8s_C1R, ((Ipp8s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8s_C3R, ((Ipp8s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8s_C4R, ((Ipp8s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_8s_AC4R,((Ipp8s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16u_C1R, ((Ipp16u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16u_C3R, ((Ipp16u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16u_C4R, ((Ipp16u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16u_AC4R,((Ipp16u*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16s_C1R, ((Ipp16s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16s_C3R, ((Ipp16s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16s_C4R, ((Ipp16s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_16s_AC4R,((Ipp16s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32s_C1R, ((Ipp32s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32s_C3R, ((Ipp32s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32s_C4R, ((Ipp32s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32s_AC4R,((Ipp32s*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32f_C1R, ((Ipp32f*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32f_C3R, ((Ipp32f*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32f_C4R, ((Ipp32f*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))
   FUNC_CALL(ippiImageRamp_32f_AC4R,((Ipp32f*)pSrc, srcStep, roiSize, (float)m_offset, (float)m_slope, m_axis))

   return stsNoFunction;
}

CString CRunRamp::GetHistoryParms()
{
   CMyString parms;
   parms << m_offset.String() << ", "
         << m_slope.String() << ", "
         << m_axis;
   return parms;
}
