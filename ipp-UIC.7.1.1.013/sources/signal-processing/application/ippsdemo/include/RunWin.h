/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWin.h : interface for the CRunWin class.
// CRunWin class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNWIN_H__017699E0_2F01_4DC2_8CD2_8D0ADEE89FDE__INCLUDED_)
#define AFX_RUNWIN_H__017699E0_2F01_4DC2_8CD2_8D0ADEE89FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
#include "Value.h"

class CRunWin : public CippsRun  
{
public:
   CRunWin();
   virtual ~CRunWin();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue alpha;
};

#endif // !defined(AFX_RUNWIN_H__017699E0_2F01_4DC2_8CD2_8D0ADEE89FDE__INCLUDED_)
