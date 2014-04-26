/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ippsDemoView.h"
#include "ippsDemoDoc.h"
#include "ContextMenu.h"
#include "AfxPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CDemoChildFrame)

BEGIN_MESSAGE_MAP(CChildFrame, CDemoChildFrame)
   ON_MESSAGE(WM_SETMESSAGESTRING, OnSetString)
   //{{AFX_MSG_MAP(CChildFrame)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
   // TODO: add member initialization code here
   
}

CChildFrame::~CChildFrame()
{
}

//////////////////////////////////////
//        Attributes

CippsDemoView* CChildFrame::GetView() {
   return (CippsDemoView*)GetActiveView();
}

int CChildFrame::GetX() {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   return wPlace.rcNormalPosition.left;
}

int CChildFrame::GetY() {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   return wPlace.rcNormalPosition.top;
}

int CChildFrame::GetWidth() {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   LPRECT pRect = &(wPlace.rcNormalPosition);
   return pRect->right - pRect->left;
}

int CChildFrame::GetHeight() {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   LPRECT pRect = &(wPlace.rcNormalPosition);
   return pRect->bottom - pRect->top;
}

void CChildFrame::SetSize(int width, int height) {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   LPRECT pRect = &(wPlace.rcNormalPosition);
   pRect->right  = pRect->left + width;
   pRect->bottom = pRect->top  + height;
   SetWindowPlacement(&wPlace);
}

void CChildFrame::SetRect(RECT rect)
{
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   wPlace.rcNormalPosition = rect;
   SetWindowPlacement(&wPlace);
}

void CChildFrame::SetRect(int x, int y, int width, int height) {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   LPRECT pRect = &(wPlace.rcNormalPosition);
   pRect->left  = x;
   pRect->top   = y;
   pRect->right  = pRect->left + width;
   pRect->bottom = pRect->top  + height;
   SetWindowPlacement(&wPlace);
}

int CChildFrame::GetBorderWidth() {
   return (::GetSystemMetrics(SM_CXEDGE) 
      +    ::GetSystemMetrics(SM_CXSIZEFRAME)) * 2;
}

int CChildFrame::GetBorderHeight() {
   return (::GetSystemMetrics(SM_CYEDGE) 
      +    ::GetSystemMetrics(SM_CYSIZEFRAME)) * 2
      +    ::GetSystemMetrics(SM_CYCAPTION);
}

int CChildFrame::GetStandardWidth() {
   return  GetX() + GetView()->GetInitialWidth()  + GetBorderWidth();
}

int CChildFrame::GetStandardHeight() {
   return GetY() + GetView()->GetInitialHeight() + GetBorderHeight();
}

//////////////////////////////////////
//        Operations

static void updateOffset(int& x, int& w, int m)
{
   if (x + w <= m) return;
   x = m - w;
   if (x >= 0) return;
   x = 0;
   w = m;
}

static BOOL ScrollFrame(int& frameX, int& frameW, int frameMax)
{
   if (frameX + frameW > frameMax) {
      frameX = frameMax - frameW;
      if (frameX < 0) {
         frameX = 0;
         frameW = frameMax;
         return TRUE;
      }
   }
   return FALSE;
}

static int GetScrollBarWidth() { 
   return ::GetSystemMetrics(SM_CXHSCROLL);
}

static int GetScrollBarHeight() { 
   return ::GetSystemMetrics(SM_CYHSCROLL);
}

void CChildFrame::Zoom(RECT* pRect)
{
   if (pRect) {
      SetRect(*pRect);
   } else {
      int viewW = GetView()->GetScaleWidth();
      int viewH = GetView()->GetScaleHeight();
      if (viewW < 128) viewW = 128;
      int frameX = GetX();
      int frameY = GetY();
      int frameW = viewW + GetBorderWidth();
      int frameH = viewH + GetBorderHeight();
//      if (GetView()->ScrollH()) frameW -= GetScrollBarWidth();
//      if (GetView()->ScrollW()) frameH -= GetScrollBarHeight();

      BOOL bScrollW = ScrollFrame(frameX,frameW,MAIN_FRAME->GetClientWidth());
      if (bScrollW)
         frameH += GetScrollBarHeight() + 1;
      BOOL bScrollH = ScrollFrame(frameY,frameH,MAIN_FRAME->GetClientHeight());
      if (bScrollH && !bScrollW) {
         frameW += GetScrollBarWidth() + 1;
         ScrollFrame(frameX,frameW,MAIN_FRAME->GetClientWidth());
      }

      SetRect(frameX,frameY,frameW,frameH);
   } 
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
   CDemoChildFrame::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
   CDemoChildFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::ActivateFrame(int nCmdShow) 
{
   SetSize(GetView()->GetInitialWidth() + GetBorderWidth(),
           GetView()->GetInitialHeight() + GetBorderHeight());

   CDemoChildFrame::ActivateFrame(nCmdShow);
   CDemoDoc *pDoc = (CDemoDoc*)(GetActiveDocument());
   pDoc->SetStatusString();
}
