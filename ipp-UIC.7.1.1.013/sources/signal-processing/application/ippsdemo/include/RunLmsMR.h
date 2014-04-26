/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunLmsMR.h : interface for the CRunLmsMR class.
// CRunLmsMR class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNLMSMR_H__3ACFFB0A_5DCF_48ED_9511_550EEDA27DC2__INCLUDED_)
#define AFX_RUNLMSMR_H__3ACFFB0A_5DCF_48ED_9511_550EEDA27DC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunLms.h"

class CRunLmsMR : public CRunLms  
{
public:
   CRunLmsMR();
   virtual ~CRunLmsMR();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual int GetDstLength();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos = 0);
   virtual void SetFilterHistory();
   virtual CString GetBaseName() { return "FIRLMSMR";}

   IppsFIRLMSMRState32s_16s* pState_16s;
   IppsFIRLMSMRState32sc_16sc* pState_16sc;
   int m_dlyStep;
   int m_updateDly;


   CFunc m_UpdateTapsName;
   CFunc m_PutValName;
   int m_iSrc;
   int m_iDst;

   IppStatus CallInit();
   IppStatus CallFree();
   IppStatus CallUpdateTaps();
   IppStatus CallGetTaps();
   IppStatus CallPutVal();
};

#endif // !defined(AFX_RUNLMSMR_H__3ACFFB0A_5DCF_48ED_9511_550EEDA27DC2__INCLUDED_)
