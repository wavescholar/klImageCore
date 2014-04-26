/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterBilat.h: interface for the CRunFilterBilat class.
// CRunFilterBilat class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFilterBilat_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
#define AFX_RUNFilterBilat_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunFilterBilat : public CippiRun
{
public:
    CRunFilterBilat();
    virtual ~CRunFilterBilat();

protected:
    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual int GetSrcBorder(int idx);
    virtual BOOL BeforeCall();
    virtual BOOL AfterCall(BOOL bOK);
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();

    IppiFilterBilateralSpec* pSpec;
    IppiFilterBilateralType filter;
    IppiSize kernelSize;
    int stepInKernel;
    Ipp32f valSquareSigma;
    Ipp32f posSquareSigma;
};

#endif // !defined(AFX_RUNFilterBilat_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
