/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDeconvLR.cpp: implementation of the CRunDeconvLR class.
// CRunDeconvLR class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CRunDeconvFFT classes for more information.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunDeconvLR.h"
#include "ParmDeconvFFTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDeconvLR::CRunDeconvLR()
{
    numIter = 1;
}

CRunDeconvLR::~CRunDeconvLR()
{

}

BOOL CRunDeconvLR::Open(CFunc func)
{
    if (!CRunDeconvFFT::Open(func)) return FALSE;

    m_initName = "ippiDeconvLRInitAlloc_" + m_Func.TypeName() + "_" + m_Func.DescrName();
    m_freeName = "ippiDeconvLRFree_" + m_Func.TypeName() + "_" + m_Func.DescrName();
    return TRUE;
}

void CRunDeconvLR::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CRunDeconvFFT::UpdateData(parmDlg, save);
    CParmDeconvFFTDlg* pDlg = (CParmDeconvFFTDlg*)parmDlg;
    if (save) {
        numIter = pDlg->order;
    } else {
        pDlg->order = numIter;
    }
}

IppStatus CRunDeconvLR::CallInit()
{
    IppiSize maxSize = {roiSize.width + kernelSize, roiSize.height + kernelSize};
    MATE_CALL(m_initName, ippiDeconvLRInitAlloc_32f_C1R, (
        (IppiDeconvLR_32f_C1R**)&pState, (Ipp32f*)pKernel, kernelSize, maxSize, threshold))
    MATE_CALL(m_initName, ippiDeconvLRInitAlloc_32f_C3R, (
        (IppiDeconvLR_32f_C3R**)&pState, (Ipp32f*)pKernel, kernelSize, maxSize, threshold))
    return stsNoFunction;
}

IppStatus CRunDeconvLR::CallFree()
{
    MATE_CALL(m_freeName, ippiDeconvLRFree_32f_C1R, ((IppiDeconvLR_32f_C1R*)pState))
    MATE_CALL(m_freeName, ippiDeconvLRFree_32f_C3R, ((IppiDeconvLR_32f_C3R*)pState))
    return stsNoFunction;
}

IppStatus CRunDeconvLR::CallIppFunction()
{
    FUNC_CALL ( ippiDeconvLR_32f_C1R, ((Ipp32f*)pSrc, srcStep, 
        (Ipp32f*)pDst,  dstStep, roiSize, numIter, (IppiDeconvLR_32f_C1R*)pState))
    FUNC_CALL ( ippiDeconvLR_32f_C3R, ((Ipp32f*)pSrc, srcStep, 
        (Ipp32f*)pDst, dstStep, roiSize, numIter, (IppiDeconvLR_32f_C3R*)pState))

    return stsNoFunction;
}

CString CRunDeconvLR::GetHistoryParms()
{
    CMyString parms;
    return parms 
        << kernelSize 
        << ", " << threshold
        << ", " << numIter
        ;
}
