/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStat.h : interface for the CRunStat class.
// CRunStat class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSTAT_H__A026630A_16BB_4388_AB51_86F99F83A11E__INCLUDED_)
#define AFX_RUNSTAT_H__A026630A_16BB_4388_AB51_86F99F83A11E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
#include "Value.h"

class CParmStatDlg;

class CRunStat : public CippsRun  
{
public:
   CRunStat();
   virtual ~CRunStat();
   void SetValues(CParmStatDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   CValue m_value;
   IppHintAlgorithm m_hint;
};

#endif // !defined(AFX_RUNSTAT_H__A026630A_16BB_4388_AB51_86F99F83A11E__INCLUDED_)
