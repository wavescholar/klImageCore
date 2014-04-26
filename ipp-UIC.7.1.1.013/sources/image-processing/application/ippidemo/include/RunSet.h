/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSet.h: interface for the CRunSet class.
// CRunSet class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSET_H__8C784217_C76F_4DB8_B80E_5727A87B27EE__INCLUDED_)
#define AFX_RUNSET_H__8C784217_C76F_4DB8_B80E_5727A87B27EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunSet : public CippiRun
{
public:
   CRunSet();
   virtual ~CRunSet();
protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CVector m_value ;
   CVector m_set8u;
   CVector m_set16s;
   CVector m_set32s;
   CVector m_set32f;

   int m_channel;

};

#endif // !defined(AFX_RUNSET_H__8C784217_C76F_4DB8_B80E_5727A87B27EE__INCLUDED_)
