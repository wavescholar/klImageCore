/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilter.h: interface for the CRunCVFilter class.
// CRunCVFilter class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVFILTER_H__A8DE1AE5_1A9B_4295_86A9_CDE12ADA6CB1__INCLUDED_)
#define AFX_RUNCVFILTER_H__A8DE1AE5_1A9B_4295_86A9_CDE12ADA6CB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVFilter : public CippiRun
{
public:
   CRunCVFilter();
   virtual ~CRunCVFilter();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   virtual IppStatus CallGetBufSize(CString name);

   IppiBorderType m_borderType;
   CValue m_borderValue;
   Ipp8u* m_pBuffer;
   int m_bufSize;

   CString m_GetBufSizeName;
};

#endif // !defined(AFX_RUNCVFILTER_H__A8DE1AE5_1A9B_4295_86A9_CDE12ADA6CB1__INCLUDED_)
