/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCompColor.h: interface for the CRunCompColor class.
// CRunCompColor class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCOMPCOLOR_H__6AF4C0A5_C1C8_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_RUNCOMPCOLOR_H__6AF4C0A5_C1C8_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunCompColor : public CippiRun
{
public:
   CRunCompColor();
   virtual ~CRunCompColor();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CVectorUnit m_colorKey;
};

#endif // !defined(AFX_RUNCOMPCOLOR_H__6AF4C0A5_C1C8_11D1_AE6B_444553540000__INCLUDED_)
