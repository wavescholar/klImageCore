/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunNorm.h: interface for the CRunNorm class.
// CRunNorm class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNNORM_H__4C1835AA_B42F_41B4_9DBA_AE303F65C9DC__INCLUDED_)
#define AFX_RUNNORM_H__4C1835AA_B42F_41B4_9DBA_AE303F65C9DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CParmNormDlg;

class CRunNorm : public CippiRun
{
public:
   CRunNorm();
   virtual ~CRunNorm();
   void SetNorm(CParmNormDlg* pDlg);
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual CString GetHistoryParms();

   Ipp64f m_norm[4];
   Ipp32f m_qualityIndex[4];
   IppHintAlgorithm hint;
};

#endif // !defined(AFX_RUNNORM_H__4C1835AA_B42F_41B4_9DBA_AE303F65C9DC__INCLUDED_)
