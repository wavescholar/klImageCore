/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDeconvLR.h: interface for the CRunDeconvLR class.
// CRunDeconvLR class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDeconvLR_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
#define AFX_RUNDeconvLR_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunDeconvFFT.h"

class CRunDeconvLR : public CRunDeconvFFT
{
public:
    CRunDeconvLR();
    virtual ~CRunDeconvLR();

protected:
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual BOOL Open(CFunc func);
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();

    int     numIter;

    virtual IppStatus CallInit();
    virtual IppStatus CallFree();
};

#endif // !defined(AFX_RUNDeconvLR_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
