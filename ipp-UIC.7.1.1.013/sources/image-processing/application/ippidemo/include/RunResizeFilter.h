/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunResizeFilter.h: interface for the CRunResizeFilter class.
// CRunResizeFilter class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CRunSuper classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRESIZE_FLT_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_RUNRESIZE_FLT_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RunSuper.h"

class CRunResizeFilter : public CRunSuper
{
public:
   CRunResizeFilter();
   virtual ~CRunResizeFilter();

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppiResizeFilterType m_filter;
   IppiResizeFilterState* m_pState;

};

#endif // !defined(AFX_RUNRESIZE_FLT_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
