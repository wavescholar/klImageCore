/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run1S2.h: interface for the CRun1S2 class.
// CRun1S2 class processes vectors by ippSP functions listed in 
// CallIppFunction member function.
// See CRun & CippsRun classes for more information. 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN1S2_H__839EBB37_0657_4E14_A0D6_2A1DF59520C8__INCLUDED_)
#define AFX_RUN1S2_H__839EBB37_0657_4E14_A0D6_2A1DF59520C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRun1S2 : public CippsRun  
{
public:
   CRun1S2();
   virtual ~CRun1S2();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int magnScaleFactor;
   int phaseScaleFactor;
};

#endif // !defined(AFX_RUN1S2_H__839EBB37_0657_4E14_A0D6_2A1DF59520C8__INCLUDED_)
