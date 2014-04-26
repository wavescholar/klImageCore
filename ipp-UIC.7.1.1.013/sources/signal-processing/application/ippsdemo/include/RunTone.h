/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTone.h : interface for the CRunTone class.
// CRunTone class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTONE_H__BCE32875_9D1E_42EA_8857_CC806583B428__INCLUDED_)
#define AFX_RUNTONE_H__BCE32875_9D1E_42EA_8857_CC806583B428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunTone : public CippsRun  
{
public:
   CRunTone();
   virtual ~CRunTone();
   enum {tn_magn, tn_rfreq, tn_phase,
      tn_NUM};
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual CString GetHistoryParms();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   IppHintAlgorithm m_hint;
   CValue m_parm[tn_NUM];
   double m_phase64f;
   float  m_phase32f;
   void* m_pState;

};

#endif // !defined(AFX_RUNTONE_H__BCE32875_9D1E_42EA_8857_CC806583B428__INCLUDED_)
