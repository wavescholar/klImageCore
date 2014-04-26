/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDeconvFFT.h: interface for the CRunDeconvFFT class.
// CRunDeconvFFT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDeconvFFT_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
#define AFX_RUNDeconvFFT_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunDeconvFFT : public CippiRun
{
public:
    CRunDeconvFFT();
    virtual ~CRunDeconvFFT();

protected:
    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual BOOL Open(CFunc func);
    virtual void Close();
    virtual CMyString GetVectorName(int vecPos);
    virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
    virtual ImgHeader GetNewDstHeader();
    virtual BOOL PrepareSrc();
    virtual void PrepareParameters();
    virtual BOOL BeforeCall();
    virtual BOOL AfterCall(BOOL bOK);
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();

    void*   pState;
    Ipp32f threshold;
    int order;
    int kernelSize;
    Ipp32f* pKernel;

    CString m_initName;
    CString m_freeName;

    virtual IppStatus CallInit();
    virtual IppStatus CallFree();

    static int CRunDeconvFFT::GetKernelSize(CImage* pImg);
    static Ipp32f* CRunDeconvFFT::createKernel(CImage* pImg);
    static void CRunDeconvFFT::deleteKernel(Ipp32f* pKernel, CImage* pImg);

};

#endif // !defined(AFX_RUNDeconvFFT_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
