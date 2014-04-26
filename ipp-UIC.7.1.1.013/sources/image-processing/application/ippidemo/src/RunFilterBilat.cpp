/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterBilat.cpp: implementation of the CRunFilterBilat class.
// CRunFilterBilat class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunFilterBilat.h"
#include "ParmFilterBilatDlg.h"

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

CRunFilterBilat::CRunFilterBilat()
{
    pSpec = NULL;
    filter = ippiFilterBilateralGauss;
    kernelSize.width = 5;
    kernelSize.height = 5;
    stepInKernel = 2;
    valSquareSigma = 4;
    posSquareSigma = 4;
}

CRunFilterBilat::~CRunFilterBilat()
{

}

int CRunFilterBilat::GetSrcBorder(int idx)
{
    switch (idx)
    {
    case LEFT: 
    case RIGHT:
        return (kernelSize.width) >> 1;
    case TOP: 
    case BOTTOM:
        return (kernelSize.height) >> 1;
    }
    return 0;
}

CParamDlg* CRunFilterBilat::CreateDlg() { return new CParmFilterBilatDlg;}

void CRunFilterBilat::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmFilterBilatDlg* pDlg = (CParmFilterBilatDlg*)parmDlg;
    if (save) {
        filter         = (IppiFilterBilateralType)(pDlg->filter + ippiFilterBilateralGauss);
        kernelSize     = pDlg->kernelSize    ;
        valSquareSigma = pDlg->valSquareSigma;
        posSquareSigma = pDlg->posSquareSigma;
        stepInKernel   = pDlg->stepInKernel  ;
    } else {
        pDlg->filter         = (int)(filter - ippiFilterBilateralGauss);
        pDlg->kernelSize     = kernelSize    ;
        pDlg->valSquareSigma = valSquareSigma;
        pDlg->posSquareSigma = posSquareSigma;
        pDlg->stepInKernel   = stepInKernel  ;
    }
}

BOOL CRunFilterBilat::BeforeCall()
{
    int bufferSize;
    CHECK_CALL(ippiFilterBilateralGetBufSize_8u_C1R,(filter, roiSize, kernelSize, &bufferSize));
    pSpec = (IppiFilterBilateralSpec*)ippMalloc(bufferSize);
    CHECK_CALL(ippiFilterBilateralInit_8u_C1R,
        (filter, kernelSize, valSquareSigma, posSquareSigma, stepInKernel, pSpec));
    return TRUE;
}

BOOL CRunFilterBilat::AfterCall(BOOL bOK)
{
    if (pSpec)
        ippFree(pSpec); pSpec = NULL;
    return bOK;
}

IppStatus CRunFilterBilat::CallIppFunction()
{
    FUNC_CALL(ippiFilterBilateral_8u_C1R,((Ipp8u*)pSrc, srcStep, (Ipp8u*)pDst, dstStep, roiSize, kernelSize, pSpec))

    return stsNoFunction;
}

CString CRunFilterBilat::GetHistoryParms()
{
    CMyString parms;
    return parms 
                << filter        
        << ", " << kernelSize    
        << ", " << valSquareSigma
        << ", " << posSquareSigma
        << ", " << stepInKernel  
        ;
}
