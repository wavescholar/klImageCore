/*
//
//                    INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// DemoView.cpp : implementation of the CippiDemoView class
//

#include <math.h>

#include "stdafx.h"
#include "ippiDemo.h"

#include "MainFrm.h"
#include "ippiDemoDoc.h"
#include "ippiDemoView.h"

#include "DemoDib.h"
#include "Director.h"
#include "ContextMenu.h"
#include "Movie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CippiDemoView

static UINT WM_Movie = ::RegisterWindowMessage(WM_MOVIE);

IMPLEMENT_DYNCREATE(CippiDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CippiDemoView, CScrollView)
    ON_REGISTERED_MESSAGE( WM_Movie, OnMovieMessage )
    //{{AFX_MSG_MAP(CippiDemoView)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippiDemoView construction/destruction

CippiDemoView::CippiDemoView() : CScrollView()
{
    m_pDib = new CDemoDib;

    m_FirstDraw = TRUE;
    m_Dragging = FALSE;
    m_NumPoints = 0;
    m_Clipping = 0;

    m_newPen = NULL;
    m_oldPen = NULL;
    m_oldBkMode = TRANSPARENT;
    m_oldROP2    = R2_COPYPEN;
}

CippiDemoView::~CippiDemoView()
{
     m_pDib->DeleteDibSection();
}

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides

BOOL CippiDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
     m_ClassName = AfxRegisterWndClass(
          CS_HREDRAW|CS_VREDRAW,
          0,
          0,
          0);
     cs.lpszClass = m_ClassName;

    return CScrollView::PreCreateWindow(cs);
}

void CippiDemoView::OnDraw(CDC* pDC)
{
    CippiDemoDoc* pDoc = GetDoc();
    ASSERT_VALID(pDoc);

     if (m_FirstDraw) {
          m_FirstDraw = FALSE;
          m_SrcDC.CreateCompatibleDC(pDC);
          m_pDib->CreateDibSection(pDC, pDoc);
     }
     if (pDoc->IsUpdated()) {
//         if (m_pDib->IsNewImageSize(pDoc)) {
//              m_pDib->DeleteDibSection();
//              m_pDib->CreateDibSection(pDC, pDoc);
//         }
          m_SrcDC.SelectObject(m_pDib->GetBitmapHandle());
          m_pDib->SetDibSection(pDoc);
          DrawRoi(&m_SrcDC, RGB(255,255,0));
          DrawQuad(&m_SrcDC, RGB(255,0,155));
          DrawParl(&m_SrcDC, RGB(155,0,255));
          DrawCenterShift(&m_SrcDC, RGB(0,255,255));
          DrawMarker(&m_SrcDC);
          pDoc->IsUpdated(FALSE);
     }
     pDC->BitBlt(0,0,pDoc->Width(),pDoc->Height(),&m_SrcDC,0,0,SRCCOPY);
}

void CippiDemoView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    CippiDemoDoc* pDoc = GetDoc();
    pDoc->InitHisto();
    pDoc->UpdateFrameTitle();
    pDoc->SetStatusString();
    CSize sizeTotal;
    sizeTotal.cx = pDoc->Width();
    sizeTotal.cy = pDoc->Height();
    SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// User Drwawing Implementation

void CippiDemoView::StartDraw()
{
    ClipCursor();
}

void CippiDemoView::CancelDraw()
{
    m_NumPoints = 0;
    ClipCursor(FALSE);
}

void CippiDemoView::ClipCursor(BOOL clip)
{
    if (clip && !m_Clipping) {
        CRect rect;
        GetClientRect(&rect);
        //rect.right  = IPP_MIN(rect.right ,GetDoc()->Width());
        //rect.bottom = IPP_MIN(rect.bottom,GetDoc()->Height());
        rect.right  = rect.right ;
        rect.bottom = rect.bottom;
        SetCapture();
        POINT origin = rect.TopLeft();    
        ::ClientToScreen(m_hWnd, &origin);
        rect = CRect(origin, rect.Size());
        ::ClipCursor(&rect);
        m_Clipping = TRUE;
    }
    if (!clip && m_Clipping) {
        m_Clipping = FALSE;
        ReleaseCapture();
        ::ClipCursor(NULL);
    }
}


/////////////////////////////////////////////////////////////////////////////
// Draw implementation

BOOL CippiDemoView::BeginDraw(CDC* pDC, COLORREF color, int& drawTool)
{
    ASSERT(pDC);
     CippiDemoDoc *pDoc = GetDoc();
    int penStyle;
     if (pDoc->GetDrawTool() == drawTool) {
        penStyle = PS_SOLID;
    } else if (DEMO_APP->GetContourShow()) {
        penStyle = PS_DASH;
        drawTool = 0;
    } else {
        return FALSE;
    }
     m_newPen = new CPen(penStyle, 1, color);
     m_oldPen     = pDC->SelectObject(m_newPen);
     m_newBrush  = new CBrush(color);
     m_oldBrush  = pDC->SelectObject(m_newBrush);
     m_oldBkMode = pDC->SetBkMode    (TRANSPARENT);
     m_oldROP2    = pDC->SetROP2      (R2_COPYPEN);
    return TRUE;
}

void CippiDemoView::EndDraw(CDC* pDC)
{
    if (m_newPen) {
        delete m_newPen;
        pDC->SelectObject(m_oldPen    );
    }
    if (m_newBrush) {
        delete m_newBrush;
        pDC->SelectObject(m_oldBrush    );
    }
     pDC->SetBkMode    (m_oldBkMode);
     pDC->SetROP2      (m_oldROP2  );
}
void CippiDemoView::DrawRoi(CDC* pDC, COLORREF color)
{
    CPoint points[2];
    if (!GetRoiFromDoc(points)) return;
    int drawTool = DRAW_ROI;
    if (!BeginDraw(pDC,color,drawTool)) return;


     pDC->MoveTo(points[0].x,points[0].y);
     pDC->LineTo(points[1].x,points[0].y);
     pDC->LineTo(points[1].x,points[1].y);
     pDC->LineTo(points[0].x,points[1].y);
     pDC->LineTo(points[0].x,points[0].y);

    EndDraw(pDC);
}

void CippiDemoView::DrawCenterShift(CDC* pDC, COLORREF color)
{
    CPoint points[2];
    if (!GetCenterShiftFromDoc(points)) return;
    int drawTool = DRAW_CENTER;
    if (!BeginDraw(pDC,color,drawTool)) return;

     pDC->MoveTo(points[0]);
     pDC->LineTo(points[1]);
    if (!drawTool) {
        DrawCircle(pDC, points[0]);
        DrawPointer(pDC, points[0], points[1]);
    }
    EndDraw(pDC);
}

void CippiDemoView::DrawQuad(CDC* pDC, COLORREF color)
{
    CPoint points[4];
    int num;
    if (!GetQuadFromDoc(points, num)) return;
    int drawTool = DRAW_QUAD;
    if (!BeginDraw(pDC,color,drawTool)) return;
     pDC->MoveTo(points[0]);
    for (int i=1; i<num; i++)
        pDC->LineTo(points[i]);
    if (num == 4)
        pDC->LineTo(points[0]);
    if (!drawTool) {
        DrawCircle(pDC, points[0]);
        DrawPointer(pDC, points[3], points[0]);
    }
    EndDraw(pDC);
}

void CippiDemoView::DrawParl(CDC* pDC, COLORREF color)
{
    CPoint points[4];
    int num;
    if (!GetParlFromDoc(points, num)) return;
    int drawTool = DRAW_PARL;
    if (!BeginDraw(pDC,color,drawTool)) return;
     pDC->MoveTo(points[0]);
    for (int i=1; i<num; i++)
        pDC->LineTo(points[i]);
    if (num == 4)
        pDC->LineTo(points[0]);
    if (!drawTool) {
        DrawCircle(pDC, points[0]);
        DrawPointer(pDC, points[3], points[0]);
    }
    EndDraw(pDC);
}

void CippiDemoView::DrawCircle(CDC* pDC, CPoint point)
{
    int r = 3;
    pDC->Ellipse(point.x-r, point.y-r,
                    point.x+r+1, point.y+r+1);
}

void CippiDemoView::DrawPointer(CDC* pDC, CPoint p0, CPoint p1)
{
     CPoint points[3];
     double len = 15.;
     double wid = .25;
     double x = p1.x - p0.x;
     double y = p1.y - p0.y;
     double a = x*x+y*y;
     if (a < 1.e-6) return;
     a = len / sqrt(x*x+y*y);
     double b = wid;
     x *= a;
     y *= a;
     points[0] = p1;
     points[1].x = (int)(p1.x - x + b*y + .5);
     points[1].y = (int)(p1.y - y - b*x + .5);
     points[2].x = (int)(p1.x - x - b*y + .5);
     points[2].y = (int)(p1.y - y + b*x + .5);
     pDC->Polygon(points,3);
}

void CippiDemoView::DrawMarker(CDC* pDC)
{
    CPoint points[3];
    BOOL bMax;
    int n = GetDoc()->GetMarker(points, bMax);
    if (n == 0) return;
    PointsFromDoc(points,n);
    COLORREF borderColor = bMax ? RGB(0, 0, 0) : RGB(255,255,255);
    COLORREF crossColor[3] = {RGB(0,0,255), RGB(0,255,0), RGB(255,0,0)};
    if (n == 1)
        crossColor[0] = bMax ? RGB(255,255,255) : RGB(0, 0, 0);
    for (int i = 0; i < n; i++) {
        DrawCrossBorder(pDC,points[i],borderColor);
    }
    DrawCrossPoints(pDC,points,crossColor,n);
}

void CippiDemoView::DrawCrossBorder(CDC* pDC, POINT point, COLORREF color)
{
    int len = 4;
    int x = point.x;
    int y = point.y;
    DrawLine(pDC, x - len, y - 1, x + len, y - 1, color);
    DrawLine(pDC, x - len, y + 1, x + len, y + 1, color);
    DrawLine(pDC, x - 1, y - len, x - 1, y + len, color);
    DrawLine(pDC, x + 1, y - len, x + 1, y + len, color);
}

void CippiDemoView::DrawLine(CDC* pDC, int x0, int y0, int x1, int y1, COLORREF clr)
{
    CPen pen(PS_SOLID, 1, clr);
    CPen* pOldPen = pDC->SelectObject(&pen);

    CPoint p1(x0, y0);
    CPoint p2(x1, y1);
    pDC->MoveTo(p1);
    pDC->LineTo(p2);

    pDC->SelectObject(pOldPen);
}

enum {LCROSS = 4, NCROSS = (LCROSS*2 + 1)*2};

static void SetCrossPoints(POINT center, POINT points[])
{
    int x = center.x;
    int y = center.y;
    int n = NCROSS>>1;
    for (int i=0; i<n; i++) {
        points[i].x = x + i - LCROSS;
        points[i].y = y;
        points[n + i].x = x;
        points[n + i].y = y + i - LCROSS;
    }
}

static void SetColorValue(COLORREF value[], COLORREF color)
{
    for (int i=0; i<NCROSS; i++) {
        value[i] = color;
    }
}

static void DrawPoints(CDC* pDC, POINT pp[], COLORREF value[])
{
    for (int i=0; i<NCROSS; i++) {
        pDC->SetPixel(pp[i], value[i]);
    }
}

static BOOL isInCross(POINT point, POINT pp[])
{
    for (int i=0; i<NCROSS; i++) {
        if ((point.x == pp[i].x) && (point.y == pp[i].y))
            return TRUE;
    }
    return FALSE;
}

static void composeColors(int index,
                                  POINT pp[][NCROSS], COLORREF value[][NCROSS],
                                  COLORREF color[], int numC)
{
    for (int iC=index + 1; iC<numC; iC++) {
        for (int iP=0; iP<NCROSS; iP++) {
            if (isInCross(pp[index][iP],pp[iC])) {
                value[index][iP] |= color[iC];
            }
        }
    }
}


void CippiDemoView::DrawCrossPoints(CDC* pDC, POINT center[], COLORREF color[], int numC)
{
    POINT pp[3][NCROSS];
    COLORREF value[3][NCROSS];
    int i;
    for (i=0; i<numC; i++) {
        SetCrossPoints(center[i],pp[i]);
        SetColorValue(value[i],color[i]);
    }
    if (numC > 1) composeColors(0,pp,value,color,numC);
    if (numC > 2) composeColors(1,pp,value,color,numC);

    for (i=numC - 1; i >= 0; i--) {
        DrawPoints(pDC, pp[i],value[i]);
    }
}

int CippiDemoView::GetReverseHeight()
{
     CRect clientRect;
     GetClientRect(&clientRect);
     return IPP_MIN(clientRect.bottom,GetDoc()->Height());
}

void CippiDemoView::ReversePoint(CPoint& point)
{
    int height = GetDoc()->Height();
    point.y = height - 1 - point.y;
}

void CippiDemoView::PointToDoc(CPoint& point)
{
    CPoint scrollPos = GetScrollPosition();
    point += scrollPos;
    ReversePoint(point);
}

void CippiDemoView::PointFromDoc(CPoint& point)
{
    CPoint scrollPos = GetScrollPosition();
    ReversePoint(point);
//    point -= scrollPos;
}

void CippiDemoView::PointsToDoc(CPoint* points, int num)
{
    for (int i=0; i<num; i++)
        PointToDoc(points[i]);
}

void CippiDemoView::PointsFromDoc(CPoint* points, int num)
{
    for (int i=0; i<num; i++)
        PointFromDoc(points[i]);
}

void CippiDemoView::SetRoiToDoc(const CPoint* srcPoints)
{
    CPoint points[2];
    points[0] = srcPoints[0];
    points[1] = srcPoints[1];
    PointsToDoc(points,2);
    GetDoc()->SetRoi(points);
}

BOOL CippiDemoView::GetRoiFromDoc(CPoint* points)
{
    if (!GetDoc()->GetRoi(points))
        return FALSE;
    PointsFromDoc(points,2);
    return TRUE;
}

void CippiDemoView::SetQuadToDoc(const CPoint* srcPoints, int num)
{
    CPoint points[4];
    for (int i=0; i<num; i++)
        points[i] = srcPoints[i];
    PointsToDoc(points,num);
    GetDoc()->SetQuad(points, num);
}

BOOL CippiDemoView::GetQuadFromDoc(CPoint* points, int& num)
{
     if (!GetDoc()->GetQuad(points, num))
        return FALSE;
    PointsFromDoc(points,num);
    return TRUE;
}

void CippiDemoView::SetParlToDoc(const CPoint* srcPoints, int num)
{
    CPoint points[4];
    for (int i=0; i<num; i++)
        points[i] = srcPoints[i];
    PointsToDoc(points,num);
    GetDoc()->SetParl(points, num);
}

BOOL CippiDemoView::GetParlFromDoc(CPoint* points, int& num)
{
     if (!GetDoc()->GetParl(points, num))
        return FALSE;
    PointsFromDoc(points,num);
    return TRUE;
}

void CippiDemoView::SetCenterShiftToDoc(const CPoint* srcPoints)
{
    CPoint points[2];
    points[0] = srcPoints[0];
    points[1] = srcPoints[1];
    PointsToDoc(points,2);
    GetDoc()->SetCenterShift(points);
}

BOOL CippiDemoView::GetCenterShiftFromDoc(CPoint* points)
{
     if (!GetDoc()->GetCenterShift(points))
        return FALSE;
    PointsFromDoc(points,2);
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CippiDemoView diagnostics

#ifdef _DEBUG
void CippiDemoView::AssertValid() const
{
    CScrollView::AssertValid();
}

void CippiDemoView::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}

CippiDemoDoc* CippiDemoView::GetDoc() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CippiDemoDoc)));
    return (CippiDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
//  Command processing implementation

//---------------------------------------------------------------------------
// Down{Roi/Quad/Parl/Center} performs LeftButtonDown command while
// user is drawing corresponding contour
//---------------------------------------------------------------------------
void CippiDemoView::DownRoi(CPoint point)
{
    m_Points[0] = point;
    m_NumPoints = 1;
    m_Points[1] = point;
    SetRoiToDoc(m_Points);
}

void CippiDemoView::DownQuad(CPoint point)
{
    if (m_NumPoints == 0) {
        m_Points[0] = point;
        m_NumPoints = 1;
    }
    m_NumPoints++;
    m_Points[m_NumPoints-1] = point;
    SetQuadToDoc(m_Points,m_NumPoints);
}

void CippiDemoView::DownParl(CPoint point)
{
    if (m_NumPoints == 0) {
        m_Points[0] = point;
        m_NumPoints = 1;
    }
    m_NumPoints++;
    m_Points[m_NumPoints-1] = point;
    if (m_NumPoints == 3) {
        m_NumPoints++;
        CImage::SetLastParlPoint(m_Points);
    }
    SetParlToDoc(m_Points,m_NumPoints);
}

void CippiDemoView::DownCenter(CPoint point)
{
    m_Points[0] = point;
    m_NumPoints = 1;
    m_Points[1] = point;
    SetCenterShiftToDoc(m_Points);
}

//---------------------------------------------------------------------------
// Move{Roi/Quad/Parl/Center} performs MouseMove command while
// user is drawing corresponding contour
//---------------------------------------------------------------------------

void CippiDemoView::MoveRoi(CPoint point)
{
    m_Points[1] = point;
    SetRoiToDoc(m_Points);
}

void CippiDemoView::MoveQuad(CPoint point)
{
    m_Points[m_NumPoints-1] = point;
    SetQuadToDoc(m_Points,m_NumPoints);
}

void CippiDemoView::MoveParl(CPoint point)
{
    if (m_NumPoints == 4) {
        m_Points[m_NumPoints-2] = point;
        CImage::SetLastParlPoint(m_Points);
    } else {
        m_Points[m_NumPoints-1] = point;
    }
    SetParlToDoc(m_Points,m_NumPoints);
}

void CippiDemoView::MoveCenter(CPoint point)
{
    m_Points[1] = point;
    SetCenterShiftToDoc(m_Points);
}

//---------------------------------------------------------------------------
// Up{Roi/Quad/Parl/Center} performs LeftButtonUp command while
// user is drawing corresponding contour
//---------------------------------------------------------------------------

BOOL CippiDemoView::UpRoi(CPoint point)
{
    m_Points[1] = point;
    if (m_Points[0] == m_Points[1])
         GetDoc()->ResetRoi();
    else
        SetRoiToDoc(m_Points);
    return TRUE;
}

BOOL CippiDemoView::UpQuad(CPoint point)
{
    if (m_NumPoints == 2 && m_Points[0] == m_Points[1]) {
         GetDoc()->ResetQuad();
        return TRUE;
    }
    m_Points[m_NumPoints-1] = point;
    SetQuadToDoc(m_Points,m_NumPoints);
    if (m_NumPoints == 4) return TRUE;
    else                        return FALSE;
}

BOOL CippiDemoView::UpParl(CPoint point)
{
    if (m_NumPoints == 2 && m_Points[0] == m_Points[1]) {
         GetDoc()->ResetParl();
        return TRUE;
    }
    if (m_NumPoints == 4) {
        m_Points[m_NumPoints-2] = point;
        CImage::SetLastParlPoint(m_Points);
    } else {
        m_Points[m_NumPoints-1] = point;
    }
    SetParlToDoc(m_Points,m_NumPoints);
    if (m_NumPoints == 4) return TRUE;
    else                        return FALSE;
}

BOOL CippiDemoView::UpCenter(CPoint point)
{
    m_Points[1] = point;
    SetCenterShiftToDoc(m_Points);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CippiDemoView message handlers

//---------------------------------------------------------------------------
// OnMovieMessage handles Movie message
//---------------------------------------------------------------------------
LRESULT CippiDemoView::OnMovieMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam) {
    case MOVIE_PLAY:
        GetDoc()->DrawMovie((LPCSTR)lParam);
        break;
    case MOVIE_STOP:
        GetDoc()->StopMovie();
        break;
    }
    return 0;
}

//---------------------------------------------------------------------------
// OnLButtonDown handles LeftButtonDown message
//---------------------------------------------------------------------------
void CippiDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CippiDemoDoc* pDoc = GetDoc();
    if (pDoc->GetDrawTool()) {
        m_Dragging = TRUE;
        if (m_NumPoints == 0) StartDraw();
        ClipCursor();
        switch (pDoc->GetDrawTool()) {
        case DRAW_ROI:  DownRoi(point); break;
        case DRAW_QUAD: DownQuad(point); break;
        case DRAW_PARL: DownParl(point); break;
        case DRAW_CENTER: DownCenter(point); break;
        }
        pDoc->UpdateView();
        pDoc->SetStatusString();
     }
    CScrollView::OnLButtonDown(nFlags, point);
}

//---------------------------------------------------------------------------
// OnMouseMove handles MouseMove message
//---------------------------------------------------------------------------
void CippiDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
    CippiDemoDoc* pDoc = GetDoc();
    if (m_Dragging) {
        switch (pDoc->GetDrawTool()) {
        case DRAW_ROI:  MoveRoi(point); break;
        case DRAW_QUAD: MoveQuad(point); break;
        case DRAW_PARL: MoveParl(point); break;
        case DRAW_CENTER: MoveCenter(point); break;
        }
        pDoc->UpdateView();
        pDoc->SetStatusString();
    }
    CScrollView::OnMouseMove(nFlags, point);
}

//---------------------------------------------------------------------------
// OnLButtonUp handles LeftButtonUp message
//---------------------------------------------------------------------------
void CippiDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
    CippiDemoDoc* pDoc = GetDoc();
    if (m_Dragging) {
        m_Dragging = FALSE;
        BOOL finish;
        switch (pDoc->GetDrawTool()) {
        case DRAW_ROI:     finish = UpRoi(point); break;
        case DRAW_QUAD:    finish = UpQuad(point); break;
        case DRAW_PARL:    finish = UpParl(point); break;
        case DRAW_CENTER: finish = UpCenter(point); break;
        }
        ClipCursor(FALSE);
        if (finish) {
            CancelDraw();
            pDoc->FinishDraw();
        }
    } else if (DEMO_APP->GetPickMode() && pDoc->MayBePicked()) {
        DEMO_APP->GetDirector()->GrabDoc(pDoc);
    }
    CScrollView::OnLButtonUp(nFlags, point);
}


//---------------------------------------------------------------------------
// OnLButtonDblClk handles LeftButtonDoubleCklick message
//---------------------------------------------------------------------------
void CippiDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CScrollView::OnLButtonDblClk(nFlags, point);
    if (m_Dragging) return;
    GetDoc()->ProcessVector();
}

//---------------------------------------------------------------------------
// OnEraseBkgnd handles EraseBackgroung message
//---------------------------------------------------------------------------
BOOL CippiDemoView::OnEraseBkgnd(CDC* pDC)
{
     CBrush brush(HS_DIAGCROSS, RGB(0,0,0));
     FillOutsideRect( pDC, &brush );
     return TRUE;                         // Erased
}

//---------------------------------------------------------------------------
// OnSetCursor handles SetCursor message
//---------------------------------------------------------------------------
BOOL CippiDemoView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    CippiDemoDoc* pDoc = GetDoc();
    if (DEMO_APP->IsCursorWait()) return TRUE;
    if (pDoc->GetDrawTool()) {
         DEMO_APP->SetCursorCross();
         return TRUE;
    } else if (DEMO_APP->GetPickMode()) {
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
         DEMO_APP->SetCursorArrow();
         return TRUE;
    }
// return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

//---------------------------------------------------------------------------
// OnRButtonDown handles RightButtonDown message
//---------------------------------------------------------------------------
void CippiDemoView::OnRButtonDown(UINT nFlags, CPoint point)
{
    UINT ident =
        (GetDoc()->GetDrawTool() != DRAW_NONE) ? IDR_IMAGE_D :
        (DEMO_APP->GetPickMode()) ? IDR_IMAGE_P :
                                             IDR_IMAGE;
    ::ClientToScreen(m_hWnd, &point);
    CContextMenu cMenu;
    cMenu.TrackPopup(ident, point);
}


