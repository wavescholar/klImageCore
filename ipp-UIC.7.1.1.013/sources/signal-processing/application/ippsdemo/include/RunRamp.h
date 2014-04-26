/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRamp.h : interface for the CRunRamp class.
// CRunRamp class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRAMP_H__84450DC7_5D50_4936_96C7_666BC794DBDE__INCLUDED_)
#define AFX_RUNRAMP_H__84450DC7_5D50_4936_96C7_666BC794DBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunRamp : public CippsRun  
{
public:
   CRunRamp();
   virtual ~CRunRamp() {}
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue m_offset;
   CValue m_slope;
};

#endif // !defined(AFX_RUNRAMP_H__84450DC7_5D50_4936_96C7_666BC794DBDE__INCLUDED_)
