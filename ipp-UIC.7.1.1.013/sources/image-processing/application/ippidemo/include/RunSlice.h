/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSlice.h: interface for the CRunSlice class.
// CRunSlice class processes image by ippCC functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSLICE_H__6DC53028_7D3C_430F_8F26_49E3E6B7D23A__INCLUDED_)
#define AFX_RUNSLICE_H__6DC53028_7D3C_430F_8F26_49E3E6B7D23A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunSlice : public CippiRun
{
public:
   CRunSlice();
   virtual ~CRunSlice();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL PrepareSrc();
   virtual IppStatus CallIppFunction();
   virtual BOOL CallIpp(BOOL bMessage = TRUE);
   virtual void Loop(int num);
   virtual CString GetHistoryInfo();

   int m_slice;
   char* m_pSrcY;
   char* m_pSrcUV;
   char* m_pDst;
   char* m_pDstP[4];
   IppiSize m_roiSize;
   int m_layout;
};

#endif // !defined(AFX_RUNSLICE_H__6DC53028_7D3C_430F_8F26_49E3E6B7D23A__INCLUDED_)
