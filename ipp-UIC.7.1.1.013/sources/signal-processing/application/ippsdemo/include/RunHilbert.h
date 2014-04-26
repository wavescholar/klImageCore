/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHilbert.h : interface for the CRunHilbert class.
// CRunHilbert class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNHILBERT_H__46403B5B_EB2C_473D_B189_E6A03EE2009F__INCLUDED_)
#define AFX_RUNHILBERT_H__46403B5B_EB2C_473D_B189_E6A03EE2009F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunHilbert : public CippsRun  
{
public:
   CRunHilbert();
   virtual ~CRunHilbert();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   IppHintAlgorithm m_hint;
   void* m_pSpec;

   CFunc m_InitName;
   CFunc m_FreeName;

   IppStatus CallInit();
   IppStatus CallFree();
};

#endif // !defined(AFX_RUNHILBERT_H__46403B5B_EB2C_473D_B189_E6A03EE2009F__INCLUDED_)
