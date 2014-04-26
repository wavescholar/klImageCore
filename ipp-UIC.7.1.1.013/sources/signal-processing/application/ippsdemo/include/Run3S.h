/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run3S.h : interface for the CRun3S class.
// CRun3S class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN3S_H__F42170B6_3C5E_4781_8423_E9D311B33170__INCLUDED_)
#define AFX_RUN3S_H__F42170B6_3C5E_4781_8423_E9D311B33170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRun3S : public CippsRun  
{
public:
   CRun3S();
   virtual ~CRun3S();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();

};

#endif // !defined(AFX_RUN3S_H__F42170B6_3C5E_4781_8423_E9D311B33170__INCLUDED_)
