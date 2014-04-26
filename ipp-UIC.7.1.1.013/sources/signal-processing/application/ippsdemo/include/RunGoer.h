/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunGoer.h : interface for the CRunGoer class.
// CRunGoer class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNGOER_H__E5071ACD_FECF_449F_9F53_925760E85A2A__INCLUDED_)
#define AFX_RUNGOER_H__E5071ACD_FECF_449F_9F53_925760E85A2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
#include "Value.h"

class CParmGoerDlg;

class CRunGoer : public CippsRun  
{
public:
   CRunGoer();
   virtual ~CRunGoer();
   void SetValues(CParmGoerDlg* pDlg);
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc func);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall(BOOL bOk) { return FALSE;}
   virtual CString GetHistoryParms();

   CValue m_Val[2];
   CValue m_Freq[2];
   CVector pVal;
   CVector pFreq;
};

#endif // !defined(AFX_RUNGOER_H__E5071ACD_FECF_449F_9F53_925760E85A2A__INCLUDED_)
