/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunColor.h: interface for the CRunColor class.
// CRunColor class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCOLOR_H__966B2BB6_E102_4ECD_B722_E460167E2E84__INCLUDED_)
#define AFX_RUNCOLOR_H__966B2BB6_E102_4ECD_B722_E460167E2E84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunColor : public CippiRun
{
public:
   CRunColor();
   virtual ~CRunColor();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   float m_coeffs[3];
};

#endif // !defined(AFX_RUNCOLOR_H__966B2BB6_E102_4ECD_B722_E460167E2E84__INCLUDED_)
