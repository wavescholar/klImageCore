/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFilterBox.h: interface for the CRunFilterBox class.
// CRunFilterBox class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNMAX_H__1F5F9DEF_C1CC_4B16_822C_BB27C4AF03F9__INCLUDED_)
#define AFX_RUNMAX_H__1F5F9DEF_C1CC_4B16_822C_BB27C4AF03F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunFilterBox : public CippiRun
{
public:
   CRunFilterBox();
   virtual ~CRunFilterBox();
   virtual BOOL IsMovie(CFunc func);

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual int GetSrcBorder(int idx);
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

   IppiSize  m_maskSize;
   IppiPoint m_anchor;
   BOOL      m_bCenter;

   IppiSize  m_maskSize_Save;
   IppiPoint m_anchor_Save;
};

#endif // !defined(AFX_RUNMAX_H__1F5F9DEF_C1CC_4B16_822C_BB27C4AF03F9__INCLUDED_)
