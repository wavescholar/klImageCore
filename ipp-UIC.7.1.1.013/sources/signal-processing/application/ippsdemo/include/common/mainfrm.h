/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MainFrm.h : interface for the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D23CAA00_A9B2_403F_A069_071FF072E8BF__INCLUDED_)
#define AFX_MAINFRM_H__D23CAA00_A9B2_403F_A069_071FF072E8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRun;
class CDropTarget;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame class provides main window functionality for all Demo 
// applications
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CMDIFrameWnd
{
   DECLARE_DYNAMIC(CMainFrame)
public:
   CMainFrame();

// Attributes
public:
   // GetClientWidth returns main window client area width
   int GetClientWidth() const;
   // GetClientHeight returns main window client area height
   int GetClientHeight() const;
   // RegMessagePick register message to be shown in status string 
   // while document is picking as IPP function argument.   
   void RegMessagePick(CString message) { m_MessagePick = message;}
   // SetMessagePick shows registered message in status string
   void SetMessagePick() { SetMessageText(m_MessagePick);}
   // RegMessageGrab register message to be shown in status string 
   // when document below cursor may be is grabbed as IPP function argument   
   void RegMessageGrab(CString message) { m_MessageGrab = message;}
   // SetMessageGrab shows registered message in status string
   void SetMessageGrab() { SetMessageText(m_MessageGrab);}


// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMainFrame)
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CMainFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
   CStatusBar  m_wndStatusBar;
   CToolBar    m_wndToolBar;

   CString m_MessagePick;
   CString m_MessageGrab;

   CDropTarget* m_pDropTarget;

// Generated message map functions
protected:
   //{{AFX_MSG(CMainFrame)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   //}}AFX_MSG
   afx_msg LRESULT OnSetString(WPARAM wParam, LPARAM lParam);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D23CAA00_A9B2_403F_A069_071FF072E8BF__INCLUDED_)
