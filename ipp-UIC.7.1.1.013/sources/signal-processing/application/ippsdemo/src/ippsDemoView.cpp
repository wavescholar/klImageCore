/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// demoView.cpp : implementation of the CippsDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#include <math.h>

#include "stdafx.h"
#include "ippsDemo.h"

#include "ippsDemoDoc.h"
#include "ippsDemoView.h"
#include "ippsDirector.h"
#include "MainFrm.h"
#include "ContextMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CippsDemoView

CFont CippsDemoView::m_Font;
COLORREF CippsDemoView::m_colorSignal     = RGB(  0,  0,  0);
COLORREF CippsDemoView::m_colorSignalBack = RGB(193,205,208); 
COLORREF CippsDemoView::m_colorAxis       = RGB(128,  0,  0);
COLORREF CippsDemoView::m_colorAxisBack   = RGB(160,192,201);

IMPLEMENT_DYNCREATE(CippsDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CippsDemoView, CScrollView)
   //{{AFX_MSG_MAP(CippsDemoView)
   ON_WM_LBUTTONUP()
   ON_WM_SETCURSOR()
   ON_WM_RBUTTONDOWN()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippsDemoView construction/destruction

CippsDemoView::CippsDemoView()
{
   m_numPoints = 0;
   m_points = NULL;
   m_pointsIm = NULL;
   if ((HFONT)m_Font == NULL) {
      m_Font.CreateFont(12, 0, 0, 0, FW_THIN, FALSE, FALSE, 0, 
         ANSI_CHARSET, OUT_CHARACTER_PRECIS,  CLIP_CHARACTER_PRECIS, 
         DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
   }
   m_pBitmap = NULL;
}

CippsDemoView::~CippsDemoView()
{
   if (m_points) delete[] m_points;
   if (m_pointsIm) delete[] m_pointsIm;
}

BOOL CippsDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CScrollView::PreCreateWindow(cs);
}

////////////////////////////////////////////////////////////////////////////
// Attributes

BOOL CippsDemoView::ScrollW() 
{ 
   CRect rect;
   GetWindowRect(&rect);
   return rect.Height() - GetClientHeight() >= ::GetSystemMetrics(SM_CYHSCROLL);
}

BOOL CippsDemoView::ScrollH() 
{ 
   CRect rect;
   GetWindowRect(&rect);
   return rect.Width() - GetClientWidth() >= ::GetSystemMetrics(SM_CXHSCROLL);
}

int CippsDemoView::GetSpaceHeight() { 
   return 24;
}
int CippsDemoView::GetSpaceCplxHeight() { 
   return 24;
}
int CippsDemoView::GetSpaceWidth() { 
   return 50;
}

static int getSpaceHeight() { 
   return DEMO_APP->GetXAxis() ? CippsDemoView::GetSpaceHeight() : 0;
}
static int getSpaceCplxHeight() { 
   return DEMO_APP->GetXAxis() ? CippsDemoView::GetSpaceCplxHeight() : 10;
}
static int getSpaceWidth() { 
   return DEMO_APP->GetYAxis() ? CippsDemoView::GetSpaceWidth() : 0;
}
static int getMaxHeight() { return INT_MAX - 100;}
static int getMinStepX() { return 60;}
static int getMinStepY() { return 30;}

int CippsDemoView::GetClientWidth() 
{ 
   RECT rect; 
   GetClientRect(&rect); 
   return rect.right;
}

int CippsDemoView::GetClientHeight() 
{ 
   RECT rect; 
   GetClientRect(&rect); 
   return rect.bottom;
}

int CippsDemoView::GetBorderHeight()
{
   CippsDemoDoc *pDoc = GetDoc();
   ASSERT(pDoc);
   int h = getSpaceHeight()*2;
   if (pDoc->Complex()) h += getSpaceCplxHeight();
   return h;
}

int CippsDemoView::GetBorderWidth()
{
   return getSpaceWidth()*2;
}


int CippsDemoView::GetInitialWidth()
{
   int w = (MAIN_FRAME->GetClientWidth()*9)/10 - GetBorderWidth();
   CippsDemoDoc *pDoc = GetDoc();
   if (w > pDoc->Length())
      w = pDoc->Length();
   return w + GetBorderWidth();
}

int CippsDemoView::GetInitialHeight()
{
   CippsDemoDoc *pDoc = GetDoc();
   return (pDoc->Complex() ? 256 : 128) + GetBorderHeight();
}

double CippsDemoView::GetAmplitude()
{
   double factor = 1;
   return GetAmplitude(factor);
}

double CippsDemoView::GetAmplitude(double factor)
{
   double min, max;
   GetMinMax(min,max);
   double ampl = (max - min)*factor;
   if (GetDoc()->Complex()) ampl *= 2;
   return ampl;
}

static void getConstFrame(double val, double& minVal, double& maxVal)
{
   if (val == 0) {
      minVal = -1;
      maxVal = 1;
      return;
   }
   double aval = val > 0 ? val : -val;   
   double step = pow((double)10,(double)(int)log10(aval));
   for (minVal = 0; aval < minVal || aval > minVal + step; minVal += step);
   maxVal = minVal + step;
   if ((aval - minVal)/aval < 0.1) minVal -=step;
   if ((maxVal - aval)/aval < 0.1) maxVal +=step;
   if (val < 0) {
      double tmp = minVal;
      minVal = -maxVal;
      maxVal = -tmp;
   }
}

void CippsDemoView::GetMinMax(double& min, double& max)
{
   min = GetDoc()->MinMin();
   max = GetDoc()->MaxMax();
   if (GetDoc()->Unsign()) {
      if (min <= max*.5) min = 0;
   } else {
      if (min < 0 && max > 0) {
         if (max < -min && max >= -min*.5)
            max = -min;
         if (-min < max && -min >= max*.5)
            min = -max;
      } else if (min > 0) {
         if (min <= max*.5) min = 0;
      } else if (max < 0) {
         if (max >= min*.5) max = 0;
      }
   }

   if (min == max)
      getConstFrame(min, min, max);
}

int CippsDemoView::GetMinPixel()
{
   double min, max;
   GetMinMax(min,max);
   return (int)(min*GetDoc()->FactorH() + .5);
}

int CippsDemoView::GetMaxPixel()
{
   double min, max;
   GetMinMax(min,max);
   return (int)(max*GetDoc()->FactorH() + .5);
}

enum {NUM_GAP = 8};
static double Gaps[NUM_GAP] = {.1, .2, .5, 1.0, 2.0, 5.0, 10.0, 20.0};

double CippsDemoView::GetStepAxis(double minStep, double magn)
{
   double step;
   do {
      double step10 = pow((double)10,(int)log10(minStep));
      for (int i=0; i<NUM_GAP; i++) {
         step = step10*Gaps[i];
         if (step >= minStep) break;
      }
      minStep *= .5;
   } while (magn > 0 && step > magn);
   return step;
}

int CippsDemoView::GetScaleWidth(double factor)
{
   CippsDemoDoc* pDoc = GetDoc();
   if (factor < 0) factor = pDoc->FactorW();
   int w = (int)(pDoc->Length()*factor + .5) + GetBorderWidth();
   return w;
}

int CippsDemoView::GetScaleHeight(double factor)
{
   CippsDemoDoc* pDoc = GetDoc();
   if (factor < 0) factor = pDoc->FactorH();
   double ampl = GetAmplitude(factor);
   int h = (ampl < getMaxHeight()) ? (int)ampl + GetBorderHeight() : getMaxHeight();
   return h;
}

CSize CippsDemoView::GetScaleSize()
{
   CSize size;
   size.cx = GetScaleWidth();
   size.cy = GetScaleHeight();
   return size;
}

////////////////////////////////////////////////////////////////////////////
// Operations

void CippsDemoView::Zoom()
{
   SetScrollSizes(MM_TEXT, GetScaleSize());

   CippsDemoDoc* pDoc = GetDoc();
   if (pDoc->Complex())
      SetPointsComplex();
   else
      SetPointsReal();
}

void CippsDemoView::SetPointsReal()
{
   CippsDemoDoc* pDoc = GetDoc();
   AllocatePoints();
   double step = 1 / pDoc->FactorW();
   double x = 0.5;
   for (int i=0; i < m_numPoints; i++, x+=step) {
      Ipp64fc value = pDoc->GetScaledValue((int)x);
      m_points[i].x = i + getSpaceWidth();
      m_points[i].y = -(int)(value.re + .5);
   }
}

void CippsDemoView::SetPointsComplex()
{
   CippsDemoDoc* pDoc = GetDoc();
   AllocatePoints();
   double step = 1 / pDoc->FactorW();
   double x = 0.5;
   for (int i=0; i < m_numPoints; i++, x+=step) {
      Ipp64fc value = pDoc->GetScaledValue((int)x);
      m_points[i].x = i + getSpaceWidth();
      m_points[i].y = -(int)(value.re + .5);
      m_pointsIm[i].x = i + getSpaceWidth();
      m_pointsIm[i].y = -(int)(value.im + .5);
   }
}

void CippsDemoView::AllocatePoints()
{
   CippsDemoDoc* pDoc = GetDoc();
   int num = (int)(pDoc->Length() * pDoc->FactorW());
   if (num == m_numPoints) return;
   m_numPoints = num;
   if (m_points) delete[] m_points;
   m_points = new POINT[m_numPoints];
   if (pDoc->Complex()) {
      if (m_pointsIm) delete[] m_pointsIm;
      m_pointsIm = new POINT[m_numPoints];
   }
}

/////////////////////////////////////////////////////////////////////////////
// CippsDemoView drawing

void CippsDemoView::OnInitialUpdate()
{
   CScrollView::OnInitialUpdate();

   CSize sizeTotal;
   sizeTotal.cx = GetInitialWidth();
   sizeTotal.cy = GetInitialHeight();
   SetScrollSizes(MM_TEXT, sizeTotal);

   CippsDemoDoc* pDoc = GetDoc();
   ASSERT(pDoc);
   pDoc->UpdateFrameTitle();
   pDoc->ZoomStandard();
   // Set the total size of this view
}

void CippsDemoView::OnDraw(CDC* pDC)
{
   CippsDemoDoc* pDoc = GetDoc();
   ASSERT_VALID(pDoc);
   if (pDoc->Length() == 0) return;
   if (m_IsUpdated) {
      m_SrcDC.DeleteDC();
      if (!m_SrcDC.CreateCompatibleDC(pDC)) return;
      if (m_pBitmap) delete m_pBitmap;
      m_pBitmap = new CBitmap;
      if (!m_pBitmap->CreateCompatibleBitmap(pDC, 
         GetScaleWidth(), GetScaleHeight())) return;
      m_SrcDC.SelectObject(m_pBitmap);
      DrawDC(&m_SrcDC);
      m_IsUpdated = FALSE;
   }
   pDC->BitBlt(0, 0, GetScaleWidth(), GetScaleHeight(), &m_SrcDC, 0, 0, SRCCOPY);
}

void CippsDemoView::DrawDC(CDC* pDC)
{
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetROP2  (R2_COPYPEN);

   if (GetDoc()->Complex())
      DrawComplex(pDC);
   else
      DrawReal(pDC);
}

int CippsDemoView::GetX0() { return getSpaceWidth(); }
int CippsDemoView::GetX1() { return GetX0() + (int)(GetDoc()->Length() * GetDoc()->FactorW() + .5);}
int CippsDemoView::GetZero() { return getSpaceHeight() + GetMaxPixel();}
int CippsDemoView::GetZeroIm() { 
   return getSpaceHeight() + getSpaceCplxHeight() + 
          GetMaxPixel()*2 - GetMinPixel();
}
int CippsDemoView::GetY0(int zero) { return zero - GetMinPixel();}
int CippsDemoView::GetY1(int zero) { return zero - GetMaxPixel();}

void CippsDemoView::SelectPenSignal(CDC* pDC)
{
   m_Pen.DeleteObject();
   m_Pen.CreatePen(PS_SOLID, 1, m_colorSignal);
   pDC->SelectObject(&m_Pen);
}

void CippsDemoView::SelectPenAxis(CDC* pDC)
{
   m_Pen.DeleteObject();
   m_Pen.CreatePen(PS_SOLID, 1, m_colorAxis);
   pDC->SelectObject(&m_Pen);
   pDC->SetTextColor(m_colorAxis);
   pDC->SetBkColor(m_colorAxisBack);
}

void CippsDemoView::DrawReal(CDC* pDC)
{
   DrawBackReal(pDC);

   int zero = GetZero();
   DrawFrame(pDC, zero);

   int i;
   for (i=0; i < m_numPoints; i++)
      m_points[i].y += zero;

   SelectPenSignal(pDC);
   pDC->Polyline(m_points,m_numPoints);

   SelectPenAxis(pDC);
   DrawAxisX(pDC,zero);
   DrawAxisY(pDC,zero);

   for (i=0; i < m_numPoints; i++)
      m_points[i].y -= zero;
}

void CippsDemoView::DrawComplex(CDC* pDC)
{
   DrawBackComplex(pDC);

   CippsDemoDoc *pDoc = GetDoc();
   int zero = GetZero();
   int zeroIm = GetZeroIm();
   DrawFrame(pDC, zero);
   DrawFrame(pDC, zeroIm);

   int i;
   for (i=0; i < m_numPoints; i++) {
      m_points  [i].y += zero;
      m_pointsIm[i].y += zeroIm;
   }

   SelectPenSignal(pDC);
   pDC->Polyline(m_points,m_numPoints);
   pDC->Polyline(m_pointsIm,m_numPoints);

   SelectPenAxis(pDC);
   DrawAxisX(pDC,zero);
   DrawAxisX(pDC,zeroIm, FALSE);
   DrawAxisY(pDC,zero);
   DrawAxisY(pDC,zeroIm);

   for (i=0; i < m_numPoints; i++) {
      m_points  [i].y -= zero;
      m_pointsIm[i].y -= zeroIm;
   }
}

void CippsDemoView::DrawFrame(CDC* pDC, int zero)
{
   int x0 = GetX0();
   int x1 = GetX1();   
   int y0 = GetY0(zero);
   int y1 = GetY1(zero);   
   if (DEMO_APP->GetXAxis()) {
      DrawLine(pDC, x0, y0, x1, y0);
      DrawLine(pDC, x0, y1, x1, y1);
   }
   if (DEMO_APP->GetYAxis()) {
      DrawLine(pDC, x0, y0, x0, y1);
      DrawLine(pDC, x1, y0, x1, y1);
   }
}

void CippsDemoView::DrawAxisX(CDC* pDC, int zero, BOOL bTopNumbers)
{
   CippsDemoDoc *pDoc = GetDoc();
   int len = pDoc->Length();
   int w = (int)(len * pDoc->FactorW());
   int x0 = GetX0(); 
   int y0 = GetY0(zero);
   int y1 = GetY1(zero);
   double factor = pDoc->FactorW();
   double step = GetStepAxis(getMinStepX()/pDoc->FactorW(), len);
   double gap = step*pDoc->FactorW();

   if (!DEMO_APP->GetXAxis()) return;
   for (int i=1 ;; i++) {
      int x = (int)(gap*i);
      if (x > w) break;
      DrawLabelX(pDC, x0 + x, y0, (int)(step*i + .5), SIDE_BOTTOM);
      DrawLabelX(pDC, x0 + x, y1, (int)(step*i + .5), bTopNumbers ? SIDE_TOP : 0);
      DrawGridLine(pDC, x0 + x, y0, x0 + x, y1);
   }
}

static double minLabel(double min, double step) 
{
   double val = min >= 0 ? min : -min;
   double lab = ((int)(val / step))*step;
   if (min < 0) return -lab;
   if (ABS(val - lab)/step < 1.e-2) return lab;
   return lab + step;
}

void CippsDemoView::DrawAxisY(CDC* pDC, int zero)
{
   CippsDemoDoc *pDoc = GetDoc();
   double factorW = pDoc->FactorW();
   double factorH = pDoc->FactorH();
   double min, max;
   GetMinMax(min,max);
   double minStep = getMinStepY()/factorH;
   if (!pDoc->Float()) minStep = MAX(minStep, 1);
   double step = GetStepAxis(minStep, pDoc->Magnitude(min,max));
   int h = (int)((max - min) * factorH);
   int w = (int)(pDoc->Length() * factorW + .5);
   int x0 = GetX0(); 
   int x1 = GetX1(); 
   double gap = step*factorH;

//   pDC->FillSolidRect(0, 0, getSpaceWidth(), GetScaleHeight(), m_colorAxisBack);  
//   pDC->FillSolidRect(x1, 0, getSpaceWidth(), GetScaleHeight(), m_colorAxisBack);  
   if (!DEMO_APP->GetYAxis()) return;

   ASSERT(h > 0);
   double val = minLabel(min,step);
   double yStart = val*factorH;
   double yEnd = max*factorH + 1;
   for (double y=yStart; y < yEnd; y += gap, val += step) {
      if (ABS(val)/step < 1.e-3) val = 0;
      int iy = (int)(y + .5);
      DrawLabelY(pDC, x0, zero - iy,  val, SIDE_LEFT);
      DrawLabelY(pDC, x1, zero - iy,  val, SIDE_RIGHT);
      DrawGridLine(pDC, x0, zero - iy, x1, zero - iy);
   }
}

void CippsDemoView::DrawGridLine(CDC* pDC, int x0, int y0, int x1, int y1)
{
   if (DEMO_APP->GetGrid())
      DrawLine(pDC, x0, y0, x1, y1);
}

void CippsDemoView::DrawLine(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF clr)
{
   if ((int)clr < 0)
      clr = GetColorAxis();
   CPen pen(PS_SOLID, 1, clr);
   CPen* pOldPen = pDC->SelectObject(&pen);

   CPoint p1(x0, y0);
   CPoint p2(x1, y1);
   pDC->MoveTo(p1);
   pDC->LineTo(p2);

   pDC->SelectObject(pOldPen);
}

void CippsDemoView::DrawLabelX(CDC* pDC, int x, int y, int val, int textSide, 
                           COLORREF clr)
{
   if ((int)clr < 0)
      clr = GetColorAxis();
   int halfLabel = 3;
   DrawLine(pDC, x, y - halfLabel, x, y + halfLabel, clr);
   if (!textSide) return;
   pDC->SelectObject(&m_Font);
   pDC->SetTextColor(clr);
   CString text;
   text.Format("%d",val);
   CSize offset = pDC->GetTextExtent(text);
   offset.cx = -offset.cx/2;
   if (textSide == SIDE_TOP)
      offset.cy = -offset.cy - halfLabel*2;
   else
      offset.cy = halfLabel*2;
   pDC->TextOut(x + offset.cx, y + offset.cy, text);
}

void CippsDemoView::DrawLabelY(CDC* pDC, int x, int y, double val, int textSide, 
                           COLORREF clr, BOOL bIntVal)
{
   int halfLabel = 3;
   DrawLine(pDC, x - halfLabel, y, x + halfLabel, y, clr);
   if (!textSide) return;
   pDC->SelectObject(&m_Font);
   pDC->SetTextColor(clr);
   CString text;
   if (bIntVal) 
      text.Format("%d",(int)val);
   else                                             
      text.Format("%.6g",val);
   CSize offset = pDC->GetTextExtent(text);
   if (textSide == SIDE_LEFT)
      offset.cx = - offset.cx - halfLabel*2;
   else
      offset.cx = halfLabel*2;
   offset.cy = -offset.cy/2;
   pDC->TextOut(x + offset.cx, y + offset.cy, text);
}

void CippsDemoView::DrawLabelY(CDC* pDC, int x, int y, double val, int textSide)
{
   BOOL bIntVal = TRUE;
   if (GetDoc()->Float()) bIntVal = FALSE;
   else if (GetDoc()->Depth() >= 32) {
      if (ABS(val) > 999999) bIntVal = FALSE;
   }
   DrawLabelY(pDC, x, y, val, textSide, GetColorAxis(), bIntVal);
}

static int cDelta = 40;

static void dark(int& c)
{
   c -= cDelta;
   if (c < 0) c = 0;
}

static void light(int& c)
{
   c += cDelta;
   if (c > 255) c = 255;
}

static COLORREF darkColor(COLORREF clr)
{
   int cRed   = clr & 0x000000FF; 
   int cGreen = (clr & 0x0000FF00) >> 8; 
   int cBlue  = (clr & 0x00FF0000) >> 16; 
   dark(cRed  );
   dark(cGreen);
   dark(cBlue );
   return RGB(cRed,cBlue,cGreen);
}

static COLORREF lightColor(COLORREF clr)
{
   int cRed   = clr & 0x000000FF; 
   int cGreen = (clr & 0x0000FF00) >> 8; 
   int cBlue  = (clr & 0x00FF0000) >> 16; 
   light(cRed);
   light(cGreen);
   light(cBlue);
   return RGB(cRed,cBlue,cGreen);
}

void CippsDemoView::DrawBackReal(CDC* pDC)
{
   DrawBackAxis(pDC);
   DrawBackSignal(pDC, GetZero());
}

void CippsDemoView::DrawBackComplex(CDC* pDC)
{
   DrawBackAxis(pDC);
   DrawBackSignal(pDC, GetZero());
   DrawBackSignal(pDC, GetZeroIm());
}

void CippsDemoView::DrawBackAxis(CDC* pDC)
{
   CBrush brush(m_colorAxisBack);
   CRect rect(0, 0, GetScaleWidth(), GetScaleHeight());
   pDC->FillRect(&rect,&brush);
}

void CippsDemoView::DrawBackSignal(CDC* pDC, int zero)
{
   CBrush brush(m_colorSignalBack);
   int left   = GetX0();
   int right  = GetX1();
   int top    = GetY1(zero);
   int bottom = GetY0(zero);
   CRect rect(left,top,right,bottom);
   pDC->FillRect(&rect,&brush);

   /*
   left--;
   top--;
   DrawLine(pDC, left, top, right, top,    darkColor(m_colorAxisBack));
   DrawLine(pDC, left, top, left,  bottom, darkColor(m_colorAxisBack));
   DrawLine(pDC, right, bottom, left,  bottom, lightColor(m_colorAxisBack));
   DrawLine(pDC, right, bottom, right, top,    lightColor(m_colorAxisBack));
   */
}

void CippsDemoView::DrawBackground(CDC* pDC)
{
   int gW = GetScaleWidth();
   int gH = GetScaleHeight();
   int cW = GetClientWidth();
   int cH = GetClientHeight();
   POINT     points[6];
   points[0].x = gW;  points[0].y = gH;
   points[1].x = gW;  points[1].y =  0;
   points[2].x = cW;  points[2].y =  0;
   points[3].x = cW;  points[3].y = cH;
   points[4].x =  0;  points[4].y = cH;
   points[5].x =  0;  points[5].y = gH;

   CPen pen(PS_NULL, 0, RGB(0,0,0));
   CBrush brush(m_colorAxisBack);
   pDC->SelectObject(&pen);
   pDC->SelectObject(&brush);
   pDC->Polygon(points,6);
}

/////////////////////////////////////////////////////////////////////////////
// CippsDemoView diagnostics

#ifdef _DEBUG
void CippsDemoView::AssertValid() const
{
   CScrollView::AssertValid();
}

void CippsDemoView::Dump(CDumpContext& dc) const
{
   CScrollView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CippsDemoView message handlers

void CippsDemoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   m_IsUpdated = TRUE;
   Zoom();
}

void CippsDemoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->GetPickMode() && pDoc->MayBePicked()) {
      DEMO_APP->GetDirector()->GrabDoc(pDoc);
   }
   CScrollView::OnLButtonUp(nFlags, point);
}

BOOL CippsDemoView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
   CippsDemoDoc *pDoc = GetDoc();
   if (DEMO_APP->IsCursorWait()) return TRUE;
   if (DEMO_APP->GetPickMode()) {
      if (pDoc->MayBePicked()) {
         DEMO_APP->SetCursorGrab();
         MAIN_FRAME->SetMessageGrab();
         return TRUE;
      } else {
         DEMO_APP->SetCursorPick();
         MAIN_FRAME->SetMessagePick();
         return TRUE;
      }
   } else {
      return CScrollView::OnSetCursor(pWnd, nHitTest, message);
   }
   
}

void CippsDemoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   UINT ident = (DEMO_APP->GetPickMode()) ? IDR_SIGNAL_P : IDR_SIGNAL;
   ClientToScreen(&point);
   CContextMenu cMenu;
   cMenu.TrackPopup(ident, point);
}
