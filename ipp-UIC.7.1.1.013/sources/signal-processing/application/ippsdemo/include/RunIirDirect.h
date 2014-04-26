/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunIirDirect.h : interface for the CRunIirDirect class.
// CRunIirDirect class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNIIRDIRECT_H__6DF14ABB_E37E_48A4_915A_0E0626270B89__INCLUDED_)
#define AFX_RUNIIRDIRECT_H__6DF14ABB_E37E_48A4_915A_0E0626270B89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunFirD.h"

class CRunIirDirect : public CRunFirD  
{
public:
   CRunIirDirect();
   virtual ~CRunIirDirect();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();

   virtual BOOL BeforeCall();
   virtual CString GetHistoryParms();
   virtual int GetTapsLen();

   BOOL m_BQ;
   int m_order;
   int m_numQuads;
};

#endif // !defined(AFX_RUNIIRDIRECT_H__6DF14ABB_E37E_48A4_915A_0E0626270B89__INCLUDED_)
