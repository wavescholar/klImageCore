/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunC13.h: interface for the CRunC13 class.
// CRunC13 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNC13_H__E8FC2013_E5A4_11D2_8EEA_00AA00A03C3C__INCLUDED_)
#define AFX_RUNC13_H__E8FC2013_E5A4_11D2_8EEA_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunC13 : public CippiRun
{
public:
   CRunC13();
   virtual ~CRunC13();

protected:

   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual void Close();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   int m_srcChannel;
   int m_dstChannel;

};

#endif // !defined(AFX_RUNC13_H__E8FC2013_E5A4_11D2_8EEA_00AA00A03C3C__INCLUDED_)
