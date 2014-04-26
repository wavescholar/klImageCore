/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Director.cpp: implementation of the CDirector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "DemoDoc.h"
#include "MainFrm.h"
#include "Director.h"
#include "CProcess.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirector::CDirector()
{
   m_pRun = NULL;
   m_pProcess = NULL;
}

CDirector::~CDirector()
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations

//---------------------------------------------------------------------------
// ProcessVector processes specified document vector by IPP function 
// that will be obtained by Process Dialog
//---------------------------------------------------------------------------

void CDirector::ProcessVector(CDemoDoc* pDoc)
{
    ASSERT(pDoc);
    CFunc func;
    CProcess* pProcess = pDoc->GetProcess();
    CVector* pVector = pDoc->GetVector();
    m_pProcess->GetCurrentSettings(pProcess);
    if (!pProcess->GetFunction(pVector, func)) return;
    m_pProcess->SetCurrentSettings(pProcess);
    ProcessVector(pDoc,func);
}

//---------------------------------------------------------------------------
// ProcessVector processes specified document vector by specified IPP 
// function 
//---------------------------------------------------------------------------
void CDirector::ProcessVector(CDemoDoc* pDoc, CFunc func)
{
   try {
        POSITION pos = GetHeadPosition();
        while (pos) {
            m_pRun = GetNext(pos);
            if (m_pRun->ProcessFunctionStart(pDoc,func)) {
               PickVectors();
               return;
            }
        }
    } catch (CSExcept except) {
        AfxMessageBox(
            _T("Exception: ") + 
            except.GetExString() + 
            _T("\nRefer to program builder"));
    }
}

//---------------------------------------------------------------------------
// ProcessVector processes picked documents by chosen IPP function
//---------------------------------------------------------------------------
void CDirector::ProcessVector()
{
   try {
      m_pRun->ProcessFunction();
      m_pRun->ProcessFunctionFinish();
      DEMO_APP->GetRecentFuncList()->AddFunc(m_pRun->GetFunc());
    } catch (CSExcept except) {
        AfxMessageBox(
            _T("Exception: ") + 
            except.GetExString() + 
            _T("\nRefer to program builder"));
    }
    m_pRun->PickRemarkDocs();
    m_pRun = NULL;
}

//---------------------------------------------------------------------------
// StopProcess cancels document processing
//---------------------------------------------------------------------------
void CDirector::StopProcess()
{
   if (!m_pRun) return;
   DEMO_APP->SetPickMode(FALSE);
   DEMO_APP->SetCursorArrow();
   MAIN_FRAME->SetMessageText(_T("Ready"));
   m_pRun->PickRemarkDocs();
   m_pRun = NULL;
}

//---------------------------------------------------------------------------
// PickNext starts to pick a document as the next IPP function parameter
// or starts IPP function call if all required function parameters have
// been picked out already
//---------------------------------------------------------------------------
void CDirector::PickNext()
{
   CMyString vectorName;
   UINT pickID, grabID;
   if (m_pRun->PickNext(vectorName, pickID, grabID)) {
      DEMO_APP->RegCursorPick(pickID);
      DEMO_APP->RegCursorGrab(grabID);
      MAIN_FRAME->RegMessagePick(
         _T("Pick over ") + vectorName + _T(" ") + m_pRun->VectorTerm() + _T(" for ") + m_pRun->GetFunc());
      MAIN_FRAME->RegMessageGrab(
         _T("Grab ") + vectorName + _T(" ") + m_pRun->VectorTerm() + _T(" for ") + m_pRun->GetFunc());

      DEMO_APP->SetPickMode(TRUE);
      DEMO_APP->SetCursorPick();
      MAIN_FRAME->SetMessagePick();
   } else {
      DEMO_APP->SetPickMode(FALSE);
      DEMO_APP->SetCursorArrow();
      MAIN_FRAME->SetMessageText(_T("Ready"));
      ProcessVector();
   }
}

//---------------------------------------------------------------------------
// GrabDoc sets chosen document as IPP function parameter and calls 
// PickNext
//---------------------------------------------------------------------------
void CDirector::GrabDoc(CDemoDoc* pDoc)
{
   m_pRun->GrabDoc(pDoc);
   PickNext();
}

//---------------------------------------------------------------------------
// PickVectors starts document picking process
//---------------------------------------------------------------------------
void CDirector::PickVectors()
{
   if (m_pRun->PickStart())
      PickNext();
}

//---------------------------------------------------------------------------
// NewProcess creates new CProcess class that provides classification for 
// all implemented IPP functions
//---------------------------------------------------------------------------
CProcess* CDirector::NewProcess() { return new CProcess;}

//---------------------------------------------------------------------------
// NewProcess creates new CProcess class that provides classification
// for IPP functions which can process specified vector 
//---------------------------------------------------------------------------
CProcess* CDirector::NewProcess(CVector* pVector) 
{
   CProcess* pProcess = NewProcess();
   if (!pProcess || !m_pProcess) return pProcess;
   *pProcess = *m_pProcess;
   pProcess->SetParentProcess(m_pProcess);
   pProcess->ApplyVectorNoSet(pVector);
   return pProcess;
}

//---------------------------------------------------------------------------
// DeleteProcess delets CProcess class created by NewProcess function
//---------------------------------------------------------------------------
void CDirector::DeleteProcess(CProcess*& pProcess) { 
   if (pProcess) delete pProcess;
   pProcess = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Arguments

//---------------------------------------------------------------------------
// GetFunc returns processing IPP function
//---------------------------------------------------------------------------
CFunc CDirector::GetFunc() const
{
   if (m_pRun) 
      return m_pRun->GetFunc();
   else
      return _T("");
}

//---------------------------------------------------------------------------
// GetFunctions sets to specified funcList all implemented IPP functions
// of IPP library with specified index
//---------------------------------------------------------------------------
void CDirector::GetFunctions(CFuncList& funcList, int idx) const
{
   funcList.RemoveAll();
   POSITION pos = GetHeadPosition(); 
   while (pos) {
      CRun *pRun = GetNext(pos);
      pRun->GetFunctions(funcList);
   }
   pos = funcList.GetHeadPosition();
   while (pos) {
      POSITION delPos = pos;
      CFunc func = funcList.GetNext(pos);
      if (!m_pProcess->ElementAt(idx).PresentBase(func.BaseName())) {
         funcList.RemoveAt(delPos);
      }
   }
}
