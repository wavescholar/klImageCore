/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCmp.h: interface for the CRunCmp class.
// CRunCmp class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCMP_H__C5D77650_36DD_40A0_A22B_C899969438E1__INCLUDED_)
#define AFX_RUNCMP_H__C5D77650_36DD_40A0_A22B_C899969438E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCmp : public CippiRun
{
public:
   CRunCmp();
   virtual ~CRunCmp();
protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual CString GetHistoryParms();

   CVectorUnit m_value ;
   IppCmpOp    m_cmpOp;
   Ipp32f      m_eps;

   int m_numCoi;
};

#endif // !defined(AFX_RUNCMP_H__C5D77650_36DD_40A0_A22B_C899969438E1__INCLUDED_)
