/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVNorm.h: interface for the CRunCVNorm class.
// CRunCVNorm class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVNORM_H__FBC9A963_FFC2_4842_BAB8_F56740E9C736__INCLUDED_)
#define AFX_RUNCVNORM_H__FBC9A963_FFC2_4842_BAB8_F56740E9C736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
class CParmCVNormDlg;

class CRunCVNorm : public CippiRun
{
public:
   CRunCVNorm();
   virtual ~CRunCVNorm();
   void SetValue(CParmCVNormDlg* pDlg);
protected:
   virtual BOOL Open(CFunc funcName);
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL AfterCall() { return FALSE;}

   int m_coi;
   Ipp64f m_value;
};

#endif // !defined(AFX_RUNCVNORM_H__FBC9A963_FFC2_4842_BAB8_F56740E9C736__INCLUDED_)
