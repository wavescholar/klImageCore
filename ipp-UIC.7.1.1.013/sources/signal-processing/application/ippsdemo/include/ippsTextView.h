/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsTextView.h : interface for the CippsTextView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTVIEW_H__5938A229_C6B3_447F_8D75_040D23FB6D54__INCLUDED_)
#define AFX_TEXTVIEW_H__5938A229_C6B3_447F_8D75_040D23FB6D54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CippsTextView view

class CippsDemoDoc;

class CippsTextView : public CEditView
{
protected:
   CippsTextView();           // protected constructor used by dynamic creation
   DECLARE_DYNCREATE(CippsTextView)

// Attributes
public:
   CippsDemoDoc* GetDoc()
   { return (CippsDemoDoc*)(CEditView::GetDocument());}

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippsTextView)
   public:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnInitialUpdate();
   virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
   protected:
   virtual void OnDraw(CDC* pDC);      // overridden to draw this view
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   //}}AFX_VIRTUAL

// Implementation
protected:
   CFont m_Font;

   void UpdateText();
   void SetMyFont();
   virtual ~CippsTextView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
protected:
   //{{AFX_MSG(CippsTextView)
   afx_msg void OnChange();
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTVIEW_H__5938A229_C6B3_447F_8D75_040D23FB6D54__INCLUDED_)
