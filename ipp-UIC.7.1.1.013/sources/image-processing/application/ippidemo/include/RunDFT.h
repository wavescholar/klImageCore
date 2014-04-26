/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDFT.h: interface for the CRunDFT class.
// CRunDFT class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDFT_H__161FDAA5_840C_4AF1_84A1_9C953195EB76__INCLUDED_)
#define AFX_RUNDFT_H__161FDAA5_840C_4AF1_84A1_9C953195EB76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunDFT : public CippiRun
{
public:
   CRunDFT();
   virtual ~CRunDFT();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppHintAlgorithm m_hint;
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

#endif // !defined(AFX_RUNDFT_H__161FDAA5_840C_4AF1_84A1_9C953195EB76__INCLUDED_)
