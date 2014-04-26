
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChReplace.h : interface for the CRunChReplace class.
// CRunChReplace class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNREPLACE_H__6FE03AD9_4DC2_4973_B591_D6264D246658__INCLUDED_)
#define AFX_RUNREPLACE_H__6FE03AD9_4DC2_4973_B591_D6264D246658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunChReplace : public CippsRun  
{
public:
   CRunChReplace();
   virtual ~CRunChReplace();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg(); 
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue m_oldVal;
   CValue m_newVal;
};

#endif // !defined(AFX_RUNREPLACE_H__6FE03AD9_4DC2_4973_B591_D6264D246658__INCLUDED_)
