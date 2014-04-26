/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDirector.h: interface for the CippsDirector class.
// class CippsDirector processes vector by any ippSP function that
// can be called by one of CRun classes listed in CippsDirector constructor.
// See CDirector class for more information
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSDIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_)
#define AFX_IPPSDIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Director.h"
#include "ippsProcess.h"

class CippsDirector : public CDirector
{
public:
   CippsDirector();
   virtual ~CippsDirector() {}
protected:
/////////////////////////////////////////////////////////////////////////////
// CDirector virtual function overrides

   // NewProcess creates new CProcess class that provides all implemented 
   // IPP functions classification for ippIP, ippCC, ippCV, ippJP libraries
   virtual CProcess* NewProcess();
   // DeleteProcess delets CProcess class created by NewProcess function
   virtual void DeleteProcess(CProcess*& pProcess);

   void InitSP();
   void InitCH();
};


#endif // !defined(AFX_IPPSDIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_)
