/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCV2.h: interface for the CRunCV2 class.
// CRunCV2 class processes image by ippCV functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCV2_H__D0A6EB84_B0A6_4527_99E1_7B574D7D51DF__INCLUDED_)
#define AFX_RUNCV2_H__D0A6EB84_B0A6_4527_99E1_7B574D7D51DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCV2 : public CippiRun
{
public:
   CRunCV2();
   virtual ~CRunCV2();
protected:
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();

};

#endif // !defined(AFX_RUNCV2_H__D0A6EB84_B0A6_4527_99E1_7B574D7D51DF__INCLUDED_)
