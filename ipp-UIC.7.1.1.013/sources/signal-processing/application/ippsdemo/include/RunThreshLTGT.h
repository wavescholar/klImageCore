/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThreshLTGT.h : interface for the CRunThreshLTGT class.
// CRunThreshLTGT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTHRESHLTGT_H__66788A1B_9DF7_46E4_80C4_822630302664__INCLUDED_)
#define AFX_RUNTHRESHLTGT_H__66788A1B_9DF7_46E4_80C4_822630302664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunThreshLTGT : public CippsRun  
{
public:
   CRunThreshLTGT();
   virtual ~CRunThreshLTGT();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue level[2];
   CValue value[2];
};

#endif // !defined(AFX_RUNTHRESHLTGT_H__66788A1B_9DF7_46E4_80C4_822630302664__INCLUDED_)
