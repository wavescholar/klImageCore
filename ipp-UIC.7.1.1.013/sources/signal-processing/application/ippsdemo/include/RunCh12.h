
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCh12.h : interface for the CRunCh12 class.
// CRunCh12 class processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_RUN12_H__66F42FA9_FEE4_447C_98E5_4B300DD211C5__INCLUDED_)
#define AFX_RUN12_H__66F42FA9_FEE4_447C_98E5_4B300DD211C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunCh12 : public CippsRun  
{
public:
   CRunCh12();
   virtual ~CRunCh12();
protected:
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();

};

#endif // !defined(AFX_RUN12_H__66F42FA9_FEE4_447C_98E5_4B300DD211C5__INCLUDED_)
