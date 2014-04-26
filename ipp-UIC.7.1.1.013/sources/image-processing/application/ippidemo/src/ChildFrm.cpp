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

#include "stdafx.h"
#include "ippiDemo.h"
#include "AfxPriv.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "ippiDemoDoc.h"
#include "ContextMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CDemoChildFrame)

BEGIN_MESSAGE_MAP(CChildFrame, CDemoChildFrame)
   //{{AFX_MSG_MAP(CChildFrame)
   //}}AFX_MSG_MAP
   ON_MESSAGE(WM_SETMESSAGESTRING, OnSetString)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
   m_dx = 0;
   m_dy = 0;
}

CChildFrame::~CChildFrame()
{
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
   InitBorderSize();
   RECT rect = GetImageRect();
   int mainWidth = MAIN_FRAME->GetClientWidth();
   int mainHeight = MAIN_FRAME->GetClientHeight();
   if (rect.right > mainWidth) {
      rect.right = mainWidth;
      rect.bottom = MIN(rect.bottom + ::GetSystemMetrics(SM_CYHSCROLL), mainHeight);
   }
   if (rect.bottom > mainHeight) {
      rect.bottom = mainHeight;
      rect.right = MIN(rect.right + ::GetSystemMetrics(SM_CXHSCROLL), mainWidth);
   }
   SetRect(rect);
   CDemoChildFrame::ActivateFrame(nCmdShow);
}

void CChildFrame::SetImageRect()
{
   SetRect(GetImageRect());
}

void CChildFrame::InitBorderSize()
{
   WINDOWPLACEMENT wPlace;
   LPRECT pwRect = &(wPlace.rcNormalPosition);
   RECT cRect;
   GetWindowPlacement(&wPlace);
   GetClientRect(&cRect);
   m_dx = (pwRect->right - pwRect->left) -
          (cRect.right - cRect.left) +
          2 * ::GetSystemMetrics(SM_CXEDGE);
   m_dy = (pwRect->bottom - pwRect->top) -
          (cRect.bottom - cRect.top) +
          2 * ::GetSystemMetrics(SM_CYEDGE);
}

RECT CChildFrame::GetImageRect()
{
   CippiDemoDoc *pDoc = (CippiDemoDoc*)(GetActiveDocument());
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   RECT wRect = wPlace.rcNormalPosition;
   wRect.right  = wRect.left + pDoc->Width() + m_dx;
   wRect.bottom = wRect.top + pDoc->Height() + m_dy;
   return wRect;
}

void CChildFrame::SetRect(RECT rect)
{
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   wPlace.rcNormalPosition = rect;
   SetWindowPlacement(&wPlace);
}

