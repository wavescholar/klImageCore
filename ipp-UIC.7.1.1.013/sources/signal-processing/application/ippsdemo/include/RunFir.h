/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFir.h : interface for the CRunFir class.
// CRunFir class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFIR_H__07BEC1B0_B216_4F7C_A791_DF5BE9058B9A__INCLUDED_)
#define AFX_RUNFIR_H__07BEC1B0_B216_4F7C_A791_DF5BE9058B9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFirD.h"

class CRunFir : public CRunFirD  
{
public:
   CRunFir();
   virtual ~CRunFir();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);
   virtual CString GetHistoryParms();

   void* m_pState;
   CFunc m_InitName;
   CFunc m_FreeName;

   virtual void SetInitFreeNames();
   virtual IppStatus CallInit();
   virtual IppStatus CallFree();
};

#endif // !defined(AFX_RUNFIR_H__07BEC1B0_B216_4F7C_A791_DF5BE9058B9A__INCLUDED_)
