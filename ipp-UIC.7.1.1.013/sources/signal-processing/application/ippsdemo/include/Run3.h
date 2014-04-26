/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run3.h : interface for the CRun3 class.
// CRun3 class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN3_H__6F634AAB_C7AE_4E0F_B6CD_9C6836A720E0__INCLUDED_)
#define AFX_RUN3_H__6F634AAB_C7AE_4E0F_B6CD_9C6836A720E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRun3 : public CippsRun  
{
public:
   CRun3(){}
   virtual ~CRun3(){}
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual int GetDstLength();
};

#endif // !defined(AFX_RUN3_H__6F634AAB_C7AE_4E0F_B6CD_9C6836A720E0__INCLUDED_)
