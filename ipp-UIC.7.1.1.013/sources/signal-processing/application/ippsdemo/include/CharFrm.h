/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
*/

// CharFrm.h : interface of the CCharFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARFRM_H__6C434D00_8148_4A76_9051_23692965986F__INCLUDED_)
#define AFX_CHARFRM_H__6C434D00_8148_4A76_9051_23692965986F__INCLUDED_

#pragma once

#include "DemoChildFrm.h"
class CippsCharView;

class CCharFrame : public CDemoChildFrame
{
   DECLARE_DYNCREATE(CCharFrame)
public:
   CCharFrame();

// Attributes
public:
   CippsCharView* GetView();

// Operations
public:
   void SetSize(int width, int height = 0);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCharFrame)
   virtual void ActivateFrame(int nCmdShow = -1);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CCharFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:

   //{{AFX_MSG(CCharFrame)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARFRM_H__6C434D00_8148_4A76_9051_23692965986F__INCLUDED_)
