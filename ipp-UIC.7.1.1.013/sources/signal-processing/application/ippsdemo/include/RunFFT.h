/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFFT.h : interface for the CRunFFT class.
// CRunFFT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFFT_H__7D290E4B_55E8_4EDC_B8D0_32A2F286C9C2__INCLUDED_)
#define AFX_RUNFFT_H__7D290E4B_55E8_4EDC_B8D0_32A2F286C9C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunFFT : public CippsRun  
{
public:
   CRunFFT();
   virtual ~CRunFFT();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual int GetDstLength();
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   void* m_pSpec;
   Ipp8u* m_buffer;
   int m_flag;
   int m_order;
   IppHintAlgorithm m_hint;

   int m_flagFwd;
   int m_flagInv;
   CFunc m_InitName;
   CFunc m_FreeName;
   CFunc m_GetBufSizeName;

   IppStatus CallInit();
   IppStatus CallFree();
   IppStatus CallGetBufSize(int& status);
};

#endif // !defined(AFX_RUNFFT_H__7D290E4B_55E8_4EDC_B8D0_32A2F286C9C2__INCLUDED_)
