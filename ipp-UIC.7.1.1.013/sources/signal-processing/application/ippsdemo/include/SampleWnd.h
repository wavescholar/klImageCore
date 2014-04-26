/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// SampleWnd.h: interface for the CSampleWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEWND_H__14C47E43_991E_4B25_94C4_C7BE25464114__INCLUDED_)
#define AFX_SAMPLEWND_H__14C47E43_991E_4B25_94C4_C7BE25464114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleWnd : public CWnd
{
// Construction
public:
   CSampleWnd();

// Attributes
public:
   enum {CLR_GRAPH, CLR_GRAPH_BK, CLR_AXIS, CLR_AXIS_BK, CLR_NUM};
   void SetColor(COLORREF clr[]) { for (int i=0; i<CLR_NUM; i++) m_Color[i] = clr[i];}

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CSampleWnd)
   protected:
    afx_msg void OnPaint( );

   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CSampleWnd();


   // Generated message map functions
protected:
   COLORREF m_Color[CLR_NUM];
   BOOL m_bDraw;
   void EmptyRect(CRect rect);
   //{{AFX_MSG(CSampleWnd)
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SAMPLEWND_H__14C47E43_991E_4B25_94C4_C7BE25464114__INCLUDED_)
