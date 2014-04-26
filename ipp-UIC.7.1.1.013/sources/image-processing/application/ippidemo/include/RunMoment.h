/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMoment.h: interface for the CRunMoment class.
// CRunMoment class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNMOMENT_H__85BF1E52_A922_4CBB_8BF2_BBAF3B34EBDE__INCLUDED_)
#define AFX_RUNMOMENT_H__85BF1E52_A922_4CBB_8BF2_BBAF3B34EBDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CParmMomentDlg;

class CRunMoment : public CippiRun
{
public:
   CRunMoment();
   virtual ~CRunMoment();

   void SetMoments(CParmMomentDlg* pDlg);
protected:
   virtual BOOL Open(CFunc func);
   virtual void UpdateData(CParamDlg* parmDlg, BOOL save = TRUE);
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   IppStatus CallInit();
   IppStatus CallFree();
   IppStatus GetMoments();
   IppStatus CallGet(int mOrd, int nOrd, void* value);

   enum {VAL_NUM = 10};
   BOOL m_bFloat;
   CFunc m_GetFunc;
   CMyString m_InitName;
   CMyString m_FreeName;
   IppHintAlgorithm   m_hint;
   void*    m_pState;
   int      m_Channel;
   Ipp64f   m_value64f[VAL_NUM];
   Ipp64s   m_value64s[VAL_NUM];
   int      m_valNum;

};

#endif // !defined(AFX_RUNMOMENT_H__85BF1E52_A922_4CBB_8BF2_BBAF3B34EBDE__INCLUDED_)
