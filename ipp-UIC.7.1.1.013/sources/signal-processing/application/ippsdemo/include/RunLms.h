/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunLms.h : interface for the CRunLms class.
// CRunLms class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNLMS_H__846326B3_446A_40E4_9E8B_B9BF26363D9B__INCLUDED_)
#define AFX_RUNLMS_H__846326B3_446A_40E4_9E8B_B9BF26363D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunLms : public CippsRun  
{
public:
   CRunLms();
   virtual ~CRunLms();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual int GetDstLength();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void ActivateDst();
   virtual CString GetHistoryParms();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);
   virtual CString GetBaseName() { return "FIRLMS";}

   IppsFIRLMSState_32f* pState_32f;
   IppsFIRLMSState32f_16s* pState_16s;
   CDemoDoc* m_pDocTaps;
   void* m_pTaps;
   CVector m_pDelay;
   CValue m_mu;
   int   m_dlyIndex;
   int   m_tapLen;

   BOOL m_bDirect;
   CFunc m_InitName;
   CFunc m_FreeName;
   CFunc m_GetTapsName;
   ppType  m_TapsType;
   CString m_FilterTitle;
   int m_dlyIndexSave;
   int m_DlyTypeDlg;

   IppStatus CallInit();
   IppStatus CallFree();
   IppStatus CallGetTaps();

   CFunc GetSpecName(CString specName);
   void SetFilterHistory();
   int GetSrcOffset();
};

#endif // !defined(AFX_RUNLMS_H__846326B3_446A_40E4_9E8B_B9BF26363D9B__INCLUDED_)
