/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// TextFrame.h : interface for the CTextFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTFRM_H__67F45FB7_6D2F_4C84_A886_8E4CA2438E5B__INCLUDED_)
#define AFX_TEXTFRM_H__67F45FB7_6D2F_4C84_A886_8E4CA2438E5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextFrm.h : header file
//

#include "DemoChildFrm.h"
class CippsTextView;

/////////////////////////////////////////////////////////////////////////////
// CTextFrame frame

class CTextFrame : public CDemoChildFrame
{
   DECLARE_DYNCREATE(CTextFrame)
protected:
   CTextFrame();           // protected constructor used by dynamic creation

// Attributes
public:
   CippsTextView* GetView();

// Operations
public:
   void SetSize(int width, int height = 0);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTextFrame)
   public:
   virtual void ActivateFrame(int nCmdShow = -1);
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual ~CTextFrame();

   // Generated message map functions
   //{{AFX_MSG(CTextFrame)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTFRM_H__67F45FB7_6D2F_4C84_A886_8E4CA2438E5B__INCLUDED_)
