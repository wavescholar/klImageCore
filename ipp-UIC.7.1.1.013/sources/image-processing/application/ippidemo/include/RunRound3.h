/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRound3.h: interface for the CRunRound3 class.
// CRunRound3 class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun & CRunRound classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNROUND3_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_)
#define AFX_RUNROUND3_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RunRound.h"

class CRunRound3 : public CRunRound
{
public:
   CRunRound3();
   virtual ~CRunRound3();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
};

#endif // !defined(AFX_RUNROUND3_H__E544B99C_D51A_4100_BD4C_A8F9EAD99232__INCLUDED_)
