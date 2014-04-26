/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConvert1u.h: interface for the CRunConvert1u class.
// CRunConvert1u class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCONVERT1U_H__1AFE87A0_0118_403A_A4A9_AFFBB51084E9__INCLUDED_)
#define AFX_RUNCONVERT1U_H__1AFE87A0_0118_403A_A4A9_AFFBB51084E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunConvert1u : public CippiRun
{
public:
   CRunConvert1u();
   virtual ~CRunConvert1u();

   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PrepareDst();
   virtual ImgHeader GetNewDstHeader();
   virtual void CopyContoursToNewDst();
   virtual void PrepareParameters();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();

   BOOL m_bToBits;
   int m_offset;
   Ipp8u m_threshold;

   void IntersectBitRoi(CImage* pBitImage, IppiSize& roiSize);
};

#endif // !defined(AFX_RUNCONVERT1U_H__1AFE87A0_0118_403A_A4A9_AFFBB51084E9__INCLUDED_)
