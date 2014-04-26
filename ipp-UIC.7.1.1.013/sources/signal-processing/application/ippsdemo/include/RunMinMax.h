/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMinMax.h : interface for the CRunMinMax class.
// CRunMinMax class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNMINMAX_H__643EDB5F_A18F_4EAA_B726_D3A99CD784F4__INCLUDED_)
#define AFX_RUNMINMAX_H__643EDB5F_A18F_4EAA_B726_D3A99CD784F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
class CParmMinMaxDlg;

class CRunMinMax : public CippsRun  
{
public:
   CRunMinMax();
   virtual ~CRunMinMax();
   void SetValues(CParmMinMaxDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   CValue m_value[2];
   int    m_index[2];
};

#endif // !defined(AFX_RUNMINMAX_H__643EDB5F_A18F_4EAA_B726_D3A99CD784F4__INCLUDED_)
