/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMove.h : interface for the CRunMove class.
// CRunMove class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNMOVE_H__7B09769A_D879_45C8_AD63_2F0F9A6308AC__INCLUDED_)
#define AFX_RUNMOVE_H__7B09769A_D879_45C8_AD63_2F0F9A6308AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunMove : public CippsRun  
{
public:
   CRunMove();
   virtual ~CRunMove();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall(); 
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryInfo();

   int   m_len;
   int   m_offsetSrc;
   int   m_offsetDst;

};

#endif // !defined(AFX_RUNMOVE_H__7B09769A_D879_45C8_AD63_2F0F9A6308AC__INCLUDED_)
