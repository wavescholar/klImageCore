/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunThresh.h: interface for the CRunThresh class.
// CRunThresh class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTHRESH_H__8D7C6498_A714_4596_B0E6_08694E186554__INCLUDED_)
#define AFX_RUNTHRESH_H__8D7C6498_A714_4596_B0E6_08694E186554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunThresh : public CippiRun
{
public:
   CRunThresh();
   virtual ~CRunThresh();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CVectorUnit m_threshLT;
   CVectorUnit m_threshGT;
   CVectorUnit m_valueLT ;
   CVectorUnit m_valueGT ;
   IppCmpOp  m_cmpOp;

   int m_numCoi;
};

#endif // !defined(AFX_RUNTHRESH_H__8D7C6498_A714_4596_B0E6_08694E186554__INCLUDED_)
