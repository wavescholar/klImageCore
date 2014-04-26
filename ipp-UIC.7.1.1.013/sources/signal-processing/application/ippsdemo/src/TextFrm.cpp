/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// TextFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "MainFrm.h"
#include "TextFrm.h"
#include "ippsTextView.h"
#include "ippsDemoDoc.h"
#include "ContextMenu.h"
#include "AfxPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextFrame

IMPLEMENT_DYNCREATE(CTextFrame, CDemoChildFrame)

CTextFrame::CTextFrame()
{
}

CTextFrame::~CTextFrame()
{
}


BEGIN_MESSAGE_MAP(CTextFrame, CDemoChildFrame)
   ON_MESSAGE(WM_SETMESSAGESTRING, OnSetString)
   //{{AFX_MSG_MAP(CTextFrame)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
//        Attributes

CippsTextView* CTextFrame::GetView() {
   return (CippsTextView*)GetActiveView();
}

//////////////////////////////////////
//        Operations

void CTextFrame::SetSize(int width, int height) {
   WINDOWPLACEMENT wPlace;
   GetWindowPlacement(&wPlace);
   LPRECT pRect = &(wPlace.rcNormalPosition);
   pRect->right  = pRect->left + width;
   if (height > 0) 
      pRect->bottom = pRect->top  + height;
   SetWindowPlacement(&wPlace);
}

/////////////////////////////////////////////////////////////////////////////
// CTextFrame message handlers

void CTextFrame::ActivateFrame(int nCmdShow) 
{
   CDemoDoc *pDoc = (CDemoDoc*)(GetActiveDocument());
   SetSize(256);

   CDemoChildFrame::ActivateFrame(nCmdShow);
   pDoc->SetStatusString();
}
