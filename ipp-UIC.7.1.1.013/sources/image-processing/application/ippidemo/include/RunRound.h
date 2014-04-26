/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRound.h: interface for the CRunRound class.
// CRunRound class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNROUND_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_)
#define AFX_RUNROUND_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunRound : public CippiRun
{
public:
   CRunRound();
   virtual ~CRunRound();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppRoundMode m_roundMode;
};

#endif // !defined(AFX_RUNROUND_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_)
