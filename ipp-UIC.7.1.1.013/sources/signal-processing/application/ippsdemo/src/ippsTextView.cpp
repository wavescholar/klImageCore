/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsTextView.cpp : implementation of CippsTextView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "MainFrm.h"
#include "ippsDemoDoc.h"
#include "ippsTextView.h"
#include "ContextMenu.h"
#include "Director.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CippsTextView

IMPLEMENT_DYNCREATE(CippsTextView, CEditView)

CippsTextView::CippsTextView()
{
}

CippsTextView::~CippsTextView()
{
}


BEGIN_MESSAGE_MAP(CippsTextView, CEditView)
   //{{AFX_MSG_MAP(CippsTextView)
   ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
   ON_WM_RBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_SETCURSOR()
   ON_WM_DESTROY()
   ON_WM_LBUTTONDOWN()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippsTextView drawing

void CippsTextView::OnDraw(CDC* pDC)
{
   CippsDemoDoc* pDoc = GetDoc();
}

/////////////////////////////////////////////////////////////////////////////
// CippsTextView diagnostics

#ifdef _DEBUG
void CippsTextView::AssertValid() const
{
   CEditView::AssertValid();
}

void CippsTextView::Dump(CDumpContext& dc) const
{
   CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CippsTextView message handlers

BOOL CippsTextView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CEditView::PreCreateWindow(cs);
}

void CippsTextView::OnInitialUpdate() 
{
   CEditView::OnInitialUpdate();
   GetDoc()->UpdateFrameTitle();
   SetMyFont();
   UpdateText();
}

void CippsTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   UpdateText();
}

void CippsTextView::UpdateText()
{
   CString breakStr = "* * * * *\r\n";
   int maxLength = 10000;
   CippsDemoDoc* pDoc = GetDoc();
   CMyString text;
   int i;
   for (i=0; i<pDoc->Length() && i < maxLength; i++) {
      CMyString sNumber;
      pDoc->Get(i,sNumber, "%s\t%s");
      text += sNumber + "\r\n";
   }
   if (i < pDoc->Length()) {
      text += breakStr;
      GetEditCtrl().SetReadOnly(TRUE);
   } else {
      GetEditCtrl().SetReadOnly(FALSE);
   }
   SetWindowText(text); 
}

void CippsTextView::OnChange() 
{
   CippsDemoDoc* pDoc = GetDoc();
   CString text;
   GetWindowText(text);
   pDoc->Set(text);
   pDoc->UpdateView(this);
}

void CippsTextView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   UINT ident = (DEMO_APP->GetPickMode()) ? IDR_TEXT_P : IDR_TEXT;
   ClientToScreen(&point);
   CContextMenu cMenu;
   cMenu.TrackPopup(ident, point);
}

void CippsTextView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->GetPickMode() && pDoc->MayBePicked()) {
   } else {
      CEditView::OnLButtonDown(nFlags, point);
   }
}

void CippsTextView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->GetPickMode() && pDoc->MayBePicked()) {
      DEMO_APP->GetDirector()->GrabDoc(pDoc);
   } else {
      CEditView::OnLButtonUp(nFlags, point);
   }
}

BOOL CippsTextView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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

void CippsTextView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
   // TODO: Add your specialized code here and/or call the base class
   
   CEditView::OnPrepareDC(pDC, pInfo);
}

void CippsTextView::SetMyFont() 
{
   LOGFONT lf;
   lf.lfHeight = 16; 
   lf.lfWidth = 0; 
   lf.lfEscapement = 0; 
   lf.lfOrientation = 0; 
   lf.lfWeight = FW_NORMAL; 
   lf.lfItalic = 0; 
   lf.lfUnderline = 0; 
   lf.lfStrikeOut = 0; 
   lf.lfCharSet = ANSI_CHARSET; 
   lf.lfOutPrecision = OUT_TT_PRECIS; 
   lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
   lf.lfQuality = PROOF_QUALITY; 
   lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN; 
#if _MSC_VER >= 1400 && _MSC_FULL_VER != 140040310
   strcpy_s(lf.lfFaceName,"Courier New");
#else
   strcpy(lf.lfFaceName,"Courier New");
#endif

   m_Font.CreateFontIndirect(&lf);
   GetEditCtrl().SetFont(&m_Font, FALSE);  
}
