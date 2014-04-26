/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDotprod.h : interface for the CRunDotprod class.
// CRunDotprod class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDOTPROD_H__C2868565_53E2_4106_899C_0A30D9163E2F__INCLUDED_)
#define AFX_RUNDOTPROD_H__C2868565_53E2_4106_899C_0A30D9163E2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CParmDotprodDlg;

class CRunDotprod : public CippsRun  
{
public:
   CRunDotprod();
   virtual ~CRunDotprod();
   void SetValues(CParmDotprodDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   CValue m_value;
};

#endif // !defined(AFX_RUNDOTPROD_H__C2868565_53E2_4106_899C_0A30D9163E2F__INCLUDED_)
