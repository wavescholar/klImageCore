/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFFT.h: interface for the CRunFFT class.
// CRunFFT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFFT_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
#define AFX_RUNFFT_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunFFT : public CippiRun
{
public:
   CRunFFT();
   virtual ~CRunFFT();
   static int flagToRadio(int flag);
   static int radioToFlag(int radio, BOOL bFwd);
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PrepareSrc();
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppHintAlgorithm m_hint;
   int    m_orderX;
   int    m_orderY;
   int    m_flag;
   void*  m_pSpec;
   int    m_bufSize;
   Ipp8u* m_pBuffer;
   CString m_initName;
   CString m_freeName;
   CString m_getBufSizeName;

   IppStatus CallInit(CString name);
   IppStatus CallFree(CString name);
   IppStatus CallGetBufSize(CString name);
};

#endif // !defined(AFX_RUNFFT_H__642DE1F5_E74A_4EEC_AC90_EC8BEAB7817B__INCLUDED_)
