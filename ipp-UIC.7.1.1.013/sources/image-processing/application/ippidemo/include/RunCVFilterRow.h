/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVFilterRow.h: interface for the CRunCVFilterRow class.
// CRunCVFilterRow class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVFILTERROW_H__A1B29B18_12A7_4FE5_8581_FBC4EFBD52D8__INCLUDED_)
#define AFX_RUNCVFILTERROW_H__A1B29B18_12A7_4FE5_8581_FBC4EFBD52D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCVFilterRow : public CippiRun
{
public:
   CRunCVFilterRow();
   virtual ~CRunCVFilterRow();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   virtual IppStatus CallGetBufSize(CString name);
   CString m_GetBufSizeName;
   Ipp8u* m_pBuffer;
   int m_bufSize;

   void** m_ppDst;
   int m_kernelSize;
   int m_anchor;
   BOOL     m_bCenter;
   CVectorUnit m_pKernel;
   int m_divisor;
   int m_DivType;
   IppiBorderType m_borderType;
   CVectorUnit m_borderValue;

};

#endif // !defined(AFX_RUNCVFILTERROW_H__A1B29B18_12A7_4FE5_8581_FBC4EFBD52D8__INCLUDED_)
