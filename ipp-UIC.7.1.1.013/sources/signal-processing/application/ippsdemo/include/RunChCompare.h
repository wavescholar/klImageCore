
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChCompare.h : interface for the CRunChCompare class.
// CRunChCompare class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNCOMPARE_H__4586C364_CE5F_430C_8D0D_9B01C5009A65__INCLUDED_)
#define AFX_RUNCOMPARE_H__4586C364_CE5F_430C_8D0D_9B01C5009A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"
class CParmChCompareDlg;

class CRunChCompare : public CippsRun  
{
public:
   CRunChCompare();
   virtual ~CRunChCompare();
   void SetValues(CParmChCompareDlg* pDlg);
   enum {FUNC_COMP, FUNC_COMP_CASE, FUNC_COMP_LATIN, FUNC_EQUAL, FUNC_NUM};
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   int m_Comp;
};

#endif // !defined(AFX_RUNCOMPARE_H__4586C364_CE5F_430C_8D0D_9B01C5009A65__INCLUDED_)
