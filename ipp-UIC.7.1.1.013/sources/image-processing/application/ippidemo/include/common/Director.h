/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Director.h: interface for the CDirector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_)
#define AFX_DIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Run.h"

class CProcess;

//////////////////////////////////////////////////////////////////////
// class CDirector processes any document by any IPP
// function
//////////////////////////////////////////////////////////////////////

class CDirector : public CRunList
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor
   CDirector();
   virtual ~CDirector();

/////////////////////////////////////////////////////////////////////////////
// Public Operations

   // ProcessVector processes specified document vector by IPP function 
   // that will be obtained by Process Dialog
   virtual void ProcessVector(CDemoDoc* pDoc);
   // ProcessVector processes specified document vector by specified IPP 
   // function 
   virtual void ProcessVector(CDemoDoc* pDoc, CFunc func);
   // StopProcess cancels document processing
   virtual void StopProcess();
   // PickNext starts to pick a document as the next IPP function parameter
   // or starts IPP function call if all required function parameters have
   // been picked out already
   virtual void PickNext();
   // GrabDoc sets chosen document as IPP function parameter and calls 
   // PickNext
   virtual void GrabDoc(CDemoDoc* pDoc);

   // NewProcess creates new CProcess class that provides classification for 
   // all implemented IPP functions 
   virtual CProcess* NewProcess();
   // NewProcess creates new CProcess class that provides classification
   // for IPP functions which can process specified vector 
   virtual CProcess* NewProcess(CVector* pVector) ;
   // DeleteProcess delets CProcess class created by NewProcess function
   virtual void DeleteProcess(CProcess*& pProcess);

/////////////////////////////////////////////////////////////////////////////
// Public Attributes
   // GetRun returns CRun class that implements processing IPP function
   CRun* GetRun() const { return m_pRun;}
   // GetFunc returns processing IPP function
   CFunc GetFunc() const;
   // GetProcess returns class that provides classification for all 
   // implemented IPP functions 
   CProcess* GetProcess() const { return m_pProcess;}
   // GetFunctions sets to specified funcList all implemented IPP functions
   // of IPP library with specified index
   virtual void GetFunctions (CFuncList& funcList, int libIdx)const;

/////////////////////////////////////////////////////////////////////////////
// Implementation
protected:
   CRun* m_pRun;
   CProcess* m_pProcess;

   // ProcessVector processes picked documents by chosen IPP function
   virtual void ProcessVector();
   // PickVectors starts document picking process
   virtual void PickVectors();

};


#endif // !defined(AFX_DIRECTOR_H__7B0CD00C_C539_4FE7_A9A4_FEA441935282__INCLUDED_)
