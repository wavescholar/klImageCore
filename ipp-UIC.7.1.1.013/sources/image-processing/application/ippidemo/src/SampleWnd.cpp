/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// SampleWnd.cpp: implementation of the CSampleWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "DemoDib.h"
#include "SampleWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSampleWnd::CSampleWnd()
{
    m_bDraw = TRUE;
    m_pImage = NULL;
   m_xOffset = 0;
   m_yOffset = 0;
   m_width = 0;
   m_height = 0;
}

CSampleWnd::~CSampleWnd()
{
}


BEGIN_MESSAGE_MAP(CSampleWnd, CWnd)
   //{{AFX_MSG_MAP(CSampleWnd)
   ON_WM_PAINT()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleWnd message handlers

void CSampleWnd::Clear()
{
   CRect rect;
   GetWindowRect(&rect);
   CBrush brush(::GetSysColor(COLOR_3DFACE));
   CPaintDC dc(this);
   dc.FillRect(rect,&brush);
}

void CSampleWnd:: OnPaint( )
{
   ASSERT (m_pImage);

   CDemoDib demoDib;
   demoDib.SetBmpInfo(m_pImage, TRUE);
   BITMAPINFO* pBmpInfo = demoDib.GetBmpInfo();

   if (!m_bDraw || !pBmpInfo) {
      CWnd::OnPaint();
      return;
   }
   if (SetNewRect(m_pImage)) Clear();

   CPaintDC dc(this);
   CBitmap bitmap;
   bitmap.CreateCompatibleBitmap(&dc, m_pImage->Width(), m_pImage->Height());
   dc.SelectObject(&bitmap);
   ::StretchDIBits(dc.m_hDC,
      m_xOffset, m_yOffset, m_width, m_height,
      0, 0, m_pImage->Width(),  m_pImage->Height(),
      m_pImage->GetData(), pBmpInfo,
      DIB_RGB_COLORS, SRCCOPY);
}

BOOL CSampleWnd::SetNewRect(CImage* pImage)
{
   CRect rect;
   GetWindowRect(&rect);
   int widthSave = m_width;
   int heightSave = m_height;
   double scaleW = (double)rect.Width() / (double)pImage->Width();
   double scaleH = (double)rect.Height() / (double)pImage->Height();
   if (scaleW < scaleH) {
      m_width = rect.Width();
      m_height = (int)(pImage->Height() * scaleW);
      m_xOffset = 0;
      m_yOffset = (rect.Height() - m_height) >> 1;
   } else {
      m_height = rect.Height();
      m_width = (int)(pImage->Width() * scaleH);
      m_yOffset = 0;
      m_xOffset = (rect.Width() - m_width) >> 1;
   }
   return
      widthSave != m_width ||
      heightSave != m_height;
}

