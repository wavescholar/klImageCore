
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChTrim.h : interface for the CRunChTrim class.
// CRunChTrim class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNTRIM_H__E572C279_823C_4B4F_A977_5E1A3938A1BA__INCLUDED_)
#define AFX_RUNTRIM_H__E572C279_823C_4B4F_A977_5E1A3938A1BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunChTrim : public CippsRun  
{
public:
   CRunChTrim();
   virtual ~CRunChTrim();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg(); 
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();
   virtual CMyString GetVectorName(int vecPos);
   virtual BOOL GetPickID(int vecPos, UINT& pickID, UINT& grabID);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual CString GetHistoryParms();

   BOOL   m_bAny;
   int m_len;
   CValue m_odd;
};

#endif // !defined(AFX_RUNTRIM_H__E572C279_823C_4B4F_A977_5E1A3938A1BA__INCLUDED_)
