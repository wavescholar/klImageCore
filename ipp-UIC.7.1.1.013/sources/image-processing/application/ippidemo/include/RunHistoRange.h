/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunHistoRange.h: interface for the CRunHistoRange class.
// CRunHistoRange class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNHISTORANGE_H__AD08DC5F_1CAE_489A_9F75_EF9C5EFCDA34__INCLUDED_)
#define AFX_RUNHISTORANGE_H__AD08DC5F_1CAE_489A_9F75_EF9C5EFCDA34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunHistoEven.h"
class CParmHistoRangeDlg;

class CRunHistoRange : public CRunHistoEven
{
public:
   CRunHistoRange();
   virtual ~CRunHistoRange();
   BOOL SetValues(CParamDlg* pDlg, BOOL bMessage = FALSE);
protected:
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual CString GetHistoryParms();
};

#endif // !defined(AFX_RUNHISTORANGE_H__AD08DC5F_1CAE_489A_9F75_EF9C5EFCDA34__INCLUDED_)
