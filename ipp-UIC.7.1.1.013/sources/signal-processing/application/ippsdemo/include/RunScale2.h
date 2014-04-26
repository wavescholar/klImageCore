
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunScale2.h : interface for the CRunScale2 class.
// CRunScale2 class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNSCALE2_H__
#define __RUNSCALE2_H__

#pragma once

#include "ippsRun.h"

class CRunScale2: public CippsRun  
{
public:
   CRunScale2();
   virtual ~CRunScale2();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_scaleFactor1;
   int m_scaleFactor2;
};

#endif // !defined __RUNSCALE2_H__
