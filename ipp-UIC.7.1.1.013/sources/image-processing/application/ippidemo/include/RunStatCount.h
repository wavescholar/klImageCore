/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStatCount.h: interface for the CRunStatCount class.
// CRunStatCount class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSTATCOUNT_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_)
#define AFX_RUNSTATCOUNT_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
class CParmStatCountDlg;

class CRunStatCount : public CippiRun
{
public:
   CRunStatCount();
   virtual ~CRunStatCount();
   void SetValues(CParmStatCountDlg* pDlg);
   void UpdateBounds(CParmStatCountDlg* pDlg, CFunc func);
   void UpdateBounds(CParmStatCountDlg* pDlg, BOOL bSave = TRUE);
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}
   virtual CString GetHistoryParms();

   CVectorUnit m_value;

   CVectorUnit m_lowerBound;
   CVectorUnit m_upperBound;
};

#endif // !defined(AFX_RUNSTATCOUNT_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_)
