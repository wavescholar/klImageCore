/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResizeFilter.cpp: implementation of the CRunResizeFilter class.
// CRunResizeFilter class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CRunSuper classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunResizeFilter.h"
#include "ParmResizeFilterDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunResizeFilter::CRunResizeFilter()
{
     m_pState = NULL;
     m_filter = ippResizeFilterHann;
}

CRunResizeFilter::~CRunResizeFilter()
{

}

CParamDlg* CRunResizeFilter::CreateDlg() { return new CParmResizeFilterDlg;}

void CRunResizeFilter::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunSuper::UpdateData(parmDlg,save);
    CParmResizeFilterDlg* pDlg = (CParmResizeFilterDlg*)parmDlg;
    if (save) {
        m_filter = (IppiResizeFilterType)pDlg->m_filter;
    } else {
        pDlg->m_filter = (int)m_filter;
    }
}

BOOL CRunResizeFilter::BeforeCall()
{
    Ipp32u stateSize = 0;
    CHECK_CALL(ippiResizeFilterGetSize_8u_C1R, (
        m_srcRoiSize, m_dstRoiSize, m_filter, &stateSize))
    if (stateSize)
    {
        m_pState = (IppiResizeFilterState*)ippMalloc(stateSize);
        CHECK_CALL(ippiResizeFilterInit_8u_C1R, (
            m_pState, m_srcRoiSize, m_dstRoiSize, m_filter))
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CRunResizeFilter::AfterCall(BOOL bOk)
{
    if (m_pState) {
         ippFree(m_pState);
         m_pState = NULL;
    }
    return TRUE;
}

IppStatus CRunResizeFilter::CallIppFunction()
{
    FUNC_CALL(ippiResizeFilter_8u_C1R, (
        (Ipp8u*)pSrc, srcStep, m_srcRoiSize,
        (Ipp8u*)pDst, dstStep, m_dstRoiSize, m_pState))
    return stsNoFunction;
}

static CString GetFilerParm(IppiResizeFilterType filter)
{
    switch (filter)
    {
    case ippResizeFilterHann: return "Hann";
    case ippResizeFilterLanczos: return "Lanczos";
    default: return "";
    }
}

CString CRunResizeFilter::GetHistoryParms()
{
    CString parms;
    parms.Format("%s", GetFilerParm(m_filter));
    return parms;
}
