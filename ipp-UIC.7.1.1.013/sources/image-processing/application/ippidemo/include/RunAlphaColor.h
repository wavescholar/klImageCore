/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunAlphaColor.h: interface for the CRunAlphaColor class.
// CRunAlphaColor class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNALPHACOLOR_H__4E085303_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
#define AFX_RUNALPHACOLOR_H__4E085303_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RunCompColor.h"

class CRunAlphaColor : public CRunCompColor
{
public:
    CRunAlphaColor();
    virtual ~CRunAlphaColor();
protected:

    virtual CParamDlg* CreateDlg();
    virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual BOOL Open(CFunc func);
    virtual IppStatus CallIppFunction();
    virtual CString GetHistoryParms();

    IppiAlphaType m_alphaType;
    CValue m_alpha1;
    CValue m_alpha2;
};

#endif // !defined(AFX_RUNALPHACOLOR_H__4E085303_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
