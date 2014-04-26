/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemoView.h : interface for the CippsDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOVIEW_H__E8B98F95_D5F6_4323_9F56_78970A67DDB4__INCLUDED_)
#define AFX_DEMOVIEW_H__E8B98F95_D5F6_4323_9F56_78970A67DDB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CippsDemoDoc;

class CippsDemoView : public CScrollView
{
protected: // create from serialization only
   CippsDemoView();
   DECLARE_DYNCREATE(CippsDemoView)

// Attributes
public:
   enum {SIDE_LEFT = -2, SIDE_TOP = -1, SIDE_NONE = 0, SIDE_BOTTOM = 1, SIDE_RIGHT = 2};
   CippsDemoDoc* GetDoc()
   { return (CippsDemoDoc*)(CScrollView::GetDocument());}
   int GetClientWidth();
   int GetClientHeight();
   int GetInitialWidth();
   int GetInitialHeight();
   int GetBorderWidth();
   int GetBorderHeight();
   int GetScaleWidth(double factor = -1);
   int GetScaleHeight(double factor = -1);
   CSize GetScaleSize();
   double GetAmplitude();
   double GetAmplitude(double factor);
   BOOL ScrollW();
   BOOL ScrollH();

   static CFont* GetFont() { return &m_Font;}
   static COLORREF GetColorSignal() { return m_colorSignal;}
   static void SetColorSignal(COLORREF value) { m_colorSignal = value;}
   static COLORREF GetColorAxis() { return m_colorAxis;}
   static void SetColorAxis(COLORREF value) { m_colorAxis = value;}
   static COLORREF GetColorSignalBack() { return m_colorSignalBack;}
   static void SetColorSignalBack(COLORREF value) { m_colorSignalBack = value;}
   static COLORREF GetColorAxisBack() { return m_colorAxisBack;}
   static void SetColorAxisBack(COLORREF value) { m_colorAxisBack = value;}
   static int GetSpaceHeight();
   static int GetSpaceCplxHeight();
   static int GetSpaceWidth();

// Operations
public:
   void Zoom();

   static void DrawLabelX(CDC* pDC, int x, int y, int val, int sideText, 
                 COLORREF clr = (COLORREF)-1);
   static void DrawLabelY(CDC* pDC, int x, int y, double val, int sideText, 
                 COLORREF clr, BOOL bIntVal = FALSE);
   static void DrawLine(CDC* pDC, int x0, int y0, int x1, int y1, 
                 COLORREF clr = (COLORREF)-1);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CippsDemoView)
   public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   protected:
   virtual void OnInitialUpdate(); // called first time after construct
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CippsDemoView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   BOOL m_IsUpdated;
   CDC m_SrcDC;
   CBitmap* m_pBitmap;
   POINT* m_points;
   POINT* m_pointsIm;
   int    m_numPoints;
   CPen   m_Pen;
   static CFont    m_Font;
   static COLORREF m_colorSignal;
   static COLORREF m_colorAxis;
   static COLORREF m_colorSignalBack;
   static COLORREF m_colorAxisBack;

   void SetPointsComplex();
   void SetPointsReal();
   void SelectPenSignal(CDC* pDC);
   void SelectPenAxis(CDC* pDC);
   void DrawDC(CDC* pDC);
   void DrawReal(CDC* pDC);
   void DrawComplex(CDC* pDC);
   void DrawAxisX(CDC* pDC, int y0, BOOL bTopNumbers = TRUE);
   void DrawAxisY(CDC* pDC, int y0);
   void DrawGridLine(CDC* pDC, int x0, int y0, int x1, int y1);
   void DrawFrame(CDC* pDC, int zero);
   void DrawBackReal(CDC* pDC);
   void DrawBackComplex(CDC* pDC);
   void DrawBackAxis(CDC* pDC);
   void DrawBackSignal(CDC* pDC, int zero);
   void DrawBackground(CDC* pDC);
   void DrawLabelY(CDC* pDC, int x, int y, double val, int sideText);

   void AllocatePoints();
   void GetMinMax(double& min, double& max);
   double GetStepAxis(double minStep, double magn = 0);
   int GetMinPixel();
   int GetMaxPixel();
   int GetX0();
   int GetX1();
   int GetZero();
   int GetZeroIm();
   int GetY0(int zero);
   int GetY1(int zero);

// Generated message map functions
protected:
   //{{AFX_MSG(CippsDemoView)
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOVIEW_H__E8B98F95_D5F6_4323_9F56_78970A67DDB4__INCLUDED_)
