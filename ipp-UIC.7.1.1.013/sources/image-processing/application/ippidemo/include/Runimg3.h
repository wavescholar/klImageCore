/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunImg3.h: interface for the CRunImg3 class.
// CRunImg3 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNIMG3_H__6AF4C0A1_C1C8_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_RUNIMG3_H__6AF4C0A1_C1C8_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"

class CRunImg3 : public CippiRun
{
public:
   CRunImg3();
   virtual ~CRunImg3();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
};

#endif // !defined(AFX_RUNIMG3_H__6AF4C0A1_C1C8_11D1_AE6B_444553540000__INCLUDED_)
