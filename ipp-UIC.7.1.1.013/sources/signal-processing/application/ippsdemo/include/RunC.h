/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunC.h : interface for the CRunC class.
// CRunC class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNC_H__30BC390B_3E62_49BC_82BF_B5442B25C092__INCLUDED_)
#define AFX_RUNC_H__30BC390B_3E62_49BC_82BF_B5442B25C092__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
#include "Value.h"

class CRunC : public CippsRun  
{
public:
   CRunC();
   virtual ~CRunC(){}
protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   CValue val;
   CValue m_valAdd  ;
   CValue m_valRev  ;
   CValue m_valMul  ;
   CValue m_valAnd  ;
   CValue m_valShift;
   CValue m_valEmphase;
};

#endif // !defined(AFX_RUNC_H__30BC390B_3E62_49BC_82BF_B5442B25C092__INCLUDED_)
