/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopyM.h: interface for the CRunCopyM class.
// CRunCopyM class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNCOPYM_H__96385C36_6AC1_4D53_BDD0_14195E79D915__INCLUDED_)
#define AFX_RUNCOPYM_H__96385C36_6AC1_4D53_BDD0_14195E79D915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippiRun.h"

class CRunCopyM : public CippiRun
{
public:
   CRunCopyM();
   virtual ~CRunCopyM();
protected:
   virtual BOOL Open(CFunc func);
   virtual IppStatus CallIppFunction();

};

#endif // !defined(AFX_RUNCOPYM_H__96385C36_6AC1_4D53_BDD0_14195E79D915__INCLUDED_)
