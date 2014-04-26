/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWt.h : interface for the CRunWt class.
// CRunWt class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNWT_H__C44437A0_A1D1_461E_B8A9_EC41728C733A__INCLUDED_)
#define AFX_RUNWT_H__C44437A0_A1D1_461E_B8A9_EC41728C733A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunWtHaar.h"

enum {wtCUSTOM, wtGAUSS, wtLAPLAS, wtLOAD};

class CRunWt : public CRunWtHaar 
{
public:
   CRunWt();
   virtual ~CRunWt();

   enum {VEC_TAPS_LOW = VEC_DST2 << 1, VEC_TAPS_HIGH = VEC_TAPS_LOW << 1 };
   virtual CString VectorTerm();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);
   virtual CString GetHistoryParms();

   virtual BOOL PickNextPos();
   virtual BOOL PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID);
   virtual BOOL PickMarkDoc(CDemoDoc* pDoc);
   virtual void GrabDoc(CDemoDoc* pDoc);

   void* m_pState;
   CDemoDoc* m_pDocTapsLow;
   CDemoDoc* m_pDocTapsHigh;
   int       m_offsLow;
   int       m_offsHigh;
   void* m_pTapsLow;
   void* m_pTapsHigh;
   int   m_tapsLenLow ;
   int   m_tapsLenHigh;


   CFunc m_InitFunc;
   CFunc m_FreeFunc;

   IppStatus CallInit();
   IppStatus CallFree();
};

#endif // !defined(AFX_RUNWT_H__C44437A0_A1D1_461E_B8A9_EC41728C733A__INCLUDED_)
