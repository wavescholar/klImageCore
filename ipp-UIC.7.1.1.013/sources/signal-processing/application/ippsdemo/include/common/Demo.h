/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Demo.h : main header file for the all DEMO application
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO_H__0DFC7926_8C07_4304_810B_2D90929E91F0__INCLUDED_)
#define AFX_DEMO_H__0DFC7926_8C07_4304_810B_2D90929E91F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include '..\stdafx.h' before including this file for PCH
#endif

#include <ippcore.h>
#include "resource.h"
#include "SExcept.h"
#include "Func.h"

#include "ipp_header.h"
#include "ipp_type.h"

// CALL macro produces IPP library call 
// (exported library or linked library)
#define CALL(name, params) \
   (DEMO_APP->GetLibHandle() ? \
      ((typ_##name)DEMO_APP->GetIppProc(_T(#name))) params : \
      name params) 

// Useful arithmetic macros
#define MAX(r1, r2) ((r1) > (r2) ? (r1) : (r2))
#define MIN(r1, r2) ((r1) < (r2) ? (r1) : (r2))
#define ABS(r) ((r) > 0 ? (r) : -(r))

class CDirector;
class CDemoDoc;
class CDropSource;
class CLib;
class CAboutDlg;

/////////////////////////////////////////////////////////////////////////////
// MY_POSITION: class for easy operating on the list of MFC documents
// produced by several templates
class MY_POSITION {
public:
   POSITION tpl;
   POSITION doc;
   operator bool() { return tpl != NULL;}
};

/////////////////////////////////////////////////////////////////////////////
// CDemoApp: base class for ippiDemo / ippsDemo / ippchDemo MFC applications
//

class CDemoApp : public CWinApp
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor / destructor

   CDemoApp();
   ~CDemoApp();

/////////////////////////////////////////////////////////////////////////////
// General Demo Operations

   // InitInstance is called at the beginning of MFC application.
   // Besides of standard MFC initialization it loads IPP libraries and
   // creates the list of IPP functions that will be called
   virtual BOOL InitInstance();
   // ExitInstance is called at the end of MFC application 
   virtual int ExitInstance();
   // LoadProfileSettings is called by CDemoApp::InitInstance.
   // It loads application settings
   virtual void LoadProfileSettings();
   // SaveProfileSettings is called by CDemoApp::InitInstance.
   // It saves application settings
   virtual void SaveProfileSettings();
   // DispatchCommands is called by CDemoApp::InitInstance after
   // ParseCommandLine
   virtual void DispatchCommands(CCommandLineInfo& cmdInfo) {}
   // InitLibrary is called by the CDemoApp::InitInstance.
   // It loads demonstrated IPP libraries
   virtual void InitLibrary();
   // InitFunctions is called by the CDemoApp::InitInstance.
   // It creates the list of demonstrated IPP functions
   virtual void InitFunctions();
   // CreateDirector creates new CDirector class that can process any document 
   // by any proper IPP function
   virtual CDirector* CreateDirector() const;
   // SetCurrentFunction sets IPP function that will be highlighted at the 
   // beginning of Process dialog. Usually it is the last called IPP function.
   void SetCurrentFunction();

/////////////////////////////////////////////////////////////////////////////
// Doc Managing Operations & Attributes

   // CreateDocTemplates creates document templates
   virtual void CreateDocTemplates();
   // GetDocCount returns number of all opened MFC documents
   int GetDocCount() const;
   // GetFirstDocPosition returns the first document position to begin
   // the iteration process
   MY_POSITION GetFirstDocPosition() const;
   // GetNextDoc returns pointer to the current document in the iteration 
   // process and sets the next doc position to the pos
   CDemoDoc* GetNextDoc(MY_POSITION& pos) const;
   // FindDoc returns pointer to the document with the specified title
   CDemoDoc* FindDoc(CString title) const;
   // UpdateAllViews updates all views of all opened MFC documents
   void UpdateAllViews();

/////////////////////////////////////////////////////////////////////////////
// Message Boxes

   // MessageBox displays message box using CDemoApp::GetDemoTitle() fot title
   int MessageBox(LPCTSTR lpszText, UINT nType=MB_OK, LPCTSTR title=NULL)const;
   // ErrorMessage displays message box for error information
   BOOL ErrorMessage(CString info) const;

/////////////////////////////////////////////////////////////////////////////
// Virtual attributes

   // GetVersionString returns current version of Demo appllication
   virtual CString GetVersionString() const;
   // GetCopyrightFirstYear returns the year of Demo creation
   virtual int GetCopyrightFirstYear() const { return 1999;}
   // GetCopyrightLastYear returns the current year
   virtual int GetCopyrightLastYear() const;
   // GetDemoTitle returns the name of the Demo that will be set to the title
   // of application and application message boxes
   virtual CString GetDemoTitle() const { return _T("ipp?");}
   // GetNumLibs returns the number of used IPP libraries in the Demo. 
   virtual int GetNumLibs() const { return 1;}
   // GetLibTitle returns the name of IPP Library with the specified index
   //  (for example "ippIP", "ippCC", "ippCV" for ippi Demo)
   virtual CString GetLibTitle(int index = 0) const { return _T("ipp?");}
   // GetLibPrefix returns the prefix in function names of IPP Library 
   // with the specified index (for example "ippi")
   virtual CString GetLibPrefix(int index = 0) const { return _T("ipp?");}
   // GetLibVersion returns the version of IPP library with the specified index 
   virtual const IppLibraryVersion* GetLibVersion(int index = 0) const { return NULL;}

/////////////////////////////////////////////////////////////////////////////
// Other Attributes

   // GetLib returns pointer to class that serves IPP library with 
   // the specified index 
   CLib* GetLib(int index = 0) const;
   // GetLibHandle returns handle of exported IPP DLL with the specified 
   // index; it returns NULL if no library is exported (linked library is used 
   // in this case)
   HINSTANCE GetLibHandle(int index = 0) const;
   // GetIppProc returns address of the specified exported IPP DLL function
   // with specified library index; it returns NULL if no library is exported
   // (common function call should be used in this case) 
   FARPROC   GetIppProc(LPCTSTR name, int index = 0) const;
   // GetIppList returns pointer to IPP function list 
   CFuncList* GetIppList() const { return m_pIppList;}
   // GetRecentFuncList returns pointer to recently called IPP function list
   CRecentFuncList* GetRecentFuncList() const { return m_pRecentFuncList;}
   // GetDirector returns pointer to processing manager; CDirector class can 
   // process any document by any proper IPP function
   CDirector* GetDirector() const { return m_pDirector;}
   // ForCpu64 returns TRUE if Demo has been built for Win64 architecture
   BOOL ForCpu64() const;

/////////////////////////////////////////////////////////////////////////////
// Pick & Grab Document Attributes

   // PickDst mode allows user to pick destination documents while processing;
   // in other case all destination documents are created automatically
   void SetPickDst(BOOL mode) { m_PickDst = mode;}
   BOOL GetPickDst() const { return m_PickDst;}
   // Pick mode indicates if user is picking source or destination documents
   // at the moment or not
   void SetPickMode(BOOL mode) { m_PickMode = mode;}
   BOOL GetPickMode() const { return m_PickMode;}

/////////////////////////////////////////////////////////////////////////////
// Cursor Managing

   void RegCursorPick(UINT id);
   void RegCursorGrab(UINT id); 
   void SetCursorPick() const {::SetCursor(m_hPick);}
   void SetCursorGrab() const {::SetCursor(m_hGrab);}
   void SetCursorCross() const { ::SetCursor(m_hCross);}
   void SetCursorArrow() const { ::SetCursor(m_hArrow);}
   void SetCursorWait () const { ::SetCursor(m_hWait );}
   BOOL IsCursorWait () const { return ::GetCursor() == m_hWait;}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

   CDropSource* GetDropSource() { return m_pDropSource;}
   UINT GetDropFormat() const { return m_DropFormat;}
   virtual CDemoDoc* CreateDropDoc(const char* pData, int DropObject) { return NULL;}
   virtual BOOL ValidDropHeader(const char* pData, int DropObject) const { return TRUE;}
   virtual CString GetDropFormatString() const;

protected:
   CLib*      m_pLib;
   CFuncList* m_pIppList;
   CRecentFuncList* m_pRecentFuncList;
   CDirector* m_pDirector;

   int m_OptLibPage;
   BOOL m_PickMode;
   BOOL m_PickDst;

   HCURSOR   m_hCross;
   HCURSOR   m_hArrow;
   HCURSOR   m_hWait;
   HCURSOR   m_hPick;
   HCURSOR   m_hGrab;

   CDropSource* m_pDropSource;
   UINT m_DropFormat;

   int m_CurrProcess;
   int m_CurrBook;
   int m_CurrChapter;
   CFunc  m_CurrFunc;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDemoApp)
   public:
   virtual BOOL OnIdle(LONG lCount);
   //}}AFX_VIRTUAL

// Implementation
   //{{AFX_MSG(CDemoApp)
   afx_msg void OnAppAbout();
   afx_msg void OnFileOpen();
   afx_msg void OnUpdateOptLib(CCmdUI* pCmdUI);
   afx_msg void OnOptLib();
   afx_msg void OnUpdateOptOutstand(CCmdUI* pCmdUI);
   afx_msg void OnOptOutstand();
   afx_msg void OnOptTiming();
   afx_msg void OnUpdatePickDst(CCmdUI* pCmdUI);
   afx_msg void OnPickDst();
   afx_msg void OnUpdateProcessStop(CCmdUI* pCmdUI);
   afx_msg void OnProcessStop();
   afx_msg void OnWindowCloseall();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

// Macro DEMO_APP produces pointer to Demo application
#define DEMO_APP ((CDemoApp*)AfxGetApp())
// Macro MAIN_FRAME produces pointer to main application window
#define MAIN_FRAME ((CMainFrame*)AfxGetMainWnd())
// Macro ACTIVE_FRAME produces pointer to active document frame window
#define ACTIVE_FRAME (MAIN_FRAME ? (CMDIChildWnd*)(MAIN_FRAME->MDIGetActive()) : NULL)
// Macro ACTIVE_FRAME produces pointer to active document
#define ACTIVE_DOC (ACTIVE_FRAME ? (CDemoDoc*)(ACTIVE_FRAME->GetActiveDocument()) : NULL)


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO_H__0DFC7926_8C07_4304_810B_2D90929E91F0__INCLUDED_)
