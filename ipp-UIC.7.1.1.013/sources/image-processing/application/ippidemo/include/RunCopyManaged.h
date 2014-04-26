/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopyManaged.h: interface for the CRunCopyManaged class.
// CRunCopyManaged class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCOPYMANAGED_H__461342FF_15B1_46E6_BD33_7208A4E21B63__INCLUDED_)
#define AFX_RUNCOPYMANAGED_H__461342FF_15B1_46E6_BD33_7208A4E21B63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCopyManaged : public CippiRun
{
public:
   CRunCopyManaged();
   virtual ~CRunCopyManaged();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_flags;
};

#endif // !defined(AFX_RUNCOPYMANAGED_H__461342FF_15B1_46E6_BD33_7208A4E21B63__INCLUDED_)
