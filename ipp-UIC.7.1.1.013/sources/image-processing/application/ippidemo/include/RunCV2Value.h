/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCV3.h: interface for the CRunCV2Value class.
// CRunCV2Value class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCV2VALUE_H__A755F261_D7A0_4179_8BE3_C20BBDA211C2__INCLUDED_)
#define AFX_RUNCV2VALUE_H__A755F261_D7A0_4179_8BE3_C20BBDA211C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCV2Value : public CippiRun
{
public:
   CRunCV2Value();
   virtual ~CRunCV2Value();
protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual CString GetHistoryParms();
   virtual IppStatus CallIppFunction();

   CValue m_value;
   CValue m_valAbs;
   CValue m_valAdd;
};

#endif // !defined(AFX_RUNCV2VALUE_H__A755F261_D7A0_4179_8BE3_C20BBDA211C2__INCLUDED_)
