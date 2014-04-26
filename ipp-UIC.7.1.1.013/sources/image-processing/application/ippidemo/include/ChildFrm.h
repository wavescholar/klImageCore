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

#if !defined(AFX_CHILDFRM_H__6068AAEB_3C14_40AE_A7C0_13C616383D74__INCLUDED_)
#define AFX_CHILDFRM_H__6068AAEB_3C14_40AE_A7C0_13C616383D74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DemoChildFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CChildFrame class provides child window functionality for ippiDemo
// application
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CDemoChildFrame
{
   DECLARE_DYNCREATE(CChildFrame)
public:
   CChildFrame();

   // Set frame size to fit image size
   void SetImageRect() ;

// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CChildFrame)
   public:
   virtual void ActivateFrame(int nCmdShow = -1);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CChildFrame();
   virtual void MDIMaximize()
      { ASSERT(::IsWindow(m_hWnd)); }

protected:
   void InitBorderSize();
   RECT GetImageRect();
   void SetRect(RECT rect);

   int m_dx;
   int m_dy;

// Generated message map functions
   //{{AFX_MSG(CChildFrame)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__6068AAEB_3C14_40AE_A7C0_13C616383D74__INCLUDED_)
