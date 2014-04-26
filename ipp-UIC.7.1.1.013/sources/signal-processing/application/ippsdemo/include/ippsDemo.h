/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemo.h : main header file for the ippsDemo application
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSDEMO_H__25DD65ED_5261_44EC_8A62_53AC26C08281__INCLUDED_)
#define AFX_IPPSDEMO_H__25DD65ED_5261_44EC_8A62_53AC26C08281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include '..\stdafx.h' before including this file for PCH
#endif

#include "demo.h"

class CNewSignal;
class CNewTaps;
class CNewChar;

class CippsDemoDoc;
class CMruMenu;

enum {CPLX_CART, CPLX_POLAR};
enum {VIEW_DEMO, VIEW_TEXT, VIEW_CHAR};

// IPP library indexes
#define LIB_SP  0
#define LIB_CH  1
#define LIB_NUM 2

// CH_CALL macro produces ippCH library call
// (exported library or linked library)
#define CH_CALL(name, params) \
   (DEMO_APP->GetLibHandle(LIB_CH) ? \
      ((typ_##name)DEMO_APP->GetIppProc(#name, LIB_CH)) params : \
      name params)

/////////////////////////////////////////////////////////////////////////////
// CippsDemoApp: application class for IPP Signal Processing Demo
// See ippsDemo.cpp for the implementation of this class
//

class CippsDemoApp : public CDemoApp
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor

   CippsDemoApp();

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

   // GetCopyrightFirstYear returns the year of Demo creation
   virtual int GetCopyrightFirstYear() const { return 1999;}
   // GetDemoTitle returns the name of the Demo that will be set to the title
   // of application and application message boxes
   virtual CString GetDemoTitle() const;
   // GetNumLibs returns the number of used IPP libraries in the Demo. 
   virtual int GetNumLibs() const { return LIB_NUM;}
   // GetLibTitle returns the name of IPP Library with the specified index
   virtual CString GetLibTitle(int idx = 0) const;
   // GetLibPrefix returns the prefix in function names of IPP Library 
   // with the specified index
   virtual CString GetLibPrefix(int idx = 0) const;
   // GetLibVersion returns the version of IPP library with the specified index 
   virtual const IppLibraryVersion* GetLibVersion(int idx = 0) const;

   // LoadProfileSettings is called by CDemoApp::InitInstance.
   // It loads application settings
   virtual void LoadProfileSettings();
   // SaveProfileSettings is called by CDemoApp::InitInstance.
   // It saves application settings
   virtual void SaveProfileSettings();
   // CreateDirector creates new CDirector class that can process any document 
   // by any proper IPP function
   virtual CDirector* CreateDirector() const;
   // CreateDocTemplates creates document templates
   virtual void CreateDocTemplates();

/////////////////////////////////////////////////////////////////////////////
// Doc Managing & Creation 

   // GetNextDoc is used in iteration process for ippsdemo documents
   CippsDemoDoc* GetNextIppsDoc(MY_POSITION& pos) const { 
      return (CippsDemoDoc*)CDemoApp::GetNextDoc(pos);}
   // GetDemoTemplate returns template for document that will be shown in 
   // graphic view at its first appearance
   CDocTemplate* GetDemoTemplate();
   // GetTextTemplate returns template for document that will be shown in 
   // digital view at its first appearance
   CDocTemplate* GetTextTemplate();
   // GetCharTemplate returns template for document that will be shown in 
   // character view at its first appearance
   CDocTemplate* GetCharTemplate();
   // CreateDemoDoc creates document with specified parameters and
   // shows it in graphic view
   CippsDemoDoc* CreateDemoDoc(ppType type, int len, BOOL bMakeVisible = TRUE, CString title = "");
   CippsDemoDoc* CreateDemoDoc(CVector* pVec, BOOL bMakeVisible = TRUE, CString title = "");
   // CreateTextDoc creates document with specified parameters and
   // shows it in digital view
   CippsDemoDoc* CreateTextDoc(ppType type, int len, BOOL bMakeVisible = TRUE, CString title = "");
   CippsDemoDoc* CreateTextDoc(CVector* pVec, BOOL bMakeVisible = TRUE, CString title = "");
   // CreateCharDoc creates document with specified parameters and
   // shows it in character view
   CippsDemoDoc* CreateCharDoc(ppType type, int len, BOOL bMakeVisible = TRUE, CString title = "");
   CippsDemoDoc* CreateCharDoc(CVector* pVec, BOOL bMakeVisible = TRUE, CString title = "");

   // OpenNewDoc opens new user customized document.
   // If newView == VIEW_DEMO then document will be created with signal default
   // parameters and will be shown in graphic view;
   // if newView == VIEW_TEXT then document will be created with taps default
   // parameters and will be shown in digital view
   // if newView == VIEW_CHAR then document will be created with characher default
   // parameters and will be shown in character view
   void OpenNewDoc(BOOL bTaps = FALSE);
   // GetNewSignal returns class creating new document with signal data
   CNewSignal* GetNewSignal () const { return m_pNewSignal ;}
   // GetNewTaps returns class creating new document with taps data
   CNewTaps*   GetNewTaps   () const { return m_pNewTaps   ;}
   // GetNewChar returns class creating new document with character data
   CNewChar*   GetNewChar   () const { return m_pNewChar   ;}
   // NewView returns weather new document should be shown in signal, digital or character view
   int NewView()  const { return m_NewView;}
   // NewIsEmpty returns weather new document data doesn't need initialization
   BOOL   NewIsEmpty() const { return m_NewIsEmpty;}
   // GetNewType returns default vector type of new empty document
   ppType GetNewType   () const { return m_NewType   ;}
   // GetNewLength returns default vector length of new empty document
   int    GetNewLength () const { return m_NewLength ;}

/////////////////////////////////////////////////////////////////////////////
// Attributes

   // GetXAxis returns weather x axis should be shown in graphic vew
   BOOL   GetXAxis     () const { return m_XAxis     ;}
   // GetYAxis returns weather y axis should be shown in graphic vew
   BOOL   GetYAxis     () const { return m_YAxis     ;}
   // GetGrid returns weather grid should be shown in graphic vew
   BOOL   GetGrid      () const { return m_Grid      ;}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

   virtual CDemoDoc* CreateDropDoc(const char* pData, int DropObject);
   virtual BOOL ValidDropHeader(const char* pData, int DropObject) const;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippsDemoApp)
   //}}AFX_VIRTUAL


// Implementation
   //{{AFX_MSG(CippsDemoApp)
   afx_msg void OnFileNew();
   afx_msg void OnFileNewSignal();
   afx_msg void OnFileNewTaps();
   afx_msg void OnFileNewChar();

   afx_msg void OnUpdateViewXaxis(CCmdUI* pCmdUI);
   afx_msg void OnUpdateViewYaxis(CCmdUI* pCmdUI);
   afx_msg void OnViewXaxis();
   afx_msg void OnViewYaxis();
   afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
   afx_msg void OnViewGrid();

   afx_msg void OnUpdateZoomAll(CCmdUI* pCmdUI);
   afx_msg void OnZoomAll();

   afx_msg void OnOptColor();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

protected:
   CNewSignal* m_pNewSignal;
   CNewTaps*   m_pNewTaps;
   CNewChar*   m_pNewChar;
   BOOL   m_NewIsEmpty;
   int    m_NewView;
   ppType m_NewType;
   int    m_NewLength;
   BOOL m_XAxis;
   BOOL m_YAxis;
   BOOL m_Grid;

   CippsDemoDoc* CreateNewDoc(CDocTemplate* pTpl, ppType type, int len, 
                          BOOL bMakeVisible, CString title) ;
public:
};

// Macro DEMO_APP produces pointer to ipps Demo application
#undef DEMO_APP
#define DEMO_APP ((CippsDemoApp*)AfxGetApp())
#undef ACTIVE_DOC
#define ACTIVE_DOC (ACTIVE_FRAME ? (CippsDemoDoc*)(ACTIVE_FRAME->GetActiveDocument()) : NULL)

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPSDEMO_H__25DD65ED_5261_44EC_8A62_53AC26C08281__INCLUDED_)
