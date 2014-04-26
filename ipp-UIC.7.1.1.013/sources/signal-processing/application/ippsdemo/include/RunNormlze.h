/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunNormlze.h : interface for the CRunNormlze class.
// CRunNormlze class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNNORMLZE_H__4972EA31_80FB_4D84_8482_E8E627610EFC__INCLUDED_)
#define AFX_RUNNORMLZE_H__4972EA31_80FB_4D84_8482_E8E627610EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunNormlze : public CippsRun  
{
public:
   CRunNormlze();
   virtual ~CRunNormlze() {}
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue vsub;
   CValue vdiv;
};

#endif // !defined(AFX_RUNNORMLZE_H__4972EA31_80FB_4D84_8482_E8E627610EFC__INCLUDED_)
