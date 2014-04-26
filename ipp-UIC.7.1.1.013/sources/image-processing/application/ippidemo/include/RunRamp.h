/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRamp.h: interface for the CRunRamp class.
// CRunRamp class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNRAMP_H__923A82C8_C2D1_4103_B1D8_D3DE01948FAB__INCLUDED_)
#define AFX_RUNRAMP_H__923A82C8_C2D1_4103_B1D8_D3DE01948FAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunRamp : public CippiRun
{
public:
   CRunRamp();
   virtual ~CRunRamp();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue m_offset;
   CValue m_slope;
   IppiAxis m_axis;
};

#endif // !defined(AFX_RUNRAMP_H__923A82C8_C2D1_4103_B1D8_D3DE01948FAB__INCLUDED_)
