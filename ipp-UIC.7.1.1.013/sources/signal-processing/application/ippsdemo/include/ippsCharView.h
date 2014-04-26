/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsCharView.h : interface of the CippsCharView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSCHARVIEW_H__89753DAC_8DC5_44A5_B5E3_66ED1938ACA4__INCLUDED_)
#define AFX_IPPSCHARVIEW_H__89753DAC_8DC5_44A5_B5E3_66ED1938ACA4__INCLUDED_

#pragma once

class CippsDemoDoc;
class CDropTarget;

class CippsCharView : public CEditView
{
protected: // create from serialization only
   CippsCharView();
   DECLARE_DYNCREATE(CippsCharView)

// Attributes
public:
#ifndef _DEBUG
   CippsDemoDoc* GetDoc() const
   { return (CippsDemoDoc*)(CEditView::GetDocument());}
#endif

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippsCharView)
   public:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnInitialUpdate();
   virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
   protected:
   virtual void OnDraw(CDC* pDC);      // overridden to draw this view
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CippsCharView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
   CippsDemoDoc* GetDoc() const;
#endif

protected:
   CFont m_Font;
   CDropTarget* m_pDropTarget;

   void UpdateText();
   void UpdateSel();
   void SaveSel();

// Generated message map functions
protected:
   //{{AFX_MSG(CippsCharView)
   afx_msg void OnChange();
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPSCHARVIEW_H__89753DAC_8DC5_44A5_B5E3_66ED1938ACA4__INCLUDED_)
