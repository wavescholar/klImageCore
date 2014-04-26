/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilter.h : interface for the CRunFilter class.
// CRunFilter class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFILTER_H__66F3E8C9_382B_4EC8_A6ED_8763A696E4BF__INCLUDED_)
#define AFX_RUNFILTER_H__66F3E8C9_382B_4EC8_A6ED_8763A696E4BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunFilter : public CippsRun  
{
public:
   CRunFilter() : maskSize(3) {}
   virtual ~CRunFilter() {}
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int maskSize;
};

#endif // !defined(AFX_RUNFILTER_H__66F3E8C9_382B_4EC8_A6ED_8763A696E4BF__INCLUDED_)
