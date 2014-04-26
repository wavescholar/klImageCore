
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChRegExp.h : interface for the CRunChRegExp class.
// CRunChRegExp class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNCHREGEXP_H__
#define __RUNCHREGEXP_H__

#pragma once

#include "ippsRun.h"
class CParmChRegExpDlg;

class CRunChRegExp: public CippsRun  
{
public:
   CRunChRegExp();
   virtual ~CRunChRegExp();
   BOOL SetValues(CParmChRegExpDlg* pDlg);
protected:
   virtual BOOL Open(CFunc func);
   virtual void  Close();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   BOOL Init();
   BOOL Free();

   CMyString       m_pPattern;
   CMyString       m_pOptions;
   IppRegExpState* m_pState;
   int             m_errOffset;
   IppRegExpFind*  m_pFind;
   int             m_numFindIn;
   int             m_numFind;

};

#endif // !defined __RUNCHREGEXP_H__
