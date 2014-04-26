/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDCT.h : interface for the CRunDCT class.
// CRunDCT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDCT_H__E430D907_1008_4D45_B0BE_E209F51D2A1A__INCLUDED_)
#define AFX_RUNDCT_H__E430D907_1008_4D45_B0BE_E209F51D2A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunDCT : public CippsRun  
{
public:
   CRunDCT();
   virtual ~CRunDCT();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   void* m_pSpec;
   Ipp8u* m_pBuffer;
   IppHintAlgorithm m_hint;

   CFunc m_InitName;
   CFunc m_FreeName;
   CFunc m_GetBufSizeName;

   IppStatus CallInit();
   IppStatus CallFree();
   IppStatus CallGetBufSize(int& sise);

};

#endif // !defined(AFX_RUNDCT_H__E430D907_1008_4D45_B0BE_E209F51D2A1A__INCLUDED_)
