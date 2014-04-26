/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCVStat.h: interface for the CRunCVStat class.
// CRunCVStat class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCVSTAT_H__52B5A784_93B0_4F10_9B5E_46E866B40CB0__INCLUDED_)
#define AFX_RUNCVSTAT_H__52B5A784_93B0_4F10_9B5E_46E866B40CB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
class CParmCVStatDlg;

class CRunCVStat : public CippiRun
{
public:
   CRunCVStat();
   virtual ~CRunCVStat();
   void SetValues(CParmCVStatDlg* pDlg);
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual BOOL AfterCall() { return FALSE;}

   int m_coi;
   Ipp32f m_pVal32f[2];
   Ipp64f m_pVal64f[2];
   IppiPoint m_pIndex[2];

   CFunc GetFuncName(int index);
   int GetFuncIndex();
};

#endif // !defined(AFX_RUNCVSTAT_H__52B5A784_93B0_4F10_9B5E_46E866B40CB0__INCLUDED_)
