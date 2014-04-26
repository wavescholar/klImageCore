/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1997-2012 Intel Corporation. All Rights Reserved.
//
*/

// PictWnd.h: interface for the CPictWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTWND_H__16712F81_8F30_4E62_AE79_E063F55B43CE__INCLUDED_)
#define AFX_PICTWND_H__16712F81_8F30_4E62_AE79_E063F55B43CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPictWnd : public CWnd {
// Construction
public:
   CPictWnd();

// Attributes
public:
   int  m_Len;
   int* m_X;
   int* m_Y;
   COLORREF m_Color;

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
   virtual ~CPictWnd();


   // Generated message map functions
protected:
   void EmptyRect(CRect rect);
   //{{AFX_MSG(CSampleWnd)
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PICTWND_H__16712F81_8F30_4E62_AE79_E063F55B43CE__INCLUDED_)
