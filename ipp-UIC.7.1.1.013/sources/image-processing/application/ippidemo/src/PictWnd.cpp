/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// PictWnd.cpp: implementation of the CPictWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippiDemo.h"
#include "PictWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPictWnd::CPictWnd()
{
   m_Len = 0;
   m_X = NULL;
   m_Y = NULL;
   m_Color = RGB(0,0,0);
}

CPictWnd::~CPictWnd()
{
}


BEGIN_MESSAGE_MAP(CPictWnd, CWnd)
   //{{AFX_MSG_MAP(CPictWnd)
   ON_WM_PAINT()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictWnd message handlers

void CPictWnd::EmptyRect(CRect rect)
{
    CBrush brush(::GetSysColor(COLOR_3DFACE));
    CPaintDC dc(this);
    dc.FillRect(rect,&brush);
}

void CPictWnd:: OnPaint( )
{
   if (!m_Len || !m_X || !m_Y) {
        CWnd::OnPaint();
        return;
    }

   CRect rect;
   GetWindowRect(&rect);

   POINT origin = rect.TopLeft();   
   ::ScreenToClient(m_hWnd, &origin);
   rect = CRect(origin, rect.Size());

   CPoint* points = new CPoint[m_Len];
   for (int i=0; i<m_Len; i++) {
      points[i].x = m_X[i];
      points[i].y = rect.bottom - m_Y[i] - 1;
   }

   CBrush brush (RGB(255,255,255));
   CPen   pen(PS_SOLID, 1, m_Color);
   CPaintDC dc(this);

   dc.SetBkMode(TRANSPARENT);
   dc.SetROP2  (R2_COPYPEN);
   dc.FillRect(&rect,&brush);
   dc.SelectObject(&pen);
   dc.Polyline(points,m_Len);

   delete(points);
}
