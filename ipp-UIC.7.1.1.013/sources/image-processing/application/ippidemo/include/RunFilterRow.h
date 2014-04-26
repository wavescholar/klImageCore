/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterRow.h: interface for the CRunFilterRow class.
// CRunFilterRow class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFILTERROW_H__E1DDD3DC_FC72_462F_8B6A_240EA0DB5151__INCLUDED_)
#define AFX_RUNFILTERROW_H__E1DDD3DC_FC72_462F_8B6A_240EA0DB5151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFilterBox.h"

class CRunFilterRow : public CRunFilterBox
{
public:
    CRunFilterRow();
    virtual ~CRunFilterRow();
protected:
    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual BOOL Open(CFunc func);
    virtual void Close();
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();
    virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

    enum {k32s, k32f, kNum};
    ppType kernelType(int idx) {
        ppType t[kNum] = {pp32s, pp32f};
        return t[idx];
    }

    CVector   m_pKernel[kNum];
    int       m_k;
    int       m_kernelLen;
    int       m_anchor;
    int       m_divisor;

    int m_DivType;
    BOOL m_bDiv;
};

#endif // !defined(AFX_RUNFILTERROW_H__E1DDD3DC_FC72_462F_8B6A_240EA0DB5151__INCLUDED_)
