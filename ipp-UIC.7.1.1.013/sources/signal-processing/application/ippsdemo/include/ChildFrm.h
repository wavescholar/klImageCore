/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ChildFrm.h : interface for the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__68811AF0_A30A_452C_B7CF_9C81E31A0303__INCLUDED_)
#define AFX_CHILDFRM_H__68811AF0_A30A_452C_B7CF_9C81E31A0303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DemoChildFrm.h"
class CippsDemoView;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame class provides child window functionality for ippsDemo 
// application
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CDemoChildFrame
{
   DECLARE_DYNCREATE(CChildFrame)
public:
   CChildFrame();

// Attributes
public:
   // GetView returns frame view
   CippsDemoView* GetView();
   // GetX, GetY, GetWidth, GetHeight, GetRect return frame window 
   // rectangle and its attributes
   int GetX();
   int GetY();
   int GetWidth();
   int GetHeight();
   void GetRect(CRect& rect) {
      rect.SetRect(GetX(), GetY(), GetX() + GetWidth(), GetY() + GetHeight());
   }
   // SetSize, SetRect update frame window rectangle due to specified values 
   void SetSize(int width, int height);
   void SetRect(int x, int y, int width, int height);
   void SetRect(RECT rect);
   // GetBorderWidth(GetBorderHeight) returns double width(height) of border
   // around client area 
   int GetBorderWidth();
   int GetBorderHeight();
   // GetStandardWidth(GetStandardHeight) returns standard width(height) of 
   // frame window
   int GetStandardWidth();
   int GetStandardHeight();

// Operations
public:
   // Zoom updates window rectangle according to specified rect or to scale
   // factors specified in view class
   void Zoom(RECT* rect = NULL);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CChildFrame)
   public:
   virtual void ActivateFrame(int nCmdShow = -1);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CChildFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
   //{{AFX_MSG(CChildFrame)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__68811AF0_A30A_452C_B7CF_9C81E31A0303__INCLUDED_)
