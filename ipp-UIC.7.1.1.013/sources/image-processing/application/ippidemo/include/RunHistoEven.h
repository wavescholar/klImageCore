/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHistoEven.h: interface for the CRunHistoEven class.
// CRunHistoEven class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNHISTOEVEN_H__3F9A27A4_207E_4E8C_89D0_E90C2BF3B353__INCLUDED_)
#define AFX_RUNHISTOEVEN_H__3F9A27A4_207E_4E8C_89D0_E90C2BF3B353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
#include "LUT.h"
class CParmHistoEvenDlg;

class CRunHistoEven : public CippiRun
{
public:
   CRunHistoEven();
   virtual ~CRunHistoEven();
   BOOL SetValues(CParamDlg* pDlg, BOOL bMessage = FALSE);
   static CString GetNextHistTitle();
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual void ActivateDst();
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual void SetHistory();
   virtual CString GetHistoryParms();

   CDemoDoc* m_pDocHist;

   CLUT m_LUT;
   CLUT m_LUT_8u;
   CLUT m_LUT_16s;
   CLUT m_LUT_32f;

   static int m_NextHistNumber;

   void UpdateLUT(BOOL bSave = TRUE);
   void ShowHistogram();
   BOOL CompareMessage(BOOL equal, Ipp32s normC[]);
};

#endif // !defined(AFX_RUNHISTOEVEN_H__3F9A27A4_207E_4E8C_89D0_E90C2BF3B353__INCLUDED_)
