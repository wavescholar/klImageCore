/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// SymbolWnd.h: interface for the CSymbolWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLWND_H__58E16FC0_713A_495A_8CAF_D433F8AF55BE__INCLUDED_)
#define AFX_SYMBOLWND_H__58E16FC0_713A_495A_8CAF_D433F8AF55BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd window

class CSymbolWnd : public CStatic
{
// Construction
public:
   CSymbolWnd();

// Attributes
public:

// Operations
public:
   void UpdateCode(int code);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CSymbolWnd)
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CSymbolWnd();
protected:
   CRect  IndexToRect(CPoint index) const;
   CPoint PointToIndex(CPoint point) const;
   int    IndexToCode(CPoint index) const; 
   CPoint CodeToIndex(int code) const; 
   void DrawSymbol(CPaintDC& dc, CPoint index) ;

   // Generated message map functions
protected:
   CSize  m_symbSize;
   CPoint m_curIndex;
   BOOL   m_bPick;
   void UpdateCurPoint(CPoint point);
   void UpdateCurIndex(CPoint index);

   //{{AFX_MSG(CSymbolWnd)
   afx_msg void OnPaint();
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLWND_H__58E16FC0_713A_495A_8CAF_D433F8AF55BE__INCLUDED_)
