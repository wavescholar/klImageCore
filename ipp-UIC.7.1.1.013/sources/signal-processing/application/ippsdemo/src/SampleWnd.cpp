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
#include "ippsDemo.h"
#include "ippsDemoView.h"
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

void CSampleWnd::EmptyRect(CRect rect) 
{
    CBrush brush(::GetSysColor(COLOR_3DFACE));
    CPaintDC dc(this);
    dc.FillRect(rect,&brush);
}

void CSampleWnd:: OnPaint( ) 
{
    if (!m_bDraw) {
        CWnd::OnPaint();
        return;
    }

   CRect rect;
   GetWindowRect(&rect);
   POINT origin = rect.TopLeft();   
   ::ScreenToClient(m_hWnd, &origin);
   rect = CRect(origin, rect.Size());
   CBrush brushAxis (m_Color[CLR_AXIS_BK] );
   CBrush brushGraph(m_Color[CLR_GRAPH_BK]);
   CPen   penAxis (PS_SOLID, 1, m_Color[CLR_AXIS] );
   CPen   penGraph(PS_SOLID, 1, m_Color[CLR_GRAPH]);
   CPaintDC dc(this);

   dc.SetBkMode(TRANSPARENT);
   dc.SetROP2  (R2_COPYPEN);
   dc.SelectObject(CippsDemoView::GetFont());
   dc.FillRect(&rect,&brushAxis);
   rect.left += CippsDemoView::GetSpaceWidth();
   rect.top += CippsDemoView::GetSpaceHeight();
   dc.FillRect(&rect,&brushGraph);

   int length = rect.Width();
   int magn = (int)(rect.Height()*0.5);
   CPoint* points = new CPoint[length];
   Ipp16s* src = new Ipp16s[length];
   ippsVectorJaehne_16s(src,length,(Ipp16s)magn);
   for (int i=0; i<length; i++) {
      points[i].x = i + rect.left;
      points[i].y = magn + src[i] + rect.top;
   }
   dc.SelectObject(&penGraph);
   dc.Polyline(points,length);

   int x0 = rect.left;
   int x1 = x0 + (int)(length*.4);
   int x2 = x0 + (int)(length*.8);
   int x3 = rect.right - 1;
   int y0 = rect.top;
   int y1 = y0 + (int)(magn*.3);
   int y2 = y0 + (int)(magn);
   int y3 = y0 + (int)(magn*1.7);
   int y4 = rect.bottom - 1;

   CippsDemoView::DrawLine(&dc, x0, y0, x3, y0, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x0, y1, x3, y1, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x0, y2, x3, y2, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x0, y3, x3, y3, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x0, y4, x3, y4, m_Color[CLR_AXIS]);

   CippsDemoView::DrawLine(&dc, x0, y0, x0, y4, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x1, y0, x1, y4, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x2, y0, x2, y4, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLine(&dc, x3, y0, x3, y4, m_Color[CLR_AXIS]);

   CippsDemoView::DrawLabelX(&dc, x1, y0, 100, CippsDemoView::SIDE_TOP, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLabelX(&dc, x2, y0, 200, CippsDemoView::SIDE_TOP, m_Color[CLR_AXIS]);

   CippsDemoView::DrawLabelY(&dc, x0, y1, 2000, CippsDemoView::SIDE_LEFT, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLabelY(&dc, x0, y2,    0, CippsDemoView::SIDE_LEFT, m_Color[CLR_AXIS]);
   CippsDemoView::DrawLabelY(&dc, x0, y3,-2000, CippsDemoView::SIDE_LEFT, m_Color[CLR_AXIS]);
}
