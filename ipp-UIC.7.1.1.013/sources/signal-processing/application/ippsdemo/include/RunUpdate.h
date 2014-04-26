/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunUpdate.h : interface for the CRunUpdate class.
// CRunUpdate class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNUPDATE_H__855AA5EE_A1F9_4366_9450_7324BAC88A97__INCLUDED_)
#define AFX_RUNUPDATE_H__855AA5EE_A1F9_4366_9450_7324BAC88A97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
class CParmUpdateDlg;

class CRunUpdate : public CippsRun  
{
public:
   CRunUpdate();
   virtual ~CRunUpdate();
   void SetValue(CParmUpdateDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}
   virtual CString GetHistoryParms();

   CValue m_alpha;
   CValue m_shift;
   CValue m_srcDst;
   CValue m_srcDstIn;
   IppHintAlgorithm m_hint;
};

#endif // !defined(AFX_RUNUPDATE_H__855AA5EE_A1F9_4366_9450_7324BAC88A97__INCLUDED_)
