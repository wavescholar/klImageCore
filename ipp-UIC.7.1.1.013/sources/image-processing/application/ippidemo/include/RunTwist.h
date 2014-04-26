/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunTwist.h: interface for the CRunTwist class.
// CRunTwist class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTWIST_H__E2579594_76FD_4B1A_AEB1_E4C71A4E42BE__INCLUDED_)
#define AFX_RUNTWIST_H__E2579594_76FD_4B1A_AEB1_E4C71A4E42BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunTwist : public CippiRun
{
public:
   CRunTwist();
   virtual ~CRunTwist();
   virtual BOOL IsMovie(CFunc func);

   enum {TW_COPY, TW_CYCLE, TW_AVER, TW_EMPH, TW_CUSTOM,
      TW_NUM};

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

   float m_twist[3][4];
   float* m_twistFP;
   int    m_twistType;

   float m_twist_Save[3][4];
};

#endif // !defined(AFX_RUNTWIST_H__E2579594_76FD_4B1A_AEB1_E4C71A4E42BE__INCLUDED_)
