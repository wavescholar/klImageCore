/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunStat.h: interface for the CRunStat class.
// CRunStat class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSTATIDX_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_)
#define AFX_RUNSTATIDX_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
class CParmStatIdxDlg;

class CRunStatIdx : public CippiRun
{
public:
   CRunStatIdx();
   virtual ~CRunStatIdx();
   void SetValues(CParmStatIdxDlg* pDlg);
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   CVectorUnit m_value;

   int    m_indexX[4];
   int    m_indexY[4];
};

#endif // !defined(AFX_RUNSTATIDX_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_)
