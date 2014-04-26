/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

// CharFrm.cpp : implementation of the CCharFrame class
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsCharView.h"
#include "ippsDemoDoc.h"

#include "CharFrm.h"
#include "MainFrm.h"
#include "ContextMenu.h"
#include "DragDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharFrame construction/destruction

CCharFrame::CCharFrame()
{
   
}

CCharFrame::~CCharFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCharFrame

IMPLEMENT_DYNCREATE(CCharFrame, CDemoChildFrame)

BEGIN_MESSAGE_MAP(CCharFrame, CDemoChildFrame)
   //{{AFX_MSG_MAP(CCharFrame)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
//        Attributes

CippsCharView* CCharFrame::GetView() {
   return (CippsCharView*)GetActiveView();
}

//////////////////////////////////////
//        Operations

void CCharFrame::SetSize(int width, int height) {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   LPRECT pRect = &(wPlace.rcNormalPosition);
   if (width > 0)
      pRect->right  = pRect->left + width;
   if (height > 0) 
      pRect->bottom = pRect->top  + height;
   SetWindowPlacement(&wPlace);
}

/////////////////////////////////////////////////////////////////////////////
// CCharFrame message handlers

void CCharFrame::ActivateFrame(int nCmdShow) 
{
   CDemoDoc *pDoc = (CDemoDoc*)(GetActiveDocument());
   SetSize(-1, 200);

   CDemoChildFrame::ActivateFrame(nCmdShow);
   pDoc->SetStatusString();
}

/////////////////////////////////////////////////////////////////////////////
// CCharFrame diagnostics

#ifdef _DEBUG
void CCharFrame::AssertValid() const
{
   CDemoChildFrame::AssertValid();
}

void CCharFrame::Dump(CDumpContext& dc) const
{
   CDemoChildFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCharFrame message handlers
