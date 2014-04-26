/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSamp.h : interface for the CRunSamp class.
// CRunSamp class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSAMP_H__3F40A167_87B8_4BF6_914D_7C1DCDA844CD__INCLUDED_)
#define AFX_RUNSAMP_H__3F40A167_87B8_4BF6_914D_7C1DCDA844CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunSamp : public CippsRun  
{
public:
   CRunSamp();
   virtual ~CRunSamp();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual int GetDstLength();
   virtual CString GetHistoryParms();

   int m_factor;
   int m_phase ;
   int m_dstLen;

   int m_phaseSave;
};

#endif // !defined(AFX_RUNSAMP_H__3F40A167_87B8_4BF6_914D_7C1DCDA844CD__INCLUDED_)
