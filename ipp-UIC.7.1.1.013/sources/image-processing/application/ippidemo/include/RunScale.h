/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunScale.h: interface for the CRunScale class.
// CRunScale class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSCALE_H__57CFCF87_28A7_4A4A_877F_5FDBEE94766A__INCLUDED_)
#define AFX_RUNSCALE_H__57CFCF87_28A7_4A4A_877F_5FDBEE94766A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunScale : public CippiRun
{
public:
   CRunScale();
   virtual ~CRunScale();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   Ipp32f m_vMin;
   Ipp32f m_vMax;
};

#endif // !defined(AFX_RUNSCALE_H__57CFCF87_28A7_4A4A_877F_5FDBEE94766A__INCLUDED_)
