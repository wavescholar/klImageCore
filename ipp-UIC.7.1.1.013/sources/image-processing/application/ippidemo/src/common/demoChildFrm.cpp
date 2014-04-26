/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DemoChildFrm.cpp : implementation of the CDemoChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
#include "DemoDoc.h"

#include "DemoChildFrm.h"
#include "MainFrm.h"
#include "ContextMenu.h"
#include "DragDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoChildFrame construction/destruction

CDemoChildFrame::CDemoChildFrame()
{
   
}

CDemoChildFrame::~CDemoChildFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDemoChildFrame

IMPLEMENT_DYNCREATE(CDemoChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CDemoChildFrame, CMDIChildWnd)
   //{{AFX_MSG_MAP(CDemoChildFrame)
   ON_WM_SETFOCUS()
   ON_WM_NCRBUTTONDOWN()
   ON_WM_MDIACTIVATE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
//        Attributes

//////////////////////////////////////
//        Operations

/////////////////////////////////////////////////////////////////////////////
// CDemoChildFrame message handlers


BOOL CDemoChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   if( !CMDIChildWnd::PreCreateWindow(cs) )
      return FALSE;
   cs.style &= ~(LONG)FWS_ADDTOTITLE;

   return TRUE;
}

void CDemoChildFrame::ActivateFrame(int nCmdShow) 
{
   CMDIChildWnd::ActivateFrame(nCmdShow);
   CDemoDoc *pDoc = (CDemoDoc*)(GetActiveDocument());
   pDoc->SetStatusString();
}

BOOL CDemoChildFrame::PreTranslateMessage(MSG* pMsg)
{
   if (pMsg->message == WM_NCRBUTTONDOWN) {
      return FALSE;
   }
   if (pMsg->message == WM_RBUTTONDOWN) {
      return FALSE;
   }
   return CMDIChildWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoChildFrame message handlers

void CDemoChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
   CMDIChildWnd::OnSetFocus(pOldWnd);
   ((CDemoDoc*)GetActiveDocument())->SetStatusString();
}

LRESULT CDemoChildFrame::OnSetString(WPARAM wParam, LPARAM lParam)
{
    LRESULT result = CFrameWnd::OnSetMessageString(wParam, lParam);
    CDemoDoc* pDoc = (CDemoDoc*)GetActiveDocument();
    if (!pDoc) return result;
    CWnd* pMessageBar = GetMessageBar();
    CString message;
    pMessageBar->GetWindowText(message);
    if (message == _T("Ready"))
        pDoc->SetStatusString();
    return result;
}

void CDemoChildFrame::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
   if ((nHitTest == HTCAPTION) && DEMO_APP->GetDropSource())
      DEMO_APP->GetDropSource()->DoDragDrop((CDemoDoc*)GetActiveDocument(), point);
   else
      CFrameWnd::OnNcRButtonDown(nHitTest, point);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoChildFrame diagnostics

#ifdef _DEBUG
void CDemoChildFrame::AssertValid() const
{
   CMDIChildWnd::AssertValid();
}

void CDemoChildFrame::Dump(CDumpContext& dc) const
{
   CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

void CDemoChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
   CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
   
   // TODO: Add your message handler code here
   
}
