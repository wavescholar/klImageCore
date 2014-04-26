/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFirD.h : interface for the CRunFirD class.
// CRunFirD class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFIRD_H__C40052E7_58DE_4F36_A8C4_65C093C90862__INCLUDED_)
#define AFX_RUNFIRD_H__C40052E7_58DE_4F36_A8C4_65C093C90862__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunFirD : public CippsRun  
{
public:
   CRunFirD();
   virtual ~CRunFirD();

   enum {DLY_ZERO, DLY_SRC};
   static int GetSrcOffset(int dlyType, int dlyLen, int srcLen);
   static int CreateDelayLine(CVector& pDelay, int dlyLen, CVector* pSrcVector, 
                              int dlyType, CFunc& func);
   static void* GetOffsetPtr(CVector* pSrcVector, int srcOffset);

   enum {VEC_TAPS = VEC_DST2 << 1};
   virtual CString VectorTerm();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual void  Close();

   virtual BOOL PickNextPos();
   virtual BOOL PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID);
   virtual BOOL PickMarkDoc(CDemoDoc* pDoc);
   virtual void GrabDoc(CDemoDoc* pDoc);

   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void PrepareParameters();
   virtual int GetDstLength();
   virtual CString GetHistoryParms();

   CDemoDoc* m_pDocTaps;
   void*     m_pTaps;
   int       m_tapsLen;

   CVector m_pDelay;

   int m_tapsFactor  ;
   int m_delayIndex;

   BOOL m_MR;
   int m_upFactor  ;
   int m_upPhase   ;
   int m_downFactor;
   int m_downPhase ;

   int GetMRLen();

   int m_scaleFactorSave;
   int m_tapsFactorSave;
   int m_docFactor;
   BOOL m_bScaleFactor;
   BOOL m_bTapsFactor;
};

#endif // !defined(AFX_RUNFIRD_H__C40052E7_58DE_4F36_A8C4_65C093C90862__INCLUDED_)
