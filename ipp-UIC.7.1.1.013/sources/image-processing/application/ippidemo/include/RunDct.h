/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDct.h: interface for the CRunDct class.
// CRunDct class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDCT_H__798CF64F_B95B_48DC_BF45_B8F4ECBDDA92__INCLUDED_)
#define AFX_RUNDCT_H__798CF64F_B95B_48DC_BF45_B8F4ECBDDA92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunDct : public CippiRun
{
public:
   CRunDct();
   virtual ~CRunDct();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppHintAlgorithm m_hint;
   void* m_pSpec;
   int m_bufSize;
   Ipp8u* m_pBuffer;

   IppStatus CallInit(CString name);
   IppStatus CallFree(CString name);
   IppStatus CallGetBufSize(CString name);
};

#endif // !defined(AFX_RUNDCT_H__798CF64F_B95B_48DC_BF45_B8F4ECBDDA92__INCLUDED_)
