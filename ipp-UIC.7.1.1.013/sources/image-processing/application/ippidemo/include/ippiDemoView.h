/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippiDemoView.h : interface for the CippiDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPIDEMOVIEW_H__8C69009A_7B46_406A_8640_886513403456__INCLUDED_)
#define AFX_IPPIDEMOVIEW_H__8C69009A_7B46_406A_8640_886513403456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CippiDemoDoc;
class CDemoDib;

/////////////////////////////////////////////////////////////////////////////
// CippiDemoView class provides image view functionality for ippiDemo
// application
/////////////////////////////////////////////////////////////////////////////

class CippiDemoView : public CScrollView
{
protected: // create from serialization only
   CippiDemoView();
   DECLARE_DYNCREATE(CippiDemoView)

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
   friend CippiDemoDoc;
   CippiDemoDoc* GetDoc();

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
   // CancelDraw cancels user drawing
   void CancelDraw();

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippiDemoView)
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnInitialUpdate(); // called first time after construct
   afx_msg LRESULT OnMovieMessage(WPARAM wParam, LPARAM lParam);
   //{{AFX_VIRTUAL(CippiDemoView)
   //}}AFX_VIRTUAL

/////////////////////////////////////////////////////////////////////////////
// Implementation
public:
   virtual ~CippiDemoView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   BOOL      m_FirstDraw;
   CDC       m_SrcDC;
   CDemoDib* m_pDib;
   CString   m_ClassName;
   CPoint    m_Points[4];
   int       m_NumPoints;
   BOOL      m_Dragging;
   BOOL      m_Clipping;

   // StartDraw starts user drawing
   void StartDraw();
   // ClipCursor cips cursor while user line drawing
   void ClipCursor(BOOL clip = TRUE) ;

   CPen* m_newPen   ;
   CPen* m_oldPen   ;
   CBrush* m_newBrush   ;
   CBrush* m_oldBrush   ;
   int   m_oldBkMode;
   int   m_oldROP2  ;

/////////////////////////////////////////////////////////////////////////////
// Draw implementation

   BOOL BeginDraw(CDC* pDC, COLORREF color, int& drawTool);
   void EndDraw(CDC* pDC);
   void DrawRoi(CDC* pDC, COLORREF color);
   void DrawCenterShift(CDC* pDC, COLORREF color);
   void DrawQuad(CDC* pDC, COLORREF color);
   void DrawParl(CDC* pDC, COLORREF color);
   void DrawCircle(CDC* pDC, CPoint point);
   void DrawPointer(CDC* pDC, CPoint p0, CPoint p1);
   void DrawMarker(CDC* pDC);
   void DrawCrossPoints(CDC* pDC, POINT center[], COLORREF color[], int numC);
   void DrawCrossBorder(CDC* pDC, POINT point, COLORREF color);
   void DrawLine(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF color);
   int  GetReverseHeight();
   void ReversePoint(CPoint& point);
   void PointToDoc(CPoint& point);
   void PointFromDoc(CPoint& point);
   void PointsToDoc(CPoint* points, int number);
   void PointsFromDoc(CPoint* points, int number);
   BOOL GetRoiFromDoc(CPoint points[2]);
   void SetRoiToDoc(const CPoint points[2]);
   BOOL GetCenterShiftFromDoc(CPoint points[2]);
   void SetCenterShiftToDoc(const CPoint points[2]);
   BOOL GetQuadFromDoc(CPoint* points, int& num);
   void SetQuadToDoc(const CPoint* points, int num);
   BOOL GetParlFromDoc(CPoint* points, int& num);
   void SetParlToDoc(const CPoint* points, int num);

/////////////////////////////////////////////////////////////////////////////
//  Command processing implementation

   // Down{Roi/Quad/Parl/Center} performs LeftButtonDown command while
   // user is drawing corresponding contour
   void DownRoi   (CPoint point);
   void DownQuad  (CPoint point);
   void DownParl  (CPoint point);
   void DownCenter(CPoint point);
   // Move{Roi/Quad/Parl/Center} performs MouseMove command while
   // user is drawing corresponding contour
   void MoveRoi   (CPoint point);
   void MoveQuad  (CPoint point);
   void MoveParl  (CPoint point);
   void MoveCenter(CPoint point);
   // Up{Roi/Quad/Parl/Center} performs LeftButtonUp command while
   // user is drawing corresponding contour
   BOOL UpRoi   (CPoint point);
   BOOL UpQuad  (CPoint point);
   BOOL UpParl  (CPoint point);
   BOOL UpCenter(CPoint point);

/////////////////////////////////////////////////////////////////////////////
// Generated message map functions
protected:
   //{{AFX_MSG(CippiDemoView)
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DemoView.cpp
inline CippiDemoDoc* CippiDemoView::GetDoc()
   { return (CippiDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPIDEMOVIEW_H__8C69009A_7B46_406A_8640_886513403456__INCLUDED_)
