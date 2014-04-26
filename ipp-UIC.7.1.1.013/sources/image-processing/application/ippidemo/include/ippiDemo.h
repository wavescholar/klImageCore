/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDemo.h : main header file for the IPPIDEMO application
//

#if !defined(AFX_IPPIDEMO_H__0DFC7926_8C07_4304_810B_2D90929E91F0__INCLUDED_)
#define AFX_IPPIDEMO_H__0DFC7926_8C07_4304_810B_2D90929E91F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include '..\stdafx.h' before including this file for PCH
#endif


#include <ipps.h>
#include "resource.h"       // main symbols
#include "demo.h"
#include "Image.h"

// IPP library indexes
#define LIB_I   0
#define LIB_CC  1
#define LIB_CV  2
#define LIB_NUM 3

#define LIB_J   111

// J_CALL macro produces ippJP library call
// (exported library or linked library)
#define J_CALL(name, params) name params

// CC_CALL macro produces ippCC library call
// (exported library or linked library)
#define CC_CALL(name, params) name params

// CV_CALL macro produces ippCV library call
// (exported library or linked library)
#define CV_CALL(name, params) name params

// Size of new destination image
//    NEWSIZE_WHOLE - equal to whole source image size
//    NEWSIZE_ROI   - equal to source image ROI size
enum {NEWSIZE_WHOLE, NEWSIZE_ROI};

// Pattern or color of new destination image
enum {SET_JAEHNE, SET_RAMP, SET_BLACK, SET_WHITE, SET_GRAY,
      SET_BLUE, SET_GREEN, SET_RED, SET_NUM};

class CippiDemoDoc;

/////////////////////////////////////////////////////////////////////////////
// CippiDemoApp: application class for ippi Demo
// See ippiDemo.cpp for the implementation of this class
//
class CippiDemoApp : public CDemoApp
{
public:
/////////////////////////////////////////////////////////////////////////////
// Constructor

   CippiDemoApp();

/////////////////////////////////////////////////////////////////////////////
// CDemoApp virtual function overrides

   // GetCopyrightFirstYear returns the year of Demo creation
   virtual int GetCopyrightFirstYear() const { return 1997;}
   // GetDemoTitle returns the name of the Demo that will be set to the title
   // of application and application message boxes
   virtual CString GetDemoTitle() const { return "ippIP";}
   // GetNumLibs returns the number of used IPP libraries in the Demo.
   virtual int GetNumLibs() const { return LIB_NUM;}
   // GetLibTitle returns the name of IPP Library with the specified index
   virtual CString GetLibTitle(int idx = 0) const;
   // GetLibPrefix returns the prefix in function names of IPP Library
   // with the specified index
   virtual CString GetLibPrefix(int idx = 0) const;
   // GetLibVersion returns the version of IPP library with the specified index
   virtual const IppLibraryVersion* GetLibVersion(int idx = 0) const;

   // InitInstance is called at the beginning of MFC application.
   // Besides of DemoApp initialization it initialize movie settings
   virtual BOOL InitInstance();
   // DispatchCommands is called by CDemoApp::InitInstance after
   // ParseCommandLine
   virtual void DispatchCommands(CCommandLineInfo& cmdInfo);
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

   // CreateDoc creates new image document according to specified image header
   CippiDemoDoc* CreateDoc(ImgHeader header, CString title = "", BOOL bMakeVisible = FALSE);

/////////////////////////////////////////////////////////////////////////////
// Attributes

   // NewHeader defines default header for new image document
   ImgHeader GetNewHeader() const      { return m_NewHeader;}
   void SetNewHeader(ImgHeader header) { m_NewHeader = header;}
   // NewData defines default pattern for new image document
   int GetNewPattern() const { return m_NewPattern;}
   void SetNewPattern(int mode) { m_NewPattern = mode;}
   // NewSize defines size mode for new destination image document
   int GetNewSize() const { return m_NewSize;}
   void SetNewSize(int mode) { m_NewSize = mode;}

   // GetContourShow returns the show contours mode
   BOOL GetContourShow() const { return m_ContourShow;}

/////////////////////////////////////////////////////////////////////////////
// Providing Drag & Drop Operations on Demo applications

   virtual CDemoDoc* CreateDropDoc(const char* pData, int DropObject);
   virtual BOOL ValidDropHeader(const char* pData, int DropObject) const;

/////////////////////////////////////////////////////////////////////////////
// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippiDemoApp)
   public:
   //}}AFX_VIRTUAL

protected:
/////////////////////////////////////////////////////////////////////////////
// Implementation

   //{{AFX_MSG(CippiDemoApp)
   afx_msg void OnFileNew();
   afx_msg void OnFileNewDlg();
   afx_msg void OnContourShow();
   afx_msg void OnUpdateContourShow(CCmdUI* pCmdUI);
   afx_msg void OnOptDst();
   afx_msg void OnUpdateOptLib(CCmdUI* pCmdUI);
   afx_msg void OnUpdateMovieArg(CCmdUI* pCmdUI);
   afx_msg void OnUpdateMovieFastArg(CCmdUI* pCmdUI);
   afx_msg void OnMovieArg();
   afx_msg void OnMovieFastArg();
   afx_msg void OnUpdateMovieZoom(CCmdUI* pCmdUI);
   afx_msg void OnMovieZoom();
   afx_msg void OnUpdateMovieWander(CCmdUI* pCmdUI);
   afx_msg void OnMovieWander();
   afx_msg void OnMovieMask();
   afx_msg void OnOptMovie();
   afx_msg void OnOptBorder();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   ImgHeader m_NewHeader;
   int       m_NewPattern;
   int       m_NewSize;
   BOOL      m_ContourShow;

protected:
    virtual void OnLibraryChange();
};

/////////////////////////////////////////////////////////////////////////////

// Macro DEMO_APP produces pointer to ippiDemo application
#undef DEMO_APP
#define DEMO_APP ((CippiDemoApp*)AfxGetApp())
#ifdef ACTIVE_DOC
#undef ACTIVE_DOC
#define ACTIVE_DOC (ACTIVE_FRAME ? (CippiDemoDoc*)(ACTIVE_FRAME->GetActiveDocument()) : NULL)
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPIDEMO_H__0DFC7926_8C07_4304_810B_2D90929E91F0__INCLUDED_)
