/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunShear.h: interface for the CRunShear class.
// CRunShear class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSHEAR_H__44AAAF63_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_RUNSHEAR_H__44AAAF63_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"
#include "ParmShearDlg.h"

class CRunShear : public CippiRun
{
public:
   CRunShear();
   virtual ~CRunShear();
    virtual CParamDlg* CreateDlg() { return new CParmShearDlg;}
    virtual void DeleteDlg(CParamDlg* pDlg) { delete pDlg;}
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
    virtual void CopyDataToNewDst() {}
    virtual void CopyContoursToNewDst();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
protected:
    int m_Type;
    Ipp64f m_shearX ;
    Ipp64f m_shearY ;
    Ipp64f m_xShift;
    Ipp64f m_yShift;
    int m_interpolate;
};

#endif // !defined(AFX_RUNSHEAR_H__44AAAF63_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
