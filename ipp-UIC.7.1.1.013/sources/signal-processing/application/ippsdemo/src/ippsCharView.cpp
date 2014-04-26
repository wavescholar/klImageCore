
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsCharView.cpp : implementation of the CippsCharView class
//

#include "stdafx.h"
#include "ippsDemo.h"

#include "ippsDemoDoc.h"
#include "ippsCharView.h"
#include "Director.h"
#include "MainFrm.h"
#include "ContextMenu.h"
#include "DragDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CippsCharView

IMPLEMENT_DYNCREATE(CippsCharView, CEditView)

BEGIN_MESSAGE_MAP(CippsCharView, CEditView)
   //{{AFX_MSG_MAP(CippsCharView)
   ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
   ON_WM_LBUTTONUP()
   ON_WM_SETCURSOR()
   ON_WM_KEYUP()
   ON_WM_CREATE()
   ON_WM_LBUTTONDOWN()
   ON_WM_DESTROY()
   ON_WM_RBUTTONDOWN()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippsCharView construction/destruction

CippsCharView::CippsCharView()
{
   m_pDropTarget = new CDropTarget;
}

CippsCharView::~CippsCharView()
{
   delete m_pDropTarget;
}

BOOL CippsCharView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   BOOL bPreCreated = CEditView::PreCreateWindow(cs);
   cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL); // Enable word-wrapping

   return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CippsCharView drawing

void CippsCharView::OnDraw(CDC* pDC)
{
   CippsDemoDoc* pDoc = GetDoc();
   ASSERT_VALID(pDoc);
   // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CippsCharView diagnostics

#ifdef _DEBUG
void CippsCharView::AssertValid() const
{
   CEditView::AssertValid();
}

void CippsCharView::Dump(CDumpContext& dc) const
{
   CEditView::Dump(dc);
}

CippsDemoDoc* CippsCharView::GetDoc() const // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CippsDemoDoc)));
   return (CippsDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CippsCharView message handlers

void CippsCharView::OnInitialUpdate() 
{
   CEditView::OnInitialUpdate();
   GetDoc()->UpdateFrameTitle();
   UpdateText();
   UpdateSel();
}

void CippsCharView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   UpdateText();
   UpdateSel();
}

void CippsCharView::UpdateText()
{
   int maxLength = 10000;
   CippsDemoDoc* pDoc = GetDoc();
   CString text;
   pDoc->GetString(text,maxLength);
   SetWindowText(text); 
}

void CippsCharView::UpdateSel()
{
   CippsDemoDoc* pDoc = GetDoc();
   if (pDoc->IsVoi()) {
      int offset, length;
      pDoc->GetVoi(offset,length);
      GetEditCtrl().SetSel(offset,offset + length);
   } else {
      GetEditCtrl().SetSel(pDoc->GetPos(), pDoc->GetPos());
   }
}

void CippsCharView::SaveSel()
{
   CippsDemoDoc* pDoc = GetDoc();
   int start, end;
   GetEditCtrl().GetSel(start,end);
   pDoc->SetVoi(start,end - start);
}

void CippsCharView::OnChange() 
{
   CippsDemoDoc* pDoc = GetDoc();
   CString text;
   GetWindowText(text);
   pDoc->SetString(text);
   pDoc->UpdateView(this);
}

void CippsCharView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->GetPickMode() && pDoc->MayBePicked()) {
   } else {
      CEditView::OnLButtonDown(nFlags, point);
   }
}

void CippsCharView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->GetPickMode() && pDoc->MayBePicked()) {
      DEMO_APP->GetDirector()->GrabDoc(pDoc);
   } else {
      CEditView::OnLButtonUp(nFlags, point);
      SaveSel();
   }
}

BOOL CippsCharView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->IsCursorWait()) return TRUE;
   if (DEMO_APP->GetPickMode()) {
      if (pDoc->MayBePicked()) {
         DEMO_APP->SetCursorGrab();
         MAIN_FRAME->SetMessageGrab();
         return TRUE;
      } else {
         DEMO_APP->SetCursorPick();
         MAIN_FRAME->SetMessagePick();
         return TRUE;
      }
   } else {
      return CEditView::OnSetCursor(pWnd, nHitTest, message);
   }
}

void CippsCharView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
   // TODO: Add your specialized code here and/or call the base class
   
   CEditView::OnPrepareDC(pDC, pInfo);
}

void CippsCharView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   CEditView::OnKeyUp(nChar, nRepCnt, nFlags);
   SaveSel();
}

int CippsCharView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
   if (CEditView::OnCreate(lpCreateStruct) == -1)
      return -1;
   
   return 0;
}

void CippsCharView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   UINT ident = (DEMO_APP->GetPickMode()) ? IDR_CHAR_P : IDR_CHAR;
   ::ClientToScreen(m_hWnd, (LPPOINT)&point);
   CContextMenu cMenu;
   cMenu.TrackPopup(ident, point);
}
