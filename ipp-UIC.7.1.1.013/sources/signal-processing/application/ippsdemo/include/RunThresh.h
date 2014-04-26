/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThresh.h : interface for the CRunThresh class.
// CRunThresh class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTHRESH_H__70713C2D_4C7F_465E_8FE0_E4CEF6893E0E__INCLUDED_)
#define AFX_RUNTHRESH_H__70713C2D_4C7F_465E_8FE0_E4CEF6893E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
#include "Value.h"

class CRunThresh : public CippsRun  
{
public:
   CRunThresh();
   virtual ~CRunThresh() {}
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue level;
   CValue value;
   IppCmpOp relOp;

   BOOL m_bValue;
};

#endif // !defined(AFX_RUNTHRESH_H__70713C2D_4C7F_465E_8FE0_E4CEF6893E0E__INCLUDED_)
