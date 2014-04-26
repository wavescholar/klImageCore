/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDotProd.h: interface for the CRunDotProd class.
// CRunDotProd class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDOTPROD_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_)
#define AFX_RUNDOTPROD_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunDotProd : public CippiRun
{
public:
   CRunDotProd();
   virtual ~CRunDotProd();
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual BOOL ProcessCall();

   Ipp64f m_DotProd[4];
   Ipp64f* pDotProd;
   IppHintAlgorithm hint;
   int m_Num;
};

#endif // !defined(AFX_RUNDOTPROD_H__8B619FE6_7CFD_4A04_A280_FEC7CF646E2B__INCLUDED_)
