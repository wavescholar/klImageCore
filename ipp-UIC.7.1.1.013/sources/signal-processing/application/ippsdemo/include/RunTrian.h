/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTrian.h : interface for the CRunTrian class.
// CRunTrian class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTRIAN_H__5631E07A_F1EA_4858_81A9_C4EECE284B19__INCLUDED_)
#define AFX_RUNTRIAN_H__5631E07A_F1EA_4858_81A9_C4EECE284B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunTrian : public CippsRun  
{
public:
   CRunTrian();
   virtual ~CRunTrian();
   enum {tr_magn, tr_rfreq, tr_asym, tr_phase,
      tr_NUM};
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc func);
   virtual CString GetHistoryParms();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   CValue m_parm[tr_NUM];
   double m_phase64;
   float  m_phase32;
   void* m_pState;
};

#endif // !defined(AFX_RUNTRIAN_H__5631E07A_F1EA_4858_81A9_C4EECE284B19__INCLUDED_)
