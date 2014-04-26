/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ippsDemoDoc.cpp : implementation of the CippsDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"

#include "ippsDemoDoc.h"
#include "ippsDemoView.h"
#include "ippsTextView.h"
#include "ippsCharView.h"
#include "ChildFrm.h"
#include "TextFrm.h"
#include "CharFrm.h"
#include "MainFrm.h"
#include "Director.h"
#include "Histo.h"
#include "FilePropDlg.h"
#include "FileHistoDlg.h"
#include "ZoomDlg.h"
#include "StoreWav.h"
#include "StoreBmp.h"
#include "NewSignal.h"
#include "NewTaps.h"
#include "NewChar.h"
#include "StringDlg.h"

#include "common\CProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CippsDemoDoc

IMPLEMENT_DYNCREATE(CippsDemoDoc, CDemoDoc)

BEGIN_MESSAGE_MAP(CippsDemoDoc, CDemoDoc)
   //{{AFX_MSG_MAP(CippsDemoDoc)
   ON_COMMAND(ID_FILE_PROP, OnFileProp)
   ON_COMMAND(ID_ZOOM_HUP, OnZoomHup)
   ON_COMMAND(ID_ZOOM_HLOW, OnZoomHlow)
   ON_COMMAND(ID_ZOOM_WUP, OnZoomWup)
   ON_COMMAND(ID_ZOOM_WLOW, OnZoomWlow)
   ON_COMMAND(ID_ZOOM_CUSTOM, OnZoomCustom)
   ON_COMMAND(ID_ZOOM_FITH, OnZoomFith)
   ON_COMMAND(ID_ZOOM_FITW, OnZoomFitw)
   ON_COMMAND(ID_ZOOM_STANDARD, OnZoomStandard)
   ON_COMMAND(ID_ZOOM_CURRENT, OnZoomCurrent)
   ON_UPDATE_COMMAND_UI(ID_WINDOW_PLOT, OnUpdateWindowPlot)
   ON_UPDATE_COMMAND_UI(ID_WINDOW_TEXT, OnUpdateWindowText)
   ON_COMMAND(ID_WINDOW_PLOT, OnWindowPlot)
   ON_COMMAND(ID_WINDOW_TEXT, OnWindowText)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_CURRENT, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_HLOW, OnUpdateZoomHlow)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_WLOW, OnUpdateZoomWlow)
   ON_UPDATE_COMMAND_UI(ID_FILE_SIGNED, OnUpdateFileSigned)
   ON_COMMAND(ID_FILE_SIGNED, OnFileSigned)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_CUSTOM, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_FITH, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_FITW, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_HUP, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_STANDARD, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_ZOOM_WUP, OnUpdateZoom)
   ON_COMMAND(ID_EDIT_INSERT, OnEditInsert)
   //}}AFX_MSG_MAP
   ON_UPDATE_COMMAND_UI(ID_WINDOW_CHAR, OnUpdateWindowChar)
   ON_COMMAND(ID_WINDOW_CHAR, OnWindowChar)
   ON_UPDATE_COMMAND_UI(ID_EDIT_INSERT, OnUpdateEditInsert)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CippsDemoDoc construction/destruction

CippsDemoDoc::CippsDemoDoc() : CDemoDoc(), CVector()
{
   m_scaleW = 1;
   m_scaleH = 1;
   m_SamplesPerSecond = 0;
}

CippsDemoDoc::~CippsDemoDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc virtual function overrides

//---------------------------------------------------------------------------
// GetFrame returns graphic view frame if it exists and returns
// text view frame otherwise
//---------------------------------------------------------------------------
CMDIChildWnd* CippsDemoDoc::GetFrame()
{
   return GetDemoFrame() ? (CMDIChildWnd*)GetDemoFrame() :
          GetCharFrame() ? (CMDIChildWnd*)GetCharFrame() :
                           (CMDIChildWnd*)GetTextFrame();
}

//---------------------------------------------------------------------------
// CreateNewData creates new vector with default parameters
//---------------------------------------------------------------------------
BOOL CippsDemoDoc::CreateNewData()
{
   if (DEMO_APP->NewIsEmpty()) {
      Init(DEMO_APP->GetNewType(), DEMO_APP->GetNewLength());
   } else if (DEMO_APP->NewView() == VIEW_DEMO) {
      if (!DEMO_APP->GetNewSignal()->CreateVector(this))
         return FALSE;
   } else if (DEMO_APP->NewView() == VIEW_TEXT) {
      if (!DEMO_APP->GetNewTaps()->CreateVector(this))
         return FALSE;
   } else {
      if (!DEMO_APP->GetNewChar()->CreateVector(this))
         return FALSE;
   }
   return TRUE;
}

//---------------------------------------------------------------------------
// UpdateView updates specified document view or all views
//---------------------------------------------------------------------------
void CippsDemoDoc::UpdateView(CView* pSender)
{
   if (GetDemoView())
      GetDemoView()->InvalidateRect(NULL);
   UpdateAllViews(pSender);
}

//---------------------------------------------------------------------------
// UpdateFrameTitle updates document title
//---------------------------------------------------------------------------
void CippsDemoDoc::UpdateFrameTitle(CString str)
{
   CDemoDoc::UpdateFrameTitle(str);
   if (GetTextFrame())
      GetTextFrame()->SetWindowText(m_FrameTitle);
   if (GetCharFrame())
      GetCharFrame()->SetWindowText(m_FrameTitle);
}

/////////////////////////////////////////////////////////////////////////////
// MFC Overrides

//---------------------------------------------------------------------------
// CippsDemoDoc serialization
//---------------------------------------------------------------------------
void CippsDemoDoc::Serialize(CArchive& ar)
{
   CFile *file = ar.GetFile();
   CStoreWav wav(this);
   if (wav.Serialize(file,ar.IsStoring())) return;
   CStoreBmp bmp(this);
   if (bmp.Serialize(file,ar.IsStoring())) return;

   CMyString filePath = file->GetFilePath();
   CString ext = filePath.GetExt();
   ext.MakeLower();
   if (ext == ".txt") {
      ((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
      if (ar.IsLoading()) {
         CString text;
         ((CEditView*)m_viewList.GetHead())->GetWindowText(text);
         Init(pp8u);
         SetString(text);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// Attributes

//---------------------------------------------------------------------------
// GetActiveView return active view
//---------------------------------------------------------------------------
CView* CippsDemoDoc::GetActiveView()
{
   return ACTIVE_FRAME == (CMDIChildWnd*)GetDemoFrame() ? (CView*)GetDemoView() :
          ACTIVE_FRAME == (CMDIChildWnd*)GetTextFrame() ? (CView*)GetTextView() :
          ACTIVE_FRAME == (CMDIChildWnd*)GetCharFrame() ? (CView*)GetCharView() :
                                                          NULL;
}

//---------------------------------------------------------------------------
// GetDemoView returns graphic view if it exists
//---------------------------------------------------------------------------
CippsDemoView* CippsDemoDoc::GetDemoView()
{
   POSITION pos = GetFirstViewPosition();
   while (pos) {
      CippsDemoView* pView = (CippsDemoView*)GetNextView(pos);
      if (strcmp(pView->GetRuntimeClass()->m_lpszClassName, "CippsDemoView") == 0)
         return pView;
   }
   return NULL;
}

//---------------------------------------------------------------------------
// GetTextView returns digital view if it exists
//---------------------------------------------------------------------------
CippsTextView* CippsDemoDoc::GetTextView()
{
   POSITION pos = GetFirstViewPosition();
   while (pos) {
      CippsTextView* pView = (CippsTextView*)GetNextView(pos);
      if (pView->GetRuntimeClass()->m_lpszClassName == "CippsTextView")
         return pView;
   }
   return NULL;
}

//---------------------------------------------------------------------------
// GetCharView returns string view if it exists
//---------------------------------------------------------------------------
CippsCharView* CippsDemoDoc::GetCharView()
{
   POSITION pos = GetFirstViewPosition();
   while (pos) {
      CippsCharView* pView = (CippsCharView*)GetNextView(pos);
      if (pView->GetRuntimeClass()->m_lpszClassName == "CippsCharView")
         return pView;
   }
   return NULL;
}

//---------------------------------------------------------------------------
// GetDemoFrame returns graphic view frame if it exists
//---------------------------------------------------------------------------
CChildFrame* CippsDemoDoc::GetDemoFrame()
{
   return (CChildFrame*)(GetDemoView() ? GetDemoView()->GetParentFrame() : NULL);
}

//---------------------------------------------------------------------------
// GetTextFrame returns digital view frame if it exists
//---------------------------------------------------------------------------
CTextFrame* CippsDemoDoc::GetTextFrame()
{
   return (CTextFrame*)(GetTextView() ? GetTextView()->GetParentFrame() : NULL);
}

//---------------------------------------------------------------------------
// GetCharFrame returns string view frame if it exists
//---------------------------------------------------------------------------
CCharFrame* CippsDemoDoc::GetCharFrame()
{
   return (CCharFrame*)(GetCharView() ? GetCharView()->GetParentFrame() : NULL);
}

//---------------------------------------------------------------------------
// GetScaledValue returns vector element at specified index
// scaled for drawing
//---------------------------------------------------------------------------
Ipp64fc CippsDemoDoc::GetScaledValue(int index) const
{
   Ipp64fc value = Get(index);
   value.re *= m_scaleH;
   value.im *= m_scaleH;
   return value;
}

//---------------------------------------------------------------------------
// GetString & SetString gets & sets data for character view
//---------------------------------------------------------------------------
void CippsDemoDoc::GetString(CString& str, int len) const
{
   str = _T("");

   if (len < 0 || len > Length()) len = Length();

#ifdef UNICODE
   Ipp16u* p16u = (Ipp16u*)GetData();
   if (p16u) {
      for (int i=0; i<len; i++) {
         str += p16u[i];
      }
   }
#else
   Ipp8u* p8u = (Ipp8u*)GetData();
   if (p8u) {
      for (int i=0; i<len; i++) {
         str += p8u[i];
      }
   }
#endif
}

void CippsDemoDoc::SetString(const CString& str)
{
#ifdef UNICODE
   Allocate(str.GetLength());
   Ipp16u* p16u = (Ipp16u*)GetData();
   if (p16u) {
      for (int i=0; i<Length(); i++) {
         p16u[i] = str[i];
      }
   }
#else
   Allocate(str.GetLength());
   Ipp8u* p8u = (Ipp8u*)GetData();
   if (p8u) {
      for (int i=0; i<Length(); i++) {
         p8u[i] = str[i];
      }
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Operations

//---------------------------------------------------------------------------
// CreateDemoView creates graphic view
//---------------------------------------------------------------------------
BOOL CippsDemoDoc::CreateDemoView(BOOL bMakeVisible)
{
   if (GetDemoFrame()) return TRUE;
   return CreateNewView(RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CippsDemoView),
                        bMakeVisible);
}

//---------------------------------------------------------------------------
// CreateTextView creates digital view
//---------------------------------------------------------------------------
BOOL CippsDemoDoc::CreateTextView(BOOL bMakeVisible)
{
   if (GetTextFrame()) return TRUE;
   return CreateNewView(RUNTIME_CLASS(CTextFrame), RUNTIME_CLASS(CippsTextView),
                        bMakeVisible);
}

//---------------------------------------------------------------------------
// CreateCharView creates string view
//---------------------------------------------------------------------------
BOOL CippsDemoDoc::CreateCharView(BOOL bMakeVisible)
{
   if (GetCharFrame()) return TRUE;
   return CreateNewView(RUNTIME_CLASS(CCharFrame), RUNTIME_CLASS(CippsCharView),
                        bMakeVisible);
}

//---------------------------------------------------------------------------
// CreateNewView creates new view for document
//---------------------------------------------------------------------------
BOOL CippsDemoDoc::CreateNewView(CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,
                             BOOL bMakeVisible)
{
   CDocTemplate *pTpl = GetDocTemplate();
   CCreateContext context;
   context.m_pCurrentFrame = m_viewList.GetTail() ?
      ((CippsDemoView*)m_viewList.GetTail())->GetParentFrame() : NULL;
   context.m_pCurrentDoc = this;
   context.m_pNewViewClass = pViewClass;
   context.m_pNewDocTemplate = pTpl;

   CFrameWnd* pFrame = (CFrameWnd*)pFrameClass->CreateObject();
   if (!pFrame) return FALSE;

   // create new from resource
   int idr = IDR_IPPSTYPE;
   if (pViewClass->m_lpszClassName == "CippsTextView")
      idr = IDR_IPPSTEXT;
   else if (pViewClass->m_lpszClassName == "CippsCharView")
      idr = IDR_IPPSCHAR;
   if (!pFrame->LoadFrame(idr, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, &context))
      return FALSE;

   pTpl->InitialUpdateFrame(pFrame, this, bMakeVisible);
   UpdateFrameTitle();
   return TRUE;
}

//---------------------------------------------------------------------------
// ZoomStandard scales graph to fit for standard window
//---------------------------------------------------------------------------
void CippsDemoDoc::ZoomStandard()
{
   CRect rect (
      GetDemoFrame()->GetX(),
      GetDemoFrame()->GetY(),
      GetDemoFrame()->GetX() + GetDemoView()->GetInitialWidth()  + GetDemoFrame()->GetBorderWidth(),
      GetDemoFrame()->GetY() + GetDemoView()->GetInitialHeight() + GetDemoFrame()->GetBorderHeight());
   ZoomToRect(&rect);
   ZoomToRect(&rect);
}

//---------------------------------------------------------------------------
// ZoomByFactors scales grapfh by specified factors and accordingly changes
// frame rectangle
//---------------------------------------------------------------------------
void CippsDemoDoc::ZoomByFactors(double scaleW, double scaleH, POINT * pOffset)
{
   if (!GetDemoFrame()) return;
   m_scaleW = scaleW;
   m_scaleH = scaleH;
   if (!pOffset) {
      Zoom();
      return;
   }
   CSize size = GetDemoView()->GetScaleSize();
   size.cx += GetDemoFrame()->GetBorderWidth();
   size.cy += GetDemoFrame()->GetBorderHeight();
   if (pOffset->x < 0) pOffset->x = GetDemoFrame()->GetX();
   if (pOffset->y < 0) pOffset->y = GetDemoFrame()->GetY();
   CRect rect(*pOffset,size);
   Zoom(&rect);
}

//---------------------------------------------------------------------------
// ZoomToRect scales grapfh to fit for specified or actual frame rectangle
//---------------------------------------------------------------------------
void CippsDemoDoc::ZoomToRect(RECT* pRect)
{
   if (!GetDemoFrame()) return;
   CChildFrame* pFrame = GetDemoFrame();
   CippsDemoView* pView = GetDemoView();
   CRect rect;
   if (pRect) rect = *pRect;
   else pFrame->GetRect(rect);
   int width = rect.Width() - pFrame->GetBorderWidth() - pView->GetBorderWidth();
   int height = rect.Height() - pFrame->GetBorderHeight() - pView->GetBorderHeight();
   m_scaleW = (double)width / Length();
   m_scaleH = (double)height / GetDemoView()->GetAmplitude();
   Zoom(&rect);
}

//---------------------------------------------------------------------------
// Zoom implementation
//---------------------------------------------------------------------------
void CippsDemoDoc::Zoom(RECT* pRect)
{
   if (!GetDemoFrame()) return;
   GetDemoFrame()->Zoom(pRect);
   UpdateView();
}

//---------------------------------------------------------------------------
// UpdateXAxis update x axis in graphic view
//---------------------------------------------------------------------------
void CippsDemoDoc::UpdateXAxis()
{
   CippsDemoView* pView = GetDemoView();
   if (!pView) return;
   int amplitude = (int)(pView->GetAmplitude()*FactorH());
   int height = pView->GetClientHeight();
   if (amplitude <= height) {
      CRect rect;
      CChildFrame* pFrame = GetDemoFrame();
      pFrame->GetRect(rect);
      rect.bottom += amplitude + pView->GetBorderHeight()
                  -  height;
      pFrame->SetRect(rect);
   }
   UpdateView();
}

//---------------------------------------------------------------------------
// UpdateYAxis update y axis in graphic view
//---------------------------------------------------------------------------
void CippsDemoDoc::UpdateYAxis()
{
   CippsDemoView* pView = GetDemoView();
   if (!pView) return;
   int length = (int)(Length()*FactorW());
   int width = pView->GetClientWidth();
   if (length <= width) {
      CRect rect;
      CChildFrame* pFrame = GetDemoFrame();
      pFrame->GetRect(rect);
      rect.right += length + pView->GetBorderWidth()
                 -  width;
      pFrame->SetRect(rect);
   }
   UpdateView();
}

/////////////////////////////////////////////////////////////////////////////
// CippsDemoDoc commands

//---------------------------------------------------------------------------
// OnFileSaveAs performs Menu-File-SaveAs command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnFileSaveAs()
{
   CDemoDoc::OnFileSaveAs();

   /*
   CDocTemplate* pTemplate = GetDocTemplate();
   ASSERT(pTemplate != NULL);
   CString fileName = GetPathName();
   if (fileName.IsEmpty())
      fileName = GetTitle() + ".wav";
   if (!AfxGetApp()->DoPromptFileName(fileName,
     AFX_IDS_SAVEFILE,
     OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
      return;
   DoSave(fileName);
   */
}

//---------------------------------------------------------------------------
// OnFileSigned checks Menu-File-Signed command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateFileSigned(CCmdUI* pCmdUI)
{
   BOOL flag = !Float() && Type() != pp64s && !Complex();
   pCmdUI->Enable(flag);
   pCmdUI->SetCheck(Sign());
}

//---------------------------------------------------------------------------
// OnFileSigned performs Menu-File-Signed command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnFileSigned()
{
   DEMO_APP->SetCursorWait();
   if (Sign())
      m_type = (ppType)((int)m_type & ~PP_SIGN);
   else
      m_type = (ppType)((int)m_type | PP_SIGN);
   m_pProcess->ApplyVector(this);
   MarkToPick();
   ZoomToRect();
   ZoomToRect();
   UpdateFrameTitle();
   SetStatusString();
   DEMO_APP->SetCursorArrow();
}


//---------------------------------------------------------------------------
// OnFileProp performs Menu-File-Properties command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnFileProp()
{
   CFilePropDlg dlg;
   dlg.m_pDoc = this;
   dlg.DoModal();
}

//---------------------------------------------------------------------------
// OnUpdateZoom enables all Menu-Zoom- command except of -Lower & -Narrow
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateZoom(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(GetDemoFrame() == ACTIVE_FRAME);
}

//---------------------------------------------------------------------------
// OnUpdateZoomHlow enables Menu-Zoom-Lower command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateZoomHlow(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(
      (GetDemoFrame() == ACTIVE_FRAME) &&
      (Difference()*m_scaleH >= 16));
}

//---------------------------------------------------------------------------
// OnUpdateZoomWlow enables Menu-Zoom-Narrow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateZoomWlow(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(
      (GetDemoFrame() == ACTIVE_FRAME) &&
      (Length()*m_scaleW >= 16));
}

//---------------------------------------------------------------------------
// OnZoomHup performs Menu-Zoom-Heighten command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomHup()
{
   m_scaleH *= 2;
   Zoom();
}

//---------------------------------------------------------------------------
// OnZoomHlow performs Menu-Zoom-Lower command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomHlow()
{
   m_scaleH *= 0.5;
   Zoom();
}

//---------------------------------------------------------------------------
// OnZoomWup performs Menu-Zoom-Widen command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomWup()
{
   m_scaleW *= 2;
   Zoom();
}

//---------------------------------------------------------------------------
// OnZoomWlow performs Menu-Zoom-Narrow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomWlow()
{
   m_scaleW *= 0.5;
   Zoom();
}

//---------------------------------------------------------------------------
// OnZoomFith performs Menu-Zoom-FitForHeight command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomFith()
{
   CRect rect (
      GetDemoFrame()->GetX(),
      0,
      GetDemoFrame()->GetX() + GetDemoFrame()->GetWidth(),
      MAIN_FRAME->GetClientHeight());
   ZoomToRect(&rect);
   ZoomToRect(&rect);
}

//---------------------------------------------------------------------------
// OnZoomFitw performs Menu-Zoom-FitForWidth command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomFitw()
{
   CRect rect (
      0,
      GetDemoFrame()->GetY(),
      MAIN_FRAME->GetClientWidth(),
      GetDemoFrame()->GetY() + GetDemoFrame()->GetHeight());
   ZoomToRect(&rect);
   ZoomToRect(&rect);
}

//---------------------------------------------------------------------------
// OnZoomStandard performs Menu-Zoom-FitForStandard command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomStandard()
{
   ZoomStandard();
}

//---------------------------------------------------------------------------
// OnZoomCurrent performs Menu-Zoom-FitForCurrent command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomCurrent()
{
   ZoomToRect();
   ZoomToRect();
}

//---------------------------------------------------------------------------
// OnZoomCustom performs Menu-Zoom-CustomZoom command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnZoomCustom()
{
   CZoomDlg dlg;
   dlg.m_pDoc = this;
   dlg.m_ScaleW = m_scaleW;
   dlg.m_ScaleH = m_scaleH;

   if (dlg.DoModal() != IDOK) return;

   m_scaleW = dlg.m_ScaleW;
   m_scaleH = dlg.m_ScaleH;

   Zoom();
}

//---------------------------------------------------------------------------
// OnUpdateWindowPlot enables&checks Menu-Window-PlotWindow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateWindowPlot(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(TRUE);
   pCmdUI->SetCheck(GetDemoFrame() == ACTIVE_FRAME);
}

//---------------------------------------------------------------------------
// OnUpdateWindowText enables&checks Menu-Window-DigitalWindow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateWindowText(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(TRUE);
   pCmdUI->SetCheck(GetTextFrame() == ACTIVE_FRAME);
}

//---------------------------------------------------------------------------
// OnUpdateWindowChar enables&checks Menu-Window-StringWindow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnUpdateWindowChar(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(GetVector()->Type() == pp8u);
   pCmdUI->SetCheck(GetCharFrame() == ACTIVE_FRAME);
}

//---------------------------------------------------------------------------
// OnWindowPlot performs Menu-Window-PlotWindow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnWindowPlot()
{
   if (GetDemoFrame())
      MAIN_FRAME->MDIActivate(GetDemoFrame());
   else {
      DEMO_APP->SetCursorWait();
      CreateDemoView();
      DEMO_APP->SetCursorArrow();
   }
}

//---------------------------------------------------------------------------
// OnWindowText performs Menu-Window-DigitalWindow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnWindowText()
{
   if (GetTextFrame())
      MAIN_FRAME->MDIActivate(GetTextFrame());
   else {
      DEMO_APP->SetCursorWait();
      CreateTextView();
      DEMO_APP->SetCursorArrow();
   }
}

//---------------------------------------------------------------------------
// OnWindowChar performs Menu-Window-StringWindow command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnWindowChar()
{
   if (GetCharFrame())
      MAIN_FRAME->MDIActivate(GetCharFrame());
   else {
      DEMO_APP->SetCursorWait();
      CreateCharView();
      DEMO_APP->SetCursorArrow();
   }
}

//---------------------------------------------------------------------------
// OnEditInsert performs Menu-Edit-Insert command
//---------------------------------------------------------------------------
void CippsDemoDoc::OnEditInsert()
{
   CStringDlg dlg;

   GetString(dlg.m_String);
   GetCharView()->GetEditCtrl().GetSel(dlg.m_start, dlg.m_end);
   if (dlg.DoModal() != IDOK) return;
   SetString(dlg.m_String);
   UpdateView();
   GetCharView()->GetEditCtrl().SetSel(dlg.m_start, dlg.m_end);
}

void CippsDemoDoc::OnUpdateEditInsert(CCmdUI *pCmdUI)
{
   pCmdUI->Enable(GetCharFrame() == ACTIVE_FRAME);
}
