/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunGamma.h: interface for the CRunGamma class.
// CRunGamma class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNGAMMA_H__A4F4966D_AB9F_40A6_A0CB_7FA179FAA9FF__INCLUDED_)
#define AFX_RUNGAMMA_H__A4F4966D_AB9F_40A6_A0CB_7FA179FAA9FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunGamma : public CippiRun
{
public:
   CRunGamma();
   virtual ~CRunGamma();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   float m_vMin;
   float m_vMax;
};

#endif // !defined(AFX_RUNGAMMA_H__A4F4966D_AB9F_40A6_A0CB_7FA179FAA9FF__INCLUDED_)
