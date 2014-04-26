/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSuper.h: interface for the CRunSuper class.
// CRunSuper class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNSUPER_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_RUNSUPER_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunSuper : public CippiRun
{
public:
   CRunSuper();
   virtual ~CRunSuper();

protected:
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual ImgHeader GetNewDstHeader();
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();

   void* m_pBuffer;

};

#endif // !defined(AFX_RUNSUPER_H__44AAAF66_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
