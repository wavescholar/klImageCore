/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterCustom.h: interface for the CRunFilterCustom class.
// CRunFilterCustom class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFILTERCUSTOM_H__26DF4EEB_08D8_4051_B989_11E4F4B8525F__INCLUDED_)
#define AFX_RUNFILTERCUSTOM_H__26DF4EEB_08D8_4051_B989_11E4F4B8525F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFilterBox.h"

class CRunFilterCustom : public CRunFilterBox
{
public:
    CRunFilterCustom();
    virtual ~CRunFilterCustom();
protected:

    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual BOOL Open(CFunc func);
    virtual void Close();
    virtual BOOL BeforeCall();
    virtual BOOL AfterCall(BOOL bOK);
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();
    virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

    IppStatus CallGetBufSize(IppiSize  kernelSize, int roiWidth, int* pSize);

    enum {k16s, k32s, k32f, kNum};
    ppType kernelType(int idx) {
        ppType t[kNum] = {pp16s, pp32s, pp32f};
        return t[idx];
    }

    IppiSize  m_maskSize;
    IppiPoint m_anchor;
    CVector m_pKernel[kNum];
    int m_k;
    int m_divisor;
    IppRoundMode m_roundMode;
    Ipp8u* m_pBuffer;

    int m_DivType;
    BOOL m_bDiv;
    BOOL m_bRound;
    CString m_nameGetBufSize;
};

#endif // !defined(AFX_RUNFILTERCUSTOM_H__26DF4EEB_08D8_4051_B989_11E4F4B8525F__INCLUDED_)
