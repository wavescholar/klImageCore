/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSumLn.h : interface for the CRunSumLn class.
// CRunSumLn class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSUMLN_H__193603CC_78B2_498B_B9EF_6710EEDCBF8B__INCLUDED_)
#define AFX_RUNSUMLN_H__193603CC_78B2_498B_B9EF_6710EEDCBF8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CParmSumLnDlg;

class CRunSumLn : public CippsRun  
{
public:
   CRunSumLn();
   virtual ~CRunSumLn();
   void SetValues(CParmSumLnDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   CValue m_value;
};

#endif // !defined(AFX_RUNSUMLN_H__193603CC_78B2_498B_B9EF_6710EEDCBF8B__INCLUDED_)
