/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunNorm.h : interface for the CRunNorm class.
// CRunNorm class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNNORM_H__E94014BE_55A1_4C2A_83F1_E3D2C47AA95B__INCLUDED_)
#define AFX_RUNNORM_H__E94014BE_55A1_4C2A_83F1_E3D2C47AA95B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
#include "Value.h"

class CParmNormDlg;

class CRunNorm : public CippsRun  
{
public:
   CRunNorm();
   virtual ~CRunNorm();
   void SetValues(CParmNormDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   CValue m_value;
};

#endif // !defined(AFX_RUNNORM_H__E94014BE_55A1_4C2A_83F1_E3D2C47AA95B__INCLUDED_)
