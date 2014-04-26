/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunLut.h: interface for the CRunLut class.
// CRunLut class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNLUT_H__FE3CC044_880B_4641_BFFF_F6D7E194295B__INCLUDED_)
#define AFX_RUNLUT_H__FE3CC044_880B_4641_BFFF_F6D7E194295B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"
#include "LUT.h"

class CRunLut : public CippiRun
{
public:
   CRunLut();
   virtual ~CRunLut();
protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual IppStatus CallIppFunction();
   virtual CString GetHistoryParms();
   virtual CString GetHistoInfo();

   CLUT m_LUT;
   CLUT m_LUT_8u;
   CLUT m_LUT_16s;
   CLUT m_LUT_32f;

   int m_F[4];
   double m_A[4];

   void UpdateLUT(BOOL bSave = TRUE);
};

#endif // !defined(AFX_RUNLUT_H__FE3CC044_880B_4641_BFFF_F6D7E194295B__INCLUDED_)
