
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChFindC.h : interface for the CRunChFindC class.
// CRunChFindC class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUNFINDC_H__AA16C80A_A806_4C89_8482_F8EB3653F588__INCLUDED_)
#define AFX_RUNFINDC_H__AA16C80A_A806_4C89_8482_F8EB3653F588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunChFind.h"
class CParmChFindCDlg;

class CRunChFindC : public CRunChFind  
{
public:
   CRunChFindC();
   virtual ~CRunChFindC();
   void SetValues(CParmChFindCDlg* pDlg);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall(BOOL bOK) { return FALSE;}

   int m_valFind;

};

#endif // !defined(AFX_RUNFINDC_H__AA16C80A_A806_4C89_8482_F8EB3653F588__INCLUDED_)
