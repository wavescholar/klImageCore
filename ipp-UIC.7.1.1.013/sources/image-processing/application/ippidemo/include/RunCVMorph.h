/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVMorph.h: interface for the CRunCVMorph class.
// CRunCVMorph class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVMORPH_H__DE50A54E_6694_42B1_9783_FB6B331B743D__INCLUDED_)
#define AFX_RUNCVMORPH_H__DE50A54E_6694_42B1_9783_FB6B331B743D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunMorph.h"

class CRunCVMorph : public CRunMorph
{
public:
   CRunCVMorph();
   virtual ~CRunCVMorph();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOK);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppiMorphState* m_pState;
   IppiBorderType m_borderType;

   CString m_InitName;
   IppStatus CallInitAlloc(CString name);
};

#endif // !defined(AFX_RUNCVMORPH_H__DE50A54E_6694_42B1_9783_FB6B331B743D__INCLUDED_)
