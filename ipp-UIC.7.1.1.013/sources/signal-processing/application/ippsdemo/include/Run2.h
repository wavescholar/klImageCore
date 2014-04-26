/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run2.h : interface for the CRun2 class.
// CRun2 class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN2_H__7F8FDB13_5C3C_4C6E_B8CA_CA90F58CE968__INCLUDED_)
#define AFX_RUN2_H__7F8FDB13_5C3C_4C6E_B8CA_CA90F58CE968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRun2 : public CippsRun  
{
public:
   CRun2(){}
   virtual ~CRun2(){}
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
};

#endif // !defined(AFX_RUN2_H__7F8FDB13_5C3C_4C6E_B8CA_CA90F58CE968__INCLUDED_)
