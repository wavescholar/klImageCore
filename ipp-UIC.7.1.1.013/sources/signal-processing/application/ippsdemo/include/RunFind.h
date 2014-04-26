/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFind.h : interface for the CRunFind class.
// CRunFind class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFIND_H__ADF25B8F_6563_4FE7_A8FB_07232A76A409__INCLUDED_)
#define AFX_RUNFIND_H__ADF25B8F_6563_4FE7_A8FB_07232A76A409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFindOne.h"

class CParmFindOneDlg;

class CRunFind : public CRunFindOne  
{
public:
   CRunFind();
   virtual ~CRunFind();
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual CString GetHistoryParms();

   Ipp32s* m_pIndex;
};

#endif // !defined(AFX_RUNFIND_H__ADF25B8F_6563_4FE7_A8FB_07232A76A409__INCLUDED_)
