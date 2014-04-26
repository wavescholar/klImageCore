/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDirector.h: interface for the CippiDirector class.
// class CippiDirector processes vector by any ippIP, ippCC, ippCV or ippJP
// function that can be called by one of CRun classes listed in
// CippsDirector constructor.
// See CDirector class for more information
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPIDIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_)
#define AFX_IPPIDIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Director.h"
#include "ippiProcess.h"

//////////////////////////////////////////////////////////////////////
// class CippiDirector processes any document by any IPP
// function implemented in ippIP DEmo
//////////////////////////////////////////////////////////////////////

class CippiDirector : public CDirector
{
public:
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

   CippiDirector();
   virtual ~CippiDirector() {}

protected:
/////////////////////////////////////////////////////////////////////////////
// CDirector virtual function overrides

   // NewProcess creates new CProcess class that provides all implemented
   // IPP functions classification for ippIP, ippCC, ippCV, ippJP libraries
   virtual CProcess* NewProcess();
   // DeleteProcess delets CProcess class created by NewProcess function
   virtual void DeleteProcess(CProcess*& pProcess);

/////////////////////////////////////////////////////////////////////////////
// Implementation

   // Init{IP/CC/CV/JP} adds to CRunList classes that implement
   // ippIP/ippCC/ippCV/ippJP library functions
   void InitIP();
   void InitCC();
   void InitCV();
   void InitJP();
};


#endif // !defined(AFX_IPPIDIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_)
