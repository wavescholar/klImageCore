/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRand.h : interface for the CRunRand class.
// CRunRand class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRAND_H__7ED67F85_AF96_4D1C_B71A_139CCD51C6E3__INCLUDED_)
#define AFX_RUNRAND_H__7ED67F85_AF96_4D1C_B71A_139CCD51C6E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunRand : public CippsRun  
{
public:
   CRunRand();
   virtual ~CRunRand(){}

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual CString GetHistoryParms();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   CValue m_low;
   CValue m_high;
   CValue m_mean;
   CValue m_stdDev;
   Ipp32u m_seed;
   void* m_pState;

   Ipp32u m_seedSave;
   BOOL m_bGauss ;
   BOOL m_bDirect;
   CFunc m_InitName;
   CFunc m_FreeName;
   
   IppStatus CallInit();
   IppStatus CallFree();
};

#endif // !defined(AFX_RUNRAND_H__7ED67F85_AF96_4D1C_B71A_139CCD51C6E3__INCLUDED_)
