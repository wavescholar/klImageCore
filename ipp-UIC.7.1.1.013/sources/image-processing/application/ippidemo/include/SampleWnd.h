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

#if !defined(AFX_SAMPLEWND_H__1580382C_6FA0_4255_BD53_1B06E151E1FB__INCLUDED_)
#define AFX_SAMPLEWND_H__1580382C_6FA0_4255_BD53_1B06E151E1FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

class CSampleWnd : public CWnd
{
// Construction
public:
   CSampleWnd();
   virtual ~CSampleWnd();

// Attributes
public:
   void SetImage(CImage* pImage) { m_pImage = pImage;}

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


   // Generated message map functions
protected:
   CImage* m_pImage;
   BOOL m_bDraw;

   int m_xOffset;
   int m_yOffset;
   int m_width;
   int m_height;

   void Clear();
   BOOL SetNewRect(CImage* pImage);
   //{{AFX_MSG(CSampleWnd)
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SAMPLEWND_H__1580382C_6FA0_4255_BD53_1B06E151E1FB__INCLUDED_)
