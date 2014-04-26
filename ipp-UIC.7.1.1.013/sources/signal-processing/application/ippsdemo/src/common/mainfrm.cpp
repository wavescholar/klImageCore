/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// MainFrm.cpp : implementation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"

#include "MainFrm.h"
#include "DemoDoc.h"
#include "DragDrop.h"
#include "Timing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
   //{{AFX_MSG_MAP(CMainFrame)
   ON_WM_CREATE()
   ON_WM_TIMER()
   //}}AFX_MSG_MAP
   ON_WM_SETCURSOR()
END_MESSAGE_MAP()

static UINT indicators[] =
{
   ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_CAPS,
   ID_INDICATOR_NUM,
   ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
   m_pDropTarget = new CDropTarget;
}

CMainFrame::~CMainFrame()
{
   delete m_pDropTarget;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
      return -1;
   
   if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
      | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
   {
      TRACE0("Failed to create toolbar\n");
      return -1;      // fail to create
   }

   if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
   {
      TRACE0("Failed to create status bar\n");
      return -1;      // fail to create
   }

   // TODO: Delete these three lines if you don't want the toolbar to
   //  be dockable
   m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
   EnableDocking(CBRS_ALIGN_ANY);
   DockControlBar(&m_wndToolBar);

   m_pDropTarget->Register(this);

   return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   if( !CMDIFrameWnd::PreCreateWindow(cs) )
      return FALSE;
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return TRUE;
}

int CMainFrame::GetClientWidth() const
{
   RECT rect; 
   ::GetClientRect(m_hWndMDIClient, &rect); 
   return rect.right;
   
}

int CMainFrame::GetClientHeight() const
{
   RECT rect; 
   ::GetClientRect(m_hWndMDIClient, &rect); 
   return rect.bottom;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
   CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
   CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


LRESULT CMainFrame::OnSetString(WPARAM wParam, LPARAM lParam)
{
   LRESULT result = CFrameWnd::OnSetMessageString(wParam, lParam);
   CDemoDoc* pDoc = (ACTIVE_FRAME ? (CDemoDoc*)(ACTIVE_FRAME->GetActiveDocument()) : NULL);
   if (!pDoc) return result;
   CWnd* pMessageBar = GetMessageBar();
   CString message;
   pMessageBar->GetWindowText(message);
   if (message == _T("Ready")) {
      if (DEMO_APP->GetPickMode()) {
         SetMessagePick();
      } else {
         pDoc->SetStatusString();
      }
   }
   return result;
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
   if (DEMO_APP->GetPickMode()) {
      DEMO_APP->SetCursorPick();
      SetMessagePick();
      return TRUE;
   }
   return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

