/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DemoChildFrm.h : interface for the CDemoChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOCHILDFRM_H__6C434D00_8148_4A76_9051_23692965986F__INCLUDED_)
#define AFX_DEMOCHILDFRM_H__6C434D00_8148_4A76_9051_23692965986F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CDemoChildFrame class: base class for Demo child window class
/////////////////////////////////////////////////////////////////////////////

class CDemoChildFrame : public CMDIChildWnd
{
   DECLARE_DYNCREATE(CDemoChildFrame)
public:
   CDemoChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDemoChildFrame)
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void ActivateFrame(int nCmdShow = -1);
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CDemoChildFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:

   //{{AFX_MSG(CDemoChildFrame)
   afx_msg void OnSetFocus(CWnd* pOldWnd);
   afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
   afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
   //}}AFX_MSG
   LRESULT OnSetString(WPARAM wParam, LPARAM lParam);
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOCHILDFRM_H__6C434D00_8148_4A76_9051_23692965986F__INCLUDED_)
