/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThreshOtsu.cpp: implementation of the CRunThreshOtsu class.
// CRunThreshOtsu class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "Timing.h"
#include "RunThreshOtsu.h"
#include "ParmThreshOtsuDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunThreshOtsu::CRunThreshOtsu()
{
    m_pThreshold = NULL;
}

CRunThreshOtsu::~CRunThreshOtsu()
{

}

BOOL CRunThreshOtsu::Open(CFunc func)
{
    if (!CippiRun::Open(func)) return FALSE;
    m_UsedVectors = VEC_SRC;
    return TRUE;
}

void CRunThreshOtsu::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg,save);
    CParmThreshOtsuDlg *pDlg = (CParmThreshOtsuDlg*)parmDlg;
    if (save) {
    } else {
        pDlg->m_Num = m_pDocSrc->GetImage()->Channels();
        for (int i=0; i < 4; i++)
            pDlg->m_Threshold[i] = m_Threshold[i];
    }
}

BOOL CRunThreshOtsu::ProcessCall()
{
    BOOL result = TRUE;

    if (ByPlanes())
    {
        double perf = 0;
        for (int coi = 0; coi < m_pDocSrc->GetImage()->Channels(); coi++) {
            SetCoiToDocs(coi);
            m_pThreshold = m_Threshold + coi;
            if (!CRun::ProcessCall()) {
                result = FALSE;
                break;
            }
            perf += m_pPerf->GetClocks();
        }
        m_pPerf->SetClocks(perf);
        SetCoiToDocs(-1);
    }
    else
    {
        m_pThreshold = m_Threshold;
        result = CRun::ProcessCall();
    }
    if (result)
    {
        CParmThreshOtsuDlg dlg;
        UpdateData(&dlg,FALSE);
        dlg.DoModal();
    }
    return result;
}

IppStatus CRunThreshOtsu::CallIppFunction()
{
    FUNC_CALL ( ippiComputeThreshold_Otsu_8u_C1R, ((Ipp8u*)pSrc, srcStep, roiSize, m_pThreshold))

    return stsNoFunction;
}
