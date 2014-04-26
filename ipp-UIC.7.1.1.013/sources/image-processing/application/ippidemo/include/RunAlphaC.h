/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunAlphaC.h: interface for the CRunAlphaC class.
// CRunAlphaC class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNALPHAS_H__4E085303_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
#define AFX_RUNALPHAS_H__4E085303_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RunAlpha.h"

class CRunAlphaC : public CRunAlpha
{
public:
   CRunAlphaC();
   virtual ~CRunAlphaC();
protected:

   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();
   virtual IppStatus CallPremul(void* pSrc, int srcStep,
                        void* pDst, int dstStep,
                        CValue& alpha);
   virtual CString GetHistoryParms();

   virtual CString GetMpyParms1();
   virtual CString GetMpyParms2();
};

#endif // !defined(AFX_RUNALPHAS_H__4E085303_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
