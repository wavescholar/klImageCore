
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// SymbolWnd.cpp : implementation of CSymbolWnd class
//

#include "stdafx.h"
#include "ippsDemo.h"
#include "SymbolWnd.h"
#include "SymbolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd

CSymbolWnd::CSymbolWnd()
{
   m_symbSize.cx = 1;
   m_symbSize.cy = 1;
   m_curIndex.x = -1;
   m_curIndex.y = -1;
}

CSymbolWnd::~CSymbolWnd()
{
}


BEGIN_MESSAGE_MAP(CSymbolWnd, CStatic)
   //{{AFX_MSG_MAP(CSymbolWnd)
   ON_WM_PAINT()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd implementation

CRect CSymbolWnd::IndexToRect(CPoint index)  const
{
   CPoint point(index.x*m_symbSize.cx, index.y*m_symbSize.cy);
   return CRect(point, m_symbSize);
}

CPoint CSymbolWnd::PointToIndex(CPoint point) const 
{
   int x = (int)((point.x - 1)/m_symbSize.cx);
   int y = (int)((point.y - 1)/m_symbSize.cy);
   if (x < 0 || x > 0xF || y < 0 || y > 0xF)
      x = y = -1;
   return CPoint(x,y);
}

int CSymbolWnd::IndexToCode(CPoint index) const 
{
   return index.x + (index.y << 4);
}

CPoint CSymbolWnd::CodeToIndex(int code) const 
{
   return CPoint(code & 0xF, code >> 4);
}

void CSymbolWnd:: DrawSymbol(CPaintDC& dc, CPoint index) 
{
   CRect rect = IndexToRect(index);
   CString str((char)IndexToCode(index));
   COLORREF brushColor;
   COLORREF textColor;
   if (IndexToCode(index) == 0) {
      brushColor = ::GetSysColor(COLOR_3DFACE);
      textColor  = ::GetSysColor(COLOR_3DFACE);
   } else if (index == m_curIndex) {
      brushColor = RGB(128,128,128);
      textColor  = RGB(255,255,255);
   } else {
      brushColor = RGB(255,255,255);
      textColor  = RGB(0,0,0);
   }
   CBrush brush (brushColor);
   dc.FillRect(&rect,&brush);
   dc.SetTextColor(textColor);
   dc.DrawText(str,rect,
      DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

void CSymbolWnd::UpdateCurPoint(CPoint point) 
{
   CPoint newIndex = PointToIndex(point);
   if (newIndex.x >= 0) {
      UpdateCurIndex(newIndex);
      CSymbolDlg* pDlg = (CSymbolDlg*)GetParent();
      pDlg->UpdateCode(IndexToCode(m_curIndex));
   }
}

void CSymbolWnd::UpdateCurIndex(CPoint newIndex) 
{
   if (m_curIndex == newIndex) return;
   CPoint oldIndex = m_curIndex;
   m_curIndex = newIndex;
   InvalidateRect(IndexToRect(oldIndex));
   UpdateWindow();
   InvalidateRect(IndexToRect(newIndex));
   UpdateWindow();
}

void CSymbolWnd::UpdateCode(int code) 
{
   UpdateCurIndex(CodeToIndex(code));
}

/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd message handlers

void CSymbolWnd:: OnPaint( ) 
{
   CRect rect;
   GetWindowRect(&rect);
   m_symbSize.cx = rect.Width() >>4;
   m_symbSize.cy = rect.Height()>>4;


   CPaintDC dc(this);
   dc.SetBkMode(TRANSPARENT);

   CPoint index;
   for (index.y = 0; index.y < 16; index.y++) 
      for (index.x = 0; index.x < 16; index.x++)
         DrawSymbol(dc, index);
}


void CSymbolWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
   CStatic::OnMouseMove(nFlags, point);
   if (nFlags != 1) return;
   UpdateCurPoint(point);
}

void CSymbolWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
   CStatic::OnLButtonDown(nFlags, point);
   UpdateCurPoint(point);
}

void CSymbolWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
   // TODO: Add your message handler code here and/or call default
   
   CStatic::OnLButtonUp(nFlags, point);
   CSymbolDlg* pDlg = (CSymbolDlg*)GetParent();
   pDlg->UpdateString();
}
