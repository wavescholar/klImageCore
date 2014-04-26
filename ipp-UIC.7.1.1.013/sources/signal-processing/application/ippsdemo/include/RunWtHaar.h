/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWtHaar.h : interface for the CRunWtHaar class.
// CRunWtHaar class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNWTHAAR_H__C44437A0_A1D1_461E_B8A9_EC41728C733A__INCLUDED_)
#define AFX_RUNWTHAAR_H__C44437A0_A1D1_461E_B8A9_EC41728C733A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunWtHaar : public CippsRun  
{
public:
   CRunWtHaar();
   virtual ~CRunWtHaar();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual BOOL BeforeCall();
   virtual int GetDstLength();
   virtual int GetDst2Length();
   virtual CString GetHistoryParms();

   BOOL m_Fwd;
};

#endif // !defined(AFX_RUNWTHAAR_H__C44437A0_A1D1_461E_B8A9_EC41728C733A__INCLUDED_)
